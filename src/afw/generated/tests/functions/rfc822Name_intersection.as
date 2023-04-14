#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_intersection.as
//? customPurpose: Part of core function tests
//? description: Tests for intersection<rfc822Name> function.
//? sourceType: expression
//?
//? test: intersection<rfc822Name>-1
//? description: x
//? expect: ...

bag<rfc822Name>(
)

//? source: ...

intersection<rfc822Name>(
    bag<rfc822Name>(
        rfc822Name("localPart@DOMAINPart"),
        rfc822Name("localPart@DOMAINPart2")
    ),
    bag<rfc822Name>(
        rfc822Name("localPart@DOMAINPart3"),
        rfc822Name("localPart@DOMAINPart4")
    )
)

//?
//? test: intersection<rfc822Name>-2
//? description: x
//? expect: ...

bag<rfc822Name>(
    rfc822Name("localPart@DOMAINPart2")
)

//? source: ...

intersection<rfc822Name>(
    bag<rfc822Name>(
        rfc822Name("localPart@DOMAINPart"),
        rfc822Name("localPart@DOMAINPart2")
    ),
    bag<rfc822Name>(
        rfc822Name("localPart@DOMAINPart2"),
        rfc822Name("localPart@DOMAINPart4")
    )
)

//?
//? test: intersection<rfc822Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

intersection<rfc822Name>(
    bag<rfc822Name>(
        rfc822Name("x"),
        rfc822Name("x")
    )
)

