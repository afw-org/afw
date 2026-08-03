#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: function_contextual.as
//? customPurpose: Part of compiler category tests
//? description: ...
Built-in execute paths that create nested calls (map/reduce/filter/sort,
switch predicate, stringify replacer) must pass AFW_FUNCTION_CONTEXTUAL
instead of NULL so nested call values share the outer call's unit link.
Core libafw has no remaining call_create(NULL) (see
designs/compile-contextual-audit.md). Curl HTTP script callbacks still
use NULL at the libcurl C edge (process flags only).
//? sourceType: script
//?
//? test: map-basic
//? description: map with lambda still works when nested call has contextual
//? expect: 0
//? source: ...

const out = map(function (x) { return x + 1; }, [1, 2, 3]);
assert(out[0] === 2);
assert(out[1] === 3);
assert(out[2] === 4);
return 0;

//?
//? test: filter-basic
//? description: filter with lambda still works
//? expect: 0
//? source: ...

const out = filter(function (x) { return x > 1; }, [1, 2, 3]);
assert(length(out) === 2);
assert(out[0] === 2);
assert(out[1] === 3);
return 0;

//?
//? test: reduce-basic
//? description: reduce with lambda still works
//? expect: 0
//? source: ...

const out = reduce(function (acc, x) { return acc + x; }, 0, [1, 2, 3]);
assert(out === 6);
return 0;

//?
//? test: sort-basic
//? description: sort with compare functor still works (boolean compare)
//? expect: 0
//? source: ...

/* sort expects a (a, b) => boolean functor: true if a should come before b. */
const out = sort(function (a, b) { return a < b; }, [3, 1, 2]);
assert(out[0] === 1);
assert(out[1] === 2);
assert(out[2] === 3);
return 0;

//?
//? test: switch-predicate
//? description: switch with using predicate uses nested call with contextual
//? expect: 0
//? source: ...

const v = 2;
let r = 0;
switch (v) using eqx {
    case 1:
        r = 10;
        break;
    case 2:
        r = 20;
        break;
    default:
        r = 0;
        break;
}
assert(r === 20);
return 0;

//?
//? test: map-nested-error-still-surfaces
//? description: functor error still propagates (nested call evaluate)
//? expect: error:Assertion failed
//? source: ...

map(function (x) {
    assert(false);
    return x;
}, [1]);
return 0;

//?
//? test: map-from-compile-unit
//? description: map from compile<script> unit still evaluates (nested call contextual)
//? expect: 0
//? source: ...

const cv = compile<script>(script(
    "return map(function (x) { return x * 2; }, [2, 4]);"
));
const out = evaluate(cv);
assert(out[0] === 4);
assert(out[1] === 8);
/* Nested functor assert still surfaces through evaluate of compiled unit. */
try {
    evaluate(compile<script>(script(
        "map(function (x) { assert(false); }, [1]);\nreturn 0;"
    )));
    assert(false);
} catch (e) {
    assert(true);
}
return 0;

//?
//? test: stringify-replacer-function
//? description: stringify with function replacer creates nested call with contextual
//? expect: 0
//? source: ...

const o = { a: 1, b: 2 };
const s = stringify(o, function (k, v) {
    if (k === "b") return undefined;
    return v;
});
assert(s === "{\"a\":1}");
return 0;
