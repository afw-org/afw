#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: higher_order_array.as
//? customPurpose: Part of language/script tests
//? description: ...
Edge cases for higher-order array functions (map, filter, find,
find_index, reduce, every/some, all_of/any_of, sort): the array first and
the callback's arguments (#599), empty arrays, undefined entries, omitted
literal elements, identity map on homogeneous strings, mixed types,
sort of already-sorted / reverse / duplicate input, and a returned
closure as the functor.
//? sourceType: script
//?
//? test: map-empty
//? description: map of empty array is empty
//? expect: 0
//? source: ...

let out = map([], function (v) { return v; });
assert(length(out) === 0);
return 0;

//?
//? test: map-identity-strings
//? description: identity map on string array keeps each element
//? expect: 0
//? source: ...

let out = map(["a", "b", "c"], function (v) { return v; });
assert(length(out) === 3);
assert(out[0] === "a" && out[1] === "b" && out[2] === "c");
return 0;

//?
//? test: map-identity-integers
//? description: identity map on integer array
//? expect: 0
//? source: ...

let out = map([10, 20, 30], function (v) { return v; });
assert(out[0] === 10 && out[1] === 20 && out[2] === 30);
return 0;

//?
//? test: map-transform
//? description: map builds new values
//? expect: 0
//? source: ...

let out = map([1, 2, 3], function (v) { return v + 1; });
assert(out[0] === 2 && out[1] === 3 && out[2] === 4);
return 0;

//?
//? test: map-filter-mutable
//? description: map and filter results are not frozen
//? expect: 0
//? source: ...

let m = map([1, 2], function (v) { return v + 1; });
push(m, 9);
assert(length(m) === 3 && m[2] === 9, "map mutable");

let f = filter([1, 2, 3], function (v) { return v > 1; });
push(f, 0);
assert(length(f) === 3 && f[2] === 0, "filter mutable");

return 0;

//?
//? test: map-undefined-and-elision
//? description: map visits undefined and omitted elements
//? expect: 0
//? source: ...

let out1 = map(["a", undefined, "b"], function (v) {
    if (v === undefined) {
        return "U";
    }
    return v;
});
assert(length(out1) === 3);
assert(out1[0] === "a" && out1[1] === "U" && out1[2] === "b");

let out2 = map(["a", , "b"], function (v) {
    if (v === undefined) {
        return "U";
    }
    return v;
});
assert(length(out2) === 3);
assert(out2[0] === "a" && out2[1] === "U" && out2[2] === "b");
return 0;

//?
//? test: map-mixed-types
//? description: map over mixed-type array
//? expect: 0
//? source: ...

let out = map([1, "x", true], function (v) { return v; });
assert(out[0] === 1 && out[1] === "x" && out[2] === true);
return 0;

//?
//? test: filter-empty
//? description: filter of empty array is empty
//? expect: 0
//? source: ...

let out = filter([], function (v) { return true; });
assert(length(out) === 0);
return 0;

//?
//? test: filter-keep-some
//? description: filter keeps matching integers
//? expect: 0
//? source: ...

let out = filter([1, 2, 3, 0], function (n) { return n > 1; });
assert(length(out) === 2);
assert(out[0] === 2 && out[1] === 3);
return 0;

//?
//? test: filter-keep-undefined
//? description: filter can keep undefined entries
//? expect: 0
//? source: ...

let out = filter(["a", undefined, "b", , "c"], function (v) { return v === undefined; });
assert(length(out) === 2);
assert(out[0] === undefined && out[1] === undefined);
return 0;

//?
//? test: filter-drop-undefined
//? description: filter can drop undefined entries
//? expect: 0
//? source: ...

let out = filter(["a", undefined, "b"], function (v) { return v !== undefined; });
assert(length(out) === 2);
assert(out[0] === "a" && out[1] === "b");
return 0;

//?
//? test: find-first
//? description: find returns first matching value
//? expect: 0
//? source: ...

let v = find([1, 3, 5], function (n) { return n > 2; });
assert(v === 3);
return 0;

