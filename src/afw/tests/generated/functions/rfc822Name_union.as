#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_union.as
//? customPurpose: Part of core function tests
//? description: Tests for union<rfc822Name> function.
//? sourceType: script
//?
//? test: union<rfc822Name>-1
//? description: x
//? expect: ...

bag<rfc822Name>(
    rfc822Name("localPart@DOMAINPart"),
    rfc822Name("localPart@DOMAINPart2")
)

//? source: ...

union<rfc822Name>(
    bag<rfc822Name>(
        rfc822Name("localPart@DOMAINPart"),
        rfc822Name("localPart@DOMAINPart")
    ),
    bag<rfc822Name>(
        rfc822Name("localPart@DOMAINPart2"),
        rfc822Name("localPart@DOMAINPart")
    )
)

//?
//? test: union<rfc822Name>-2
//? description: x
//? expect: ...

bag<rfc822Name>(
    rfc822Name("localPart@DOMAINPart"),
    rfc822Name("localPart@DOMAINPart2"),
    rfc822Name("localPart@DOMAINPart3")
)

//? source: ...

union<rfc822Name>(
    bag<rfc822Name>(
        rfc822Name("localPart@DOMAINPart"),
        rfc822Name("localPart@DOMAINPart2")
    ),
    bag<rfc822Name>(
        rfc822Name("localPart@DOMAINPart2"),
        rfc822Name("localPart@DOMAINPart3")
    )
)

//?
//? test: union<rfc822Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: union<rfc822Name>()
