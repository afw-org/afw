#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_to_anyURI.as
//? customPurpose: Part of core function tests
//? description: Tests for to_anyURI<string> function.
//? sourceType: expression
//?
//? test: to_anyURI<string>-1
//? description: x
//? expect: anyURI("http://www.w3schools.com/images/smiley.gif")
//? source: ...

to_anyURI<string>(
    string("http://www.w3schools.com/images/smiley.gif")
)

//?
//? test: to_anyURI<string>-2
//? description: x
//? expect: anyURI("../%C3%A9dition.html")
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

