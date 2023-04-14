#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/anyURI_intersection.as
//? customPurpose: Part of core function tests
//? description: Tests for intersection<anyURI> function.
//? sourceType: expression
//?
//? test: intersection<anyURI>-1
//? description: x
//? expect: ...

bag<anyURI>(
    anyURI("http://datypic.com/prod.html#shirt"),
    anyURI("../prod.html#shirt"),
    anyURI("urn:example:org")
)

//? source: ...

intersection<anyURI>(
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
        anyURI("http://www.w3schools.com/images/smiley.gifx"),
        anyURI("http://datypic.comx"),
        anyURI("../%C3%A9dition.htmlx"),
        anyURI("../dition.htmlx"),
        anyURI("http://datypic.com/prod.html#shirt"),
        anyURI("../prod.html#shirt"),
        anyURI("urn:example:org")
    )
)

//?
//? test: intersection<anyURI>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

intersection<anyURI>(
    bag<anyURI>(
        anyURI("x"),
        anyURI("x")
    )
)

