#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_is_in.as
//? customPurpose: Part of core function tests
//? description: Tests for is_in<rfc822Name> function.
//? sourceType: expression
//?
//? test: is_in<rfc822Name>-1
//? description: x
//? expect: boolean(true)
//? source: ...

is_in<rfc822Name>(
    rfc822Name("localPart@DOMAINPart"),
    bag<rfc822Name>(
        rfc822Name("localPart@DOMAINPart"),
        rfc822Name("x")
    )
)

//?
//? test: is_in<rfc822Name>-2
//? description: x
//? expect: boolean(false)
//? source: ...

is_in<rfc822Name>(
    rfc822Name("LocalPart@DOMAINPart"),
    bag<rfc822Name>(
        rfc822Name("localPart@DOMAINPart"),
        rfc822Name("x")
    )
)

//?
//? test: is_in<rfc822Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

is_in<rfc822Name>(
    rfc822Name("x")
)

