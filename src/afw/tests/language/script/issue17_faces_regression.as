#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: issue17_faces_regression.as
//? customPurpose: Part of language/script tests
//? description: ...
Cross-path regression for issue #17 / #110 faces: mix property_get defaults,
literals, wrap helpers, and nested structure without adapter conf.
//? sourceType: script
//?
//? test: face-default-then-literal
//? description: ...
property_get default face and separate object literal faces do not cross-talk.
//? skip: false
//? expect: 0
//? source: ...

const d = property_get({}, "d", {});
d.fromDefault = true;

const lit = { fromLit: true };
lit.extra = 1;

assert(is_nullish(property_get(d, "fromLit", null)));
assert(is_nullish(property_get(lit, "fromDefault", null)));
assert(d.fromDefault === true);
assert(lit.extra === 1);

return 0;

//?
//? test: face-variable-get-default-and-const
//? description: variable_get default face vs const object independent
//? skip: false
//? expect: 0
//? source: ...

const v = variable_get("nope_17", { a: 1 });
const c = { a: 1 };
v.a = 2;
assert(c.a === 1);
c.a = 3;
assert(v.a === 2);

return 0;

//?
//? test: face-array-default-and-literal
//? description: property_get array default and array literal independent
//? skip: false
//? expect: 0
//? source: ...

const def = property_get({}, "x", [0]);
const lit = [0];
def[0] = 1;
push(def, 2);
assert(lit[0] === 0);
assert(length(lit) === 1);

return 0;

//?
//? test: face-double-wrap-literal-object
//? description: wrap_literal_object twice is idempotent
//? skip: false
//? expect: 0
//? source: ...

const base = { k: 1 };
const w1 = wrap_literal_object(base);
const w2 = wrap_literal_object(w1);
w1.k = 5;
assert(w2.k === 5);

return 0;

//?
//? test: face-double-wrap-literal-array
//? description: wrap_literal_array twice is idempotent
//? skip: false
//? expect: 0
//? source: ...

const base = [1];
const w1 = wrap_literal_array(base);
const w2 = wrap_literal_array(w1);
w1[0] = 7;
assert(w2[0] === 7);

return 0;

//?
//? test: face-object-pattern-default-param
//? description: function param default {} isolates across calls
//? skip: false
//? expect: 0
//? source: ...

function f(o = {}) {
    o.n = (is_nullish(o.n) ? 0 : o.n) + 1;
    return o.n;
}

assert(f() === 1);
assert(f() === 1);

return 0;

//?
//? test: face-array-pattern-default-param
//? description: function param default [] isolates across calls
//? skip: false
//? expect: 0
//? source: ...

function f(a = []) {
    push(a, 1);
    return length(a);
}

assert(f() === 1);
assert(f() === 1);

return 0;

//?
//? test: face-nested-object-in-object-literal
//? description: Nested object under {} isolate residual props
//? skip: false
//? expect: 0
//? source: ...

function f() {
    const o = { child: {} };
    o.child.x = 1;
    return o;
}

const a = f();
assert(a.child.x === 1);
const b = f();
assert(b.child.x === 1);
a.child.y = 2;
assert(is_nullish(property_get(b.child, "y", null)));

return 0;

//?
//? test: face-scalar-default-unchanged
//? description: Non-object defaults still behave as simple values
//? skip: false
//? expect: 0
//? source: ...

assert(property_get({}, "a", 1) === 1);
assert(property_get({}, "a", 1) === 1);
assert(variable_get("no_s", "x") === "x");
assert(variable_get("no_s2", false) === false);

return 0;

//?
//? test: face-try-catch-multi-eval
//? description: try body with object literal across two calls
//? skip: false
//? expect: 0
//? source: ...

function f() {
    try {
        const o = { ok: true };
        o.n = 1;
        return o;
    } catch (e) {
        return null;
    }
}

const a = f();
a.n = 2;
const b = f();
assert(b.n === 1, "try-body literal face independent");

return 0;
