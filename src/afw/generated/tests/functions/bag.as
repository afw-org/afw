#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/bag.as
//? customPurpose: Part of core function tests
//? description: Tests for polymorphic functions.
//? sourceType: expression
//?
//? test: bag-1
//? description: BAG with integers
//? expect: ...

bag<integer>(
    integer(6),
    integer(5),
    integer(4),
    integer(2)
)

//? source: ...

bag(
    integer(6),
    integer(5),
    integer(4),
    integer(2)
)

