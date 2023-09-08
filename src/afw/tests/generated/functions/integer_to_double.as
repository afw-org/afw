#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_to_double.as
//? customPurpose: Part of core function tests
//? description: Tests for to_double<integer> function.
//? sourceType: script
//?
//? test: to_double<integer>-1
//? description: 9223372036854775807
//? expect: double(9223372036854775807.0)
//? source: ...

to_double<integer>(
    integer(9223372036854775807)
)

//?
//? test: to_double<integer>-2
//? description: -9223372036854775807
//? expect: double(-9223372036854775807.0)
//? source: ...

to_double<integer>(
    integer(-9223372036854775807)
)

//?
//? test: to_double<integer>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

to_double<integer>(
    integer(x)
)

