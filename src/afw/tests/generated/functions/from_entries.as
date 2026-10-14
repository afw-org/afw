#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/from_entries.as
//? customPurpose: Part of core function tests
//? description: Tests for from_entries: the other half of entries.
//? sourceType: script
//?
//? test: from_entries-basic
//? description: [key, value] pairs.
//? expect: ({"a": 1, "b": "x"})
//? source: ...

from_entries([["a", 1], ["b", "x"]])

//?
//? test: from_entries-last-wins
//? description: The last value of a key wins.
//? expect: ({"a": 3, "b": 2})
//? source: ...

from_entries([["a", 1], ["b", 2], ["a", 3]])

//?
//? test: from_entries-round-trip
//? description: entries then from_entries.
//? expect: ({"x": 1, "y": [2]})
//? source: ...

from_entries({x: 1, y: [2]}->entries())

//?
//? test: from_entries-not-pair
//? description: An entry must be an array of two.
//? expect: error:from_entries entry 1 must be a [key, value] array of two
//? source: ...

from_entries([["a", 1], ["b"]])

//?
//? test: from_entries-key
//? description: A key must be a string.
//? expect: error:from_entries entry 0 key must be a string (it is not converted); write to_string(v)
//? source: ...

from_entries([[1, "a"]])
