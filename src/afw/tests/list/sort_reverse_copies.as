#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: sort_reverse_copies.as
//? customPurpose: Part of core function tests
//? description: ...
sort and reverse change the array in place, and to_sorted, to_reversed,
to_spliced and with_at make copies, as in ECMAScript (#599).
//? sourceType: script
//?
//? test: sort-in-place
//? description: sort orders the array itself and returns it
//? expect: 0
//? source: ...

let a = [3, 1, 2];
const r = sort(a, function (x, y) { return x < y; });
assert(eq(a, [1, 2, 3]), "the array is sorted");
assert(eq(r, [1, 2, 3]), "and returned");
push(r, 4);
assert(length(a) === 4, "the same array");
return 0;

//?
//? test: sort-integer-comparator
//? description: a comparator may return an integer, as in ECMAScript
//? expect: 0
//? source: ...

let a = [3, 1, 2];
sort(a, function (x, y) { return x - y; });
assert(eq(a, [1, 2, 3]));
sort(a, function (x, y) { return y - x; });
assert(eq(a, [3, 2, 1]));
let o = [{k: 1, n: "a"}, {k: 0, n: "b"}, {k: 1, n: "c"}, {k: 0, n: "d"}];
sort(o, function (x, y) { return x.k - y.k; });
assert(eq(map(o, function (v) { return v.n; }), ["b", "d", "a", "c"]), "stable");
return 0;

//?
//? test: sort-stable-boolean
//? description: equal entries keep their order with a boolean comparator
//? expect: 0
//? source: ...

let o = [{k: 1, n: "a"}, {k: 0, n: "b"}, {k: 1, n: "c"}, {k: 0, n: "d"}];
sort(o, function (x, y) { return x.k < y.k; });
assert(eq(map(o, function (v) { return v.n; }), ["b", "d", "a", "c"]));
return 0;

//?
//? test: sort-one-call-per-comparison
//? description: a merge sort calls the comparator at most n log2 n times
//? expect: 0
//? source: ...

let a = [];
for (let i = 0; i < 64; i = i + 1) {
    push(a, (i * 37) % 64);
}
let calls = 0;
sort(a, function (x, y) { calls = calls + 1; return x < y; });
assert(calls <= 64 * 6, "calls: " + to_string(calls));
for (let i = 0; i < 64; i = i + 1) {
    assert(a[i] === i);
}
return 0;

//?
//? test: sort-comparator-must-return-boolean-or-integer
//? description: a double, or anything else, from the comparator is an error
//? expect: 0
//? source: ...

for (const f of [
    function (x, y) { return 0.5; },
    function (x, y) { return "a"; }
]) {
    let message = "";
    try { sort([2, 1], f); } catch (e) { message = e.message; }
    assert(message === "compareFn must return a boolean or an integer", "got: " + message);
}
return 0;

//?
//? test: sort-throw-leaves-array
//? description: a comparator that throws leaves the array as it was
//? expect: 0
//? source: ...

let a = [3, 1, 2];
let n = 0;
try {
    sort(a, function (x, y) { n = n + 1; if (n === 2) { throw "stop"; } return x < y; });
} catch (e) {
}
assert(eq(a, [3, 1, 2]));
return 0;

//?
//? test: sort-frozen
//? description: sort and reverse of a frozen array throw before the comparator is called
//? expect: 0
//? source: ...

const a = freeze([2, 1]);
let calls = 0;
let message = "";
try { sort(a, function (x, y) { calls = calls + 1; return x < y; }); } catch (e) { message = e.message; }
assert(index_of(message, "to_sorted") >= 0, "got: " + message);
assert(calls === 0);
message = "";
try { reverse(a); } catch (e) { message = e.message; }
assert(index_of(message, "to_reversed") >= 0, "got: " + message);
assert(eq(a, [2, 1]));
assert(eq(to_sorted(a, function (x, y) { return x < y; }), [1, 2]));
assert(eq(to_reversed(a), [1, 2]));
return 0;

//?
//? test: reverse-in-place
//? description: reverse reverses the array itself and returns it
//? expect: 0
//? source: ...

let a = ["a", "b", "c", "d"];
const r = reverse(a);
assert(eq(a, ["d", "c", "b", "a"]));
push(r, "z");
assert(length(a) === 5, "the same array");
assert(eq(reverse([]), []));
assert(eq(reverse([1]), [1]));
assert(eq([1, 2, 3]->reverse(), [3, 2, 1]), "a literal");
return 0;

//?
//? test: to-sorted
//? description: to_sorted returns a sorted copy and leaves the array alone
//? expect: 0
//? source: ...

const a = [3, 1, 2];
const s = to_sorted(a, function (x, y) { return x - y; });
assert(eq(s, [1, 2, 3]));
assert(eq(a, [3, 1, 2]));
assert(eq(to_sorted([], function (x, y) { return x < y; }), []));
assert(meta(to_sorted(["b", "a"], function (x, y) { return x < y; })[0]).dataType === "string");
return 0;

//?
//? test: to-reversed
//? description: to_reversed returns a reversed copy and leaves the array alone
//? expect: 0
//? source: ...

const a = [1, 2, 3];
assert(eq(to_reversed(a), [3, 2, 1]));
assert(eq(a, [1, 2, 3]));
assert(eq(a->to_reversed(), [3, 2, 1]));
return 0;

//?
//? test: to-spliced
//? description: to_spliced returns a copy with the changes splice would make
//? expect: 0
//? source: ...

const a = [1, 2, 3, 4];
assert(eq(to_spliced(a, 1, 2), [1, 4]));
assert(eq(to_spliced(a, 1, 1, "x", "y"), [1, "x", "y", 3, 4]));
assert(eq(to_spliced(a, -1), [1, 2, 3]));
assert(eq(to_spliced(a, 2), [1, 2]));
assert(eq(to_spliced(a, 0, 0, 0), [0, 1, 2, 3, 4]));
assert(eq(a, [1, 2, 3, 4]), "unchanged");
let b = [1, 2, 3];
assert(eq(splice(b, 1, 1), [2]), "splice still returns what it removed");
assert(eq(b, [1, 3]));
return 0;

//?
//? test: with-at
//? description: with_at returns a copy with one value replaced; out of range is an error
//? expect: 0
//? source: ...

const a = [1, 2, 3];
assert(eq(with_at(a, 1, 9), [1, 9, 3]));
assert(eq(with_at(a, -1, 9), [1, 2, 9]));
assert(eq(a, [1, 2, 3]), "unchanged");
assert(with_at(a, 0, undefined)[0] === undefined);
for (const i of [3, -4]) {
    let message = "";
    try { with_at(a, i, 0); } catch (e) { message = e.message; }
    assert(index_of(message, "out of range for an array of length 3") >= 0, "got: " + message);
}
return 0;

//?
//? test: copies-share-entries
//? description: the copies are shallow, as in ECMAScript: entries are shared, not cloned
//? expect: 0
//? source: ...

let inner = [1];
const c = to_reversed([inner, 2]);
push(c[1], 5);
assert(eq(inner, [1, 5]));
return 0;

//?
//? test: copies-are-not-narrowed
//? description: a copy of an array that happens to hold one data type can hold others, as in ECMAScript
//? expect: 0
//? source: ...

const a = [1, 2];
for (const c of [
    slice(a, 0),
    to_reversed(a),
    to_sorted(a, function (x, y) { return x < y; }),
    to_spliced(a, 0, 0),
    with_at(a, 0, 1)
]) {
    push(c, "x");
    assert(length(c) === 3);
}
assert(eq(to_spliced(a, 1, 0, "x"), [1, "x", 2]));
assert(eq(with_at(a, 0, "x"), ["x", 2]));
return 0;

//?
//? test: sort-default-order
//? description: with no compareFn, entries are ordered as < orders them, not as strings (#599)
//? expect: 0
//? source: ...

assert(eq(sort([10, 9, 1]), [1, 9, 10]), "integers, not \"1\", \"10\", \"9\"");
assert(eq(sort(["b", "a", "c"]), ["a", "b", "c"]));
assert(eq(sort([true, false]), [false, true]));
assert(eq(sort([2.5, -1.0, 0.0]), [-1.0, 0.0, 2.5]));
assert(eq(sort(["2021-01-01"date, "2020-01-01"date]), ["2020-01-01"date, "2021-01-01"date]));
assert(eq(sort([]), []));
assert(eq(to_sorted([3, 1, 2]), [1, 2, 3]));
assert(eq([3, 1, 2]->sort(), [1, 2, 3]));
return 0;

//?
//? test: sort-default-objects-and-arrays
//? description: with no compareFn, objects and arrays are ordered as < orders them
//? expect: 0
//? source: ...

const o = [{a: 2}, {a: 1}];
assert(({a: 1} < {a: 2}) === true, "the < this relies on");
assert(eq(sort(o), [{a: 1}, {a: 2}]));
assert(([1] < [2]) === true, "the < this relies on");
assert(eq(sort([[2], [1]]), [[1], [2]]));
return 0;

//?
//? test: sort-default-mixed-throws
//? description: with no compareFn, a mix of data types is an error, as it is for <
//? expect: 0
//? source: ...

for (const a of [[1, "a"], [1, 1.5]]) {
    let message = "";
    try { sort(a); } catch (e) { message = e.message; }
    assert(index_of(message, "pass a compareFn") >= 0, "got: " + message);
}
assert(eq(sort([1, "a"], function (x, y) { return to_string(x) < to_string(y); }), [1, "a"]),
    "a compareFn says how to order a mix");
return 0;

//?
//? test: sort-default-nan
//? description: with no compareFn, NaN is an error: it has no order (#628)
//? expect: error:sort with no compareFn orders entries as < does, and entry 0 is NaN, which has no order; pass a compareFn, or filter it out with is_NaN(v)
//? source: ...

return sort([NaN, 2.0, NaN, -1.0]);

//?
//? test: sort-undefined-last
//? description: undefined entries go last and are not passed to compareFn, as in ECMAScript
//? expect: 0
//? source: ...

let calls = 0;
const r = sort([3, undefined, 1, , 2], function (x, y) {
    calls = calls + 1;
    assert(x !== undefined && y !== undefined, "compareFn gets defined entries");
    return x < y;
});
assert(r[0] === 1 && r[1] === 2 && r[2] === 3);
assert(r[3] === undefined && r[4] === undefined && length(r) === 5);
assert(calls > 0);
const d = sort([undefined, "b", "a"]);
assert(d[0] === "a" && d[1] === "b" && d[2] === undefined);
assert(eq(sort([undefined]), [undefined]));
return 0;
