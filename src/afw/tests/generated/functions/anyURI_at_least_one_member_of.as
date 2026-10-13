#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/anyURI_at_least_one_member_of.as
//? customPurpose: Part of core function tests
//? description: Tests for at_least_one_member_of<anyURI> function.
//? sourceType: script
//?
//? test: at_least_one_member_of<anyURI>-1
//? description: x
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<anyURI>(
    array<anyURI>(
        "http://www.w3schools.com/images/smiley.gif"anyURI,
        "http://datypic.com"anyURI,
        "../%C3%A9dition.html"anyURI,
        "../dition.html"anyURI,
        "http://datypic.com/prod.html#shirt"anyURI,
        "../prod.html#shirt"anyURI,
        "urn:example:org"anyURI
    ),
    array<anyURI>(
        "../dition.html"anyURI
    )
)

//?
//? test: at_least_one_member_of<anyURI>-2
//? description: x
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<anyURI>(
    array<anyURI>(
        "http://www.w3schools.com/images/smiley.gif"anyURI,
        "http://datypic.com"anyURI,
        "../%C3%A9dition.html"anyURI,
        "../dition.html"anyURI,
        "http://datypic.com/prod.html#shirt"anyURI,
        "../prod.html#shirt"anyURI,
        "urn:example:org"anyURI
    ),
    array<anyURI>(
        "../ditionxx.html"anyURI
    )
)

//?
//? test: at_least_one_member_of<anyURI>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

at_least_one_member_of<anyURI>(
    array<anyURI>(
        "x"anyURI,
        "x"anyURI
    )
)

