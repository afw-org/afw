#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: emit_and_roundtrip.as
//? customPurpose: Part of compiler decompile_accept tests
//? description: ...
Per-form emit and d1==d2 round-trip for compiler-private # shapes.
Broader construct matrix remains decompile_fidelity.as. Inventory:
designs/decompile-compiler-private-inventory.md.
//? sourceType: script
//?
//? test: emit-block-assignment-target
//? description: const decompiles with #block and #assignment_target; round-trips
//? expect: 0
//? source: ...

const src = "const x = 1;\nreturn x;";
const d1 = decompile(compile<script>(script(src)));
assert(includes(d1, "#block("));
assert(includes(d1, "#assignment_target("));
const d2 = decompile(compile<script>(script(d1)));
assert(d1 == d2);
assert(evaluate(compile<script>(script(d1))) === 1);
return 0;

//?
//? test: emit-script-function
//? description: function value decompiles as #script_function; round-trips
//? expect: 0
//? source: ...

const src =
    "const f = function (a: integer): integer { return a + 1; };\n" +
    "return f(1);";
const d1 = decompile(compile<script>(script(src)));
assert(includes(d1, "#script_function("));
const d2 = decompile(compile<script>(script(d1)));
assert(d1 == d2);
assert(evaluate(compile<script>(script(d1))) === 2);
return 0;

//?
//? test: emit-template-definition
//? description: template string decompiles as #template_definition
//? expect: 0
//? source: ...

const src = "return `a${1}b`;";
const d1 = decompile(compile<script>(script(src)));
assert(includes(d1, "#template_definition("));
const d2 = decompile(compile<script>(script(d1)));
assert(d1 == d2);
assert(evaluate(compile<script>(script(d1))) === "a1b");
return 0;

//?
//? test: emit-switch-default-and-statements
//? description: switch default emits #switch_default and #statements
//? expect: 0
//? source: ...

const src = "switch (9) { case 1: return 1; default: return 99; }";
const d1 = decompile(compile<script>(script(src)));
assert(includes(d1, "#switch_default"));
assert(includes(d1, "#statements("));
const d2 = decompile(compile<script>(script(d1)));
assert(d1 == d2);
assert(evaluate(compile<script>(script(d1))) === 99);
return 0;

//?
//? test: emit-for-statements
//? description: for-loop decompile uses #statements for clause lists
//? expect: 0
//? source: ...

const src = "for (let i = 0; i < 1; i = i + 1) { return i; }";
const d1 = decompile(compile<script>(script(src)));
assert(includes(d1, "#statements("));
assert(includes(d1, "#assignment_target("));
const d2 = decompile(compile<script>(script(d1)));
assert(d1 == d2);
assert(evaluate(compile<script>(script(d1))) === 0);
return 0;

//?
//? test: emit-call-spread-surface-ellipsis
//? description: call-site spread decompiles as ... not #list_expression
//? expect: 0
//? source: ...

const src =
    "const f = function (...r) { return r; };\n" +
    "return f(...[1, 2], 3);";
const d1 = decompile(compile<script>(script(src)));
assert(includes(d1, "..."));
/* Prefer surface ellipsis; #list_expression should not appear in call args. */
assert(!includes(d1, "#list_expression"));
const d2 = decompile(compile<script>(script(d1)));
assert(d1 == d2);
const r = evaluate(compile<script>(script(d1)));
assert(r[0] === 1);
assert(r[1] === 2);
assert(r[2] === 3);
return 0;

//?
//? test: emit-script-function-pattern-and-rest
//? description: Pattern and rest params survive decompile text
//? expect: 0
//? source: ...

const src =
    "const f = function ({a}, ...r) { return a; };\n" +
    "return f({a: 4}, 1, 2);";
const d1 = decompile(compile<script>(script(src)));
assert(includes(d1, "#script_function("));
const d2 = decompile(compile<script>(script(d1)));
assert(d1 == d2);
assert(evaluate(compile<script>(script(d1))) === 4);
return 0;
