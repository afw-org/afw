#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: object_literal_wrapper.as
//? customPurpose: Part of language/script tests
//? description: wrap_literal_object and constant object isolation (issue #17)
//? sourceType: script
//?
//? test: wrap_literal_object-basic
//? description: Function evaluates object, wraps, and returns object face
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
//? description: Independent literal sites; idempotent re-wrap same face
//? skip: false
//? expect: 0
//? source: ...

/* Two separate literal sites → two faces (auto-wrap + explicit wrap). */
const w1 = wrap_literal_object({ a: 1 });
const w2 = wrap_literal_object({ a: 1 });

w1.a = 99;
w1.local = "only-w1";

assert(w1.a === 99, "wrapper local override");
assert(w1.local === "only-w1", "wrapper local property");
assert(w2.a === 1, "second face still sees its base a");
assert(is_nullish(w2.local), "second face has no local prop");

/* Idempotent: wrap of an already-wrapped face returns the same face. */
const face = wrap_literal_object({ b: 1 });
const again = wrap_literal_object(face);
face.b = 2;
assert(again.b === 2, "idempotent wrap must return the same face");

return 0;

//?
//? test: wrap_literal_object-literal-arg
//? description: Direct object literal argument works
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
//? description: Function twice; mutate object literal must not leak (issue #17 baseline)
//? skip: false
//? expect: 0
//? source: ...

/*
 * Original class of bug: script compiled once (e.g. model onGetObject), each
 * evaluation mutates a const/let object literal; next evaluation still sees
 * those properties because the compile graph held one shared bag.
 */
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
//? description: compile once, run body twice via returned function (issue #17)
//? skip: false
//? expect: 0
//? source: ...

/*
 * Top-level scripts that only mutate and return a constant object can be
 * constant-folded at compile (decompile becomes the object). Model-style
 * isolation is a function body invoked twice after one compile.
 */
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
//? description: compile function once, call twice; count must restart (issue #17 baseline)
//? skip: false
//? expect: 0
//? source: ...

/*
 * Model on* style: one compiled function value, many invocations. Avoid a
 * top-level script that constant-folds to a scalar (see count pitfall above).
 */
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
//? description: let (not const) object literal must isolate across calls (issue #17)
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
//? description: return {…} then mutate; next return must be clean (issue #17)
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
//? description: Nested object literal property must isolate across calls (issue #17)
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
//? description: y.z = {} shared empty bag across calls (issue #17 / #110 shape)
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
//? description: Object param default {} must isolate across calls (issue #17)
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
//? description: Whole-parameter default object Pattern must isolate (issue #17)
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
//? description: const { x } = { x: { n: 0 } }; mutate x across calls (issue #17)
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
//? description: Loop calling mutator; each iteration fresh literal (issue #17)
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
    /* If shared, second iteration still has hit before set — still true.
     * Detect share via residual key: */
    assert(
        is_nullish(property_get(o, "i", null)),
        "loop iteration must not see prior residual property on shared literal"
    );
    o.i = i;
}

return 0;

//?
//? test: shared-literal-lambda-two-calls
//? description: Lambda mutates object literal; two calls isolate (issue #17)
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
//? description: Empty {} gets a property; next call property_exists false (issue #17)
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
/* Independence: if a and b share, still both true — check identity or extra */
a.onlyA = 1;
assert(
    is_nullish(property_get(b, "onlyA", null)),
    "second empty literal face must not have onlyA from first"
);

return 0;
