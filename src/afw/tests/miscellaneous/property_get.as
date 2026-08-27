#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: property_get.as
//? customPurpose: Part of miscellaneous category tests
//? description: ...
property_get: existing/missing, scalar defaults, and object/array default
isolation (issue #110 faces / #17). Multi-call and nested residual checks.
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
assert(property_get(o, "a", { should: "ignore" }) === 1);

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
assert(property_get(o, "missing", "s") === "s");

return 0;

//?
//? test: property_get-default-object-face
//? description: ...
Default object from property_get is a mutable face; mutations must not leak
to a later default for the same expression (issue #110 / #17).
//? skip: false
//? expect: 0
//? source: ...

const y_a = property_get({}, "y", {});
y_a.z = "from_a";
y_a.extra = 1;

const y_b = property_get({}, "y", {});
assert(
    is_nullish(property_get(y_b, "z", null)),
    "property_get default object residual z"
);
assert(
    is_nullish(property_get(y_b, "extra", null)),
    "property_get default object residual extra"
);
assert(y_a.z === "from_a");
assert(y_a !== y_b);

return 0;

//?
//? test: property_get-default-array-face
//? description: ...
Default array from property_get is isolated across calls (face; issue #110/#17).
//? skip: false
//? expect: 0
//? source: ...

const a1 = property_get({}, "list", []);
push(a1, "x");
assert(length(a1) === 1);

const a2 = property_get({}, "list", []);
assert(length(a2) === 0, "default array must not retain push");
assert(a1 !== a2);

return 0;

//?
//? test: property_get-default-array-index-mutate
//? description: ...
Mutate default array by index; next property_get default must be clean.
//? skip: false
//? expect: 0
//? source: ...

const a1 = property_get({}, "arr", [0]);
a1[0] = 99;
assert(a1[0] === 99);

const a2 = property_get({}, "arr", [0]);
assert(a2[0] === 0, "default array index must not leak");

return 0;

//?
//? test: property_get-issue-110-onGetObject-shape
//? description: ...
Nested default object mutation must not poison later calls (issue #110
onGetObject shape). Nested {} assigned on the default face must not share.
//? skip: false
//? expect: 0
//? source: ...

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

//?
//? test: property_get-default-loop-isolation
//? description: ...
Loop calling property_get with {} default; each iteration independent face.
//? skip: false
//? expect: 0
//? source: ...

for (let i = 0; i < 5; i = i + 1) {
    const d = property_get({}, "k", {});
    assert(is_nullish(property_get(d, "i", null)), "loop residual on default face");
    d.i = i;
    assert(d.i === i);
}

return 0;

//?
//? test: property_get-default-shared-constant-object
//? description: ...
Default that is a variable is identity (same occupant as def).
//? skip: false
//? expect: 0
//? source: ...

const def = { base: 1 };
const a = property_get({}, "m", def);
const b = property_get({}, "m", def);
a.base = 2;
a.local = true;
assert(b.base === 2, "default that is a variable is identity");
assert(property_get(b, "local", null) === true);
assert(def.base === 2);

return 0;

//?
//? test: property_get-default-null-and-undefined-scalar
//? description: Scalar null/string defaults still work without face requirements
//? skip: false
//? expect: 0
//? source: ...

assert(property_get({}, "n", null) === null);
assert(property_get({}, "s", "hi") === "hi");
assert(property_get({}, "i", 3) === 3);
assert(property_get({}, "b", true) === true);

return 0;
