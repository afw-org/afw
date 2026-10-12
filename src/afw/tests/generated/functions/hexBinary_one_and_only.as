#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/hexBinary_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<hexBinary> function.
//? sourceType: script
//?
//? test: one_and_only<hexBinary>-1
//? description: x
//? expect: "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary
//? source: ...

one_and_only<hexBinary>(
    array<hexBinary>(
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary
    )
)

//?
//? test: one_and_only<hexBinary>-2
//? description: x
//? expect: "0FB8"hexBinary
//? source: ...

one_and_only<hexBinary>(
    array<hexBinary>(
        "0FB8"hexBinary
    )
)

//?
//? test: one_and_only<hexBinary>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

one_and_only<hexBinary>(
    array<hexBinary>(
        "x"hexBinary,
        "x"hexBinary
    )
)

