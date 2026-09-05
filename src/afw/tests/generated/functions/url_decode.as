#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/url_decode.as
//? customPurpose: Part of core function tests
//? description: url_decode is currently an unimplemented stub. This locks in that known behavior so a future implementation change is a deliberate test update, not a silent behavior change.
//? sourceType: script
//?
//? test: url_decode-error-not-implemented
//? description: url_decode always throws "Not implemented"
//? expect: error:Not implemented
//? source: ...

url_decode(string("a%20b"))
