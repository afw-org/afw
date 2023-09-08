#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/split.as
//? customPurpose: Part of core function tests
//? description: Tests for split function.
//? sourceType: script
//?
//? test: split-1
//? description: Test split function with no optional parameters
//? expect: ...

bag<string>(
    string("H"),
    string("e"),
    string("l"),
    string("l"),
    string("o"),
    string(" "),
    string("W"),
    string("o"),
    string("r"),
    string("l"),
    string("d"),
    string("!")
)

//? source: ...

split(
    string("Hello World!")
)

//?
//? test: split-2
//? description: Test split function with limit of 5
//? expect: ...

bag<string>(
    string("H"),
    string("e"),
    string("l"),
    string("l"),
    string("o")
)

//? source: ...

split(
    string("Hello World!"),
    "",
    integer(5)
)

//?
//? test: split-3
//? description: Test split function with l as separator
//? expect: ...

bag<string>(
    string("He"),
    "",
    string("o Wor"),
    string("d!")
)

//? source: ...

split(
    string("Hello World!"),
    string("l")
)

//?
//? test: split-4
//? description: Test split function with World as separator
//? expect: ...

bag<string>(
    string("Hello "),
    string("!")
)

//? source: ...

split(
    string("Hello World!"),
    string("World")
)

