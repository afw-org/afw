#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_union.as
//? customPurpose: Part of core function tests
//? description: Tests for union<date> function.
//? sourceType: script
//?
//? test: union<date>-1
//? description: test 1
//? expect: ...

bag<date>(
    date("2004-04-12"),
    date("1970-01-01"),
    date("2004-05-12"),
    date("2004-04-13")
)

//? source: ...

union<date>(
    bag<date>(
        date("2004-04-12")
    ),
    bag<date>(
        date("1970-01-01"),
        date("2004-04-12"),
        date("2004-05-12"),
        date("2004-04-13")
    )
)

//?
//? test: union<date>-2
//? description: test 2
//? expect: ...

bag<date>(
    date("1970-01-01"),
    date("1970-01-02")
)

//? source: ...

union<date>(
    bag<date>(
        date("1970-01-01")
    ),
    bag<date>(
        date("1970-01-02")
    )
)

