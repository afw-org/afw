#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: loop_trip_scope.as
//? customPurpose: Part of language/script tests
//? description: ...
Each loop trip runs in a scope that holds its condition, increment, and
for-of target as well as its body: what they make goes with the trip, not
at the end of the loop. A for or for-of with let/const in its head runs
each trip in a copy of the block that holds those names (the body is its
own block); every other loop runs each trip in its body's block.
//? sourceType: script
//?
//? test: while-condition-per-trip
//? description: what a while condition makes goes each trip
//? expect: 20000
//? source: ...

let t = "abc";
let n = 0;
let start = pool_bytes_in_use();
while (length(t + string(n)) > 0 && n < 20000) { n = n + 1; }
let used = pool_bytes_in_use() - start;
assert(used < 1000000, "while condition temporaries released per trip");
return n;

//?
//? test: while-condition-per-trip-unbraced-and-empty
//? description: unbraced and empty while bodies still have a trip scope
//? expect: 0
//? source: ...

let t = "abc";
let n = 0;
let start = pool_bytes_in_use();
while (length(t + string(n)) > 0 && n < 20000) n = n + 1;
let m = 0;
function inc() { m = m + 1; return m; }
while (length(t + string(inc())) > 0 && m < 20000) { }
let used = pool_bytes_in_use() - start;
assert(used < 1000000, "while condition temporaries released per trip");
assert(stringify([n, m]) === "[20000,20000]");
return 0;

//?
//? test: do-while-condition-per-trip
//? description: what a do while condition makes goes each trip
//? expect: 20000
//? source: ...

let t = "abc";
let n = 0;
let start = pool_bytes_in_use();
do { n = n + 1; } while (length(t + string(n)) > 0 && n < 20000);
let used = pool_bytes_in_use() - start;
assert(used < 1000000, "do while condition temporaries released per trip");
return n;

//?
//? test: for-condition-and-increment-per-trip
//? description: what a for condition and increment make goes each trip
//? expect: 0
//? source: ...

let t = "abc";
let start = 0;
let used = 0;
for (let i = 0; length(t + string(i)) > 0 && i < 20000;
    i = i + 1 + length(t + string(i)) * 0)
{
    if (i == 100) { start = pool_bytes_in_use(); }
    if (i == 19999) { used = pool_bytes_in_use() - start; }
}
assert(used < 1000000, "for (let) temporaries released per trip");
let j = 0;
start = pool_bytes_in_use();
for (j = 0; j < 20000; j = j + 1 + length(t + string(j)) * 0) { }
used = pool_bytes_in_use() - start;
assert(used < 1000000, "for (assign) temporaries released per trip");
return 0;

//?
//? test: for-of-target-per-trip
//? description: for-of code points made for the target go each trip
//? expect: 20000
//? source: ...

let s = "";
for (let i = 0; i < 2000; i = i + 1) { s = s + "abcdefghij"; }
let n = 0;
let start = 0;
let used = 0;
for (const c of s) {
    n = n + 1;
    if (n == 100) { start = pool_bytes_in_use(); }
    if (n == 20000) { used = pool_bytes_in_use() - start; }
}
assert(used < 1000000, "for-of target values released per trip");
return n;

//?
//? test: do-while-continue-evaluates-condition
//? description: continue in do while still evaluates the condition
//? expect: 3
//? source: ...

let n = 0;
do {
    n = n + 1;
    if (n < 5) { continue; }
} while (n < 3);
return n;

//?
//? test: for-of-outer-target-keeps-last
//? description: a for-of target outside the loop keeps the last value
//? expect: 3
//? source: ...

let c = 0;
for (c of [1, 2, 3]) { }
return c;

//?
//? test: closures-see-their-trip
//? description: closures made in each trip see that trip's names
//? expect: 0
//? source: ...

let fs = [];
for (let i = 0; i < 3; i = i + 1) { push(fs, function () { return i; }); }
for (const x of [10, 20]) { push(fs, function () { return x; }); }
let k = 0;
while (k < 2) { const y = k * 100; push(fs, function () { return y; }); k = k + 1; }
assert(stringify(map(function (f) { return f(); }, fs)) ===
    "[0,1,2,10,20,0,100]");
return 0;

