#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: keys_values_entries.as
//? customPurpose: Part of core function tests
//? description: Tests for keys, values, and entries object functions (issue #55).
//? sourceType: script
//?
//? test: keys-basic
//? description: keys returns property names
//? expect: 0
//? source: ...

let o = { "a": 1, "b": 2, "c": 3 };
let k = keys(o);

assert(length(k) === 3, "keys length");
assert(k[0] === "a", "keys[0]");
assert(k[1] === "b", "keys[1]");
assert(k[2] === "c", "keys[2]");

/* Method form */
let k2 = o->keys();
assert(length(k2) === 3, "method keys length");

return 0;

//?
//? test: keys-empty
//? description: keys of empty object
//? expect: 0
//? source: ...

let k = keys({});
assert(length(k) === 0, "empty keys");

return 0;

//?
//? test: values-basic
//? description: values returns property values in keys order
//? expect: 0
//? source: ...

let o = { "x": 10, "y": "hello", "z": true };
let v = values(o);

assert(length(v) === 3, "values length");
assert(v[0] === 10, "values[0]");
assert(v[1] === "hello", "values[1]");
assert(v[2] === true, "values[2]");

let v2 = o->values();
assert(v2[0] === 10, "method values");

return 0;

//?
//? test: values-nested
//? description: values includes nested objects by reference shape
//? expect: 0
//? source: ...

let inner = { "n": 1 };
let o = { "inner": inner };
let v = values(o);

assert(length(v) === 1, "one value");
assert(v[0].n === 1, "nested value");

return 0;

//?
//? test: entries-basic
//? description: entries returns [name, value] pairs
//? expect: 0
//? source: ...

let o = { "a": 1, "b": 2 };
let e = entries(o);

assert(length(e) === 2, "entries length");
assert(length(e[0]) === 2, "pair length");
assert(e[0][0] === "a", "entry0 name");
assert(e[0][1] === 1, "entry0 value");
assert(e[1][0] === "b", "entry1 name");
assert(e[1][1] === 2, "entry1 value");

/* Order matches keys */
let k = keys(o);
assert(e[0][0] === k[0], "order with keys");
assert(e[1][0] === k[1], "order with keys 1");

let e2 = o->entries();
assert(e2[0][1] === 1, "method entries");

return 0;

//?
//? test: entries-empty
//? description: entries of empty object
//? expect: 0
//? source: ...

assert(length(entries({})) === 0, "empty entries");

return 0;

//?
//? test: keys-values-entries-consistent
//? description: keys/values/entries stay consistent after mutation of source object keys list is snapshot
//? expect: 0
//? source: ...

let o = { "a": 1 };
let k = keys(o);
assert(length(k) === 1, "before");

o.b = 2;
/* k is a snapshot array of names at call time */
assert(length(k) === 1, "keys snapshot unchanged");
assert(length(keys(o)) === 2, "fresh keys sees new prop");

return 0;
