#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: negative_index.as
//? customPurpose: Part of language tests
//? description: ...
a[i] and s[i] count from 0 only, as in ECMAScript (#630): a negative
index reads as undefined and is an error to assign. at(), with_at(),
slice() and splice() count from the end, as in ECMAScript.
//? sourceType: script
//?
//? test: read
//? description: a negative index reads as undefined for an array, a string and an anyURI
//? expect: 0
//? source: ...

const a = [1, 2, 3];
assert(a[-1] === undefined && a[-3] === undefined && a[-4] === undefined);
assert("abc"[-1] === undefined);
assert("http://a"anyURI[-1] === undefined);
assert(a?.[-1] === undefined, "optional chaining");
const i = 0;
assert(a[i - 1] === undefined, "the accident: a[i - 1] at i = 0");
return 0;

//?
//? test: functions-count-from-the-end
//? description: at, with_at, slice and splice still count from the end
//? expect: 0
//? source: ...

const a = [1, 2, 3];
assert(a->at(-1) === 3 && "abc"->at(-1) === "c");
assert(eq(a->with_at(-1, 9), [1, 2, 9]));
assert(eq(a->slice(-2), [2, 3]));
assert(eq(a->to_spliced(-1, 1), [1, 2]));
return 0;

//?
//? test: assign
//? description: assigning a negative index is an error that says what to use
//? expect: error:Index -1 is before the start of an array of length 3; at(a, i) and with_at(a, i, v) count from the end
//? source: ...

let a = [1, 2, 3];
a[-1] = 9;

//?
//? test: compound-assign
//? description: a compound assignment to a negative index is an error too
//? expect: error
//? source: ...

let a = [1, 2, 3];
a[-1] += 1;

//?
//? test: assign-error-ids
//? description: a[i] = v errors are argument_error or read_only, not general
//? expect: ["argument_error", "argument_error", "read_only"]
//? source: ...

let ids = [];
let a = [1];
try { a[-1] = 1; } catch (e) { ids->push(e.id); }
try { a["0"] = 1; } catch (e) { ids->push(e.id); }
const f = freeze([1]);
try { f[0] = 2; } catch (e) { ids->push(e.id); }
return ids;
