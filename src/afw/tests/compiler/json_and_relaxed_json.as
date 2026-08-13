#!/usr/bin/env -S afw --syntax test_script
//? testScript: json_and_relaxed_json.as
//? description: json and relaxed_json source-holding types and compile
//? sourceType: script
//?
//? test: source-holds-text
//? description: json() and relaxed_json() hold source, not a parsed object
//? expect: 0
//? source: ...

const j = json("{}");
assert(string(j) === "{}");
assert(is_string(j) === false);
assert(is_object(compile(j)) === true);

const r = relaxed_json("{a:1}");
assert(string(r) === "{a:1}");
assert(is_string(r) === false);
const o = compile(r);
assert(o.a === 1);
return 0;

//?
//? test: json-rejects-relaxed-syntax
//? description: Strict json compile rejects an unquoted property name
//? expect: error
//? source: ...

compile(json("{a:1}"));

//?
//? test: json-rejects-comments
//? expect: error
//? source: ...

compile(json("[/* c */ 1]"));

//?
//? test: json-rejects-trailing-comma
//? expect: error
//? source: ...

compile(json("[1,]"));

//?
//? test: json-rejects-infinity
//? description: Strict json does not allow Infinity
//? expect: error
//? source: ...

compile(json("[Infinity]"));

//?
//? test: json-rejects-nan
//? description: Strict json does not allow NaN
//? expect: error
//? source: ...

compile(json("[NaN]"));

//?
//? test: json-rejects-negative-infinity
//? description: Strict json does not allow -Infinity
//? expect: error
//? source: ...

compile(json("[-Infinity]"));

//?
//? test: relaxed-accepts-documented-forms
//? description: Comments, unquoted names, trailing comma, Infinity and NaN
//? expect: 0
//? source: ...

const withComment = compile(relaxed_json("[/* c */ 1]"));
assert(withComment[0] === 1);

const named = compile(relaxed_json("{a: 1,}"));
assert(named.a === 1);

const nums = compile(relaxed_json("[Infinity, -Infinity, NaN]"));
assert(is_NaN(nums[2]) === true);
return 0;
