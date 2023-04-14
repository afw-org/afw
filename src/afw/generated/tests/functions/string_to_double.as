#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_to_double.as
//? customPurpose: Part of core function tests
//? description: Tests for to_double<string> function.
//? sourceType: expression
//?
//? test: to_double<string>-1
//? description: double from string test 1.2
//? expect: double(1.2)
//? source: ...

to_double<string>(
    string("1.2")
)

//?
//? test: to_double<string>-2
//? description: double from string test 1.23E4
//? expect: double(1.23E4)
//? source: ...

to_double<string>(
    string("12300.0")
)

//?
//? test: to_double<string>-3
//? description: double from string test -INF
//? expect: double(-Infinity)
//? source: ...

to_double<string>(
    string("-INF")
)

//?
//? test: to_double<string>-4
//? description: Invalid arguments test.
//? expect: error
//? source: ...

to_double<string>(
    string("x")
)

