#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: meta_values.as
//? customPurpose: Part of core function tests
//? description: Tests for metas() lazy meta-values arrays (array/object views) with issue #55 helpers.
//? sourceType: script
//?
//? test: metas-array-length-and-at
//? description: metas(array) length and at match entries
//? expect: 0
//? source: ...

let a = [10, "x", true];
let m = metas(a);

assert(length(m) === 3, "metas length");
assert(length(m) === length(a), "same length as array");

let m0 = at(m, 0);
assert(property_get(m0, "dataType") === "integer", "dataType 0");
assert(property_get(m0, "key") === "0", "key 0");
assert(property_get(m0, "value") === 10, "value 0");

let m1 = m->at(1);
assert(property_get(m1, "dataType") === "string", "dataType 1");
assert(property_get(m1, "value") === "x", "value 1");

let m2 = at(m, -1);
assert(property_get(m2, "dataType") === "boolean", "dataType last");
assert(property_get(m2, "value") === true, "value last");

assert(is_nullish(at(m, 3)), "oob");
assert(is_nullish(at(m, -4)), "oob neg");

return 0;

//?
//? test: metas-array-for-of-and-keys
//? description: for-of metas and keys on each meta object
//? expect: 0
//? source: ...

let a = [1, 2];
let m = metas(a);
let i = 0;
for (let meta of m) {
    let k = keys(meta);
    assert(length(k) >= 2, "meta has keys");
    assert(property_exists(meta, "value"), "has value");
    assert(property_get(meta, "value") === a[i], "value matches");
    i = i + 1;
}
assert(i === 2, "visited two");

return 0;

//?
//? test: metas-array-empty
//? description: metas of empty array
//? expect: 0
//? source: ...

let m = metas([]);
assert(length(m) === 0, "empty");
assert(is_nullish(at(m, 0)), "no entry");
let n = 0;
for (let x of m) {
    n = n + 1;
}
assert(n === 0, "for-of empty");

return 0;

//?
//? test: metas-object-length-and-entries
//? description: metas(object) with length, at, entries on meta objects
//? expect: 0
//? source: ...

let o = { "a": 1, "b": "two" };
let m = metas(o);

assert(length(m) === 2, "metas length");
assert(length(m) === length(keys(o)), "matches keys length");

let names = [];
for (let meta of m) {
    push(names, property_get(meta, "key"));
    assert(property_exists(meta, "dataType"), "dataType");
    assert(property_exists(meta, "value"), "value prop");
}

assert(length(names) === 2, "two names");
/* keys order matches object property order */
let ok = keys(o);
assert(names[0] === ok[0] && names[1] === ok[1], "key order");

let first = at(m, 0);
let fe = entries(first);
assert(length(fe) >= 2, "meta entries");

return 0;

//?
//? test: metas-object-values-and-every
//? description: every/some over metas values
//? expect: 0
//? source: ...

let o = { "x": 2, "y": 4, "z": 6 };
let m = metas(o);

let hasValue = function (meta) {
    return property_exists(meta, "value");
};
assert(every(hasValue, m) === true, "every has value");

let isEvenValue = function (meta) {
    let v = property_get(meta, "value");
    return v % 2 === 0;
};
assert(every(isEvenValue, m) === true, "all even values");

let o2 = { "x": 2, "y": 3 };
assert(some(isEvenValue, metas(o2)) === true, "some even");
assert(every(isEvenValue, metas(o2)) === false, "not all even");

return 0;

//?
//? test: metas-immutable-view
//? description: metas view rejects mutation
//? expect: error
//? source: ...

let m = metas([1, 2, 3]);
push(m, {});

//?
//? test: metas-immutable-view-pop
//? description: pop on metas view throws
//? expect: error
//? source: ...

let m = metas([1]);
pop(m);

//?
//? test: metas-with-meta-on-elements
//? description: compare metas entry value with meta of element
//? expect: 0
//? source: ...

let a = ["hello"];
let m = metas(a);
let entryMeta = at(m, 0);
let direct = meta(a[0]);

assert(property_get(entryMeta, "dataType") === property_get(direct, "dataType"),
    "dataType match");
assert(property_get(entryMeta, "value") === property_get(direct, "value"),
    "value match");

return 0;

//?
//? test: metas-nested-array
//? description: metas of array of objects; use at/keys on nested
//? expect: 0
//? source: ...

let a = [{ "n": 1 }, { "n": 2 }];
let m = metas(a);
assert(length(m) === 2, "two metas");

let v0 = property_get(at(m, 0), "value");
assert(v0.n === 1, "nested object");
assert(length(keys(v0)) === 1, "nested keys");

return 0;

//?
//? test: meta-single-value-keys
//? description: meta() on scalar has expected property listing
//? expect: 0
//? source: ...

let m = meta(42);
let k = keys(m);
assert(length(k) >= 1, "has keys");
assert(property_get(m, "dataType") === "integer", "integer");
assert(property_get(m, "value") === 42, "value");

return 0;
