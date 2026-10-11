#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: read_value_holders.as
//? customPurpose: Part of language/script tests
//? description: ...
A value read from a variable, a property, or an array element stays usable
while user code that runs before its last use (a later argument, a loop
body, a for increment) reassigns the variable or changes the container.
Whatever collects the value (an argument list, an array or object literal,
a for-of over an array) holds it (issues #556, #529).
//? sourceType: script
//?
//? test: for-of-source-reassigned
//? description: for-of keeps iterating after the variable it reads is reassigned
//? expect: 0
//? source: ...

let a = [];
for (let i = 0; i < 2000; i = i + 1) { push(a, "s" + string(i)); }
let n = 0;
for (const x of a) { a = null; n = n + length(x); }
assert(n === 8890);
return 0;

//?
//? test: operand-variable-reassigned-by-later-operand
//? description: s + g() where g reassigns s
//? expect: 0
//? source: ...

let s = "abcdefghijklmnop";
function g() { s = "zz"; return "y"; }
assert(s + g() === "abcdefghijklmnopy");
return 0;

//?
//? test: operand-element-replaced-by-later-operand
//? description: a[0] + g() where g replaces a[0]
//? expect: 0
//? source: ...

let a = ["abcdefghijklmnop"];
function g() { a[0] = "z"; return "y"; }
assert(a[0] + g() === "abcdefghijklmnopy");
return 0;

//?
//? test: operand-property-replaced-by-later-operand
//? description: o.k + g() where g replaces o.k
//? expect: 0
//? source: ...

let o = {k: "abcdefghijklmnop"};
function g() { o.k = "z"; return "y"; }
assert(o.k + g() === "abcdefghijklmnopy");
return 0;

//?
//? test: script-argument-reassigned-by-later-argument
//? description: h(x, g()) where g reassigns x
//? expect: 0
//? source: ...

let x = "abcdefghijklmnop";
function g() { x = "z"; return "y"; }
function h(p, q) { return p + q; }
assert(h(x, g()) === "abcdefghijklmnopy");
return 0;

//?
//? test: array-literal-element-reassigned-by-later-element
//? description: [s, g()] where g reassigns s
//? expect: 0
//? source: ...

let s = "abcdefghijklmnop";
function g() { s = "zz"; return "y"; }
let r = [s, g()];
assert(r[0] === "abcdefghijklmnop");
assert(r[1] === "y");
return 0;

//?
//? test: object-literal-property-replaced-by-later-property
//? description: {p: o.k, q: g()} where g replaces o.k
//? expect: 0
//? source: ...

let o = {k: "abcdefghijklmnop"};
function g() { o.k = "z"; return "y"; }
let r = {p: o.k, q: g()};
assert(r.p === "abcdefghijklmnop");
assert(r.q === "y");
return 0;

//?
//? test: array-literal-spread-source-reassigned-by-later-element
//? description: [...a, g()] where g reassigns a
//? expect: 0
//? source: ...

let a = ["abcdefghijklmnop"];
function g() { a = null; return "y"; }
let r = [...a, g()];
assert(r[0] === "abcdefghijklmnop");
assert(r[1] === "y");
return 0;

//?
//? test: for-increment-after-body-ends-in-array-call
//? description: for (i = …; i = i + 1) whose body ends in array(i)
//? expect: 0
//? source: ...

let i = 0;
for (i = 0; i < 3; i = i + 1) { array(i); }
assert(i === 3);
return 0;

//?
//? test: for-increment-after-body-ends-in-object-call
//? description: for (i = …; i = i + 1) whose body ends in object({n: i})
//? expect: 0
//? source: ...

let i = 0;
for (i = 0; i < 3; i = i + 1) { object({n: i}); }
assert(i === 3);
return 0;

//?
//? test: for-increment-after-body-ends-in-at-call
//? description: for (i = …; i = i + 1) whose body ends in at([{n: i}], 0)
//? expect: 0
//? source: ...

let i = 0;
for (i = 0; i < 3; i = i + 1) { at([{n: i}], 0); }
assert(i === 3);
return 0;

//?
//? test: closure-read-in-its-own-unit-frame
//? description: ...
Reading a closure in the frame it captured registers its release on that
frame's pool while the closure holds the frame. The scope runs that
release when it deactivates, so the cycle collector can free the closure
and its frame (#556).
//? expect: 0
//? source: ...

let g = 0;
let start = pool_bytes_in_use();
for (let i = 0; i < 1000; i = i + 1) {
    g = eval<string>("function a() { return 7; } let b = a; return b;");
}
let used = pool_bytes_in_use() - start;
assert(g() === 7);
assert(used < 1000000, "closures read in their own frame are freed: " + string(used));
return 0;

//?
//? test: closure-read-in-its-own-loop-trip-frame
//? description: a function defined and read in each loop trip stays flat (guard; does not need the deactivation order)
//? expect: 0
//? source: ...

let g = 0;
let start = pool_bytes_in_use();
for (let i = 0; i < 1000; i = i + 1) {
    function a() { return i; }
    let b = a;
    g = b;
}
let used = pool_bytes_in_use() - start;
assert(g() === 999);
assert(used < 1000000, "closures read in their own frame are freed: " + string(used));
return 0;
