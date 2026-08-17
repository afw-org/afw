#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: object_literal_wrapper.as
//? customPurpose: Part of language/script tests
//? description: ...
Issue #17 — mutable object faces / shared object-literal isolation.

Compiler emits wrap_literal_object for top-level script object literals so each
evaluation gets a memory look-through face (sets stay local; base not poisoned).
Also covers explicit wrap_literal_object, multi-call isolation (function /
lambda / compiled function / return mutate), nested literals, param defaults,
pattern bind, and face delete / tombstone. Arrays and #110 default clone are
separate.

//? sourceType: script
//?
//? test: wrap_literal_object-basic
//? description: ...
Explicit wrap_literal_object: evaluate object arg, return object face; look-through
get for base properties; meta dataType remains object.
//? skip: false
//? expect: 0
//? source: ...

const base = { a: 1, b: "x" };
const w = wrap_literal_object(base);

assert(w.a === 1, "look-through get a");
assert(w.b === "x", "look-through get b");
assert(meta(w).dataType === "object");

return 0;

//?
//? test: wrap_literal_object-set-local-not-base
//? description: ...
Two independent wrap_literal_object({a:1}) sites get separate faces (mutate one
does not affect the other). Re-wrap of an already-wrapped face is idempotent
(same face, mutations visible through both handles).
//? skip: false
//? expect: 0
//? source: ...

const w1 = wrap_literal_object({ a: 1 });
const w2 = wrap_literal_object({ a: 1 });

w1.a = 99;
w1.local = "only-w1";

assert(w1.a === 99, "wrapper local override");
assert(w1.local === "only-w1", "wrapper local property");
assert(w2.a === 1, "second face still sees its base a");
assert(is_nullish(w2.local), "second face has no local prop");

const face = wrap_literal_object({ b: 1 });
const again = wrap_literal_object(face);
face.b = 2;
assert(again.b === 2, "idempotent wrap must return the same face");

return 0;

//?
//? test: wrap_literal_object-literal-arg
//? description: ...
Direct object literal as wrap_literal_object argument; get and set on the face.
//? skip: false
//? expect: 0
//? source: ...

const w = wrap_literal_object({ k: 7 });
assert(w.k === 7);
w.k = 8;
assert(w.k === 8);

return 0;

