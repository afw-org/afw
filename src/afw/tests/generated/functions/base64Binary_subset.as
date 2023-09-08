#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/base64Binary_subset.as
//? customPurpose: Part of core function tests
//? description: Tests for subset<base64Binary> function.
//? sourceType: script
//?
//? test: subset<base64Binary>-1
//? description: x
//? expect: boolean(true)
//? source: ...

subset<base64Binary>(
    bag<base64Binary>(
        base64Binary("0fa8"),
        base64Binary("0fa80f+9")
    ),
    bag<base64Binary>(
        base64Binary("0fa8"),
        base64Binary("0F+40A=="),
        base64Binary("0fa80f+9")
    )
)

//?
//? test: subset<base64Binary>-2
//? description: x
//? expect: boolean(false)
//? source: ...

subset<base64Binary>(
    bag<base64Binary>(
        base64Binary("0fa8"),
        base64Binary("0fa80f+9")
    ),
    bag<base64Binary>(
        base64Binary("0fa9"),
        base64Binary("0F+40A=="),
        base64Binary("0fa80f+9")
    )
)

//?
//? test: subset<base64Binary>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

subset<base64Binary>(
    bag<base64Binary>(
        base64Binary("x"),
        base64Binary("x")
    ),
    bag<base64Binary>(
        base64Binary("x"),
        base64Binary("x")
    )
)

