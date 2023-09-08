#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: datatypes/double.as
//? customPurpose: Part of core dataType tests
//? description: Tests for double dataType.
//? sourceType: script
//?
//? test: double-1
//? description: Test 1 for double dataType.
//? expect: boolean(true)
//? source: ...

eq<double>(
    double(-3E2),
    double(-3E2)
)

//?
//? test: double-2
//? description: Test 2 for double dataType.
//? expect: boolean(true)
//? source: ...

eq<double>(
    double(4268.22752E11),
    double(4268.22752E11)
)

//?
//? test: double-3
//? description: Test 3 for double dataType.
//? expect: boolean(true)
//? source: ...

eq<double>(
    double(24.3e-3),
    double(24.3e-3)
)

//?
//? test: double-4
//? description: Test 4 for double dataType.
//? expect: boolean(true)
//? source: ...

eq<double>(
    double(12.0),
    double(12.0)
)

//?
//? test: double-5
//? description: Test 5 for double dataType.
//? expect: boolean(true)
//? source: ...

eq<double>(
    double(3.5),
    double(3.5)
)

//?
//? test: double-6
//? description: Test 6 for double dataType.
//? expect: boolean(true)
//? source: ...

eq<double>(
    double(-Infinity),
    double(-Infinity)
)

//?
//? test: double-7
//? description: Test 7 for double dataType.
//? expect: boolean(true)
//? source: ...

eq<double>(
    double(-0.0),
    double(-0.0)
)

