#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: higher_order_array.as
//? customPurpose: Part of language/script tests
//? description: ...
Edge cases for higher-order array functions (map, filter, find, reduce,
every/some, all_of/any_of, sort): empty arrays, undefined entries, omitted
literal elements, identity map on homogeneous strings, and mixed types.
//? sourceType: script
//?
//? test: map-empty
//? description: map of empty array is empty
//? expect: 0
//? source: ...

let out = map(function (v) { return v; }, []);
assert(length(out) === 0);
return 0;

//?
//? test: map-identity-strings
//? description: identity map on string array keeps each element
//? expect: 0
//? source: ...

let out = map(function (v) { return v; }, ["a", "b", "c"]);
assert(length(out) === 3);
assert(out[0] === "a" && out[1] === "b" && out[2] === "c");
return 0;

//?
//? test: map-identity-integers
//? description: identity map on integer array
//? expect: 0
//? source: ...

let out = map(function (v) { return v; }, [10, 20, 30]);
assert(out[0] === 10 && out[1] === 20 && out[2] === 30);
return 0;

//?
//? test: map-transform
//? description: map builds new values
//? expect: 0
//? source: ...

let out = map(function (v) { return v + 1; }, [1, 2, 3]);
assert(out[0] === 2 && out[1] === 3 && out[2] === 4);
return 0;

//?
//? test: map-undefined-and-elision
//? description: map visits undefined and omitted elements
//? expect: 0
//? source: ...

let out1 = map(function (v) {
    if (v === undefined) {
        return "U";
    }
    return v;
}, ["a", undefined, "b"]);
assert(length(out1) === 3);
assert(out1[0] === "a" && out1[1] === "U" && out1[2] === "b");

let out2 = map(function (v) {
    if (v === undefined) {
        return "U";
    }
    return v;
}, ["a", , "b"]);
assert(length(out2) === 3);
assert(out2[0] === "a" && out2[1] === "U" && out2[2] === "b");
return 0;

//?
//? test: map-mixed-types
//? description: map over mixed-type array
//? expect: 0
//? source: ...

let out = map(function (v) { return v; }, [1, "x", true]);
assert(out[0] === 1 && out[1] === "x" && out[2] === true);
return 0;

//?
//? test: filter-empty
//? description: filter of empty array is empty
//? expect: 0
//? source: ...

let out = filter(function (v) { return true; }, []);
assert(length(out) === 0);
return 0;

//?
//? test: filter-keep-some
//? description: filter keeps matching integers
//? expect: 0
//? source: ...

let out = filter(function (n) { return n > 1; }, [1, 2, 3, 0]);
assert(length(out) === 2);
assert(out[0] === 2 && out[1] === 3);
return 0;

//?
//? test: filter-keep-undefined
//? description: filter can keep undefined entries
//? expect: 0
//? source: ...

let out = filter(function (v) { return v === undefined; }, ["a", undefined, "b", , "c"]);
assert(length(out) === 2);
assert(out[0] === undefined && out[1] === undefined);
return 0;

//?
//? test: filter-drop-undefined
//? description: filter can drop undefined entries
//? expect: 0
//? source: ...

let out = filter(function (v) { return v !== undefined; }, ["a", undefined, "b"]);
assert(length(out) === 2);
assert(out[0] === "a" && out[1] === "b");
return 0;

//?
//? test: find-first
//? description: find returns first matching value
//? expect: 0
//? source: ...

let v = find(function (n) { return n > 2; }, [1, 3, 5]);
assert(v === 3);
return 0;

//?
//? test: find-none
//? description: find returns undefined when nothing matches
//? expect: 0
//? source: ...

let v = find(function (n) { return n > 10; }, [1, 2, 3]);
assert(v === undefined);
return 0;

//?
//? test: find-undefined-entry
//? description: find can return an undefined array entry
//? expect: 0
//? source: ...

let v = find(function (x) { return x === undefined; }, ["a", undefined, "b"]);
assert(v === undefined);
/* still found first undefined — distinguish from not found via length of filter */
let kept = filter(function (x) { return x === undefined; }, ["a", undefined, "b"]);
assert(length(kept) === 1);
return 0;

//?
//? test: find-empty
//? description: find on empty array is undefined
//? expect: 0
//? source: ...

assert(find(function (v) { return true; }, []) === undefined);
return 0;

//?
//? test: reduce-sum
//? description: reduce sums integers
//? expect: 0
//? source: ...

let s = reduce(function (acc, n) { return acc + n; }, 0, [1, 2, 3, 4]);
assert(s === 10);
return 0;

//?
//? test: reduce-empty
//? description: reduce empty array returns accumulator
//? expect: 0
//? source: ...

assert(reduce(function (acc, n) { return acc + n; }, 42, []) === 42);
return 0;

//?
//? test: reduce-with-undefined
//? description: reduce visits undefined entries
//? expect: 0
//? source: ...

let count = reduce(function (acc, v) {
    return acc + 1;
}, 0, ["a", undefined, "b"]);
assert(count === 3);
return 0;

//?
//? test: every-empty
//? description: every on empty is true
//? expect: 0
//? source: ...

assert(every(function (n) { return false; }, []) === true);
assert(all_of(function (n) { return false; }, []) === true);
return 0;

//?
//? test: some-empty
//? description: some on empty is false
//? expect: 0
//? source: ...

assert(some(function (n) { return true; }, []) === false);
assert(any_of(function (n) { return true; }, []) === false);
return 0;

//?
//? test: every-with-undefined
//? description: every sees undefined as a real entry
//? expect: 0
//? source: ...

assert(every(function (v) { return v !== undefined; }, ["a", "b"]) === true);
assert(every(function (v) { return v !== undefined; }, ["a", undefined, "b"]) === false);
return 0;

//?
//? test: some-with-undefined
//? description: some can match undefined
//? expect: 0
//? source: ...

assert(some(function (v) { return v === undefined; }, ["a", "b"]) === false);
assert(some(function (v) { return v === undefined; }, ["a", , "b"]) === true);
return 0;

//?
//? test: sort-integers
//? description: sort integers ascending (compare returns boolean a < b)
//? expect: 0
//? source: ...

let out = sort(function (a, b) { return a < b; }, [3, 1, 2]);
assert(length(out) === 3);
assert(out[0] === 1 && out[1] === 2 && out[2] === 3);
return 0;

//?
//? test: sort-empty-untyped
//? description: sort requires a single-type array; empty [] is untyped and errors
//? expect: error
//? source: ...

sort(function (a, b) { return a < b; }, []);
return 0;

//?
//? test: sort-strings
//? description: sort homogeneous strings
//? expect: 0
//? source: ...

let out = sort(function (a, b) { return a < b; }, ["c", "a", "b"]);
assert(out[0] === "a" && out[1] === "b" && out[2] === "c");
return 0;

//?
//? test: all_of_any_of_basic
//? description: all_of and any_of match every/some on one array
//? expect: 0
//? source: ...

let isPos = function (n) { return n > 0; };
assert(all_of(isPos, [1, 2, 3]) === true);
assert(all_of(isPos, [1, 0, 3]) === false);
assert(any_of(isPos, [-1, 0, 2]) === true);
assert(any_of(isPos, [-1, 0, -2]) === false);
return 0;
