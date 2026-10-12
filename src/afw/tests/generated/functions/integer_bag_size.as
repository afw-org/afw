#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<integer> function.
//? sourceType: script
//?
//? test: bag_size<integer>-1
//? description: Integer bag size of 0.
//? expect: 0
//? source: ...

bag_size<integer>(
    bag<integer>(
    )
)

//?
//? test: bag_size<integer>-2
//? description: Integer bag size of 2.
//? expect: 2
//? source: ...

bag_size<integer>(
    bag<integer>(
        52,
        -15
    )
)

//?
//? test: bag_size<integer>-3
//? description: Integer bag size of 21.
//? expect: 21
//? source: ...

bag_size<integer>(
    bag<integer>(
        200,
        11,
        -11,
        96,
        99,
        -102,
        -586,
        986,
        387,
        512618,
        -512618,
        200,
        11,
        -11,
        96,
        99,
        -102,
        -586,
        986,
        -0,
        0
    )
)

