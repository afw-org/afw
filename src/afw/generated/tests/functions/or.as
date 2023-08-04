#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/or.as
//? customPurpose: Part of core function tests
//? description: Tests for or function.
//? sourceType: expression
//?
//? test: or-1
//? description: x
//? expect: boolean(false)
//? source: ...

or(
    boolean(false)
)

//?
//? test: or-2
//? description: x
//? expect: boolean(true)
//? source: ...

or(
    boolean(true)
)

//?
//? test: or-3
//? description: x
//? expect: boolean(true)
//? source: ...

or(
    boolean(false),
    boolean(false),
    boolean(false),
    boolean(false),
    boolean(false),
    boolean(true)
)

//?
//? test: or-4
//? description: Invalid arguments test.
//? expect: error
//? source: ...

or(
    boolean(x)
)

