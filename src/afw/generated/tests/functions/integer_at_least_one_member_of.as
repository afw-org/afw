#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_at_least_one_member_of.as
//? customPurpose: Part of core function tests
//? description: Tests for at_least_one_member_of<integer> function.
//? sourceType: expression
//?
//? test: at_least_one_member_of<integer>-1
//? description: ...

            Integer at least one member of for bags:
            {
                "200", "11", "-11.2", "96", "99", "-102", "-586.5", "986", "387", "512618", "-512618", "200",
                "11", "-11", "96", "99", "-102", "-586", "986", "-0.0", "0"
            },
            { }
        
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<integer>(
    bag<integer>(
        integer(200),
        integer(11),
        integer(-11),
        integer(96),
        integer(99),
        integer(-102),
        integer(-586),
        integer(986),
        integer(387),
        integer(512618),
        integer(-512618),
        integer(200),
        integer(11),
        integer(-11),
        integer(96),
        integer(99),
        integer(-102),
        integer(-586),
        integer(986),
        integer(-0),
        integer(0)
    ),
    bag<integer>(
    )
)

//?
//? test: at_least_one_member_of<integer>-2
//? description: ...

            Integer at least one member of for bags:
            {
                "200", "11", "-11.2", "96", "99", "-102", "-586.5", "986", "387", "512618", "-512618", "200",
                "11", "-11", "96", "99", "-102", "-586", "986", "-0.0", "0"
            },
            { "1024", "-1024" }
        
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<integer>(
    bag<integer>(
        integer(200),
        integer(11),
        integer(-11),
        integer(96),
        integer(99),
        integer(-102),
        integer(-586),
        integer(986),
        integer(387),
        integer(512618),
        integer(-512618),
        integer(200),
        integer(11),
        integer(-11),
        integer(96),
        integer(99),
        integer(-102),
        integer(-586),
        integer(986),
        integer(-0),
        integer(0)
    ),
    bag<integer>(
        integer(1024),
        integer(-1024)
    )
)

//?
//? test: at_least_one_member_of<integer>-3
//? description: ...

            Integer at least one member of for bags:
            { "1024", "-1024", "512618", "1" },
            {
                "11", "-11.2", "96", "-102", "-586.5", "986", "387", "512618",
                "-512618", "11", "96", "-102", "-586", "986", "0"
            }
        
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<integer>(
    bag<integer>(
        integer(1024),
        integer(-1024),
        integer(-512618),
        integer(1)
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
//? test: at_least_one_member_of<integer>-4
//? description: ...

            Integer at least one member of for bags:
            { "11", "1024", "-11", "-1024", "0", "512618", "1" },
            { "512618",  "1" }
        
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<integer>(
    bag<integer>(
        integer(-1),
        integer(11),
        integer(1024),
        integer(-11),
        integer(-1024),
        integer(0),
        integer(-512618),
        integer(1)
    ),
    bag<integer>(
        integer(512618),
        integer(1)
    )
)

