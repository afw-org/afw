#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/regexp_match.as
//? customPurpose: Part of core function tests
//? description: Error-path tests for polymorphic functions. Happy-path coverage lives in the per-dataType files (e.g. string_regexp_match.as).
//? sourceType: script
//?
//? test: regexp_match-error-invalid-regexp
//? description: regexp_match throws when the regexp does not compile. The thrown message comes from libxml2 and is not asserted verbatim.
//? expect: error
//? source: ...

regexp_match<string>(
    string("abc"),
    string("(")
)
