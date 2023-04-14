#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_bag.as
//? customPurpose: Part of core function tests
//? description: Tests for bag<integer> function.
//? sourceType: expression
//?
//? test: bag<integer>-1
//? description: Create empty integer bag.
//? expect: ...

bag<integer>(
)

//? source: bag<integer>()
//?
//? test: bag<integer>-2
//? description: Create integer bag with 2 integers.
//? expect: ...

bag<integer>(
    integer(52),
    integer(-15)
)

//? source: ...

bag<integer>(
    integer(52),
    integer(-15)
)

//?
//? test: bag<integer>-3
//? description: Create integer bag with 21 integers.
//? expect: ...

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
    integer(5126189454),
    integer(-5126189454),
    integer(200),
    integer(11),
    integer(-11),
    integer(96),
    integer(99),
    integer(-102),
    integer(-586),
    integer(986),
    integer(0),
    integer(0)
)

//? source: ...

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
    integer(5126189454),
    integer(-5126189454),
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

