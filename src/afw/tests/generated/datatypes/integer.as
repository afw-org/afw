#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: datatypes/integer.as
//? customPurpose: Part of core dataType tests
//? description: Tests for integer dataType.
//? sourceType: script
//?
//? test: integer-1
//? description: Test 1 for integer dataType.
//? expect: boolean(true)
//? source: ...

eq<integer>(
    -1,
    -1
)

//?
//? test: integer-2
//? description: Test 2 for integer dataType.
//? expect: boolean(true)
//? source: ...

eq<integer>(
    122,
    122
)

//?
//? test: integer-3
//? description: Test 3 for integer dataType.
//? expect: boolean(true)
//? source: ...

eq<integer>(
    1220,
    1220
)

//?
//? test: integer-4
//? description: Test 4 for integer dataType.
//? expect: boolean(true)
//? source: ...

eq<integer>(
    -3,
    -3
)

//?
//? test: integer-5
//? description: Test 5 for integer dataType.
//? expect: boolean(true)
//? source: ...

eq<integer>(
    3,
    3
)

