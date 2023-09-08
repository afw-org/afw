#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/anyURI_set_equals.as
//? customPurpose: Part of core function tests
//? description: Tests for set_equals<anyURI> function.
//? sourceType: script
//?
//? test: set_equals<anyURI>-1
//? description: x
//? expect: boolean(true)
//? source: ...

set_equals<anyURI>(
    bag<anyURI>(
        anyURI("http://www.w3schools.com/images/smiley.gif"),
        anyURI("http://datypic.com"),
        anyURI("../%C3%A9dition.html"),
        anyURI("../dition.html"),
        anyURI("http://datypic.com/prod.html#shirt"),
        anyURI("../prod.html#shirt"),
        anyURI("urn:example:org")
    ),
    bag<anyURI>(
        anyURI("http://www.w3schools.com/images/smiley.gif"),
        anyURI("http://datypic.com"),
        anyURI("../%C3%A9dition.html"),
        anyURI("../dition.html"),
        anyURI("http://datypic.com/prod.html#shirt"),
        anyURI("../prod.html#shirt"),
        anyURI("urn:example:org")
    )
)

//?
//? test: set_equals<anyURI>-2
//? description: duplicates
//? expect: boolean(true)
//? source: ...

set_equals<anyURI>(
    bag<anyURI>(
        anyURI("http://www.w3schools.com/images/smiley.gif"),
        anyURI("http://datypic.com"),
        anyURI("../%C3%A9dition.html"),
        anyURI("../dition.html"),
        anyURI("http://datypic.com/prod.html#shirt"),
        anyURI("../prod.html#shirt"),
        anyURI("urn:example:org"),
        anyURI("urn:example:org"),
        anyURI("urn:example:org")
    ),
    bag<anyURI>(
        anyURI("http://www.w3schools.com/images/smiley.gif"),
        anyURI("http://datypic.com"),
        anyURI("../%C3%A9dition.html"),
        anyURI("../dition.html"),
        anyURI("http://datypic.com/prod.html#shirt"),
        anyURI("../prod.html#shirt"),
        anyURI("urn:example:org")
    )
)

//?
//? test: set_equals<anyURI>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

set_equals<anyURI>(
    bag<anyURI>(
        anyURI("x"),
        anyURI("x")
    )
)

