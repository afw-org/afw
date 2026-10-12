#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/hexBinary_intersection.as
//? customPurpose: Part of core function tests
//? description: Tests for intersection<hexBinary> function.
//? sourceType: script
//?
//? test: intersection<hexBinary>-1
//? description: x
//? expect: ...

bag<hexBinary>(
    "0FB7"hexBinary,
    "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB9"hexBinary
)

//? source: ...

intersection<hexBinary>(
    bag<hexBinary>(
        "0fb7"hexBinary,
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary,
        "1FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary,
        "2FB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary,
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB9"hexBinary
    ),
    bag<hexBinary>(
        "0fb7"hexBinary,
        "FFB80FB80FB80FB80FB80FB80FB80FB80FB80FB8"hexBinary,
        "0FB80FB80FB80FB80FB80FB80FB80FB80FB80FB9"hexBinary
    )
)

//?
//? test: intersection<hexBinary>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

intersection<hexBinary>(
    bag<hexBinary>(
        "x"hexBinary,
        "x"hexBinary
    ),
    bag<hexBinary>(
        "x"hexBinary,
        "x"hexBinary
    )
)

