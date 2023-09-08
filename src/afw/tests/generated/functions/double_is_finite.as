#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_is_finite.as
//? customPurpose: Part of core function tests
//? description: Tests for is_finite function.
//? sourceType: script
//?
//? test: is_finite-1
//? description: Is 'Infinity' finite
//? expect: boolean(false)
//? source: ...

is_finite(
    double(Infinity)
)

//?
//? test: is_finite-2
//? description: Is '-Infinity' finite
//? expect: boolean(false)
//? source: ...

is_finite(
    double(-Infinity)
)

//?
//? test: is_finite-3
//? description: Is double '23.0E35' finite
//? expect: boolean(true)
//? source: ...

is_finite(
    double(23.0E35)
)

//?
//? test: is_finite-4
//? description: Is double '-23.0E35' finite
//? expect: boolean(true)
//? source: ...

is_finite(
    double(-23.0E35)
)

//?
//? test: is_finite-5
//? description: Is integer '20' finite
//? expect: error
//? source: ...

is_finite(
    integer(20)
)

//?
//? test: is_finite-6
//? description: Is integer '-20' finite
//? expect: error
//? source: ...

is_finite(
    integer(-20)
)

//?
//? test: is_finite-7
//? description: Is 'null' finite
//? expect: error
//? source: ...

is_finite(
    null(null)
)

//?
//? test: is_finite-8
//? description: Invalid argument test
//? expect: error
//? source: ...

is_finite(
    string("x")
)

