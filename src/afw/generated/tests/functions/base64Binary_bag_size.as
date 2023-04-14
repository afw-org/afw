#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/base64Binary_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<base64Binary> function.
//? sourceType: expression
//?
//? test: bag_size<base64Binary>-1
//? description: x
//? expect: integer(3)
//? source: ...

bag_size<base64Binary>(
    bag<base64Binary>(
        base64Binary("0fa8"),
        base64Binary("0fa80f+9"),
        base64Binary("0F+40A==")
    )
)

//?
//? test: bag_size<base64Binary>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

bag_size<base64Binary>(
    bag<base64Binary>(
        base64Binary("x"),
        base64Binary("x")
    )
)

