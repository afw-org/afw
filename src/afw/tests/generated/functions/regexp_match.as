#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/regexp_match.as
//? customPurpose: Part of core function tests
//? description: Error-path tests for polymorphic functions. Happy-path coverage lives in the per-dataType files (e.g. string_regexp_match.as).
//? sourceType: script
//?
//? test: regexp_match-error-string-pattern
//? description: A string is not a regexp; write a regexp literal or use to_regexp.
//? expect: error:Parameter 2 of function 'regexp_match<string>' must evaluate to data type 'regexp' but evaluated to be 'string'
//? source: ...

regexp_match<string>(
    string("abc"),
    string("a")
)

//?
//? test: regexp_match-error-invalid-regexp
//? description: to_regexp throws conversion_error when the pattern does not compile.
//? expect: error:Invalid regular expression: U_REGEX_MISMATCHED_PAREN at line 1 offset 1
//? source: ...

regexp_match<string>(
    string("abc"),
    to_regexp("(")
)

//?
//? test: regexp_match-backtracking-limit
//? description: A pattern that backtracks exponentially on the value stops at the match limit instead of holding the thread.
//? expect: error:Regular expression match exceeded its limit: the pattern backtracks too much on this value
//? source: ...

regexp_match(repeat("a", 40) + "!", /^(a+)+$/)

//?
//? test: regexp_match-long-linear
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
    regexp_match(s, /[a-z]+b/),
    regexp_index_of(s, /b/),
    length(regexp_replace(s, /a/, "", -1))
];
