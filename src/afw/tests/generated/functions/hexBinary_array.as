#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/hexBinary_array.as
//? customPurpose: Part of core function tests
//? description: Tests for array<hexBinary> function.
//? sourceType: script
//?
//? test: array<hexBinary>-1
//? description: x
//? expect: ...

array<hexBinary>(
    "0FB80BB2"hexBinary,
    "0FB80BB3"hexBinary,
    "0FB80BB4"hexBinary,
    "0FB80BB5"hexBinary,
    "0FB80BB6"hexBinary
)

//? source: ...

array<hexBinary>(
    "0FB80BB2"hexBinary,
    "0FB80BB3"hexBinary,
    "0FB80BB4"hexBinary,
    "0FB80BB5"hexBinary,
    "0FB80BB6"hexBinary
)

//?
//? test: array<hexBinary>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

array<hexBinary>(
    "x"hexBinary
)

