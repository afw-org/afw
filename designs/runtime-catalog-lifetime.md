# Runtime / `afw` adapter catalog lifetime

**Audience:** maintainers / assistants. **Not** handbook.  
**Status:** discovery notes only — **not** blocking issue #17 (mutable faces).  
**Architecture (preferred map):** [`runtime-objects-and-environment.md`](runtime-objects-and-environment.md) — generate → maps → accessors → env registration → #149 checklist.  
**Value accessor catalog snapshot:** [`runtime-value-accessors.md`](runtime-value-accessors.md) (from live `afw -x` retrieve).  
**Parent / umbrella:** GitHub **[#2 Memory management](https://github.com/afw-org/afw/issues/2)** and [`memory-management.md`](memory-management.md).  
**Related:** [#49](https://github.com/afw-org/afw/issues/49) `maxObjects`, [#127](https://github.com/afw-org/afw/issues/127) progressive release, admin `AfwModel` `maxObjects: 0` for metadata catalogs.  
**GitHub tracking issue:** [#149](https://github.com/afw-org/afw/issues/149) (parent pointer on [#2](https://github.com/afw-org/afw/issues/2)).

---

## What this is (and is not)

| This topic | Not this topic |
|------------|----------------|
| How **`adapterId=afw`** / **runtime** objects expose process registries | Script **mutable faces** (#17) |
| Live C maps vs materializing huge registry trees | Deep freeze / write-through stores |
| Admin “know the whole environment” retrieves | File/model adapter entity mutability |

**#17 does not depend on fixing this.** Faces over immutable catalog objects only stop script sets from pretending to own env state; they do not snapshot or deep-clone registries.

---

## Mental model (from 2026-08 discussion)

### Kinds of objects on the `afw` adapter

1. **Const / permanent** — e.g. large generated `afw_const_objects.c` (object types, metadata). Process lifetime, immutable, paid once at load.
2. **Runtime mapped** — `afw_runtime` object maps: property get → **accessor** over **offset into live C struct** (or custom accessor). Immutable (`get_setter` NULL). Release/get_reference often no-ops.
3. **Custom “current” shells** — e.g. `/afw/_AdaptiveEnvironmentRegistry_/current`: permanent shell; **properties materialize** nested bags of registered entries on get.

Default value accessor path (`afw_runtime_value_accessor_default`): builds an Adaptive value with `afw_value_common_create` — **memcpy of the data-type’s internal size**. Scalars are snapshotted bits; `afw_utf8_t` copies the struct (pointer + len) so **bytes still live where `s` points**; pointer types copy the pointer.

### Liveness worry (valid but bounded)

- **Between two property gets:** no full-tree consistency; mid-update possible if env mutates. After boot, registries are mostly append-only / controlled reregister.
- **Dangling pointers:** mainly if something **unloads/frees** while a request still holds a value that points into that memory — not typical steady-state admin browse.
- **Not** the same as #17 shared mutable bags.

### Materialize cost (the “huge object” worry)

`_AdaptiveEnvironmentRegistry_/current` **`get_property(registryType)`** allocates a **new** memory object and fills it via `afw_runtime_foreach` for every registered key of that type. Walking or JSON-encoding the **whole** registry expands **all** types (functions, data types, adapters, …) into request-pool + wire memory.

Admin SPA patterns that hit this world:

- `get_object_with_uri` → `/afw/_AdaptiveEnvironmentRegistry_/current` (e.g. Services UI after start/stop)
- `retrieve_objects` on `_AdaptiveManifest_`, extensions, **`_AdaptiveObjectType_`** catalogs
- Client uses **`maxObjects: 0`** so full catalogs are not cut off by #49’s default 100

Dominant cost for catalogs is usually **count × serialize**, not #17 faces.

---

## Possible later phases (not committed plan)

1. **Document** product semantics: live/immutable catalog; prefer typed retrieve over full registry walk when possible.
2. **Accessor safety** where unload can free string/pointer payload — clone into request pool only where needed.
3. **Admin / API shape** — slice retrieves vs one giant `EnvironmentRegistry/current` expand.
4. Fold into **#2** long-running value/pool story; keep progressive release **#127** separate.

---

## Session archaeology

Discussed on branch `issue-#17-object-literals-immutable` while finishing faces (2026-08). Decision: **do not implement in #17 PR**; track under #2 with a dedicated issue for comments and progress.
