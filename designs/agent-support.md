# Agent support — maps and playbooks

**Audience:** maintainers and AI assistants (ongoing support + development partner).  
**Not** published handbook or end-user docs.  
**Hub:** [`AGENTS.md`](../AGENTS.md). **Topic index:** [`knowledge-atlas.md`](knowledge-atlas.md).

## Intent

Issue work builds deep AFW skill. Durable notes favor **maps** (concept, layer, probe, code entry) over ticket status dumps. Issue numbers are **pointers** only.

Prefer thin, accurate units. After deep sessions use the capture checklist; promote into git, not unbounded MEMORY.

## Capture checklist (after a deep thread)

- [ ] Mental model change (what is true now?)
- [ ] Wrong path never to take again
- [ ] Live probe (command or retrieve that proves it)
- [ ] Optional one-sentence user/operator note (`whats-new.md` if shipping)
- [ ] Atlas topic row touched if the map changed

## Playbooks

Shape: **symptom → layer → probe → code / doc entry**.

### Hosts and process lifetime

| Field | Notes |
|-------|--------|
| Symptom | Process won’t die cleanly; Unix socket left behind; hung accept; SIGTERM ignored; 503 while draining |
| Layer | Host (`afw` vs `afwfcgi`) vs core `env->terminating` / request path |
| Probe | `src/afw/tests/advanced/afwfcgi_signal_shutdown/`; `afwfcgi --help`; grep `AFW_XCTX_THROW_IF_TERMINATING` |
| Entry | #158 closed (PR **#165**); rules `afw-server-fcgi`, `afw-command`, `afw-server`; `whats-new.md`; atlas §6 |
| Status | **Filled (first pass)** |

**Contracts**

| Host | On SIGTERM/SIGINT |
|------|-------------------|
| `afwfcgi` | Set terminating; FCGI shutdown pending; **SIGUSR1** request threads; join; close listen; **unlink Unix `-p` path** (not TCP `:<port>`) |
| `afw` | Set terminating **only** (no accept wake) |

**Core:** `terminating` → HTTP **503**; macro at object/work-unit boundaries (retrieve loops, stream write, runtime walks, …). Soft partial retrieve (callback true) ≠ hard stop.

**Never:** close listen fd alone as the multi-thread wake story while main is joining — **thread signal** is the libfcgi path. Don’t invent one wake strategy for every future `afw_server` host.

