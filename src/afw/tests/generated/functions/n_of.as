#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/n_of.as
//? customPurpose: Part of core function tests
//? description: Tests for n_of function.
//? sourceType: script
//?
//? test: n_of-1
//? description: x
//? expect: boolean(false)
//? source: ...

n_of(
    integer(6),
    boolean(true),
    boolean(true),
    boolean(true),
    boolean(true),
    boolean(true)
)

//?
//? test: n_of-2
//? description: x
//? expect: boolean(true)
//? source: ...

n_of(
    integer(2),
    boolean(false),
    boolean(false),
    boolean(false),
    boolean(true),
    boolean(true)
)

//?
//? test: n_of-3
//? description: x
//? expect: boolean(true)
//? source: ...

n_of(
    integer(6),
    boolean(true),
    boolean(true),
    boolean(true),
    boolean(true),
    boolean(true),
    boolean(true)
)

//?
//? test: n_of-4
//? description: Invalid arguments test.
//? expect: error
//? source: ...

n_of(
    integer(x),
    boolean(x)
)

