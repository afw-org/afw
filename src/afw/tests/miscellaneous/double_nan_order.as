#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: double_nan_order.as
//? customPurpose: Part of core function tests
//? description: ...
How NaN compares (#599). The < family keeps NaN unordered, as in
ECMAScript. Underneath, the double compare is a total order (NaN after
+Infinity, equal to itself), so sort and the equality behind includes
can rely on it, and max / min return NaN for any NaN argument instead of
an answer that depended on argument order.
//? sourceType: script
//?
//? test: operators-unordered
//? description: <, <=, >, >=, ==, === are false for NaN and !=, !== true, as in ECMAScript
//? expect: 0
//? source: ...

for (const x of [NaN, 1.0, Infinity, -Infinity]) {
    assert(!(NaN < x) && !(x < NaN), "<");
    assert(!(NaN <= x) && !(x <= NaN), "<=");
    assert(!(NaN > x) && !(x > NaN), ">");
    assert(!(NaN >= x) && !(x >= NaN), ">=");
    assert(!(NaN == x) && !(x == NaN), "==");
    assert(!(NaN === x) && !(x === NaN), "===");
    assert(NaN != x && x != NaN, "!=");
    assert(NaN !== x && x !== NaN, "!==");
}
return 0;

//?
//? test: max-min-nan
//? description: max and min of doubles are NaN if any value is, wherever it is, as ECMAScript Math.max / Math.min
//? expect: 0
//? source: ...

assert(is_NaN(max<double>(NaN, 1.0)));
assert(is_NaN(max<double>(1.0, NaN)));
assert(is_NaN(max<double>(1.0, NaN, Infinity)));
assert(is_NaN(min<double>(NaN, 1.0)));
assert(is_NaN(min<double>(1.0, NaN)));
assert(is_NaN(min<double>(-Infinity, 1.0, NaN)));
assert(max<double>(1.0, Infinity, -2.0) === Infinity);
assert(min<double>(1.0, -Infinity, 2.0) === -Infinity);
return 0;

//?
//? test: sort-total-order
//? description: sort orders -Infinity, the finite values, +Infinity, then NaN
//? expect: 0
//? source: ...

const r = sort([NaN, Infinity, 1.0, NaN, -Infinity, -0.5]);
assert(r[0] === -Infinity && r[1] === -0.5 && r[2] === 1.0 && r[3] === Infinity);
assert(is_NaN(r[4]) && is_NaN(r[5]));
return 0;

//?
//? test: includes-finds-nan
//? description: includes finds NaN, as ECMAScript includes does
//? expect: 0
//? source: ...

assert(includes([1.0, NaN], NaN));
assert(!includes([1.0, 2.0], NaN));
return 0;
