#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/from_code_point.as
//? customPurpose: Part of core function tests
//? description: Tests for from_code_point: a string of code points.
//? sourceType: script
//?
//? test: from_code_point-basic
//? description: Code points to a string.
//? expect: "a🙂"
//? source: ...

from_code_point(97, 0x1F642)

//?
//? test: from_code_point-none
//? description: No code points is the empty string.
//? expect: ""
//? source: ...

from_code_point()

//?
//? test: from_code_point-nfc
//? description: The result is NFC, like every string.
//? expect: "é"
//? source: ...

from_code_point(0x65, 0x301)

//?
//? test: from_code_point-round-trip
//? description: The other half of code_point_at.
//? expect: "🙂"
//? source: ...

from_code_point("🙂"->code_point_at(0))

//?
//? test: from_code_point-surrogate
//? description: A surrogate is an error: a string can not hold one.
//? expect: error:Parameter 1 of function 'from_code_point' is 55296, which is not a code point a string can hold (0 to 0x10FFFF, not a surrogate)
//? source: ...

from_code_point(0xD800)

//?
//? test: from_code_point-too-big
//? description: Above 0x10FFFF is an error.
//? expect: error:Parameter 2 of function 'from_code_point' is 1114112, which is not a code point a string can hold (0 to 0x10FFFF, not a surrogate)
//? source: ...

from_code_point(97, 0x110000)
