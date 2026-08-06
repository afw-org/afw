#!/usr/bin/env python3
"""
YAML raw_to_object / conf parse / file-adapter regression.

Why Python (not .as)?
  YAML has no Adaptive Script converter; file adapters with contentType yaml
  need a conf that loads extension afw_yaml. Spawning `afw` is the natural
  surface for content-type input + adapter get/mutate.

Coverage:
  - conf -t yaml with nested object and array entries
  - file adapter contentType yaml: get_object, property read, path meta
  - issue #17: get_object face isolation (mutate face, second get clean)
  - add_object + get round-trip through YAML encode/decode
  - non-object YAML root rejected for adapter object load
"""

from __future__ import annotations

import os
import subprocess
import tempfile
import textwrap


def _case(name, description, passed, detail=None, skip=False):
    t = {
        "test": name,
        "description": description,
        "passed": bool(passed),
        "skip": bool(skip),
    }
    if detail and not passed:
        t["error"] = detail
    return t


def _run_afw(args, cwd=None, timeout=30):
    r = subprocess.run(
        ["afw", *args],
        capture_output=True,
        text=True,
        timeout=timeout,
        cwd=cwd,
    )
    return r.returncode, r.stdout, r.stderr


def _write(path, content):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8") as fd:
        fd.write(content)


def run():
    description = (
        "YAML to_object / conf nested parse / file adapter faces (beta hygiene)"
    )
    tests = []

    # --- conf: nested structure via adapter entry (raw_to_value) ---------
    # Application conf is strict; use a file adapter conf with nested-looking
    # properties only where valid. Nested mappings/sequences are covered by
    # file-adapter get of seed.yaml below.
    with tempfile.TemporaryDirectory(prefix="afw_yaml_conf_nested_") as td:
        conf_path = os.path.join(td, "nested.yaml")
        objects = os.path.join(td, "objects")
        os.makedirs(objects, exist_ok=True)
        _write(
            conf_path,
            textwrap.dedent(
                """\
                # YAML conf with nested-ish list of adapter conf objects
                - type: adapter
                  adapterType: file
                  adapterId: confyaml
                  root: objects/
                  filenameSuffix: .yaml
                  contentType: yaml
                """
            ),
        )
        code, out, err = _run_afw(
            [
                "-e",
                "afw_yaml",
                "-t",
                "yaml",
                "-f",
                conf_path,
                "-s",
                "expression",
                "-x",
                "integer(1)",
            ],
            cwd=td,
        )
        tests.append(
            _case(
                "conf_type_yaml_adapter_entry",
                "content type input: -t yaml loads file adapter conf (contentType yaml)",
                code == 0,
                detail="exit=%s stderr=%r" % (code, err[-400:]),
            )
        )

    # --- file adapter contentType yaml ----------------------------------
    with tempfile.TemporaryDirectory(prefix="afw_yaml_file_") as td:
        objects = os.path.join(td, "objects")
        ot_dir = os.path.join(objects, "YamlOt")
        os.makedirs(ot_dir, exist_ok=True)

        # Seed object as YAML (raw_to_object path)
        _write(
            os.path.join(ot_dir, "seed.yaml"),
            textwrap.dedent(
                """\
                name: seed
                count: 3
                nested:
                  inner: ok
                tags:
                  - one
                  - two
                """
            ),
        )

        conf_path = os.path.join(td, "afw.conf")
        # conf is Adaptive relaxed JSON (default -t). Load extension with -e;
        # only the file adapter contentType is yaml (raw_to_object path).
        _write(
            conf_path,
            textwrap.dedent(
                """\
                [
                    {
                        type: "adapter",
                        adapterType: "file",
                        adapterId: "yamlfile",
                        root: "objects/",
                        filenameSuffix: ".yaml",
                        contentType: "yaml"
                    }
                ]
                """
            ),
        )

        def _yaml_script(script):
            return _run_afw(
                [
                    "-e",
                    "afw_yaml",
                    "-f",
                    conf_path,
                    "-s",
                    "script",
                    "-x",
                    textwrap.dedent(script),
                ],
                cwd=td,
            )

        # get_object + nested props + meta path (object options like file tests)
        code, out, err = _yaml_script(
            """\
            let o = get_object("yamlfile", "YamlOt", "seed",
                { path: true, objectId: true, objectType: true });
            assert(o.name === "seed", "name");
            assert(o.nested.inner === "ok", "nested");
            assert(meta(o).path === anyURI("/yamlfile/YamlOt/seed"), "path");
            assert(meta(o).objectId === "seed", "objectId");
            assert(meta(o).objectType === "YamlOt", "objectType");
            return 0;
            """
        )
        body = out.strip()
        tests.append(
            _case(
                "file_adapter_yaml_get_object",
                "file adapter contentType yaml: get_object + nested + meta ids",
                code == 0 and body == "0",
                detail="exit=%s body=%r stderr=%r" % (code, body, err[-500:]),
            )
        )

        # issue #17 face: mutate face must not poison next get
        code, out, err = _yaml_script(
            """\
            let a = get_object("yamlfile", "YamlOt", "seed");
            assert(a.name === "seed");
            a.name = "mutated on face";
            assert(a.name === "mutated on face");
            let b = get_object("yamlfile", "YamlOt", "seed");
            assert(b.name === "seed",
                "second get must not see face mutation");
            return 0;
            """
        )
        body = out.strip()
        tests.append(
            _case(
                "file_adapter_yaml_get_object_mutable_face",
                "issue #17: YAML file get_object returns face; second get clean",
                code == 0 and body == "0",
                detail="exit=%s body=%r stderr=%r" % (code, body, err[-500:]),
            )
        )

        # add_object then get (encode + decode round-trip)
        code, out, err = _yaml_script(
            """\
            let result = add_object("yamlfile", "YamlOt",
                { name: "added", n: 7 }, "added1");
            assert(result.objectId === "added1");
            let o = get_object("yamlfile", "YamlOt", "added1");
            assert(o.name === "added");
            return 0;
            """
        )
        body = out.strip()
        added_path = os.path.join(ot_dir, "added1.yaml")
        tests.append(
            _case(
                "file_adapter_yaml_add_get_roundtrip",
                "file adapter contentType yaml: add_object then get_object",
                code == 0 and body == "0" and os.path.isfile(added_path),
                detail="exit=%s body=%r exists=%s stderr=%r"
                % (code, body, os.path.isfile(added_path), err[-500:]),
            )
        )

        # non-object root must fail raw_to_object (array file)
        _write(os.path.join(ot_dir, "badarray.yaml"), "- only\n- a\n- list\n")
        code, out, err = _yaml_script(
            """\
            assert(
                safe_evaluate(
                    get_object("yamlfile", "YamlOt", "badarray"),
                    "error"
                ) == "error",
                "array root must not load as object"
            );
            return 0;
            """
        )
        body = out.strip()
        tests.append(
            _case(
                "file_adapter_yaml_reject_array_root",
                "YAML raw_to_object rejects non-mapping root",
                code == 0 and body == "0",
                detail="exit=%s body=%r stderr=%r" % (code, body, err[-500:]),
            )
        )

    return {"description": description, "tests": tests}
