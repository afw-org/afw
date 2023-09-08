#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_to_string.as
//? customPurpose: Part of core function tests
//? description: Tests for to_string<double> function.
//? sourceType: script
//?
//? test: to_string<double>-1
//? description: string from double test 1.3
//? expect: string("1.3E0")
//? source: ...

to_string<double>(
    double(1.3)
)

//?
//? test: to_string<double>-2
//? description: string from double test 1.24e4
//? expect: string("1.24E4")
//? source: ...

to_string<double>(
    double(12400.0)
)

//?
//? test: to_string<double>-3
//? description: string from double test -Infinity
//? expect: string("-Infinity")
//? source: ...

to_string<double>(
    double(-Infinity)
)

//?
//? test: to_string<double>-4
//? description: string from double test
//? expect: string("1.844224E19")
//? source: ...

to_string<double>(
    double(18.44224E18)
)

//?
//? test: to_string<double>-5
//? description: string from double test
//? expect: string("1.844002E-3")
//? source: ...

to_string<double>(
    double(18.44002E-004)
)

//?
//? test: to_string<double>-6
//? description: Invalid arguments test.
//? expect: string("0.0E0")
//? source: ...

to_string<double>(
    double(0.0)
)

