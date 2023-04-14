#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_subset.as
//? customPurpose: Part of core function tests
//? description: Tests for subset<integer> function.
//? sourceType: expression
//?
//? test: subset<integer>-1
//? description: ...

            Integer subset of bags:
            { },
            { }
        
//? expect: boolean(true)
//? source: ...

subset<integer>(
    bag<integer>(
    ),
    bag<integer>(
    )
)

//?
//? test: subset<integer>-2
//? description: ...

            Integer subset of bags:
            { "11", "-11", "-1024", "0" },
            {
                "200", "11", "-11.2", "96", "99", "-102", "-586.5", "387", "512618", "-512618", "200",
                "11", "-11", "96", "-1024", "99", "-102", "-586", "986", "-0.0", "0"
            }
        
//? expect: boolean(true)
//? source: ...

subset<integer>(
    bag<integer>(
        integer(11),
        integer(-11),
        integer(-1024),
        integer(0),
        integer(-0),
        integer(986)
    ),
    bag<integer>(
        integer(200),
        integer(11),
        integer(-11),
        integer(96),
        integer(99),
        integer(-102),
        integer(-586),
        integer(387),
        integer(512618),
        integer(-512618),
        integer(200),
        integer(11),
        integer(-11),
        integer(96),
        integer(-1024),
        integer(99),
        integer(-102),
        integer(-586),
        integer(986),
        integer(-0),
        integer(0)
    )
)

//?
//? test: subset<integer>-3
//? description: ...

            Integer subset of bags:
            { "11", "-11", "0", "-512618", "1024" },
            {
                "11", "-11.2", "96", "-102", "-586.5", "986", "387", "512618",
                "-512618", "11", "96", "-102", "-586", "986", "0"
            }
        
//? expect: boolean(false)
//? source: ...

subset<integer>(
    bag<integer>(
        integer(11),
        integer(-11),
        integer(0),
        integer(-512618),
        integer(1024)
    ),
    bag<integer>(
        integer(11),
        integer(-11),
        integer(96),
        integer(-102),
        integer(-586),
        integer(986),
        integer(387),
        integer(512618),
        integer(-512618),
        integer(11),
        integer(96),
        integer(-102),
        integer(-586),
        integer(986),
        integer(-0)
    )
)

//?
//? test: subset<integer>-4
//? description: ...

            Integer subset of bags:
            { "11",  "1",  "1",  "1",  "1",  "1",  "1",  "1" },
            { "-1", "11", "1024", "-11", "-1024", "0", "-512618", "1" }
        
//? expect: boolean(true)
//? source: ...

subset<integer>(
    bag<integer>(
        integer(11),
        integer(1),
        integer(1),
        integer(1),
        integer(1),
        integer(1),
        integer(1),
        integer(1)
    ),
    bag<integer>(
        integer(-1),
        integer(11),
        integer(1024),
        integer(-11),
        integer(-1024),
        integer(0),
        integer(-512618),
        integer(1)
    )
)

