#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: deep_data.as
//? customPurpose: Part of miscellaneous category tests
//? description: ...
Deeply nested data built at runtime (#482). Releasing it used to
recurse once per level and overflow the C stack (SIGSEGV); container
element releases now defer past a fixed depth (256 levels). Depths
are the smallest that still matter, since deeper data only makes
valgrind slower: the release code before #482 crashed between 50000
and 100000 levels (mixed data by 50000), and the error tests need data
deep enough to exhaust the C stack headroom (about 50000 levels). Recursive operations
(stringify, decompile, string, ==, ===, clone) stop at
limitCStackHeadroomBytes with "C stack headroom exhausted", and the
data is still released afterwards.
//? sourceType: script
//?
//? test: deep-array-drop
//? description: 100000 nested arrays are built and released
//? expect: 0
//? source: ...

let a = [];
for (let i = 0; i < 100000; i = i + 1) {
    a = [a];
}
return 0;

//?
//? test: deep-object-drop
//? description: 100000 nested objects are built and released
//? expect: 0
//? source: ...

let o = {};
for (let i = 0; i < 100000; i = i + 1) {
    o = { x: o };
}
return 0;

//?
//? test: deep-mixed-drop
//? description: nested arrays of objects with siblings are released
//? expect: 0
//? source: ...

let a = [];
for (let i = 0; i < 50000; i = i + 1) {
    a = [{ x: a, y: [1, "two", { z: 3 }] }];
}
return 0;

//?
//? test: deep-replace-in-place
//? description: replacing the only reference to deep data releases it
//? expect: 0
//? source: ...

let a = [];
for (let i = 0; i < 100000; i = i + 1) {
    a = [a];
}
a = 1;
return a - 1;

//?
//? test: deep-stringify
//? description: stringify of deep data is an error, not a stack overflow
//? expect: error:C stack headroom exhausted.
//? source: ...

let a = [];
for (let i = 0; i < 100000; i = i + 1) {
    a = [a];
}
return stringify(a);

//?
//? test: deep-decompile
//? description: decompile of deep data is an error
//? expect: error:C stack headroom exhausted.
//? source: ...

let a = [];
for (let i = 0; i < 100000; i = i + 1) {
    a = [a];
}
return decompile(a);

//?
//? test: deep-equal
//? description: == of deep data is an error
//? expect: error:C stack headroom exhausted.
//? source: ...

let a = [];
for (let i = 0; i < 100000; i = i + 1) {
    a = [a];
}
return a == a;

//?
//? test: deep-clone
//? description: clone of deep data is an error
//? expect: error:C stack headroom exhausted.
//? source: ...

let o = {};
for (let i = 0; i < 100000; i = i + 1) {
    o = { x: o };
}
return clone(o);

//?
//? test: moderate-depth-works
//? description: 500 levels round-trip through stringify, json, clone, ==, decompile
//? expect: 0
//? source: ...

let a = [];
for (let i = 0; i < 500; i = i + 1) {
    a = [a];
}
const s = stringify(a);
assert(stringify(evaluate(compile<json>(json(s)))) === s);
assert(a == clone(a));
assert(length(decompile(a)) > 1000);
let o = {};
for (let i = 0; i < 500; i = i + 1) {
    o = { x: o };
}
const t = stringify(o);
assert(stringify(evaluate(compile<json>(json(t)))) === t);
return 0;
