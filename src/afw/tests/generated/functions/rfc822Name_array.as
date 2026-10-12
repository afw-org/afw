#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_array.as
//? customPurpose: Part of core function tests
//? description: Tests for array<rfc822Name> function.
//? sourceType: script
//?
//? test: array<rfc822Name>-1
//? description: x
//? expect: ...

array<rfc822Name>(
    "LOCALPart@DomainPart"rfc822Name,
    "localPart@DOMAINPart"rfc822Name,
    "differentlocalPart@DOMAINPart"rfc822Name,
    "localPart@differentDOMAINPart"rfc822Name
)

//? source: ...

array<rfc822Name>(
    "LOCALPart@DomainPart"rfc822Name,
    "localPart@DOMAINPart"rfc822Name,
    "differentlocalPart@DOMAINPart"rfc822Name,
    "localPart@differentDOMAINPart"rfc822Name
)

//?
//? test: array<rfc822Name>-2
//? skip: true
//? description: Invalid arguments test.
//? expect: error
//? source: array<rfc822Name>()
