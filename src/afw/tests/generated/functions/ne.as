#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/ne.as
//? customPurpose: Part of core function tests
//? description: Tests for polymorphic functions. No per-dataType ne_* files exist yet, so this covers a happy path plus the error path.
//? sourceType: script
//?
//? test: ne-1
//? description: NE with integers
//? expect: boolean(true)
//? source: ...

ne(
    integer(6),
    integer(5)
)

//?
//? test: ne-error-mismatched-types
//? description: NE throws when arg1 and arg2 data types do not match
//? expect: error:Data types do not match
//? source: ...

ne(
    integer(6),
    string("6")
)
