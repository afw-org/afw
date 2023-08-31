#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/anyURI_ends_with.as
//? customPurpose: Part of core function tests
//? description: Tests for ends_with<anyURI> function.
//? sourceType: expression
//?
//? test: ends_with<anyURI>-1
//? description: x
//? expect: boolean(true)
//? source: ...

ends_with<anyURI>(
    anyURI("http://datypic.com/prod.html#shirt"),
    string("shirt")
)

//?
//? test: ends_with<anyURI>-2
//? description: x
//? expect: boolean(false)
//? source: ...

ends_with<anyURI>(
    anyURI("http://datypic.com/prod.html#shirt"),
    string("shir")
)

//?
//? test: ends_with<anyURI>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

ends_with<anyURI>(
    anyURI("x")
)

