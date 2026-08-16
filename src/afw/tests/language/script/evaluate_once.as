#!/usr/bin/env -S afw --syntax test_script
//? testScript: evaluate_once.as
//? customPurpose: Part of language/script tests
//? description: User expressions in recent compiler syntax evaluate once (same class as #181)
//? sourceType: script
//?
//? test: object-construct-computed-name-once
//? description: Object value [expr] name is evaluated once
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let n = 0;
function key() {
    n = n + 1;
    return "a";
}

const o = { [key()]: 1 };
assert(o.a === 1);
assert(n === 1);

return 0;
//?
//? test: object-construct-spread-once
//? description: Object value ...expr is evaluated once
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let n = 0;
function make() {
    n = n + 1;
    return { a: n };
}

const o = { ...make(), b: 2 };
assert(o.a === 1);
assert(o.b === 2);
assert(n === 1);

return 0;
//?
//? test: array-constructor-spread-once
//? description: array(...expr) evaluates the spread expression once
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let n = 0;
function make() {
    n = n + 1;
    return [n];
}

const a = array(...make());
assert(length(a) === 1);
assert(a[0] === 1);
assert(n === 1);

return 0;
//?
//? test: array-literal-spread-once
//? description: [...expr] compiles to array() and evaluates expr once
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let n = 0;
function make() {
    n = n + 1;
    return [n, n + 10];
}

const a = [...make(), 99];
assert(length(a) === 3);
assert(a[0] === 1);
assert(a[1] === 11);
assert(a[2] === 99);
assert(n === 1);

return 0;
//?
//? test: parameter-default-once
//? description: Parameter default Expression evaluates once when used
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let n = 0;
function make() {
    n = n + 1;
    return n;
}

function f(x = make()) {
    return x;
}

assert(f() === 1);
assert(n === 1);
assert(f(9) === 9);
assert(n === 1);

return 0;
//?
//? test: list-pattern-default-once
//? description: List Pattern element default evaluates once at bind (not on each use)
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let n = 0;
function make() {
    n = n + 1;
    return n;
}

const [a = make()] = [];
assert(n === 1);
assert(a === 1);
assert(a === 1);
assert(n === 1);

const [b = make()] = [7];
assert(b === 7);
assert(n === 1);

return 0;
//?
//? test: object-pattern-default-once
//? description: Object Pattern property default evaluates once at bind (not on each use)
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let n = 0;
function make() {
    n = n + 1;
    return n;
}

const { a = make() } = {};
assert(n === 1);
assert(a === 1);
assert(a === 1);
assert(n === 1);

const { b = make() } = { b: 7 };
assert(b === 7);
assert(n === 1);

return 0;
//?
//? test: param-pattern-default-once
//? description: Pattern formal default evaluates once at bind (same path as let/const)
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let n = 0;
function make() {
    n = n + 1;
    return n;
}

function f({ x = make() }) {
    return x;
}

assert(f({}) === 1);
assert(n === 1);
assert(f({ x: 9 }) === 9);
assert(n === 1);

return 0;
//?
//? test: reference-by-key-get-once
//? description: obj[expr] evaluates the key expression once
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let n = 0;
function key() {
    n = n + 1;
    return "a";
}

const o = { a: 42 };
assert(o[key()] === 42);
assert(n === 1);

return 0;
//?
//? test: reference-by-key-assign-once
//? description: obj[expr] = value evaluates the key expression once
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let n = 0;
function key() {
    n = n + 1;
    return "a";
}

const o = {};
o[key()] = 42;
assert(o.a === 42);
assert(n === 1);

return 0;
//?
//? test: chained-assignment-rhs-once
//? description: x = y = expr evaluates the right-hand Expression once (#62)
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let n = 0;
function make() {
    n = n + 1;
    return n;
}

let x;
let y;
x = y = make();
assert(x === 1);
assert(y === 1);
assert(n === 1);

return 0;
//?
//? test: for-of-source-once
//? description: for (x of expr) evaluates the source expression once
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let n = 0;
function make() {
    n = n + 1;
    return [10, 20];
}

let sum = 0;
for (const x of make()) {
    sum = sum + x;
}
assert(sum === 30);
assert(n === 1);

return 0;
//?
//? test: throw-data-computed-name-once
//? description: throw data { [expr]: … } evaluates the name expression once (#33)
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let n = 0;
function key() {
    n = n + 1;
    return "k";
}

try {
    throw "boom" data { [key()]: 1 };
}
catch (e) {
    assert(e.data.k === 1);
    assert(n === 1);
}

return 0;
//?
