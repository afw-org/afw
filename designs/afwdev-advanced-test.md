# afwdev advanced tests (marker leaves)

**Audience:** maintainers / assistants. **Not** handbook.  
**Status:** **\*\*\* Experimental \*\*\*** — implemented and usable for comment; **not** a frozen green contract.  
**GitHub issue:** [#157](https://github.com/afw-org/afw/issues/157).  
**Branch:** `feature-afwfcgi-scenario-tests` (off `mgg-develop`).  
**Related:** [#2 Memory management](https://github.com/afw-org/afw/issues/2) ([`memory-management.md`](memory-management.md)), [#149 Runtime catalog lifetime](https://github.com/afw-org/afw/issues/149) ([`runtime-catalog-lifetime.md`](runtime-catalog-lifetime.md)).  
**Depends on:** installed `afwfcgi` / `afw` via `./afwdev build … --install` (e.g. `--cdev` / `--fulldev`).

**Rename / rethink (2026-08):** Product direction is shifting toward **orchestrated tests** with an **`orchestration.yaml` / `.json`** control file (host + **feed** + include list + schedule), not “advanced” as a second Adaptive dialect. **North-star schema + pretend scenario leaves** (not wired to the runner): [`src/afw/tests-extra/`](../src/afw/tests-extra/). Live runner still uses `advanced-test.yaml` until migration.

---

## \*\*\* Experimental \*\*\*

This feature is **out for comment and real use**, not a promise of long-term stability of names, schema, or behavior.

| Expect | Do not expect (yet) |
|--------|---------------------|
| Useful hermetic multi-request / `afwfcgi` regression leaves today | Marker name, YAML fields, or runner semantics frozen forever |
| Many early decisions to **stick** (leaf discovery, `host`, FCGI client default, env-mode split) | No breaking tweaks over the next few months |
| Growth driven by **#149**, **#2**, and whoever tries it | “Green” handbook-grade API and support story |

**Feedback welcome** on issue **#157** and PRs. If you invest in many leaves, prefer the *ideas* (hermetic server, multi-step, fixture dir) over coupling to every field name—we may reshape surfaces as we learn.

When the shape settles, drop the experimental banner and promote invariants into afwdev help / developer notes.

---

## Product statement

> **Advanced tests** are multi-file, harness-driven tests discovered by a **marker file** in a **leaf directory**.  
> Under default `afwdev test -j` (`--env-mode afw`), an `afwfcgi` host leaf **starts the installed `afwfcgi`**, drives it with a harness **FastCGI client**, runs ordered **steps**, then **tears down**.  
> They exercise **process lifetime, conf, adapters, and multi-request** behavior that one-shot `afw` CLI scripts cannot.  
> They fit Jeremy’s test model: **env-mode** chooses the *environment*; the **marker** is a new *test kind* (like `.py` / `commands_*.txt`), not merely another way to run `.as` files.

---

## Why

| Gap today | Advanced tests |
|-----------|----------------|
| `.as` under `afw` | One process per script; weak multi-request / server lifetime |
| `--env-mode afwfcgi` | Replays `.as` against **already-up** stack; skips custom `afw.conf` |
| Hand `.py` + `Session("local")` | Flexible but bespoke; not a shared conf+steps fixture pattern |

**Primary consumers after merge:**

1. **#149** — multi-request runtime catalog / registry behavior on a long-lived server process.  
2. **#2** (umbrella) — long-running value/pool/escape regressions that need repeated eval in one process, optional short concurrency later.  
3. Conf / extension / service lifecycle that only makes sense with a real `afw_server` host.

**Branch policy:** implement harness on `feature-afwfcgi-scenario-tests` → merge to `mgg-develop` → **rebase** `issue-#149-runtime-catalog-lifetime` and add catalog scenarios there (no afwdev runner work on #149).

### Handoff / sibling sessions (2026-08)

- This work was **intentionally split** from the **#149 runtime catalog** Grok session so that branch’s context (live afwfcgi poking, env/registry mental model) stayed unpolluted.
- **#149 session** owns product/reliability for catalogs/accessors; **this branch** owns experimental **afwdev test advanced-test** + **`afwdev blast`**.
- After PR merges: on `issue-#149-runtime-catalog-lifetime`, rebase onto updated `mgg-develop`, then add **purpose-built advanced-test leaves** for multi-request catalog behavior; use **`afwdev blast`** for load/crash hunts (not as the #149 correctness suite).
- On-demand load design pad: [`afwdev-blast.md`](afwdev-blast.md). afwfcgi SIGTERM/SIGINT graceful stop: **#158** (done).

---

## Jeremy’s model (do not break)

From `src/afw_dev/_afwdev/test/test.py` and `common.run_test` dispatch:

| Concept | Meaning | Today |
|---------|---------|--------|
| **`--env-mode`** | Which **environment** runs compatible tests | `afw`, `valgrind`, `afwfcgi`, `actions` |
| **Artifact → mode** | Path forces runner; ignores env-mode | `.py` → `python`, `commands_*.txt` → `commands` |
| **`afwfcgi` env-mode** | Live HTTP stack (`http://localhost:8080/afw`); no spawn; skip custom conf for `.as` | `modes/afwfcgi.py` |

**Advanced tests** join the **artifact** family: marker present → scenario runner.  
Env-mode still **modulates** how that runner attaches (hermetic vs live vs valgrind).

---

## Decisions (frozen)

### 1. Marker basename and formats

| | |
|--|--|
| **Stem** | `advanced-test` |
| **Files** | `advanced-test.yaml` and/or `advanced-test.json` |
| **Both present** | **Error** (ambiguous) |
| **Parser** | YAML via **PyYAML `safe_load`**; JSON via stdlib |
| **Dependency** | **Require PyYAML** in project Python requirements (do not rely on ambient install alone) |

Prefer **YAML** in examples and smoke leaves (comments, multi-line scripts). JSON is the same object model for authors who want it.

Harness parses markers in **Python**. Do **not** use Adaptive content types / `afw_yaml` to read the marker (different layer). Fixture *data* may still be YAML/JSON Adaptive objects once the server is up.

### 2. Discriminant: `host`

```yaml
host: afwfcgi    # required in v1
```

| Rule | Choice |
|------|--------|
| Field name | **`host`** |
| v1 allowed value | **`afwfcgi` only** |
| Missing `host` | **Error** (self-describing leaves; no silent default) |
| Unknown `host` | **Error** |
| Future | `afw-local` (and others) with their own fixture contracts — same marker name |

Optional host-specific block (ignored keys under wrong host = error or warn; v1: only read `afwfcgi:` when `host: afwfcgi`):

```yaml
host: afwfcgi
afwfcgi:
  threads: 1          # maps to afwfcgi -n; default 1
```

### 3. Env-mode matrix (marker leaves)

| `--env-mode` | Marker leaf behavior |
|--------------|----------------------|
| **`afw`** (default) | **Run hermetic:** spawn installed `afwfcgi` + **FCGI client** + leaf conf |
| **`afwfcgi`** | **Same policy as today’s mode for custom stacks:** do **not** spawn; use live base URL **`http://localhost:8080/afw`** (whatever `modes/afwfcgi.py` uses **now**). If that mode later gains `--server-url` or similar, markers follow it. Leaves that need a **private** conf/process are **skipped** (debug reason), analogous to skipping `.as` with local `afw.conf`. |
| **`valgrind`** | **Prefer implement if not too hard:** spawn `valgrind` + same suppressions style as CLI valgrind mode + **`afwfcgi`**, traffic via FCGI client; treat valgrind XML/`<error>` as leaf failure. **Hold off in first PR** if it delays the smoke vertical—but keep the hook obvious (`hosts/afwfcgi.py` spawn argv builder) so #2 can rely on it soon. Until then **skip** markers under valgrind with a clear debug reason. |
| **`actions`** | **Skip** markers (local Session is not this kind). |
| **Forced** `python` / `commands` | Unchanged; markers never go through those paths. |

Ordinary `.as` / `.py` behavior **unchanged**.

### 4. Live URL / nginx

- **Not** a per-leaf `transport: nginx` for normal commit runs.  
- Live front door = **`--env-mode afwfcgi`** only for now (hardcoded URL as today).  
- Custom FCGI client is the **default** path for `afwdev test -j`.  
- Direct FCGI client replaces **nginx as FastCGI peer only**; **`afwfcgi` + `afw.conf` are the same** either way.  
- `request::` fidelity vs real nginx params is a **live / later** concern, not v1 blocker.

### 5. Discovery: leaf semantics

```text
os.walk(tests/)
  skip environments/, _*
  if dir contains advanced-test.yaml|json:
    → exactly ONE test (the marker path)
    → do NOT discover .as/.py/commands as separate tests inside this tree
    → do NOT walk children for more test groups / markers
    → nested advanced-test under a leaf → ERROR
  else:
    → existing discovery
```

Parents may nest freely for organization (`tests/runtime/catalog/multi-request/advanced-test.yaml`).

### 6. Fixture contract (`host: afwfcgi`)

| Path | Role |
|------|------|
| **`advanced-test.yaml`** (or `.json`) | Instructions (required) |
| **`afw.conf`** | Required — server conf (`-f`) |
| **`config/`**, **`objects/`**, other files | Optional assets; copied into work dir like today’s group copy |
| Sibling `*.as` | Optional **step scripts**, not independent tests |

Work dir: same pattern as current test environments (copy leaf → temp work dir under runner prefix).  
Socket: **Unix domain socket under work dir** (parallel-safe for `-j`).  
Binary: **`afwfcgi` on PATH** from install (`./afwdev build` with `--install`).

### 7. Schema (simple, powerful enough)

Inspired by common integration styles (ordered steps, fixture dir = env, one scenario = one result): **setup is the directory**, **behavior is an ordered list of steps**, **teardown is harness-owned**.

#### Minimal v1 document

```yaml
# advanced-test.yaml
host: afwfcgi
description: Short summary for --list / failure headers

# optional
timeout_s: 120              # whole leaf; harness default if omitted (e.g. 60)

afwfcgi:
  threads: 1                # optional; default 1

steps:
  - name: smoke eval
    eval: |
      assert(true);
      return true;

  - name: script file
    script: step_catalog.as   # relative to leaf; often #! … --syntax test_script
```

#### Step kinds (v1)

| Field | Meaning |
|-------|---------|
| **`name`** | Required. Used in failure output. |
| **`eval`** | Inline Adaptive Script source (mutually exclusive with `script`) |
| **`script`** | Path relative to leaf to a `.as` file (mutually exclusive with `eval`) |

Exactly one of `eval` / `script` per step in v1.

#### Success rules (v1)

| Situation | Pass step? |
|-----------|------------|
| FCGI/server error, timeout, non-success transport | **Fail** leaf |
| `eval` / non–test_script script: completed without thrown error | **Pass** step |
| `script` with `--syntax test_script`: parse result like CLI mode; **any** embedded test `passed: false` → **Fail** leaf | |
| First failing step | **Stop** (fail-fast); remaining steps not run |

#### Result grain

- **One leaf → one pass/fail** in suite counts / default summary.  
- Failure detail **must** name the step and include useful server/harness stderr.  
- Per-step sub-results in the JSON summary are **optional later** if useful; not required for v1.

#### Explicitly deferred in schema

- `expect:` deep matchers / JSONPath  
- Variables passed between steps  
- `parallel` / `stress` step kinds  
- HTTP method/path REST steps (can add when needed; eval covers most Adaptive API checks)  
- `before` / `after` hooks beyond process lifecycle  

Grow syntax when a real scenario demands it (#149 / #2 will teach us).

### 8. Process lifecycle

1. Allocate work dir; copy leaf.  
2. Start: `afwfcgi -f afw.conf -p <work_dir>/afw.sock -n <threads>` (plus any fixed harness needs).  
3. Health: connect/accept readiness with timeout.  
4. Run steps in order via **FCGI client** (params sufficient for director / eval path; document minimal param set in implementation).  
5. Always teardown: signal process group, wait, force kill, remove socket.  
6. Missing `afwfcgi` on PATH → **fail** with install hint (do not silent-skip on default mode).

### 9. FCGI client

- **In-tree** thin client (stdlib sockets + FastCGI records) preferred over abandoned deps; optional tiny vendor only if justified.  
- Sets FCGI params + stdin; reads stdout to completion.  
- Concurrent clients **later** (stress); v1 sequential steps are enough.

#### Default FCGI params (nginx reference parity)

Mirror the **reference** FastCGI front door used with afwfcgi: dev **`/afw/nginx.conf`**, docker **`docker/images/afw-admin/nginx.conf`** / **`docker/images/afw/etc/nginx/nginx.conf`**. The client is not nginx, but **default params should match what we pass today** so `request::` / request-property behavior is close for normal steps. Steps may **override** individual params later if needed.

| Param | Hermetic default (sensible stand-in) | nginx source |
|-------|--------------------------------------|--------------|
| `IGNORE_URI_PREFIX` | `/` | fixed in conf |
| `URI` | request path (e.g. eval/perform path the harness uses) | `$uri` |
| `QUERY_STRING` | `""` or step query | `$query_string` |
| `REQUEST_METHOD` | `POST` for eval/body steps; overridable | `$request_method` |
| `CONTENT_TYPE` | as needed for body (e.g. JSON) | `$content_type` |
| `CONTENT_LENGTH` | len(body) | `$content_length` |
| `SCRIPT_FILENAME` | `""` or placeholder | `$document_root$fastcgi_script_name` |
| `SCRIPT_NAME` | `""` | `$fastcgi_script_name` |
| `PATH_INFO` | `""` | `$fastcgi_path_info` |
| `PATH_TRANSLATED` | `""` | `$document_root$fastcgi_path_info` |
| `REQUEST_URI` | same as path + optional query | `$request_uri` |
| `DOCUMENT_URI` | path | `$document_uri` |
| `DOCUMENT_ROOT` | work dir or `""` | `$document_root` |
| `SERVER_PROTOCOL` | `HTTP/1.1` | `$server_protocol` |
| `GATEWAY_INTERFACE` | `CGI/1.1` | fixed |
| `SERVER_SOFTWARE` | `afwdev-fcgi-client/1` (not `nginx/…`) | `$nginx_version` in real nginx |
| `REMOTE_ADDR` | `127.0.0.1` | `$remote_addr` |
| `REMOTE_PORT` | `0` or ephemeral stand-in | `$remote_port` |
| `SERVER_ADDR` | `127.0.0.1` | `$server_addr` |
| `SERVER_PORT` | `0` or `8080` stand-in | `$server_port` |
| `SERVER_NAME` | `localhost` | `$server_name` |
| `HTTPS` | `""` | `$https` |
| `SCHEME` | `http` | `$scheme` |
| `TIME_ISO8601` | harness UTC now ISO-8601 | `$time_iso8601` |
| `REDIRECT_STATUS` | `200` | fixed in conf |

**HTTP_*** headers:** set when a step needs them (e.g. `HTTP_ACCEPT`, `HTTP_CONTENT_TYPE` if used); not all required for smoke eval. Live `--env-mode afwfcgi` still goes through real nginx and real header mapping.

Implementation: one shared dict builder `default_fcgi_params(path, method, body, overrides=)` used by every step.

### 10. v1 implementation cut line

| In first useful merge | Later |
|-----------------------|--------|
| Discovery + leaf prune + list | `host: afw-local` |
| YAML + JSON load; require PyYAML | Rich `expect` language |
| `host: afwfcgi` + fixture checks | **Stress / parallel steps** (critical for **#2**) |
| Spawn/stop installed `afwfcgi`, Unix socket | **Observability** under load (#2) — see below |
| FCGI client + nginx-parity default params | Param override per step (if not free with overrides dict) |
| Sequential `eval` / `script` steps | `--server-url` (unless env-mode gains it first) |
| Env-mode matrix above | Full nginx integration as harness-spawned front door |
| **Valgrind-on-afwfcgi if low cost**; else skip + hook | Valgrind if deferred |
| At least **one smoke leaf** under `src/afw/tests/` | #149 / #2 real scenarios (on those branches after rebase) |
| Default `afwdev test -j` runs markers | Heavy soak profiles / tags |

**As much as makes sense in one feature branch:** prefer a **complete thin vertical** (discover → run smoke → teardown) over half of stress/valgrind/local.

### 11. Suite integration

- Part of **normal** `afwdev test` / `afwdev test -j` — no extra flag to enable.  
- `--test-pattern` matches marker path or leaf dir name.  
- `--tags` via optional sibling `config.py` **or** optional `tags:` in marker later; v1 can use `config.py` Tags if already loaded for that dir—or skip tags until needed.  
- Parallel `-j`: one marker leaf = one group unit (one server process), same as today’s group isolation.

### 12. Documentation

| Doc | Role |
|-----|------|
| **This pad** | Design + decisions (experimental) |
| **Smoke / showcase leaves** | Living examples under `src/afw/tests/advanced/` |
| **`src/afw/doc/developer/writing-tests.md`** | Builder-facing how-to (test_script first; advanced-test section experimental) |
| **`designs/README.md`** | Index entry |

---

## Example smoke leaf (illustrative)

```text
src/afw/tests/advanced/smoke/
  advanced-test.yaml
  afw.conf
```

```yaml
host: afwfcgi
description: Spawn afwfcgi and eval a trivial script

steps:
  - name: return true
    eval: |
      return true;
```

`afw.conf` must define enough `requestHandler` / application surface for the harness’s eval (or perform) path—mirror a **minimal** slice of `/afw` conf, not the full dev app.

---

## Future use: #149 and #2

These are **why** the harness exists; scenarios land **after** harness merges (and #149 rebases). Design the runner so #2 stress and observation are **extensions**, not a rewrite.

### #149 — runtime catalog lifetime

Ideas for leaves (not implemented on the harness branch):

- Multiple sequential evals/gets against `/afw/_AdaptiveEnvironmentRegistry_/…` or catalog retrieves in **one** process.  
- Compare “first request vs Nth request” correctness (and later memory signals).  
- Leaf-local conf + minimal adapters — not the full live `/afw` admin app.

See [`runtime-catalog-lifetime.md`](runtime-catalog-lifetime.md).

### #2 — memory management (umbrella) — stress + observe

**Stress will be a large part of #2 validation.** v1 only needs sequential multi-request; the schema and process model must leave room for:

| Direction | Notes |
|-----------|--------|
| **Parallel / burst steps** | `concurrency`, `repeat`, hard `max_seconds`; client fan-out + `afwfcgi.threads` |
| **Soak profiles** | Longer runs opt-in (tags / flag) so default `afwdev test -j` stays a commit gate |
| **Valgrind on `afwfcgi`** | Catch definite leaks/invalid access across many requests in one process |
| **Observability** | See below — “are we screwing up memory management?” needs more than pass/fail |

#### Observability (design intent — evolve with #2)

We should figure out **what is worth observing** so advanced leaves can fail or warn when lifetime/pools go wrong—not only when Adaptive returns a wrong value. Candidates (research during #2; not all v1):

| Signal | Possible source | Use |
|--------|-----------------|-----|
| Valgrind errors | `--env-mode valgrind` + XML | Hard fail |
| Process RSS / growth over N requests | `/proc`, `resource`, or sampling around steps | Soft bound in stress leaves (“RSS must not grow unbounded”) |
| Request/pool metrics already exposed | Adaptive runtime / admin / debug objects if any | Assert stable or non-increasing where product defines |
| afwfcgi stderr patterns | harness capture | Unexpected fatal / assert |
| Step latency distribution | harness timers under stress | Catch pathological slowdown (often allocation thrash) |
| Graceful teardown | process exit code after SIGTERM | Pools released; no hang |

**Principle:** prefer **signals the product already exposes** or **standard process tools** over inventing a second memory debugger inside Adaptive. When #2 defines invariants (“after M evals, metric X ≤ bound”), advanced-test is the place to encode them as steps or leaf-level checks.

Harness may later support optional leaf fields like `observe: { rss_max_mb, … }` or post-step hooks—**do not invent the full language in v1**; keep stderr, timing, and exit status available first.

See [`memory-management.md`](memory-management.md).

### Stress (parked detail, high priority after vertical)

- Step kind or leaf profile: `concurrency` / `repeat` / time cap.  
- Default `-j`: tiny bursts only if any; intensive soaks behind tags/flags.  
- Always one server process per leaf (isolation); stress is **in-process request concurrency**, not cross-leaf.

---

## Non-goals

- Replacing all `.as` tests or generated function suites with advanced leaves  
- Harness starting full `/afw` nginx+apps for every test  
- Parsing markers with running AFW  
- Implementing #149/#2 product fixes on the harness branch  

---

## Implementation sketch (for implementers)

```text
_afwdev/test/
  common.py          # discovery: advanced-test leaf; prune; dispatch
  advanced/          # or scenario/
    load.py          # yaml/json → dict; validate schema
    runner.py        # lifecycle + step loop
    fcgi_client.py   # thin FastCGI client
    hosts/
      afwfcgi.py     # spawn args, health, env-mode variants
  modes/
    afw.py           # unchanged for .as; markers not routed here if dispatch is earlier
    afwfcgi.py       # live URL shared helper for .as + live-ok markers
    valgrind.py      # skip or wrap afwfcgi
```

Dispatch should treat marker paths **before** env-mode module selection (same pattern as `.py`), then pass `options['mode']` into the advanced runner for attach policy.

---

## Open only if implementation forces it

- Exact **URI / body envelope** for eval/perform—**match what already works** against live afwfcgi for `.as` (`eval_script` / Session). Default **FCGI params** are specified above from nginx reference.  
- Default `timeout_s` number (start with 60–120s; adjust for CI).  
- Valgrind in first PR vs skip-with-hook (prefer implement if small; do not block smoke).  
- Which #2 metrics are stable enough to assert (decide on #2 work, not guess here).

---

## Checklist

- [x] Marker name `advanced-test`  
- [x] `host` discriminant; v1 `afwfcgi`  
- [x] Env-mode matrix  
- [x] Live = current `afwfcgi` mode URL behavior  
- [x] Schema sketch + one pass/fail per leaf  
- [x] Installed binary; Unix socket; work dir copy  
- [x] PyYAML required  
- [x] Part of normal `afwdev test -j`  
- [x] #149 / #2 + stress/observability called out  
- [x] Nginx-parity FCGI default params  
- [x] GitHub issue [#157](https://github.com/afw-org/afw/issues/157)  
- [x] Implement vertical + smoke leaf (`src/afw/tests/advanced/smoke/`)  
- [x] Valgrind-on-afwfcgi (spawn hook + works under `--env-mode valgrind`)  
- [x] Merge → rebase #149 → catalog scenarios (`tests/advanced/catalog-value-accessors`, `tests_special/`)
- [x] Failure reporting: Adaptive-shaped `error` dict; one-line console reason; full nav `--verbose`/`--debug`
- [x] `stepTimings` on leaf response (`[{name, ms, passed}, …]`) for debug / future summaries
- [x] Exceptions: host/load/FCGI inherit `_afwdev.common.errors` (**#61**); digests use `error_message` / `to_error_dict`
- [x] Opt-in trees: `afwdev test -T src/afw/tests_special/…` (lifecycle leaf); always-on catalog leaf under `tests/advanced/`

Agent recipe: [`afwdev-test-recipe.md`](afwdev-test-recipe.md).  
- [ ] #2 stress + observe extensions  
