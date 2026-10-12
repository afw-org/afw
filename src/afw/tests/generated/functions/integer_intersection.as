#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_intersection.as
//? customPurpose: Part of core function tests
//? description: Tests for intersection<integer> function.
//? sourceType: script
//?
//? test: intersection<integer>-1
//? description: ...

            Integer intersection of arrays:
            {
                '200', '11', '-11.2', '96', '99', '-102', '-586.5', '986', '387', '512618', '-512618', '200', 
                '11', '-11', '96', '99', '-102', '-586', '986', '-0.0', '0'
            },
            { '11', '1024', '-11', '-1024', '0' }
        
//? expect: ...

array<integer>(
    11,
    -11,
    0
)

//? source: ...

intersection<integer>(
    array<integer>(
        200,
        11,
        -11,
        96,
        99,
        -102,
        -586,
        986,
        387,
        512618,
        -512618,
        200,
        11,
        -11,
        96,
        99,
        -102,
        -586,
        986,
        -0,
        0
    ),
    array<integer>(
        11,
        1024,
        -11,
        -1024,
        0
    )
)

//?
//? test: intersection<integer>-2
//? description: ...

            Integer intersection of arrays:
            { '11', '1024', '-11', '-1024', '0', '512618', '1' },
            {
                '11', '-11.2', '96', '-102', '-586.5', '986', '387', '512618',
                '-512618', '11', '96', '-102', '-586', '986', '0'
            }
        
//? expect: ...

array<integer>(
    11,
    -11,
    0,
    -512618
)

//? source: ...

intersection<integer>(
    array<integer>(
        11,
        1024,
        -11,
        -1024,
        0,
        -512618,
        1
    ),
    array<integer>(
        11,
        -11,
        96,
        -102,
        -586,
        986,
        387,
        512618,
        -512618,
        11,
        96,
        -102,
        -586,
        986,
        -0
    )
)

//?
//? test: intersection<integer>-3
//? description: ...

            Integer intersection of arrays:
            { '11', '1024', '-11', '-1024', '0', '512618', '1' },
            { '11',  '-1' }
        
//? expect: ...

array<integer>(
    -1,
    11
)

//? source: ...

intersection<integer>(
    array<integer>(
        -1,
        11,
        1024,
        -11,
        -1024,
        0,
        -512618,
        1
    ),
    array<integer>(
        11,
        -1
    )
)

