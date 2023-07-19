#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: coalesce.as
//? customPurpose: Part of test262
//? description: expressions/coalesce
//? sourceType: script
//?
//? test: abrupt-is-a-short-circuit
//? description: Abrupt completions are also a Short circuit and prevent evaluation of the right-side expressions
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// It's not entirely clear why x is used in this test

var x;
function poison() {
    throw new Test262Error('poison handled');
}

function morePoison() {
    throw 'poison!!!!';
}

x = undefined;
assert.throws(Test262Error, function() {
    undefined ?? poison() ?? morePoison();
}, 'undefined ?? poison() ?? morePoison();');

x = undefined;
assert.throws(Test262Error, function() {
    null ?? poison() ?? morePoison();
}, 'null ?? poison() ?? morePoison();');

assert.throws(Test262Error, function() {
    poison() ?? morePoison();
}, 'poison() ?? morePoison();');


//? test: cannot-chain-head-with-logical-and
//? description: Cannot immediately contain, or be contained within, an && or || operation.
//? expect: error
//? source: ...
#!/usr/bin/env afw

0 && 0 ?? true;


//? test: cannot-chain-head-with-logical-or
//? description: Cannot immediately contain, or be contained within, an && or || operation.
//? expect: error
//? source: ...
#!/usr/bin/env afw

0 || 0 ?? true;


//? test: cannot-chain-tail-with-logical-and
//? description: If the CoalesceExpressionHead is undefined or null, follow return the right-side value. Otherwise, return the left-side value.
//? expect: error
//? skip: true
//? source: ...
#!/usr/bin/env afw

0 ?? 0 && true;


//? test: cannot-chain-tail-with-logical-or
//? description: If the CoalesceExpressionHead is undefined or null, follow return the right-side value. Otherwise, return the left-side value.
//? expect: error
//? skip: true
//? source: ...
#!/usr/bin/env afw

0 ?? 0 || true;

//? test: follows-null
//? description: If the CoalesceExpressionHead is null, follow return the right-side eval.
//? expect: null
//? source: ...
#!/usr/bin/env afw

let x;

x = null ?? 42;
assert(x === 42, 'null ?? 42');

x = null ?? undefined;
assert(x === undefined, 'null ?? undefined');

x = null ?? null;
assert(x === null, 'null ?? null');

x = null ?? false;
assert(x === false, 'null ?? false');


//? test: follows-undefined
//? description: If the CoalesceExpressionHead is undefined, follow return the right-side eval.
//? expect: null
//? source: ...
#!/usr/bin/env afw

let x;

x = undefined ?? 42;
assert(x === 42, 'undefined ?? 42');

x = undefined ?? undefined;
assert(x === undefined, 'undefined ?? undefined');

x = undefined ?? null;
assert(x === null, 'undefined ?? null');

x = undefined ?? false;
assert(x === false, 'undefined ?? false');


//? test: short-circuit-number-0
//? description: Short circuit if the CoalesceExpressionHead is not undefined or null (0)
//? expect: null
//? source: ...
#!/usr/bin/env afw

let x;

x = undefined;
x = 0 ?? 1;
assert(x === 0, '0 ?? 1');

x = undefined;
x = 0 ?? null;
assert(x === 0, '0 ?? null');

x = undefined;
x = 0 ?? null ?? undefined;
assert(x === 0, '0 ?? null ?? undefined');

x = undefined;
x = 0 ?? undefined ?? null;
assert(x === 0, '0 ?? undefined ?? null');

x = undefined;
x = 0 ?? null ?? null;
assert(x === 0, '0 ?? null ?? null');

x = undefined;
x = 0 ?? undefined ?? undefined;
assert(x === 0, '0 ?? null ?? null');

x = undefined;
x = null ?? 0 ?? null;
assert(x === 0, 'null ?? 0 ?? null');

x = undefined;
x = null ?? 0 ?? undefined;
assert(x === 0, 'null ?? 0 ?? undefined');

x = undefined;
x = undefined ?? 0 ?? null;
assert(x === 0, 'undefined ?? 0 ?? null');

x = undefined;
x = undefined ?? 0 ?? undefined;
assert(x === 0, 'undefined ?? 0 ?? undefined');


//? test: short-circuit-number-42
//? description: Short circuit if the CoalesceExpressionHead is not undefined or null (42)
//? expect: null
//? source: ...
#!/usr/bin/env afw

let x;

x = undefined;
x = 42 ?? 1;
assert(x === 42, '42 ?? 1');

x = undefined;
x = 42 ?? null;
assert(x === 42, '42 ?? null');

x = undefined;
x = 42 ?? undefined;
assert(x === 42, '42 ?? undefined');

x = undefined;
x = 42 ?? null ?? undefined;
assert(x === 42, '42 ?? null ?? undefined');

x = undefined;
x = 42 ?? undefined ?? null;
assert(x === 42, '42 ?? undefined ?? null');

x = undefined;
x = 42 ?? null ?? null;
assert(x === 42, '42 ?? null ?? null');

x = undefined;
x = 42 ?? undefined ?? undefined;
assert(x === 42, '42 ?? null ?? null');

x = undefined;
x = null ?? 42 ?? null;
assert(x === 42, 'null ?? 42 ?? null');

x = undefined;
x = null ?? 42 ?? undefined;
assert(x === 42, 'null ?? 42 ?? undefined');

