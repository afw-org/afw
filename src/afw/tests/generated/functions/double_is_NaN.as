#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_is_NaN.as
//? customPurpose: Part of core function tests
//? description: Tests for is_NaN function.
//? sourceType: expression
//?
//? test: is_NaN-1
//? description: Is 'Infinity' NaN
//? expect: boolean(false)
//? source: ...

is_NaN(
    double(Infinity)
)

//?
//? test: is_NaN-2
//? description: Is '-Infinity' NaN
//? expect: boolean(false)
//? source: ...

is_NaN(
    double(-Infinity)
)

//?
//? test: is_NaN-3
//? description: Is double '23.0E35' NaN
//? expect: boolean(false)
//? source: ...

is_NaN(
    double(23.0E35)
)

//?
//? test: is_NaN-4
//? description: Is double '-23.0E35' NaN
//? expect: boolean(false)
//? source: ...

is_NaN(
    double(-23.0E35)
)

//?
//? test: is_NaN-5
//? description: Is integer '20' NaN
//? expect: error
//? source: ...

is_NaN(
    integer(20)
)

//?
//? test: is_NaN-6
//? description: Is integer '-20' NaN
//? expect: error
//? source: ...

is_NaN(
    integer(-20)
)

//?
//? test: is_NaN-7
//? description: Is 'null' NaN
//? expect: error
//? source: ...

is_NaN(
    null(null)
)

//?
//? test: is_NaN-8
//? description: Invalid argument test
//? expect: error
//? source: ...

is_NaN(
    string("x")
)

