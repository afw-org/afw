#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/replace_all.as
//? customPurpose: Part of core function tests
//? description: Tests for replace_all: replace with no limit.
//? sourceType: script
//?
//? test: replace_all-string
//? description: A string search is literal text; every match is replaced.
//? expect: "a-b-c"
//? source: ...

"a.b.c"->replace_all(".", "-")

//?
//? test: replace_all-regexp
//? description: A regexp needs no g flag.
//? expect: "a#b#"
//? source: ...

"a1b22"->replace_all(/\d+/, "#")

//?
//? test: replace_all-function
//? description: A function replacement is called with each match object.
//? expect: "[a][b]"
//? source: ...

"ab"->replace_all(/./, function(m) { return "[" + m.match + "]"; })

//?
//? test: replace_all-empty
//? description: An empty search matches at every code-point boundary.
//? expect: "-a-é-"
//? source: ...

"aé"->replace_all("", "-")

//?
//? test: replace_all-no-limit
//? description: There is no limit parameter.
//? expect: error:'replace_all' expects no more than 3 parameters
//? source: ...

"aa"->replace_all("a", "b", 1)
