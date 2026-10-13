#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/base64Binary_array.as
//? customPurpose: Part of core function tests
//? description: Tests for array<base64Binary> function.
//? sourceType: script
//?
//? test: array<base64Binary>-1
//? description: x
//? expect: ...

array<base64Binary>(
    "0fa8"base64Binary,
    "0fa80f+9"base64Binary,
    "0F+40A=="base64Binary
)

//? source: ...

array<base64Binary>(
    "0fa8"base64Binary,
    "0fa80f+9"base64Binary,
    "0F+40A=="base64Binary
)

//?
//? test: array<base64Binary>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

array<base64Binary>(
    "x"base64Binary
)