//?
//? test: find-none
//? description: find returns undefined when nothing matches
//? expect: 0
//? source: ...

let v = find([1, 2, 3], function (n) { return n > 10; });
assert(v === undefined);
return 0;

//?
//? test: find-undefined-entry
//? description: find can return an undefined array entry
//? expect: 0
//? source: ...

let v = find(["a", undefined, "b"], function (x) { return x === undefined; });
assert(v === undefined);
/* still found first undefined — distinguish from not found via length of filter */
let kept = filter(["a", undefined, "b"], function (x) { return x === undefined; });
assert(length(kept) === 1);
return 0;

//?
//? test: find-empty
//? description: find on empty array is undefined
//? expect: 0
//? source: ...

assert(find([], function (v) { return true; }) === undefined);
return 0;

//?
//? test: reduce-sum
//? description: reduce sums integers
//? expect: 0
//? source: ...

let s = reduce([1, 2, 3, 4], function (acc, n) { return acc + n; }, 0);
assert(s === 10);
return 0;

//?
//? test: reduce-empty
//? description: reduce empty array returns accumulator
//? expect: 0
//? source: ...

assert(reduce([], function (acc, n) { return acc + n; }, 42) === 42);
return 0;

//?
//? test: reduce-with-undefined
//? description: reduce visits undefined entries
//? expect: 0
//? source: ...

let count = reduce(["a", undefined, "b"], function (acc, v) {
    return acc + 1;
}, 0);
assert(count === 3);
return 0;

//?
//? test: every-empty
//? description: every on empty is true
//? expect: 0
//? source: ...

assert(every([], function (n) { return false; }) === true);
assert(all_of(function (n) { return false; }, []) === true);
return 0;

//?
//? test: some-empty
//? description: some on empty is false
//? expect: 0
//? source: ...

assert(some([], function (n) { return true; }) === false);
assert(any_of(function (n) { return true; }, []) === false);
return 0;

//?
//? test: every-with-undefined
//? description: every sees undefined as a real entry
//? expect: 0
//? source: ...

assert(every(["a", "b"], function (v) { return v !== undefined; }) === true);
assert(every(["a", undefined, "b"], function (v) { return v !== undefined; }) === false);
return 0;

//?
//? test: some-with-undefined
//? description: some can match undefined
//? expect: 0
//? source: ...

assert(some(["a", "b"], function (v) { return v === undefined; }) === false);
assert(some(["a", , "b"], function (v) { return v === undefined; }) === true);
return 0;

//?
//? test: sort-integers
//? description: sort integers ascending (compare returns boolean a < b)
//? expect: 0
//? source: ...

let out = sort([3, 1, 2], function (a, b) { return a < b; });
assert(length(out) === 3);
assert(out[0] === 1 && out[1] === 2 && out[2] === 3);
return 0;

//?
//? test: sort-empty-untyped
//? description: as in ECMAScript, sort needs no typed array: [] sorts to [] (it threw "sort() requires array to be typed"), and a mixed array sorts by the compare function
//? expect: 0
//? source: ...

assert(length(sort([], function (a, b) { return a < b; })) === 0, "[]");
const mixed = sort([2, "a", 1], function (a, b) { return string(a) < string(b); });
assert(stringify(mixed) === '[1,2,"a"]', stringify(mixed));
return 0;

//?
//? test: all-of-all-untyped
//? description: as in ECMAScript every / some, the pair functions need no typed arrays (they threw "array1 and array2 must both be typed")
//? expect: 0
//? source: ...

assert(all_of_all(eq<integer>, [], [1]) === true, "all_of_all [] is true");
assert(all_of_all(eq<integer>, [], []) === true, "all_of_all [] []");
assert(any_of_any(eq<integer>, [], [1]) === false, "any_of_any [] is false");
assert(all_of_any(eq<integer>, [1], []) === false, "all_of_any with nothing to match");
assert(any_of_all(function (a, b) { return string(a) == string(b); }, ["1", 1], [1]) === true, "mixed");
return 0;

//?
//? test: pair-functions-xacml-examples
//? description: the XACML examples; any_of_all is some value of array1 with every value of array2, the predicate getting the array1 value first (it swapped the arrays and the arguments, so this was false)
//? expect: 0
//? source: ...

