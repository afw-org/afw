#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: closures.as
//? customPurpose: Part of language/script tests
//? description: Closure checklist for issue #35 (escape/lifetime tied to #2)
//? sourceType: script
//?
//? test: nested-function-outer-let
//? description: Nested function sees enclosing let (issue #35; supported)
//? skip: false
//? expect: 0
//? source: ...

function f1() {
    let x = 1;
    function f2() {
        return x;
    }
    return f2();
}

assert(f1() === 1, "nested f2 should see outer let x");
return 0;

//?
//? test: returned-closures-independent
//? description: Each returned function keeps its own closed-over binding (issue #35; supported)
//? skip: false
//? expect: 0
//? source: ...

function make(n) {
    let v = n;
    return function() {
        return v;
    };
}

const a = make(10);
const b = make(20);
assert(a() === 10, "closure a");
assert(b() === 20, "closure b");
assert(a() === 10, "closure a still 10 after b()");
return 0;

//?
//? test: parameter-capture
//? description: Returned function closes over formal parameter (issue #35; supported)
//? skip: false
//? expect: 0
//? source: ...

function make(a) {
    return function() {
        return a;
    };
}

assert(make(42)() === 42);
assert(make(7)() === 7);
return 0;

//?
//? test: multi-parameter-capture
//? description: Closure closes over multiple formals (issue #35; supported)
//? skip: false
//? expect: 0
//? source: ...

function make(a, b) {
    return function() {
        return a + b;
    };
}

assert(make(3, 4)() === 7);
return 0;

//?
//? test: const-capture
//? description: Closure closes over const (issue #35; supported)
//? skip: false
//? expect: 0
//? source: ...

function make() {
    const c = "fixed";
    return function() {
        return c;
    };
}

assert(make()() === "fixed");
return 0;

//?
//? test: catch-block-let-close
//? description: Function in catch body closes over catch-block let (issue #35; supported)
//? skip: false
//? expect: 0
//? source: ...

let probe;
let x;

try {
    throw "null";
} catch (_) {
    let x = "inside";
    probe = function() {
        return x;
    };
}
x = "outside";

assert(x === "outside", "outer x after catch");
assert(probe() === "inside", "probe should keep catch-block let");
return 0;

//?
//? test: catch-param-lex-close
//? description: Function in catch closes over catch parameter (issue #35; supported short path)
//? skip: false
//? expect: 0
//? source: ...

/* Adaptive throw wraps the value as an error object (not a bare string). */
let probe;
let x;

try {
    throw "inside";
} catch (x) {
    probe = function() {
        return x;
    };
}
x = "outside";

assert(x === "outside", "outer x after catch");
const p = probe();
assert(meta(p).dataType === "object", "catch param is error object");
assert(p.message === "inside", "probe should keep catch parameter message");
return 0;

//?
//? test: finally-block-let-close
//? description: Function in finally closes over finally-block let (issue #35; supported)
//? skip: false
//? expect: 0
//? source: ...

let probe;
try {
    throw 1;
} catch (_) {
} finally {
    let x = "fin";
    probe = function() {
        return x;
    };
}
assert(probe() === "fin");
return 0;

//?
//? test: switch-case-lex-close
//? description: Closures from switch case keep case lexical environment (issue #35; supported)
//? skip: false
//? expect: 0
//? source: ...

let x = "outside";
let probe1;
let probe2;

switch (null) {
    case null:
        let x = "inside";
        probe1 = function() {
            return x;
        };
    case null:
        probe2 = function() {
            return x;
        };
}

assert(probe1() === "inside", "from first case clause");
assert(probe2() === "inside", "from second case clause");
assert(x === "outside", "outer x unchanged");
return 0;

//?
//? test: switch-default-let-close
//? description: Closure from switch default keeps default lexical binding (issue #35; supported)
//? skip: false
//? expect: 0
//? source: ...

let probe;
switch (1) {
    default:
        let x = "dflt";
        probe = function() {
            return x;
        };
}
assert(probe() === "dflt");
return 0;

//?
//? test: for-let-per-iteration
//? description: Each for-let iteration has its own binding for closures (issue #35; supported)
//? skip: false
//? expect: 0
//? source: ...

let f0;
let f1;
let f2;

