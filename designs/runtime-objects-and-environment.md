# Runtime objects and the environment

**Audience:** maintainers and AI assistants; useful secondary reading for extension/command authors.  
**Not** published handbook or end-user docs.  
**Status:** architecture reference for **[#149](https://github.com/afw-org/afw/issues/149)** (child of **[#2 Memory management](https://github.com/afw-org/afw/issues/2)**).  
**Related pads:** [`runtime-catalog-lifetime.md`](runtime-catalog-lifetime.md) (discovery notes; may be superseded by this file for architecture), [`memory-management.md`](memory-management.md) (#2 umbrella).  
**Related issues (separate tracks):** [#49](https://github.com/afw-org/afw/issues/49) `maxObjects`, [#127](https://github.com/afw-org/afw/issues/127) progressive retrieve release, [#17](https://github.com/afw-org/afw/issues/17) faces (different lifetime problem).

**Source of truth:** Adaptive **metadata** (object types, functions, data types, generate JSON). This document is the **map** of how that metadata becomes C runtime views of the environment—not a second registry of every field.

---

## 1. Purpose: the environment as Adaptive objects

Runtime objects exist so **everything about the AFW environment** can be viewed from outside the server/command process:

- HTTP / request handlers (`adapterId=afw`)
- Adaptive functions and scripts
- Admin tooling

**Read path:** object get / retrieve → Adaptive values built from live C state (or static const graphs).  
**Write path:** modifications go through Adaptive functions / requests (start service, load extension, configure, …)—**not** write-through mutation of env C structs via object setters.

Invariants that matter for #149:

| Invariant | Implication |
|-----------|-------------|
| Runtime objects are generally **immutable** (`get_setter` → NULL) | No shared mutable “face” over env; not #17 |
| **No whole-server mutex** | Env changes over time; cannot lock the world for a consistent snapshot |
| **Many locks already exist** | Use them at the points that need them (register, adapters stopping, …) |
| **One-size “clone everything on get” is wrong** | Huge graphs (functions, object types, full registry expand); fix per type at accessors |
| **Most memory does not go away** | Static const from generate; built-in + extension-loaded functions typically never unload; adapters often until use count → 0 |

Prefer the term **runtime env metadata / registry views** over informal “catalog” when it confuses: there is no formal Adaptive type named Catalog. The `afw` adapter exposes process registries and permanent/const metadata as Adaptive objects.

---

## 2. Generate pipeline (OT → maps / const / bindings)

### 2.1 Inputs under `generate/objects/`

| Folder (examples) | Role |
|-------------------|------|
| `_AdaptiveObjectType_/` | Built-in object types (extensions/commands can ship the same layout) |
| `_AdaptiveFunctionGenerate_/` | Function definitions (become bindings + runtime function objects) |
| `_AdaptiveDataTypeGenerate_/` | Data types |
| `_AdaptiveManifest_/` | Extension lazy-load manifests |
| Other type folders | More **instances** that are themselves Adaptive objects (const registration) |

Object types are **objects too**. afwdev generate treats OTs specially in places (maps, const emission, validation).

### 2.2 Generator of interest for runtime views

Primary file: `src/afw_dev/_afwdev/generate/runtime_object_maps.py`.

For each OT under `generate/objects/_AdaptiveObjectType_/`:

1. Emit `afw_runtime_inf_<ObjectTypeId>` (object interface + type meta).
2. If the OT has a top-level **`runtime`** object (including empty `{}`):
   - Emit `impl_properties_<id>[]` — per-property offsets / accessors.
   - Emit `impl_runtime_object_map_<id>`.
3. Always emit `impl_runtime_meta_<id>` (`afw_runtime_object_type_meta_t`).
4. Collect NULL-terminated list of infs → `<prefix>register_runtime_object_maps()`.

Outputs (core):

- `src/afw/generated/afw_runtime_object_maps.c`
- `src/afw/generated/afw_runtime_object_maps.h`

Do **not** hand-edit `generated/`.

### 2.3 Other generate outputs (summary)

| Output | Role for runtime views |
|--------|------------------------|
| `const_objects.*` | Static const Adaptive object graphs (OTs, collections, conf shapes, …) registered into the runtime object hash |
| Function bindings / `afw_value_function_definition_t` | Live structs; often paired with an indirect runtime object shell |
| Data type structs | Registered + often a const or set runtime object |
| Strings / `self_s_*` | Shared permanent labels used by maps |
| Interfaces | Orthogonal; C capability contracts, not the env view layer |

Rough scale in core today: ~120 OT JSON files; a minority declare top-level `runtime` with a C `typedef` (mapped structs). Many OTs exist only as const metadata or pure type definitions. Model “current::” OTs use `runtime: {}` plus property-level `onGetValueCFunctionName` (callback accessors, no offsetof map into a fixed env struct).

### 2.4 Registration order (bootstrap)

From environment bootstrap (`afw_environment_register_core` / create path)—order matters:

1. **`afw_generated_register`** — data types, functions, **runtime object maps**, const objects, …
2. Early flags → value infs → **`afw_runtime_register_core_value_accessors`**
3. `prepare_environment` for operators (needs functions from step 1)
4. Conf types, adapter types, factories, OS, …

See `.cursor/rules/afw-environment.mdc` for the full sequence. Extension/command packages call their own `<prefix>register_runtime_object_maps` / generated register the same way.

---

## 3. OT `runtime` property vocabulary

Metadata schemas: `_AdaptiveRuntimeObject_`, `_AdaptiveRuntimeProperty_`, `_AdaptiveRuntimeLabels_`.

### 3.1 Object-type level (`objectType` JSON → `obj.runtime`)

| Field | Meaning |
|-------|---------|
| **`typedef`** | C type name used with `offsetof` for members (e.g. `afw_adapter_id_anchor_t`, `afw_value_function_definition_t`) |
| **`indirect`** | If true, instances are `afw_runtime_object_indirect_t`: `pub` + `void *internal` pointing at the struct; offsets apply to `*internal`. If false, offsets are relative to the object instance itself (const-style layout) |
| **`labels`** | Member names for object id / optional extra properties array (`objectId`, `indirectObjectId`, `properties`, …) |

If top-level `runtime` is **absent**, the generator still emits an inf using default const layout (`afw_runtime_const_object_instance_t`) and **no** property offset map (`property_map` NULL).  
If `runtime` is present but `typedef` omitted, generator defaults to const-instance labels (used by some hybrid cases).

### 3.2 Property level (`propertyTypes.<name>.runtime`)

| Field | Meaning |
|-------|---------|
| **`memberName`** | C struct member; default = Adaptive property name |
| **`valueAccessor`** | Name of registered accessor; default **`default`**. Becomes `afw_runtime_value_accessor_<name>` in generated maps |
| **`onGetValueCFunctionName`** | Hand C function used as accessor; **offset = -1**; ignores `valueAccessor`. Declared in generated maps header |
| **`zeroOffset`** | Force offset 0 (accessor sees start of struct / internal) |
| **`count_offset`** | Supported by generator → `count_offset` in map property (arrays with separate count member); rarely used in OT JSON today |

Example — `_AdaptiveAdapter_`:

```json
"runtime": {
    "indirect": true,
    "labels": { "indirectObjectId": true, "objectId": "adapter_id" },
    "typedef": "afw_adapter_id_anchor_t"
}
```

Property `stopping` uses `valueAccessor: "stopping_adapter_instances"` (lock + copy of refcounts). Property `adapterId` uses `valueAccessor: "indirect"`.

### 3.3 Common value accessors (core)

Registered in `afw_runtime_register_core_value_accessors` (`runtime/afw_runtime_value_accessor.c`).

**Registration model:** each accessor has a co-located `afw_runtime_value_accessor_info_t` (key, function, brief, description, `copies_under_lock`, `returns_live_reference`) next to the C function. Register **that struct** via `afw_environment_register_runtime_value_accessor(info, xctx)`. Registry value is the info; C gets the function with `afw_environment_get_runtime_value_accessor(name)`. Adaptive catalog is `/afw/_AdaptiveRuntimeValueAccessor_/<key>` (`register_default` maps the info struct). Extensions use the same register API.

| Accessor | Behavior sketch |
|----------|-----------------|
| **`default`** | `afw_value_common_create` over the member bits (memcpy of data-type internal size). `afw_utf8_t` copies pointer+len—**bytes still live where `s` points** |
| **`indirect`** | One pointer dereference, then default |
| **`value`** | Member is already `const afw_value_t *` — return it |
| **`size` / `octet` / `uint32`** | Widen to Adaptive integer |
| **`compile_type` / `data_type_id` / `service_status` / …** | Enum/pointer → string (or similar) |
| **`null_terminated_array_of_*`** | Build array views from C NULL-terminated tables |
| **`stopping_adapter_instances` / `stopping_authorization_handler_instances`** | **Lock** + copy integer array into call pool |
| **`adapter_metrics` / `adapter_additional_metrics`** | Nested object / metrics pull |
| **`applicable_flags`** | Builds list from flag state |
| **`ensure_afw_components_extension_loaded`** | Side-effecting get (lazy extension load) |

Discover/filter: `retrieve_objects("afw", "_AdaptiveRuntimeValueAccessor_", {}, undefined, undefined, 0)` (`maxObjects` **0** = unlimited) and query on `copiesUnderLock` / `returnsLiveReference` / `key` / text fields.

**Consumable snapshot for assistants/maintainers:** [`runtime-value-accessors.md`](runtime-value-accessors.md) (tables + full descriptions; refresh command at top of that file).

**Automated checks:**

| Kind | Location | Run |
|------|----------|-----|
| Multi-request correctness (hermetic afwfcgi) | `src/afw/tests/advanced/catalog-value-accessors/` | `afwdev test --test-pattern catalog-value-accessors` |
| Load thrash (not in `-j`) | `src/afw/tests_special/catalog/` | `afwdev blast -T src/afw/tests_special/catalog -d 15s -m 50` |

---

## 4. Runtime object maps and create instance

### 4.1 Core structs (`afw_runtime.h`)

```text
afw_runtime_object_type_meta_t
  object_type_id
  property_map → afw_runtime_object_map_t
                  property_count
                  properties[] → name, offset, count_offset, data_type,
                                 accessor_name, accessor fn
  properties_offset   // optional NULL-terminated afw_runtime_property_t **
  indirect            // true → afw_runtime_object_indirect_t layout

afw_runtime_object_indirect_t
  afw_object_t pub    // inf → meta in rti.implementation_specific
  void *internal      // live C struct
```

`AFW_RUNTIME_OBJECT_INF(inf_name, meta)` wires the shared runtime object method table to that meta.

### 4.2 Get property path

`afw_runtime_object_get_property` → find map entry by name → `impl_make_value_from_map_entry`:

- **offset == -1** (`onGetValueCFunctionName`): call `accessor(prop, internal, p, xctx)` with the whole internal pointer.
- **else**: call `accessor(prop, internal + offset, p, xctx)`.

Then optional **unmapped** properties from `properties_offset` (NULL-terminated `afw_runtime_property_t` name/value pairs)—used heavily by const instances.

`get_setter` is NULL. `release` / `get_reference` on mapped runtime objects are currently no-ops / TODOs—lifetime is usually “env owns the internal struct” or “const forever.”

### 4.3 Creating instances

| API | Use |
|-----|-----|
| `afw_runtime_object_create_indirect(type, id, internal, p, xctx)` | Shell over live struct using registered map inf |
| `afw_runtime_object_create_indirect_using_inf(inf, …)` | Same when inf is already known |
| `afw_runtime_env_create_and_set_indirect_object*` | Create in **env pool** and insert into env runtime object hash |
| `afw_runtime_env_set_object` / `set_objects` | Register an existing object pointer (const or custom impl) |
| `afw_runtime_env_set_object_cb_wrapper` | Lazy object: hash stores callback + data; materialize on get |
| `afw_runtime_xctx_set_object*` | Per-xctx overlay of runtime objects |
| `afw_runtime_get_object` / `afw_runtime_foreach` | Lookup / iterate registered objects of a type |
| `afw_runtime_remove_object` | Drop a key (e.g. unregister) |

Maps are registered with `afw_runtime_register_object_map_infs` → env registry type **`runtime_object_map_inf`**.

### 4.4 Const objects

`const_objects.py` emits `afw_runtime_const_object_instance_t` graphs (NULL-terminated property arrays of permanent values). Registration via `afw_const_objects_register` / package equivalents sets them into the same runtime object hash as mapped instances. Property get walks the property list (no offsetof into a live mutable struct). Cheap to expose; process lifetime.

---

## 5. Environment registration surface

### 5.1 Registry types

`AFW_ENVIRONMENT_REGISTRY_TYPE_MAP` in `environment/afw_environment.h` defines core kinds: adapters, functions, data types, locks, logs, services, runtime maps/accessors/custom, content types, etc.

Each kind has:

- Registry type id (C enum; note historical typo `afw_environemnt_registry_type_*`)
- **`property_name`** — property on `_AdaptiveEnvironmentRegistry_/current` (e.g. `function`, `adapterId`)
- **`object_type_id`** — Adaptive type used when viewing entries as runtime objects
- **`register_additional`** callback — side effects when a key is registered
- **`allow_reregister`**

Extensions can add kinds with `afw_environment_create_registry_type()`.

### 5.2 Register → runtime object coupling

When something is registered into the env, `register_additional` often **also** installs a runtime object so `/afw/<ObjectType>/<key>` works:

| Callback (in `afw_environment.c`) | Behavior |
|-----------------------------------|----------|
| **`impl_internal_additional_register_default`** | `afw_runtime_env_create_and_set_indirect_object(object_type_id, key, value)` — value is the registered C pointer (adapter anchor, lock, **value accessor info**, …) |
| **`impl_internal_additional_register_key_only`** | Alloc tiny struct holding key pointer; create indirect object (singletons, content types, value_inf, …) under lock |
| **`impl_internal_additional_register_object`** | `afw_runtime_env_set_object` — value **is** an Adaptive object (e.g. context types) |
| **NULL** (e.g. function, data_type, flag) | Handled specially inside `register_function` / data type paths: set prebuilt `function->object` / `data_type->object` |

Many register paths take **`environment_lock`** while mutating hashes. That is **not** held across an entire HTTP request or full registry serialize.

### 5.3 Locks that already exist (inventory seed)

Created at environment create (non-exhaustive):

| Lock | Typical use |
|------|-------------|
| `environment_lock` | Registry register/get mutations |
| `adapter_id_anchor_lock` | Adapter anchors / stopping list (used by stopping accessor) |
| `authorization_handler_id_anchor_rw_lock` | Auth handler anchors / stopping |
| `active_log_list_lock` | Active logs |
| `flags_lock` | Flags |
| `multithreaded_pool_lock` | Nested pool / MT pool |

#149 work should **inventory and reuse** these (and any service/adapter-local locks) rather than inventing a parallel locking story.

### 5.4 Runtime objects without registry entries

Possible patterns:

- **Const metadata** registered only into the runtime object hash (object types, collections).
- **Custom object implementations** set with `afw_runtime_env_set_object` (e.g. process ambient objects).
- **Unregistered shells**: `afw_runtime_object_create_indirect` can create a view over a struct for a request pool **without** env hash insertion (caller holds the pointer).
- **Callback wrappers** for lazy materialization.

### 5.5 Non–runtime-map objects (examples)

Not everything under `/afw/...` is an offsetof map:

| Example | Implementation | How it is registered |
|---------|----------------|----------------------|
| **`_AdaptiveEnvironmentRegistry_/current`** | Hand `afw_object` impl in `afw_environment_registry_object.c` | Permanent static shell; not a map of a C struct |
| **`_AdaptiveEnvironmentVariables_`** | Hand impl `afw_environment_variables_object.c` (lazy environ cache) | `afw_runtime_env_set_object` at create |
| **`_AdaptiveService_`** | **`runtime_custom`** retrieve/get | `afw_environment_register_runtime_custom` — not pure hash iteration of map shells |
| **Context types** | Real Adaptive objects | `register_object` additional |
| **Model `current::` qualifiers** | Map with `onGetValueCFunctionName` only | Callbacks read thread/xctx model state |

`runtime_custom` is the extension point when get/retrieve cannot be “iterate env hash of prebuilt objects.”

---

## 6. Value accessors and when to lock/copy

### 6.1 Where correctness lives

Care is usually **when property values are obtained** (accessors), not cloning whole objects.

```text
get_property / get_next_property
  → map entry
  → value accessor(prop, internal±offset, p, xctx)
       → may lock
       → may copy into p (often request/xctx pool)
       → returns const afw_value_t *
```

Whole objects are often:

- static const, or  
- long-lived indirect shells pointing at env-pool structs, or  
- request-pool materializations that **contain** live pointers from accessors.

### 6.2 What `default` actually snapshots

`afw_runtime_value_accessor_default` uses `afw_value_common_create`:

- Scalars: copy of the data-type internal representation into a value on `p`.
- **`afw_utf8_t`**: copies the **struct** (pointer + length). The UTF-8 **bytes** remain wherever `s` pointed (often permanent or env-owned).
- Pointer-ish cTypes: may return NULL if pointer is NULL; otherwise copy pointer bits into the value.

So “default” is **not** a deep clone of string storage or nested objects.

### 6.3 When lock + selective copy is already required

Clear pattern in tree: **stopping instance refcount arrays** for adapters and authorization handlers—comment in code: *“A copy is required since it may change by a different thread.”* Lock → walk list → allocate integer array on `p` → return.

Other candidates for the #149 checklist (not a mandate that all need changes):

- Metrics that mutate while read
- Service status/startTime while start/stop races
- Any pointer into memory that can be **freed** while a request still holds the Adaptive value (adapter destroy after use count 0, log teardown, …)
- Arrays of pointers into transient tables

### 6.4 What usually does **not** need clone-on-get

- Static const graphs from generate
- Built-in function definitions and their permanent string values
- Extension-loaded functions (typically never unloaded)
- Object type / data type permanent metadata
- Adapter fields that are fixed for the life of the anchor while use count > 0, if the request cannot outlive that guarantee (study carefully)

---

## 7. Const vs mapped vs composed shells

Three common shapes (and mixtures):

### 7.1 Const / permanent

- Generated `const_objects` or permanent value wrappers.
- Property values are already Adaptive values; little per-get work.
- Process lifetime; safe to share pointers widely.

### 7.2 Mapped (indirect or embedded)

- Inf from `runtime_object_maps`.
- `get_property` runs accessors against a C struct.
- Instance may live in env pool (registered) or any pool (ephemeral view).
- Immutable as Adaptive object; underlying C may still change between gets.

### 7.3 Composed shells — `_AdaptiveEnvironmentRegistry_/current`

File: `environment/afw_environment_registry_object.c`.

```text
Permanent static object
  path: /afw/_AdaptiveEnvironmentRegistry_/current
  get_property(registryTypePropertyName)
    → resolve registry type by property name
    → afw_object_create(xctx->p)          // request/xctx pool bag
    → afw_runtime_foreach(object_type_id) // each registered entry
    → set property objectId → object
    → return unmanaged object value
```

Implications:

- The **shell** is permanent and cheap.
- Each property get **materializes** that registry type’s entries into a **new** request-pool object.
- Full walk / JSON encode of `current` expands **all** types (functions are huge).
- Same data is available as typed retrieve (`/afw/_AdaptiveFunction_/…`, etc.) without building the aggregation shell.
- Nested entries are still normal runtime objects (const or mapped); the shell does not deep-clone their property payloads.

This is the right mental model for admin “big object” behavior: **composition + materialize-on-get**, not one giant static bag of the whole process.

### 7.4 Custom retrieve/get

`_AdaptiveService_` (and layout component types, etc.) register `afw_runtime_custom_t` so session retrieve/get call specialized functions instead of pure hash dump.

---

## 8. Extension and command registration patterns

| Actor | Pattern |
|-------|---------|
| **Core** | `afw_generated_register` + `afw_environment_internal_register_core` |
| **Extension DSO** | `AFW_ENVIRONMENT_DEFINE_EXTENSION_IMPL()`; `initialize` → `<prefix>_generated_register` (maps, const, functions) + hand `register_*` |
| **Command** | Create env; command generated register; not a DSO |
| **Lazy extension** | Manifest `registers` → load on first miss |

After register, extension-provided functions/adapters/types appear in the **same** registries and the **same** runtime object namespace as core. Maps from extension OTs with `runtime` typedefs work identically if the extension generates and registers its maps and accessors.

When adding a new env-visible type:

1. Define OT metadata (and `runtime` map if viewing a C struct).
2. Generate / implement accessors if non-default.
3. Register map infs.
4. Choose registry type + `register_additional` so keys appear under `/afw/...` and, if desired, under EnvironmentRegistry property names.
5. Think through lifetime: permanent, env-stable, adapter-refcount, or needs lock+copy.

---

## 9. Product context (admin “big get”) — not the fix plan

- One consumer prefers full **`/afw/_AdaptiveEnvironmentRegistry_/current`**.
- Another prefers **retrieve individual types** (especially functions).
- That preference is **context** for cost awareness only.
- **Do not** treat “document that clients should fetch less” as the #149 solution path.
- **Do not** redesign the admin SPA as phase 1 of this work.
- **Do not** merge #127 progressive release or full #2 into a #149 PR by default.
- #2 helps long-running hosts under memory pressure; **#149 is correctness of values from accessors / per-type lifetime study**.

Discovery-era notes on materialize cost remain in [`runtime-catalog-lifetime.md`](runtime-catalog-lifetime.md).

---

## 10. #149 classification checklist (per type / property)

Use this when auditing a runtime-visible type. Goal: **surgical** lock/copy at accessors where needed—not global clone-all.

### 10.1 Classify the object shell

| Class | Questions | Typical action |
|-------|-----------|----------------|
| **A. Static const** | Generated const graph? Permanent values only? | No accessor clone; leave alone |
| **B. Env-stable mapped** | Struct lives in env pool for process life? Fields only grow/append? | Usually live pointers OK; note mid-update multi-get inconsistency is accepted |
| **C. Refcount-gated** | Adapter/log/service instance freed when use count → 0? | Ensure request cannot hold dangling internals after destroy; may need get_reference on internals or snapshot at accessor |
| **D. Custom / composed** | EnvironmentRegistry, custom retrieve, cb wrapper, hand object impl? | Audit materialize path and nested objects separately |
| **E. Unregistered ephemeral** | `create_indirect` only for local use? | Caller owns pool lifetime |

### 10.2 Per-property questions

1. What is the **storage** of the Adaptive value after get (scalar copy, utf8 pointer, nested object pointer, array view)?
2. Can that storage **disappear or move** while a request still holds the value?
3. Can another thread **mutate** it mid-read such that a torn value is unsafe (not merely slightly stale)?
4. Is there an **existing lock** that already protects this state?
5. If copy is needed: copy **only that property’s payload** into `p` (request/xctx pool)—not the whole object / whole registry.
6. Does the accessor already lock+copy (stopping lists)? Treat as reference implementation.
7. Is this path on the **hot full-registry expand**? Prefer correctness first; avoid new O(registry) clones.

### 10.3 Suggested audit order (core)

High signal for #149 (adjust as inventory proceeds):

1. Accessors that already take locks (stopping_*) — confirm completeness.
2. `_AdaptiveAdapter_` / metrics / referenceCount / properties.
3. `_AdaptiveService_` custom path + status fields.
4. Authorization handler anchors (mirror adapter stopping pattern).
5. Logs, request handlers, anything unloadable.
6. Functions / data types / object types — expect **mostly class A/B** (document as stable).
7. EnvironmentRegistry `current` — composition only; push issues down into entry types.
8. Extension-specific runtime maps (ldap, lmdb, …) on a later pass.

### 10.4 Out of scope for architecture v1 / early #149 phases

- Admin SPA rewrite
- Making full registry expand “cheap”
- Global clone-on-get
- Deep #127 progressive release design
- Full #2 managed-value escape program
- Treating #17 faces as related blockers

---

## 11. Key file map

| Path | Role |
|------|------|
| `src/afw/generate/objects/_AdaptiveObjectType_/*.json` | OT metadata + `runtime` maps |
| `src/afw_dev/_afwdev/generate/runtime_object_maps.py` | Map / inf generation |
| `src/afw_dev/_afwdev/generate/const_objects.py` | Const object generation |
| `src/afw/generated/afw_runtime_object_maps.*` | Generated maps (do not hand-edit) |
| `src/afw/runtime/afw_runtime.h` / `.c` | Create, register, get, foreach, adapter session |
| `src/afw/runtime/afw_runtime_value_accessor.h` / `.c` | Accessors + lock/copy examples |
| `src/afw/environment/afw_environment.h` | Registry type map + public register/get |
| `src/afw/environment/afw_environment.c` | Register, locks, register_additional |
| `src/afw/environment/afw_environment_register_core.c` | Bootstrap order |
| `src/afw/environment/afw_environment_registry_object.c` | `EnvironmentRegistry/current` shell |
| `src/afw/environment/afw_environment_variables_object.c` | Non-map ambient example |
| `.cursor/rules/afw-environment.mdc` | Always-on env registration rule |

---

## 12. Working plan for the #149 branch

1. ~~Deep dive / architecture MD (this file).~~  
2. Inventory risk types with §10 checklist (notes can live in [`runtime-catalog-lifetime.md`](runtime-catalog-lifetime.md) or issue comments).  
3. Surgical accessor fixes only where dangling or unsafe concurrent read is real.  
4. PR to `mgg-develop` with clear **phase N** scope—no “fix catalogs” mega-PR.

When decisions stabilize, promote invariants into `.cursor/rules` or developer docs and thin the pads.

---

## 13. Session addenda (2026-08) — hooks that affect inventory

### 13.1 Runtime objects need not be registered

`afw_runtime_object_create_indirect` (and friends) can build a mapped shell **without** `afw_runtime_env_set_object`. Registry publish is how most catalog instances become `/afw/…` discoverable; it is not the definition of “runtime object.”

Callers only hold `const afw_object_t *` and use the **object inf**. They do not know const vs mapped runtime vs memory vs face vs env-vars impl. Content types only serialize values.

### 13.2 Const generate objects

`const_objects` emit static graphs in the DSO. Retrieve/get walks the compiled property table and encodes via content type (usually JSON; progressive **application/x-afw** frames for stream retrieves). No live struct; class **A** for #149.

### 13.3 Object type inheritance — do not scrape generate/ alone

Many OT JSON files store **deltas**. Inheritance is declared with **`parentPaths`** (often `propertyTypes._meta_.parentPaths`, e.g. conf subtypes → parent conf OT).

**Effective** property set requires resolution:

- Live: `get_object(…, options: { composite: true, … })`
- Code: `afw_object_view_create` after adapter fetch (`afw_adapter_internal_process_object_from_adapter`)

Example: `_AdaptiveConf_adapter_file` ~7 local propertyTypes raw; **~16** with `composite`. Meta-schema: `/afw/_AdaptiveObjectType_/_AdaptiveObjectType_`. Option vocabulary: `_AdaptiveObjectOptions_`.

Views reshape presentation; **#149 still reasons about the base object + accessors** (views do not always deep-copy property values).

### 13.4 Adapter active / stopping / refcount (P0 for accessors)

`afw_adapter_id_anchor_t` (lock: `adapter_id_anchor_lock`):

- **Active** instance accepts new `get_reference` traffic.
- On replace/stop: old anchor copy chained on **`stopping`**; new active installed; old drains until **reference_count → 0** then destroy.
- Runtime OT `_AdaptiveAdapter_` maps the anchor. Accessors **`stopping_adapter_instances`** and **`adapter_reference_count`** **lock + copy** into the request pool. **`metrics` / `properties`** remain **live while active** (documented on the OT; product choice — not full-clone on get). Parallel pattern for authorization handlers (`authorization_handler_reference_count`, stopping_*).

Service control for probes: `service_get` / `service_start` / `service_stop` / `service_restart`. Prefer **files / vfs / lmdb** (etc.). Do **not** stop permanent **adapter-afw** / **adapter-conf** (can strand the host).

### 13.5 Progressive retrieve (`application/x-afw`)

`retrieve_objects_to_response` requires response content type **application/x-afw**. Body frames:

```text
<sequence> <size> <streamId> [info]\n
<size bytes of payload>
```

Client: `AfwStreams` (JS). Server flush uses `FCGX_FFlush`; HTTP chunked/nginx/JS may still delay end-to-end delivery—custom framing is the reliable semantic unit (related to #127 free-as-you-go, not a substitute for #149 accessor study).

### 13.6 Product framing (adapters)

Adapter types normalize **object stores** behind one object API (provisioning). Model adapter maps logical ↔ physical. Long-running scripts act as **peers** on that model. The `afw` runtime adapter is one type among many: process registries as objects.

### 13.7 Inventory plan reminder

1. Classify mapped properties by accessor semantics (prefer fixing **named accessors** once).  
2. Use **composite** OT views when listing “all properties” of a type; use **maps + accessor C** for lifetime.  
3. P0 adapter/auth anchors with stop-start probes.  
4. Surgical lock+copy only where unsafe; document intentional live/refcount contracts where correct.

---

## 14. Runtime value-accessor inventory (working — 2026-08-08)

**Status:** analysis + live probes; **no code changes committed.**  
**Method:** OT generate `runtime` props + `src/afw/generated/afw_runtime_object_maps.c` + `afw_runtime_value_accessor.c`.  
**Raw dump:** regenerate with a small Python walk of `_AdaptiveObjectType_/*.json` (agent used `/tmp/runtime_accessor_inventory.json`).

### 14.1 Core named accessors — copy vs point

| Accessor | Lock? | Semantics (get result) | Class | Notes |
|----------|-------|------------------------|-------|--------|
| **default** | no | `afw_value_common_create`: memcpy of data-type internal size into value on `p` | **S** for scalars; **L** for pointer cTypes | `afw_utf8_t` copies struct only — **bytes still at `s`**. Object/array cTypes copy the **pointer**. |
| **indirect** | no | deref once → default | same as default on pointee | NULL → NULL |
| **value** | no | returns `*(const afw_value_t **)` as-is | **P/E** if value permanent; else **L** | Used heavily on function defs (permanent) |
| **size** / **octet** / **uint32** | no* | integer snapshot on `p` | **S** | Widening numeric; *octet path may share nearby lock macros—treat as snapshot |
| **compile_type** | no | permanent name string for enum | **P** | |
| **data_type_id** | no | string of `data_type->data_type_id` on `p` (utf8 struct points at permanent id) | **P** | iteratorReturnDataType path |
| **service_startup** / **service_status** | no | memcpy enum → permanent utf8 name string | **S** (enum) + **P** (name) | |
| **stopping_adapter_instances** | **yes** `adapter_id_anchor_lock` | count + copy refcounts into array on `p` | **S** | **Reference implementation for #149** |
| **stopping_authorization_handler_instances** | **yes** rw lock | same pattern | **S** | |
| **adapter_metrics** | no | unmanaged object value wrapping `adapter->impl->metrics_object` | **R/L** | Pointer into instance; OK while instance refcount holds; dangling after destroy |
| **adapter_additional_metrics** | no | may call adapter for extra metrics object on `p` | **R** / varies | |
| **applicable_flags** | no | builds array of flag id values on `p` | **S** (array); flag ids **P** | |
| **null_terminated_array_of_*** | no | builds array view/wrappers on `p` | often **S** shell + **P** element ptrs | Function metadata tables (permanent) |
| **ensure_afw_components_extension_loaded** | no | side-effect load + true | n/a | Not a data lifetime issue |

**Classes:** **P** permanent · **E** env-stable · **S** snapshot on get · **L** live pointer · **R** refcount-coupled instance.

### 14.2 Usage frequency (core OT generate, approximate)

| Accessor | # properties (approx) |
|----------|----------------------|
| indirect | 42 |
| value | 34 |
| default (explicit or implicit memberName) | 30+ |
| size / null_terminated_* / specialized | few each |
| stopping_* / adapter_metrics / adapter_additional_metrics | 1 each |

Plus **18** `onGetValueCFunctionName` model `current::` callbacks (xctx model state — separate from env catalog lifetime).

### 14.3 P0: `_AdaptiveAdapter_` (`afw_adapter_id_anchor_t`)

| Property | Accessor | Class | Assessment |
|----------|----------|-------|------------|
| adapterId | indirect | **E** | id in env/anchor; stable for process life of id registration |
| serviceId | indirect | **E** | same |
| referenceCount | **adapter_reference_count** | **P (snapshot)** | lock + copy integer under `adapter_id_anchor_lock` |
| properties | default (object *) | **R/L** | copies **pointer** to properties object into value; object lives with adapter/conf lifetime; **NULL when fully stopped** |
| metrics | adapter_metrics | **R/L** | pointer to metrics runtime object on instance; **NULL when no active adapter** |
| stopping | stopping_adapter_instances | **S** | lock+copy; empty/absent when no draining instances |

**Live probes (full AFWDev afwfcgi, 2026-08-08):**

- `service_restart(adapter-files)`: immediate get shows healthy active, `stopping` null (no concurrent holder → drain finished before get).
- `service_stop(adapter-vfs)` then get: **success**, `referenceCount: 0`, **`metrics: null`**, `stopping` null (fully stopped, not mid-drain). `service_start` restores ref=1.
- Concurrent mid-drain `stopping[]` non-empty needs a **second request** holding adapter ref/session across stop (single-request eval never sees the chain).
- Single `_AdaptiveAdapter_/files` with metaFull+normalize+objectTypes: **success** (~20 KiB); rich options fail on permanent NULL-pool trees (EnvironmentRegistry `current`), not all runtime objects.

**Auth handler** mirror: same pattern on `_AdaptiveAuthorizationHandler_` + stopping_* accessor.

### 14.4 Safer / lower priority (sample)

| Area | Why lower |
|------|-----------|
| `_AdaptiveFunction_`, `_AdaptiveDataType_`, most **value**/indirect on generate defs | Permanent / never unloaded in normal hosts |
| Const OT / collection graphs | Static const |
| Flag id strings, data_type_id | Permanent |
| Model onGet current::* | Request/xctx scoped model state, not env stop/start |

| Area | Why later |
|------|-----------|
| `_AdaptiveService_` | **runtime_custom** path (no map typedef); status can change; inventory custom get separately |
| `_AdaptiveLog_` | service-coupled; stoppable |
| Extension-loaded maps | After core P0 |

### 14.5 Candidate fixes (not implemented this session)

1. **referenceCount (adapter + auth):** done — `adapter_reference_count` / `authorization_handler_reference_count` (lock + copy). Metrics/properties remain live-while-active + OT prose.  
2. **metrics / properties:** either (a) document **R**: only valid while active or while you hold a session ref on that instance; or (b) snapshot needed fields under lock into request pool when returning from runtime get. Prefer (a) if product accepts; (b) if dangling after destroy is proven.  
3. **Keep** stopping_* as the gold standard; extend pattern only where inventory proves need.  
4. Optional: hold session + stop to **test** non-empty `stopping` array and metrics identity across restart.

### 14.6 Inventory methodology notes

- Props without a `runtime` key but on an OT with top-level `runtime` still get map entries (**default** accessor, memberName = property name) — e.g. `_AdaptiveAdapter_.properties`.  
- Do **not** use generate/ alone for full OT property sets (use composite views for documentation completeness).  
- Prefer live **OT get** or instance get with options (`metaFull`, `objectTypes`, …) for property meta including `runtime.valueAccessor`.  
- For **lifetime**, the **generated map + accessor** is authoritative.

### 14.7 Bug: rich objectOptions on EnvironmentRegistry/`current` (2026-08-08)

**Symptom:** `get_object(afw, _AdaptiveEnvironmentRegistry_, current, options: …)` with certain option sets fails; afwfcgi stays up.

| Options | Error |
|---------|--------|
| bare / `metaFull` alone / identity flags | **success** (~1.3–1.4 MiB) |
| `metaFull` + `normalize` + path/ids | **`Object must have a pool`** |
| `metaFull` + `objectTypes` + … | **`Object must have a pool`** |
| kitchen sink incl. `composite` + `inheritedFrom` + defaults | **`Object immutable`** |

**Root cause (backtrace):**

1. `afw_object_view_create` → `impl_additional_object_option_processing` / normalize path.  
2. Tries to annotate meta (`impl_meta_set_property_type_property`, or on catch `afw_object_meta_add_property_error` → `afw_object_set_property_as_array`).  
3. **`EnvironmentRegistry/current` is a static const object with `p == NULL`** (`afw_environment_registry_object.c`: `impl_current_object.p = NULL`). Nested values include other permanent/immutable runtime/const objects.  
4. `afw_object_set_property_as_array` / `afw_object_set_property` require a **pool** and/or **mutable** meta — permanent objects throw.

**Why it may feel new:** #2-era tightening of managed/unmanaged create paths and stricter “must have a pool” checks on `set_property_as_*` (generated data-type bindings) make meta mutation fail loudly; older code may have been laxer or admin may have stopped passing these options on the big get.

**Product note:** Jeremy’s app loads `current` with **`modelOptions.adaptiveObject: false` and no objectOptions** — so UI avoids this. Object viewer uses rich options on **individual** objects (composite/normalize/…), which often have pooled views. Fixing meta annotation for permanent objects would still be correct for API completeness.

**Fix direction (not implemented yet):** ensure option processing only mutates **view-owned pooled meta** (never the underlying permanent object’s meta); when recording property errors, use `view->p` / meta objects created in the view pool; skip or soft-fail meta mutation for `!object->p` / immutable bases. Related to #149 only insofar as views + immutable runtime bases interact.

**Tests to add when fixing:** get `current` with normalize+metaFull+path; with composite+inheritedFrom; expect success (or documented subset of options), not throw.

### 14.8 Decade-old problem: `object->p` for “stuff that dies with the object”

**Intent of `object->p`:** many APIs allocate ephemeral/meta/string clones into the **object’s pool** so they go away when that pool is released (memory objects, request-scoped objects). Right default for **pooled** objects.

**Const / permanent objects:** `p == NULL` by design (static in the DSO). Nothing to free when “the object goes away.” Call sites that still do `create_*(…, instance->p)` or `set_property_as_*(instance, …)` either throw (**Object must have a pool** after #2-era checks), hit **Object immutable**, or used to hide behind a fallback.

**Past experiment (Mike):** helper along the lines of “use `object->p` if non-NULL else `xctx->p`.” Unblocks const objects but **hides lifetime bugs** (request-pool alloc while assuming object lifetime). May have been **removed on purpose** to surface call sites that need real policy — which is what we’re seeing again on EnvironmentRegistry + options.

**Existing partial guards:**

- `IMPL_ASSERT_META_MUTABLE` in `afw_object_meta.c`: cannot set meta on const (`!instance->p`).
- `afw_object_meta_get_property_type`: early `return NULL` if `!meta_object && !p` — comment: *“This is here for const objects for now.”*
- View create uses its **own subpool** (`view->p`) for the view shell, but option processing still walks **nested** permanent objects and tries to write **their** meta using **their** `p`.

**Ways to deal (no one-size; scratch later):**

| Approach | Pros | Cons |
|----------|------|------|
| **A. Explicit `p` on every API** | Clear ownership | Wide API churn |
| **B. View-only mutation** | Options never write base meta | All meta writes must go through view helpers |
| **C. Lazy meta clone into view/request pool** when base is const | Const stays pure; options work | Cost; meta object identity |
| **D. Soft no-op** when `!p` / immutable | No crash | Silent incomplete meta |
| **E. Scoped “option processing pool” on xctx** for one get tree | One allocation arena | Easy to misuse / leak across requests |
| **F. Global `object->p ?: xctx->p`** | Simple | Masks bugs (why abandoned) |

**Likely for EnvironmentRegistry + options:** **B+C** — option processing is a **view** job; permanent shell never grows a pool. Broader meta.c uses of `instance->p` need inventory: skip for const, or allocate in **caller/view** pool.

**Adjacent to #149 / #2:** same theme as accessor copy-vs-point — **where does this value live**, when the producer is permanent vs request-scoped.

**Object wrappers / faces (#17 and views):** a pooled **wrapper** over a permanent/const base is a natural place to hang request- or view-scoped meta and option side-effects without giving the base a fake pool. `afw_object_view` already aims at “reshape without always cloning the store”; mutable faces hang mutation off the face. Either (or both) may be part of solving const-`p` meta writes — keep in mind when designing B/C above, and when #17 face lifetime meets #2/#149.