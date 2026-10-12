#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/anyURI_is_in.as
//? customPurpose: Part of core function tests
//? description: Tests for is_in<anyURI> function.
//? sourceType: script
//?
//? test: is_in<anyURI>-1
//? description: x
//? expect: boolean(true)
//? source: ...

is_in<anyURI>(
    "urn:example:org"anyURI,
    bag<anyURI>(
        "http://www.w3schools.com/images/smiley.gif"anyURI,
        "http://datypic.com"anyURI,
        "../%C3%A9dition.html"anyURI,
        "../dition.html"anyURI,
        "http://datypic.com/prod.html#shirt"anyURI,
        "../prod.html#shirt"anyURI,
        "urn:example:org"anyURI
    )
)

//?
//? test: is_in<anyURI>-2
//? description: x
//? expect: boolean(false)
//? source: ...

is_in<anyURI>(
    "../edition.html"anyURI,
    bag<anyURI>(
        "http://www.w3schools.com/images/smiley.gif"anyURI,
        "http://datypic.com"anyURI,
        "../%C3%A9dition.html"anyURI,
        "../dition.html"anyURI,
        "http://datypic.com/prod.html#shirt"anyURI,
        "../prod.html#shirt"anyURI,
        "urn:example:org"anyURI
    )
)

//?
//? test: is_in<anyURI>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

is_in<anyURI>(
    "x"anyURI
)

