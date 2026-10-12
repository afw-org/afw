#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/anyURI_substring.as
//? customPurpose: Part of core function tests
//? description: Tests for substring<anyURI> function.
//? sourceType: script
//?
//? test: substring<anyURI>-1
//? description: string substring test http://www.clemson.edu 0 5
//? expect: string("http:")
//? source: ...

substring<anyURI>(
    "http://www.clemson.edu"anyURI,
    0,
    5
)

//?
//? test: substring<anyURI>-2
//? description: string substring test http://www.clemson.edu 11 18
//? expect: string("clemson")
//? source: ...

substring<anyURI>(
    "http://www.clemson.edu"anyURI,
    11,
    18
)

//?
//? test: substring<anyURI>-3
//? description: string substring test http://www.clemson.edu 18 -1
//? expect: string("edu")
//? source: ...

substring<anyURI>(
    "http://www.clemson.edu"anyURI,
    19,
    -1
)

