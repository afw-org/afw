# Orchestration file schema (north star sketch)

**Audience:** maintainers designing orchestrated tests. **Not implemented.**  
**Marker (proposed):** `orchestration.yaml` or `orchestration.json` in a leaf directory.  
**Ambiguous both present:** error (same as experimental advanced-test).

This is the **full-ish** shape we want to aim at. First implementation should **trim**, not invent a different dialect.

---

## 1. Document top level

| Field | Req | Meaning |
|-------|----------|---------|
| `version` | no | Schema version integer; default `1` if omitted |
| `description` | no | One-line summary for `--list` / failure headers |
| `host` | **yes** | Host family. v1: `afwfcgi`. Later: `afw-local`, … |
| `afwfcgi` | no | Host-specific block (only when `host: afwfcgi`) |
| `timeout_s` | no | Whole-leaf wall clock (default e.g. 120) |
| `feed` | no | **Default** feed for tests (overridable per item) |
| `tests` | **yes\*** | Include list of work items (see below) |
| `schedule` | no | How `tests` run (default: sequential in list order) |
| `onFailure` | no | `failFast` (default) \| `continue` (later) |

\*Or `schedule` alone that embeds named tests — prefer explicit `tests` + optional `schedule`.

### `afwfcgi` block

| Field | Default | Meaning |
|-------|---------|---------|
| `threads` | `1` | Maps to `afwfcgi -n` |
| `conf` | `afw.conf` | Relative to leaf |

