#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/regexp_replace.as
//? customPurpose: Part of core function tests
//? description: Tests for regexp_replace: limit like replace, and $n, ${name}, and backslash in the replacement.
//? sourceType: script
//?
//? test: regexp_replace-default-limit
//? description: The default limit replaces the first match.
//? expect: "a#b22c333"
//? source: ...

regexp_replace("a1b22c333", "[0-9]+"re, "#")

//?
//? test: regexp_replace-all
//? description: A negative limit replaces every match.
//? expect: "a#b#c#"
//? source: ...

regexp_replace("a1b22c333", "[0-9]+"re, "#", -1)

//?
//? test: regexp_replace-limit
//? description: A limit of 2 replaces the first two matches; 0 replaces none.
//? expect: ["a#b#c333", "a1b22c333"]
//? source: ...

[
    regexp_replace("a1b22c333", "[0-9]+"re, "#", 2),
    regexp_replace("a1b22c333", "[0-9]+"re, "#", 0)
]

//?
//? test: regexp_replace-groups
//? description: $n is a numbered group and $0 the whole match.
//? expect: ["Smith, John", "[John Smith]"]
//? source: ...

[
    regexp_replace("John Smith", "(\\w+) (\\w+)"re, "$2, $1"),
    regexp_replace("John Smith", "\\w+ \\w+"re, "[$0]")
]

//?
//? test: regexp_replace-named-groups
//? description: ${name} is a named group.
//? expect: "Smith John"
//? source: ...

regexp_replace("John Smith", "(?<first>\\w+) (?<last>\\w+)"re, "${last} ${first}")

//?
//? test: regexp_replace-group-not-in-match
//? description: A group that did not take part in the match is empty.
//? expect: "[]b"
//? source: ...

regexp_replace("b", "(a)?b"re, "[$1]b")

//?
//? test: regexp_replace-backslash
//? description: A backslash makes the next character literal.
//? expect: "a$1b"
//? source: ...

regexp_replace("a.b", "\\."re, "\\$1")

//?
//? test: regexp_replace-empty-match
//? description: An empty match is replaced, then the search moves on one code point.
//? expect: "-é-b-"
//? source: ...

regexp_replace("éb", "x*"re, "-", -1)

//?
//? test: regexp_replace-anyURI
//? description: regexp_replace<anyURI> returns a string.
//? expect: "https://www.oasis.org"
//? source: ...

regexp_replace<anyURI>("http://www.oasis.org"anyURI, "^http:"re, "https:")

//?
//? test: regexp_replace-group-out-of-range
//? description: A group the regexp does not have is an argument error.
//? expect: error:The replacement refers to group 9, but the regular expression has 1
//? source: ...

regexp_replace("ab", "(a)"re, "$9")

//?
//? test: regexp_replace-bare-dollar
//? description: A $ not followed by a group number or {name} is an argument error.
//? expect: error:In the replacement, $ must be followed by a group number or {name}; write \$ for a $
//? source: ...

regexp_replace("ab", "a"re, "$x")

//?
//? test: regexp_replace-trailing-backslash
//? description: A replacement that ends with a backslash is an argument error.
//? expect: error:The replacement ends with a backslash; write \\ for a backslash
//? source: ...

regexp_replace("ab", "a"re, "x\\")

//?
//? test: regexp_replace-unknown-name
//? description: A group name the regexp does not have is an argument error.
//? expect: error:The replacement refers to group ${zz}, which the regular expression does not name
//? source: ...

regexp_replace("ab", "a"re, "${zz}")
