#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/boolean_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<boolean> function.
//? sourceType: script
//?
//? test: bag_size<boolean>-1
//? description: Boolean bag size of 2
//? expect: integer(2)
//? source: ...

bag_size<boolean>(
    bag<boolean>(
        boolean(true),
        boolean(0)
    )
)

//?
//? test: bag_size<boolean>-2
//? description: Boolean bag size of 0
//? expect: integer(0)
//? source: ...

bag_size<boolean>(
    bag<boolean>(
    )
)

//?
//? test: bag_size<boolean>-3
//? description: Boolean bag size of 12
//? expect: integer(12)
//? source: ...

bag_size<boolean>(
    bag<boolean>(
        boolean(true),
        boolean(0),
        boolean(true),
        boolean(true),
        boolean(false),
        boolean(1),
        boolean(1),
        boolean(false),
        boolean(true),
        boolean(1),
        boolean(0),
        boolean(0)
    )
)

