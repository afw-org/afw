#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_substring.as
//? customPurpose: Part of core function tests
//? description: Tests for substring<string> function.
//? sourceType: script
//?
//? test: substring<string>-1
//? description: string substring test abc 0 1
//? expect: string("a")
//? source: ...

substring<string>(
    string("abc"),
    0,
    1
)

//?
//? test: substring<string>-2
//? description: string substring test abc 1 2
//? expect: string("b")
//? source: ...

substring<string>(
    string("abc"),
    1,
    2
)

//?
//? test: substring<string>-3
//? description: a negative endIndex is 0, then swapped, as ECMAScript "abc".substring(1, -1) (#543)
//? expect: string("a")
//? source: ...

substring<string>(
    string("abc"),
    1,
    -1
)

//?
//? test: substring<string>-equal-indexes
//? description: an endIndex equal to startIndex is "", as ECMAScript substring (#543)
//? expect: ""
//? source: ...

substring<string>("abc", 1, 1)

//?
//? test: substring<string>-equal-indexes-at-end
//? description: equal indexes at the end of the string are ""
//? expect: ""
//? source: ...

substring<string>("abc", 3, 3)

//?
//? test: substring<string>-swapped
//? description: an endIndex before startIndex swaps the two, as ECMAScript substring (#543)
//? expect: "bc"
//? source: ...

"abc"->substring(3, 1)

//?
//? test: substring<string>-swapped-code-points
//? description: swapped indexes count code points
//? expect: "éü"
//? source: ...

"aéüz"->substring(3, 1)
