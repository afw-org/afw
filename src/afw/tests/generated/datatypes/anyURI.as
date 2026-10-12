#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: datatypes/anyURI.as
//? customPurpose: Part of core dataType tests
//? description: Tests for anyURI dataType.
//? sourceType: script
//?
//? test: anyURI-1
//? description: Test 1 for anyURI dataType.
//? expect: boolean(true)
//? source: ...

eq<anyURI>(
    "http://datypic.com"anyURI,
    "http://datypic.com"anyURI
)

//?
//? test: anyURI-2
//? description: Test 2 for anyURI dataType.
//? expect: boolean(true)
//? source: ...

eq<anyURI>(
    "mailto:info@datypic.com"anyURI,
    "mailto:info@datypic.com"anyURI
)

//?
//? test: anyURI-3
//? description: Test 3 for anyURI dataType.
//? expect: boolean(true)
//? source: ...

eq<anyURI>(
    "../%C3%A9dition.html"anyURI,
    "../%C3%A9dition.html"anyURI
)

//?
//? test: anyURI-4
//? description: Test 4 for anyURI dataType.
//? expect: boolean(true)
//? source: ...

eq<anyURI>(
    "../édition.html"anyURI,
    "../édition.html"anyURI
)

//?
//? test: anyURI-5
//? description: Test 5 for anyURI dataType.
//? expect: boolean(true)
//? source: ...

eq<anyURI>(
    "http://datypic.com/prod.html#shirt"anyURI,
    "http://datypic.com/prod.html#shirt"anyURI
)

//?
//? test: anyURI-6
//? description: Test 6 for anyURI dataType.
//? expect: boolean(true)
//? source: ...

eq<anyURI>(
    "../prod.html#shirt"anyURI,
    "../prod.html#shirt"anyURI
)

//?
//? test: anyURI-7
//? description: Test 7 for anyURI dataType.
//? expect: boolean(true)
//? source: ...

eq<anyURI>(
    "urn:example:org"anyURI,
    "urn:example:org"anyURI
)

