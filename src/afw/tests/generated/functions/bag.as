#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/bag.as
//? customPurpose: Part of core function tests
//? description: Tests for polymorphic functions.
//? sourceType: script
//?
//? test: bag-1
//? description: BAG with integers
//? expect: ...

bag<integer>(
    6,
    5,
    4,
    2
)

//? source: ...

bag(
    6,
    5,
    4,
    2
)

