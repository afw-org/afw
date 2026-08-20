#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: property_name_values.as
//? customPurpose: Part of language/script tests
//? description: ...
Object property names are values. Script/JSON are string-only: integer
object keys throw (no as_utf8 autoconvert). Array indexes stay integer.
Generated afw_v_* names match unmanaged/JSON string names via
afw_value_equal.
//? sourceType: script
//?
//? test: generated-name-match
//? description: Identifier, quoted, and computed string names are the same key
//? expect: 0
//? source: ...

const o = { foo: 1 };
assert(o.foo === 1);
assert(o["foo"] === 1);
const k = "foo";
assert(o[k] === 1);
assert(property_get(o, "foo") === 1);
assert(property_exists(o, "foo") === true);
return 0;

//?
//? test: two-unmanaged-strings-same-text
//? description: JSON-parsed string key matches script identifier (same text)
//? expect: 0
//? source: ...

const o = compile(json("{\"status\": 7}"));
assert(o.status === 7);
assert(o["status"] === 7);
const k = "status";
assert(o[k] === 7);
return 0;

//?
//? test: json-string-keys
//? description: JSON object keys are strings; lookup by string works
//? expect: 0
//? source: ...

const o = compile(json("{\"1\": 9, \"a\": 2}"));
assert(o["1"] === 9);
assert(o.a === 2);
assert(o["a"] === 2);
return 0;

//?
//? test: json-numeric-looking-key-is-string
//? description: JSON key 1 is the string "1", not integer 1
//? expect: 0
//? source: ...

const o = compile(json("{\"1\": true}"));
assert(o["1"] === true);
assert(property_exists(o, "1") === true);
return 0;

//?
//? test: object-integer-index-throws
//? description: obj[1] on an object is a type error, not lookup of "1"
//? expect: error
//? source: ...

const o = { a: 1 };
return o[1];

//?
//? test: object-integer-assign-throws
//? description: Assign through obj[1] throws (no convert to "1")
//? expect: error
//? source: ...

const o = {};
o[1] = "x";
return 0;

//?
//? test: object-construct-integer-name-throws
//? description: { [1]: x } throws (no convert to "1")
//? expect: error
//? source: ...

return { [1]: "x" };

//?
//? test: array-integer-index-still-works
//? description: arr[1] is still an integer index
//? expect: 0
//? source: ...

const a = [10, 20, 30];
assert(a[1] === 20);
assert(a[0] === 10);
assert(a[-1] === 30);
return 0;

//?
//? test: object-string-one-vs-integer
//? description: Object key "1" is not found via integer 1
//? expect: error
//? source: ...

const o = compile(json("{\"1\": 9}"));
return o[1];

//?
//? test: object-boolean-index-throws
//? description: obj[true] is a type error, not lookup of "true"
//? expect: error
//? source: ...

const o = { "true": 1 };
return o[true];

//?
//? test: object-construct-boolean-name-throws
//? description: { [true]: x } throws (no convert to "true")
//? expect: error
//? source: ...

return { [true]: "x" };
