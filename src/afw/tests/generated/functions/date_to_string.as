#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_to_string.as
//? customPurpose: Part of core function tests
//? description: Tests for to_string<date> function.
//? sourceType: script
//?
//? test: to_string<date>-1
//? description: x
//? expect: string("2004-04-12")
//? source: ...

to_string<date>(
    "2004-04-12"date
)

//?
//? test: to_string<date>-2
//? description: x
//? expect: string("0200-12-12")
//? source: ...

to_string<date>(
    "0200-12-12"date
)

//?
//? test: to_string<date>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

to_string<date>(
    "x"date
)