assert(any_of_all(gt<integer>, [3, 5], [1, 2]) === true, "any_of_all 3 > 1 and 3 > 2");
assert(any_of_all(gt<integer>, [1, 2], [3, 5]) === false, "any_of_all none");
assert(all_of_any(gt<integer>, [10, 20], [1, 3, 5, 19]) === true, "all_of_any");
assert(all_of_all(gt<integer>, [6, 7], [1, 2, 3, 4, 5]) === true, "all_of_all");
assert(any_of_any(eq<string>, ["Ringo", "Mary"], ["John", "Paul", "George", "Ringo"]) === true, "any_of_any");
assert(any_of_all(function (a, b) { return a == 3 && b < 3; }, [3, 5], [1, 2]) === true, "argument order");
return 0;

//?
//? test: sort-strings
//? description: sort homogeneous strings
//? expect: 0
//? source: ...

let out = sort(["c", "a", "b"], function (a, b) { return a < b; });
assert(out[0] === "a" && out[1] === "b" && out[2] === "c");
return 0;

//?
//? test: sort-already-sorted
//? description: already-sorted integers stay in order
//? expect: 0
//? source: ...

let out = sort([1, 2, 3, 4, 5], function (a, b) { return a < b; });
assert(out[0] === 1 && out[4] === 5);
assert(out[1] === 2 && out[2] === 3 && out[3] === 4);
return 0;

//?
//? test: sort-reverse-sorted
//? description: reverse-sorted integers become ascending
//? expect: 0
//? source: ...

let out = sort([5, 4, 3, 2, 1], function (a, b) { return a < b; });
assert(out[0] === 1 && out[1] === 2 && out[2] === 3);
assert(out[3] === 4 && out[4] === 5);
return 0;

//?
//? test: sort-duplicates
//? description: equal integers stay together
//? expect: 0
//? source: ...

let out = sort([2, 1, 2, 1, 2], function (a, b) { return a < b; });
assert(length(out) === 5);
assert(out[0] === 1 && out[1] === 1);
assert(out[2] === 2 && out[3] === 2 && out[4] === 2);
return 0;

//?
//? test: sort-single
//? description: one-element typed array is unchanged
//? expect: 0
//? source: ...

let out = sort([7], function (a, b) { return a < b; });
assert(length(out) === 1 && out[0] === 7);
return 0;

//?
//? test: sort-already-sorted-64
//? description: already-sorted 64-integer array stays in order
//? expect: 0
//? source: ...

let a = [0];
for (let i = 1; i < 64; i = i + 1) {
    push(a, i);
}
let out = sort(a, function (x, y) { return x < y; });
assert(length(out) === 64);
for (let i = 0; i < 64; i = i + 1) {
    assert(out[i] === i);
}
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

//?
//? test: returned-closure-as-functor
//? description: map, filter, reduce, and sort evaluate a factory once and call the returned closure
//? expect: 0
//? source: ...

/*
 * Companion to function.as function_return_closure_as_argument, which
 * passes a returned closure into a regular function. High-level array
 * functions evaluate the functor parameter once, then call that value
 * per entry. reduce and sort use their own execute paths.
 */
let makes = 0;

function makeAdder() {
    makes = makes + 1;
    let n = 5;
    return function (v) {
        return v + n;
    };
}

function makeGt() {
    let min = 2;
    return function (v) {
        return v > min;
    };
}

function makeSum() {
    let extra = 10;
    return function (acc, v) {
        return acc + v + extra;
    };
}

function makeBefore() {
    return function (a, b) {
        return a < b;
    };
}

let mapped = map([1, 2, 3], makeAdder());
assert(makes === 1, "functor factory runs once");
assert(length(mapped) === 3);
assert(mapped[0] === 6 && mapped[1] === 7 && mapped[2] === 8);

let kept = filter([1, 2, 3, 4], makeGt());
assert(length(kept) === 2);
assert(kept[0] === 3 && kept[1] === 4);

