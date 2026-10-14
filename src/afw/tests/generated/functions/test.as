#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/test.as
//? customPurpose: Part of core function tests
//? description: Tests for test: whether a regexp matches anywhere in a value.
//? sourceType: script
//?
//? test: test-found
//? description: True when the regexp matches anywhere in the value.
//? expect: [true, false]
//? source: ...

[test(/a+b/, "xyzaab"), test(/a+b/, "xyz")]

//?
//? test: test-method
//? description: The regexp is the receiver, as in ECMAScript's re.test(s).
//? expect: [true, true]
//? source: ...

let re = /^adm/;
return [re->test("admin"), /^adm/->test("admin")];

//?
//? test: test-anchored
//? description: ^ and \z match the whole value.
//? expect: [true, false]
//? source: ...

[test(/^ab\z/, "ab"), test(/^ab\z/, "abc")]

//?
//? test: test-text-types
//? description: Any value whose data type holds text can be tested.
//? expect: [true, true, true, true, true]
//? source: ...

[
    test(/oasis/, "http://www.oasis.org"anyURI),
    test(/\.org$/, "www.oasis.org"dnsName),
    test(/^10\./, "10.0.0.1"ipAddress),
    test(/@example/, "a@example.com"rfc822Name),
    test(/cn=/i, "CN=Bob, O=Example"x500Name)
]

//?
//? test: test-not-text
//? description: A value that does not hold text is an error.
//? expect: error:Value's data type not supported for this function
//? source: ...

test(/1/, 1)

//?
//? test: test-string-pattern
//? description: A string is not a regexp.
//? expect: error:Parameter 1 of function 'test' must evaluate to data type 'regexp' but evaluated to be 'string'; write "a"regexp or to_regexp(v)
//? source: ...

test("a", "abc")

//?
//? test: test-error-invalid-regexp
//? description: to_regexp throws conversion_error when the pattern does not compile.
//? expect: error:Invalid regular expression: U_REGEX_MISMATCHED_PAREN at line 1 offset 1
//? source: ...

test(
    to_regexp("("),
    string("abc")
)

//?
//? test: test-lazy-loop-over-empty-group
//? description: An ICU issue (#613): a lazy loop over a group that can match nothing never finishes; the step limit stops it, and the message says how to write it.
//? expect: ["payload_too_large", false]
//? source: ...

let id = "no error";
try {
    test(/(b|)*?^(?=a)/, "   .xb");
}
catch (e) {
    id = e.id;
}
return [id, test(/b*?^(?=a)/, "   .xb")];

//?
//? test: test-backtrack-memory
//? description: The backtrack stack is at most 8 MB, outside the pools: ^(a|b)*$ on 131,072 a matches, and on 262,144 it stops with payload_too_large instead of growing.
//? expect: [true, "payload_too_large"]
//? source: ...

let t = "a";
for (let i: integer = 0; i < 17; i = i + 1) {
    t = t + t;
}
const fits = test(/^(a|b)*$/, t);
let id = "no error";
try {
    test(/^(a|b)*$/, t + t);
}
catch (e) {
    id = e.id;
}
return [fits, id];

//?
//? test: test-default-limit
//? description: The default limitRegexpMatchSteps is 1000; at it, ^(a+)+$ on 22 a and ! completes and on 23 does not.
//? expect: [1000, false, "payload_too_large"]
//? source: ...

let over = "no error";
try {
    test(/^(a+)+$/, repeat("a", 23) + "!");
}
catch (e) {
    over = e.id;
}
return [
    process::limitRegexpMatchSteps,
    test(/^(a+)+$/, repeat("a", 22) + "!"),
    over
];

//?
//? test: test-backtracking-limit
//? description: A pattern that backtracks exponentially on the value stops at the match limit instead of holding the thread.
//? expect: error:Regular expression match exceeded limitRegexpMatchSteps: the pattern backtracks too much on this value, or has a lazy loop over a group that can match nothing, such as (b|)*? (write b*?)
//? source: ...

test(/^(a+)+$/, repeat("a", 40) + "!")

//?
//? test: test-long-linear
//? description: The limit leaves a linear scan of a long value alone (2 MB, and 2 million replacements).
//? expect: [2048001, true, 2048000, 1]
//? source: ...

let s = repeat("a", 1000);
for (let i: integer = 0; i < 11; i = i + 1) {
    s = s + s;
}
s = s + "b";
return [
    length(s),
    test(/[a-z]+b/, s),
    search(s, /b/),
    length(replace(s, /a/, "", -1))
];
