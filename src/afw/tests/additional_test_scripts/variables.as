#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: variables.as
//? customPurpose: Part of afw core tests
//? description: Test variable functions (bound vs value; issue #131)
//? sourceType: script
//?
//? test: variable_exists
//? description: Bound name exists; missing name does not
//? expect: 0
//? source: ...

const myvar: string = "abc";

assert(variable_exists("myvar"), "Could not find variable!");
assert(variable_exists("myvar2") === false, "Found variable that does not exist!");

return 0;

//?
//? test: variable_exists-uninit-and-undefined
//? description: Uninit let and explicit undefined are bound (#131)
//? expect: 0
//? source: ...

let uninit: any;
let explicit_undef = undefined;
let explicit_null = null;

assert(variable_exists("uninit") === true, "uninit let should be bound");
assert(variable_exists("explicit_undef") === true, "explicit undefined should be bound");
assert(variable_exists("explicit_null") === true, "null value should be bound");
assert(variable_exists("nope") === false, "missing name");

assert(is_nullish(uninit) === true);
assert(is_defined(uninit) === false);
assert(is_nullish(explicit_undef) === true);
assert(is_defined(explicit_null) === true);

return 0;

//?
//? test: variable_exists-optional-formal
//? description: Optional formal is bound when arg omitted (#131)
//? expect: 0
//? source: ...

function f(a?) {
    assert(variable_exists("a") === true, "optional formal should be bound");
    assert(is_nullish(a) === true, "omitted optional formal is undefined");
    return 0;
}

assert(f() === 0);
return 0;

//?
//? test: variable_get
//? description: Test variable_get() for bound value
//? expect: 0
//? source: ...

const myvar: string = "def";

assert(variable_get("myvar") === "def", "variable_get() returned incorrect value!");

return 0;

//?
//? test: variable_get-default-only-if-unbound
//? description: Default applies only when name is not bound (#131)
//? expect: 0
//? source: ...

let uninit: any;
let explicit_undef = undefined;

assert(is_nullish(variable_get("uninit", "D")), "bound uninit: no default");
assert(is_nullish(variable_get("explicit_undef", "D")), "bound undefined: no default");
assert(variable_get("nope", "D") === "D", "unbound: use default");
assert(is_nullish(variable_get("nope")), "unbound without default → undefined");

return 0;

//?
//? test: variable_is_not_null
//? description: Bound and not Adaptive null; undefined is not null (#131)
//? expect: 0
//? source: ...

const myvar: string = "def";
let uninit: any;
let n = null;

assert(variable_is_not_null("myvar") === true, "string value");
assert(variable_is_not_null("uninit") === true, "undefined is not Adaptive null");
assert(variable_is_not_null("n") === false, "Adaptive null");
assert(variable_is_not_null("missing") === false, "unbound");

return 0;

//?
//? test: property_exists-undefined-value
//? description: Property present with undefined still exists
//? expect: 0
//? source: ...

let o = { "a": undefined, "b": null, "c": 1 };

assert(property_exists(o, "a") === true);
assert(property_exists(o, "b") === true);
assert(property_exists(o, "c") === true);
assert(property_exists(o, "z") === false);

assert(is_nullish(property_get(o, "a", "D")), "present undefined: no default");
assert(property_get(o, "z", "D") === "D", "missing: default");

assert(property_is_not_null(o, "a") === true, "undefined is not null");
assert(property_is_not_null(o, "b") === false);
assert(property_is_not_null(o, "z") === false);

return 0;
