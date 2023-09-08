#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dnsName_regexp_match.as
//? customPurpose: Part of core function tests
//? description: Tests for regexp_match<dnsName> function.
//? sourceType: script
//?
//? test: regexp_match<dnsName>-1
//? description: Matches
//? expect: boolean(true)
//? source: ...

regexp_match<dnsName>(
    dnsName("xacml.oasis.org"),
    string(".*\\.oasis\\.org")
)

//?
//? test: regexp_match<dnsName>-2
//? description: Does not match
//? expect: boolean(false)
//? source: ...

regexp_match<dnsName>(
    dnsName("xacml.oasis.com"),
    string(".*\\.oasis\\.org")
)

//?
//? test: regexp_match<dnsName>-3
//? description: Invalid expression test.
//? expect: error
//? source: ...

regexp_match<dnsName>(
    dnsName("xacml.oasis.org"),
    string(".*\\.oasis.org)")
)

