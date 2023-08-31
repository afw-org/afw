#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: datatypes/anyURI.as
//? customPurpose: Part of core dataType tests
//? description: Tests for anyURI dataType.
//? sourceType: expression
//?
//? test: anyURI-1
//? description: Test 1 for anyURI dataType.
//? expect: boolean(true)
//? source: ...

eq<anyURI>(
    anyURI("http://datypic.com"),
    anyURI("http://datypic.com")
)

//?
//? test: anyURI-2
//? description: Test 2 for anyURI dataType.
//? expect: boolean(true)
//? source: ...

eq<anyURI>(
    anyURI("mailto:info@datypic.com"),
    anyURI("mailto:info@datypic.com")
)

//?
//? test: anyURI-3
//? description: Test 3 for anyURI dataType.
//? expect: boolean(true)
//? source: ...

eq<anyURI>(
    anyURI("../%C3%A9dition.html"),
    anyURI("../%C3%A9dition.html")
)

//?
//? test: anyURI-4
//? description: Test 4 for anyURI dataType.
//? expect: boolean(true)
//? source: ...

eq<anyURI>(
    anyURI("../édition.html"),
    anyURI("../édition.html")
)

//?
//? test: anyURI-5
//? description: Test 5 for anyURI dataType.
//? expect: boolean(true)
//? source: ...

eq<anyURI>(
    anyURI("http://datypic.com/prod.html#shirt"),
    anyURI("http://datypic.com/prod.html#shirt")
)

//?
//? test: anyURI-6
//? description: Test 6 for anyURI dataType.
//? expect: boolean(true)
//? source: ...

eq<anyURI>(
    anyURI("../prod.html#shirt"),
    anyURI("../prod.html#shirt")
)

//?
//? test: anyURI-7
//? description: Test 7 for anyURI dataType.
//? expect: boolean(true)
//? source: ...

eq<anyURI>(
    anyURI("urn:example:org"),
    anyURI("urn:example:org")
)

