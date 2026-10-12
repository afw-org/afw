#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_bag.as
//? customPurpose: Part of core function tests
//? description: Tests for bag<date> function.
//? sourceType: script
//?
//? test: bag<date>-1
//? description: One
//? expect: ...

bag<date>(
    "2004-04-12"date,
    "-1970-01-01"date,
    "2004-04-12"date,
    "2004-04-12-05:00"date,
    "2004-04-12Z"date
)

//? source: ...

bag<date>(
    "2004-04-12"date,
    "-1970-01-01"date,
    "2004-04-12"date,
    "2004-04-12-05:00"date,
    "2004-04-12Z"date
)

//?
//? test: bag<date>-2
//? description: Empty
//? expect: ...

bag<date>(
)

//? source: bag<date>()
