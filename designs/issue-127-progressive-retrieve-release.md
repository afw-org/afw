# Progressive retrieve release (issue #127)

**Audience:** maintainers / assistants. **Not** user docs.  
**GitHub:** [#127](https://github.com/afw-org/afw/issues/127)  
**Related:** [#49](https://github.com/afw-org/afw/issues/49) maxObjects (materialize only), [#2](https://github.com/afw-org/afw/issues/2) long-running MM, [#17](https://github.com/afw-org/afw/issues/17) faces.

## Goal

Progressive retrieve paths should **write (or call) then release** each object so large result sets do not keep every adapter object pool alive until the request ends. Not a full #2 redesign; not admin-app work.

## Ownership contract (adapter → CB)

1. **File adapter** (`afw_file.c`): on criteria match, `callback(obj, …)`; comment: **“Callback will release object.”** On miss, adapter releases.
2. **`afw_adapter_retrieve_objects`** (`afw_adapter_retrieve.c`): intermediate CB runs `afw_adapter_internal_process_object_from_adapter` (get_reference + optional view), then **“Callback will release view.”**
3. **Encode** (`afw_content_type_write_value`): synchronous for stream/response body — after return + flush, writers do not keep the object.

So write-only progressive CBs **must** `afw_object_release` after successful write.

## Paths

| Path | File / CB | Release after use? | Notes |
|------|-----------|--------------------|--------|
| `retrieve_objects*_to_stream` | `impl_retrieve_to_stream_cb` | **Yes** (was already) | Reference path |
| `retrieve_objects*_to_response` | `impl_retrieve_to_response_cb` | **Yes** (re-enabled) | Intermediate `{ intermediate, result }` is only for encode; unmanaged pointer to result |
| HTTP collection GET | `afw_request_handler_adapter.c` `impl_retrieve_cb` | **Yes** (aligned) | Object-list writer |
| `retrieve_objects` materialize | `impl_retrieve_cb` + array | **get_reference** into array | Not free-as-you-go; holds set |
| `retrieve_objects_to_callback` | `impl_retrieve_cb` + face | **Not yet** | Script may retain face (#17); releasing after CB would UAF retained bindings |

## Why to_response was commented out

`//afw_object_release(object, xctx)` dates to early alpha (long-standing workaround), not a recent intentional disable. Intermediate shell uses `set_property_as_object` (unmanaged value → object). That looks like “still holding object,” but encode finishes before release; shell is not used after. Aligning with **to_stream** is correct for write-complete ownership.

**Do not** “just uncomment” without that write-complete check; residual risk is any path that retains the object across CB return (script callback, faces).

## Residuals (not this slice)

- **to_callback / materialize**: need a coherent get_reference + release when script does *not* retain, without breaking faces that do. Closer to #2 / value lifetime.
- **Extra refs** from `process_object_from_adapter` + empty-options `view_create` (both get_reference; empty options returns same instance) may leave elevated pool RC after a single CB release — progressive free-as-you-go is still better than zero release on to_response; full accounting is #2.
- **Admin / JS progressive consumer** — out of scope for core fix.
- **maxObjects** — #49 only on materializing array functions.

## Tests (core, no app)

- **Gate** — `src/afw/tests/file_adapter/retrieve_objects.as`
  - `retrieve_objects_to_stream-progressive` — multi-object to_stream (release path already live)
  - `retrieve_objects_to_callback-count` — walk count vs materialize (no CB release yet)
  - `retrieve_objects_to_response-requires-x-afw` — CLI negative (no request / not x-afw)
- **Wire proof (opt-in)** — `src/afw/tests-extra/03-progressive-to-response/`
  - Orchestrated host `afwfcgi`, per-step `Accept: application/x-afw`
  - Demuxed `expect-response` golden (`goldens/three_intermediates.payload.bin`)
  - Seed → progressive `to_response` → count still healthy → cleanup
  - Landed with **PR #167** (orchestrated tests / stream expects / `tests-extra`); not in default `test -j`
  - Run: `afwdev test -T src/afw/tests-extra/03-progressive-to-response`
- Prefer valgrind on file_adapter retrieve **and** the progressive leaf when finishing a PR

Harness background: PR #167 replaced advanced-test/blast; authors use `expect` for eval return only and `expect-response` / `expect-raw-response` for wire. See `src/afw/tests-extra/README.md` and `SCHEMA.md`.

## Branch

`issue-127-progressive-retrieve-release` rebased onto `mgg-develop` (includes #167).
