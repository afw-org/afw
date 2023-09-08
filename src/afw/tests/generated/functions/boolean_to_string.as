#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/boolean_to_string.as
//? customPurpose: Part of core function tests
//? description: Tests for to_string<boolean> function.
//? sourceType: script
//?
//? test: to_string<boolean>-1
//? description: String from boolean true
//? expect: string("true")
//? source: ...

to_string<boolean>(
    boolean(1)
)

//?
//? test: to_string<boolean>-2
//? description: String from boolean true2
//? expect: string("true")
//? source: ...

to_string<boolean>(
    boolean(true)
)

//?
//? test: to_string<boolean>-3
//? description: String from boolean false
//? expect: string("false")
//? source: ...

to_string<boolean>(
    boolean(0)
)

//?
//? test: to_string<boolean>-4
//? description: String from boolean false2
//? expect: string("false")
//? source: ...

to_string<boolean>(
    boolean(false)
)

//?
//? test: to_string<boolean>-5
//? description: Invalid arguments test.
//? expect: error
//? source: ...

to_string<boolean>(
    boolean(x)
)