for (let i = 0; i < 3; i = i + 1) {
    if (i === 0) {
        f0 = function() {
            return i;
        };
    } else if (i === 1) {
        f1 = function() {
            return i;
        };
    } else {
        f2 = function() {
            return i;
        };
    }
}

assert(f0() === 0, "iteration 0");
assert(f1() === 1, "iteration 1");
assert(f2() === 2, "iteration 2");
return 0;

//?
//? test: while-block-let-per-iteration
//? description: while body let is per-iteration for closures (issue #35; supported)
//? skip: false
//? expect: 0
//? source: ...

let i = 0;
let f0;
let f1;
while (i < 2) {
    let v = i;
    if (i === 0) {
        f0 = function() {
            return v;
        };
    } else {
        f1 = function() {
            return v;
        };
    }
    i = i + 1;
}
assert(f0() === 0);
assert(f1() === 1);
return 0;

//?
//? test: do-while-block-let-per-iteration
//? description: do-while body let is per-iteration for closures (issue #35; supported)
//? skip: false
//? expect: 0
//? source: ...

let i = 0;
let f0;
let f1;
do {
    let v = i;
    if (i === 0) {
        f0 = function() {
            return v;
        };
    } else {
        f1 = function() {
            return v;
        };
    }
    i = i + 1;
} while (i < 2);
assert(f0() === 0);
assert(f1() === 1);
return 0;

//?
//? test: block-shadow-closures
//? description: Closures capture the binding from their lexical block, not outer shadow (issue #35)
//? skip: false
//? expect: 0
//? source: ...

let x = "outer";
let probe_outer;
let probe_inner;
{
    probe_outer = function() {
        return x;
    };
    let x = "inner";
    probe_inner = function() {
        return x;
    };
}
assert(probe_outer() === "outer");
assert(probe_inner() === "inner");
assert(x === "outer");
return 0;

//?
//? test: if-block-let-close
//? description: Closure in if-block closes over block let (issue #35; supported)
//? skip: false
//? expect: 0
//? source: ...

let probe;
if (true) {
    let x = "in_if";
    probe = function() {
        return x;
    };
}
assert(probe() === "in_if");
return 0;

//?
//? test: nested-block-return-captures-both
//? description: Returned closure from inner block sees outer and inner lets (issue #35)
//? skip: false
//? expect: 0
//? source: ...

function make() {
    let a = 1;
    {
        let b = 2;
        return function() {
            return a + b;
        };
    }
}
assert(make()() === 3);
return 0;

//?
//? test: mutate-closed-let
//? description: Closure sees later mutations of the closed-over let (issue #35; supported)
//? skip: false
//? expect: 0
//? source: ...

let n = 1;
const get = function() {
    return n;
};
const set = function(v) {
    n = v;
    return n;
};

assert(get() === 1, "initial");
assert(set(5) === 5, "set");
assert(get() === 5, "see mutation");
return 0;

//?
//? test: two-closures-same-binding
//? description: Two functions sharing one let see the same mutable binding (issue #35)
//? skip: false
//? expect: 0
//? source: ...

let n = 0;
const inc = function() {
    n = n + 1;
    return n;
};
const get = function() {
    return n;
};
assert(inc() === 1);
assert(inc() === 2);
assert(get() === 2);
return 0;

//?
//? test: reassign-closed-from-outside
//? description: Outer reassignment visible to existing closure (issue #35; supported)
//? skip: false
//? expect: 0
//? source: ...

let n = 1;
const get = function() {
    return n;
};
n = 99;
assert(get() === 99);
return 0;

//?
//? test: nested-let-declared-before-inner
//? description: Inner function sees sibling let when declared before use (no ES hoisting)
//? skip: false
//? expect: 0
//? source: ...

function f1() {
    let x;
    function f2() {
        return x;
    }
    x = 1;
    return f2();
}

assert(f1() === 1, "f2 should see x assigned after f2 is defined");
return 0;

//?
//? test: close-over-nested-function
//? description: Closure can call a nested function declared in the same scope (issue #35)
//? skip: false
//? expect: 0
//? source: ...

function outer() {
    function inner() {
        return 11;
    }
    return function() {
        return inner();
    };
}
assert(outer()() === 11);
return 0;

//?
//? test: curry-two-levels
//? description: Returned function closes over parameter; second call applies more args (issue #35)
//? skip: false
//? expect: 0
//? source: ...

