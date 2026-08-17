#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/replace.as
//? customPurpose: Part of core function tests
//? description: Tests for replace function.
//? sourceType: script
//?
//? test: replace-1
//? description: Test replace function - limit not specified
//? expect: string("Hexlo World!")
//? source: ...

replace(
    string("Hello World!"),
    string("l"),
    string("x")
)

//?
//? test: replace-2
//? description: Test replace function - limit 2
//? expect: string("Hexxo World!")
//? source: ...

replace(
    string("Hello World!"),
    string("l"),
    string("x"),
    integer(2)
)

//?
//? test: replace-3
//? description: Test replace function - limit -1
//? expect: string("Hexxo Worxd!")
//? source: ...

replace(
    string("Hello World!"),
    string("l"),
    string("x"),
    integer(-1)
)

//?
//? test: replace-4
//? description: Test replace function - bigger replace
//? expect: string("Hello Earth!")
//? source: ...

replace(
    string("Hello World!"),
    string("World"),
    string("Earth")
)

//?
//? test: replace-5
//? description: Empty match default limit inserts once at the start
//? expect: string("xabc")
//? source: ...

replace(
    string("abc"),
    string(""),
    string("x")
)

//?
//? test: replace-6
//? description: Empty match limit -1 inserts at every code-point boundary
//? expect: string("xaxbxcx")
//? source: ...

replace(
    string("abc"),
    string(""),
    string("x"),
    integer(-1)
)

//?
//? test: replace-7
//? description: Empty value and empty match insert once
//? expect: string("x")
//? source: ...

replace(
    string(""),
    string(""),
    string("x")
)

