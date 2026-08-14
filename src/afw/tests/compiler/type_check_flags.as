#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: type_check_flags.as
//? customPurpose: Part of compiler category tests
//? description: ...
Flag and #compile pragma *contract* only (issue #28 / Pattern B).
Proves how checking is turned on/off: env/flag_set compile:typeCheck,
typeCheckCompileOnly (compileOnly wins when both mode flags are set),
noImplicitAny, strictNullChecks, compile:strict, and matching #compile
operands (flag short names). Behavioral rules live in type_check.as under
#compile typeCheck in the unit under test.
Nuances: all //? test: cases share one afw process, so each case clears
the type-flag family before enabling what it needs; #compile does not set
process flags (see pragma-does-not-affect-sibling-compile). Flags are
snapshotted at compile start — set them before compile…, or use #compile
inside the unit (mid-unit applies from that statement on). typeCheck =
compile when known + runtime assign/params; compileOnly = no runtime
formal checks. Without strictNullChecks, null may assign to integer under
typeCheck alone.
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
//? test: flags-typeCheck-uninit-let-loose
//? description: typeCheck without strictNull allows uninit let integer (NULL≡undefined)
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
    "let x: integer;\nreturn is_nullish(x);"
)));
assert(r === true);
return 0;

//?
//? test: pragma-on
//? description: #compile typeCheck; enables checking for that compile unit
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
    "#compile typeCheck;\nconst x: integer = \"hello\";\nreturn 0;"
));
return 0;

//?
//? test: pragma-compileOnly
//? description: #compile typeCheckCompileOnly;
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
    "#compile typeCheckCompileOnly;\nconst x: integer = \"hello\";\nreturn 0;"
));
return 0;

//?
//? test: pragma-noTypeCheck-after-flags
//? description: #compile noTypeCheck; disables checking for that unit despite flags
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
    "#compile noTypeCheck;\nconst x: integer = \"hello\";\nreturn 0;"
)));
return 0;

//?
//? test: pragma-noImplicitAny
//? description: #compile typeCheck noImplicitAny;
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
    "#compile typeCheck noImplicitAny;\nconst x = 1;\nreturn 0;"
));
return 0;

//?
//? test: pragma-strict-bundle
//? description: #compile strict; enables noImplicitAny
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
    "#compile strict;\nconst x = 1;\nreturn 0;"
));
return 0;

//?
//? test: pragma-strictNullChecks
//? description: #compile typeCheck strictNullChecks;
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
    "#compile typeCheck strictNullChecks;\n" +
    "const x: integer = null;\nreturn 0;"
)));
return 0;

//?
//? test: pragma-does-not-affect-sibling-compile
//? description: #compile on one unit must not change process flags for a later unit
//? expect: 0
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
/* First unit turns checking on via #compile only (policy, not flags). */
try {
    compile<script>(script(
        "#compile typeCheck;\nconst x: integer = \"bad\";\nreturn 0;"
    ));
    assert(false);
} catch (e) {
    /* expected */
}
/*
 * Do not clear flags — #compile must not have set process typeCheck.
 * Sibling unit with defaults off must still allow the wrong assign.
 */
const r: any = evaluate(compile<script>(script(
    "const x: integer = \"hello\";\nreturn 0;"
)));
return 0;

//?
//? test: mid-unit-typeCheck-after-unchecked
//? description: mid-unit #compile typeCheck checks only statements after the pragma
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
 * Before the pragma, process snapshot is off — bad assign is not compile-checked.
 * After #compile typeCheck, the bad assign must fail compile.
 */
compile<script>(script(
    "const a: integer = \"x\";\n" +
    "#compile typeCheck;\n" +
    "const b: integer = \"y\";\n" +
    "return 0;"
));
return 0;

//?
//? test: mid-unit-unchecked-before-typeCheck-ok
//? description: bad assign before mid-unit #compile typeCheck is not re-checked at compile
//? expect: 0
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
/*
 * Compile-time only: constructs already parsed are not re-checked after the
 * pragma. (Runtime uses final unit policy — see mid-unit-runtime-final-policy.)
 */
compile<script>(script(
    "const a: integer = \"x\";\n" +
    "#compile typeCheck;\n" +
    "const b: integer = 1;\n" +
    "return 0;"
));
return 0;

//?
//? test: mid-unit-runtime-final-policy
//? description: runtime typeCheck uses final unit policy (mid-unit typeCheck applies to earlier assigns)
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
 * Compile succeeds (bad assign was before typeCheck). Evaluating still fails
 * because unit policy ends with typeCheck on — runtime uses last-mutated policy.
 */
evaluate(compile<script>(script(
    "const a: integer = \"x\";\n" +
    "#compile typeCheck;\n" +
    "const b: integer = 1;\n" +
    "return 0;"
)));
return 0;

//?
//? test: mid-unit-noTypeCheck-after-typeCheck
//? description: mid-unit #compile noTypeCheck allows bad assigns after typeCheck
//? expect: 0
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
const r_mid2: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const a: integer = 1;\n" +
    "#compile noTypeCheck;\n" +
    "const b: integer = \"ok\";\n" +
    "return 0;"
)));
return 0;
