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
    "localPart@DOMAINPart"rfc822Name,
    "localPart@DOMAINPart2"rfc822Name
)

//? source: ...

union<rfc822Name>(
    bag<rfc822Name>(
        "localPart@DOMAINPart"rfc822Name,
        "localPart@DOMAINPart"rfc822Name
    ),
    bag<rfc822Name>(
        "localPart@DOMAINPart2"rfc822Name,
        "localPart@DOMAINPart"rfc822Name
    )
)

//?
//? test: union<rfc822Name>-2
//? description: x
//? expect: ...

bag<rfc822Name>(
    "localPart@DOMAINPart"rfc822Name,
    "localPart@DOMAINPart2"rfc822Name,
    "localPart@DOMAINPart3"rfc822Name
)

//? source: ...

union<rfc822Name>(
    bag<rfc822Name>(
        "localPart@DOMAINPart"rfc822Name,
        "localPart@DOMAINPart2"rfc822Name
    ),
    bag<rfc822Name>(
        "localPart@DOMAINPart2"rfc822Name,
        "localPart@DOMAINPart3"rfc822Name
    )
)

//?
//? test: union<rfc822Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: union<rfc822Name>()
