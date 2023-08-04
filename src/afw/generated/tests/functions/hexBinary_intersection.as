#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/hexBinary_intersection.as
//? customPurpose: Part of core function tests
//? description: Tests for intersection<hexBinary> function.
//? sourceType: expression
//?
//? test: intersection<hexBinary>-1
//? description: x
//? expect: ...

bag<hexBinary>(
    hexBinary("0FB7"),
    hexBinary("0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB9")
)

//? source: ...

intersection<hexBinary>(
    bag<hexBinary>(
        hexBinary("0fb7"),
        hexBinary("0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("1FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("2FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB9")
    ),
    bag<hexBinary>(
        hexBinary("0fb7"),
        hexBinary("FFB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"),
        hexBinary("0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB9")
    )
)

//?
//? test: intersection<hexBinary>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

intersection<hexBinary>(
    bag<hexBinary>(
        hexBinary("x"),
        hexBinary("x")
    ),
    bag<hexBinary>(
        hexBinary("x"),
        hexBinary("x")
    )
)

