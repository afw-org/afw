#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_set_equals.as
//? customPurpose: Part of core function tests
//? description: Tests for set_equals<rfc822Name> function.
//? sourceType: script
//?
//? test: set_equals<rfc822Name>-1
//? description: x
//? expect: boolean(true)
//? source: ...

set_equals<rfc822Name>(
    bag<rfc822Name>(
        rfc822Name("localPart@DOMAINPart"),
        rfc822Name("localPart@DOMAINPart2")
    ),
    bag<rfc822Name>(
        rfc822Name("localPart@DOMAINPart"),
        rfc822Name("localPart@DOMAINPart2")
    )
)

//?
//? test: set_equals<rfc822Name>-2
//? description: x
//? expect: boolean(false)
//? source: ...

set_equals<rfc822Name>(
    bag<rfc822Name>(
        rfc822Name("localPart@DOMAINPart"),
        rfc822Name("localPart@DOMAINPart2")
    ),
    bag<rfc822Name>(
        rfc822Name("localPart@DOMAINPart"),
        rfc822Name("localPart@DOMAINParT2")
    )
)

//?
//? test: set_equals<rfc822Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

set_equals<rfc822Name>(
    bag<rfc822Name>(
        rfc822Name("x"),
        rfc822Name("x")
    )
)

