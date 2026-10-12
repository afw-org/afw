#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/hexBinary_eq.as
//? customPurpose: Part of core function tests
//? description: Tests for eq<hexBinary> function.
//? sourceType: script
//?
//? test: eq<hexBinary>-1
//? description: x
//? expect: boolean(true)
//? source: ...

eq<hexBinary>(
    "0FB8"hexBinary,
    "0fb8"hexBinary
)

//?
//? test: eq<hexBinary>-2
//? description: x
//? expect: boolean(false)
//? source: ...

eq<hexBinary>(
    "0FB8"hexBinary,
    "0eb8"hexBinary
)

//?
//? test: eq<hexBinary>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

eq<hexBinary>(
    "x"hexBinary,
    "x"hexBinary
)

