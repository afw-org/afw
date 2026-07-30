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
//? test: metas-object-length-and-at
//? description: metas(object) length and at match properties
//? expect: 0
//? source: ...

let o = { "a": 1, "b": "two", "c": true };
let m = metas(o);

assert(length(m) === 3, "metas length");
assert(length(m) === length(keys(o)), "matches keys length");
assert(length(m) === length(values(o)), "matches values length");

let m0 = at(m, 0);
assert(property_get(m0, "dataType") === "integer", "dataType 0");
assert(property_get(m0, "key") === "a", "key 0");
assert(property_get(m0, "value") === 1, "value 0");

let m1 = m->at(1);
assert(property_get(m1, "dataType") === "string", "dataType 1");
assert(property_get(m1, "key") === "b", "key 1");
assert(property_get(m1, "value") === "two", "value 1");

let m2 = at(m, -1);
assert(property_get(m2, "dataType") === "boolean", "dataType last");
assert(property_get(m2, "key") === "c", "key last");
assert(property_get(m2, "value") === true, "value last");

assert(is_nullish(at(m, 3)), "oob");
assert(is_nullish(at(m, -4)), "oob neg");

return 0;

//?
//? test: metas-object-for-of-and-keys
//? description: for-of metas(object) and keys/entries on each meta object
//? expect: 0
//? source: ...

let o = { "a": 1, "b": "two" };
let m = metas(o);

let names = [];
let i = 0;
let ok = keys(o);
let ov = values(o);
for (let meta of m) {
    let k = keys(meta);
    assert(length(k) >= 2, "meta has keys");
    assert(property_exists(meta, "value"), "has value");
    assert(property_exists(meta, "dataType"), "has dataType");
    assert(property_exists(meta, "key"), "has key");
    push(names, property_get(meta, "key"));
    assert(property_get(meta, "key") === ok[i], "key matches keys(o)");
    assert(property_get(meta, "value") === ov[i], "value matches values(o)");
    i = i + 1;
}
assert(i === 2, "visited two");
assert(names[0] === ok[0] && names[1] === ok[1], "key order");

let first = at(m, 0);
let fe = entries(first);
assert(length(fe) >= 2, "meta entries");
let fv = values(first);
assert(length(fv) >= 2, "meta values");

return 0;

//?
//? test: metas-object-empty
//? description: metas of empty object
//? expect: 0
//? source: ...

let m = metas({});
assert(length(m) === 0, "empty");
assert(is_nullish(at(m, 0)), "no entry");
let n = 0;
for (let x of m) {
    n = n + 1;
}
assert(n === 0, "for-of empty");

return 0;

//?
//? test: metas-object-values-and-every
//? description: every/some over metas(object) values
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
//? description: metas(array) view rejects mutation
//? expect: error
//? source: ...

let m = metas([1, 2, 3]);
push(m, {});

//?
//? test: metas-immutable-view-pop
//? description: pop on metas(array) view throws
//? expect: error
//? source: ...

let m = metas([1]);
pop(m);

//?
//? test: metas-object-immutable-view
//? description: metas(object) view rejects push
//? expect: error
//? source: ...

let m = metas({ "a": 1, "b": 2 });
push(m, {});

//?
//? test: metas-object-immutable-view-pop
//? description: pop on metas(object) view throws
//? expect: error
//? source: ...

let m = metas({ "a": 1 });
pop(m);

//?
//? test: metas-with-meta-on-elements
//? description: compare metas(array) entry value with meta of element
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
//? test: metas-object-with-meta-on-properties
//? description: compare metas(object) entry with meta of property value
//? expect: 0
//? source: ...

let o = { "greeting": "hello" };
let m = metas(o);
let entryMeta = at(m, 0);
let direct = meta(o.greeting);

assert(property_get(entryMeta, "dataType") === property_get(direct, "dataType"),
    "dataType match");
assert(property_get(entryMeta, "value") === property_get(direct, "value"),
    "value match");
assert(property_get(entryMeta, "key") === "greeting", "property key");
assert(property_get(direct, "key") === "greeting", "meta key from property");

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
//? test: metas-nested-object
//? description: metas of object of objects; nested value is object
//? expect: 0
//? source: ...

let o = { "first": { "n": 1 }, "second": { "n": 2 } };
let m = metas(o);
assert(length(m) === 2, "two metas");

let v0 = property_get(at(m, 0), "value");
assert(property_get(at(m, 0), "dataType") === "object", "nested dataType");
assert(v0.n === 1, "nested object value");
assert(length(keys(v0)) === 1, "nested keys");

let nestedMetas = metas(v0);
assert(length(nestedMetas) === 1, "nested metas length");
assert(property_get(at(nestedMetas, 0), "key") === "n", "nested prop key");
assert(property_get(at(nestedMetas, 0), "value") === 1, "nested prop value");

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

//?
//? test: meta-object-value
//? description: meta() on an object lists dataType object and value
//? expect: 0
//? source: ...

let o = { "a": 1, "b": "x" };
let m = meta(o);
let k = keys(m);
assert(length(k) >= 2, "has keys");
assert(property_get(m, "dataType") === "object", "object dataType");
assert(property_get(m, "value").a === 1, "value.a");
assert(property_get(m, "value").b === "x", "value.b");
assert(length(keys(property_get(m, "value"))) === 2, "value keys");

return 0;
