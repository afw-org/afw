#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: type_check.as
//? customPurpose: Part of compiler category tests
//? description: Type checking flags (issue #28)
//? sourceType: script
//?
//? test: typecheck-default-off
//? description: Without flags, wrong type assigns are allowed
//? expect: 0
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
const x: integer = "hello";
return 0;

//?
//? test: typecheck-runtime-assign
//? description: compile:typeCheck rejects wrong assign at runtime
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck"], true);
evaluate(compile<script>(script(
    "const x: integer = \"hello\";\nreturn 0;"
)));
return 0;

//?
//? test: typecheck-compile-only-literal
//? description: compileOnly catches bad literal at compile
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheckCompileOnly"], true);
compile<script>(script("const x: integer = \"hello\";\nreturn 0;"));
return 0;

//?
//? test: typecheck-compile-only-no-runtime
//? description: compileOnly does not fail runtime convert path for dynamic values
//? expect: 0
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheckCompileOnly"], true);
const r = evaluate(compile<script>(script(
    "const s: string = \"1\";\nconst x: integer = integer(s);\nreturn x;"
)));
assert(r === 1);
return 0;

//?
//? test: typecheck-full-param
//? description: typeCheck rejects wrong script param at runtime
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck"], true);
evaluate(compile<script>(script(
    "const f = function (a: integer) { return a; };\nreturn f(\"x\");"
)));
return 0;

//?
//? test: typecheck-noImplicitAny
//? description: noImplicitAny with typeCheck requires annotations at compile
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck", "compile:noImplicitAny"], true);
compile<script>(script("const x = 1;\nreturn 0;"));
return 0;

//?
//? test: typecheck-strict-bundle
//? description: compile:strict enables typeCheck and noImplicitAny
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:strict"], true);
compile<script>(script("const x = 1;\nreturn 0;"));
return 0;

//?
//? test: typecheck-ok-assign
//? description: matching types succeed with typeCheck on
//? expect: 0
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck"], true);
const r: any = evaluate(compile<script>(script(
    "const x: integer = 1;\n" +
    "const f = function (a: integer): integer { return a + 1; };\n" +
    "return f(x);"
)));
assert(r === 2);
return 0;

//?
//? test: typecheck-pragma-on
//? description: #typecheck; enables full checking for rest of compile unit
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
/*
 * Inner script: pragma turns typeCheck on at parse time, then bad assign.
 */
compile<script>(script(
    "#typecheck;\nconst x: integer = \"hello\";\nreturn 0;"
));
return 0;

//?
//? test: typecheck-pragma-compileOnly
//? description: #typecheck compileOnly;
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
compile<script>(script(
    "#typecheck compileOnly;\nconst x: integer = \"hello\";\nreturn 0;"
));
return 0;

//?
//? test: typecheck-pragma-off
//? description: #typecheck off; after flags
//? expect: 0
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck"], true);
/* Pragma off inside inner script disables for that compile. */
const r: any = evaluate(compile<script>(script(
    "#typecheck off;\nconst x: integer = \"hello\";\nreturn 0;"
)));
return 0;

//?
//? test: typecheck-union-runtime
//? description: integer|string accepts both with typeCheck
//? expect: 0
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck"], true);
const r: any = evaluate(compile<script>(script(
    "const a: integer|string = 1;\n" +
    "const b: integer|string = \"x\";\n" +
    "return a;"
)));
assert(r === 1);
return 0;

//?
//? test: typecheck-compileOnly-wins-over-typeCheck
//? description: both flags → compile only (no runtime param check)
//? expect: 0
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck", "compile:typeCheckCompileOnly"], true);
/* Literal mismatch still fails at compile. */
let saw: boolean = false;
try {
    compile<script>(script("const x: integer = \"h\";\nreturn 0;"));
} catch (e) {
    saw = true;
}
assert(saw);
/*
 * compileOnly: no runtime typeCheck. Pass any to typed formal after
 * legacy convert or accept depending on leaf path.
 */
const r: any = evaluate(compile<script>(script(
    "const f = function (a: integer) { return a; };\n" +
    "return f(integer(\"5\"));"
)));
assert(r === 5);
return 0;

//?
//? test: typecheck-object-shape-ok
//? description: structural object/interface assign succeeds
//? expect: 0
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck"], true);
const r: any = evaluate(compile<script>(script(
    "interface Base { id: integer };\n" +
    "interface Person extends Base { name: string, age?: integer };\n" +
    "const p: Person = { id: 1, name: \"a\" };\n" +
    "const o: { host: string, port?: integer } = { host: \"h\" };\n" +
    "return p.id;"
)));
assert(r === 1);
return 0;

