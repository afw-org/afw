#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_bag.as
//? customPurpose: Part of core function tests
//? description: Tests for bag<date> function.
//? sourceType: expression
//?
//? test: bag<date>-1
//? description: One
//? expect: ...

bag<date>(
    date("2004-04-12"),
    date("-1970-01-01"),
    date("2004-04-12"),
    date("2004-04-12-05:00"),
    date("2004-04-12Z")
)

//? source: ...

bag<date>(
    date("2004-04-12"),
    date("-1970-01-01"),
    date("2004-04-12"),
    date("2004-04-12-05:00"),
    date("2004-04-12Z")
)

//?
//? test: bag<date>-2
//? description: Empty
//? expect: ...

bag<date>(
)

//? source: bag<date>()
