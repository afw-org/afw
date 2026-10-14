#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/flat.as
//? customPurpose: Part of core function tests
//? description: Tests for flat: flatten nested arrays.
//? sourceType: script
//?
//? test: flat-default
//? description: The default depth is 1.
//? expect: [1, 2, [3, [4]]]
//? source: ...

[1, [2, [3, [4]]]]->flat()

//?
//? test: flat-depth
//? description: depth levels.
//? expect: [1, 2, 3, [4]]
//? source: ...

[1, [2, [3, [4]]]]->flat(2)

//?
//? test: flat-negative
//? description: A negative depth flattens every level.
//? expect: [1, 2, 3, 4]
//? source: ...

[1, [2, [3, [4]]]]->flat(-1)

//?
//? test: flat-zero
//? description: depth 0 copies.
//? expect: [1, [2]]
//? source: ...

[1, [2]]->flat(0)

//?
//? test: flat-undefined
//? description: Undefined entries are kept (no holes).
//? expect: [true, 3]
//? source: ...

(function() { const a = [undefined, [undefined]]->flat(); return [a[0] === undefined && a[1] === undefined, length([1, undefined, 3]->flat())]; })()

//?
//? test: flat-new
//? description: The result is a new array.
//? expect: [[1, [2]], [1, 2]]
//? source: ...

(function() { const a = [1, [2]]; const b = a->flat(); return [a, b]; })()
