#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/ipAddress_regexp_match.as
//? customPurpose: Part of core function tests
//? description: Tests for regexp_match<ipAddress> function.
//? sourceType: expression
//?
//? test: regexp_match<ipAddress>-1
//? description: Matches
//? expect: boolean(true)
//? source: ...

regexp_match<ipAddress>(
    ipAddress("127.0.0.1"),
    string("127\\.0\\.0\\..*")
)

//?
//? test: regexp_match<ipAddress>-2
//? description: Does not match
//? expect: boolean(false)
//? source: ...

regexp_match<ipAddress>(
    ipAddress("127.0.1.1"),
    string("127\\.0\\.0\\..*")
)

//?
//? test: regexp_match<ipAddress>-3
//? description: Invalid expression test.
//? expect: error
//? source: ...

regexp_match<ipAddress>(
    ipAddress("127.0.0.1"),
    string("127\\.0\\.0\\..*)")
)

