#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: property_get.as
//? customPurpose: Part of miscellaneous category tests
//? description: Test object property_get, including default cloning (issue #110)
//? sourceType: script
//?
//? test: property_get-existing
//? description: property_get returns existing property and ignores default
//? skip: false
//? expect: 0
//? source: ...

const o = { "a": 1 };

assert(property_get(o, "a") === 1);
assert(property_get(o, "a", 99) === 1);

return 0;

//?
//? test: property_get-missing-default
//? description: property_get returns default or undefined when property is missing
//? skip: false
//? expect: 0
//? source: ...

const o = { "a": 1 };

assert(property_get(o, "missing", 99) === 99);
assert(is_nullish(property_get(o, "missing")));

return 0;

//?
//? test: property_get-default-object-clone
//? description: Default object from property_get is cloned so mutations do not leak (issue #110)
//? skip: false
//? expect: 0
//? source: ...

// Mutating a returned default must not affect a later default for the same expression.
// Object === is structural equality, so independence is checked via mutation.
const y_a = property_get({}, "y", {});
y_a.z = "from_a";

const y_b = property_get({}, "y", {});
assert(
    is_nullish(property_get(y_b, "z", null)),
    "property_get default object was not cloned; residual property present"
);
assert(y_a.z === "from_a");
assert(y_a !== y_b);

return 0;

//?
//? test: property_get-issue-110-onGetObject-shape
//? description: Nested default object mutation must not poison later calls (issue #110)
//? skip: false
//? expect: 0
//? source: ...

// Contrived shape from issue #110 onGetObject (without requiring model adapter/afwfcgi).
function issue110_onGetObject_shape() {
    const x = {};
    const y = property_get(x, "y", {});

    if (is_nullish(property_get(y, "z", null))) {
        y.z = {};
    }

    return y;
}

const first = issue110_onGetObject_shape();
assert(property_exists(first, "z") === true);
first.z.marker = "from_first";

const second = issue110_onGetObject_shape();
assert(property_exists(second, "z") === true);
assert(
    is_nullish(property_get(second.z, "marker", null)),
    "issue #110: shared default object retained residual nested property"
);
assert(first.z.marker === "from_first");
assert(first !== second);

return 0;
