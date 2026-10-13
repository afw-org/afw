#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/anyURI_regexp_match.as
//? customPurpose: Part of core function tests
//? description: Tests for regexp_match<anyURI> function.
//? sourceType: script
//?
//? test: regexp_match<anyURI>-1
//? description: Matches
//? expect: boolean(true)
//? source: ...

regexp_match<anyURI>(
    "http://www.oasis.org"anyURI,
    "http://.*\\.oasis\\.org"re
)

//?
//? test: regexp_match<anyURI>-2
//? description: Does not match
//? expect: boolean(false)
//? source: ...

regexp_match<anyURI>(
    "http://www.oasis.com"anyURI,
    "http://.*\\.oasis\\.org"re
)

//?
//? test: regexp_match<anyURI>-3
//? description: Invalid expression test.
//? expect: error
//? source: ...

regexp_match<anyURI>(
    "http://www.oasis.org"anyURI,
    to_regexp("http://.*\\.oasis\\.org)")
)

