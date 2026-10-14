#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/code_point_at.as
//? customPurpose: Part of core function tests
//? description: Tests for code_point_at: code points, not UTF-16 units.
//? sourceType: script
//?
//? test: code_point_at-ascii
//? description: An ASCII letter.
//? expect: 97
//? source: ...

"abc"->code_point_at(0)

//?
//? test: code_point_at-astral
//? description: A code point above U+FFFF is one index.
//? expect: [128578, 98]
//? source: ...

["a🙂b"->code_point_at(1), "a🙂b"->code_point_at(2)]

//?
//? test: code_point_at-negative
//? description: Negative indexes count from the end, as at.
//? expect: 99
//? source: ...

"abc"->code_point_at(-1)

//?
//? test: code_point_at-out-of-range
//? description: Out of range is undefined, as at.
//? expect: [true, true]
//? source: ...

["abc"->code_point_at(3) === undefined, "abc"->code_point_at(-4) === undefined]
