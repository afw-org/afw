#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: pool_eval_lifetime.as
//? customPurpose: Part of language/script tests
//? description: ...
Compiled-value heap wrap and slot-protocol lifetime (issue #2 pool split).
Inner evaluate(compile) clones onto the caller; eval-created objects/arrays
and scalars must still be usable after the inner heap is released. Throw-path
scope rewind and nested-eval closures: language/script/throw_rewind.as (#35).
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
//? description: object returned from inner evaluate is cloned to the caller
//? expect: 0
//? source: ...

const o = evaluate(compile<script>(script(
    "let x = { a: 1, b: \"z\" }; x.a = 2; return x;")));
assert(o.a === 2, "cloned object a");
assert(o.b === "z", "cloned object b");
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
//? description: one compiled_value, two evaluate wraps (two heaps)
//? expect: 0
//? source: ...

const cv = compile<script>(script("let n = 4; return n + 1;"));
assert(evaluate(cv) === 5);
assert(evaluate(cv) === 5);
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
//? description: inner script itself calls evaluate (nested compiled-value wrap)
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
