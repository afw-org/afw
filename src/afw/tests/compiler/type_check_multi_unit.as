#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: type_check_multi_unit.as
//? customPurpose: Part of compiler category tests
//? description: ...
Multi-unit compile_policy smoke (issue #28 / Pattern B).
Each compile<script> is its own unit with its own compile_policy
(snapshotted from process flags, then only #compile mutates that unit).
Script function *runtime* formals/returns prefer definition unit
(script->contextual). Call-site unit policy still drives compile-time
formal checks when a call is built under that unit. Flag/pragma plumbing:
type_check_flags.as.
Nuances: cases share one afw process — clear the type-flag family at the
start of each case, and again at the end if the case enabled process
typeCheck (so the next case's *outer* source is not compiled under a
stale process snapshot).
//? sourceType: script
//?
//? test: sibling-pragma-does-not-set-process-flags
//? description: #compile typeCheck on unit A must not enable checking for later unit B
//? expect: 0
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
try {
    compile<script>(script(
        "#compile typeCheck;\nconst x: integer = \"bad\";\nreturn 0;"
    ));
    assert(false);
} catch (e) {
    /* expected compile failure under unit A policy */
}
/* Unit B still has defaults off (no process flag leak from #compile). */
const r: any = evaluate(compile<script>(script(
    "const x: integer = \"hello\";\nreturn 0;"
)));
return 0;

//?
//? test: nested-inner-pragma-on
//? description: nested compile unit with #compile typeCheck fails independently of outer
//? expect: 0
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
/* Outer unit: no typeCheck; nesting is another compile<script> value. */
const outer = evaluate(compile<script>(script(
    "let saw = false;\n" +
    "try {\n" +
    "  compile<script>(script(\n" +
    "    \"#compile typeCheck;\\n\" +\n" +
    "    \"const x: integer = \\\"bad\\\";\\n\" +\n" +
    "    \"return 0;\"\n" +
    "  ));\n" +
    "} catch (e) {\n" +
    "  saw = true;\n" +
    "}\n" +
    "assert(saw);\n" +
    "return 1;"
)));
assert(outer === 1);
return 0;

//?
//? test: nested-outer-pragma-inner-default-off
//? description: outer #compile does not force policy on a nested unit without #compile
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
 * Outer source has #compile typeCheck (would reject bad assign itself).
 * Nested unit has no #compile and process flags stay off — nested must
 * still allow a wrong literal assign.
 */
const outer = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const ok: integer = 1;\n" +
    "const nested = evaluate(compile<script>(script(\n" +
    "  \"const x: integer = \\\"hello\\\";\\n\" +\n" +
    "  \"return 0;\"\n" +
    ")));\n" +
    "return ok;"
)));
assert(outer === 1);
return 0;

//?
//? test: two-units-independent-snapshots
//? description: strict and loose definition units coexist; each keeps its policy at eval
//? expect: 0
//? source: ...

/*
 * Must run with process type flags off so this case's outer unit is not
 * compiled under typeCheck (call-site compile formal checks would then
 * fire on loose_f("1") even though definition policy is off).
 */
flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
const strict_f = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "return function (a: integer) { return a; };"
)));
const loose_f = evaluate(compile<script>(script(
    "#compile noTypeCheck;\n" +
    "return function (a: integer) { return a; };"
)));
const loose_r = loose_f("1");
assert(loose_r === 1);
let saw = false;
try {
    strict_f("1");
} catch (e) {
    saw = true;
}
assert(saw);
return 0;

//?
//? test: definition-policy-rejects-bad-param
//? description: function defined under typeCheck rejects bad arg when called with flags off
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
const f = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "return function (a: integer) { return a; };"
)));
/* Call site process flags are off; definition unit still has typeCheck. */
f("1");
return 0;

//?
//? test: definition-policy-off-allows-convert-param
//? description: function defined under #compile noTypeCheck converts leaf arg despite process typeCheck
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
const f = evaluate(compile<script>(script(
    "#compile noTypeCheck;\n" +
    "return function (a: integer) { return a; };"
)));
/* Definition policy off → convert path; "1" becomes 1. */
const r = f("1");
assert(r === 1);
/* Leave process flags off for the next case's outer compile. */
flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
return 0;

//?
//? test: definition-policy-rejects-bad-return
//? description: return type checked under definition unit policy
//? expect: error
//? source: ...

flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
const f = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "return function (): integer { return \"x\"; };"
)));
f();
return 0;

//?
//? test: definition-policy-off-skips-return-check
//? description: #compile noTypeCheck on definition unit skips runtime return check despite process typeCheck
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
const f = evaluate(compile<script>(script(
    "#compile noTypeCheck;\n" +
    "return function (): integer { return \"x\"; };"
)));
const r = f();
assert(r === "x");
flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
return 0;
