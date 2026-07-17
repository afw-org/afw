#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: variable_get.as
//? customPurpose: Part of miscellaneous category tests
//? description: Test miscellaneous variable_get function, including default cloning (issue #110)
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

return 0;

//?
//? test: variable_get-missing-default
//? description: variable_get returns default or undefined when variable is missing
//? skip: false
//? expect: 0
//? source: ...

assert(variable_get("absent_scalar_110", "default") === "default");
assert(is_nullish(variable_get("absent_no_default_110")));

return 0;

//?
//? test: variable_get-default-object-clone
//? description: Default object from variable_get is cloned so mutations do not leak (issue #110)
//? skip: false
//? expect: 0
//? source: ...

// Mutating a returned default must not affect a later default for the same expression.
// Object === is structural equality, so independence is checked via mutation.
const d1 = variable_get("no_such_var_issue110", {});
d1.z = "mutated";

const d2 = variable_get("no_such_var_issue110", {});
assert(
    is_nullish(property_get(d2, "z", null)),
    "variable_get default object was not cloned; residual property present"
);
assert(d1.z === "mutated");
assert(d1 !== d2);

return 0;