//?
//? test: condition-error-names-the-loop
//? description: a non-boolean condition still names its loop
//? expect: 0
//? source: ...

const check = function (src, name) {
    try {
        evaluate(compile<script>(script(src)));
    }
    catch (e) {
        assert(includes<string>(e.message, "function '" + name + "'"),
            e.message);
        return;
    }
    assert(false, "no error from " + src);
};
check("let i = \"x\"; while (i) { break; }", "while");
check("let i = \"x\"; do { } while (i);", "do_while");
check("for (let i = 0; \"x\"; i = i + 1) { }", "for");
return 0;

//?
//? test: loops-decompile-round-trip
//? description: loop shapes decompile and recompile to the same text
//? expect: 0
//? source: ...

const check = function (src) {
    const d1 = decompile(compile<script>(script(src)));
    const d2 = decompile(compile<script>(script(d1)));
    assert(d1 == d2, d1);
    assert(stringify(evaluate(compile<script>(script(d1)))) ==
        stringify(evaluate(compile<script>(script(src)))), d1);
};
check("let i = 0; while (i < 3) i = i + 1; return i;");
check("let i = 0; let r = 0; while (i < 3) { r = r + i; i = i + 1; } return r;");
check("let i = 0; do { i = i + 1; } while (i < 3); return i;");
check("let r = 0; for (let i = 0; i < 3; i = i + 1) { r = r + i; } return r;");
check("let r = 0; for (const x of [1, 2, 3]) { r = r + x; } return r;");
check("let r = 0; for (let i = 0; i < 3; i = i + 1); for (let i = 0; i < 3; i = i + 1) { } return r;");
check("let i = 0; for (i = 0; i < 3; i = i + 1); let c = 0; for (c of [1, 2]); for (const x of [1]) { } return [i, c];");
check("if (true) { } else { return 2; } return 1;");
check("let fs = []; for (let i = 0; i < 3; i = i + 1) { push(fs, function () { return i; }); } return map(function (f) { return f(); }, fs);");
check("let fs = []; for (const x of [1, 2]) { push(fs, function () { return x; }); } return map(function (f) { return f(); }, fs);");
check("let r = 0; if (true) { } else { r = 2; } return r;");
check("let n = 0; try { throw \"e\"; } catch (e) { for (let c of [1, 2]) { n = n + c; } } return n;");
return 0;

//?
//? test: for-let-without-increment-copies-each-trip
//? description: for (let) with no increment still copies its names each trip
//? expect: 0
//? source: ...

let fs = [];
for (let i = 0; i < 3; ) { push(fs, function () { return i; }); i = i + 1; }
assert(stringify(map(function (f) { return f(); }, fs)) === "[1,2,3]");
return 0;

//?
//? test: for-empty-bodies
//? description: empty and missing for bodies, with and without head names
//? expect: 0
//? source: ...

let t = "abc";
let i = 0;
let start = pool_bytes_in_use();
for (i = 0; i < 20000; i = i + 1 + length(t + string(i)) * 0);
let used = pool_bytes_in_use() - start;
assert(used < 1000000, "assign-for with ; body released per trip");
let n = 0;
function inc() { n = n + 1; return n; }
for (let j = 0; j < 3; j = j + inc());
for (let j = 0; j < 3; j = j + inc()) { }
let c = 0;
for (c of [1, 2, 3]);
for (const x of [1, 2]) { }
assert(stringify([i, n, c]) === "[20000,3,3]");
return 0;

//?
//? test: loop-alone-in-for-body
//? description: a loop that is the only statement of a for body is not that for's head
//? expect: 0
//? source: ...

let a = [1, 2];
let r = [];
for (let k = 0; k < 2; k = k + 1) {
    for (const x of a) { push(r, function () { return [k, x]; }); }
}
let c = 0;
let n = 0;
for (let k = 0; k < 2; k = k + 1) { for (c of a) { n = n + k; } }
let m = 0;
for (let k = 0; k < 2; k = k + 1) { for (let j = 0; j < 2; j = j + 1) { m = m + j; } }
assert(stringify(map(function (f) { return f(); }, r)) ===
    "[[0,1],[0,2],[1,1],[1,2]]");
assert(stringify([c, n, m]) === "[2,2,2]");
return 0;
