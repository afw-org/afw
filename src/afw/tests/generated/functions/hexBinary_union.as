#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/hexBinary_union.as
//? customPurpose: Part of core function tests
//? description: Tests for union<hexBinary> function.
//? sourceType: script
//?
//? test: union<hexBinary>-1
//? description: x
//? expect: ...

bag<hexBinary>(
    hexBinary("0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
    hexBinary("1FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
    hexBinary("2FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
    hexBinary("3FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
    hexBinary("4FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
    hexBinary("0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB9"),
    hexBinary("1FB80FB80FB80FB80FB80FB80FB80FB80FB80FB1"),
    hexBinary("2FB80FB80FB80FB80FB80FB80FB80FB80FB80FB2"),
    hexBinary("3FB80FB80FB80FB80FB80FB80FB80FB80FB80FB3"),
    hexBinary("4FB80FB80FB80FB80FB80FB80FB80FB80FB80FB4")
)

//? source: ...

union<hexBinary>(
    bag<hexBinary>(
        hexBinary("0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("1FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("2FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("3FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("4FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8")
    ),
    bag<hexBinary>(
        hexBinary("0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB9"),
        hexBinary("1FB80FB80FB80FB80FB80FB80FB80FB80FB80FB1"),
        hexBinary("2FB80FB80FB80FB80FB80FB80FB80FB80FB80FB2"),
        hexBinary("3FB80FB80FB80FB80FB80FB80FB80FB80FB80FB3"),
        hexBinary("4FB80FB80FB80FB80FB80FB80FB80FB80FB80FB4")
    )
)

//?
//? test: union<hexBinary>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

union<hexBinary>(
    bag<hexBinary>(
        hexBinary("x"),
        hexBinary("x")
    ),
    bag<hexBinary>(
        hexBinary("x"),
        hexBinary("x")
    )
)

