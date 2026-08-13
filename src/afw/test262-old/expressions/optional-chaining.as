#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: optional-chaining.as
//? customPurpose: Part of test262
//? description: expressions/optional-chaining
//? sourceType: script
//?
//? test: iteration-statement-do
//? description: optional chain in test portion of do while statement
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

let count: integer = 0;
const obj: object = {a: true};
do {
    count = count + 1;
    break;
} while (obj?.a);
assert(count == 1, "count == 1");



//? test: iteration-statement-for
//? description: optional chain in init/test/update of for statement
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

let count: integer;
const obj: object = {a: true};
for (count = 0; obj?.a; count = count + 1) {
    if (count > 0) break;
}
assert(count == 1, "count == 1");



//? test: iteration-statement-while
//? description: optional chain in test portion of while statement
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

let count: integer = 0;
const obj: object = {a: true};
while (obj?.a) {
    count = count + 1;
    break;
}
assert(count == 1, "count == 1");



//? test: member-expression
//? description: optional chain on member expression
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

const a: object = {b: 22};
assert(22 === a?.b, "22 === a?.b");

// \fixme we don't support these
//assert(2 === [1,2]?.[1]);
//assert(44 === {a: 44}?.a);
//assert('h' === `hello`?.[0]);

const arr: array = [{a: 33}];
assert(33 === arr[0]?.a, "33 === arr[0]?.a");

const obj: object = {a: {b: 44}};
assert(44 === obj.a?.b, "44 === obj.a?.b");
assert(undefined === obj.c?.b, "undefined === obj.c?.b");



//? test: optional-chain-expression-optional-expression
//? description: optional chain bracket notation containing optional expresion
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

const a: any = undefined;
const b: object = {e: 0};
const c: object = {};
//c[undefined] = 11;
const d: array = [22];

assert(undefined === a?.[a?.b], "undefined === a?.[a?.b]");
//assert(11 === c?.[a?.b], "11 === c?.[a?.b]");
assert(22 === d?.[b?.e], "22 === d?.[b?.e]");



//? test: optional-chain
//? description: various optional chain expansions
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

const arr: array = [10, 11];
const obj: object = {
    a: "hello",
    b: {val: 13},
    c: function (arg1: integer): integer { return arg1 * 2; },
    arr: [11, 12],
};
const i: integer = 0;

// OptionalChain: ?.[Expression]
assert(11 === arr?.[i + 1], "11 === arr?.[i + 1]");

// OptionalChain: ?.IdentifierName
assert("hello" === obj?.a, "hello === obj?.a");

// OptionalChain: OptionalChain [Expression]
assert(12 === obj?.arr[i + 1], "12 === obj?.arr[i + 1]");

// OptionalChain: OptionalChain .IdentifierName
assert(13 === obj?.b.val, "13 === obj?.b.val");

// OptionalChain: OptionalChain Arguments
assert(20 === obj?.c(10), "20 === obj?.c(10)");



//? test: optional-expression
//? description: optional chain on recursive optional expression
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

const obj: object = {
    a: {
        b: 22
    }
};

function fn(): object {
    return {};
}

assert(22 === obj?.a?.b, "22 === obj?.a?.b");
assert(undefined === fn()?.a?.b, "undefined === fn()?.a?.b");



//? test: runtime-semantics-evaluation
//? description: accessing optional value on undefined or null returns undefined.
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

const nul: any = null;
const undf: any = undefined;

assert(undefined === nul?.a, "undefined === nul?.a");
assert(undefined === undf?.b, "undefined === undf?.b");



//? test: short-circuiting
//? description: demonstrate syntax-based short-circuiting.
//? expect: error:Parse error at offset 109 around line 7 column 5: Expecting Value
//? source: ...
#!/usr/bin/env afw

const a: any = undefined;
let x: integer = 1;

// we do not support pre-increment ++
a?.[++x];
a?.b.c(++x).d;

assert(1 === x, "1 === x");



//? test: static-semantics-simple-assignment
//? description: an optional expression cannot be target of assignment
//? expect: error
//? source: ...
#!/usr/bin/env afw

const obj: object = {};

obj?.a = 33;



//? test: update-expression-postfix
//? description: optional chaining is forbidden in write contexts
//? expect: error
//? source: ...
#!/usr/bin/env afw

const a: object = {};
a?.b++;



//? test: update-expression-prefix
//? description: optional chaining is forbidden in write contexts
//? expect: error
//? source: ...
#!/usr/bin/env afw

const a: object = {};
--a?.b;

