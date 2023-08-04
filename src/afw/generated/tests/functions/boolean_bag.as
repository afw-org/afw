#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/boolean_bag.as
//? customPurpose: Part of core function tests
//? description: Tests for bag<boolean> function.
//? sourceType: expression
//?
//? test: bag<boolean>-1
//? description: Create empty boolean bag.
//? expect: ...

bag<boolean>(
)

//? source: bag<boolean>()
//?
//? test: bag<boolean>-2
//? description: Create boolean bag with 2 booleans.
//? expect: ...

bag<boolean>(
    boolean(true),
    boolean(false)
)

//? source: ...

bag<boolean>(
    boolean(true),
    boolean(false)
)

//?
//? test: bag<boolean>-3
//? description: Create boolean bag with 21 booleans.
//? expect: ...

bag<boolean>(
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

bag<boolean>(
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
//? test: bag<boolean>-4
//? description: Create boolean bag with invalid arguments test.
//? expect: error
//? source: ...

bag<boolean>(
    boolean(x)
)

