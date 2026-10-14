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

array<string>(
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

array<string>(
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
    5
)

//?
//? test: split-3
//? description: Test split function with l as separator
//? expect: ...

array<string>(
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

array<string>(
    string("Hello "),
    string("!")
)

//? source: ...

split(
    string("Hello World!"),
    string("World")
)


//?
//? test: split-regexp
//? description: A regexp separator is a pattern.
//? expect: ["a", "b", "c"]
//? source: ...

"a, b ,c"->split(/\s*,\s*/)

//?
//? test: split-regexp-groups
//? description: The text of each group is put after the piece before the match, undefined when the group did not take part, as in ECMAScript.
//? expect: [["a", "1", "b", "2", "c"], ["a", undefined, "b"]]
//? source: ...

[split("a1b22c", /(\d)+/), split("a1b", /(x)?\d/)]

//?
//? test: split-regexp-empty-match
//? description: An empty match splits between code points, but not at the start or the end.
//? expect: [["a", "Ä", "c"], ["a", "b"]]
//? source: ...

[split("aÄc", /(?:)/), split("ab", /x*/)]

//?
//? test: split-regexp-empty-value
//? description: An empty value is [] if the regexp matches it, else [""].
//? expect: [[], [""]]
//? source: ...

[split("", /(?:)/), split("", /x/)]

//?
//? test: split-regexp-limit
//? description: limit is the most entries in the result, groups included; a negative limit is no limit, as in ECMAScript.
//? expect: [["a"], ["a", "1"], [], ["a", "b", "c"], ["a", "b"]]
//? source: ...

[
    split("a1b2c", /\d/, 1),
    split("a1b2c", /(\d)/, 2),
    split("a1b2c", /\d/, 0),
    split("a1b2c", /\d/, -1),
    split("a,b", ",", -1)
]

//?
//? test: split-regexp-string-is-literal
//? description: A string separator is literal text, not a pattern.
//? expect: [["a", "b"], ["a.b"]]
//? source: ...

[split("a.b", "."), split("a.b", "\\.")]

//?
//? test: split-other-separator
//? description: The separator must be a string or a regexp.
//? expect: error:Parameter 2 of function 'split<string>' must evaluate to data type 'string' or 'regexp' but evaluated to be 'integer'
//? source: ...

split("a1b", 1)
