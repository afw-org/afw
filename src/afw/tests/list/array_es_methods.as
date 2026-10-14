#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: array_es_methods.as
//? customPurpose: Part of core function tests
//? description: ...
ECMAScript-named functions on arrays (#599): index_of / last_index_of
with the equality includes uses, fromIndex as in ECMAScript, entries /
keys / values of an array, concat following its first argument, and log
removed.
//? sourceType: script
//?
//? test: index-of-array
//? description: index_of and last_index_of find an array entry by value, or -1
//? expect: 0
//? source: ...

const a = [1, 2, 1];
assert(index_of(a, 1) === 0);
assert(last_index_of(a, 1) === 2);
assert(index_of(a, 3) === -1 && last_index_of(a, 3) === -1);
assert(index_of([[1], {x: 1}], {x: 1}) === 1, "by value: AFW values have no identity");
assert(index_of([[1]], [1]) === 0);
assert(index_of([undefined, 1], undefined) === 0);
assert(a->index_of(2) === 1, "a->index_of");
assert(index_of("abcb", "b") === 1 && last_index_of("abcb", "b") === 3, "strings unchanged");
return 0;

//?
//? test: index-of-nan
//? description: NaN is found, as includes finds it, so includes(a, v) is always index_of(a, v) >= 0
//? expect: 0
//? source: ...

assert(index_of([1.0, NaN], NaN) === 1);
assert(last_index_of([NaN, NaN], NaN) === 1);
assert(includes([1.0, NaN], NaN));
return 0;

//?
//? test: from-index
//? description: fromIndex as in ECMAScript; negative counts from the end
//? expect: 0
//? source: ...

const a = [1, 2, 3, 1];
assert(index_of(a, 1, 1) === 3);
assert(index_of(a, 1, -1) === 3);
assert(index_of(a, 1, -9) === 0, "before the start: from 0");
assert(index_of(a, 1, 4) === -1, "at the end: not found");
assert(index_of(a, 1, 9) === -1, "past the end: not found");
assert(!includes(a, 1, 9), "includes too (it used to wrap around)");
assert(includes(a, 2, -9));
assert(last_index_of(a, 1, 2) === 0);
assert(last_index_of(a, 1, -2) === 0);
assert(last_index_of(a, 1, 9) === 3, "past the end: from the last");
assert(last_index_of(a, 1, -9) === -1, "before the start: not found");
assert(index_of([], 1, 0) === -1 && last_index_of([], 1) === -1);
return 0;

//?
//? test: entries-keys-values-array
//? description: entries, keys and values take an array, as in ECMAScript
//? expect: 0
//? source: ...

assert(eq(entries([10, 20]), [[0, 10], [1, 20]]));
assert(eq(keys(["a", "b"]), [0, 1]));
assert(eq(values(["a", "b"]), ["a", "b"]));
const k = keys([1, , 3]);
assert(eq(k, [0, 1, 2]), "a hole has a key");
assert(eq([5]->entries(), [[0, 5]]));
for (const [i, v] of entries(["x", "y"])) {
    assert(["x", "y"][i] === v);
}
assert(eq(keys({b: 1, a: 2}), ["b", "a"]), "objects unchanged");
assert(eq(entries({a: 1}), [["a", 1]]));
let message = "";
try { keys(5); } catch (e) { message = e.message; }
assert(index_of(message, "must be an object or an array") >= 0, "got: " + message);
return 0;

//?
//? test: concat-arrays
//? description: concat of an array concatenates arrays, as in ECMAScript
//? expect: 0
//? source: ...

assert(eq(concat([1], [2], [3]), [1, 2, 3]));
assert(eq([1]->concat([2, 3]), [1, 2, 3]));
assert(eq(concat([1]), [1]));
assert(eq(concat([], []), []));
assert(eq(concat([1], ["a"]), [1, "a"]), "a mix of data types");
assert(eq(concat([[1]], [[2]]), [[1], [2]]), "one level, as in ECMAScript");
const a = [1];
const c = concat(a, [2]);
assert(eq(a, [1]), "a new array");
push(c, 3);
assert(length(a) === 1);
return 0;

//?
//? test: concat-array-takes-only-arrays
//? description: concat of an array with a value that is not an array is an error, not a quiet append
//? expect: 0
//? source: ...

for (const f of [
    function () { return concat([1], 2); },
    function () { return concat([1], "ab"); },
    function () { return concat([1], [2], undefined); }
]) {
    let message = "";
    try { f(); } catch (e) { message = e.message; }
    assert(index_of(message, "write [...a, v] or push(a, v)") >= 0, "got: " + message);
}
assert(eq([...[1], 2], [1, 2]), "the way to add a value");
return 0;

//?
//? test: concat-strings
//? description: concat with anything but an array first converts every value to a string, as before
//? expect: 0
//? source: ...

assert(concat("a", 1, true) === "a1true");
assert(concat("a", [1]) === "a[1]");
assert(concat(1, 2) === "12");
assert(concat(undefined, "x") === "undefinedx");
return 0;

//?
//? test: log-removed
//? description: log was never implemented; trace, debug and print log
//? expect: 0
//? source: ...

let message = "";
try { compile<script>(script("log(1);")); } catch (e) { message = e.message; }
assert(index_of(message, "Unknown built-in function 'log'") >= 0, "got: " + message);
return 0;
