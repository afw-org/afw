#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/base64Binary_set_equals.as
//? customPurpose: Part of core function tests
//? description: Tests for set_equals<base64Binary> function.
//? sourceType: script
//?
//? test: set_equals<base64Binary>-1
//? description: x
//? expect: boolean(true)
//? source: ...

set_equals<base64Binary>(
    bag<base64Binary>(
        base64Binary("0fa8"),
        base64Binary("0F+40A=="),
        base64Binary("0fa80f+9")
    ),
    bag<base64Binary>(
        base64Binary("0fa8"),
        base64Binary("0F+40A=="),
        base64Binary("0fa80f+9"),
        base64Binary("0fa80f+9")
    )
)

//?
//? test: set_equals<base64Binary>-2
//? description: x
//? expect: boolean(true)
//? source: ...

set_equals<base64Binary>(
    bag<base64Binary>(
        base64Binary("0fa8"),
        base64Binary("0F+40A=="),
        base64Binary("0fa80f+9")
    ),
    bag<base64Binary>(
        base64Binary("0fa8"),
        base64Binary("0F+40A=="),
        base64Binary("0fa80f+9")
    )
)

//?
//? test: set_equals<base64Binary>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

set_equals<base64Binary>(
    bag<base64Binary>(
        base64Binary("x"),
        base64Binary("x")
    ),
    bag<base64Binary>(
        base64Binary("x"),
        base64Binary("x")
    )
)

