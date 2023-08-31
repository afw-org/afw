#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/hexBinary_bag.as
//? customPurpose: Part of core function tests
//? description: Tests for bag<hexBinary> function.
//? sourceType: expression
//?
//? test: bag<hexBinary>-1
//? description: x
//? expect: ...

bag<hexBinary>(
    hexBinary("0FB80BB2"),
    hexBinary("0FB80BB3"),
    hexBinary("0FB80BB4"),
    hexBinary("0FB80BB5"),
    hexBinary("0FB80BB6")
)

//? source: ...

bag<hexBinary>(
    hexBinary("0FB80BB2"),
    hexBinary("0FB80BB3"),
    hexBinary("0FB80BB4"),
    hexBinary("0FB80BB5"),
    hexBinary("0FB80BB6")
)

//?
//? test: bag<hexBinary>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

bag<hexBinary>(
    hexBinary("x")
)

