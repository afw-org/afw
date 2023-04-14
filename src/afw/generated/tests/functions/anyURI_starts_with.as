#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/anyURI_starts_with.as
//? customPurpose: Part of core function tests
//? description: Tests for starts_with<anyURI> function.
//? sourceType: expression
//?
//? test: starts_with<anyURI>-1
//? description: x
//? expect: boolean(true)
//? source: ...

starts_with<anyURI>(
    anyURI("http://www.w3schools.com/images/smiley.gif"),
    string("http://www.w3")
)

//?
//? test: starts_with<anyURI>-2
//? description: x
//? expect: boolean(false)
//? source: ...

starts_with<anyURI>(
    anyURI("http://www.w3schools.com/images/smiley.gif"),
    string("http://ww.w3")
)

//?
//? test: starts_with<anyURI>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

starts_with<anyURI>(
    anyURI("x")
)

