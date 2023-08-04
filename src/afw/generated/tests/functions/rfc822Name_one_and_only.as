#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<rfc822Name> function.
//? sourceType: expression
//?
//? test: one_and_only<rfc822Name>-1
//? description: x
//? expect: rfc822Name("localPart@DOMAINPart")
//? source: ...

one_and_only<rfc822Name>(
    bag<rfc822Name>(
        rfc822Name("localPart@DOMAINPart")
    )
)

//?
//? test: one_and_only<rfc822Name>-2
//? description: x
//? expect: rfc822Name("localPart@DOMAINPart")
//? source: ...

one_and_only<rfc822Name>(
    bag<rfc822Name>(
        rfc822Name("localPart@DOMAINPart")
    )
)

//?
//? test: one_and_only<rfc822Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

one_and_only<rfc822Name>(
    bag<rfc822Name>(
        rfc822Name("x"),
        rfc822Name("x")
    )
)

