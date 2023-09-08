#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/anyURI_includes.as
//? customPurpose: Part of core function tests
//? description: Tests for includes<anyURI> function.
//? sourceType: script
//?
//? test: includes<anyURI>-1
//? description: x
//? expect: boolean(true)
//? source: ...

includes<anyURI>(
    anyURI("http://datypic.com/prod.html#shirt"),
    string("http://datypic.com/prod.html#shirt")
)

//?
//? test: includes<anyURI>-2
//? description: x
//? expect: boolean(false)
//? source: ...

includes<anyURI>(
    anyURI("http://datypic.com/prod.html#shirt"),
    string("http://datypiccom/prod.html#shirt")
)

//?
//? test: includes<anyURI>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

includes<anyURI>(
    anyURI("x")
)

