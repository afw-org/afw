#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/base64Binary_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<base64Binary> function.
//? sourceType: expression
//?
//? test: one_and_only<base64Binary>-1
//? description: x
//? expect: base64Binary("0F+40A==")
//? source: ...

one_and_only<base64Binary>(
    bag<base64Binary>(
        base64Binary("0F+40A==")
    )
)

//?
//? test: one_and_only<base64Binary>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

one_and_only<base64Binary>(
    bag<base64Binary>(
        base64Binary("x"),
        base64Binary("x")
    )
)