function outer(x) {
    return function(y) {
        return x + y;
    };
}
assert(outer(10)(5) === 15);
return 0;

//?
//? test: curry-three-levels
//? description: Deep nested returned functions close over all outer formals (issue #35)
//? skip: false
//? expect: 0
//? source: ...

function a(x) {
    return function(y) {
        return function(z) {
            return function() {
                return x + y + z;
            };
        };
    };
}
assert(a(1)(2)(3)() === 6);
return 0;

//?
//? test: recursive-closure-self-call
//? description: Closure can recurse via its own binding (issue #35; supported)
//? skip: false
//? expect: 0
//? source: ...

function make() {
    let n = 0;
    const f = function() {
        n = n + 1;
        if (n < 3) {
            return f();
        }
        return n;
    };
    return f;
}
assert(make()() === 3);
return 0;

//?
//? test: assign-property-then-call
//? description: o.fn = function… then o.fn() keeps closed-over let (issue #35; supported)
//? skip: false
//? expect: 0
//? source: ...

/* Property *assignment* of a function works; object-literal method form may not. */
function make() {
    let n = 3;
    let o = {};
    o.get = function() {
        return n;
    };
    return o;
}
assert(make().get() === 3);
return 0;

// ---------------------------------------------------------------------------
// Could work on before #2 — one-shot failures / language gaps (not lifetime)
// ---------------------------------------------------------------------------

//?
//? test: before2-object-literal-method-closure
//? description: Object-literal method closing over outer let (could fix before #2; #35)
//? skip: true
//? expect: 0
//? source: ...

/*
 * Currently: "Can not determine parent static scope for function" on bag.get().
 * Property assignment form works (see assign-property-then-call). ES authors
 * prefer { get: function()… }. Fix is call/binding path, not #2 memory.
 */
function factory() {
    let n = 7;
    return {
        get: function() {
            return n;
        },
        set: function(v) {
            n = v;
            return n;
        }
    };
}

const bag = factory();
assert(bag.get() === 7, "read after factory returned");
assert(bag.set(9) === 9, "write after factory returned");
assert(bag.get() === 9, "see mutation via escaped closure");
return 0;

//?
//? test: before2-list-literal-of-closures
//? description: List literal of functions closing over outer lets (could fix before #2; #35)
//? skip: true
//? expect: 0
//? source: ...

/*
 * Same parent-static-scope class of failure as object-literal methods when
 * calling fns[i](). Building via separate assigns may work; literal form is
 * the ES-comfortable shape.
 */
function make() {
    let a = 1;
    let b = 2;
    return [
        function() {
            return a;
        },
        function() {
            return b;
        }
    ];
}
const fns = make();
assert(fns[0]() === 1);
assert(fns[1]() === 2);
return 0;

//?
//? test: before2-for-of-let-per-iteration
//? description: for-of let should be per-iteration like for-let (could fix before #2; #35)
//? skip: true
//? expect: 0
//? source: ...

/*
 * Today both closures see the final iteration value (shared binding). ES and
 * Adaptive for(let i=…) use per-iteration bindings; for-of should match for
 * author comfort. Scope-per-iteration work, not #2 lifetime.
 */
let f0;
let f1;
const items = [10, 20];
let idx = 0;
for (let v of items) {
    if (idx === 0) {
        f0 = function() {
            return v;
        };
    } else {
        f1 = function() {
            return v;
        };
    }
    idx = idx + 1;
}
assert(f0() === 10, "first iteration value");
assert(f1() === 20, "second iteration value");
return 0;

//?
//? test: before2-pass-closure-as-argument
//? description: Pass returned closure into another function and call it (#89 call path)
//? expect: 0
//? source: ...

/*
 * Regression for function-as-value call args (issue #89). Lifetime/escape of
 * the closed-over binding under long-running use remains #2 (see after2-*).
 * Duplicate coverage lives in language/script/function.as.
 */
function apply(fn) {
    return fn();
}
function make() {
    let n = 5;
    return function() {
        return n;
    };
}
assert(apply(make()) === 5);
return 0;

// ---------------------------------------------------------------------------
// After / while #2 — escaped lifetime, long-running; unskip under valgrind
// ---------------------------------------------------------------------------

//?
//? test: after2-escape-via-list-from-loop
//? description: List of for-let closures remains valid after builder returns (#2)
//? skip: true
//? expect: 0
//? source: ...

