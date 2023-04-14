#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/not.as
//? customPurpose: Part of core function tests
//? description: Tests for not function.
//? sourceType: expression
//?
//? test: not-1
//? description: x
//? expect: boolean(true)
//? source: ...

not(
    boolean(false)
)

//?
//? test: not-2
//? description: x
//? expect: boolean(false)
//? source: ...

not(
    boolean(true)
)

//?
//? test: not-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

not(
    boolean(x)
)

