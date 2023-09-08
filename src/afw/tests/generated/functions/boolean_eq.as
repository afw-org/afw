#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/boolean_eq.as
//? customPurpose: Part of core function tests
//? description: Tests for eq<boolean> function.
//? sourceType: script
//?
//? test: eq<boolean>-1
//? description: True equals False test.
//? expect: boolean(false)
//? source: ...

eq<boolean>(
    boolean(true),
    boolean(false)
)

//?
//? test: eq<boolean>-2
//? description: False equals True test.
//? expect: boolean(false)
//? source: ...

eq<boolean>(
    boolean(false),
    boolean(true)
)

//?
//? test: eq<boolean>-3
//? description: True equals True test.
//? expect: boolean(true)
//? source: ...

eq<boolean>(
    boolean(true),
    boolean(true)
)

//?
//? test: eq<boolean>-4
//? description: False equals False test.
//? expect: boolean(true)
//? source: ...

eq<boolean>(
    boolean(false),
    boolean(false)
)

//?
//? test: eq<boolean>-5
//? description: 1 equals True test.
//? expect: boolean(true)
//? source: ...

eq<boolean>(
    boolean(1),
    boolean(true)
)

//?
//? test: eq<boolean>-6
//? description: 1 equals False test.
//? expect: boolean(false)
//? source: ...

eq<boolean>(
    boolean(1),
    boolean(false)
)

//?
//? test: eq<boolean>-7
//? description: 0 equals True test.
//? expect: boolean(false)
//? source: ...

eq<boolean>(
    boolean(0),
    boolean(true)
)

//?
//? test: eq<boolean>-8
//? description: 0 equals False test.
//? expect: boolean(true)
//? source: ...

eq<boolean>(
    boolean(0),
    boolean(false)
)

//?
//? test: eq<boolean>-9
//? description: 0 equals 0 test.
//? expect: boolean(true)
//? source: ...

eq<boolean>(
    boolean(0),
    boolean(0)
)

//?
//? test: eq<boolean>-10
//? description: 0 equals 1 test.
//? expect: boolean(false)
//? source: ...

eq<boolean>(
    boolean(0),
    boolean(1)
)

//?
//? test: eq<boolean>-11
//? description: 1 equals 1 test.
//? expect: boolean(true)
//? source: ...

eq<boolean>(
    boolean(1),
    boolean(1)
)

//?
//? test: eq<boolean>-12
//? description: Invalid boolean test.
//? expect: error
//? source: ...

eq<boolean>(
    boolean(false),
    boolean(invalidboolean)
)

