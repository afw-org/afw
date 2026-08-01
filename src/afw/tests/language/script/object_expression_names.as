#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: object_expression_names.as
//? customPurpose: Part of language/script tests
//? description: Expression property names in object values (issue #38)
//? sourceType: script
//?
//? test: expression-name-variable
//? description: Property name from a variable expression
//? skip: false
//? expect: 0
//? source: ...

const k = "status";
const o = { [k]: "ok", id: 1 };
assert(o.status === "ok");
assert(o.id === 1);
assert(o[k] === "ok");
return 0;

//?
//? test: expression-name-string-literal
//? description: Bracket name with string literal (non-identifier)
//? skip: false
//? expect: 0
//? source: ...

const o = { ["Customer Name"]: "Ada", n: 2 };
assert(o["Customer Name"] === "Ada");
assert(o.n === 2);
return 0;

//?
//? test: expression-name-concat
//? description: Property name from string concatenation
//? skip: false
//? expect: 0
//? source: ...

const n = 3;
const o = { ["col " + string(n)]: 42 };
assert(o["col 3"] === 42);
return 0;

//?
//? test: expression-name-template
//? description: Property name from template string
//? skip: false
//? expect: 0
//? source: ...

const n = 7;
const o = { [`prefix_${n}`]: true };
assert(o.prefix_7 === true);
assert(o[`prefix_${n}`] === true);
return 0;

//?
//? test: expression-name-mix-static-and-spread
//? description: Static keys, expression key, and spread; last wins
//? skip: false
//? expect: 0
//? source: ...

const base = { a: 1, b: 2 };
const k = "b";
const o = {
    ...base,
    c: 3,
    [k]: 99
};
assert(o.a === 1);
assert(o.b === 99);
assert(o.c === 3);
return 0;

//?
//? test: expression-name-before-static
//? description: Expression name first then static properties
//? skip: false
//? expect: 0
//? source: ...

const k = "x";
const o = { [k]: 10, y: 20 };
assert(o.x === 10);
assert(o.y === 20);
return 0;

//?
//? test: expression-name-nested-object
//? description: Nested object value under expression property name
//? skip: false
//? expect: 0
//? source: ...

const k = "child";
const o = { [k]: { inner: 5 } };
assert(o.child.inner === 5);
assert(o[k].inner === 5);
return 0;

//?
//? test: expression-name-expression-value
//? description: Both name and value are expressions
//? skip: false
//? expect: 0
//? source: ...

const k = "sum";
const o = { [k]: 1 + 2 };
assert(o.sum === 3);
return 0;

//?
//? test: expression-name-matches-assign
//? description: Object value form matches obj[k] = v assignment
//? skip: false
//? expect: 0
//? source: ...

const k = "field";
const via_assign = {};
via_assign[k] = "v";
const via_literal = { [k]: "v" };
assert(via_assign[k] === via_literal[k]);
assert(via_literal.field === "v");
return 0;

//?
//? test: literal-meta-still-sideband
//? description: Literal _meta_ still installs sideband (not a normal property)
//? skip: false
//? expect: 0
//? source: ...

const o = {
    _meta_: { objectId: "x1" },
    a: 1
};
assert(o.a === 1);
assert(property_exists(o, "_meta_") === false);
return 0;

//?
//? test: expression-name-meta-string-is-property
//? description: Computed name _meta_ is a normal property (not sideband)
//? skip: false
//? expect: 0
//? source: ...

const k = "_meta_";
const o = { [k]: 123 };
assert(property_exists(o, "_meta_") === true);
assert(o["_meta_"] === 123);
return 0;
