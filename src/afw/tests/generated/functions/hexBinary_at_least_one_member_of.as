#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/hexBinary_at_least_one_member_of.as
//? customPurpose: Part of core function tests
//? description: Tests for at_least_one_member_of<hexBinary> function.
//? sourceType: expression
//?
//? test: at_least_one_member_of<hexBinary>-1
//? description: x
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<hexBinary>(
    bag<hexBinary>(
        hexBinary("0FB80BB6"),
        hexBinary("0FB80BB0"),
        hexBinary("0FB80BB1"),
        hexBinary("0FB80BBA"),
        hexBinary("0FB80BBb")
    ),
    bag<hexBinary>(
        hexBinary("0FB80BB2"),
        hexBinary("0FB80BB3"),
        hexBinary("0FB80BB4"),
        hexBinary("0FB80BB5"),
        hexBinary("0FB80BB6")
    )
)

//?
//? test: at_least_one_member_of<hexBinary>-2
//? description: x
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<hexBinary>(
    bag<hexBinary>(
        hexBinary("0FB80BB7"),
        hexBinary("0FB80BB0"),
        hexBinary("0FB80BB1"),
        hexBinary("0FB80BBA"),
        hexBinary("0FB80BBb")
    ),
    bag<hexBinary>(
        hexBinary("0FB80BB2"),
        hexBinary("0FB80BB3"),
        hexBinary("0FB80BB4"),
        hexBinary("0FB80BB5"),
        hexBinary("0FB80BB6")
    )
)

//?
//? test: at_least_one_member_of<hexBinary>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

at_least_one_member_of<hexBinary>(
    bag<hexBinary>(
        hexBinary("x"),
        hexBinary("x")
    ),
    bag<hexBinary>(
        hexBinary("x"),
        hexBinary("x")
    )
)

