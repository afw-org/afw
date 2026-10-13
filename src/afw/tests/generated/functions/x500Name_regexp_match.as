#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/x500Name_regexp_match.as
//? customPurpose: Part of core function tests
//? description: Tests for regexp_match<x500Name> function.
//? sourceType: script
//?
//? test: regexp_match<x500Name>-1
//? description: Matches
//? expect: boolean(true)
//? source: ...

regexp_match<x500Name>(
    "CN=Foo,DC=xyz,DC=bar"x500Name,
    "CN=Foo,DC=.*,DC=bar"re
)

//?
//? test: regexp_match<x500Name>-2
//? description: Does not match
//? expect: boolean(false)
//? source: ...

regexp_match<x500Name>(
    "CN=Foo,DC=xyz,DC=abc"x500Name,
    "CN=Foo,DC=.*,DC=bar"re
)

//?
//? test: regexp_match<x500Name>-3
//? description: Invalid expression test.
//? expect: error
//? source: ...

regexp_match<x500Name>(
    "CN=Foo,DC=xyz,DC=bar"x500Name,
    to_regexp("CN=Foo,DC=.*,DC=bar)")
)

