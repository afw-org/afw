# Agent support — maps and playbook stubs

**Audience:** maintainers and AI assistants (ongoing support + development partner).  
**Not** published handbook or end-user docs.  
**Hub:** [`AGENTS.md`](../AGENTS.md) (mission, knowledge map, how we learn).

## Intent

Issue work builds deep AFW skill. Durable notes should favor **maps** (concept, layer, probe, code entry) over ticket status dumps. Issue numbers are **pointers** only.

Fill stubs below after deep sessions. Prefer thin, accurate units over long diaries.

## Capture checklist (after a deep thread)

When promoting out of chat / session memory:

- [ ] Mental model change (what is true now?)
- [ ] Wrong path never to take again
- [ ] Live probe (command or retrieve that proves it)
- [ ] Optional one-sentence user/operator note (`whats-new.md` if shipping)

## Playbook stubs

Shape for each: **symptom → layer → probe → code / doc entry**.

### Hosts and process lifetime

| Field | Notes |
|-------|--------|
| Symptom | Process won’t die cleanly; Unix socket left behind; hung accept; SIGTERM ignored |
| Layer | Host (`afw` vs `afwfcgi`) vs core `env->terminating` / request path |
| Probe | `afwfcgi` signal hermetic test; help text; `AFW_XCTX_THROW_IF_TERMINATING` call sites |
| Entry | `#158` graceful stop (merged); rules `afw-server-fcgi`, `afw-command`; `whats-new.md` |
| Status | **Stub** — flesh as support cases appear |

### Environment registries and runtime objects

| Field | Notes |
|-------|--------|
| Symptom | “Where is X registered?”; empty/wrong runtime object; catalog field stale or racy |
| Layer | `afw_environment.h` registries; `adapterId=afw` runtime OT views; generate maps vs accessors |
| Probe | `retrieve_objects` / `get_object` on `_AdaptiveEnvironmentRegistryType_*`; `/afw/_AdaptiveRuntimeValueAccessor_/`; avoid stopping permanent adapters |
| Entry | `designs/runtime-objects-and-environment.md`; `afw-environment` rule; #149 closed (PRs #160–#162) |
| Status | **Stub** — architecture pad is the deep map |

### Compile / eval / values / memory

| Field | Notes |
|-------|--------|
| Symptom | Leak under long run; use-after-free; wrong lifetime; decompile mismatch |
| Layer | Pools, managed values, compiled_value, scope stack, statement_flow |
| Probe | Targeted `.as` + valgrind; advanced-test multi-request when process-scoped |
| Entry | `afw-value-memory`, `afw-script-eval`, `afw-compile`; umbrella **#2** / `memory-management.md` |
| Status | **Stub** |

### afwdev test surfaces (gate vs lab)

| Field | Notes |
|-------|--------|
| Symptom | Confused about `-j` vs `-T` vs blast; load thrash treated as language gate |
| Layer | `afwdev test` discovery vs `tests_special` vs `afwdev blast` |
| Probe | `afwdev test -j`; `afwdev test -T src/afw/tests_special/...`; blast attach only |
| Entry | `designs/afwdev-test-recipe.md`, `afwdev-advanced-test.md`, `afwdev-blast.md` |
| Status | **Stub** — recipe pad is the day-to-day map |

## Concept cards (short)

Add one-liners here when a durable fact is not worth a full playbook yet.

| Concept | One-liner | Deeper |
|---------|-----------|--------|
| Host contracts | `afwfcgi` wakes workers + unlinks Unix path; `afw` sets `terminating` only | #158 section in project memory / rules |
| Env discovery | Everything registered can be read as runtime objects on `adapterId=afw` | runtime-objects pad |
| Beta | Quality campaign, not partnership end date | `AGENTS.md` mission |

## Out of scope for this pad

- Rewriting all `.cursor/rules`
- Full operator handbook
- Auto-sync of every PR into memory
- Product C/runtime changes (those ship on issue branches)
