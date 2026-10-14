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
    2
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
    -1
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
    -1
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


//?
//? test: replace-regexp
//? description: A regexp search is a pattern; the default limit replaces once, -1 every match.
//? expect: ["a-b.c", "a-b-c"]
//? source: ...

["a.b.c"->replace(/\./, "-"), replace("a.b.c", /\./, "-", -1)]

//?
//? test: replace-regexp-groups
//? description: With a regexp, $n and ${name} in the replacement are the text of a group.
//? expect: ["10/2024", "10/2024"]
//? source: ...

[
    replace("2024-10", /(\d+)-(\d+)/, "$2/$1"),
    replace("2024-10", /(?<y>\d+)-(?<m>\d+)/, "${m}/${y}")
]

//?
//? test: replace-string-literal
//? description: With a string search, the search and the replacement are literal text.
//? expect: ["a$1b", "a.b"]
//? source: ...

[replace("a.b", ".", "$1"), replace("a.b", "\\.", "-")]

//?
//? test: replace-regexp-dollar
//? description: With a regexp, a $ that is not a group is an error; write \$.
//? expect: error:In the replacement, $ must be followed by a group number or {name}; write \$ for a $
//? source: ...

replace("a", /a/, "$")

//?
//? test: replace-other-search
//? description: The search must be a string or a regexp.
//? expect: error:Parameter 2 of function 'replace<string>' must evaluate to data type 'string' or 'regexp' but evaluated to be 'integer'
//? source: ...

replace("a", 1, "x")
