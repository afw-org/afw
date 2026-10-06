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

at_least_one_member_of<anyURI>(dayTimeDuration("P1D"), continue())

//?
//? test: void-try-call
//? description: try() result as a bag argument is a conversion error
//? expect: error
//? source: ...

one_and_only<dnsName>(try("x", 1, hexBinary("00ff"), null))

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

object(base64Binary(""))

//?
//? test: date-year-overflow
//? description: a year too large for 32 bits is an invalid date
//? expect: error
//? source: ...

date("99999999999-01-01")

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
//? description: string() of a script function is its unit's source
//? expect: 0
//? source: ...

const s = string(function (x) { return x; });
assert(index_of(s, "function (x)") >= 0);
return 0;
