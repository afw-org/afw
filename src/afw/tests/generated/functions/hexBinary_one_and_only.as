#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/hexBinary_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<hexBinary> function.
//? sourceType: script
//?
//? test: one_and_only<hexBinary>-1
//? description: x
//? expect: hexBinary("0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8")
//? source: ...

one_and_only<hexBinary>(
    bag<hexBinary>(
        hexBinary("0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8")
    )
)

//?
//? test: one_and_only<hexBinary>-2
//? description: x
//? expect: hexBinary("0FB8")
//? source: ...

one_and_only<hexBinary>(
    bag<hexBinary>(
        hexBinary("0FB8")
    )
)

//?
//? test: one_and_only<hexBinary>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

one_and_only<hexBinary>(
    bag<hexBinary>(
        hexBinary("x"),
        hexBinary("x")
    )
)

