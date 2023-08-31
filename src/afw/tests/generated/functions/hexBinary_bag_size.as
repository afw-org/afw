#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/hexBinary_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<hexBinary> function.
//? sourceType: expression
//?
//? test: bag_size<hexBinary>-1
//? description: x
//? expect: integer(2)
//? source: ...

bag_size<hexBinary>(
    bag<hexBinary>(
        hexBinary("0FB8000A"),
        hexBinary("0fb8")
    )
)

//?
//? test: bag_size<hexBinary>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

bag_size<hexBinary>(
    bag<hexBinary>(
        hexBinary("x"),
        hexBinary("x")
    )
)