let summed = reduce([1, 2, 3], makeSum(), 0);
assert(summed === 36);

let sorted = sort([3, 1, 2], makeBefore());
assert(length(sorted) === 3);
assert(sorted[0] === 1 && sorted[1] === 2 && sorted[2] === 3);
return 0;

//?
//? test: functor-adds-to-array-terminates
//? description: A functor that adds to the array it is called over visits only the entries there at the start (it used to loop forever)
//? expect: 0
//? source: ...

let a = [1, 2, 3];
assert(length(map(a, function (e) { push(a, e); return e; })) === 3, "map");
assert(length(a) === 6, "map pushed");

a = [1, 2, 3];
assert(length(filter(a, function (e) { unshift(a, 0); return true; })) === 3, "filter");

a = [1, 2, 3];
assert(reduce(a, function (acc, e) { push(a, 1); return acc + e; }, 0) === 6, "reduce");

a = [1, 2];
assert(all_of_all(function (p, q) { push(a, 1); return true; }, a, [1, 2]) === true, "all_of_all");
return 0;

//?
//? test: functor-over-array-of-functions
//? description: map over an array of functions passes each function as is (was "Invalid function_value")
//? expect: 0
//? source: ...

const f = function () { return 1; };
const g = function () { return 2; };
const r = map([f, g], function (h) { return h(); });
assert(r[0] === 1 && r[1] === 2, "closures: " + stringify(r));
const b = map([abs], function (h) { return h(-3); });
assert(b[0] === 3, "built-in: " + stringify(b));
return 0;

//?
//? test: sort-is-stable
//? description: sort keeps the order of entries that compare equal (as ECMAScript does)
//? expect: 0
//? source: ...

let a = [];
for (let i = 0; i < 40; i = i + 1) { push(a, {k: i % 3, i: i}); }
const r = sort(a, function (x, y) { return x.k < y.k; });
for (let j = 1; j < length(r); j = j + 1) {
    assert(r[j - 1].k <= r[j].k, "sorted");
    if (r[j - 1].k == r[j].k) { assert(r[j - 1].i < r[j].i, "stable at " + string(j)); }
}
return 0;

//?
//? test: reduce-keeps-only-the-current-accumulator
//? description: ...
reduce frees each accumulator once the next one is made, so memory follows
the result, not the sum of every accumulator (#517: it grew with the square
of the length, about 8.8 MB for 4000 entries).
//? expect: 0
//? source: ...

let a = create_array(4000);
let start = pool_bytes_in_use();
let r = reduce(a, function (acc, x) { return acc + "x"; }, "");
let used = pool_bytes_in_use() - start;
assert(length(r) === 4000);
assert(used < 200000, "reduce memory: " + string(used));
return 0;

//?
//? test: call-args-cleared-under-type-check
//? description: reduce and the pair functions build their call with cleared arguments, so compile-time type checking of a script function does not read unset ones (valgrind)
//? expect: 0
//? source: ...

#compile typeCheck;
assert(reduce(["x", "y"], function (a, c) { return a + c; }, "") === "xy");
assert(any_of_any(function (a, b) { return a == b; }, [1, 2], [2]));
assert(all_of_all(function (a, b) { return a > b; }, [6, 5], [1, 2]));
return 0;

//?
//? test: script-callback-gets-index-and-array
//? description: a script function callback is called with (value, index, array), as in ECMAScript (#599)
//? expect: 0
//? source: ...

const a = [10, 20, 30];
assert(eq(map(a, function (v, i, arr) { return v + i + length(arr); }), [13, 24, 35]));
assert(eq(filter(a, function (v, i) { return i !== 1; }), [10, 30]));
assert(find(a, function (v, i) { return i === 2; }) === 30);
assert(find_index(a, function (v, i, arr) { return arr[i] === 20; }) === 1);
assert(some(a, function (v, i) { return i === 2; }));
assert(!every(a, function (v, i) { return i < 2; }));
assert(reduce(a, function (acc, v, i, arr) { return acc + i * length(arr); }, 0) === 9);
assert(eq(map(a, function (v) { return v; }), a), "a callback that wants the value alone");
return 0;

