#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/base64Binary_intersection.as
//? customPurpose: Part of core function tests
//? description: Tests for intersection<base64Binary> function.
//? sourceType: script
//?
//? test: intersection<base64Binary>-1
//? description: x
//? expect: ...

bag<base64Binary>(
)

//? source: ...

intersection<base64Binary>(
    bag<base64Binary>(
        base64Binary("0fa8"),
        base64Binary("0fa80f+9")
    ),
    bag<base64Binary>(
        base64Binary("0F+40A==")
    )
)

//?
//? test: intersection<base64Binary>-2
//? description: x
//? expect: ...

bag<base64Binary>(
    base64Binary("0F+40A==")
)

//? source: ...

intersection<base64Binary>(
    bag<base64Binary>(
        base64Binary("0fa8"),
        base64Binary("0fa80f+9"),
        base64Binary("0F+40A==")
    ),
    bag<base64Binary>(
        base64Binary("0F+40A==")
    )
)

//?
//? test: intersection<base64Binary>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

intersection<base64Binary>(
    bag<base64Binary>(
        base64Binary("x"),
        base64Binary("x")
    ),
    bag<base64Binary>(
        base64Binary("x"),
        base64Binary("x")
    )
)

