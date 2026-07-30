#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: issue55_combined.as
//? customPurpose: Part of core function tests
//? description: Combined object/array helpers scenarios for issue #55.
//? sourceType: script
//?
//? test: map-from-entries
//? description: rebuild object-like map using entries and keys
//? expect: 0
//? source: ...

let o = { "name": "ada", "id": 42 };
let names = keys(o);
let pairs = entries(o);

assert(length(names) === length(pairs), "same count");

let rebuilt = {};
for (const pair of pairs) {
    rebuilt[pair[0]] = pair[1];
}
assert(rebuilt.name === "ada", "name");
assert(rebuilt.id === 42, "id");

return 0;

//?
//? test: process-queue-with-at
//? description: queue processing using at for peek and shift for dequeue
//? expect: 0
//? source: ...

let q = [];
push(q, "job1", "job2", "job3");

assert(at(q, 0) === "job1", "peek");
assert(shift(q) === "job1", "dequeue");
assert(at(q, -1) === "job3", "last");
assert(length(q) === 2, "remaining");

return 0;

//?
//? test: freeze-then-keys-still-work
//? description: freeze does not block pure reads
//? expect: 0
//? source: ...

let o = { "a": 1, "b": 2 };
freeze(o);
assert(length(keys(o)) === 2, "keys after freeze");
assert(values(o)[0] === 1, "values after freeze");
assert(entries(o)[1][0] === "b", "entries after freeze");

return 0;

//?
//? test: filter-with-every-some
//? description: every/some compose with filter
//? expect: 0
//? source: ...

let nums = [1, 2, 3, 4, 5, 6];
let isEven = function (n) { return n % 2 === 0; };
let gt4 = function (n) { return n > 4; };
let gt10 = function (n) { return n > 10; };
let evens = filter(isEven, nums);
assert(every(isEven, evens) === true, "all even");
assert(some(gt4, evens) === true, "has > 4");
assert(some(gt10, evens) === false, "no huge");

return 0;

//?
//? test: splice-build-list
//? description: splice as insert/delete building block
//? expect: 0
//? source: ...

let a = ["a", "b", "c", "d"];
/* delete b,c insert X */
splice(a, 1, 2, "X");
assert(join(a, "") === "aXd", "join result");

/* insert at end via splice length */
splice(a, length(a), 0, "!");
assert(at(a, -1) === "!", "appended");

return 0;
