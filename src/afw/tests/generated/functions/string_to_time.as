#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_to_time.as
//? customPurpose: Part of core function tests
//? description: Tests for to_time<string> function.
//? sourceType: script
//?
//? test: to_time<string>-1
//? description: x
//? expect: time("13:20:30.5555")
//? source: ...

to_time<string>(
    string("13:20:30.5555")
)

//?
//? test: to_time<string>-2
//? description: x
//? expect: time("13:20:00-05:00")
//? source: ...

to_time<string>(
    string("13:20:00-05:00")
)

//?
//? test: to_time<string>-3
//? description: x
//? expect: time("13:20:30")
//? source: ...

to_time<string>(
    string("13:20:30")
)

