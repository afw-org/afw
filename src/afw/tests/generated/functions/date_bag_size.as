#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<date> function.
//? sourceType: script
//?
//? test: bag_size<date>-1
//? description: Five
//? expect: integer(5)
//? source: ...

bag_size<date>(
    bag<date>(
        date("2004-04-12"),
        date("-1970-01-01"),
        date("2004-04-12"),
        date("2004-04-12-05:00"),
        date("2004-04-12Z")
    )
)

//?
//? test: bag_size<date>-2
//? description: One
//? expect: integer(1)
//? source: ...

bag_size<date>(
    bag<date>(
        date("2004-04-12Z")
    )
)

//?
//? test: bag_size<date>-3
//? description: Zero
//? expect: integer(0)
//? source: ...

bag_size<date>(
    bag<date>(
    )
)

