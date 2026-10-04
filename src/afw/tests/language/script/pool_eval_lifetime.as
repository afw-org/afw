#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: pool_eval_lifetime.as
//? customPurpose: Part of language/script tests
//? description: ...
Compiled-value heap wrap and slot-protocol lifetime (issue #2 pool split).
Inner evaluate(compile) is caller does not release. A managed result lives
in dest p->managed_p of the isolate write (scope->p at deactivate);
this evaluate registers last-release of that one hold on dest p. Nested
pools are shorter. Eval-created objects/arrays and scalars must still be
usable after the inner heap is released. Throw-path scope rewind and
nested-eval closures: language/script/throw_rewind.as (#35).
//? sourceType: script
//?
//? test: nested-eval-scalar
//? description: evaluate(compile) scalar result survives inner heap release
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script("return 1 + 2;")));
assert(r === 3);
return 0;

//?
//? test: nested-eval-object
//? description: object returned from inner evaluate is usable after the inner heap is released
//? expect: 0
//? source: ...

const o = evaluate(compile<script>(script(
    "let x = { a: 1, b: \"z\" }; x.a = 2; return x;")));
assert(o.a === 2, "returned object a");
assert(o.b === "z", "returned object b");
o.a = 9;
assert(o.a === 9, "caller can still set after inner heap gone");
return 0;

//?
//? test: nested-eval-array
//? description: array returned from inner evaluate survives inner heap
//? expect: 0
//? source: ...

const a = evaluate(compile<script>(script(
    "let x = [1, 2]; x[0] = 3; return x;")));
assert(a[0] === 3);
assert(a[1] === 2);
return 0;

//?
//? test: compile-once-eval-twice
//? description: one compiled_value, two evaluate (each caller does not release)
//? expect: 0
//? source: ...

const cv = compile<script>(script("let n = 4; return n + 1;"));
assert(evaluate(cv) === 5);
assert(evaluate(cv) === 5);
return 0;

//?
//? test: compile-stores-unevaluated
//? description: compile() result is a unit until evaluate()
//? expect: 0
//? source: ...

const cv = compile<script>(script("return 7;"));
assert(meta(cv).dataType === "unevaluated");
assert(evaluate(cv) === 7);
return 0;

//?
//? test: many-inner-evals
//? description: loop of evaluate(compile) object results
//? expect: 0
//? source: ...

let i = 0;
for (i = 0; i < 40; i = i + 1) {
    const o = evaluate(compile<script>(script(
        "let x = { n: 1 }; x.n = 2; return x;")));
    assert(o.n === 2);
}
return 0;

//?
//? test: nested-eval-inside-eval
//? description: inner script itself calls evaluate (nested compiled-value; isolate dest stays outermost)
//? expect: 0
//? source: ...

function g() {
    return evaluate(compile<script>(script("return 6 + 1;")));
}
assert(g() === 7);
return 0;

//?
//? test: function-return-object
//? description: callee returns {} after its scope tracker is gone (#2)
//? expect: 0
//? source: ...

function f() {
    let o = { k: 1 };
    o.k = 2;
    return o;
}

const x = f();
assert(x.k === 2);
x.k = 3;
assert(x.k === 3);
return 0;

//?
//? test: overwrite-object-in-loop
//? description: slot overwrite of {} in one eval (wrapper + heap)
//? expect: 0
//? source: ...

let o = { n: -1 };
let i = 0;
for (i = 0; i < 30; i = i + 1) {
    o = { n: i };
}
assert(o.n === 29);
return 0;

//?
//? test: overwrite-array-in-loop
//? description: slot overwrite of [] in one eval
//? expect: 0
//? source: ...

let a = [0];
let i = 0;
for (i = 0; i < 30; i = i + 1) {
    a = [i];
}
assert(a[0] === 29);
return 0;

//?
//? test: nested-blocks-then-return
//? description: inner block scopes die; returned object still holds
//? expect: 0
//? source: ...

function f() {
    let o = { x: 0 };
    {
        let inner = { y: 5 };
        o.x = inner.y;
    }
    return o;
}

assert(f().x === 5);
return 0;

//?
//? test: let-from-call-object
//? description: let x = f() holds returned object across callee last-release
//? expect: 0
//? source: ...

function f() {
    return { a: 10 + 1 };
}

let x = f();
assert(x.a === 11);
x = f();
assert(x.a === 11);
return 0;

//?
//? test: eval-returns-closure
//? description: eval<script> closure keeps its compile unit after the call (#342)
//? expect: 0
//? source: ...

const g = eval<script>(script(
    "let n = 4; return function() { return n; };"));
assert(g() === 4);
assert(g() === 4);
return 0;

//?
//? test: eval-factory-returns-closure
//? description: eval<script> factory returns an inner closure over a local (#342)
//? expect: 0
//? source: ...

const g = eval<script>(script(
    "function make() { let n = 11; return function() { return n; }; }" +
    "return make();"));
assert(g() === 11);
return 0;

//?
//? test: eval-closure-overwrite-in-loop
//? description: overwriting an eval<script> closure drops the previous unit (#342)
//? expect: 0
//? source: ...

let g = function() { return -1; };
let i = 0;
for (i = 0; i < 40; i = i + 1) {
    g = eval<script>(script(
        "let n = 4; return function() { return n; };"));
    assert(g() === 4);
}
assert(g() === 4);
return 0;

//?
//? test: eval-string-returns-closure
//? description: eval<string> closure keeps its compile unit after the call (#342)
//? expect: 0
//? source: ...

const g = eval<string>("let n = 4; return function() { return n; };");
assert(g() === 4);
assert(g() === 4);
return 0;

