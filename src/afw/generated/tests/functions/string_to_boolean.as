#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_to_boolean.as
//? customPurpose: Part of core function tests
//? description: Tests for to_boolean<string> function.
//? sourceType: expression
//?
//? test: to_boolean<string>-1
//? description: Boolean False from string "" (empty string)
//? expect: boolean(false)
//? source: ...

to_boolean<string>(
    ""
)

//?
//? test: to_boolean<string>-2
//? description: Boolean True from string "whatever" (any non-empty string)
//? expect: boolean(true)
//? source: ...

to_boolean<string>(
    string("whatever")
)

