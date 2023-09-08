#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/hexBinary_is_in.as
//? customPurpose: Part of core function tests
//? description: Tests for is_in<hexBinary> function.
//? sourceType: script
//?
//? test: is_in<hexBinary>-1
//? description: x
//? expect: boolean(false)
//? source: ...

is_in<hexBinary>(
    hexBinary("0FB8"),
    bag<hexBinary>(
        hexBinary("0fb7"),
        hexBinary("0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8")
    )
)

//?
//? test: is_in<hexBinary>-2
//? description: x
//? expect: boolean(true)
//? source: ...

is_in<hexBinary>(
    hexBinary("0FB8"),
    bag<hexBinary>(
        hexBinary("0fb8"),
        hexBinary("0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8")
    )
)

//?
//? test: is_in<hexBinary>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

is_in<hexBinary>(
    hexBinary("x"),
    bag<hexBinary>(
        hexBinary("x"),
        hexBinary("x")
    )
)

