#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/match.as
//? customPurpose: Part of core function tests
//? description: Tests for match: the first match of a regexp with its groups.
//? sourceType: script
//?
//? test: match-groups
//? description: match, index, captures (group n at [n]) and groups (named).
//? expect: ({"match": "2024-10", "index": 3, "captures": ["2024-10", "2024", "10"], "groups": {"y": "2024", "m": "10"}})
//? source: ...

"on 2024-10-13"->match(/(?<y>\d{4})-(?<m>\d\d)/)

//?
//? test: match-none
//? description: undefined when the regexp does not match.
//? expect: undefined
//? source: ...

match("xyz", /a/)

//?
//? test: match-no-groups
//? description: Without named groups there is no groups property.
//? expect: [["ab", "b"], false]
//? source: ...

let m = match("xab", /a(b)/);
return [m.captures, is_defined(m.groups)];

//?
//? test: match-group-not-taking-part
//? description: A group that did not take part is undefined in captures and absent from groups.
//? expect: [2, true, []]
//? source: ...

let m = match("x", /(?<n>y)?x/);
return [length(m.captures), m.captures[1] === undefined, keys(m.groups)];

//?
//? test: match-code-points
//? description: index counts code points, not bytes.
//? expect: [2, "aab"]
//? source: ...

let m = match("ÄÖaab", /a+b/);
return [m.index, m.match];

//?
//? test: match-empty
//? description: An empty match is a match.
//? expect: ({"match": "", "index": 0, "captures": [""]})
//? source: ...

match("abc", /x*/)

//?
//? test: match-quoted-name
//? description: Text that looks like a named group (here in \Q...\E) is not one.
//? expect: ({"match": "(?<n>b", "index": 0, "captures": ["(?<n>b", "b"], "groups": {"n": "b"}})
//? source: ...

match("(?<n>b", /\Q(?<n>\E(?<n>b)/)

//?
//? test: match-anyURI
//? description: match<anyURI> searches the URI text.
//? expect: "www"
//? source: ...

match<anyURI>("http://www.oasis.org"anyURI, /w+/).match
