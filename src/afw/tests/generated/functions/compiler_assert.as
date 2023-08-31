#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/compiler_assert.as
//? customPurpose: Part of core function tests
//? description: Tests for assert.
//? sourceType: expression
//?
//? test: assert-1
//? description: Assertion on true
//? expect: undefined
//? source: ...

assert(
    boolean(true)
)

//?
//? test: assert-2
//? description: Assertion on false
//? expect: error
//? source: ...

assert(
    boolean(false)
)

