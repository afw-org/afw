#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_to_anyURI.as
//? customPurpose: Part of core function tests
//? description: Tests for to_anyURI<string> function.
//? sourceType: script
//?
//? test: to_anyURI<string>-1
//? description: x
//? expect: "http://www.w3schools.com/images/smiley.gif"anyURI
//? source: ...

to_anyURI<string>(
    string("http://www.w3schools.com/images/smiley.gif")
)

//?
//? test: to_anyURI<string>-2
//? description: x
//? expect: "../%C3%A9dition.html"anyURI
//? source: ...

to_anyURI<string>(
    string("../%C3%A9dition.html")
)

//?
//? test: to_anyURI<string>-3
//? skip: true
//? description: Invalid arguments test.
//? expect: error
//? source: ...

to_anyURI<string>(
    string("x")
)

