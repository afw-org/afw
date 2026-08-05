#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: variable_get.as
//? customPurpose: Part of miscellaneous category tests
//? description: ...
variable_get: bound values, missing defaults, object/array default isolation
(issue #110 faces / #17).
//? sourceType: script
//?
//? test: variable_get
//? description: Test variable_get for an existing variable
//? skip: false
//? expect: 0
//? source: ...

let result: string = "x";

assert(variable_get("result") === "x");
assert(variable_get("result", "default") === "x");
assert(variable_get("result", { ignore: true }) === "x");

return 0;

//?
//? test: variable_get-missing-default
//? description: variable_get returns default or undefined when variable is missing
//? skip: false
//? expect: 0
//? source: ...

assert(variable_get("absent_scalar_110", "default") === "default");
assert(is_nullish(variable_get("absent_no_default_110")));
assert(variable_get("absent_int_110", 7) === 7);

return 0;

//?
//? test: variable_get-default-object-face
//? description: ...
Default object from variable_get is a mutable face; mutations must not leak
(issue #110 / #17).
//? skip: false
//? expect: 0
//? source: ...

const d1 = variable_get("no_such_var_issue110", {});
d1.z = "mutated";
d1.n = 1;

const d2 = variable_get("no_such_var_issue110", {});
assert(
    is_nullish(property_get(d2, "z", null)),
    "variable_get default object residual z"
);
assert(
    is_nullish(property_get(d2, "n", null)),
    "variable_get default object residual n"
);
assert(d1.z === "mutated");
assert(d1 !== d2);

return 0;

//?
//? test: variable_get-default-array-face
//? description: Default array from variable_get isolated across calls
//? skip: false
//? expect: 0
//? source: ...

const a1 = variable_get("no_arr_110", []);
push(a1, 1);
assert(length(a1) === 1);

const a2 = variable_get("no_arr_110", []);
assert(length(a2) === 0, "variable_get default array must not retain push");

return 0;

//?
//? test: variable_get-default-nested-onGetObject-shape
//? description: ...
Same nested default pattern as property_get issue #110 via variable_get.
//? skip: false
//? expect: 0
//? source: ...

function shape() {
    const y = variable_get("never_bound_y_110", {});
    if (is_nullish(property_get(y, "z", null))) {
        y.z = {};
    }
    return y;
}

const first = shape();
first.z.marker = "from_first";
const second = shape();
assert(
    is_nullish(property_get(second.z, "marker", null)),
    "variable_get nested default residual"
);

return 0;

//?
//? test: variable_get-default-multi-call-function
//? description: Function using variable_get default twice must isolate
//? skip: false
//? expect: 0
//? source: ...

function once() {
    const o = variable_get("unbound_once_110", { n: 0 });
    o.n = o.n + 1;
    return o.n;
}

assert(once() === 1);
assert(once() === 1, "second call must not see n already 1 on default face");

return 0;
