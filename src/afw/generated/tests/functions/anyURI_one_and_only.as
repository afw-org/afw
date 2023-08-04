#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/anyURI_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<anyURI> function.
//? sourceType: expression
//?
//? test: one_and_only<anyURI>-1
//? description: x
//? expect: anyURI("http://datypic.com")
//? source: ...

one_and_only<anyURI>(
    bag<anyURI>(
        anyURI("http://datypic.com")
    )
)

//?
//? test: one_and_only<anyURI>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

one_and_only<anyURI>(
    bag<anyURI>(
        anyURI("http://datypic.com"),
        anyURI("http://datypic.coms")
    )
)