**Later (not bugs in #158):** drain timeout, SIGHUP, Windows service, full signal framework, every extension loop, unblock `--local` read.

---

### Environment registries and runtime objects

| Field | Notes |
|-------|--------|
| Symptom | “Where is X registered?”; empty/wrong runtime object; catalog field stale/racy; huge `/current` hangs or OOMs |
| Layer | `afw_environment.h` registries; `adapterId=afw` runtime OT views; generate maps vs value accessors |
| Probe | Typed `retrieve_objects` / `get_object`; `/afw/_AdaptiveRuntimeValueAccessor_/`; avoid stopping `adapter-afw` / `adapter-conf` |
| Entry | `runtime-objects-and-environment.md`; `afw-environment`; #149 closed (PRs #160–#162); atlas §5 |
| Status | **Filled (first pass)** — architecture pad is the deep map |

**Condensed model**

- One process → one `afw_environment_t` → keyed registries; register usually publishes **runtime objects** on adapter `afw`.
- Runtime objects are **immutable views** (often live C behind accessors). Requester side of `afw_object` does not know storage class — only the inf.
- Metadata SoT: OT `runtime` props → maps/accessors. Prefer **fix named accessors once** over clone-all.

| Want | How |
|------|-----|
| All of a kind | `retrieve_objects(afw, _Adaptive…_, maxObjects=0)` |
| One | `get_object` / `/afw/<type>/<id>` |
| Accessor policy | `/afw/_AdaptiveRuntimeValueAccessor_/<key>` |
| Everything-ish | `/afw/_AdaptiveEnvironmentRegistry_/current` — **materializes** each kind (functions dominate); prefer typed retrieves |

**maxObjects:** default ~100 may error; **0 = unlimited**.

**Adapter catalog (shipped pattern):** `referenceCount` lock+snapshot; `stopping_*` lock+copy; `metrics`/`properties` pointer under lock, **live while active** — do not cache across stop without a session ref. The lock is only the load: after unlock nothing owns that object unless the caller already holds a session. Views + `metaFull`: clone permanent OT propertyTypes onto view pool.

**HTTP:** GET `/afw/...` and POST `/afw` actions (`function` / `actions[]`) — same env.

---

### Compile / eval / values / memory

| Field | Notes |
|-------|--------|
| Symptom | Leak under long run; use-after-free; wrong lifetime; decompile mismatch; scope/closure surprise |
| Layer | Pools, managed values, `compiled_value`, scope stack, `statement_flow`, value inf policy |
| Probe | Narrow `.as` + valgrind; orchestrated multi-request leaves when process-scoped; don’t soak via default `test -j` |
| Entry | `afw-value-memory`, `afw-script-eval`, `afw-compile`, `afw-runtime-model`; **#2** / `memory-management.md`; philosophy pad; atlas §3–4 |
| Status | **Filled (pointer-heavy)** — deep work stays in memory pad |

**First questions**

1. Is this **request-scoped** (pool teardown hides bugs) or **long-running** (escape must be correct)?  
2. Is the value **permanent / managed / managed_slice / unmanaged**?  
3. Did evaluation allocate into **`scope->p`** (or the intended pool)?  
4. Did a **closure** keep a scope alive (expected RC path)?

**Shapes that keep coming back (learn these, not ticket lists)**

- **Create vs evaluate** — do not mix (`afw-script-eval`). `argv[0]` at create is the callee expression; `x->function` is harvest at evaluate.  
- **New get, old delete** — look-through / face / view added on read; mutate/delete/count still the old impl. Faces: delete is a local NULL tombstone (`issue-17`).  
- **Sibling already learned it** — `split()` empty separator vs `replace()` empty match; `create_array()` cap vs `read(n)`; `copies_under_lock` vs live metrics.  
- **Two impls of one interface** — memory array index uses `>= count`; C-array view used `> count`.  
- **Script integer → malloc / spin / C stack** — APR pools often abort on huge alloc; empty match + “replace all” never advances; type and destructure parse have a nesting limit (`AFW_COMPILE_PARSE_NESTING_MAX`); other grammars may not.  
- **Type names are declared before use** — like script values, not hoisted. Self-ref in the same `type` / `interface` body is allowed. Unknown names are a compile error even with typeCheck off.  
- **Evaluate twice, first result sizes a buffer** — call-site `...expr` (#181, fixed). Same class: Pattern rest keys.
- **Early return skips eval-stack / error-contextual pop** — a not-found or empty path returns before the cleanup every other exit does; the next unrelated error points at the wrong source. Route through the same pop/restore.
- **Allocator is sacred** — overflow on `size + prefix`, or a free-list that only relinks adjacent blocks, is a local check / missing `else` splice. Do **not** invent a new pool to “fix” it.

**Wrong path:** deep-cloning whole adapters/registries to “fix” one bad lifetime; treating short-test green as long-run proof.

---

### afwdev test surfaces (gate vs lab)

| Field | Notes |
|-------|--------|
| Symptom | Confused about `-j` vs `-T` vs firehose; load thrash treated as language gate; stale afwfcgi after rebuild; valgrind `-j` “stuck” |
| Layer | `afwdev test` discovery vs `tests-extra` vs orchestrated leaves (`orchestration.yaml`) |
| Probe | Commands in recipe pad; after `--cdev`/`--install`, restart long-lived afwfcgi before attach |
| Entry | `afwdev-test-recipe.md`, `src/afw/tests-extra/{README,SCHEMA}.md`, `afw-tests`; atlas §11 |
| Status | **Filled** (PR **#167**) |

| Surface | Job | Gate? |
|---------|-----|-------|
| `afwdev test -j` | Language/package suite + short orchestrated leaves under `src/*/tests/` | **Yes** |
| `afwdev test -T path` | Opt-in trees only (`tests-extra/`, etc.) | Opt-in |
| `schedule.firehose` leaves | Load thrash (replaces retired `afwdev blast`) | **No** |

**Never:** redefine default `test -j` as soak/load. Suite green ≠ firehose green (timeouts under high concurrency are load).

**Orchestrated:** marker `orchestration.yaml`/`json`; hosts **`afwfcgi`** (hermetic) or **`local`** (`afw --local`); x-afw demux expects. **#13** still open for Jeremy’s stress knobs/stats story.

**Valgrind hang lesson:** parallel valgrind can park the pool if one worker blocks forever (e.g. `Session("local").close()` → `wait()`). Harness now times out/kills; if wall time is absurd with idle CPU, check for a stuck worker before assuming “just slow.”

---

### Live stack (`afwfcgi` + nginx) and stale binaries

| Field | Notes |
|-------|--------|
| Symptom | `--env-mode afwfcgi` fails after a green CLI run; app/Fiddle “old” after `--cdev`; map/filter or new syntax missing only on HTTP |
| Layer | Long-lived `afwfcgi` still mapping **deleted** `/usr/local/lib/afw/libafw.so` / `afwfcgi (deleted)`. CLI `afw` always loads the current install. |
| Probe | `/proc/<pid>/exe` and mapped libs must not say `(deleted)`. Restart after every install. |
| Entry | atlas §6 and §11; `.devcontainer/afw/nginx.conf`; `/afw/afw.conf` |
| Status | **Filled** (8 Aug live tour) |

**Dev container (typical):** nginx `:8080` → Unix `/var/run/afw.sock` → `afwfcgi -f /afw/afw.conf`. Tests in `--env-mode afwfcgi` use `Session("http://localhost:8080/afw")` (`.as` only; skips custom local `afw.conf`).

```bash
# after ./afwdev build --cdev / --install
pkill -x afwfcgi
# leave nginx; it proxies the socket
afwfcgi -f /afw/afw.conf -p /var/run/afw.sock -n 25 &
```

**GDB:** `afwfcgi -n 1` (one worker). Break `afw_action_perform` or an accessor; drive with curl `POST /afw` from another shell. Binary has debug info.

**Never:** stop permanent services `adapter-afw` / `adapter-conf` as a “catalog” experiment.

---

### Actions, Fiddle, and the admin app

| Field | Notes |
|-------|--------|
| Symptom | “How does Fiddle run a script?”; GET vs POST confusion; extension_load “does nothing” under AFWDev |
| Layer | Same env. **GET** `/afw/<type>/[id]` = adapter CRUD. **POST** `/afw` = `afw_action_perform` (any Adaptive function). |
| Probe | `{"function":"get_object","adapterId":"afw","objectType":"_AdaptiveFunction_","objectId":"map"}`. Scripts: `function` `eval<script>` or `eval_script`, `source` the text. Batch: `{"actions":[…]}`. |
| Entry | atlas §16; `afw_action.c` / `afw_request_handler_adapter.c`; app `AfwClient.perform` |
| Status | **Filled** (8 Aug live tour) |

Clients: admin Fiddle (`src/afw_app` + `src/afw_client`), Python `Session` / `Request().add_action().perform()`, curl. Optional `Accept: application/x-afw` for progressive frames.

AFWDev application conf already loads **`afw_crypto`** (and others). `extension_load` returns false if already loaded — use a **minimal** conf when measuring registry deltas.

Do **not** implement admin JS unless asked. The support model is the C/request contract.

---

### Adapter types vs instances

| Field | Notes |
|-------|--------|
| Symptom | “Why is there no file adapter?” when `_AdaptiveAdapterType_` lists `file` |
| Layer | **Type** = factory in the env (`_AdaptiveAdapterType_`). **Instance** = started service (`_AdaptiveAdapter_`, `adapter-…`). Conf / `service_start` creates instances. |
| Probe | Retrieve both object types with `maxObjects=0`. Minimal conf often has only instance `afw`. |
| Entry | atlas §5 and §7; `afw-core-services`; `service_start` / `service_stop` |
| Status | **Filled** |

---

### Disclosure-sensitive C review (private board → public issue)

| Field | Notes |
|-------|--------|
| Symptom | A review dumps many crash / use-after-free / overflow / “guardrail” items; someone wants them tracked; they must not all become public `afw` issues |
| Layer | Process, not a runtime layer. Public `afw` = real issues and PRs. Who/status for the list stays **private** until a row starts and disclosure is OK |
| Probe | None in-tree. Maintainers: private org Project **Board** view + its how-to issue (the **issue body** is the handbook, not a comment). Do not grep this playbook for finding ids — they do not belong here |
| Entry | This playbook; pairing note in `ai-partner-lessons` / `AGENTS.md`. How-to and the living board live **outside** public `afw` |
| Status | **Filled (2026-08)** — pattern only |

**Repos (origins, not a finding list)**

| Repo | Use for |
|------|---------|
| Public **`afw`** | The issue/PR when we start an item and are willing to talk in the open |
| **`inter-afw`** | Whole-project private AFW (pre-public history; current home for this kind of board) |
| **`inter-afw-private`** | Not-yet-public **extensions** (Oracle, Berkeley DB, …), not the whole-AFW audit board |

**How we run it**

- One private **Project** (Board view) is the living Who / Status. GitHub **Assignees** on draft cards (drafts *can* be assigned; a custom Who field will not show like avatars).
- How-to is a **private issue** on the Board (often **No status**). That card *is* the handbook. Comments there are how-to changes only — not a start/ship log. GitHub does **not** paint the Project README on the Board.
- Cards stay **drafts** so we do not mint one GitHub issue per finding. The **card body** has the working detail (location, mechanism) plus public `afw` markdown links when they exist (`[PR #191](https://github.com/afw-org/afw/pull/191)`). Do not put `#N` or URLs in a Project text field — the Board treats them as other cards. Drafts cannot use Linked pull requests. Do not paste hang/crash/overflow PoCs onto cards.
- A **report issue** is a frozen dump (history + PoCs). Not Who/Status. Wave 1 is closed; do not grow it. The Board is the living list.
- Do **not** file the whole list on public `afw`. Crash / use-after-free / overflow stay private (`hold` / `working`) until disclosure is decided. Language guardrails can go public earlier if we want.
- New session: discuss first (“what do you think about \<id\>?”), not “implement this PoC.” Flip the **card**.
- New dump: “triage these findings” → **match first** (same hole → note it on that card; only new holes get a card). A pile can be a new frozen report issue. A handful is chat. How-to comments are handbook edits only. Do not open a second how-to or a living table.

**Wrong path:** paste the review table into a public issue; pick the extensions-private repo because of `-private` in the name; dual-maintain a catalog issue and the Board.

---

## Concept cards

| Concept | One-liner | Deeper |
|---------|-----------|--------|
| Host contracts | `afwfcgi` wakes workers + unlinks Unix path; `afw` sets `terminating` only | atlas §6; this playbook |
| Env discovery | Registered capabilities readable as runtime objects on `adapterId=afw` | atlas §5; runtime-objects pad |
| Two doors | GET `/afw/…` CRUD vs POST `/afw` actions (Fiddle) | atlas §16; this playbook |
| Type vs instance | Factory registered ≠ adapter started | this playbook |
| Stale afwfcgi | Long-lived process after install maps deleted libs | this playbook |
| Values first | Script/eval/memory hang on `afw_value` policy, then pools | value-memory; #2 |
| Gate vs lab | `test -j` is correctness; blast/lab is separate | recipe pad |
| Beta | Quality campaign, not partnership end date | `AGENTS.md` mission |
| Philosophy / taste | Metadata truth, values+pools, small patterned pieces | philosophy pad |
| Mantras | “Get it right at most once,” maps over tickets, record decided-not, process serves efficiency/quality, decide while in the work, tests as lasting pretend mode, a new error gets a test that would have caught it (plus related cases), plain language, … | mantras pad |
| Knowledge atlas | Topic → sources → probe → gaps | [`knowledge-atlas.md`](knowledge-atlas.md) |
| Private review board | Who/status for crash-shaped review lists stays private; public `afw` gets one issue when work starts | this playbook *Disclosure-sensitive C review* |

## Out of scope for this pad

- Rewriting all `.cursor/rules`
- Full operator handbook
- Auto-sync of every PR into memory
- Product C/runtime changes (issue branches)
- Replacing large design novels (memory, crypto) with copies here