x = undefined;
x = undefined ?? 42 ?? null;
assert(x === 42, 'undefined ?? 42 ?? null');

x = undefined;
x = undefined ?? 42 ?? undefined;
assert(x === 42, 'undefined ?? 42 ?? undefined');


//? test: short-circuit-empty-string
//? description: Short circuit if the CoalesceExpressionHead is not undefined or null (the empty string)
//? expect: null
//? source: ...
#!/usr/bin/env afw


let x;
let str = '';

x = undefined;
x = str ?? 1;
assert(x === str, 'str ?? 1');

x = undefined;
x = str ?? null;
assert(x === str, 'str ?? null');

x = undefined;
x = str ?? undefined;
assert(x === str, 'str ?? undefined');

x = undefined;
x = str ?? null ?? undefined;
assert(x === str, 'str ?? null ?? undefined');

x = undefined;
x = str ?? undefined ?? null;
assert(x === str, 'str ?? undefined ?? null');

x = undefined;
x = str ?? null ?? null;
assert(x === str, 'str ?? null ?? null');

x = undefined;
x = str ?? undefined ?? undefined;
assert(x === str, 'str ?? null ?? null');

x = undefined;
x = null ?? str ?? null;
assert(x === str, 'null ?? str ?? null');

x = undefined;
x = null ?? str ?? undefined;
assert(x === str, 'null ?? str ?? undefined');

x = undefined;
x = undefined ?? str ?? null;
assert(x === str, 'undefined ?? str ?? null');

x = undefined;
x = undefined ?? str ?? undefined;
assert(x === str, 'undefined ?? str ?? undefined');


//? test: short-circuit-number-false
//? description: Short circuit if the CoalesceExpressionHead is not undefined or null (false)
//? expect: null
//? source: ...
#!/usr/bin/env afw

let x;

x = undefined;
x = false ?? 1;
assert(x === false, 'false ?? 1');

x = undefined;
x = false ?? null;
assert(x === false, 'false ?? null');

x = undefined;
x = false ?? undefined;
assert(x === false, 'false ?? undefined');

x = undefined;
x = false ?? null ?? undefined;
assert(x === false, 'false ?? null ?? undefined');

x = undefined;
x = false ?? undefined ?? null;
assert(x === false, 'false ?? undefined ?? null');

x = undefined;
x = false ?? null ?? null;
assert(x === false, 'false ?? null ?? null');

x = undefined;
x = false ?? undefined ?? undefined;
assert(x === false, 'false ?? null ?? null');

x = undefined;
x = null ?? false ?? null;
assert(x === false, 'null ?? false ?? null');

x = undefined;
x = null ?? false ?? undefined;
assert(x === false, 'null ?? false ?? undefined');

x = undefined;
x = undefined ?? false ?? null;
assert(x === false, 'undefined ?? false ?? null');

x = undefined;
x = undefined ?? false ?? undefined;
assert(x === false, 'undefined ?? false ?? undefined');


//? test: short-circuit-number-true
//? description: Short circuit if the CoalesceExpressionHead is not undefined or null (true)
//? expect: null
//? source: ...
#!/usr/bin/env afw

let x;

x = undefined;
x = true ?? 1;
assert(x === true, 'true ?? null');

x = undefined;
x = true ?? null;
assert(x === true, 'true ?? null');

x = undefined;
x = true ?? undefined;
assert(x === true, 'true ?? undefined');

x = undefined;
x = true ?? null ?? undefined;
assert(x === true, 'true ?? null ?? undefined');

x = undefined;
x = true ?? undefined ?? null;
assert(x === true, 'true ?? undefined ?? null');

x = undefined;
x = true ?? null ?? null;
assert(x === true, 'true ?? null ?? null');

x = undefined;
x = true ?? undefined ?? undefined;
assert(x === true, 'true ?? null ?? null');

x = undefined;
x = null ?? true ?? null;
assert(x === true, 'null ?? true ?? null');

x = undefined;
x = null ?? true ?? undefined;
assert(x === true, 'null ?? true ?? undefined');

x = undefined;
x = undefined ?? true ?? null;
assert(x === true, 'undefined ?? true ?? null');

x = undefined;
x = undefined ?? true ?? undefined;
assert(x === true, 'undefined ?? true ?? undefined');


//? test: short-circuit-prevents-evaluation
//? description: Short circuit can prevent evaluation of the right-side expressions
//? expect: null
//? source: ...
#!/usr/bin/env afw

let x;
function poison() {
    throw "should not evaluate poison";
}

x = undefined;
x = 42 ?? undefined ?? poison();
assert(x === 42);

x = undefined;
x = undefined ?? 42 ?? poison();
assert(x === 42);

x = undefined;
x = 42 ?? poison();
assert(x === 42);


//? test: tco-pos-null
//? description: Expression is a candidate for tail-call optimization.
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// We do not currently support tail-call optimization

let callCount = 0;
function f(n) {
    if (n === 0) {
        callCount += 1;
        return;
    }
    return null ?? f(n - 1);
}
f(100000);
assert(callCount === 1);


//? test: tco-pos-undefined
//? description: Expression is a candidate for tail-call optimization.
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// We do not currently support tail-call optimization

let callCount = 0;
function f(n) {
    if (n === 0) {
        callCount += 1;
        return;
    }
    return undefined ?? f(n - 1);
}
f(100000);
assert(callCount === 1);