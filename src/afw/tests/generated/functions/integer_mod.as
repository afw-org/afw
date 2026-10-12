#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_mod.as
//? customPurpose: Part of core function tests
//? description: Tests for mod<integer> function.
//? sourceType: script
//?
//? test: mod<integer>-1
//? description: 15 mod 5
//? expect: 0
//? source: ...

mod<integer>(
    15,
    5
)

//?
//? test: mod<integer>-2
//? description: 15 mod 4
//? expect: 3
//? source: ...

mod<integer>(
    15,
    4
)

//?
//? test: mod<integer>-3
//? description: -15 mod 4
//? expect: -3
//? source: ...

mod<integer>(
    -15,
    4
)

