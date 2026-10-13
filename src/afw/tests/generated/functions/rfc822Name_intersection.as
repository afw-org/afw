#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_intersection.as
//? customPurpose: Part of core function tests
//? description: Tests for intersection<rfc822Name> function.
//? sourceType: script
//?
//? test: intersection<rfc822Name>-1
//? description: x
//? expect: ...

array<rfc822Name>(
)

//? source: ...

intersection<rfc822Name>(
    array<rfc822Name>(
        "localPart@DOMAINPart"rfc822Name,
        "localPart@DOMAINPart2"rfc822Name
    ),
    array<rfc822Name>(
        "localPart@DOMAINPart3"rfc822Name,
        "localPart@DOMAINPart4"rfc822Name
    )
)

//?
//? test: intersection<rfc822Name>-2
//? description: x
//? expect: ...

array<rfc822Name>(
    "localPart@DOMAINPart2"rfc822Name
)

//? source: ...

intersection<rfc822Name>(
    array<rfc822Name>(
        "localPart@DOMAINPart"rfc822Name,
        "localPart@DOMAINPart2"rfc822Name
    ),
    array<rfc822Name>(
        "localPart@DOMAINPart2"rfc822Name,
        "localPart@DOMAINPart4"rfc822Name
    )
)

//?
//? test: intersection<rfc822Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

intersection<rfc822Name>(
    array<rfc822Name>(
        "x@y"rfc822Name,
        "x@y"rfc822Name
    )
)

