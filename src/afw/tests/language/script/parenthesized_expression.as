#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: parenthesized_expression.as
//? customPurpose: Part of language/script tests
//? description: ...
A parenthesized expression can be called and its members accessed, like a
name: (v as object).name, (a ?? b)?.[k], (f)(x) (#582). A statement that
starts with '(' and then '{' or '[' is a destructuring assignment only if an
assignment operator follows the matching '}' or ']'.
//? sourceType: script
//?
//? test: member-access
//? description: '.', '?.', '[ ]', and '?.[ ]' after a parenthesized expression
//? expect: 0
//? source: ...

const o = {x: {y: 2}, list: [5, 6]};
const n = null;
assert((o).x.y === 2);
assert((o.list)[1] === 6);
assert((n ?? o)?.x?.y === 2);
assert((n)?.x === undefined);
assert((n)?.["x"] === undefined);
assert(({a: 11}).a === 11);
assert(([3, 4])[1] === 4);
assert((o.x ?? {}).y === 2);
assert(((true) ? o : n).x.y === 2);
return 0;

//?
//? test: type-assertion-chain
//? description: (v as T) can be the base of a chain
//? expect: 0
//? source: ...

const v: any = {name: "a", list: [1, 2]};
const s: any = "text";
assert((v as object).name === "a");
assert((v as object)?.list[1] === 2);
assert(length((s as string)) === 4);
assert((s as string)->length() === 4);
return 0;

//?
//? test: calls
//? description: a parenthesized function can be called, and the chain goes on after the call
//? expect: 0
//? source: ...

const f = function (x) { return {value: x}; };
const g = function (x) { return function (y) { return x + y; }; };
assert((f)(3).value === 3);
assert((g)(1)(2) === 3);
assert((f ?? g)(4)?.value === 4);
assert(([1, 2, 3])->length() === 3);
return 0;

//?
//? test: assignment-targets
//? description: a member of a parenthesized expression can be assigned, and (a) = 1 still assigns a
//? expect: 0
//? source: ...

let o = {x: 1, list: [1]};
let a = 1;
(o).x = 12;
assert(o.x === 12);
(o).x++;
assert(o.x === 13);
(o.list)[0] += 4;
assert(o.list[0] === 5);
(a) = 14;
assert(a === 14);
return 0;

//?
//? test: parenthesized-destructuring
//? description: ({…} = v) and ([…] = v) destructure; the lookahead skips nested brackets
//? expect: 0
//? source: ...

let a = 0;
let b = 0;
({a, b} = {a: 3, b: 4});
assert(a === 3 && b === 4);
([a, b] = [5, 6]);
assert(a === 5 && b === 6);
({a, b: [b]} = {a: 7, b: [8]});
assert(a === 7 && b === 8);
([a, {b}] = [9, {b: 10}]);
assert(a === 9 && b === 10);
return 0;

//?
//? test: parenthesized-literal-statement
//? description: ({a: 1}); is an expression statement, which is an error like [1]; and 1; (not "Invalid assignment target")
//? expect: 0
//? source: ...

function compileError(source) {
    try {
        compile<script>(script(source));
    }
    catch (e) {
        return e.message;
    }
    return "compiled";
}

assert(includes(compileError("({a: 1}); return 0;"),
    "Expression can not be followed by Statement"), "object literal");
assert(includes(compileError("({}); return 0;"),
    "Expression can not be followed by Statement"), "empty object");
assert(includes(compileError("[1]; return 0;"),
    "Expression can not be followed by Statement"), "array literal");
assert(compileError("return ({a: 1}).a;") === "compiled", "member of literal");
assert(compileError("let o = {a: 1}; ({a: 1}).a === o.a;") === "compiled",
    "last expression");
return 0;

//?
//? test: script-is-parenthesized-chain
//? description: a script that is one expression can start with a parenthesized chain
//? expect: 11
//? source: ...

({a: 11}).a
