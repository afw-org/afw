#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_at_least_one_member_of.as
//? customPurpose: Part of core function tests
//? description: Tests for at_least_one_member_of<rfc822Name> function.
//? sourceType: expression
//?
//? test: at_least_one_member_of<rfc822Name>-1
//? description: x
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<rfc822Name>(
    bag<rfc822Name>(
        rfc822Name("LOCALPart@DomainPart"),
        rfc822Name("localPart@DOMAINPart"),
        rfc822Name("differentlocalPart@DOMAINPart"),
        rfc822Name("localPart@differentDOMAINPart")
    ),
    bag<rfc822Name>(
        rfc822Name("xLOCALPart@DomainPart"),
        rfc822Name("xlocalPart@DOMAINPart"),
        rfc822Name("xdifferentlocalPart@DOMAINPart"),
        rfc822Name("localPart@differentDOMAINPart")
    )
)

//?
//? test: at_least_one_member_of<rfc822Name>-2
//? description: x
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<rfc822Name>(
    bag<rfc822Name>(
        rfc822Name("LOCALPart@DomainPart"),
        rfc822Name("localPart@DOMAINPart"),
        rfc822Name("differentlocalPart@DOMAINPart"),
        rfc822Name("localPart@differentDOMAINPart")
    ),
    bag<rfc822Name>(
        rfc822Name("xLOCALPart@DomainPart"),
        rfc822Name("xlocalPart@DOMAINPart"),
        rfc822Name("xdifferentlocalPart@DOMAINPart"),
        rfc822Name("xlocalPart@differentDOMAINPart")
    )
)

//?
//? test: at_least_one_member_of<rfc822Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

at_least_one_member_of<rfc822Name>(
    bag<rfc822Name>(
        rfc822Name("x"),
        rfc822Name("x")
    )
)

