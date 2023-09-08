#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/anyURI_subset.as
//? customPurpose: Part of core function tests
//? description: Tests for subset<anyURI> function.
//? sourceType: script
//?
//? test: subset<anyURI>-1
//? description: x
//? expect: boolean(true)
//? source: ...

subset<anyURI>(
    bag<anyURI>(
        anyURI("urn:example:org")
    ),
    bag<anyURI>(
        anyURI("http://datypic.com/prod.html#shirt"),
        anyURI("../prod.html#shirt"),
        anyURI("urn:example:org")
    )
)

//?
//? test: subset<anyURI>-2
//? description: x
//? expect: boolean(false)
//? source: ...

subset<anyURI>(
    bag<anyURI>(
        anyURI("../prod.html#shirt"),
        anyURI("urn:example:org")
    ),
    bag<anyURI>(
        anyURI("http://datypic.com/prod.html#shirt"),
        anyURI("../prod.html#shirt")
    )
)

//?
//? test: subset<anyURI>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

subset<anyURI>(
    bag<anyURI>(
        anyURI("x"),
        anyURI("x")
    )
)

