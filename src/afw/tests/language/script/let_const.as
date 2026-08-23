#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: let_const.as
//? customPurpose: Part of language/script tests
//? description: ...
Multiple names on one let / const statement (issue #62). Same block as
separate lets; no extra scope. const still requires an initializer on
every binding. Trailing comma is not allowed.
//? sourceType: script
//?
//? test: let-two-initialized
//? description: let a = 1, b = 2 binds both
//? expect: 0
//? source: ...

let a = 1, b = 2;
assert(a === 1);
assert(b === 2);
return 0;

//?
//? test: let-mixed-init
//? description: let a, b = 2 leaves a undefined
//? expect: 0
//? source: ...

let a, b = 2;
assert(a === undefined);
assert(b === 2);
a = 1;
assert(a === 1);
return 0;

//?
//? test: let-three
//? description: three let bindings
//? expect: 0
//? source: ...

let a = 1, b = 2, c = a + b;
assert(c === 3);
return 0;

//?
//? test: const-two
//? description: const a = 1, b = 2 binds both
//? expect: 0
//? source: ...

const a = 1, b = 2;
assert(a === 1);
assert(b === 2);
return 0;

//?
//? test: const-reassign-still-errors
//? description: comma const is still const
//? expect: error
//? source: ...

const a = 1, b = 2;
b = 3;

//?
//? test: const-missing-initializer
//? description: each const binding needs =
//? expect: error
//? source: ...

const a = 1, b;

//?
//? test: let-trailing-comma
//? description: trailing comma is a syntax error
//? expect: error
//? source: ...

let a = 1,;

//?
//? test: let-types
//? description: typed multi let
//? expect: 0
//? source: ...

let a: integer = 1, b: string = "x";
assert(a === 1);
assert(b === "x");
return 0;

//?
//? test: const-types
//? description: typed multi const
//? expect: 0
//? source: ...

const a: integer = 1, b: integer = 2;
assert(a + b === 3);
return 0;

//?
//? test: let-destructure-pair
//? description: object and array destructure in one let
//? expect: 0
//? source: ...

let { host } = { host: "h" }, [first] = [7, 8];
assert(host === "h");
assert(first === 7);
return 0;

//?
//? test: let-same-block
//? description: comma lets are visible later in the same block
//? expect: 0
//? source: ...

let a = 1, b = 2;
{
    assert(a + b === 3);
}
return 0;

//?
//? test: let-inner-block
//? description: inner multi let does not leak
//? expect: 0
//? source: ...

let a = 1;
{
    let b = 2, c = 3;
    assert(b + c === 5);
}
assert(a === 1);
return 0;

//?
//? test: let-in-function
//? description: multi let in a function
//? expect: 0
//? source: ...

function addPair() {
    let x = 1, y = 2;
    return x + y;
}
assert(addPair() === 3);
return 0;

//?
//? test: single-let-still-works
//? description: one-name let unchanged
//? expect: 0
//? source: ...

let x = 1;
assert(x === 1);
let y;
assert(y === undefined);
return 0;

//?
//? test: decompile-evaluates
//? description: decompile of multi let still evaluates
//? expect: 0
//? source: ...

const src = "let a = 1, b = 2; return a + b;";
const d1 = decompile(compile<script>(script(src)));
assert(evaluate(compile<script>(script(d1))) === 3);
return 0;

//?
//? test: extra-braces-inner-let
//? description: `{ { let x } }` — outer 0-symbol `{ }` is not a scope
//? expect: 0
//? source: ...

let outer = 1;
{
    {
        let inner = 2;
        assert(outer + inner === 3);
    }
}
return 0;

//?
//? test: while-zero-symbol-assigns-outer
//? description: braced while body with no let still assigns the outer name
//? expect: 0
//? source: ...

let i = 0;
while (i < 3) {
    i = i + 1;
}
assert(i === 3);
return 0;
