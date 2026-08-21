#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: scalar_escape.as
//? customPurpose: Part of language/script tests
//? description: ...
Returned scalars must survive the callee scope pool (#2 slot protocol
plus unmanaged clone_or_reference boxing into xctx->p).
//? sourceType: script
//?
//? test: return-integer-temp
//? description: let x = 1 + 2; return x survives callee last-release
//? expect: 0
//? source: ...

function f() {
    let x = 1 + 2;
    return x;
}

assert(f() === 3);
return 0;

//?
//? test: return-integer-expression
//? description: return 1 + 2 boxes before callee walk
//? expect: 0
//? source: ...

function f() {
    return 1 + 2;
}

assert(f() === 3);
return 0;

//?
//? test: return-concat-string
//? description: returned concatenated string copies octets into xctx->p
//? expect: 0
//? source: ...

function f() {
    let s = "a" + "b";
    return s;
}

assert(f() === "ab");
return 0;

//?
//? test: overwrite-then-return
//? description: slot overwrite of a boxed integer then return
//? expect: 0
//? source: ...

function f() {
    let x = 1;
    x = 2 + 3;
    return x;
}

assert(f() === 5);
return 0;

//?
//? test: let-from-call
//? description: let y = f() holds the boxed integer
//? expect: 0
//? source: ...

function f() {
    return 10 + 1;
}

let y = f();
assert(y === 11);
y = f();
assert(y === 11);
return 0;
