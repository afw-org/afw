#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_at_least_one_member_of.as
//? customPurpose: Part of core function tests
//? description: Tests for at_least_one_member_of<date> function.
//? sourceType: expression
//?
//? test: at_least_one_member_of<date>-1
//? description: true
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<date>(
    bag<date>(
        date("2004-04-12"),
        date("1970-01-01"),
        date("2004-04-12"),
        date("2004-04-12-05:00"),
        date("2004-04-12Z")
    ),
    bag<date>(
        date("1970-01-01")
    )
)

//?
//? test: at_least_one_member_of<date>-2
//? description: true
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<date>(
    bag<date>(
        date("1970-01-01")
    ),
    bag<date>(
        date("1970-01-01"),
        date("2004-04-12"),
        date("1969-01-01"),
        date("2004-04-12"),
        date("-2004-04-12-05:00"),
        date("2004-04-12Z")
    )
)

//?
//? test: at_least_one_member_of<date>-3
//? skip: true
//? description: false
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<date>(
    bag<date>(
        date("-1946-01-01")
    ),
    bag<date>(
        date("-2045-01-01"),
        date("2004-04-12"),
        date("2045-01-01"),
        date("2004-04-12"),
        date("2004-04-12-05:00"),
        date("2004-04-12Z")
    )
)

