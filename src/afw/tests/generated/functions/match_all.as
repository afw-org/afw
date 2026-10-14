#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/match_all.as
//? customPurpose: Part of core function tests
//? description: Tests for match_all: every match of a regexp with its groups.
//? sourceType: script
//?
//? test: match_all-each
//? description: Every match, left to right, with its groups.
//? expect: [{"match": "2024-10", "index": 0, "captures": ["2024-10", "2024", "10"], "groups": {"y": "2024", "m": "10"}}, {"match": "2025-01", "index": 12, "captures": ["2025-01", "2025", "01"], "groups": {"y": "2025", "m": "01"}}]
//? source: ...

"2024-10 and 2025-01"->match_all(/(?<y>\d{4})-(?<m>\d\d)/)

//?
//? test: match_all-none
//? description: An empty array when the regexp does not match.
//? expect: []
//? source: ...

match_all("xyz", /a/)

//?
//? test: match_all-count
//? description: length counts the matches.
//? expect: 3
//? source: ...

length(match_all("a1b22c333", /\d+/))

//?
//? test: match_all-limit
//? description: limit is the most matches to return; a negative limit is every match.
//? expect: [["1"], [], ["1", "22", "333"]]
//? source: ...

[
    map(match_all("a1b22c333", /\d+/, 1), function (m) { return m.match; }),
    match_all("a1b22c333", /\d+/, 0),
    map(match_all("a1b22c333", /\d+/, -1), function (m) { return m.match; })
]

//?
//? test: match_all-empty-matches
//? description: After an empty match the next search starts one code point later, as in ECMAScript.
//? expect: [["aaa", ""], [0, 1, 2, 3]]
//? source: ...

[
    map(match_all("aaa", /a*/), function (m) { return m.match; }),
    map(match_all("aÄc", /(?:)/), function (m) { return m.index; })
]

//?
//? test: match_all-code-points
//? description: Each index counts code points, not bytes.
//? expect: [1, 4]
//? source: ...

map(match_all("ÄbÖÜb", /b/), function (m) { return m.index; })

//?
//? test: match_all-backtracking-limit
//? description: A match that backtracks too much stops at limitRegexpMatchSteps, after earlier matches were found.
//? expect: error:Regular expression match exceeded limitRegexpMatchSteps: the pattern backtracks too much on this value, or has a lazy loop over a group that can match nothing, such as (b|)*? (write b*?)
//? source: ...

match_all("b " + repeat("a", 40) + "!", /\b(a+|b)+\b(?!!)/)
