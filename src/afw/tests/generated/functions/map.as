#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/map.as
//? customPurpose: Part of core function tests
//? description: Tests for map function.
//? sourceType: script
//?
//? test: map-1
//? description: x
//? expect: ...

array<string>(
    string("hello"),
    string("world")
)

//? source: ...

map(
    array<string>(
        string("Hello"),
        string("World")
    ),
    string("normalize_to_lower_case<string>")
)

//?
//? test: map-2
//? description: x
//? expect: ...

array<integer>(
    52,
    15
)

//? source: ...

map(
    array<integer>(
        52,
        -15
    ),
    string("abs<integer>")
)

//?
//? test: map-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

map(
    string("x")
)

