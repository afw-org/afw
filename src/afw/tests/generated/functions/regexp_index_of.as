#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/regexp_index_of.as
//? customPurpose: Part of core function tests
//? description: Tests for regexp_index_of: the code point index of the first match, or -1.
//? sourceType: script
//?
//? test: regexp_index_of-found
//? description: The index of the first match, anywhere in the value.
//? expect: 3
//? source: ...

regexp_index_of("xyzaab", "a+b"re)

//?
//? test: regexp_index_of-code-points
//? description: The index counts code points, not bytes, as index_of does.
//? expect: [2, 2]
//? source: ...

[regexp_index_of("ÄÖaab", "a+b"re), index_of("ÄÖaab", "aab")]

//?
//? test: regexp_index_of-not-found
//? description: -1 when the regexp does not match.
//? expect: -1
//? source: ...

regexp_index_of("xyz", "a"re)

//?
//? test: regexp_index_of-anchored
//? description: ^ anchors the match at the start.
//? expect: [-1, 0]
//? source: ...

[regexp_index_of("xab", "^ab"re), regexp_index_of("abx", "^ab"re)]

//?
//? test: regexp_index_of-anyURI
//? description: regexp_index_of<anyURI> searches the URI text.
//? expect: 7
//? source: ...

regexp_index_of<anyURI>("http://www.oasis.org"anyURI, "w+\\."re)

//?
//? test: regexp_index_of-string-pattern
//? description: A string is not a regexp.
//? expect: error:Parameter 2 of function 'regexp_index_of<string>' must evaluate to data type 'regexp' but evaluated to be 'string'; write "a"regexp or to_regexp(v)
//? source: ...

regexp_index_of("abc", "a")
