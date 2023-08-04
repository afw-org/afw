#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: datatypes/boolean.as
//? customPurpose: Part of core dataType tests
//? description: Tests for boolean dataType.
//? sourceType: expression
//?
//? test: boolean-1
//? description: Test 1 for boolean dataType.
//? expect: boolean(true)
//? source: ...

eq<boolean>(
    boolean(true),
    boolean(true)
)

//?
//? test: boolean-2
//? description: Test 2 for boolean dataType.
//? expect: boolean(true)
//? source: ...

eq<boolean>(
    boolean(false),
    boolean(false)
)

//?
//? test: boolean-3
//? description: Test 3 for boolean dataType.
//? expect: boolean(true)
//? source: ...

eq<boolean>(
    boolean(0),
    boolean(0)
)

//?
//? test: boolean-4
//? description: Test 4 for boolean dataType.
//? expect: boolean(true)
//? source: ...

eq<boolean>(
    boolean(1),
    boolean(1)
)

