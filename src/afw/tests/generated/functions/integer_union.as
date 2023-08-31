#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_union.as
//? customPurpose: Part of core function tests
//? description: Tests for union<integer> function.
//? sourceType: expression
//?
//? test: union<integer>-1
//? description: ...

            Integer union of bags:
            {
                '200', '11', '-11.2', '96', '99', '-102', '-586.5', '986', '387', '512618', '-512618', '200',
                '11', '-11', '96', '99', '-102', '-586', '986', '-0.0', '0'
            },
            { '11', '1024', '-11', '-1024', '0' }
        
//? expect: ...

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
    integer(0),
    integer(1024),
    integer(-1024)
)

//? source: ...

union<integer>(
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
        integer(11),
        integer(1024),
        integer(-11),
        integer(-1024),
        integer(0)
    )
)

//?
//? test: union<integer>-2
//? description: ...

            Integer union of bags:
            { '11', '1024', '-11', '-1024', '0', '-512618', '1' },
            {
                '11', '-11.2', '96', '-102', '-586.5', '986', '387', '512618',
                '-512618', '11', '96', '-102', '-586', '986', '0'
            }
        
//? expect: ...

bag<integer>(
    integer(11),
    integer(1024),
    integer(-11),
    integer(-1024),
    integer(0),
    integer(-512618),
    integer(1),
    integer(96),
    integer(-102),
    integer(-586),
    integer(986),
    integer(387),
    integer(512618)
)

//? source: ...

union<integer>(
    bag<integer>(
        integer(11),
        integer(1024),
        integer(-11),
        integer(-1024),
        integer(0),
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
//? test: union<integer>-3
//? description: ...

            Integer union of bags:
            { '-1', '11', '1024', '-11', '-1024', '0', '-512618', '1' },
            { '11',  '-1' }
        
//? expect: ...

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

//? source: ...

union<integer>(
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
        integer(11),
        integer(-1)
    )
)

