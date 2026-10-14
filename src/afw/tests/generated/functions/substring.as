#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/substring.as
//? customPurpose: Part of core function tests
//? description: Out-of-range indexes for polymorphic substring, which clamps them as ECMAScript does (#543). Other coverage lives in the per-dataType files (e.g. string_substring.as).
//? sourceType: script
//?
//? test: substring-negative-start
//? description: a negative startIndex is 0
//? expect: "hello"
//? source: ...

substring("hello", -1)

//?
//? test: substring-start-past-end
//? description: a startIndex past the end is ""
//? expect: ""
//? source: ...

substring("hello", 10)

//?
//? test: substring-end-past-end
//? description: an endIndex past the end is the end
//? expect: "llo"
//? source: ...

substring("hello", 2, 100)

//?
//? test: substring-both-negative
//? description: two negative indexes are both 0, so ""
//? expect: ""
//? source: ...

substring("hello", -3, -2)

//?
//? test: substring-swapped-past-end
//? description: an endIndex before startIndex swaps the two; the end is clamped
//? expect: "llo"
//? source: ...

substring("hello", 10, 2)

//?
//? test: substring-swapped-negative
//? description: a negative endIndex is 0 and swaps with startIndex
//? expect: "hel"
//? source: ...

"hello"->substring(3, -2)

//?
//? test: substring-empty-string
//? description: any indexes on "" give ""
//? expect: ""
//? source: ...

substring("", 1, 5)
