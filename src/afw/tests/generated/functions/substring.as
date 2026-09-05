#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/substring.as
//? customPurpose: Part of core function tests
//? description: Error-path tests for polymorphic functions. Happy-path coverage lives in the per-dataType files (e.g. string_substring.as).
//? sourceType: script
//?
//? test: substring-error-negative-start
//? description: substring throws when startIndex is negative
//? expect: error:startIndex is out of bounds
//? source: ...

substring(
    string("hello"),
    integer(-1)
)

//?
//? test: substring-error-start-past-end
//? description: substring throws when startIndex is past the end of the string
//? expect: error:startIndex is out of bounds
//? source: ...

substring(
    string("hello"),
    integer(10)
)

//?
//? test: substring-error-end-not-after-start
//? description: substring throws when endIndex does not come after startIndex
//? expect: error:endIndex is out of bounds
//? source: ...

substring(
    string("hello"),
    integer(3),
    integer(2)
)
