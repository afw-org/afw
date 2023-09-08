#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_regexp_match.as
//? customPurpose: Part of core function tests
//? description: Tests for regexp_match<rfc822Name> function.
//? sourceType: script
//?
//? test: regexp_match<rfc822Name>-1
//? description: Matches
//? expect: boolean(true)
//? source: ...

regexp_match<rfc822Name>(
    rfc822Name("root@oasis.org"),
    string(".*@oasis.org")
)

//?
//? test: regexp_match<rfc822Name>-2
//? description: Does not match
//? expect: boolean(false)
//? source: ...

regexp_match<rfc822Name>(
    rfc822Name("root@oasis.com"),
    string(".*@oasis.org")
)

//?
//? test: regexp_match<rfc822Name>-3
//? description: Invalid expression test.
//? expect: error
//? source: ...

regexp_match<rfc822Name>(
    rfc822Name("root@oasis.org"),
    string(".*@oasis.org)")
)

