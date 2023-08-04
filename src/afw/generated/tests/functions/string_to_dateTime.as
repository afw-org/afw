#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_to_dateTime.as
//? customPurpose: Part of core function tests
//? description: Tests for to_dateTime<string> function.
//? sourceType: expression
//?
//? test: to_dateTime<string>-1
//? description: 2004-04-12T13:20:00
//? expect: dateTime("2004-04-12T13:20:00")
//? source: ...

to_dateTime<string>(
    string("2004-04-12T13:20:00")
)

//?
//? test: to_dateTime<string>-2
//? description: 2004-04-12T13:20:15.5
//? expect: dateTime("2004-04-12T13:20:15.5")
//? source: ...

to_dateTime<string>(
    string("2004-04-12T13:20:15.5")
)

//?
//? test: to_dateTime<string>-3
//? description: 2004-04-12T13:20:00-05:00
//? expect: dateTime("2004-04-12T13:20:00-05:00")
//? source: ...

to_dateTime<string>(
    string("2004-04-12T13:20:00-05:00")
)

//?
//? test: to_dateTime<string>-4
//? description: 2004-04-12T13:20:00Z
//? expect: dateTime("2004-04-12T13:20:00Z")
//? source: ...

to_dateTime<string>(
    string("2004-04-12T13:20:00Z")
)

