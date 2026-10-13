#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/boolean_array.as
//? customPurpose: Part of core function tests
//? description: Tests for array<boolean> function.
//? sourceType: script
//?
//? test: array<boolean>-1
//? description: Create empty boolean array.
//? expect: ...

array<boolean>(
)

//? source: array<boolean>()
//?
//? test: array<boolean>-2
//? description: Create boolean array with 2 booleans.
//? expect: ...

array<boolean>(
    boolean(true),
    boolean(false)
)

//? source: ...

array<boolean>(
    boolean(true),
    boolean(false)
)

//?
//? test: array<boolean>-3
//? description: Create boolean array with 21 booleans.
//? expect: ...

array<boolean>(
    boolean(true),
    boolean(false),
    boolean(0),
    boolean(1),
    boolean(true),
    boolean(false),
    boolean(true),
    boolean(1),
    boolean(1),
    boolean(0),
    boolean(true),
    boolean(false),
    boolean(0),
    boolean(1),
    boolean(true),
    boolean(false),
    boolean(true),
    boolean(1),
    boolean(1),
    boolean(0),
    boolean(false)
)

//? source: ...

array<boolean>(
    boolean(true),
    boolean(false),
    boolean(0),
    boolean(1),
    boolean(true),
    boolean(false),
    boolean(true),
    boolean(1),
    boolean(1),
    boolean(0),
    boolean(true),
    boolean(false),
    boolean(0),
    boolean(1),
    boolean(true),
    boolean(false),
    boolean(true),
    boolean(1),
    boolean(1),
    boolean(0),
    boolean(false)
)

//?
//? test: array<boolean>-4
//? description: Create boolean array with invalid arguments test.
//? expect: error
//? source: ...

array<boolean>(
    boolean(x)
)

