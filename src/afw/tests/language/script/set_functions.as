#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: set_functions.as
//? customPurpose: Part of core function tests
//? description: union, intersection, subset, set_equals, at_least_one_member_of
//? sourceType: script
//?
//? test: set-functions
//? description: results as sets (duplicates, order)
//? expect: 0
//? source: ...

assert(stringify(union<string>(["a", "b", "a"], ["b", "c"])) === '["a","b","c"]');
assert(stringify(union<integer>([1], [2, 1], [3])) === '[1,2,3]');
assert(stringify(intersection<string>(["a", "b", "a"], ["a", "c"])) === '["a"]');
assert(subset<string>(["a", "a"], ["a", "b"]) === true);
assert(subset<string>(["a", "c"], ["a", "b"]) === false);
assert(set_equals<string>(["a", "b", "a"], ["b", "a"]) === true);
assert(at_least_one_member_of<string>(["x", "b"], ["a", "b"]) === true);
return 0;

//? test: empty-literal-array
//? description: [] is an empty set of the function's data type (it threw "must have a data type")
//? expect: 0
//? source: ...

assert(stringify(union<string>([], ["a"])) === '["a"]', "union first");
assert(stringify(union<string>(["a"], [])) === '["a"]', "union second");
assert(stringify(union<string>([], [])) === '[]', "union both");
assert(stringify(intersection<string>(["a"], [])) === '[]', "intersection");
assert(subset<string>([], ["a"]) === true, "[] subset");
assert(subset<string>(["a"], []) === false, "subset of []");
assert(set_equals<string>([], []) === true, "set_equals [] []");
assert(set_equals<string>([], ["a"]) === false, "set_equals [] a");
assert(at_least_one_member_of<string>([], ["a"]) === false, "at_least_one");
return 0;
