#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/base64Binary_eq.as
//? customPurpose: Part of core function tests
//? description: Tests for eq<base64Binary> function.
//? sourceType: script
//?
//? test: eq<base64Binary>-1
//? description: x
//? expect: boolean(true)
//? source: ...

eq<base64Binary>(
    base64Binary("0f+41A=="),
    base64Binary("0f+41A==")
)

//?
//? test: eq<base64Binary>-2
//? description: x
//? expect: boolean(false)
//? source: ...

eq<base64Binary>(
    base64Binary("0F+41A=="),
    base64Binary("0E+41A==")
)

//?
//? test: eq<base64Binary>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

eq<base64Binary>(
    base64Binary("x"),
    base64Binary("x")
)

