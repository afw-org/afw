#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/base64Binary_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<base64Binary> function.
//? sourceType: script
//?
//? test: one_and_only<base64Binary>-1
//? description: x
//? expect: "0F+40A=="base64Binary
//? source: ...

one_and_only<base64Binary>(
    array<base64Binary>(
        "0F+40A=="base64Binary
    )
)

//?
//? test: one_and_only<base64Binary>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

one_and_only<base64Binary>(
    array<base64Binary>(
        "x"base64Binary,
        "x"base64Binary
    )
)

