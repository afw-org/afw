#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/boolean_is_in.as
//? customPurpose: Part of core function tests
//? description: Tests for is_in<boolean> function.
//? sourceType: expression
//?
//? test: is_in<boolean>-1
//? description: Boolean true in empty bag.
//? expect: boolean(false)
//? source: ...

is_in<boolean>(
    boolean(true),
    bag<boolean>(
    )
)

//?
//? test: is_in<boolean>-2
//? description: Boolean true in bag of only falses.
//? expect: boolean(false)
//? source: ...

is_in<boolean>(
    boolean(true),
    bag<boolean>(
        boolean(0),
        boolean(false)
    )
)

//?
//? test: is_in<boolean>-3
//? description: Boolean false in bag of only trues.
//? expect: boolean(false)
//? source: ...

is_in<boolean>(
    boolean(false),
    bag<boolean>(
        boolean(true),
        boolean(1)
    )
)

//?
//? test: is_in<boolean>-4
//? description: ...
Boolean true in bag where only the first item in the bag is true.
//? expect: boolean(true)
//? source: ...

is_in<boolean>(
    boolean(true),
    bag<boolean>(
        boolean(true),
        boolean(0),
        boolean(false),
        boolean(0),
        boolean(false)
    )
)

//?
//? test: is_in<boolean>-5
//? description: ...
Boolean false in bag where only the last item in the bag is false.
//? expect: boolean(true)
//? source: ...

is_in<boolean>(
    boolean(false),
    bag<boolean>(
        boolean(true),
        boolean(1),
        boolean(true),
        boolean(1),
        boolean(false)
    )
)

//?
//? test: is_in<boolean>-6
//? description: Boolean true in bag where only the middle item is true
//? expect: boolean(true)
//? source: ...

is_in<boolean>(
    boolean(true),
    bag<boolean>(
        boolean(0),
        boolean(false),
        boolean(0),
        boolean(false),
        boolean(true),
        boolean(0),
        boolean(false),
        boolean(0),
        boolean(false)
    )
)

//?
//? test: is_in<boolean>-7
//? description: Boolean false in bag where all items are false
//? expect: boolean(false)
//? source: ...

is_in<boolean>(
    boolean(true),
    bag<boolean>(
        boolean(0),
        boolean(false),
        boolean(0),
        boolean(false),
        boolean(false),
        boolean(0),
        boolean(false),
        boolean(0),
        boolean(false)
    )
)

