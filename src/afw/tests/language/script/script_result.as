#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: script_result.as
//? customPurpose: Part of language/script tests
//? description: ...
Running script result (issue #62). Assignment and return write it.
let/const, call statements, if/for/while/try, and break do not reset
it. Nested assignment does write it. for init/increment do not write.
A lone expression is the result; a call statement is not.
//? sourceType: script
//?
//? test: only-let
//? description: let alone does not write the result
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script("let x = 1;")));
assert(r === undefined);
return 0;

//?
//? test: assign-writes
//? description: assignment writes the result
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script("let x; x = 7;")));
assert(r === 7);
return 0;

//?
//? test: let-after-assign
//? description: let after assignment leaves the assignment
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script("let x; x = 1; let y = 2;")));
assert(r === 1);
return 0;

//?
//? test: if-let-does-not-wipe
//? description: if with only let keeps the prior assignment
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script(
    "let x; x = 1; if (true) { let y = 2; }")));
assert(r === 1);
return 0;

//?
//? test: if-assign-writes
//? description: assignment inside if writes the result
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script(
    "let x; let y; x = 1; if (true) { y = 2; }")));
assert(r === 2);
return 0;

//?
//? test: if-false-does-not-wipe
//? description: if false does not reset the result
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script(
    "let x; let y; x = 1; if (false) { y = 2; }")));
assert(r === 1);
return 0;

//?
//? test: call-after-assign-does-not-write
//? description: abs(-3); after assignment keeps the assignment
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script("let x; x = 1; abs(-3);")));
assert(r === 1);
return 0;

//?
//? test: lone-call
//? description: a script that is only a call yields the call value
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script("abs(-3);")));
assert(r === 3);
return 0;

//?
//? test: lone-expression
//? description: a script that is only an expression yields it
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script("1 + 2")));
assert(r === 3);
return 0;

//?
//? test: return-wins
//? description: return sets the result and leaves
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script("let x; x = 1; return 9;")));
assert(r === 9);
return 0;

//?
//? test: break-preserves
//? description: break; does not wipe the running result
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script(
    "let x; x = 1; while (true) { break; }")));
assert(r === 1);
return 0;

//?
//? test: break-after-assign-in-loop
//? description: assignment before break is the result
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script(
    "let x; x = 1; while (true) { x = 2; break; }")));
assert(r === 2);
return 0;

//?
//? test: call-does-not-adopt-function-result
//? description: f(); as a statement does not write the caller result
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script(
    "function f() { return 4; } f();")));
assert(r === undefined);
const r2 = evaluate(compile<script>(script(
    "function f() { return 4; } let x; x = f();")));
assert(r2 === 4);
return 0;

//?
//? test: for-increment-does-not-write
//? description: i = i + 1 in the for head does not override a body assign
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script(
    "let x; let i; for (i = 0; i < 2; i = i + 1) { x = 10; }")));
assert(r === 10);
return 0;

//?
//? test: for-increment-does-not-wipe-prior
//? description: increment does not replace an assignment before the loop
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script(
    "let x; let i; x = 1; for (i = 0; i < 3; i = i + 1) { }")));
assert(r === 1);
return 0;
