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
