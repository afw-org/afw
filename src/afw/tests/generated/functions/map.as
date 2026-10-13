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
    string("normalize_to_lower_case<string>"),
    array<string>(
        string("Hello"),
        string("World")
    )
)

//?
//? test: map-2
//? description: x
//? expect: ...

array<integer>(
    62,
    -5
)

//? source: ...

map(
    string("add"),
    array<integer>(
        52,
        -15
    ),
    10
)

//?
//? test: map-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

map(
    string("x")
)

