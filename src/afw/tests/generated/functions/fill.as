#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/fill.as
//? customPurpose: Part of core function tests
//? description: Tests for fill: set entries in place and return the array.
//? sourceType: script
//?
//? test: fill-all
//? description: Every entry.
//? expect: [0, 0, 0]
//? source: ...

[1, 2, 3]->fill(0)

//?
//? test: fill-range
//? description: start up to end; negative counts from the end.
//? expect: [1, 0, 0, 4]
//? source: ...

[1, 2, 3, 4]->fill(0, 1, -1)

//?
//? test: fill-clamp
//? description: Indexes out of range are clamped; the length does not change.
//? expect: [1, 9, 9]
//? source: ...

[1, 2, 3]->fill(9, 1, 10)

//?
//? test: fill-in-place
//? description: The same array is changed.
//? expect: [7, 7]
//? source: ...

(function() { let a = [1, 2]; a->fill(7); return a; })()

//?
//? test: fill-function
//? description: A function is stored, not called.
//? expect: true
//? source: ...

is_function(create_array(1)->fill(function() { return 1; })[0])

//?
//? test: fill-frozen
//? description: A frozen array is an error.
//? expect: error:fill of a frozen array
//? source: ...

freeze([1])->fill(2)
