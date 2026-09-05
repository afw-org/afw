#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/match_x500Name.as
//? customPurpose: Part of core function tests
//? description: match_x500Name is currently an unimplemented stub. This locks in that known behavior so a future implementation change is a deliberate test update, not a silent behavior change.
//? sourceType: script
//?
//? test: match_x500Name-error-not-implemented
//? description: match_x500Name always throws "Not implemented"
//? expect: error:Not implemented
//? source: ...

match_x500Name(x500Name("cn=test"), regexp(".*"))
