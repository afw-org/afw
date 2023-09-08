#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/anyURI_bag.as
//? customPurpose: Part of core function tests
//? description: Tests for bag<anyURI> function.
//? sourceType: script
//?
//? test: bag<anyURI>-1
//? description: x
//? expect: ...

bag<anyURI>(
    anyURI("http://www.w3schools.com/images/smiley.gif"),
    anyURI("http://datypic.com"),
    anyURI("../%C3%A9dition.html"),
    anyURI("../dition.html"),
    anyURI("http://datypic.com/prod.html#shirt"),
    anyURI("../prod.html#shirt"),
    anyURI("urn:example:org")
)

//? source: ...

bag<anyURI>(
    anyURI("http://www.w3schools.com/images/smiley.gif"),
    anyURI("http://datypic.com"),
    anyURI("../%C3%A9dition.html"),
    anyURI("../dition.html"),
    anyURI("http://datypic.com/prod.html#shirt"),
    anyURI("../prod.html#shirt"),
    anyURI("urn:example:org")
)

//?
//? test: bag<anyURI>-2
//? description: x
//? expect: ...

bag<anyURI>(
)

//? source: bag<anyURI>()
//?
//? test: bag<anyURI>-3
//? skip: true
//? description: Invalid arguments test.
//? expect: error
//? source: ...

bag<anyURI>(
    anyURI("x")
)

