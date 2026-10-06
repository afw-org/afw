#! /usr/bin/env python3
##
# @file baseline.py
# @brief Baselines, what a run compares against, and history retention.
#
# --baseline marks a run as this branch's baseline for its env mode: a
# tagged history file labelled baseline-<branch>. A later --baseline on
# the same branch and mode replaces it. The PR gate runs use it.
#
# By default a run compares against the newest baseline of its mode whose
# commit is an ancestor of HEAD, skipping this branch's own. With merge
# commits that is the previous PR's gate run: "since this branch
# started". --compare-to picks something else: last, a commit, a tag, or
# a history file.
#
# Retention, at the start of a run, for this mode:
#   - untagged runs whose commit is an ancestor of that baseline's commit
#     (work that has landed where this branch started) are removed, except
#     dirty runs at that very commit (new work on a branch with no commits);
#   - untagged runs older than test_orphan_history_days (30) are removed;
#   - only the newest test_keep_runs_per_commit (20) untagged runs of
#     one commit are kept (an overnight loop on one branch);
#   - only the newest test_keep_baselines (10) baselines are kept.
# Other tagged runs (older -ref- labels) are never removed.
#

import os
import re
import subprocess
from datetime import datetime, timedelta, timezone

from _afwdev.test import history

BASELINE_PREFIX = "baseline-"
KEEP_BASELINES = 10
KEEP_RUNS_PER_COMMIT = 20
ORPHAN_HISTORY_DAYS = 30

_COMMIT_RE = re.compile(r'"commit":\s*"([0-9a-f]+)"')
_COMMIT_FULL_RE = re.compile(r'"commit_full":\s*"([0-9a-f]+)"')
_STAMP_RE = re.compile(r"^(\d{4}-\d{2}-\d{2}T\d{6})(\d{3})Z-")


def _setting(options, key, default):
    s = (options or {}).get("afwdev_settings") or {}
    try:
        n = int(s.get(key))
    except (TypeError, ValueError):
        return default
    return n if n >= 1 else default


def _git(args):
    try:
        return subprocess.run(
            ["git"] + args, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL,
            text=True, check=False)
    except OSError:
        return None


def is_ancestor(commit, of="HEAD"):
    if not commit:
        return False
    r = _git(["merge-base", "--is-ancestor", commit, of])
    return bool(r) and r.returncode == 0


def label_for_branch(branch):
    return history.sanitize_ref_label(BASELINE_PREFIX + (branch or "detached"))


def is_baseline_path(path, mode):
    label = history.ref_label_from_name(path, mode)
    return bool(label) and label.startswith(BASELINE_PREFIX)


def file_commit(path):
    """(full or None, short or None) from a history file, without parsing.

    History is pretty-printed JSON with sorted keys; "commit" and
    "commit_full" only appear in its git object.
    """
    try:
        with open(path, "r", encoding="utf-8", errors="replace") as fd:
            text = fd.read()
    except OSError:
        return None, None
    full = _COMMIT_FULL_RE.search(text)
    short = _COMMIT_RE.search(text)
    return (full.group(1) if full else None,
            short.group(1) if short else None)


def file_time(path):
    m = _STAMP_RE.match(os.path.basename(path))
    if not m:
        return None
    try:
        return datetime.strptime(m.group(1), "%Y-%m-%dT%H%M%S").replace(
            tzinfo=timezone.utc)
    except ValueError:
        return None


def baselines(options):
    """Baseline paths for this mode, newest first."""
    dir_path = history.history_dir(options)
    mode = history.env_mode(options)
    found = [p for p in history.list_run_files(dir_path, mode)
             if is_baseline_path(p, mode)]
    return sorted(found, key=os.path.basename, reverse=True)


def select_baseline(options, branch=None):
    """Newest baseline from another branch that HEAD contains, or None."""
    own = label_for_branch(
        branch if branch is not None else history.git_meta().get("branch"))
    mode = history.env_mode(options)
    for path in baselines(options):
        if history.ref_label_from_name(path, mode) == own:
            continue
        full, short = file_commit(path)
        if is_ancestor(full or short):
            return path
    return None


def _untagged(options):
    dir_path = history.history_dir(options)
    mode = history.env_mode(options)
    return [p for p in history.list_run_files(dir_path, mode)
            if not history.is_reference_name(p)]


def _narrowed_file(path):
    try:
        with open(path, "r", encoding="utf-8", errors="replace") as fd:
            return '"narrowed": true' in fd.read()
    except OSError:
        return False


def last_run(options, whole_suite=False):
    """The previous run of this mode, tagged or not.

    whole_suite skips runs narrowed by --test-pattern and the like, for
    the default comparison when there is no baseline.
    """
    dir_path = history.history_dir(options)
    mode = history.env_mode(options)
    for path in reversed(history.list_run_files(dir_path, mode)):
        if whole_suite and _narrowed_file(path):
            continue
        return path
    return None


