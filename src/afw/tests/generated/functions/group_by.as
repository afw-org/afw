#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/group_by.as
//? customPurpose: Part of core function tests
//? description: Tests for group_by: group entries by a string key.
//? sourceType: script
//?
//? test: group_by-basic
//? description: Entries by key, in order; properties in the order keys first appear.
//? expect: ({"odd": [1, 3, 5], "even": [2, 4]})
//? source: ...

[1, 2, 3, 4, 5]->group_by(function(v) { return v % 2 == 0 ? "even" : "odd"; })

//?
//? test: group_by-empty
//? description: An empty array gives an empty object.
//? expect: ({})
//? source: ...

[]->group_by(function(v) { return "a"; })

//?
//? test: group_by-index
//? description: A script function gets the index.
//? expect: ({"0": ["a"], "1": ["b"]})
//? source: ...

["a", "b"]->group_by(function(v, i) { return string(i); })

//?
//? test: group_by-key
//? description: The key must be a string; it is not converted.
//? expect: error:group_by callbackfn must return a string key, but for entry 0 it returned integer; write to_string(v)
//? source: ...

[1]->group_by(function(v) { return v; })