Harness owns: work dir copy, Unix socket under work dir, start/ready/stop, SIGTERM path (see #158).

---

## 2. Feed (how work hits the host)

**Document-level `feed:` is a default only.** Every field there applies to all
tests that do not set their own `feed:`. Each test may supply a full or
partial `feed:` map that **overrides** the defaults (merge: test wins on
keys it sets; other keys inherit from the document).

Why this helps:

- Most leaves share one Accept / action path — set once at the top.  
- Progressive, REST, or special Accept steps override only what differs  
  (e.g. `accept: application/x-afw` on one test).  
- Avoids repeating `kind: action` on every item.

```yaml
feed:
  kind: action
  accept: application/json

tests:
  - name: normal
    sourceType: script
    source: <<< tests/a.as
    # uses document feed

  - name: progressive
    feed:
      accept: application/x-afw   # kind still action from default
    sourceType: script
    source: <<< tests/b.as
    expectResponse: <<< goldens/b.xafw

  - name: rest-get
    feed:
      kind: rest                 # overrides kind
      method: GET
      path: /file/Demo
      accept: application/json
```

### Common feed fields

| Field | Meaning |
|-------|---------|
| `kind` | `action` \| `rest` \| later `raw-fcgi` |

### `kind: action` (Adaptive `_AdaptiveAction_` / perform)

| Field | Default | Meaning |
|-------|---------|---------|
| `path` | `/afw` | Request path |
| `function` | see payload | Adaptive function id (e.g. `eval<script>`, or any other) |
| `contentType` | `application/json` | Request body content type |
| `accept` | `application/json` | Response Accept |

When the payload is source-shaped (below), harness may default `function` from `sourceType`:

| sourceType | default function (sketch) |
|------------|---------------------------|
| `script` | `eval<script>` |
| `expression` | `eval<expression>` (or project’s real name) |
| `template` | `eval<template>` |
| `test_script` | function that runs a test_script value, **or** `eval<script>` if the file is a full test_script shebang and perform already supports that path |
| other | must set `feed.function` explicitly |

Action body is built from function + args; source payload becomes the usual `source` argument (or function-specific args map — later `feed.args`).

### `kind: rest`

| Field | Meaning |
|-------|---------|
| `method` | `GET` \| `POST` \| `PUT` \| `PATCH` \| `DELETE` \| … |
| `path` | URI path (e.g. `/file/Demo`) |
| `accept` | e.g. `application/x-afw`, `application/json` |
| `contentType` | if body present |
| `body` | optional raw/string/object (later) |

No Adaptive function; judges HTTP-ish status + body.

---

## 3. Tests (include list / work items)

`tests` is an array of objects. Each item is one **work unit** the orchestrator will feed.

### Identity

| Field | Req | Meaning |
|-------|----------|---------|
| `name` | **yes** | Stable id for schedule, logs, failures |
| `description` | no | Human text |
| `skip` | no | bool; default false |

### Payload (aligned with test_script evaluate model)

| Field | Meaning |
|-------|---------|
| `sourceType` | Compile/eval type pneumonic: `script` (default), `expression`, `template`, `test_script`, `json`, … |
| `source` | Payload text: inline string, or file form **`<<< rel/path`** (exact file bytes; path relative to **leaf** or to this file — pick one in impl; sketch: **relative to leaf root**) |

**Convenience aliases (optional sugar, not required in north star):**

- `sourcePath: steps/a.as` ≡ `source: <<< steps/a.as`
- Inline multiline YAML `|` ≡ same as test_script embedded source

**Not a separate universe from test scripts:** same ideas as directives `sourceType` + `source` (+ `expect`).

### Judging

| Field | Meaning |
|-------|---------|
| `expect` | Same *idea* as test_script `expect`: Adaptive source for expected **return value**, or `error` / `error:…` / `undefined`. Applied when the feed returns an Adaptive evaluation result (action eval path). |
| `expectResponse` | Raw expected **response body** (string or `<<< file`). Exact match after optional decode. For REST and progressive frames. |
| `expectStatus` | HTTP-ish status code (REST); default 2xx success if omitted |
| `decode` | later: e.g. `x-afw-payloads` before comparing `expectResponse` |

For `sourceType: test_script`, prefer judging via embedded case `passed` flags in the result object (like today’s advanced-test), optionally plus outer `expect`.

### Per-item feed override

```yaml
feed:
  kind: rest
  method: GET
  path: /file/Demo
  accept: application/x-afw
```

---

## 4. Schedule

If `schedule` omitted: run `tests` **in array order**, fail-fast, one at a time.

### Sequential (explicit)

```yaml
schedule:
  - sequential:
      - seed
      - get
      - cleanup
```

Names refer to `tests[].name`.

### Parallel group (**later**, but in the sketch)

```yaml
schedule:
  - sequential: [warmup]
  - parallel:
      n: 4                    # max concurrent feeds
      tests: [t1, t2, t3, t4]
  - sequential: [verify]
```

### Rate / soak / firehose (**later** — blast replacement)

See leaves `07`, `07b`, `07c` under this directory for full pretend examples.

```yaml
schedule:
  - firehose:
      duration_s: 60            # and/or maxRequests
      concurrency: 8
      fromTests: [a, b, c]      # names from tests[]
      # includeGlob: "tests/random/*.as"   # later alternative
      stopOnError: false        # blast-like: keep going, tally errors
      # seed: 42
      # policy: random | roundRobin
```

```yaml
schedule:
  - repeat:
      times: 100
      tests: [catalog-read]
```

Orchestrator owns timing and client pool; payloads stay in `tests` or globs.
Pass criteria for firehose may be “completed duration with error rate &lt; X /
no process crash,” not only every request green — still open.

---

## 5. Discovery (outside the file, but part of the product)

| Root | When |
|------|------|
| `src/*/tests/**/orchestration.yaml` | Day-to-day gate candidates (after rename from advanced-test) |
| `src/afw/orchestrated-tests/**` (this tree) | Sketch + heavier / design scenarios; **`--tests-path`** or explicit opt-in |
| Other trees | Load/soak/blast-like; never default `test -j` |

One engine; intensity = **where the leaf lives** + schedule, not a second subcommand forever.

---

## 6. Result grain

- **One leaf → one pass/fail** in suite counts (same as experimental advanced).  
- Failures name **`tests[].name`** (and schedule phase if useful).  
- test_script payloads: any inner `passed: false` fails the work item.

---

## 7. v1 trim suggestion (when implementing)

**Keep:**

- `host: afwfcgi`, `afwfcgi.threads`, `timeout_s`, `description`
- `feed` default `kind: action` + `accept`
- `tests[]` with `name`, `sourceType`, `source` (inline or `<<<` / path)
- sequential run order, fail-fast
- `expect` for action/eval results
- REST feed + `expectResponse` if progressive #127 leaf is in v1

**Defer:**

- `schedule.parallel`, `firehose`, `repeat`, `rate`
- `decode`
- `onFailure: continue`
- hosts other than afwfcgi
- auto-expand test_script into N feeds

**Migrate:**

- `eval:` → `sourceType: script` + `source:`
- `script: f.as` → `source: <<< f.as` or `sourcePath: f.as`
- marker `advanced-test.yaml` → `orchestration.yaml` (alias period optional)

---

## 8. Non-goals of the sketch

- Replacing test_script authoring for high-volume language tests  
- Making default `test -j` a soak  
- Full JSONPath expect language on day one  
