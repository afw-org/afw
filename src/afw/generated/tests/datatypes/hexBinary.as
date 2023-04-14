#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: datatypes/hexBinary.as
//? customPurpose: Part of core dataType tests
//? description: Tests for hexBinary dataType.
//? sourceType: expression
//?
//? test: hexBinary-1
//? description: Test 1 for hexBinary dataType.
//? expect: boolean(true)
//? source: ...

eq<hexBinary>(
    hexBinary("0FB8"),
    hexBinary("0FB8")
)

//?
//? test: hexBinary-2
//? description: Test 2 for hexBinary dataType.
//? expect: boolean(true)
//? source: ...

eq<hexBinary>(
    hexBinary("0fb8"),
    hexBinary("0fb8")
)

//?
//? test: hexBinary-3
//? description: Test 3 for hexBinary dataType.
//? expect: boolean(true)
//? source: ...

eq<hexBinary>(
    hexBinary(""),
    hexBinary("")
)

