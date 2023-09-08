#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/base64Binary_union.as
//? customPurpose: Part of core function tests
//? description: Tests for union<base64Binary> function.
//? sourceType: script
//?
//? test: union<base64Binary>-1
//? description: x
//? expect: ...

bag<base64Binary>(
    base64Binary("0F+40A=="),
    base64Binary("0fa8"),
    base64Binary("0fa80f+9")
)

//? source: ...

union<base64Binary>(
    bag<base64Binary>(
        base64Binary("0F+40A==")
    ),
    bag<base64Binary>(
        base64Binary("0fa8"),
        base64Binary("0F+40A=="),
        base64Binary("0fa80f+9")
    )
)

//?
//? test: union<base64Binary>-2
//? description: x
//? expect: ...

bag<base64Binary>(
    base64Binary("0F+40A=="),
    base64Binary("0fa8"),
    base64Binary("0fa80f+9")
)

//? source: ...

union<base64Binary>(
    bag<base64Binary>(
        base64Binary("0F+40A==")
    ),
    bag<base64Binary>(
        base64Binary("0fa8"),
        base64Binary("0F+40A=="),
        base64Binary("0fa80f+9")
    )
)

//?
//? test: union<base64Binary>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

union<base64Binary>(
    bag<base64Binary>(
        base64Binary("x"),
        base64Binary("x")
    ),
    bag<base64Binary>(
        base64Binary("x"),
        base64Binary("x")
    )
)

