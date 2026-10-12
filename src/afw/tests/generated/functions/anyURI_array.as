#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/anyURI_array.as
//? customPurpose: Part of core function tests
//? description: Tests for array<anyURI> function.
//? sourceType: script
//?
//? test: array<anyURI>-1
//? description: x
//? expect: ...

array<anyURI>(
    "http://www.w3schools.com/images/smiley.gif"anyURI,
    "http://datypic.com"anyURI,
    "../%C3%A9dition.html"anyURI,
    "../dition.html"anyURI,
    "http://datypic.com/prod.html#shirt"anyURI,
    "../prod.html#shirt"anyURI,
    "urn:example:org"anyURI
)

//? source: ...

array<anyURI>(
    "http://www.w3schools.com/images/smiley.gif"anyURI,
    "http://datypic.com"anyURI,
    "../%C3%A9dition.html"anyURI,
    "../dition.html"anyURI,
    "http://datypic.com/prod.html#shirt"anyURI,
    "../prod.html#shirt"anyURI,
    "urn:example:org"anyURI
)

//?
//? test: array<anyURI>-2
//? description: x
//? expect: ...

array<anyURI>(
)

//? source: array<anyURI>()
//?
//? test: array<anyURI>-3
//? skip: true
//? description: Invalid arguments test.
//? expect: error
//? source: ...

array<anyURI>(
    "x"anyURI
)

