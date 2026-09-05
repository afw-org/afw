#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/match_rfc822Name.as
//? customPurpose: Part of core function tests
//? description: match_rfc822Name is currently an unimplemented stub. This locks in that known behavior so a future implementation change is a deliberate test update, not a silent behavior change.
//? sourceType: script
//?
//? test: match_rfc822Name-error-not-implemented
//? description: match_rfc822Name always throws "Not implemented"
//? expect: error:Not implemented
//? source: ...

match_rfc822Name(rfc822Name("a@b.com"), regexp(".*"))
