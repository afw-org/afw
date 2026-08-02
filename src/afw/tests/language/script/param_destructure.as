#!/usr/bin/env -S afw --syntax test_script
//? testScript: param_destructure.as
//? customPurpose: Part of language/script tests
//? description: Function parameter and catch Pattern destructure (issue #140)
//? sourceType: script
//?
//? test: param-object-pattern
//? description: Object Pattern parameter (options-object style)
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function connect({ host, port = 443 }) {
    return host + ":" + string(port);
}

assert(connect({ host: "x" }) === "x:443");
assert(connect({ host: "x", port: 80 }) === "x:80");

function sumPair({ a, b }) {
    return a + b;
}
assert(sumPair({ a: 1, b: 2 }) === 3);

return 0;
//?
//? test: param-array-pattern
//? description: Array Pattern parameter with hole and rest
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function headTail([first, , third, ...rest]) {
    assert(first === 1);
    assert(third === 3);
    assert(length(rest) === 2);
    assert(rest[0] === 4);
    assert(rest[1] === 5);
    return first;
}

assert(headTail([1, 2, 3, 4, 5]) === 1);

function emptyRest([a, ...rest]) {
    assert(a === 9);
    assert(length(rest) === 0);
    return a;
}
assert(emptyRest([9]) === 9);

return 0;
//?
//? test: param-pattern-default-whole
//? description: Whole-parameter default before Pattern bind
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function withDefault({ host, port = 443 } = { host: "localhost" }) {
    return host + ":" + string(port);
}

assert(withDefault() === "localhost:443");
assert(withDefault({ host: "h", port: 9 }) === "h:9");

return 0;
//?
//? test: param-pattern-rename-nested
//? description: Nested object Pattern and property rename
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function nested({ outer: { inner }, name: n }) {
    assert(inner === 7);
    assert(n === "ok");
    return inner;
}

assert(nested({ outer: { inner: 7 }, name: "ok" }) === 7);

return 0;
//?
//? test: param-pattern-missing-undefined
//? description: Missing property without default binds undefined
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function miss({ a, b }) {
    assert(a === 1);
    assert(b === undefined);
    return 0;
}

assert(miss({ a: 1 }) === 0);

return 0;
//?
//? test: param-mixed-simple-and-pattern
//? description: Mix simple name and Pattern parameters
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function mixed(label, { x, y }) {
    assert(label === "L");
    assert(x + y === 5);
    return label;
}

assert(mixed("L", { x: 2, y: 3 }) === "L");

return 0;
//?
//? test: lambda-param-pattern
//? description: Lambda / expression function with Pattern param
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const f = function ({ a, b = 1 }) {
    return a + b;
};
assert(f({ a: 2 }) === 3);
assert(f({ a: 2, b: 10 }) === 12);

return 0;
//?
//? test: catch-object-pattern
//? description: catch ({ message }) Pattern binding
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let saw = "";
try {
    throw "boom";
}
catch ({ message }) {
    saw = message;
}
assert(saw === "boom");

return 0;
//?
//? test: simple-params-still-work
//? description: Existing simple/optional/rest params unchanged
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function plain(a, b = 2, c?) {
    assert(a === 1);
    assert(b === 2);
    assert(c === undefined);
    return a + b;
}
assert(plain(1) === 3);

function withRest(a, ...rest) {
    assert(a === 1);
    assert(length(rest) === 2);
    return length(rest);
}
assert(withRest(1, 2, 3) === 2);

return 0;
//?
//? test: ts-prior-param-default
//? description: Default Expression may reference an earlier parameter (TS-like)
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function f(x, y = x) {
    return y;
}
assert(f(3) === 3);
assert(f(3, 9) === 9);

function g({ a }, b = a) {
    return b;
}
assert(g({ a: 7 }) === 7);

function exprDefault(x = 1 + 2 * 3) {
    return x;
}
assert(exprDefault() === 7);

