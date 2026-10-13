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
