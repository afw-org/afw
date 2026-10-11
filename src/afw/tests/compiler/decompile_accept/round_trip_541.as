#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: round_trip_541.as
//? customPurpose: Part of compiler decompile_accept tests
//? description: ...
Forms found by the decompile corpus sweep (#541): the decompiled text must
recompile, evaluate like the source, and be a fixed point.
//? sourceType: script
//?
//? test: call-of-function-expression
//? description: (function () { ... })() decompiles to a callable form
//? expect: true
//? source: ...

function round_trip(src: string): boolean {
    const d1 = decompile(compile<script>(script(src)));
    const d2 = decompile(compile<script>(script(d1)));
    assert(d1 === d2, "fixed point: " + d1 + " vs " + d2);
    return stringify(evaluate(compile<script>(script(src)))) ===
        stringify(evaluate(compile<script>(script(d1))));
}
assert(round_trip("return (function () { return 5; })();"));
assert(round_trip("function g(n) { return n + 1; } return g(2);"));
assert(round_trip("const o = {f: function(n) { return n * 2; }}; return o.f(4);"));
return true;

//? test: optional-typed-parameter
//? description: An optional parameter with a type decompiles as name?:Type
//? expect: true
//? source: ...

const src = "function t(a: string, c?: string) { return a + string(c); } return t(\"1\");";
const d1 = decompile(compile<script>(script(src)));
assert(index_of(d1, "c?:string") >= 0, d1);
return evaluate(compile<script>(script(d1))) === "1undefined";

//? test: assign-only-destructure-no-types
//? description: Assigning to declared variables through a Pattern prints no types
//? expect: true
//? source: ...

const src = "let h: string; let w: string; [h, , w] = [\"a\", \"b\", \"c\"]; return h + w;";
const d1 = decompile(compile<script>(script(src)));
assert(index_of(d1, "[h,,w]") >= 0, d1);
return evaluate(compile<script>(script(d1))) === "ac";

//? test: is-null
//? description: is_null(x) decompiles to is<null>(x), which parses
//? expect: true
//? source: ...

const d1 = decompile(compile<script>(script("return [is_null(null), is_null(1)];")));
const r = evaluate(compile<script>(script(d1)));
return r[0] === true && r[1] === false;

//? test: catch-as-last-statement
//? description: A try whose catch binds a name, as the last statement
//? expect: true
//? source: ...

const src = "let r = 0; try { throw \"a\"; } catch (e) { r = 1; }";
const d1 = decompile(compile<script>(script(src)));
return evaluate(compile<script>(script(d1))) ===
    evaluate(compile<script>(script(src)));

//? test: empty-branch-as-last-statement
//? description: An omitted empty then-branch decompiles as #block(), not undefined
//? expect: true
//? source: ...

const src = "let x = 0; x = 1; if (x !== 0) { }";
const d1 = decompile(compile<script>(script(src)));
assert(index_of(d1, "#block()") >= 0, d1);
return evaluate(compile<script>(script(d1))) ===
    evaluate(compile<script>(script(src)));
