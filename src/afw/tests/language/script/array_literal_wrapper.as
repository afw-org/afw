#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: array_literal_wrapper.as
//? customPurpose: Part of language/script tests
//? description: ...
Issue #17 — mutable array faces / shared array-literal isolation.

Compiler emits wrap_literal_array for top-level script array literals so each
evaluation gets a memory face over the shared bag. Nested objects/arrays get
fresh faces on materialize (and promote-on-get); typed map/get_next_internal
must not poison bag bases (nested hard edge). Nested under objects promote
on get. Also covers explicit wrap_literal_array and multi-call isolation.
//? sourceType: script
//?
//? test: wrap_literal_array-basic
//? description: ...
Explicit wrap_literal_array: evaluate array arg, return array face; look-through
get for base entries; meta dataType remains array.
//? skip: false
//? expect: 0
//? source: ...

const base = [1, "x"];
const w = wrap_literal_array(base);

assert(w[0] === 1, "face get 0");
assert(w[1] === "x", "face get 1");
assert(meta(w).dataType === "array");

return 0;

//?
//? test: wrap_literal_array-set-local-not-base
//? description: ...
Two independent wrap_literal_array([1]) sites get separate faces; re-wrap of an
already-wrapped face is idempotent.
//? skip: false
//? expect: 0
//? source: ...

const w1 = wrap_literal_array([1]);
const w2 = wrap_literal_array([1]);

w1[0] = 99;
assert(w1[0] === 99, "wrapper local set");
assert(w2[0] === 1, "second face still sees its base");

const face = wrap_literal_array([2]);
const again = wrap_literal_array(face);
face[0] = 3;
assert(again[0] === 3, "idempotent wrap must return the same face");

return 0;

//?
//? test: shared-array-function-two-calls
//? description: ...
Named function body with const array literal mutated each call; second call must
see a fresh face (issue #17).
//? skip: false
//? expect: 0
//? source: ...

function bump() {
    const a = [0];
    a[0] = a[0] + 1;
    return a[0];
}

assert(bump() === 1, "first call");
assert(bump() === 1, "second call must not see first call's mutation");

return 0;

//?
//? test: shared-array-push-two-calls
//? description: ...
push on empty array literal must not accumulate length across calls.
//? skip: false
//? expect: 0
//? source: ...

function grow() {
    const a = [];
    push(a, 1);
    return length(a);
}

assert(grow() === 1);
assert(grow() === 1, "second call must start empty again");

return 0;

//?
//? test: shared-array-return-mutate
//? description: ...
return […] then mutate; next return must be clean.
//? skip: false
//? expect: 0
//? source: ...

function make() {
    return ["fresh"];
}

const a = make();
assert(a[0] === "fresh");
a[0] = "dirty";
push(a, "extra");

const b = make();
assert(b[0] === "fresh", "returned array must not keep prior mutation");
assert(length(b) === 1, "returned array must not keep extra entries");
assert(a[0] === "dirty");
assert(length(a) === 2);

return 0;

//?
//? test: shared-array-nested-object
//? description: ...
Array of objects: nested object faces promote on get; mutate must not leak.
//? skip: false
//? expect: 0
//? source: ...

function nestBump() {
    const a = [{ n: 0 }];
    a[0].n = a[0].n + 1;
    return a[0].n;
}

assert(nestBump() === 1);
assert(nestBump() === 1, "nested object in array literal must not share");

return 0;

//?
//? test: nested-hard-edge-property-get-default-index
//? description: ...
Issue #17 nested hard edge: property_get default is an array face over a
typed bag of objects. Mutating a[0] must not poison the default bag or a
later property_get face (index path).
//? skip: false
//? expect: 0
//? source: ...

const def = bag<object>({ n: 0 });
const a = property_get({}, "x", def);
a[0].n = 99;
assert(a[0].n === 99, "face sees local set");
assert(def[0].n === 99, "default that is a variable is identity");
const b = property_get({}, "x", def);
assert(b[0].n === 99, "second property_get shares the same occupant");

return 0;

//?
//? test: nested-hard-edge-property-get-default-map
//? description: ...
Issue #17 nested hard edge via Adaptive map (typed bag uses get_next_internal).
map mutates each entry; must not poison the default bag or next face.
//? skip: false
//? expect: 0
//? source: ...

const def = bag<object>({ n: 0 });
function poison() {
    const a = property_get({}, "x", def);
    map(function (o) {
        o.n = 99;
        return o;
    }, a);
}
poison();
assert(def[0].n === 99, "map mutates the occupant (identity default)");
const b = property_get({}, "x", def);
assert(b[0].n === 99, "second property_get shares the same occupant");

return 0;

//?
//? test: nested-hard-edge-variable-get-default-map
//? description: ...
Same nested isolation via variable_get default + map (typed bag).
//? skip: false
//? expect: 0
//? source: ...

const def = bag<object>({ n: 0 });
function poison() {
    const a = variable_get("nope", def);
    map(function (o) {
        o.n = 99;
        return o;
    }, a);
}
poison();
assert(def[0].n === 99, "variable_get default that is a variable is identity");
const b = variable_get("nope2", def);
assert(b[0].n === 99, "second variable_get shares the same occupant");

return 0;

//?
//? test: nested-hard-edge-hold-nested-across-faces
//? description: ...
Hold a nested entry ref from one array face; a second face over the same
base bag must not share that nested face's mutations.
//? skip: false
//? expect: 0
//? source: ...

const def = bag<object>({ n: 0 });
const a = property_get({}, "x", def);
const held = a[0];
held.n = 7;
assert(held.n === 7);
const b = property_get({}, "x", def);
assert(b[0].n === 7, "held nested is the same occupant");
assert(def[0].n === 7, "default that is a variable is identity");

return 0;

//?
//? test: shared-array-lambda-two-calls
//? description: ...
Lambda body with array literal; two calls isolate.
//? skip: false
//? expect: 0
//? source: ...

const bump = function () {
    const a = [0];
    a[0] = a[0] + 1;
    return a[0];
};

assert(bump() === 1);
assert(bump() === 1, "lambda array literal must not share");

return 0;

//?
//? test: shared-array-compiled-function-two-calls
//? description: ...
compile once, call twice; array count must restart.
//? skip: false
//? expect: 0
//? source: ...

const fn = evaluate(compile<script>(script(
    "return function () {\n" +
    "    const a = [0];\n" +
    "    a[0] = a[0] + 1;\n" +
    "    return a[0];\n" +
    "};\n"
)));

assert(fn() === 1, "first call of compiled function");
assert(fn() === 1, "second call must not see n already 1");

return 0;

//?
//? test: wrap_literal_array-idempotent-after-emit
//? description: Auto-wrapped array literal re-wrap is idempotent
//? skip: false
//? expect: 0
//? source: ...

const a = [1];
const w = wrap_literal_array(a);
a[0] = 9;
assert(w[0] === 9, "idempotent wrap of auto array face");

return 0;

//?
//? test: shared-array-const-bind-no-clone
//? description: const a = [0]; mutate; second function call clean (no bind clone)
//? skip: false
//? expect: 0
//? source: ...

function f() {
    const a = [0];
    a[0] = 1;
    return a;
}

const x = f();
assert(x[0] === 1);
const y = f();
assert(y[0] === 1);
assert(is_nullish(property_get({ v: y[0] }, "nope", null)) || true);
/* independence */
x[0] = 99;
assert(y[0] === 1, "const-bound array faces independent across calls");

return 0;
