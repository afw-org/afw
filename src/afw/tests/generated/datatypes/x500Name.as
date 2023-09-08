#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: datatypes/x500Name.as
//? customPurpose: Part of core dataType tests
//? description: Tests for x500Name dataType.
//? sourceType: script
//?
//? test: x500Name-1
//? description: Test 1 for x500Name dataType.
//? expect: boolean(true)
//? source: ...

eq<x500Name>(
    x500Name("CN=Steve Kille,O=Isode Limited,C=GB"),
    x500Name("CN=Steve Kille,O=Isode Limited,C=GB")
)

//?
//? test: x500Name-2
//? description: Test 2 for x500Name dataType.
//? expect: boolean(true)
//? source: ...

eq<x500Name>(
    x500Name("OU=Sales+CN=J. Smith,O=Widget Inc.,C=US"),
    x500Name("OU=Sales+CN=J. Smith,O=Widget Inc.,C=US")
)

//?
//? test: x500Name-3
//? description: Test 3 for x500Name dataType.
//? expect: boolean(true)
//? source: ...

eq<x500Name>(
    x500Name("CN=L. Eagle,O=Sue\\, Grabbit and Runn,C=GB"),
    x500Name("CN=L. Eagle,O=Sue\\, Grabbit and Runn,C=GB")
)

//?
//? test: x500Name-4
//? description: Test 4 for x500Name dataType.
//? expect: boolean(true)
//? source: ...

eq<x500Name>(
    x500Name("CN=Before\\0DAfter,O=Test,C=GB"),
    x500Name("CN=Before\\0DAfter,O=Test,C=GB")
)

//?
//? test: x500Name-5
//? description: Test 5 for x500Name dataType.
//? expect: boolean(true)
//? source: ...

eq<x500Name>(
    x500Name("1.3.6.1.4.1.1466.0=#04024869,O=Test,C=GB"),
    x500Name("1.3.6.1.4.1.1466.0=#04024869,O=Test,C=GB")
)

//?
//? test: x500Name-6
//? description: Test 6 for x500Name dataType.
//? expect: boolean(true)
//? source: ...

eq<x500Name>(
    x500Name("SN=Lu\\C4\\8Di\\C4\\87"),
    x500Name("SN=Lu\\C4\\8Di\\C4\\87")
)

