#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: throw_rewind.as
//? customPurpose: Part of language/script tests
//? description: ...
Throw-path scope rewind and escaped closures across compiled-value wrap
(issue #35 / #2). Nested try/catch/finally, C throws (divide-by-zero), and
evaluate(compile) must leave the original error (not "Scope stack still has
active scopes") and must not destroy a scope a closure still holds. Heap
wrap finally is release, not force-destroy. Object/array literal methods
live in closures.as.
//? sourceType: script
//?
//? test: nested-block-throw-catch-original
//? description: Nested blocks throw; catch sees original message, not scope-stack leftover
//? expect: 0
//? source: ...

function f() {
    let a = 1;
    {
        let b = 2;
        throw "boom-nested";
    }
}

try {
    f();
    assert(false, "should have thrown");
} catch (e) {
    assert(e.message === "boom-nested", "original throw");
}
return 0;

//?
//? test: nested-block-throw-uncaught
//? description: Uncaught throw from nested blocks is the original error
//? expect: error:boom-uncaught
//? source: ...

function f() {
    let a = 1;
    {
        let b = 2;
        throw "boom-uncaught";
    }
}
f();
return 0;

//?
//? test: nested-call-throw-catch-original
//? description: Throw from inner function, catch at caller, original message
//? expect: 0
//? source: ...

function inner() {
    let a = 1;
    throw "from-inner";
}
function outer() {
    let b = 2;
    inner();
    return b;
}
try {
    outer();
    assert(false, "should have thrown");
} catch (e) {
    assert(e.message === "from-inner");
}
return 0;

//?
//? test: div0-from-nested-original
//? description: C throw (divide-by-zero) from nested lets is the original error
//? expect: 0
//? source: ...

function f() {
    let a = 1;
    {
        let b = 2;
        return 1 / 0;
    }
}
try {
    f();
    assert(false, "should have thrown");
} catch (e) {
    assert(e.message === "Integer divide by zero error");
}
return 0;

//?
//? test: throw-in-finally-original
//? description: Throw in finally replaces the try error; not a scope-stack leftover
//? expect: 0
//? source: ...

let probe;
try {
    try {
        throw "inner";
    } finally {
        let x = "ff";
        probe = function() {
            return x;
        };
        throw "from-finally";
    }
} catch (e) {
    assert(e.message === "from-finally");
    assert(probe() === "ff");
}
return 0;

//?
//? test: throw-from-catch-uncaught
//? description: Throw inside catch is that throw, not the original or a stack leftover
//? expect: error:rethrown-manual
//? source: ...

try {
    throw "orig";
} catch (e) {
    throw "rethrown-manual";
}
return 1;

//?
//? test: rethrow-bare-throw
//? description: Bare throw; in catch rethrows the original
//? expect: error:Inner throw
//? source: ...

try {
    throw "Inner throw";
} catch (e) {
    throw;
}
return 1;

//?
//? test: closure-from-try-after-throw
//? description: Closure assigned in try before throw still works after catch
//? expect: 0
//? source: ...

let probe;
try {
    let x = "try-x";
    probe = function() {
        return x;
    };
    throw "from-try";
} catch (e) {
}
assert(probe() === "try-x");
return 0;

//?
//? test: escaped-closure-after-throw-from-function
//? description: Function assigns a closure then throws; caller still calls it
//? expect: 0
//? source: ...

let probe;
function f() {
    let n = 8;
    probe = function() {
        return n;
    };
    throw "x";
}
try {
    f();
} catch (_) {
}
assert(probe() === 8);
return 0;

//?
//? test: return-closure-from-catch
//? description: Catch returns a closure over the function's let
//? expect: 0
//? source: ...

function f() {
    let n = 5;
    try {
        throw "x";
    } catch (_) {
        return function() {
            return n;
        };
    }
}
const g = f();
assert(g() === 5);
return 0;

//?
//? test: for-let-throw-mid-loop-then-call
//? description: for-let closure from the throwing iteration survives catch
//? expect: 0
//? source: ...

let f0;
try {
    for (let n = 0; n < 3; n = n + 1) {
        if (n === 1) {
            f0 = function() {
                return n;
            };
            throw "loop";
        }
    }
} catch (_) {
}
assert(f0() === 1);
return 0;

//?
//? test: nested-eval-returns-closure
//? description: evaluate(compile) result is a closure; call after inner heap release
//? expect: 0
//? source: ...

const g = evaluate(compile<script>(script(
    "let n = 4; return function() { return n; };")));
assert(g() === 4);
assert(g() === 4);
return 0;

//?
//? test: nested-eval-factory-returns-closure
//? description: Inner compiled_value factory returns a closure the caller can call
//? expect: 0
//? source: ...

const g = evaluate(compile<script>(script(
    "function make() { let n = 11; return function() { return n; }; }" +
    "return make();")));
assert(g() === 11);
return 0;

//?
//? test: nested-eval-throw-original
//? description: evaluate(compile) throw from nested lets is the original error
//? expect: 0
//? source: ...

try {
    evaluate(compile<script>(script(
        "let a = 1; { let b = 2; throw \"inner-eval\"; }")));
    assert(false, "should have thrown");
} catch (e) {
    assert(e.message === "inner-eval");
}
return 0;

//?
//? test: nested-eval-throw-from-function
//? description: Inner compiled_value function throw is the original error
//? expect: 0
//? source: ...

let saw = "";
try {
    evaluate(compile<script>(script(
        "function f() { let a = 1; { let b = 2; throw \"deep\"; } } f();")));
} catch (e) {
    saw = e.message;
}
assert(saw === "deep");
return 0;
