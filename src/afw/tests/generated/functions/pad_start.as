#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/pad_start.as
//? customPurpose: Part of core function tests
//? description: Tests for pad_start and pad_end: lengths in code points; an empty fillString is an error when padding is needed.
//? sourceType: script
//?
//? test: pad_start-zeros
//? description: Pad a number with zeros.
//? expect: "005"
//? source: ...

"5"->pad_start(3, "0")

//?
//? test: pad_start-default
//? description: The default fillString is a space.
//? expect: "  a"
//? source: ...

"a"->pad_start(3)

//?
//? test: pad_start-cut
//? description: fillString is repeated and cut to fit.
//? expect: "xyzxyab"
//? source: ...

"ab"->pad_start(7, "xyz")

//?
//? test: pad_start-code-points
//? description: Lengths and the cut are code points, not UTF-16 units.
//? expect: "🙂é🙂a"
//? source: ...

"a"->pad_start(4, "🙂é")

//?
//? test: pad_start-long-enough
//? description: A value at least maxLength long is returned.
//? expect: "abc"
//? source: ...

"abc"->pad_start(2, "x")

//?
//? test: pad_start-end
//? description: pad_end pads at the end.
//? expect: "abxyzxy"
//? source: ...

"ab"->pad_end(7, "xyz")

//?
//? test: pad_start-empty-fill
//? description: An empty fillString when padding is needed is an error.
//? expect: error:pad_start<string> with an empty fillString can not make the value 5 code points long
//? source: ...

"abc"->pad_start(5, "")

//?
//? test: pad_start-empty-fill-not-needed
//? description: An empty fillString is fine when no padding is needed.
//? expect: "abc"
//? source: ...

"abc"->pad_end(3, "")

//?
//? test: pad_start-max
//? description: maxLength has a limit.
//? expect: error:pad_end<string> maxLength must not exceed 1000000
//? source: ...

""->pad_end(1000001)
