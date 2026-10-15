#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: compile_memory.as
//? customPurpose: Part of compiler tests
//? description: ...
Compiling many times in one evaluation stays flat. The compiler looks
up each identifier in the environment's string literal registry; a
miss scans extension manifests, and what that scan makes must not stay
in the job heap until the xctx ends.
//? sourceType: script
//?
//? test: compile-with-names-is-flat
//? description: compile a script that declares names, many times
//? expect: 0
//? source: ...

function f() {
    const c = compile<script>(script("let someName = 1; return someName;"));
    return 1;
}
let start = 0;
for (let i = 0; i < 2000; i = i + 1) {
    if (i == 10) { start = pool_bytes_in_use(); }
    f();
}
assert(pool_bytes_in_use() - start < 200000, "compile per call is flat");
return 0;

//?
//? test: compile-and-evaluate-with-names-is-flat
//? description: compile and evaluate per call, names unknown to the environment
//? expect: 0
//? source: ...

let start = 0;
for (let i = 0; i < 2000; i = i + 1) {
    if (i == 10) { start = pool_bytes_in_use(); }
    assert(evaluate(compile<script>(script(
        "const notALiteralName = 2; return notALiteralName;"))) === 2);
}
assert(pool_bytes_in_use() - start < 200000, "compile and evaluate is flat");
return 0;

//?
//? test: json-parse-is-flat
//? description: ...
object() of a JSON string and compile<json> / compile<relaxedJson>,
many times: the parsed data is built in dest p and goes with it.
//? expect: 0
//? source: ...

function f() {
    const o = object("{\"a\": 1, \"b\": [1, 2]}");
    const a = compile<json>(json("[3, {\"c\": 4}]"));
    const r = compile<relaxedJson>(relaxedJson("{d: [5] /* c */}"));
    return o.a + a[0] + r.d[0];
}
let start = 0;
for (let i = 0; i < 2000; i = i + 1) {
    if (i == 10) { start = pool_bytes_in_use(); }
    assert(f() === 9);
}
assert(pool_bytes_in_use() - start < 200000, "json parse is flat");
return 0;

//?
//? test: json-parse-error-is-flat
//? description: ...
A JSON parse that throws, caught, many times: the parse goes with dest
p, and the caught error's message is still readable.
//? expect: 0
//? source: ...

function f(s) {
    try {
        object(s);
    }
    catch (e) {
        return length(e.message) > 0 ? 1 : 0;
    }
    return 0;
}
let start = 0;
for (let i = 0; i < 2000; i = i + 1) {
    if (i == 10) { start = pool_bytes_in_use(); }
    assert(f("ab,c") === 1);
    assert(f("[1, 2]") === 1);
}
assert(pool_bytes_in_use() - start < 200000, "json parse error is flat");
return 0;
