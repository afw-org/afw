#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/base64Binary_bag.as
//? customPurpose: Part of core function tests
//? description: Tests for bag<base64Binary> function.
//? sourceType: expression
//?
//? test: bag<base64Binary>-1
//? description: x
//? expect: ...

bag<base64Binary>(
    base64Binary("0fa8"),
    base64Binary("0fa80f+9"),
    base64Binary("0F+40A==")
)

//? source: ...

bag<base64Binary>(
    base64Binary("0fa8"),
    base64Binary("0fa80f+9"),
    base64Binary("0F+40A==")
)

//?
//? test: bag<base64Binary>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

bag<base64Binary>(
    base64Binary("x")
)

