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

### Destructuring `#assignment_target` (agreed direction)

Current decompile of list/object destructure is a **placeholder** (not the API):

```text
#assignment_target("const","list_destructure")
#assignment_target("const","object_destructure")
```

**Agreed approach** (when we implement round-trip):

- Keep **`#assignment_target(kind, Pattern)`**.
- First arg: assignment kind string (`"const"`, `"let"`, …) as today.
- Second arg: **Pattern**, not Expression — identifier/string **or** list/object pattern using surface-like `[…]` / `{…}` (holes, defaults, rest, rename, nesting).
- Parse Pattern by **reusing** existing assignment-target / list / object destructure parsers with that `assignment_type` so bindings and nesting stay correct.
- Decompile emits that pattern text; kill the placeholder strings.
- Prefer **not** a forest of nested `#list_destructure` / `#assignment_element` pragmas as the primary form.

**Process / constraints:**

- Leave **destructure parsing as it is** for now; may improve later — do not thrash `afw_compile_parse_assignment_target.c` unless a real feature needs it.
- Round-trip / richer decompile is a dedicated slice when we pick it up.

### Function parameter destructuring (future sugar — Jeremy)

ECMAScript-style “destruct in the parameter list”, e.g. conceptually:

```text
function f([a, b], {x, y}) { … }
// or AFW-flavored sugar along the same idea
```

Intent: **syntax sugar** that desugars into AFW’s existing model (param symbols + destructure assign into locals / the same IR we already have for `const [a,b] = …`), not a parallel binding system.

Implications when we touch this area later:

- Pattern machinery for `#assignment_target` and Script targets should stay **shareable** with param patterns.
- `#script_function` decompile/pragma may need to grow beyond bare param **names** once param patterns exist (or keep names as the desugared form and only surface sugar on `function (…)`) — decide then; prefer one binding story.
- Stay **AFW Script** (explicit, metadata-friendly), not a full ES port; sugar only where it maps cleanly.

### `#closure_binding`

Decompile of a **runtime** closure is `#closure_binding(#script_function(...))`. The binding holds a live `enclosing_lexical_scope` (xctx scope), which is not reconstructible from decompile text alone (free variables like outer `x` need that scope).

Do **not** implement a fake `#closure_binding` pragma that drops the scope: calls with free vars would mis-evaluate. Treat runtime-closure decompile as **display / debug** unless a future design serializes closed-over bindings deliberately.
