#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_bag.as
//? customPurpose: Part of core function tests
//? description: Tests for bag<rfc822Name> function.
//? sourceType: expression
//?
//? test: bag<rfc822Name>-1
//? description: x
//? expect: ...

bag<rfc822Name>(
    rfc822Name("LOCALPart@DomainPart"),
    rfc822Name("localPart@DOMAINPart"),
    rfc822Name("differentlocalPart@DOMAINPart"),
    rfc822Name("localPart@differentDOMAINPart")
)

//? source: ...

bag<rfc822Name>(
    rfc822Name("LOCALPart@DomainPart"),
    rfc822Name("localPart@DOMAINPart"),
    rfc822Name("differentlocalPart@DOMAINPart"),
    rfc822Name("localPart@differentDOMAINPart")
)

//?
//? test: bag<rfc822Name>-2
//? skip: true
//? description: Invalid arguments test.
//? expect: error
//? source: bag<rfc822Name>()
