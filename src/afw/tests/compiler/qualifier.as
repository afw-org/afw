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

//?
//? test: qualifier-unknown-nullish
//? description: issue #9 qualifier for name with no stack frame is nullish
//? expect: 0
//? source: ...

assert(is_nullish(qualifier("no_such_qualifier_xyz_issue9")),
    "unknown qualifier should be undefined/nullish, not empty object");
return 0;

//?
//? test: qualifier-includeUntrusted-when-not-secure
//? description: issue #9 when not secure, includeUntrusted true/false match ::
//? expect: 0
//? source: ...

/* When xctx is not secure, true and false are the same (full :: visibility). */
const a = qualifier("environment", false);
const b = qualifier("environment", true);
assert(!is_nullish(a) && !is_nullish(b));
assert(a.HOME === b.HOME && a.HOME === environment::HOME);
return 0;
