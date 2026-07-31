#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: decompile_fidelity.as
//? customPurpose: Part of compiler category tests
//? description: decompile recompile preserves IR (d1 == d2)
//? sourceType: script
//?
//? test: fidelity-core-constructs
//? description: Major constructs round-trip decompile text stably
//? skip: false
//? expect: 0
//? source: ...

/*
 * Goal: decompile(compile(src)) recompiles to the same decompile string.
 * Not source listing recovery - same value-tree / behavior IR.
 */
const check = function (src) {
    const d1 = decompile(compile<script>(script(src)));
    const d2 = decompile(compile<script>(script(d1)));
    assert(d1 == d2);
    return 0;
};

check("return 1+2*3;");
check("const x = 1;\nreturn x;");
check("const x: integer = 1;\nreturn x;");
check("let y = 2;\ny = 3;\nreturn y;");
check("const f = function (a) { return a; };\nreturn f(1);");
check("function f(a) { return a; }\nreturn f(1);");
check("const f = function (a: integer): integer { return a + 1; };\nreturn f(1);");
check("if (true) return 1; else return 2;");
check("const a = [1,2];\nreturn [...a, 3];");
check("const [a,b] = [1,2];\nreturn a;");
check("const {a,b} = {a:1,b:2};\nreturn b;");
check("for (let i = 0; i < 1; i = i + 1) { return i; }");
check("let i = 0;\nwhile (i < 1) { i = i + 1; }\nreturn i;");
check("return {a: 1+2};");
check("const a = [10,20];\nreturn a[1];");
check("const o = {a: 1};\nreturn o[\"a\"];");
check("return process::osType;");
check("const f = function (x, y) { return x + y; };\nreturn f(3,4);");
check("const f = function (a = 3) { return a; };\nreturn f();");
check("const f = function (...r) { return r; };\nreturn f(1,2);");
check("switch (1) { case 1: return 1; default: return 0; }");
check("try { return 1; } catch (e) { return 0; }");
check("try { throw 1; } catch (e) { return e; }");
return 0;

//? test: fidelity-eval-throw-catch
//? description: recompiled try/throw catch preserves evaluate result
//? skip: false
//? expect: 0
//? source: ...

/* throw requires a string message */
const src = "try { throw \"boom\"; } catch (e) { return e.message; }";
const d = decompile(compile<script>(script(src)));
assert(d == decompile(compile<script>(script(d))));
assert(evaluate(compile<script>(script(src))) == "boom");
assert(evaluate(compile<script>(script(d))) == "boom");
return 0;

//? test: fidelity-eval-switch-default
//? description: switch default marker survives recompile (#switch_default)
//? skip: false
//? expect: 0
//? source: ...

const src = "switch (9) { case 1: return 1; default: return 99; }";
const d = decompile(compile<script>(script(src)));
assert(d == decompile(compile<script>(script(d))));
assert(evaluate(compile<script>(script(src))) == 99);
assert(evaluate(compile<script>(script(d))) == 99);
return 0;