def _label(path, prefix):
    full, short = file_commit(path)
    return "{} {}".format(prefix, short or (full or "?")[:8])


def resolve(options):
    """(path or None, label) for what this run compares against."""
    ref = (options or {}).get("compare_to") or "baseline"
    mode = history.env_mode(options)
    if ref == "baseline":
        path = select_baseline(options)
        if path:
            return path, "{} ({})".format(
                _label(path, "baseline"),
                history.ref_label_from_name(path, mode)[len(BASELINE_PREFIX):])
        path = last_run(options, whole_suite=True)
        if path:
            return path, _label(path, "last run") + " (no baseline found)"
        return None, "nothing (no history yet)"
    if ref == "last":
        path = last_run(options)
        return (path, _label(path, "last run")) if path else (
            None, "nothing (no history yet)")
    expanded = os.path.expanduser(ref)
    if os.path.isfile(expanded):
        return expanded, os.path.basename(expanded)
    dir_path = history.history_dir(options)
    files = history.list_run_files(dir_path, mode)
    label = history.sanitize_ref_label(ref)
    tagged = [p for p in files
              if history.ref_label_from_name(p, mode) == label]
    if tagged:
        return tagged[-1], "tag " + label
    if re.fullmatch(r"[0-9a-fA-F]{4,40}", ref):
        want = ref.lower()
        hits = []
        for p in files:
            full, short = file_commit(p)
            if (full and full.startswith(want)) or (
                    short and (short.startswith(want) or want.startswith(short))):
                hits.append(p)
        if hits:
            clean = [p for p in hits if not _dirty(p)]
            path = (clean or hits)[-1]
            return path, _label(path, "commit")
    raise ValueError(
        "--compare-to {r}: no history run, tag, or commit for mode {m} in "
        "{d}".format(r=ref, m=mode, d=dir_path))


def _dirty(path):
    try:
        with open(path, "r", encoding="utf-8", errors="replace") as fd:
            return '"dirty": true' in fd.read()
    except OSError:
        return False


def _remove(path):
    try:
        os.remove(path)
        return 1
    except OSError:
        return 0


def _rev_list(commit):
    r = _git(["rev-list", commit])
    if not r or r.returncode != 0:
        return set()
    return set(r.stdout.split())


def prune(options, baseline_path=None, now=None):
    """Retention at the start of a run. Returns the number removed."""
    mode = history.env_mode(options)
    dir_path = history.history_dir(options)
    removed = 0
    now = now or datetime.now(timezone.utc)

    landed = set()
    base_full = None
    if baseline_path:
        full, short = file_commit(baseline_path)
        landed = _rev_list(full or short)
        r = _git(["rev-parse", full or short])
        base_full = r.stdout.strip() if r and r.returncode == 0 else None
    by_len = {}

    def is_landed(full, short):
        if full:
            return full in landed
        if not short:
            return False
        if len(short) not in by_len:
            by_len[len(short)] = {h[:len(short)] for h in landed}
        return short in by_len[len(short)]

    orphan = timedelta(days=_setting(
        options, "test_orphan_history_days", ORPHAN_HISTORY_DAYS))
    keep_per_commit = _setting(
        options, "test_keep_runs_per_commit", KEEP_RUNS_PER_COMMIT)
    per_commit = {}
    for path in reversed(_untagged(options)):
        full, short = file_commit(path)
        when = file_time(path)
        # A dirty run at the baseline's own commit is new work on top
        # of it (a branch with no commits yet), not work that landed.
        at_base = bool(base_full) and (
            full == base_full or (not full and short and
                                  base_full.startswith(short)))
        if landed and is_landed(full, short) and not (
                at_base and _dirty(path)):
            removed += _remove(path)
            continue
        if when and now - when > orphan:
            removed += _remove(path)
            continue
        key = full or short or "?"
        per_commit[key] = per_commit.get(key, 0) + 1
        if per_commit[key] > keep_per_commit:
            removed += _remove(path)

    keep_baselines = _setting(options, "test_keep_baselines", KEEP_BASELINES)
    for path in baselines(options)[keep_baselines:]:
        removed += _remove(path)
    if dir_path and os.path.isdir(dir_path):
        removed += history._drop_dangling_latest(dir_path, mode)
    return removed


def replace_own(options, written_path):
    """After a --baseline write, remove this branch's older ones."""
    mode = history.env_mode(options)
    label = history.ref_label_from_name(written_path, mode)
    removed = 0
    for path in baselines(options):
        if path == written_path:
            continue
        if history.ref_label_from_name(path, mode) == label:
            removed += _remove(path)
    return removed
