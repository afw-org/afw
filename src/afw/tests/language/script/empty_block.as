#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: empty_block.as
//? customPurpose: Part of language/script tests
//? description: ...
Empty `{ }` has no names and no statements, so compile omits the
runtime frame. `{ stmt }` and `{ let … }` stay frames.
//? sourceType: script
//?
//? test: empty-is-noop
//? description: empty `{ }` does not change last
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script("let x; x = 7; {};")));
assert(r === 7);
return 0;

//?
//? test: nested-empty
//? description: nested empty `{ }` is still a no-op
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script("let x; x = 3; { {} };")));
assert(r === 3);
return 0;

//?
//? test: empty-semicolon
//? description: `{ ; }` is empty
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script("let x; x = 4; { ; };")));
assert(r === 4);
return 0;

//?
//? test: block-with-statement
//? description: `{ stmt }` still runs
//? expect: 0
//? source: ...

let n = 0;
{ n = 1; }
assert(n === 1);
return 0;

//?
//? test: block-with-let
//? description: `{ let x }` still scopes
//? expect: 0
//? source: ...

let x = 9;
{ let x = 1; assert(x === 1); }
assert(x === 9);
return 0;

//?
//? test: while-empty
//? description: `while (false) {}` is a valid empty body
//? expect: 0
//? source: ...

while (false) {}
return 0;

//?
//? test: if-empty
//? description: `if` with empty then/else
//? expect: 0
//? source: ...

if (true) {}
if (false) {} else {}
return 0;

//?
//? test: try-empty-body
//? description: `try {}` with catch/finally
//? expect: 0
//? source: ...

let saw = false;
try {} catch (e) { saw = true; }
assert(saw === false);
try {} finally { saw = true; }
assert(saw === true);
return 0;

//?
//? test: function-empty-body
//? description: empty function body is still callable
//? expect: 0
//? source: ...

const f = function() {};
assert(f() === undefined);
return 0;
