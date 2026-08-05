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
//? description: Sets on wrapper stay on face; shared base is not mutated
//? skip: false
//? expect: 0
//? source: ...

const base = { a: 1 };
const w1 = wrap_literal_object(base);
const w2 = wrap_literal_object(base);

w1.a = 99;
w1.local = "only-w1";

assert(w1.a === 99, "wrapper local override");
assert(w1.local === "only-w1", "wrapper local property");
assert(w2.a === 1, "second wrap still sees base a");
assert(is_nullish(w2.local), "second wrap has no local prop");
assert(base.a === 1, "base instance not mutated by wrapper set");

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
//? test: wrap_literal_object-future-auto-isolate
//? description: Placeholder — compiler emit / auto isolation of constant literals
//? skip: true
//? expect: 0
//? source: ...

/*
 * Future (issue #17): constant object literals should isolate across
 * evaluations without an explicit wrap_literal_object call, e.g.:
 *
 *   const f = () => { const o = { n: 0 }; o.n = o.n + 1; return o.n; };
 *   assert(f() === 1);
 *   assert(f() === 1);  // not 2 — second eval must not share mutated base
 *
 * Keep this test skipped until compiler/runtime wiring lands; then enable and
 * flesh out. Prefer covering: nested objects, empty {}, assign/param bind,
 * decompile if IR is exposed.
 */
return 0;
