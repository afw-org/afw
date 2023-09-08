#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/anyURI_to_string.as
//? customPurpose: Part of core function tests
//? description: Tests for to_string<anyURI> function.
//? sourceType: script
//?
//? test: to_string<anyURI>-1
//? description: test http
//? expect: string("http://datypic.com")
//? source: ...

to_string<anyURI>(
    anyURI("http://datypic.com")
)

//?
//? test: to_string<anyURI>-2
//? description: test mailto
//? expect: string("mailto:info@datypic.com")
//? source: ...

to_string<anyURI>(
    anyURI("mailto:info@datypic.com")
)

//?
//? test: to_string<anyURI>-3
//? description: test urn
//? expect: string("urn:example:org")
//? source: ...

to_string<anyURI>(
    anyURI("urn:example:org")
)

//?
//? test: to_string<anyURI>-4
//? skip: true
//? description: Invalid arguments test.
//? expect: error
//? source: ...

to_string<anyURI>(
    anyURI("x")
)

