#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/anyURI_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<anyURI> function.
//? sourceType: script
//?
//? test: bag_size<anyURI>-1
//? description: x
//? expect: integer(7)
//? source: ...

bag_size<anyURI>(
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
//? test: bag_size<anyURI>-2
//? description: Empty
//? expect: integer(0)
//? source: ...

bag_size<anyURI>(
    bag<anyURI>(
    )
)

//?
//? test: bag_size<anyURI>-3
//? description: Invalid arguments test.
//? expect: error
//? source: bag_size<anyURI>()
