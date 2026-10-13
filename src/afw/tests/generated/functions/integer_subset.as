#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_subset.as
//? customPurpose: Part of core function tests
//? description: Tests for subset<integer> function.
//? sourceType: script
//?
//? test: subset<integer>-1
//? description: ...

            Integer subset of arrays:
            { },
            { }
        
//? expect: boolean(true)
//? source: ...

subset<integer>(
    array<integer>(
    ),
    array<integer>(
    )
)

//?
//? test: subset<integer>-2
//? description: ...

            Integer subset of arrays:
            { '11', '-11', '-1024', '0' },
            {
                '200', '11', '-11.2', '96', '99', '-102', '-586.5', '387', '512618', '-512618', '200',
                '11', '-11', '96', '-1024', '99', '-102', '-586', '986', '-0.0', '0'
            }
        
//? expect: boolean(true)
//? source: ...

subset<integer>(
    array<integer>(
        11,
        -11,
        -1024,
        0,
        -0,
        986
    ),
    array<integer>(
        200,
        11,
        -11,
        96,
        99,
        -102,
        -586,
        387,
        512618,
        -512618,
        200,
        11,
        -11,
        96,
        -1024,
        99,
        -102,
        -586,
        986,
        -0,
        0
    )
)

//?
//? test: subset<integer>-3
//? description: ...

            Integer subset of arrays:
            { '11', '-11', '0', '-512618', '1024' },
            {
                '11', '-11.2', '96', '-102', '-586.5', '986', '387', '512618',
                '-512618', '11', '96', '-102', '-586', '986', '0'
            }
        
//? expect: boolean(false)
//? source: ...

subset<integer>(
    array<integer>(
        11,
        -11,
        0,
        -512618,
        1024
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
//? test: subset<integer>-4
//? description: ...

            Integer subset of arrays:
            { '11',  '1',  '1',  '1',  '1',  '1',  '1',  '1' },
            { '-1', '11', '1024', '-11', '-1024', '0', '-512618', '1' }
        
//? expect: boolean(true)
//? source: ...

subset<integer>(
    array<integer>(
        11,
        1,
        1,
        1,
        1,
        1,
        1,
        1
    ),
    array<integer>(
        -1,
        11,
        1024,
        -11,
        -1024,
        0,
        -512618,
        1
    )
)

