#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/base64Binary_is_in.as
//? customPurpose: Part of core function tests
//? description: Tests for is_in<base64Binary> function.
//? sourceType: expression
//?
//? test: is_in<base64Binary>-1
//? description: x
//? expect: boolean(true)
//? source: ...

is_in<base64Binary>(
    base64Binary("0fa8"),
    bag<base64Binary>(
        base64Binary("0fa8"),
        base64Binary("0fa80f+9"),
        base64Binary("0F+40A==")
    )
)

//?
//? test: is_in<base64Binary>-2
//? description: x
//? expect: boolean(false)
//? source: ...

is_in<base64Binary>(
    base64Binary("0fa9"),
    bag<base64Binary>(
        base64Binary("0fa8"),
        base64Binary("0fa80f+9"),
        base64Binary("0F+40A==")
    )
)

//?
//? test: is_in<base64Binary>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

is_in<base64Binary>(
    base64Binary("x"),
    bag<base64Binary>(
        base64Binary("x"),
        base64Binary("x")
    )
)

