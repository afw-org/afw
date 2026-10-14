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
//? test: replace-other-search
//? description: The search must be a string or a regexp.
//? expect: error:Parameter 2 of function 'replace<string>' must evaluate to data type 'string' or 'regexp' but evaluated to be 'integer'
//? source: ...

replace("a", 1, "x")

//?
//? test: replace-regexp-default-limit
//? description: The default limit replaces the first match.
//? expect: "a#b22c333"
//? source: ...

replace("a1b22c333", "[0-9]+"re, "#")

//?
//? test: replace-regexp-all
//? description: A negative limit replaces every match.
//? expect: "a#b#c#"
//? source: ...

replace("a1b22c333", "[0-9]+"re, "#", -1)

//?
//? test: replace-regexp-limit
//? description: A limit of 2 replaces the first two matches; 0 replaces none.
//? expect: ["a#b#c333", "a1b22c333"]
//? source: ...

[
    replace("a1b22c333", "[0-9]+"re, "#", 2),
    replace("a1b22c333", "[0-9]+"re, "#", 0)
]

//?
//? test: replace-regexp-groups
//? description: $n is a numbered group and $0 the whole match.
//? expect: ["Smith, John", "[John Smith]"]
//? source: ...

[
    replace("John Smith", "(\\w+) (\\w+)"re, "$2, $1"),
    replace("John Smith", "\\w+ \\w+"re, "[$0]")
]

//?
//? test: replace-regexp-named-groups
//? description: ${name} is a named group.
//? expect: "Smith John"
//? source: ...

replace("John Smith", "(?<first>\\w+) (?<last>\\w+)"re, "${last} ${first}")

//?
//? test: replace-regexp-group-not-in-match
//? description: A group that did not take part in the match is empty.
//? expect: "[]b"
//? source: ...

replace("b", "(a)?b"re, "[$1]b")

//?
//? test: replace-regexp-backslash
//? description: A backslash makes the next character literal.
//? expect: "a$1b"
//? source: ...

replace("a.b", "\\."re, "\\$1")

//?
//? test: replace-regexp-empty-match
//? description: An empty match is replaced, then the search moves on one code point.
//? expect: "-é-b-"
//? source: ...

replace("éb", "x*"re, "-", -1)

//?
//? test: replace-regexp-anyURI
//? description: replace<anyURI> with a regexp returns a string.
//? expect: "https://www.oasis.org"
//? source: ...

replace<anyURI>("http://www.oasis.org"anyURI, "^http:"re, "https:")

//?
//? test: replace-regexp-group-out-of-range
//? description: A group the regexp does not have is an argument error.
//? expect: error:The replacement refers to group 9, but the regular expression has 1
//? source: ...

replace("ab", "(a)"re, "$9")

//?
//? test: replace-regexp-bare-dollar
//? description: A $ not followed by a group number or {name} is an argument error.
//? expect: error:In the replacement, $ must be followed by a group number or {name}; write \$ for a $
//? source: ...

replace("ab", "a"re, "$x")

//?
//? test: replace-regexp-trailing-backslash
//? description: A replacement that ends with a backslash is an argument error.
//? expect: error:The replacement ends with a backslash; write \\ for a backslash
//? source: ...

replace("ab", "a"re, "x\\")

//?
//? test: replace-regexp-unknown-name
//? description: A group name the regexp does not have is an argument error.
//? expect: error:The replacement refers to group ${zz}, which the regular expression does not name
//? source: ...

replace("ab", "a"re, "${zz}")

//?
//? test: replace-function
//? description: A function replacement is called with each match's _AdaptiveRegexpMatch_ object and returns the text to put in.
//? expect: ["13/10/2024", "a<1@1>b<22@3>", "é1b"]
//? source: ...

return [
    replace("2024-10-13", /(?<y>\d+)-(?<m>\d+)-(?<d>\d+)/,
        function (m) { return m.groups.d + "/" + m.groups.m + "/" + m.groups.y; }),
    replace("a1b22", /\d+/,
        function (m) { return "<" + m.match + "@" + to_string(m.index) + ">"; }, -1),
    replace("éab", /a/, function (m) { return to_string(m.index); })
];

//?
//? test: replace-function-captures
//? description: captures[n] is group n; a group that did not take part is undefined.
//? expect: "3:true:b"
//? source: ...

return replace("ab", /(a)(x)?/, function (m) {
    return to_string(length(m.captures)) + ":" + to_string(m.captures[2] === undefined) + ":";
});

//?
//? test: replace-function-string-search
//? description: With a string search the function gets a match object for the literal text; an empty string matches at each boundary.
//? expect: ["a[.]b[.]c", "-a-b-c-", "-abc", "x"]
//? source: ...

let f = function (m) { return "-"; };
return [
    replace("a.b.c", ".", function (m) { return "[" + m.match + "]"; }, -1),
    replace("abc", "", f, -1),
    replace("abc", "", f),
    replace("", "", function (m) { return "x"; })
];

//?
//? test: replace-function-not-called
//? description: With no match, or limit 0, the function is not called.
//? expect: ["ab", "ab"]
//? source: ...

let boom = function (m) { throw "called"; };
return [replace("ab", /x/, boom), replace("ab", /a/, boom, 0)];

//?
//? test: replace-function-returns-string
//? description: The function must return a string; nothing is converted.
//? expect: error:The replacement function of 'replace<string>' must return a string, but returned data type 'integer'
//? source: ...

replace("ab", /a/, function (m) { return 1; })

//?
//? test: replace-function-returns-undefined
//? description: Returning nothing is an error too.
//? expect: error:The replacement function of 'replace<string>' must return a string, but returned undefined
//? source: ...

replace("ab", /a/, function (m) { })

//?
//? test: replace-function-throws
//? description: An error the function throws goes to the caller.
//? expect: error:called
//? source: ...

replace("ab", /a/, function (m) { throw "called"; })

//?
//? test: replace-other-replacement
//? description: The replacement must be a string or a function.
//? expect: error:Parameter 3 of function 'replace<string>' must evaluate to data type 'string' or 'function' but evaluated to be 'integer'
//? source: ...

replace("ab", /a/, 5)

//?
//? test: replace-empty-regexp
//? description: An empty regexp matches the empty string at each boundary, as ECMAScript's new RegExp("") does.
//? expect: ["-a-b-c-", ["a", "b"], true]
//? source: ...

return [
    "abc"->replace(to_regexp(""), "-", -1),
    split("ab", to_regexp("")),
    test(to_regexp(""), "")
];
