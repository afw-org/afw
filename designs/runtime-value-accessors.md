# Runtime value accessors (live catalog snapshot)

**Audience:** maintainers / AI assistants. **Not** handbook.
**Related:** [#149](https://github.com/afw-org/afw/issues/149), [`runtime-objects-and-environment.md`](runtime-objects-and-environment.md) §3.3.

## Purpose

Consumable view of registered **`_AdaptiveRuntimeValueAccessor_`** entries:
brief, description, and lifetime flags (`copiesUnderLock`, `returnsLiveReference`).

**Source of truth** is the running environment (and the co-located
`afw_runtime_value_accessor_info_t` next to each function in
`src/afw/runtime/afw_runtime_value_accessor.c`). This file is a **snapshot** for
reading in context without hunting C or firing afwfcgi every time.

Extensions may register more accessors; re-run the refresh command after
loading those packages if you need their entries here.

## Refresh this file

After changing accessors (or to re-snapshot core):

```bash
# From package root; maxObjects 0 = unlimited (do not omit on larger catalogs)
afw -x 'retrieve_objects("afw", "_AdaptiveRuntimeValueAccessor_", {}, undefined, undefined, 0)'
```

Service start lines go to stderr; JSON array is stdout. Sort by `key` when
editing the tables below. Property meaning:

| Property | Meaning |
|----------|---------|
| `key` | Registry name; use as OT `propertyTypes.*.runtime.valueAccessor` |
| `brief` | Short label for lists / choose-an-accessor |
| `description` | Internal shape, locks, result lifetime |
| `copiesUnderLock` | Locks (or equivalent) and copies into caller pool — stable snapshot |
| `returnsLiveReference` | Adaptive value may alias live env/instance state |

**Snapshot date:** 2026-08-08  
**Core entry count:** 23

## Summary table

| key | brief | copiesUnderLock | returnsLiveReference |
|-----|-------|-----------------|----------------------|
| `adapter_additional_metrics` | Call adapter get_additional_metrics() | no | no |
| `adapter_metrics` | Return live adapter metrics object | no | yes |
| `adapter_reference_count` | Snapshot adapter anchor reference_count under lock | yes | no |
| `afw_components_extension_loaded` | Ensure afw_components extension is loaded | no | no |
| `applicable_flags` | Build array of applicable flag ids for a flag | no | no |
| `authorization_handler_reference_count` | Snapshot authorization handler anchor reference_count under lock | yes | no |
| `compile_type` | Map afw_compile_type_t to its name string | no | no |
| `data_type_id` | Map const afw_data_type_t * to dataType id string | no | no |
| `default` | Map a struct member as its Adaptive data type | no | yes |
| `indirect` | Like default, but member is a pointer to the value | no | yes |
| `null_terminated_array_of_internal` | Map NULL-terminated array of value internals | no | no |
| `null_terminated_array_of_objects` | Map NULL-terminated array of object pointers | no | yes |
| `null_terminated_array_of_pointers` | Map NULL-terminated array of pointers to values | no | no |
| `null_terminated_array_of_utf8_z_key_value_pair_objects` | Map NULL-terminated utf8_z key/value pair tables to objects | no | no |
| `null_terminated_array_of_values` | Map NULL-terminated array of afw_value_t * | no | yes |
| `octet` | Map afw_octet_t member as integer | no | no |
| `service_startup` | Map afw_service_startup_t to status string | no | no |
| `service_status` | Map afw_service_status_t to status string | no | no |
| `size` | Map afw_size_t member as integer | no | no |
| `stopping_adapter_instances` | Snapshot stopping adapter instance reference counts | yes | no |
| `stopping_authorization_handler_instances` | Snapshot stopping authorization handler instance reference counts | yes | no |
| `uint32` | Map afw_uint32_t member as integer | no | no |
| `value` | Return afw_value_t * member as-is | no | yes |

### By lifetime class (quick filter)

- **copiesUnderLock:** `adapter_reference_count`, `authorization_handler_reference_count`, `stopping_adapter_instances`, `stopping_authorization_handler_instances`
- **returnsLiveReference:** `adapter_metrics`, `default`, `indirect`, `null_terminated_array_of_objects`, `null_terminated_array_of_values`, `value`
- **Neither (typically scalar/copy into `p`):** `adapter_additional_metrics`, `afw_components_extension_loaded`, `applicable_flags`, `compile_type`, `data_type_id`, `null_terminated_array_of_internal`, `null_terminated_array_of_pointers`, `null_terminated_array_of_utf8_z_key_value_pair_objects`, `octet`, `service_startup`, `service_status`, `size`, `uint32`

## Full descriptions

### `adapter_additional_metrics`

- **Brief:** Call adapter get_additional_metrics()
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `false`
- **Description:** internal points at afw_adapter_impl_t. Calls afw_adapter_get_additional_metrics() with the adapter. Returned object lifetime follows that API (typically allocated in p).

### `adapter_metrics`

- **Brief:** Return live adapter metrics object
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `true`
- **Description:** internal is a pointer to const afw_adapter_t * on an anchor. Under `adapter_id_anchor_lock`, loads `metrics_object` and wraps it after the lock (no deep copy). Live environment state (`returnsLiveReference`). The lock makes the pointer load safe, not the object’s life after unlock — do not cache across stop/replace unless a session (or other) reference keeps the instance. Same leftover as `adapter_properties`.

### `adapter_reference_count`

- **Brief:** Snapshot adapter anchor reference_count under lock
- **copiesUnderLock:** `true`
- **returnsLiveReference:** `false`
- **Description:** internal points to afw_integer_t reference_count on an afw_adapter_id_anchor_t. Under adapter_id_anchor_lock, copies the integer into a value in p. Mutators of the anchor use the same lock.

### `afw_components_extension_loaded`

- **Brief:** Ensure afw_components extension is loaded
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `false`
- **Description:** Side-effect accessor: loads the afw_components extension if needed and returns boolean true. Key is afw_components_extension_loaded.

### `applicable_flags`

- **Brief:** Build array of applicable flag ids for a flag
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `false`
- **Description:** internal points at an afw_flag_t (typically offset of flag_id at start of struct). Builds a new array in p of flag id values for each applicable flag bit. Array contents are permanent flag id strings; the array itself is allocated in p.

### `authorization_handler_reference_count`

- **Brief:** Snapshot authorization handler anchor reference_count under lock
- **copiesUnderLock:** `true`
- **returnsLiveReference:** `false`
- **Description:** internal points to afw_integer_t reference_count on an afw_authorization_handler_id_anchor_t. Under authorization_handler_id_anchor_rw_lock (read), copies the integer into a value in p.

### `compile_type`

- **Brief:** Map afw_compile_type_t to its name string
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `false`
- **Description:** internal is a pointer to afw_compile_type_t (stored as afw_octet_t). Returns the permanent name string value for that compile type.

### `data_type_id`

- **Brief:** Map const afw_data_type_t * to dataType id string
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `false`
- **Description:** internal is a pointer to const afw_data_type_t *. Returns a string value of data_type->data_type_id, or NULL if the pointer is NULL.

### `default`

- **Brief:** Map a struct member as its Adaptive data type
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `true`
- **Description:** Default accessor. internal points at the member storage for prop->data_type. Builds an Adaptive value via afw_value_common_create. Returns NULL for NULL pointer cTypes and zero-length afw_utf8_t / afw_memory_t. The value may alias the live member storage (returnsLiveReference).

### `indirect`

- **Brief:** Like default, but member is a pointer to the value
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `true`
- **Description:** Like default, but internal points to a pointer to the storage used as the Adaptive value internal (e.g. afw_utf8_t ** instead of afw_utf8_t *). Result may alias that live storage.

### `null_terminated_array_of_internal`

- **Brief:** Map NULL-terminated array of value internals
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `false`
- **Description:** internal points to a pointer to a NULL-terminated C array of internals of the array dataTypeParameter type. Builds a view then clones into p. dataType must be array.

### `null_terminated_array_of_objects`

- **Brief:** Map NULL-terminated array of object pointers
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `true`
- **Description:** internal points to const afw_object_t * const * (NULL-terminated). Returns an array value of those objects. Object identities are live references.

### `null_terminated_array_of_pointers`

- **Brief:** Map NULL-terminated array of pointers to values
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `false`
- **Description:** internal points to a pointer to a NULL-terminated array of pointers to internals of the array dataTypeParameter type. Builds a view then clones into p.

### `null_terminated_array_of_utf8_z_key_value_pair_objects`

- **Brief:** Map NULL-terminated utf8_z key/value pair tables to objects
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `false`
- **Description:** internal points to const afw_utf8_z_t *[] groups of key/value pairs (each object ends with NULL; list ends with extra NULL). Builds new objects and an array in p.

### `null_terminated_array_of_values`

- **Brief:** Map NULL-terminated array of afw_value_t *
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `true`
- **Description:** internal points to a NULL-terminated array of const afw_value_t *. Returns an array value of those values (live value pointers).

### `octet`

- **Brief:** Map afw_octet_t member as integer
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `false`
- **Description:** internal points to an afw_octet_t. Returns an integer value in the caller pool (scalar copy).

### `service_startup`

- **Brief:** Map afw_service_startup_t to status string
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `false`
- **Description:** internal points to afw_service_startup_t. Copies the enum and returns the corresponding permanent/utf8 name as a string value in p.

### `service_status`

- **Brief:** Map afw_service_status_t to status string
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `false`
- **Description:** internal points to afw_service_status_t. Copies the enum and returns the corresponding name as a string value in p.

### `size`

- **Brief:** Map afw_size_t member as integer
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `false`
- **Description:** internal points to an afw_size_t. Returns an integer value in the caller pool (scalar copy).

### `stopping_adapter_instances`

- **Brief:** Snapshot stopping adapter instance reference counts
- **copiesUnderLock:** `true`
- **returnsLiveReference:** `false`
- **Description:** internal is adapter_id (afw_utf8_t **). Under adapter_id_anchor_lock, walks the stopping chain and copies each stopping instance's reference_count into an integer array allocated in p. Safe snapshot; does not return pointers into anchors.

### `stopping_authorization_handler_instances`

- **Brief:** Snapshot stopping authorization handler instance reference counts
- **copiesUnderLock:** `true`
- **returnsLiveReference:** `false`
- **Description:** internal is authorization_handler_id (afw_utf8_t **). Under the authorization handler id rw lock (write), walks the stopping chain and copies reference counts into an integer array in p.

### `uint32`

- **Brief:** Map afw_uint32_t member as integer
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `false`
- **Description:** internal points to an afw_uint32_t. Returns an integer value in the caller pool (scalar copy).

### `value`

- **Brief:** Return afw_value_t * member as-is
- **copiesUnderLock:** `false`
- **returnsLiveReference:** `true`
- **Description:** internal points to an afw_value_t *. Returns that value pointer without cloning (returnsLiveReference). Lifetime is that of the stored value.