return 0;
//?
//? test: ts-required-pattern-missing-arg
//? description: Required Pattern formal with no argument throws
//? expect: error:Parameter 1 is required
//? source: ...
#!/usr/bin/env afw

function req({ a }) {
    return a;
}
req();
//?
//? test: ts-object-rest-on-param
//? description: Object rest in a parameter Pattern
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function pack({ a, ...r }) {
    assert(a === 1);
    assert(r.b === 2);
    assert(r.c === 3);
    return length(keys(r));
}
assert(pack({ a: 1, b: 2, c: 3 }) === 2);

return 0;
//?
//? test: ts-catch-rename-and-data
//? description: catch Pattern rename and data property
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let m = "";
let code = 0;
try {
    throw "fail" { "code": 42 };
}
catch ({ message: msg, data }) {
    m = msg;
    code = data.code;
}
assert(m === "fail");
assert(code === 42);

return 0;
//?
//? test: ts-recursive-pattern-formal
//? description: Recursive call with object Pattern formal (bind order)
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function sumDown({ n, acc = 0 }) {
    if (n === 0) {
        return acc;
    }
    return sumDown({ n: n - 1, acc: acc + n });
}
assert(sumDown({ n: 4 }) === 10);

return 0;
//?
//? test: ts-pattern-then-rest-param
//? description: Pattern formal followed by rest parameter
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function mix({ a }, ...r) {
    assert(a === 1);
    assert(length(r) === 2);
    assert(r[0] === "x");
    assert(r[1] === "y");
    return length(r);
}
assert(mix({ a: 1 }, "x", "y") === 2);

return 0;
//?
//? test: ts-wrong-type-array-pattern
//? description: Array Pattern on a non-array argument errors
//? expect: error:Array destructure can only be performed on an array
//? source: ...
#!/usr/bin/env afw

function takePair([a, b]) {
    return a + b;
}
takePair({ a: 1, b: 2 });
//?
//? test: ts-optional-pattern-no-default
//? description: Optional Pattern formal with no arg leaves binding undefined
//? expect: 0
//? source: ...
#!/usr/bin/env afw

/* Adaptive: omit whole arg without = {} → Pattern not applied; leaves undefined.
 * Prefer = {} for TS options-object style. */
function opt({ a }?) {
    return a;
}
assert(opt() === undefined);
assert(opt({ a: 5 }) === 5);

return 0;
//?
//? test: ts-call-site-spread
//? description: Call-site ...array expands into separate arguments
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function sum3(a, b, c) {
    return a + b + c;
}
assert(sum3(...[1, 2, 3]) === 6);
assert(sum3(1, ...[2, 3]) === 6);
assert(sum3(...[1, 2], 3) === 6);

function join(...parts) {
    return string(parts);
}
assert(join(...["a", "b"], "c") === '["a","b","c"]');

return 0;
//?
//? test: ts-computed-key-destructure
//? description: Computed and string keys in object Patterns
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const key = "inner";
function take({ [key]: v, "other": o }) {
    assert(v === 11);
    assert(o === 22);
    return v + o;
}
assert(take({ inner: 11, other: 22 }) === 33);

const { [key]: x } = { inner: 5 };
assert(x === 5);

return 0;
//?
//? test: ts-typed-pattern-leaves
//? description: Type annotations on Pattern leaves and whole Pattern formal
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function typedLeaves([a: integer, b: integer]: integer[]) {
    return a + b;
}
assert(typedLeaves([3, 4]) === 7);

function typedObj({ n: n: integer }) {
    return n;
}
assert(typedObj({ n: 9 }) === 9);

return 0;
//?
//? test: ts-catch-pattern-decompile-fidelity
//? description: catch Pattern decompile recompiles and evaluates
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const src = "try { throw \"z\"; } catch ({message}) { return message; }";
const d1 = decompile(compile<script>(script(src)));
const d2 = decompile(compile<script>(script(d1)));
assert(d1 == d2);
assert(evaluate(compile<script>(script(src))) == "z");
assert(evaluate(compile<script>(script(d1))) == "z");

return 0;
