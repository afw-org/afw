#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_eq.as
//? customPurpose: Part of core function tests
//? description: Tests for eq<rfc822Name> function.
//? sourceType: script
//?
//? test: eq<rfc822Name>-1
//? description: x
//? expect: boolean(false)
//? source: ...

eq<rfc822Name>(
    "localPart@DOMAINPart"rfc822Name,
    "localPart@dOMAINPart"rfc822Name
)

//?
//? test: eq<rfc822Name>-2
//? description: x
//? expect: boolean(true)
//? source: ...

eq<rfc822Name>(
    "localPart@DOMAINPart"rfc822Name,
    "localPart@DOMAINPart"rfc822Name
)

//?
//? test: eq<rfc822Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

eq<rfc822Name>(
    "x"rfc822Name
)