//?
//? test: typecheck-object-missing-required
//? description: missing required property fails with typeCheck
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck"], true);
evaluate(compile<script>(script(
    "const o: { host: string, port: integer } = { host: \"h\" };\n" +
    "return 0;"
)));
return 0;

//?
//? test: typecheck-object-wrong-prop-type
//? description: wrong property type fails with typeCheck
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck"], true);
evaluate(compile<script>(script(
    "const o: { host: string } = { host: 1 };\n" +
    "return 0;"
)));
return 0;

//?
//? test: typecheck-array-element
//? description: integer[] rejects string element
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck"], true);
evaluate(compile<script>(script(
    "const a: integer[] = [1, \"x\"];\n" +
    "return 0;"
)));
return 0;

//?
//? test: typecheck-tuple-length
//? description: tuple length mismatch fails
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck"], true);
evaluate(compile<script>(script(
    "const t: [integer, string] = [1];\n" +
    "return 0;"
)));
return 0;

//?
//? test: typecheck-tuple-ok
//? description: matching tuple assigns
//? expect: 0
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck"], true);
const r: any = evaluate(compile<script>(script(
    "const t: [integer, string] = [1, \"a\"];\n" +
    "return t[0];"
)));
assert(r === 1);
return 0;

//?
//? test: typecheck-pragma-noImplicitAny
//? description: #typecheck on noImplicitAny;
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
compile<script>(script(
    "#typecheck on noImplicitAny;\nconst x = 1;\nreturn 0;"
));
return 0;

//?
//? test: typecheck-pragma-strict-bundle
//? description: #typecheck strict; enables noImplicitAny
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
compile<script>(script(
    "#typecheck strict;\nconst x = 1;\nreturn 0;"
));
return 0;

//?
//? test: typecheck-pragma-options-comma
//? description: #typecheck on, noImplicitAny; with comma
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
compile<script>(script(
    "#typecheck on, noImplicitAny;\nconst x = 1;\nreturn 0;"
));
return 0;

//?
//? test: typecheck-strictNull-rejects-null
//? description: typeCheck + strictNullChecks rejects null for integer
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck", "compile:strictNullChecks"], true);
evaluate(compile<script>(script(
    "const x: integer = null;\nreturn 0;"
)));
return 0;

//?
//? test: typecheck-without-strictNull-allows-null
//? description: typeCheck alone allows null assign to integer (loose)
//? expect: 0
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck"], true);
const r: any = evaluate(compile<script>(script(
    "const x: integer = null;\nreturn x;"
)));
assert(r === null);
return 0;

//?
//? test: typecheck-pragma-strictNullChecks
//? description: #typecheck on strictNullChecks;
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
evaluate(compile<script>(script(
    "#typecheck on strictNullChecks;\n" +
    "const x: integer = null;\nreturn 0;"
)));
return 0;

//?
//? test: typecheck-interface-extends-missing-base
//? description: missing property required by extends base fails
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck"], true);
evaluate(compile<script>(script(
    "interface Base { id: integer };\n" +
    "interface Person extends Base { name: string };\n" +
    "const p: Person = { name: \"a\" };\n" +
    "return 0;"
)));
return 0;

//?
//? test: typecheck-optional-prop-wrong-type
//? description: optional property present with wrong type fails
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck"], true);
evaluate(compile<script>(script(
    "const o: { host: string, port?: integer } = { host: \"h\", port: \"x\" };\n" +
    "return 0;"
)));
return 0;

//?
//? test: typecheck-intersection-runtime
//? description: intersection requires both sides
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck"], true);
/*
 * integer & string has no runtime value that is both; object shapes
 * A & B is more useful — require props from both shapes.
 */
evaluate(compile<script>(script(
    "const o: { a: integer } & { b: string } = { a: 1 };\n" +
    "return 0;"
)));
return 0;

//?
//? test: typecheck-intersection-ok
//? description: value matching both sides of object intersection
//? expect: 0
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
flag_set(["compile:typeCheck"], true);
const r: any = evaluate(compile<script>(script(
    "const o: { a: integer } & { b: string } = { a: 1, b: \"x\" };\n" +
    "return o.a;"
)));
assert(r === 1);
return 0;
