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
