#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: type_check_flags.as
//? customPurpose: Part of compiler category tests
//? description: ...
Flag and #typecheck pragma *contract* only (issue #28).
Proves how checking is turned on/off: env/flag_set compile:typeCheck,
typeCheckCompileOnly (compileOnly wins when both mode flags are set),
noImplicitAny, strictNullChecks, compile:strict, and matching #typecheck
forms. Behavioral assignability/shapes/Adaptive formals live in
type_check.as under #typecheck in the unit under test.
Nuances: all //? test: cases share one afw process, so each case clears
the type-flag family before enabling what it needs; #typecheck can leave
env flags set after that compile (see pragma-does-not-affect-sibling-compile).
typeCheck = compile when known + runtime assign/params; compileOnly = no
runtime formal checks. Without strictNullChecks, null may assign to
integer under typeCheck alone.
//? sourceType: script
//?
//? test: flags-default-off
//? description: With all type flags off, wrong type assigns are allowed
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
//? test: flags-typeCheck-runtime-assign
//? description: compile:typeCheck rejects wrong assign at evaluate
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
//? test: flags-compileOnly-literal
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
//? test: flags-compileOnly-no-runtime-formal
//? description: compileOnly does not apply runtime formal checks on convert path
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
//? test: flags-typeCheck-runtime-param
//? description: typeCheck rejects wrong script param at evaluate
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
//? test: flags-noImplicitAny
//? description: typeCheck + noImplicitAny requires annotations at compile
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
//? test: flags-strict-bundle
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
//? test: flags-compileOnly-wins-over-typeCheck
//? description: both mode flags → compile only (no runtime param check)
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
let saw: boolean = false;
try {
    compile<script>(script("const x: integer = \"h\";\nreturn 0;"));
} catch (e) {
    saw = true;
}
assert(saw);
const r: any = evaluate(compile<script>(script(
    "const f = function (a: integer) { return a; };\n" +
    "return f(integer(\"5\"));"
)));
assert(r === 5);
return 0;

//?
//? test: flags-strictNull-rejects-null
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
//? test: flags-without-strictNull-allows-null
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
//? test: pragma-on
//? description: #typecheck; enables checking for that compile unit
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
    "#typecheck;\nconst x: integer = \"hello\";\nreturn 0;"
));
return 0;

//?
//? test: pragma-compileOnly
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
//? test: pragma-off-after-flags
//? description: #typecheck off; disables checking for that unit despite flags
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
    "#typecheck off;\nconst x: integer = \"hello\";\nreturn 0;"
)));
return 0;

//?
//? test: pragma-noImplicitAny
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
//? test: pragma-strict-bundle
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
//? test: pragma-options-comma
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
//? test: pragma-strictNullChecks
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
//? test: pragma-does-not-affect-sibling-compile
//? description: pragma in one unit does not force checking on a later unit
//? expect: 0
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
/* First unit turns checking on (and may leave env flags set). */
try {
    compile<script>(script(
        "#typecheck;\nconst x: integer = \"bad\";\nreturn 0;"
    ));
    assert(false);
} catch (e) {
    /* expected */
}
/*
 * Clear flags explicitly so a sibling compile is opt-in again.
 * Without clear, env flags can linger for the process.
 */
flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
const r: any = evaluate(compile<script>(script(
    "const x: integer = \"hello\";\nreturn 0;"
)));
return 0;
