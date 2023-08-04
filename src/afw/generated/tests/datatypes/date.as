#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: datatypes/date.as
//? customPurpose: Part of core dataType tests
//? description: Tests for date dataType.
//? sourceType: expression
//?
//? test: date-1
//? description: Test 1 for date dataType.
//? expect: boolean(true)
//? source: ...

eq<date>(
    date("2004-04-12"),
    date("2004-04-12")
)

//?
//? test: date-2
//? description: Test 2 for date dataType.
//? expect: boolean(true)
//? source: ...

eq<date>(
    date("-1970-01-01"),
    date("-1970-01-01")
)

//?
//? test: date-3
//? description: Test 3 for date dataType.
//? expect: boolean(true)
//? source: ...

eq<date>(
    date("2004-04-12"),
    date("2004-04-12")
)

//?
//? test: date-4
//? description: Test 4 for date dataType.
//? expect: boolean(true)
//? source: ...

eq<date>(
    date("2004-04-12-05:00"),
    date("2004-04-12-05:00")
)

//?
//? test: date-5
//? description: Test 5 for date dataType.
//? expect: boolean(true)
//? source: ...

eq<date>(
    date("2004-04-12Z"),
    date("2004-04-12Z")
)

