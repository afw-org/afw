#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/anyURI_eq.as
//? customPurpose: Part of core function tests
//? description: Tests for eq<anyURI> function.
//? sourceType: script
//?
//? test: eq<anyURI>-1
//? description: Compare two identical URIs
//? expect: boolean(true)
//? source: ...

eq<anyURI>(
    anyURI("http://datypic.com/prod.html#shirt"),
    anyURI("http://datypic.com/prod.html#shirt")
)

//?
//? test: eq<anyURI>-2
//? description: Compare two different URIs
//? expect: boolean(false)
//? source: ...

eq<anyURI>(
    anyURI("http://datypic.com/prod.html#shirt"),
    anyURI("https://datypic.com/prod.html#shirt")
)

//?
//? test: eq<anyURI>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

eq<anyURI>(
    anyURI("x")
)