//?
//? test: built-in-callback-gets-value-alone
//? description: a built-in callback is called with the value alone (reduce: accumulator and value), so its other parameters keep their own meanings (#599)
//? expect: 0
//? source: ...

assert(eq(map([1, -2], abs<integer>), [1, 2]));
assert(eq(map([1, 2], to_string), ["1", "2"]));
assert(eq(filter([1, -2, 3], function (v) { return v > 0; }), [1, 3]));
assert(reduce([1, 2, 3], add<integer>) === 6);
assert(reduce([1, 2, 3], add<integer>, 10) === 16);
let message = "";
try { map([1, 2], add<integer>); } catch (e) { message = e.message; }
assert(index_of(message, "expects 2 required parameters") >= 0,
    "add<integer> is not quietly given the index: " + message);
return 0;

//?
//? test: array-first-chains
//? description: the array is the first parameter, so a->fn(…) chains (#599)
//? expect: 0
//? source: ...

const roles = ["user", "admin"];
assert(roles->some(function (r) { return r === "admin"; }));
assert(roles->every(function (r) { return length(r) > 3; }));
assert(roles->find_index(function (r) { return r === "admin"; }) === 1);
const out = [1, 2, 3, 4]
    ->filter(function (v) { return v % 2 === 0; })
    ->map(function (v, i, arr) { return v * 10 + length(arr); });
assert(eq(out, [22, 42]));
assert([1, 2, 3]->reduce(function (acc, v) { return acc + v; }) === 6);
assert(eq([3, 1, 2]->sort(function (a, b) { return a < b; }), [1, 2, 3]));
return 0;

//?
//? test: reduce-without-initial-value
//? description: with no initialValue the first entry is the first accumulator; an empty array then throws (#599)
//? expect: 0
//? source: ...

let calls = 0;
assert(reduce(["a", "b", "c"], function (acc, v, i) { calls = calls + 1; return acc + v + to_string(i); }) === "ab1c2");
assert(calls === 2, "the first entry is not passed as a value");
assert(reduce([7], function (acc, v) { return 0; }) === 7, "one entry: no call");
assert(reduce([], function (acc, v) { return 0; }, undefined) === undefined, "undefined passed is an initialValue");
let message = "";
try { reduce([], function (acc, v) { return acc; }); } catch (e) { message = e.message; }
assert(index_of(message, "reduce of an empty array with no initialValue") >= 0, "got: " + message);
return 0;

//?
//? test: find-index
//? description: find_index returns the index of the first entry the predicate accepts, or -1 (#599)
//? expect: 0
//? source: ...

assert(find_index([1, 3, 5], function (v) { return v > 2; }) === 1);
assert(find_index([1, 2], function (v) { return v > 9; }) === -1);
assert(find_index([], function (v) { return true; }) === -1);
assert(find_index(["a", undefined], function (v) { return v === undefined; }) === 1,
    "an undefined entry is told apart from not found");
let message = "";
try { find_index([1], function (v) { return 1; }); } catch (e) { message = e.message; }
assert(message !== "", "the predicate must return a boolean");
return 0;

//?
//? test: some-every-stop-early
//? description: some stops at the first true and every at the first false
//? expect: 0
//? source: ...

let calls = 0;
assert(some([1, 2, 3], function (v) { calls = calls + 1; return v === 2; }));
assert(calls === 2, "some");
calls = 0;
assert(!every([1, 2, 3], function (v) { calls = calls + 1; return v < 2; }));
assert(calls === 2, "every");
return 0;

//?
//? test: callback-index-type-check
//? description: compile-time type checking accepts a callback that declares the index and array, or only the value
//? expect: 0
//? source: ...

#compile typeCheck;
assert(eq(map([1, 2], function (v: integer, i: integer, arr: array): integer { return v + i; }), [1, 3]));
assert(eq(map([1, 2], function (v: integer): integer { return v; }), [1, 2]));
assert(reduce([1, 2], function (acc: integer, v: integer, i: integer): integer { return acc + v + i; }, 0) === 4);
return 0;
