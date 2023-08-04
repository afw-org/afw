#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_mod.as
//? customPurpose: Part of core function tests
//? description: Tests for mod<integer> function.
//? sourceType: expression
//?
//? test: mod<integer>-1
//? description: 15 mod 5
//? expect: integer(0)
//? source: ...

mod<integer>(
    integer(15),
    integer(5)
)

//?
//? test: mod<integer>-2
//? description: 15 mod 4
//? expect: integer(3)
//? source: ...

mod<integer>(
    integer(15),
    integer(4)
)

//?
//? test: mod<integer>-3
//? description: -15 mod 4
//? expect: integer(-3)
//? source: ...

mod<integer>(
    integer(-15),
    integer(4)
)