//?
//? test: eval-string-factory-returns-closure
//? description: eval<string> factory returns an inner closure over a local (#342)
//? expect: 0
//? source: ...

const g = eval<string>(
    "function make() { let n = 11; return function() { return n; }; }" +
    "return make();");
assert(g() === 11);
return 0;

//?
//? test: eval-string-named-function-return
//? description: eval<string> named function then return a (slot held inner)
//? expect: 0
//? source: ...

const g = eval<string>("function a() { return 7; } return a;");
assert(g() === 7);
assert(g() === 7);
return 0;

//?
//? test: eval-string-named-function-overwrite-in-loop
//? description: overwriting eval<string> named-then-return drops the previous unit
//? expect: 0
//? source: ...

let g = function() { return -1; };
let i = 0;
for (i = 0; i < 40; i = i + 1) {
    g = eval<string>("function a() { return 7; } return a;");
    assert(g() === 7);
}
assert(g() === 7);
return 0;

//?
//? test: eval-string-closure-overwrite-in-loop
//? description: overwriting an eval<string> closure drops the previous unit (#342)
//? expect: 0
//? source: ...

let g = function() { return -1; };
let i = 0;
for (i = 0; i < 40; i = i + 1) {
    g = eval<string>("let n = 4; return function() { return n; };");
    assert(g() === 4);
}
assert(g() === 4);
return 0;

//?
//? test: eval-script-object-of-functions
//? description: eval<script> object of functions keeps the unit (#342)
//? expect: 0
//? source: ...

const o = eval<script>(script(
    "function func1() { return true; }" +
    "function func2() { return false; }" +
    "return { func1: func1, func2: func2, nest: { func1: func1 } };"));
assert(o.func1());
assert(!o.func2());
assert(o.nest.func1());
return 0;

//?
//? test: eval-script-array-of-functions
//? description: eval<script> array of functions keeps the unit (#342)
//? expect: 0
//? source: ...

const a = eval<script>(script(
    "function f() { return 3; } return [f];"));
assert(a[0]() === 3);
return 0;

//?
//? test: eval-string-object-of-functions
//? description: eval<string> object of functions keeps the unit (#342)
//? expect: 0
//? source: ...

const o = eval<string>(
    "function func1() { return true; }" +
    "function func2() { return false; }" +
    "return { func1: func1, func2: func2 };");
assert(o.func1());
assert(!o.func2());
return 0;

//?
//? test: eval-script-object-of-functions-overwrite-in-loop
//? description: overwriting an eval<script> object of functions drops the previous unit (#342)
//? expect: 0
//? source: ...

let o = { func1: function() { return false; } };
let i = 0;
for (i = 0; i < 40; i = i + 1) {
    o = eval<script>(script(
        "function func1() { return true; }" +
        "return { func1: func1 };"));
    assert(o.func1());
}
assert(o.func1());
return 0;

//?
//? test: eval-script-array-of-functions-overwrite-in-loop
//? description: overwriting an eval<script> array of functions drops the previous unit (#342)
//? expect: 0
//? source: ...

let a = [function() { return -1; }];
let i = 0;
for (i = 0; i < 40; i = i + 1) {
    a = eval<script>(script(
        "function f() { return 3; } return [f];"));
    assert(a[0]() === 3);
}
assert(a[0]() === 3);
return 0;

//?
//? test: eval-string-object-of-functions-overwrite-in-loop
//? description: overwriting an eval<string> object of functions drops the previous unit (#342)
//? expect: 0
//? source: ...

let o = { func1: function() { return false; } };
let i = 0;
for (i = 0; i < 40; i = i + 1) {
    o = eval<string>(
        "function func1() { return true; } return { func1: func1 };");
    assert(o.func1());
}
assert(o.func1());
return 0;

//?
//? test: eval-closure-as-map-functor
//? description: map functor from eval<script> / eval<string> capturing closures (#342)
//? expect: 0
//? source: ...

/*
 * eval<script> transfers a top-level closure (unit get_reference on
 * the binding), then a high-level array function evaluates that
 * factory-or-value once and calls it per entry. Companion to
 * higher_order_array returned-closure-as-functor.
 */
const f = eval<script>(script(
    "let n = 5; return function (v) { return v + n; };"));
let out1 = map(f, [1, 2, 3]);
assert(length(out1) === 3);
assert(out1[0] === 6 && out1[1] === 7 && out1[2] === 8);

let out2 = map(eval<string>(
    "let n = 5; return function (v) { return v + n; };"), [1, 2, 3]);
assert(length(out2) === 3);
assert(out2[0] === 6 && out2[1] === 7 && out2[2] === 8);

let i = 0;
for (i = 0; i < 40; i = i + 1) {
    let out = map(eval<string>(
        "let n = 5; return function (v) { return v + n; };"), [1, 2, 3]);
    assert(out[0] === 6 && out[2] === 8);
}
return 0;

//?
//? test: evaluate-compile-closure-as-map-functor
//? description: map functor from evaluate(compile()) capturing closure
//? expect: 0
//? source: ...

/*
 * Adaptive compile() does not last-release the unit (evaluate(compile())
 * and closures still need that heap). evaluate() of that unit returns
 * the capturing closure; map evaluates the functor once and calls it.
 */
const f = evaluate(compile<script>(script(
    "let n = 5; return function (v) { return v + n; };")));
let out = map(f, [1, 2, 3]);
assert(length(out) === 3);
assert(out[0] === 6 && out[1] === 7 && out[2] === 8);

let i = 0;
for (i = 0; i < 40; i = i + 1) {
    let g = evaluate(compile<script>(script(
        "let n = 5; return function (v) { return v + n; };")));
    let mapped = map(g, [1, 2, 3]);
    assert(mapped[0] === 6 && mapped[2] === 8);
}
return 0;
