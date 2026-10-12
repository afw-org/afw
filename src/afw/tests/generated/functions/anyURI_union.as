#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/anyURI_union.as
//? customPurpose: Part of core function tests
//? description: Tests for union<anyURI> function.
//? sourceType: script
//?
//? test: union<anyURI>-1
//? description: Union set of 4 with set of 3.
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

union<anyURI>(
    array<anyURI>(
        "http://www.w3schools.com/images/smiley.gif"anyURI,
        "http://datypic.com"anyURI,
        "../%C3%A9dition.html"anyURI,
        "../dition.html"anyURI
    ),
    array<anyURI>(
        "http://datypic.com/prod.html#shirt"anyURI,
        "../prod.html#shirt"anyURI,
        "urn:example:org"anyURI
    )
)

//?
//? test: union<anyURI>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

union<anyURI>(
    array<anyURI>(
        "x"anyURI,
        "x"anyURI
    )
)

