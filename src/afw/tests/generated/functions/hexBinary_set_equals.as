#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/hexBinary_set_equals.as
//? customPurpose: Part of core function tests
//? description: Tests for set_equals<hexBinary> function.
//? sourceType: script
//?
//? test: set_equals<hexBinary>-1
//? description: x
//? expect: boolean(true)
//? source: ...

set_equals<hexBinary>(
    bag<hexBinary>(
        "0fb7"hexBinary,
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary,
        "0fb7"hexBinary,
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary,
        "0fb7"hexBinary,
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary,
        "0fb7"hexBinary,
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary,
        "0fb7"hexBinary,
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary,
        "0fb7"hexBinary,
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary
    ),
    bag<hexBinary>(
        "0fb7"hexBinary,
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary
    )
)

//?
//? test: set_equals<hexBinary>-2
//? description: x
//? expect: boolean(true)
//? source: ...

set_equals<hexBinary>(
    bag<hexBinary>(
        "0fb7"hexBinary,
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary,
        "0fb7"hexBinary,
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary,
        "0fb7"hexBinary,
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary,
        "0fb7"hexBinary,
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary,
        "0fb7"hexBinary,
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary,
        "0fb7"hexBinary,
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary
    ),
    bag<hexBinary>(
        "0fb7"hexBinary,
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary,
        "0fb7"hexBinary,
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary
    )
)

//?
//? test: set_equals<hexBinary>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

set_equals<hexBinary>(
    bag<hexBinary>(
        "x"hexBinary,
        "x"hexBinary
    ),
    bag<hexBinary>(
        "x"hexBinary,
        "x"hexBinary
    )
)

