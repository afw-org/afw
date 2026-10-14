#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/flat_map.as
//? customPurpose: Part of core function tests
//? description: Tests for flat_map: map, then flatten one level.
//? sourceType: script
//?
//? test: flat_map-basic
//? description: An array result adds its entries.
//? expect: [1, 1, 2, 2]
//? source: ...

[1, 2]->flat_map(function(v) { return [v, v]; })

//?
//? test: flat_map-zero-or-more
//? description: An empty array adds nothing; another value adds itself.
//? expect: [1, 3, 3]
//? source: ...

[1, 2, 3]->flat_map(function(v) { return v == 2 ? [] : v == 3 ? [v, v] : v; })

//?
//? test: flat_map-one-level
//? description: Only one level.
//? expect: [[1]]
//? source: ...

[1]->flat_map(function(v) { return [[v]]; })

//?
//? test: flat_map-index
//? description: A script function gets the index.
//? expect: [0, 1]
//? source: ...

["a", "b"]->flat_map(function(v, i) { return [i]; })
