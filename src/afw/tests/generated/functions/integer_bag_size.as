#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<integer> function.
//? sourceType: script
//?
//? test: bag_size<integer>-1
//? description: Integer bag size of 0.
//? expect: integer(0)
//? source: ...

bag_size<integer>(
    bag<integer>(
    )
)

//?
//? test: bag_size<integer>-2
//? description: Integer bag size of 2.
//? expect: integer(2)
//? source: ...

bag_size<integer>(
    bag<integer>(
        integer(52),
        integer(-15)
    )
)

//?
//? test: bag_size<integer>-3
//? description: Integer bag size of 21.
//? expect: integer(21)
//? source: ...

bag_size<integer>(
    bag<integer>(
        integer(200),
        integer(11),
        integer(-11),
        integer(96),
        integer(99),
        integer(-102),
        integer(-586),
        integer(986),
        integer(387),
        integer(512618),
        integer(-512618),
        integer(200),
        integer(11),
        integer(-11),
        integer(96),
        integer(99),
        integer(-102),
        integer(-586),
        integer(986),
        integer(-0),
        integer(0)
    )
)

