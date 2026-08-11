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

**Adapter catalog (shipped pattern):** `referenceCount` lock+snapshot; `stopping_*` lock+copy; `metrics`/`properties` pointer under lock, **live while active** — do not cache across stop without a session ref. Views + `metaFull`: clone permanent OT propertyTypes onto view pool.

**HTTP:** GET `/afw/...` and POST `/afw` actions (`function` / `actions[]`) — same env.

---

### Compile / eval / values / memory

| Field | Notes |
|-------|--------|
| Symptom | Leak under long run; use-after-free; wrong lifetime; decompile mismatch; scope/closure surprise |
| Layer | Pools, managed values, `compiled_value`, scope stack, `statement_flow`, value inf policy |
| Probe | Narrow `.as` + valgrind; advanced-test multi-request when process-scoped; don’t soak via default `test -j` |
| Entry | `afw-value-memory`, `afw-script-eval`, `afw-compile`, `afw-runtime-model`; **#2** / `memory-management.md`; philosophy pad; atlas §3–4 |
| Status | **Filled (pointer-heavy)** — deep work stays in memory pad |

**First questions**

1. Is this **request-scoped** (pool teardown hides bugs) or **long-running** (escape must be correct)?  
2. Is the value **permanent / managed / managed_slice / unmanaged**?  
3. Did evaluation allocate into **`scope->p`** (or the intended pool)?  
4. Did a **closure** keep a scope alive (expected RC path)?

**Wrong path:** deep-cloning whole adapters/registries to “fix” one bad lifetime; treating short-test green as long-run proof.

---

### afwdev test surfaces (gate vs lab)

| Field | Notes |
|-------|--------|
| Symptom | Confused about `-j` vs `-T` vs blast; load thrash treated as language gate; stale afwfcgi after rebuild |
| Layer | `afwdev test` discovery vs `tests-extra` vs `afwdev blast` vs advanced-test leaves |
| Probe | Commands in recipe pad; after `--cdev`/`--install`, restart long-lived afwfcgi before attach |
| Entry | `afwdev-test-recipe.md`, `afwdev-advanced-test.md`, `afwdev-blast.md`, `afw-tests`; atlas §11 |
| Status | **Filled (first pass)** |

| Surface | Job | Gate? |
|---------|-----|-------|
| `afwdev test -j` | Language/package suite + advanced leaves under `src/*/tests/` | **Yes** |
| `afwdev test -T path` | Opt-in trees only | Opt-in |
| `afwdev blast` | Random load at afwfcgi | **No** |

**Never:** redefine default `test -j` as soak/load. Suite green ≠ blast green (timeouts under high concurrency are load).

**advanced-test:** leaf marker `advanced-test.yaml`/`json`, hermetic `afwfcgi` + FCGI client; experimental (#157). **blast:** separate subcommand (#13 asked for rounds on test — we shipped sibling blast instead).

---

## Concept cards

| Concept | One-liner | Deeper |
|---------|-----------|--------|
| Host contracts | `afwfcgi` wakes workers + unlinks Unix path; `afw` sets `terminating` only | atlas §6; this playbook |
| Env discovery | Registered capabilities readable as runtime objects on `adapterId=afw` | atlas §5; runtime-objects pad |
| Values first | Script/eval/memory hang on `afw_value` policy, then pools | value-memory; #2 |
| Gate vs lab | `test -j` is correctness; blast/lab is separate | recipe pad |
| Beta | Quality campaign, not partnership end date | `AGENTS.md` mission |
| Philosophy / taste | Metadata truth, values+pools, small patterned pieces | philosophy pad |
| Mantras | “Get it right at most once,” maps over tickets, plain language, … | mantras pad |
| Knowledge atlas | Topic → sources → probe → gaps | [`knowledge-atlas.md`](knowledge-atlas.md) |

## Out of scope for this pad

- Rewriting all `.cursor/rules`
- Full operator handbook
- Auto-sync of every PR into memory
- Product C/runtime changes (issue branches)
- Replacing large design novels (memory, crypto) with copies here
