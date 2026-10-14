#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/reduce_right.as
//? customPurpose: Part of core function tests
//? description: Tests for reduce_right: reduce from the last entry to the first.
//? sourceType: script
//?
//? test: reduce_right-basic
//? description: The last entry is the first accumulator.
//? expect: "cba"
//? source: ...

["a", "b", "c"]->reduce_right(function(acc, v) { return acc + v; })

//?
//? test: reduce_right-initial
//? description: With initialValue, every entry is visited, with its index.
//? expect: "x2c1b0a"
//? source: ...

["a", "b", "c"]->reduce_right(function(acc, v, i) { return acc + string(i) + v; }, "x")

//?
//? test: reduce_right-empty
//? description: An empty array with no initialValue is an error.
//? expect: error:reduce_right of an empty array with no initialValue
//? source: ...

[]->reduce_right(function(acc, v) { return acc; })

//?
//? test: reduce_right-empty-initial
//? description: An empty array with initialValue gives it.
//? expect: 1
//? source: ...

[]->reduce_right(function(acc, v) { return acc; }, 1)
//?
//? test: reduce_right-callback-changes-array
//? description: reduce_right reads each index as it is now, as ECMAScript reduceRight
//? expect: "ddcb"
//? source: ...

let a = ["a", "b", "c", "d"];
return a->reduce_right(function(acc, v, i) {
    if (i === 3) { a->shift(); }
    return acc + v;
}, "");
//?
//? test: reduce_right-callback-removes
//? description: an index a callback removed is skipped, as ECMAScript reduceRight skips a missing index
//? expect: "dba"
//? source: ...

let a = ["a", "b", "c", "d"];
return a->reduce_right(function(acc, v, i) {
    if (i === 3) { a->pop(); a->pop(); }
    return acc + v;
}, "");