//?
//? test: shared-literal-function-two-calls
//? description: ...
Core isolation baseline (issue #17): named function body with const object
literal mutated each call. Second call must see a fresh face (n starts 0), not
the first call's mutated bag. Models this class of bug for scripts compiled once
and re-entered (e.g. model onGetObject).
//? skip: false
//? expect: 0
//? source: ...

function bump() {
    const o = { n: 0 };
    o.n = o.n + 1;
    return o.n;
}

assert(bump() === 1, "first call should see fresh n=0 then return 1");
assert(bump() === 1, "second call must not see first call's mutated literal");

return 0;

//?
//? test: shared-literal-compile-evaluate-twice
//? description: ...
compile<script> once of a factory that returns a function; each fn() call must
get an isolated object literal face (extra set on first result must not appear
on second). Avoids top-level scripts that constant-fold away multi-eval.
//? skip: false
//? expect: 0
//? source: ...

const compiled = compile<script>(script(
    "return function () {\n" +
    "    const o = {};\n" +
    "    o.marker = \"from_eval\";\n" +
    "    return o;\n" +
    "};\n"
));

const fn = evaluate(compiled);
const first = fn();
assert(first.marker === "from_eval");
first.extra = "only_first";

const second = fn();
assert(
    second.marker === "from_eval",
    "second call should still set marker on its face"
);
assert(
    is_nullish(property_get(second, "extra", null)),
    "second call must not retain first call's extra property on shared literal"
);
assert(
    first.extra === "only_first",
    "first result face should keep its own mutation"
);

return 0;

//?
//? test: shared-literal-compiled-function-two-calls
//? description: ...
Same multi-eval shape as model on* hooks: one compiled function value, many
invocations; count on object literal must restart each call (not climb).
//? skip: false
//? expect: 0
//? source: ...

const fn = evaluate(compile<script>(script(
    "return function () {\n" +
    "    const o = { n: 0 };\n" +
    "    o.n = o.n + 1;\n" +
    "    return o.n;\n" +
    "};\n"
)));

assert(fn() === 1, "first call of compiled function");
assert(fn() === 1, "second call must not see n already 1 on shared literal");

return 0;

//?
//? test: shared-literal-let-two-calls
//? description: ...
let-bound object literal (not only const) must isolate across function calls.
//? skip: false
//? expect: 0
//? source: ...

function bumpLet() {
    let o = { n: 0 };
    o.n = o.n + 1;
    return o.n;
}

assert(bumpLet() === 1);
assert(bumpLet() === 1, "let-bound literal must not share mutations across calls");

return 0;

//?
//? test: shared-literal-return-mutate
//? description: ...
return {…} then mutate the result; next return of the same function must be a
clean face (prior tag/extra must not leak).
//? skip: false
//? expect: 0
//? source: ...

function make() {
    return { tag: "fresh" };
}

const a = make();
assert(a.tag === "fresh");
a.tag = "dirty";
a.extra = 1;

const b = make();
assert(b.tag === "fresh", "returned literal must not keep prior mutation");
assert(is_nullish(property_get(b, "extra", null)), "returned literal must not keep extra props");
assert(a.tag === "dirty");

return 0;

//?
//? test: shared-literal-nested-object
//? description: ...
Nested object literal property (o.child) must isolate across calls; promote-on-get
/ face semantics for nested objects.
//? skip: false
//? expect: 0
//? source: ...

function nestBump() {
    const o = { child: { n: 0 } };
    o.child.n = o.child.n + 1;
    return o.child.n;
}

assert(nestBump() === 1);
assert(nestBump() === 1, "nested object literal must not share across calls");

return 0;

//?
//? test: shared-literal-nested-assign-empty
//? description: ...
Assign empty {} into a property (y.z = {}) each call; marker on first.z must not
appear on second.z. Same shape as default-object / #110 style nested bags.
//? skip: false
//? expect: 0
//? source: ...

function shape() {
    const y = {};
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
    "nested {} assigned as property must not retain marker across calls"
);

return 0;

//?
//? test: shared-literal-param-default
//? description: ...
Object parameter default {} must isolate across calls; explicit arg still works.
//? skip: false
//? expect: 0
//? source: ...

function withDefault(o = {}) {
    o.n = (is_nullish(o.n) ? 0 : o.n) + 1;
    return o.n;
}

assert(withDefault() === 1);
assert(withDefault() === 1, "default object param literal must not share across calls");
assert(withDefault({ n: 10 }) === 11, "explicit arg still works");

return 0;

//?
//? test: shared-literal-param-pattern-default
//? description: ...
Whole-parameter default object Pattern with nested bag must isolate across calls.
//? skip: false
//? expect: 0
//? source: ...

function withBagDefault({ bag } = { bag: { n: 0 } }) {
    bag.n = bag.n + 1;
    return bag.n;
}

assert(withBagDefault() === 1);
assert(withBagDefault() === 1, "pattern default nested object must not share");

return 0;

//?
//? test: shared-literal-object-pattern-bind
//? description: ...
const { x } = { x: { n: 0 } }; mutate x.n across calls — nested literal in
destructure RHS must not share.
//? skip: false
//? expect: 0
//? source: ...

function patternBind() {
    const { x } = { x: { n: 0 } };
    x.n = x.n + 1;
    return x.n;
}

assert(patternBind() === 1);
assert(patternBind() === 1, "destructured nested literal must not share");

return 0;

//?
//? test: shared-literal-loop-calls
//? description: ...
Loop invoking a mutator that starts from {}; residual property from prior
iteration must not appear on the next face.
//? skip: false
//? expect: 0
//? source: ...

function once() {
    const o = {};
    o.hit = true;
    return o;
}

for (let i = 0; i < 3; i = i + 1) {
    const o = once();
    assert(o.hit === true);
    assert(
        is_nullish(property_get(o, "i", null)),
        "loop iteration must not see prior residual property on shared literal"
    );
    o.i = i;
}

return 0;

//?
//? test: shared-literal-lambda-two-calls
//? description: ...
Lambda (function expression) body with object literal; two calls isolate like
named functions.
//? skip: false
//? expect: 0
//? source: ...

const bump = function () {
    const o = { n: 0 };
    o.n = o.n + 1;
    return o.n;
};

assert(bump() === 1);
assert(bump() === 1, "lambda body literal must not share across calls");

return 0;

//?
//? test: shared-literal-empty-property-exists
//? description: ...
Empty {} each call: property_exists starts false; after set, sibling call's face
must not see the first face's onlyA.
//? skip: false
//? expect: 0
//? source: ...

function flag() {
    const o = {};
    assert(
        property_exists(o, "seen") === false,
        "fresh empty literal must not already have seen"
    );
    o.seen = true;
    return o;
}

const a = flag();
assert(a.seen === true);
const b = flag();
assert(b.seen === true, "second call sets seen on its face");
a.onlyA = 1;
assert(
    is_nullish(property_get(b, "onlyA", null)),
    "second empty literal face must not have onlyA from first"
);

return 0;

//?
//? test: shared-literal-object-with-nested-array
//? description: ...
Object literal with nested array property; mutate array across calls must
isolate (promote nested array on face get).
//? skip: false
//? expect: 0
//? source: ...

function nestArr() {
    const o = { items: [0] };
    o.items[0] = o.items[0] + 1;
    return o.items[0];
}

assert(nestArr() === 1);
assert(nestArr() === 1, "nested array under object literal must not share");

return 0;

//?
//? test: wrap_literal_object-idempotent-after-emit
//? description: ...
const o = {} is already auto-wrapped; wrap_literal_object(o) is idempotent.
//? skip: false
//? expect: 0
//? source: ...

const o = { a: 1 };
const w = wrap_literal_object(o);
o.a = 2;
assert(w.a === 2, "idempotent wrap of auto face shares same face");

return 0;

//?
//? test: wrap_literal_object-delete-unshadowed
//? description: ...
property_delete of a never-shadowed face property must hide the wrapped base
(local NULL tombstone). has/get/keys see it gone; a missing name still returns
false; a later set on the same name works.
//? skip: false
//? expect: 0
//? source: ...

const w = wrap_literal_object({ a: 1, b: 2 });

assert(property_delete(w, "nope") === false, "missing name");
assert(property_delete(w, "a") === true, "unshadowed delete claims success");
assert(property_exists(w, "a") === false, "tombstone hides base from has");
assert(is_nullish(w.a), "tombstone hides base from get");
assert(w.b === 2, "sibling property still look-through");
const names = keys(w);
assert(length(names) === 1, "iterator skips tombstone");
assert(names[0] === "b", "iterator still yields b");

w.a = 7;
assert(w.a === 7, "set after tombstone");
assert(property_exists(w, "a") === true);

const local = wrap_literal_object({ a: 1 });
local.a = 99;
assert(property_delete(local, "a") === true, "delete after local override");
assert(property_exists(local, "a") === false);

return 0;

//?
//? test: wrap_literal_object-delete-isolates
//? description: ...
Delete on one face must not poison a sibling face or the next evaluation of
the same object literal (compile-time shared base stays intact).
//? skip: false
//? expect: 0
//? source: ...

const w1 = wrap_literal_object({ a: 1, b: 2 });
const w2 = wrap_literal_object({ a: 1, b: 2 });
assert(property_delete(w1, "a") === true);
assert(property_exists(w1, "a") === false);
assert(w2.a === 1, "sibling face still sees its base a");
assert(w2.b === 2);

function make() {
    return { a: 1 };
}

const first = make();
assert(property_delete(first, "a") === true);
assert(property_exists(first, "a") === false);
const second = make();
assert(second.a === 1, "next eval of the literal must still see a");
assert(property_exists(first, "a") === false, "first face keeps its tombstone");

const nest = { inner: { x: 1, y: 2 } };
assert(property_delete(nest.inner, "x") === true);
assert(property_exists(nest.inner, "x") === false, "nested face tombstone");
assert(nest.inner.y === 2);

return 0;
