#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/search.as
//? customPurpose: Part of core function tests
//? description: Tests for search: the code point index of the first match, or -1.
//? sourceType: script
//?
//? test: search-found
//? description: The index of the first match, anywhere in the value.
//? expect: [3, 3]
//? source: ...

["xyzaab"->search(/a+b/), search("xyzaab", /a+b/)]

//?
//? test: search-code-points
//? description: The index counts code points, not bytes, as index_of does.
//? expect: [2, 2]
//? source: ...

[search("ÄÖaab", /a+b/), index_of("ÄÖaab", "aab")]

//?
//? test: search-not-found
//? description: -1 when the regexp does not match.
//? expect: -1
//? source: ...

search("xyz", /a/)

//?
//? test: search-anyURI
//? description: search<anyURI> searches the URI text.
//? expect: 7
//? source: ...

search<anyURI>("http://www.oasis.org"anyURI, /w+\./)

//?
//? test: search-string-pattern
//? description: A string is not a regexp; index_of finds literal text.
//? expect: error:Parameter 2 of function 'search<string>' must evaluate to data type 'regexp' but evaluated to be 'string'; write "a"regexp or to_regexp(v)
//? source: ...

search("abc", "a")
