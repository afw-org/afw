#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: function_edge_args.as
//? customPurpose: Part of miscellaneous category tests
//? description: ...
Built-in functions called with edge arguments found by a function-call
fuzzer. Each used to trap (SIGFPE), dereference a bad pointer, overflow
a signed integer, or pass NULL to memcpy. Under --env-mode asan these
also fail on an ASan or UBSan report.
//? sourceType: script
//?
//? test: divide-min-by-minus-one
//? description: divide #integerMin by -1 throws instead of SIGFPE
//? expect: error
//? source: ...

divide<integer>(#integerMin, -1)

//?
//? test: mod-min-by-minus-one
//? description: mod by -1 is 0, including #integerMin (used to SIGFPE)
//? expect: 0
//? source: ...

assert(mod<integer>(#integerMin, -1) === 0);
assert(mod<integer>(7, -1) === 0);
assert(mod<integer>(7, -2) === 1);
return 0;

//?
//? test: random-integer-full-range
//? description: random_integer over the whole range does not overflow
//? expect: 0
//? source: ...

const r = random_integer(#integerMin, #integerMax);
assert(meta(r).dataType === "integer");
const s = random_integer(-3, 3);
assert(s >= -3 && s <= 3);
return 0;

//?
//? test: void-to-array
//? description: continue() as an array argument is a conversion error
//? expect: error
//? source: ...

at_least_one_member_of<anyURI>("P1D"dayTimeDuration, continue())

//?
//? test: void-try-call
//? description: try() result as an array argument is an error
//? expect: error
//? source: ...

add_entries([], try("x", 1, "00ff"hexBinary, null))

//?
//? test: splice-huge-counts
//? description: splice clamps a huge start and delete count
//? expect: 0
//? source: ...

const a = [1, 2, 3];
const removed = splice(a, #integerMax, #integerMax, 4);
assert(length(removed) === 0);
assert(length(a) === 4);
const b = [1, 2, 3];
const removed2 = splice(b, 1, #integerMax);
assert(length(removed2) === 2);
assert(length(b) === 1);
return 0;

//?
//? test: empty-strings
//? description: concat, join, add<string>, string with empty strings
//? expect: 0
//? source: ...

assert(concat("", "") === "");
assert(concat("", "a", "") === "a");
assert(join(["", ""], "") === "");
assert(join(["", "a"], "") === "a");
assert(add<string>("", "") === "");
assert(string("", "") === "");
return 0;

//?
//? test: object-from-empty-binary
//? description: object() of an empty base64Binary throws (no NULL memcpy)
//? expect: error
//? source: ...

object(""base64Binary)

//?
//? test: date-year-overflow
//? description: a year too large for 32 bits is an invalid date
//? expect: error
//? source: ...

"99999999999-01-01"date

//?
//? test: date-from-integer-max
//? description: bag<date> of #integerMax is an invalid date
//? expect: error
//? source: ...

bag<date>(#integerMax)

//?
//? test: dateTime-from-integer-min
//? description: bag<dateTime> of #integerMin is an invalid dateTime
//? expect: error
//? source: ...

bag<dateTime>(#integerMin)

//?
//? test: function-to-duration
//? description: converting a script function value to a duration throws
//? expect: error
//? source: ...

bag<dayTimeDuration>(function (x) { return x; })

//?
//? test: function-to-string
//? description: string() of a script function is its own source (#498)
//? expect: 0
//? source: ...

const f = function (x) { return x; };
function h(y) { return y * 2; }
const o = { m: function (a: integer, b: integer): integer { return a + b; } };
assert(string(function (x) { return x; }) === "function (x) { return x; }");
assert(string(f) === "function (x) { return x; }");
assert(string(h) === "function h(y) { return y * 2; }");
assert(string(o.m) ===
    "function (a: integer, b: integer): integer { return a + b; }");
return 0;

//?
//? test: meta-of-unset-qualified
//? description: meta of an unset current:: variable is undefined (used to SEGV)
//? expect: 0
//? source: ...

const m = meta(current::not_set_anywhere);
assert(m.dataType === "undefined");
return 0;

//?
//? test: compile-depth
//? description: deeply nested source is an error, not a stack overflow
//? expect: error
//? source: ...

evaluate(compile<script>("return " + repeat<string>("(", 200000) + "1"
    + repeat<string>(")", 200000) + ";"))

//?
//? test: includes-position-min
//? description: includes with #integerMin position is false, not overflow
//? expect: 0
//? source: ...

assert(includes<string>("abc", "a", #integerMin) === false);
assert(includes<string>("abc", "c", -1) === true);
return 0;

//?
//? test: string-to-function
//? description: a string is not a function, and is not converted to one (#583)
//? expect: error:Parameter 1 of function 'bag<function>' must evaluate to data type 'function' but evaluated to be 'string'
//? source: ...

bag<function>("abc")

//?
//? test: returned-string-to-function
//? description: a string for a function throws, not a read of freed memory
//? expect: error
//? source: ...

return bag<function>("x");

//?
//? test: compile-listing-no-whitespace
//? description: compile listing with false/0 whitespace (used to deref NULL)
//? expect: 0
//? source: ...

const a = compile(template("a ${1+1}"), false);
const b = compile(script("return 1;"), 0);
assert(index_of(a, "Compiled Adaptive Value Listing") >= 0);
assert(index_of(b, "Compiled Adaptive Value Listing") >= 0);
return 0;

//?
//? test: integer-from-string-overflow
//? description: to_integer() of a string past the integer range throws; the digit sum used to overflow and could wrap back to a wrong value (2^64 gave 0)
//? expect: 0
//? source: ...

const bad = [
    "9223372036854775808",
    "-9223372036854775809",
    "18446744073709551616",
    "18446744073709551617",
    "-18446744073709551616",
    "92233720368547758080",
    "99999999999999999999999999999999999999"
];
for (const s of bad) {
    const r = safe_evaluate(to_integer(s), "error");
    assert(r === "error", s + " gave " + string(r));
}
assert(to_integer("9223372036854775807") === #integerMax);
assert(to_integer("-9223372036854775808") === #integerMin);
assert(to_integer("-0") === 0);
assert(to_double("18446744073709551616") === 1.8446744073709552e19);
return 0;

//?
//? test: join-function-values
//? description: join of an array holding a script function uses the function's string form (used to read past the value)
//? expect: 0
//? source: ...

const s = join(["a", function (x) { return x; }], "|");
assert(starts_with<string>(s, "a|"), s);
assert(index_of(s, "function (x)") >= 0, s);
return 0;
