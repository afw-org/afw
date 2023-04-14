#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: conditional.as
//? customPurpose: Part of test262
//? description: Tests ? expression : expression
//? sourceType: script
//?
//? test: 11.12_A2.1_T1
//? description: checking boolean conditions
//? expect: null
//? source: ...
#!/usr/bin/env afw

assert((true ? false : true) === false, "true ? false : true !== false");
assert((false ? false : true) === true, "false ? false : true !== true");

loc x: boolean = true;
loc y: boolean = false;
if ((x ? y : true) !== y) {
    assert(false, "x ? y : true !== y");
}

loc z: boolean = true;
if ((false ? false : z) !== z) {
    assert(false, "false ? false : z !== z");
}

x = true;
y = false;
z = true;
if ((x ? y : z) !== y) {
    assert(false, "x ? y : z !== y");
}

x = false;
y = false;
z = true;
if ((x ? y : z) !== z) {
    assert(false, "x ? y : z !== z");
}


//? test: 11.12_A2.1_T2
//? description: undeclared reference in first argument throws error
//? expect: error
//? source: ...
#!/usr/bin/env afw

if (x ? true : false) {
    assert(false, "x ? true : false");
}


//? test: 11.12_A2.1_T3
//? description: undeclared reference in second argument throws error
//? expect: error
//? source: ...
#!/usr/bin/env afw

if (true ? y : false) {
    assert(false, "true ? y : false");
}


//? test: 11.12_A2.1_T4
//? description: undeclared reference in third argument throws error
//? expect: error
//? source: ...
#!/usr/bin/env afw

if (false ? true : z) {
    assert(false, "false ? true : z");
}


//? test: 11.12_A3_T1
//? description: If x is false, return z (booleans)
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((false ? false : true) !== true) {
    assert(false, "false ? false : true !== true");
}

loc z: boolean = true;
if ((false ? true : z) !== z) {
    assert(false, "false ? true : z !== z");
}


//? test: 11.12_A3_T4
//? description: If boolean(x) is false, return z
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((false ? true : undefined) !== undefined) {
    assert(false, "false ? true : undefined");
}

if ((false ? true : null) !== null) {
    assert(false, "false ? true : null");
}


//? test: 11.12_A4_T1
//? description: If x is true, return y (boolean)
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((true ? false : true) !== false) {
    assert(false, "true ? false : true !== false");
}

loc y: boolean = true;
if ((true ? y : false) !== y) {
    assert(false, "true ? y : false !== y");
}


//? test: 11.12_A4_T4
//? description: If x is true, return y (null/undefined)
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((true ? undefined : true) !== undefined) {
    assert(false, "true ? undefined : true !== undefined");
}

if ((true ? null : true) !== null) {
    assert(false, "true ? null : true !== null");
}


//? test: coalesce-expr-ternary
//? description: short-circut expression in the conditional expression (? :)
//? expect: null
//? source: ...
#!/usr/bin/env afw

loc x: integer;
x = undefined ?? true ? 0 : 42;
assert(x === 0, "undefined ?? true ? 0 : 42");

x = undefined;
x = null ?? true ? 0 : 42;
assert(x === 0, "null ?? true ? : 42");

x = undefined;
x = undefined ?? false ? 0 : 42;
assert(x === 42, "undefined ?? false ? 0 : 42");

x = undefined;
x = null ?? false ? 0 : 42;
assert(x === 42, "null ?? false ? 0 : 42");

x = undefined;
x = false ?? true ? 0 : 42;
assert(x === 42, "false ?? true ? 0 : 42");

x = undefined;
x = false ?? true ? 0 : 42;
assert(x === 42, "false ?? true ? 0 : 42");

x = undefined;
x = true ?? false ? 0 : 42;
assert(x === 0, "true ?? false ? 0 : 42");

x = undefined;
x = true ?? true ? 0 : 42;
assert(x === 0, "true ?? true ? 0 : 42");