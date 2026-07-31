# Compile-time optimize (notes for a future branch)

**Not part of issue #18 / pragma IR work.** Separate feature branch and design conversation later.

## Idea

At compile time, fold calls where:

- the function is **`pure`** (function metadata already has `pure: true`), and  
- every argument is a **compile-time constant** (literal / permanent / already folded data-type value).

Replace the call node with the evaluated result in the value graph (classic constant folding).

Example: `add(1, multiply(2, 3))` → `7`.

## How current #18 work helps testing later

- **`decompile(compile(...))`** shows IR shape before/after fold (call disappears, literal appears).
- **`#block(...)`** and other PragmaValues let tests inject IR without going through full Script surface.
- Round-trip tests establish eval + decompile stability; optimize tests can assert **structural** decompile change under an optimize flag/pragma.

## Possible controls

- Compile option (whole unit).
- Future **`#optimize`** (or similar) as **PragmaStatement** for regions — not implemented yet.

## Engine notes (sketch)

- Trust `pure` on builtins first (arithmetic, etc.).
- Constant lattice: permanent evaluated data types; extend carefully to const bindings already folded in-block.
- Existing `optimized_value` fields on some value kinds may relate; decompile of “what runs” should prefer folded form.
- Policy for pure calls that throw at fold time: compile error vs leave call.

## Explicitly out of scope for #18

Implementing the optimizer, pragma, or permanent purity audits. This file is only so the idea is not lost in chat.

---

## Related #18 follow-ups (not optimize)

### Destructuring `#assignment_target`

Current decompile of list/object destructure is a **placeholder**:

```text
#assignment_target("const","list_destructure")
#assignment_target("const","object_destructure")
```

That is not round-trippable into real bindings (and recompile would invent a symbol named `list_destructure`). Full support needs:

1. Richer decompile of `list_destructure` / `object_destructure` trees (elements, renames, defaults, rest).
2. Matching `#assignment_target` pragma parse that builds the same create structures as `afw_compile_parse_assignment_target.c`.

Do that as its own slice after more structural pragmas if needed; do not pretend the placeholder string form is the API.

### `#closure_binding`

Decompile of a **runtime** closure is `#closure_binding(#script_function(...))`. The binding holds a live `enclosing_lexical_scope` (xctx scope), which is not reconstructible from decompile text alone (free variables like outer `x` need that scope).

Do **not** implement a fake `#closure_binding` pragma that drops the scope: calls with free vars would mis-evaluate. Treat runtime-closure decompile as **display / debug** unless a future design serializes closed-over bindings deliberately.
