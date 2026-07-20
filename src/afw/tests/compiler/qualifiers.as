#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: qualifiers.as
//? customPurpose: Part of compiler category tests
//? description: Test qualifiers function (issue #9 snapshot)
//? sourceType: script
//?
//? test: qualifiers-returns-object
//? description: issue #9 qualifiers() returns object with environment
//? expect: 0
//? source: ...

const qs = qualifiers();
assert(qs !== null && qs !== undefined);
assert(qs.environment !== null && qs.environment !== undefined,
    "expected environment qualifier");
assert(qs.environment.HOME === environment::HOME);
return 0;

//?
//? test: qualifiers-fresh-object-each-call
//? description: issue #9 each qualifiers() call returns a new independent object
//? expect: 0
//? source: ...

const a = qualifiers();
const b = qualifiers();
a.__issue9_marker = true;
assert(b.__issue9_marker === undefined || b.__issue9_marker === null);
return 0;

//?
//? test: qualifiers-current-application
//? description: issue #9 qualifiers may include current from application push
//? expect: 0
//? source: ...

const qs = qualifiers();
/* current:: is pushed by application; at least mode or programName if present */
if (qs.current !== null && qs.current !== undefined) {
    assert(qs.current.programName !== undefined || qs.current.mode !== undefined ||
        qs.current.xctxUUID !== undefined || qs.current.pid !== undefined);
}
return 0;
