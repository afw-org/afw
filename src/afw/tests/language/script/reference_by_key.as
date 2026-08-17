#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: reference_by_key.as
//? customPurpose: Part of language/script tests
//? description: ...
Object property get via reference_by_key: missing name is undefined at
script level; the C evaluate path must still pop the eval stack and
restore error contextual. A braced block's AFW_ENDTRY resets the stack
and hides a leak; a single-statement for body does not.
//? sourceType: script
//?
//? test: missing-is-undefined
//? description: Missing object property is undefined (dot, bracket, computed)
//? expect: 0
//? source: ...

const o = { a: 1 };
const empty = {};
assert(o.a === 1, "present");
assert(o.missing === undefined, "dot missing");
assert(o["missing"] === undefined, "bracket missing");
const k = "nope";
assert(o[k] === undefined, "computed missing");
assert(empty.x === undefined, "empty object");
return 0;

//?
//? test: present-undefined-vs-missing
//? description: Stored undefined is still a present property; missing is undefined
//? expect: 0
//? source: ...

const o = { u: undefined };
assert(o.u === undefined, "stored undefined");
assert(o.missing === undefined, "missing");
return 0;

//?
//? test: array-oor-still-undefined
//? description: Array and string out of range stay undefined (sibling already pops)
//? expect: 0
//? source: ...

const a = [1];
const s = "ab";
assert(a[9] === undefined, "array out of range");
assert(s[9] === undefined, "string out of range");
return 0;

//?
//? test: missing-does-not-grow-eval-stack
//? description: Repeated missing gets do not exceed the eval-stack max
//? expect: 0
//? source: ...

/* Default evaluation_stack_maximum_count is 500. A braced while body
   is its own block: AFW_ENDTRY resets the stack each iteration and
   hides the leak. A single-statement for body does not. */
let last;
const o = {};
for (let i = 0; i < 600; i = i + 1) last = o.missing;
assert(last === undefined);
return 0;