/* Unskip when #2 managed escape is solid; prove under valgrind / multi-request. */
function build() {
    let out0;
    let out1;
    let out2;
    for (let i = 0; i < 3; i = i + 1) {
        if (i === 0) {
            out0 = function() {
                return i;
            };
        } else if (i === 1) {
            out1 = function() {
                return i;
            };
        } else {
            out2 = function() {
                return i;
            };
        }
    }
    return [out0, out1, out2];
}

const fns = build();
assert(fns[0]() === 0, "escaped f0");
assert(fns[1]() === 1, "escaped f1");
assert(fns[2]() === 2, "escaped f2");
return 0;

//?
//? test: after2-capture-mutable-object
//? description: Closure over mutable object keeps fields after factory returns (#2)
//? skip: true
//? expect: 0
//? source: ...

/* Pool/refcount mistakes here become UAF or residual state on long-running hosts. */
function make_counter() {
    let state = {
        n: 0
    };
    return function() {
        state.n = state.n + 1;
        return state.n;
    };
}

const tick = make_counter();
assert(tick() === 1);
assert(tick() === 2);
assert(tick() === 3);
return 0;

//?
//? test: after2-nested-factory-return
//? description: Nested returned factories share outer binding after escape (#2)
//? skip: true
//? expect: 0
//? source: ...

/* Multi-scope references and release order under retained callbacks. */
function outer(a) {
    return function(b) {
        return function() {
            return a + b;
        };
    };
}

const mid = outer(10);
const fn = mid(5);
assert(fn() === 15, "nested escaped closures");
assert(fn() === 15, "stable on second call");
return 0;

//?
//? test: after2-catch-param-escaped
//? description: Closure over catch parameter returned from function (#2)
//? skip: true
//? expect: 0
//? source: ...

/* Short-path catch param is green; stored handler for later needs escape. */
function capture_error() {
    try {
        throw "later";
    } catch (e) {
        return function() {
            return e;
        };
    }
}

const get_err = capture_error();
const e = get_err();
assert(meta(e).dataType === "object");
assert(e.message === "later");
return 0;

//?
//? test: after2-for-let-escaped-from-function
//? description: for-let closures returned in object keep per-iteration bindings (#2)
//? skip: true
//? expect: 0
//? source: ...

function make_getters() {
    let g0;
    let g1;
    let g2;
    for (let i = 0; i < 3; i = i + 1) {
        if (i === 0) {
            g0 = function() {
                return i;
            };
        } else if (i === 1) {
            g1 = function() {
                return i;
            };
        } else {
            g2 = function() {
                return i;
            };
        }
    }
    /* Use property assignment (literal methods may still be before2). */
    let o = {};
    o.g0 = g0;
    o.g1 = g1;
    o.g2 = g2;
    return o;
}

const g = make_getters();
assert(g.g0() === 0);
assert(g.g1() === 1);
assert(g.g2() === 2);
return 0;

//?
//? test: after2-escape-via-property-assign
//? description: Factory returns object with assigned closures; call after return (#2)
//? skip: true
//? expect: 0
//? source: ...

/*
 * Short path assign-property-then-call is green; this stresses retained bag
 * after factory scope ends (same shape, explicit escape checklist for #2).
 */
function factory() {
    let n = 7;
    let o = {};
    o.get = function() {
        return n;
    };
    o.set = function(v) {
        n = v;
        return n;
    };
    return o;
}

const bag = factory();
assert(bag.get() === 7);
assert(bag.set(9) === 9);
assert(bag.get() === 9);
return 0;

//?
//? test: after2-list-of-mutable-captures
//? description: Several closures each with own mutable object after builder returns (#2)
//? skip: true
//? expect: 0
//? source: ...

function build() {
    let c0;
    let c1;
    for (let i = 0; i < 2; i = i + 1) {
        let state = {
            n: i
        };
        const tick = function() {
            state.n = state.n + 10;
            return state.n;
        };
        if (i === 0) {
            c0 = tick;
        } else {
            c1 = tick;
        }
    }
    return [c0, c1];
}

const cs = build();
assert(cs[0]() === 10);
assert(cs[1]() === 11);
assert(cs[0]() === 20);
assert(cs[1]() === 21);
return 0;
