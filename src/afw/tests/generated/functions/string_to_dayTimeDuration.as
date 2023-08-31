#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_to_dayTimeDuration.as
//? customPurpose: Part of core function tests
//? description: Tests for to_dayTimeDuration<string> function.
//? sourceType: expression
//?
//? test: to_dayTimeDuration<string>-1
//? description: -P1DT10H30M
//? expect: dayTimeDuration("-P1DT10H30M")
//? source: ...

to_dayTimeDuration<string>(
    string("-P1DT10H30M")
)

//?
//? test: to_dayTimeDuration<string>-2
//? description: P1DT10H30M
//? expect: dayTimeDuration("P1DT10H30M")
//? source: ...

to_dayTimeDuration<string>(
    string("P1DT10H30M")
)

//?
//? test: to_dayTimeDuration<string>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

to_dayTimeDuration<string>(
    string("x")
)

