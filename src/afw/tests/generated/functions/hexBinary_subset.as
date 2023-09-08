#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/hexBinary_subset.as
//? customPurpose: Part of core function tests
//? description: Tests for subset<hexBinary> function.
//? sourceType: script
//?
//? test: subset<hexBinary>-1
//? description: x
//? expect: boolean(true)
//? source: ...

subset<hexBinary>(
    bag<hexBinary>(
        hexBinary("0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("1FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("2FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("3FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("4FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8")
    ),
    bag<hexBinary>(
        hexBinary("0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("1FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("2FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("3FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("4FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("5FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8")
    )
)

//?
//? test: subset<hexBinary>-2
//? description: x
//? expect: boolean(false)
//? source: ...

subset<hexBinary>(
    bag<hexBinary>(
        hexBinary("0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("1FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("2FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("3FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("4FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8")
    ),
    bag<hexBinary>(
        hexBinary("0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("2FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("3FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("4FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("5FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8")
    )
)

//?
//? test: subset<hexBinary>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

subset<hexBinary>(
    bag<hexBinary>(
        hexBinary("x"),
        hexBinary("x")
    ),
    bag<hexBinary>(
        hexBinary("x"),
        hexBinary("x")
    )
)

