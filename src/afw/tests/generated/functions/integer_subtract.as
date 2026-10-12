#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_subtract.as
//? customPurpose: Part of core function tests
//? description: Tests for subtract<integer> function.
//? sourceType: script
//?
//? test: subtract<integer>-1
//? description: Subtract 67 and 1
//? expect: 66
//? source: ...

subtract<integer>(
    67,
    1
)

//?
//? test: subtract<integer>-2
//? description: Subtract -9223372036854775807 and -7
//? expect: -9223372036854775800
//? source: ...

subtract<integer>(
    -9223372036854775807,
    -7
)

//?
//? test: subtract<integer>-3
//? description: Subtract 9223372036854775807 and 10
//? expect: 9223372036854775797
//? source: ...

subtract<integer>(
    9223372036854775807,
    10
)

//?
//? test: subtract<integer>-4
//? description: Subtract 9223372036854775807 and -1 which will overflow
//? expect: error
//? source: ...

subtract<integer>(
    9223372036854775807,
    -1
)

//?
//? test: subtract<integer>-5
//? description: Invalid arguments test.
//? expect: error
//? source: ...

subtract<integer>(
    to_integer(x),
    to_integer(x)
)

//?
//? test: subtract<integer>-6
//? description: Subtract the minimum integer from 0, which will overflow
//? expect: error:Integer subtract overflow
//? source: ...

subtract<integer>(
    0,
    to_integer("-9223372036854775808")
)

//?
//? test: subtract<integer>-7
//? description: Subtract the minimum integer from -1, which is the maximum integer (no overflow)
//? expect: 9223372036854775807
//? source: ...

subtract<integer>(
    -1,
    to_integer("-9223372036854775808")
)
