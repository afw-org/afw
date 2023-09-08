#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/and.as
//? customPurpose: Part of core function tests
//? description: Tests for and function.
//? sourceType: script
//?
//? test: and-1
//? description: x
//? expect: boolean(true)
//? source: ...

and(
    boolean(true),
    boolean(true),
    boolean(true)
)

//?
//? test: and-2
//? description: x
//? expect: boolean(false)
//? source: ...

and(
    boolean(true),
    boolean(true),
    boolean(false)
)

//?
//? test: and-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

and(
    boolean(x)
)

