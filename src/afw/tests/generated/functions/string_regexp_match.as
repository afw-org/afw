#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_regexp_match.as
//? customPurpose: Part of core function tests
//? description: Tests for regexp_match<string> function.
//? sourceType: expression
//?
//? test: regexp_match<string>-1
//? description: Matches
//? expect: boolean(true)
//? source: ...

regexp_match<string>(
    string("aaaab"),
    string("(a)*b")
)

//?
//? test: regexp_match<string>-2
//? description: Does not match
//? expect: boolean(false)
//? source: ...

regexp_match<string>(
    string("aaaa"),
    string("(a)*b")
)

//?
//? test: regexp_match<string>-3
//? description: Invalid expression test.
//? expect: error
//? source: ...

regexp_match<string>(
    string("aaaab"),
    string("(a*b")
)

