#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: qualifier.as
//? customPurpose: Part of compiler category tests
//? description: Test qualifier function (issue #9 snapshot)
//? sourceType: script
//?
//? test: qualifier-no-params
//? description: Test qualifier without any parameters
//? expect: error
//? source: ...

qualifier();

//?
//? test: qualifier-environment-home
//? description: issue #9 qualifier("environment") matches environment::HOME
//? expect: 0
//? source: ...

const q = qualifier("environment");
assert(q !== null && q !== undefined, "qualifier returned nullish");
assert(q.HOME === environment::HOME, "HOME mismatch");
assert(meta(q.HOME).dataType === "string", "HOME should be string");
return 0;

//?
//? test: qualifier-fresh-object-each-call
//? description: issue #9 each qualifier() call returns a new independent object
//? expect: 0
//? source: ...

const a = qualifier("environment");
const b = qualifier("environment");
assert(a.HOME === b.HOME);
/* Mutate first snapshot; second must not share residual props */
a.__issue9_marker = true;
assert(b.__issue9_marker === undefined || b.__issue9_marker === null,
    "second qualifier() snapshot shared mutated property");
const c = qualifier("environment");
assert(c.__issue9_marker === undefined || c.__issue9_marker === null,
    "third call should be a fresh snapshot");
return 0;
