# Review / change error codes (issue #33)

**Audience:** maintainers / assistants. **Not** user docs.  
**GitHub:** [#33](https://github.com/afw-org/afw/issues/33) — *Review/Change Error Codes*  
**Status:** best-effort pass on branch `issue-#33-error-codes`. Map reordered; some HTTP throws retargeted; script `throw` may set `id`; remaining unclear ids renamed. Extension-defined names still open.  
**Related:** try/catch (`afw-script-errors`), HTTP request path (`afw-server` / `afw-server-fcgi`), crypto prefix convention (`secrets-and-afw-crypto.md`), #158 `terminating` → 503.

## Landed (this branch)

Beta rebuild is assumed, so the enum was **reordered**: `none`, `general`, `throw` first, then language, then request/HTTP, then host. **Prefer `e.id`**. Unused HTTP tokens were wired where the message already said that. Unclear ids were renamed (`arg_error` → `argument_error`, `cast_error` → `conversion_error`, `evaluate` → `evaluation_error`, `undefined` → `undefined_value`, `code` → `coding_error`, `client_time_out` → `client_timeout`). A required parameter that is `undefined` now throws `undefined_value` instead of `general`.

| Was `general` (500) | Now | HTTP |
|---------------------|-----|------|
| REST unknown method | `method_not_allowed` | 405 |
| Bad URI / missing object type / dotted id / missing body | `bad_request` | 400 |
| Invalid POST body shape | `request_syntax` | 400 |
| Unsupported `Content-Type` | `unsupported_content` | 415 |
| Adapter not registered | `not_found` | 404 |
| Journal retrieve/delete/modify/replace; adapter has no journal | `method_not_supported` | 501 |
| LDAP unsupported query operator | `query_too_complex` | 400 |

`syntax` HTTP **500 → 400** (Adaptive parse is not a server bug). `general` stays the default for everything else (including `open_file` miss + errno `rv`).

### Script `throw` and `id`

```adaptive
throw "Person not found";
throw "Person not found" data { personId: id };
throw "Person not found" id "not_found";
throw "Person not found" id "not_found" data { personId: id };
```

`data` and `id` are names only in this statement (not new reserved words). Either order. At most one of each.

`throw "…" { … }` (second expression, no `data` name) is **deprecated**, kept for a while so people can test on `mgg-develop`. Remove it after that. `data` is a common variable name: while the old form remains, `throw "…" data` cannot mean “payload is the variable `data`” (it starts the `data` clause). After removal, that is only `throw "…" data data`.

`id` must evaluate to a string allowed on script throw: `throw`, `not_found`, `denied`, `authentication_required`, `conflict`, `bad_request`, `read_only`, `payload_too_large`, `query_too_complex`, `method_not_allowed`. A string literal that is not allowed is a compile error; a value computed at run time that is not allowed is `argument_error`.

Sets `e.id` and the HTTP status from the map. Phrase on the status line stays the map text.

Built-in `errorsThrown` (this pass): catch-worthy names on adapter get/add/delete/replace/modify/retrieve, `assert`, `compile` / `compile_from_file` / `eval_from_file`, `authorization_check`, `journal_get_by_cursor`. `eq` / `ne` use `conversion_error` (was the non-name `conversion`). File and VFS “already exists” throw `conflict`. Empty `errorsThrown` still means nothing special.

Not in this pass: extension-defined names; catch-by-id syntax.

## Issue text (all of it)

Opened 2023-07-25. No comments.

> Now that we can try/catch in Adaptive Script, we need to review current error codes and change any, or create new ones that may be more appropriate.

Labels: **documentation**, **review**, **test**. Assignees: Mike, Jeremy.

Vague on purpose: it is a **review**, not a specified remap. The useful work is to say what the codes are *for* now, what is stale, and what a catcher or HTTP client can actually rely on.

## Intent (best judgment)

The enum and `AFW_ERROR_CODE_MAP` were written as a **request/HTTP classification**: each code has `id`, `error_allow_in_response`, `http_response_code`, and a short status phrase. Uncaught errors in `afwfcgi` become `Status:` plus an optional `_AdaptiveError_` body.

Then Adaptive Script got **`try` / `catch` / `throw`**. The same C `afw_error_t` is turned into a catch object (`afw_error_to_object`). Scripts now see **`e.id`** (mnemonic) and **`e.errorCode`** (numeric). Tests already branch on `e.id` (`throw`, `denied`, `payload_too_large`, `not_found`, `general`).

So #33 is: **the codes stopped being only an HTTP table and became a script-visible API.** Review whether the set, the throw sites, and the HTTP mappings still make sense for both consumers.

Why that is useful:

1. **Catch can classify without parsing `e.message`.** `Try-30` in `try.as` already does `if (e.id != 'throw') { throw; }`. Auth tests assert `e.id === "denied"`. That only works if C throws the specific token, not `general`.
2. **Uncaught errors are the HTTP contract.** Clients and reverse proxies key off 401/403/404/413/503. A `general` (500) for “method not supported” or a parse error (500 via `syntax`) is the wrong signal.
3. **Most throws are still `general`.** A catcher cannot tell “missing file” from “internal bug” except by message text. `open_file` missing file is asserted as `id === "general"` plus `rv`/`rvDecoded` (`stream_file.as`).
4. **Docs never caught up.** Handbook Features shows `e.message` / `e.data` only. Language Reference Statements has no Try/Throw section. `_AdaptiveError_` is the object type; there is no published code list.

## Where truth lives

| Layer | What |
|-------|------|
| Map + ABI comment | `src/afw/include/afw_common.h` — `AFW_ERROR_CODE_MAP` |
| Throw / TRY macros | `src/afw/error/afw_error.h` |
| HTTP + object fill | `src/afw/error/afw_error.c` — `impl_error_code_map`, `afw_error_http_status`, `afw_error_allow_in_response`, `afw_error_add_to_object` |
| Uncaught HTTP write | `afw_request_write_error_to_response_body` → `afwfcgi` `AFW_CATCH_UNHANDLED` |
| Script catch object | `afw_function_execute_try` → `afw_error_to_object`; type `generate/objects/_AdaptiveObjectType_/_AdaptiveError_.json` |
| Script `throw` | always code **`throw`** (`afw_function_execute_throw`) |
| Parse errors | `AFW_COMPILE_THROW_ERROR_*` → **`syntax`** |
| Day rule | `.cursor/rules/afw-script-errors.mdc` |
| Crypto (no new enums) | `designs/secrets-and-afw-crypto.md` — message prefix + existing code |

Ground truth is the map + throw sites + tests. This pad is inventory.

## Two consumers, one enum

```text
C: AFW_THROW_ERROR_*(code, …)
      → afw_error_t { code, message_z, data, rv, … }
      → longjmp

caught by script try
      → _AdaptiveError_  { id, errorCode, message, data, rv, … }

uncaught in afwfcgi
      → HTTP Status from map[code]
      → body may include the same object (if allow_in_response)
```

`afw` CLI prints the error; it does not apply HTTP status. `--local` / local-server embeds `afw_error_to_object` in a `{ status, error }` wrapper without the HTTP line.

Script `catch` is **not** filtered by code (unlike C `AFW_CATCH(denied)`). You catch everything, inspect `e.id` / `e.errorCode`, and `throw;` to rethrow.

Script `throw "msg" { … }` **cannot pick a code**. `id` is always `"throw"` (HTTP 400 if it escapes). Extra classification goes in **`e.data`** (handbook example: `{ "code": 123 }`). `throw.json` still says property `additional`; implementation and `_AdaptiveError_` use **`data`**. Map comments still say `error.additional` in a few places.

## ABI / numbers

This beta line **reordered** the map (`none`, `general`, `throw` first). Numeric `errorCode` is **not** stable vs older builds. Scripts and tests should use **`e.id`**.

New tokens can still be appended before `highest_afw = 1000`. User codes above 1000 remain reserved; no script API assigns them.

Sentinel: enum value `0` is `is_not_specified` (hardcoded row, not in the `XX` map).

## Current map (after reorder)

| n | `id` | HTTP | In body? | Phrase | Notes |
|---|------|------|----------|--------|-------|
| 0 | `is_not_specified` | 500† | no | OK / html 200 OK | sentinel |
| 1 | `none` | 200 | yes | OK | success |
| 2 | `general` | 500 | yes | General Error | default; still most throws |
| 3 | `throw` | 400 | yes | Statement throw encountered | script `throw` |
| 4 | `assertion_failed` | 400 | yes | Assertion failed | `assert()` |
| 5 | `argument_error` | 400 | yes | Argument Error | was `arg_error`; args; `1 / 0` |
| 6 | `conversion_error` | 400 | yes | Conversion Error | was `cast_error` |
| 7 | `evaluation_error` | 400 | yes | Evaluation Error | was `evaluate` |
| 8 | `undefined_value` | 400 | yes | Undefined Value | was `undefined`; required param |
| 9 | `syntax` | 400 | yes | Syntax Error | Adaptive parse |
| 10 | `bad_request` | 400 | yes | Bad Request | REST path/URI |
| 11 | `query_too_complex` | 400 | yes | Query Too Complex | LDAP op |
| 12 | `request_syntax` | 400 | yes | Request Syntax Error | bad POST body |
| 13 | `objects_needed` | 400 | yes | Objects Needed… | cache resolve |
| 14 | `authentication_required` | 401 | yes | Authentication Needed | still unused |
| 15 | `denied` | 403 | yes | Forbidden - Access Denied | authz |
| 16 | `read_only` | 403 | yes | Forbidden - Read Only | |
| 17 | `not_found` | 404 | yes | Not Found | adapter/object/URI |
| 18 | `method_not_allowed` | 405 | yes | Method Not Allowed | REST method |
| 19 | `unsupported_accept` | 406 | **no** | Unsupported Content Type Requested | Accept |
| 20 | `client_timeout` | 408 | yes | Request Timeout | was `client_time_out`; unused |
| 21 | `conflict` | 409 | yes | Conflict | file/VFS already exists; script throw |
| 22 | `length_required` | 411 | yes | Content Length Required | unused |
| 23 | `payload_too_large` | 413 | yes | Payload Too Large | maxObjects / crypto |
| 24 | `unsupported_content` | 415 | yes | Unsupported Content Type | request CT |
| 25 | `im_a_teapot` | 418 | yes | I'm a Teapot | unused |
| 26 | `memory` | 500 | yes | Memory Error | fcgi special catch |
| 27 | `coding_error` | 500 | yes | Internal Coding Error | was `code` |
| 28 | `method_not_supported` | 501 | yes | Method Not Supported | journal / capability |
| 29 | `client_closed` | 000 | **no** | Client Closed Connection | fcgi; no body |
| 30 | `terminating` | 503 | yes | Server Terminating | #158 |

Comment vs token drift in the same header: `query_to_complex` vs `query_too_complex`; `unsupported_content_type` vs `unsupported_content`. Comments also mention `error.additional`; the catch object uses `data`.

## What already works (do not break)

Scripts and tests **already** treat `e.id` as the public classifier:

| `e.id` | Who depends |
|--------|-------------|
| `throw` | `src/afw/tests/language/script/try.as` (handle only `throw`, rethrow others) |
| `denied` | authorization application tests |
| `payload_too_large` | `max_objects.as` (#49) |
| `not_found` | VFS tests (`get_object` / `delete_object` miss) |
| `general` | `stream_file.as` missing file (plus errno `rv`) |
| `assertion_failed` | `assert()` (adaptive-script rule) |

C `AFW_CATCH(client_closed)` / `AFW_CATCH(memory)` in `afwfcgi` are host recovery, not script.

`not_found` header policy: **do not throw** when “missing” is a normal get (property miss → NULL / `found=false`). **Do throw** when a named resource the caller asked for is absent (HTTP GET object, adapter `get_object`). That split is still right.

Crypto / extensions: **do not add** `afw_error_code_crypto_*`. Use an existing code + stable `error:crypto:…` message prefix (`secrets-and-afw-crypto.md`). Same idea if we add more domains before a core-code review lands.

## Mismatches that look like the original itch

These are observations, not a ship list.

**HTTP table exists; request path often ignores it**

- REST handler: `"Method not supported."` → **`general` (500)** instead of `method_not_supported` (501) or `method_not_allowed` (405).
- Several “Invalid post data” / “URI missing Object type” → `general` rather than `bad_request` / `request_syntax`.
- `request_syntax` (400) is unused; compiler uses `syntax` (500, “Server Side Syntax Error”) for Adaptive parse errors. A bad script in a request looks like a server failure.
- `unsupported_content` (415) unused; `unsupported_accept` (406) is used and **strips the error body**.
- `authentication_required` (401) unused — authz failures are `denied` (403). Fine if we never distinguish “please log in” from “you may not.”

**Language codes are HTTP 400**

Uncaught `throw`, `assert`, `argument_error`, `evaluation_error`, `undefined_value`, `conversion_error` all become **400**. That is “client request was bad” in HTTP. Reasonable if the request *body* is a script the client sent. Odd if a **server-side** model `on*` or conf script blows up — the client did not send a bad request; the app failed. One enum cannot say both without a host policy.

**`syntax` → 500** vs **`request_syntax` → 400** looks like the intended split (server compile vs request parse) that was never applied: compile always uses `syntax`.

**Script authors cannot throw a 404/403**

A model `onGetObject` that does `throw "no such user"` is `id=throw` → HTTP **400**, not 404. To get `not_found` you must go through C (or we add a way to throw with an id). That is probably the sharpest “now that we have try/catch” product gap.

**Catchers drown in `general`**

File-not-found, many adapter failures, and real bugs share `general`. `rv` / `rvSourceId` help for errno/APR/ICU. They do not help for Adaptive-level “this is a missing resource vs a bug.”

**Docs / metadata**

- Features: exception handling, no `id` / `errorCode` / HTTP map.
- Statements: no Try/Throw pages (EBNF images exist elsewhere).
- `throw.json`: `additional` vs real `data`.
- `_AdaptiveError_.json`: `errorCode` is “the numeric error code” with no table.

## Candidate directions (discuss, do not implement yet)

Flexible order — pick after consensus. Small verticals beat a big bang remap.

1. **Document the current map** (handbook + maybe a runtime/catalog view). Cheap; matches the documentation label; makes `e.id` official.
2. **Retarget existing throw sites** to unused codes (`method_not_supported`, `bad_request`, `request_syntax` vs `syntax`) where the message already says that. Low ABI risk; HTTP status will change for those paths — tests that expect `general` or `error:` message-only must be checked.
3. **Let script `throw` name an existing id** (e.g. `throw "gone" { id: "not_found" }` or a second parameter). Uncaught model/script errors could then be 404/403/409. Needs a whitelist (probably not `memory` / `client_closed` / `coding_error`). This is the feature that makes try/catch *control HTTP*.
4. **Add codes at the end** only when catchers have a real branch (e.g. I/O / timeout / conflict). Do not add one code per subsystem.
5. **Catch-by-id syntax** (`catch (e if e.id == "denied")`) is C `AFW_CATCH` envy. Not required if (1)+(inspect `e.id`) is enough. Language work; not implied by the issue text.
6. **HTTP remap of language codes** (`evaluation_error`/`syntax`/`throw` → 500 vs 400) is policy. Do not flip globally without host/context.

Non-goals unless someone asks:

- Reorder or renumber the enum.
- Per-extension enum values.
- Soft return codes (`-1` + error string). Failures still **throw**.
- Changing C `AFW_TRY` / `longjmp` mechanics.

## Suggested first vertical (when we decide to code)

Inventory and this pad first (this pass). Next, if we agree:

- One **docs** slice: table of `id` → HTTP → when; mention `e.id` in Features.
- One **C retarget** that is obviously wrong (`method_not_supported` on the REST “Method not supported” path) plus a test that asserts `e.id` and, if we have a wire leaf, the status line.
- Then talk about **script-selected id** before touching more throw sites.

## Open questions

1. Should uncaught **script** `throw` stay HTTP 400, or should authors be able to choose a mapped id?
2. Is `syntax` (500) correct for Adaptive parse errors in a request, or should those be `request_syntax` (400)?
3. Do we want 401 (`authentication_required`) distinct from 403 (`denied`)?
4. Should `open_file` missing file stay `general` + errno, or become `not_found`? Header says don’t throw `not_found` for normal misses — file open is closer to a named resource.
5. Keep `im_a_teapot`?
6. Publish the map as a runtime object on `adapterId=afw`, or handbook-only?
7. Is `e.errorCode` something we still want scripts to use, or document “prefer `id`”?

## Probe

Use a **file** with `afw -s script file.as` (CLI `-s` is the syntax name, not the source).

Live on this branch after reorder (2026-08-12):

| Script | `e.id` | `e.errorCode` |
|--------|--------|----------------|
| `throw "x" { k: 1 }` | `throw` | 3 |
| `1 / 0` | `argument_error` | 5 |
| `assert(false, "nope")` | `assertion_failed` | 4 |

```text
# Existing id assertions
afwdev test -j --srcdir-pattern afw --test-pattern 'language/script/try'
afwdev test -j --srcdir-pattern afw --test-pattern 'authorization/application'
```

Wire HTTP status: orchestrated `afwfcgi` leaf (or curl to a running server) and read the FastCGI `Status:` header. Not in default `test -j`.
