#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: cycle_buffered.as
//? customPurpose: Part of language tests
//? description: ...
A counted instance in the possible roots is marked buffered, so a release
that leaves it there again costs one test (#563). The collector clears
the mark when it takes an instance out of the roots. These hold a cycle
across collections (each read makes it a possible root again), then drop
it: it must be collected, so memory stays flat.
//? sourceType: script
//?
//? test: object-cycle-read-across-collections
//? description: an object cycle read while collections run is collected once dropped
//? expect: 0
//? source: ...

let start = pool_bytes_in_use();
for (let i = 0; i < 2000; i = i + 1) {
    let o = {n: i};
    o.self = o;
    for (let j = 0; j < 60; j = j + 1) {
        let p = {};
        p.self = p;
        let x = o.n + o.self.n;
    }
}
let used = pool_bytes_in_use() - start;
assert(used < 500000, "object cycles are collected: " + string(used));
return 0;

//? test: array-cycle-read-across-collections
//? description: an array cycle read while collections run is collected once dropped
//? expect: 0
//? source: ...

let start = pool_bytes_in_use();
for (let i = 0; i < 2000; i = i + 1) {
    let a = [i, "s"];
    push(a, a);
    for (let j = 0; j < 60; j = j + 1) {
        let p = {};
        p.self = p;
        let x = a[0] + a[2][0];
    }
}
let used = pool_bytes_in_use() - start;
assert(used < 500000, "array cycles are collected: " + string(used));
return 0;

//? test: closure-cycle-read-across-collections
//? description: a closure that captures its own frame, read while collections run, is collected once dropped
//? expect: 0
//? source: ...

let start = pool_bytes_in_use();
for (let i = 0; i < 2000; i = i + 1) {
    function f() { return i; }
    let g = f;
    for (let j = 0; j < 60; j = j + 1) {
        let p = {};
        p.self = p;
        let x = g() + f();
    }
}
let used = pool_bytes_in_use() - start;
assert(used < 500000, "closure cycles are collected: " + string(used));
return 0;
