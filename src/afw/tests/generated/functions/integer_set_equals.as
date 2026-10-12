#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_set_equals.as
//? customPurpose: Part of core function tests
//? description: Tests for set_equals<integer> function.
//? sourceType: script
//?
//? test: set_equals<integer>-1
//? description: ...

            Integer set equals of bags:
            { },
            { }
        
//? expect: boolean(true)
//? source: ...

set_equals<integer>(
    array<integer>(
    ),
    array<integer>(
    )
)

//?
//? test: set_equals<integer>-2
//? description: ...

            Integer set equals of bags:
            {
                '200', '11', '-11.2', '96', '99', '-102', '-586.5', '387', '512618', '-512618', '200',
                '11', '-11', '96', '-1024', '99', '-102', '-586', '986', '-0.0', '0'
            },
            {
                '200', '11', '-11.2', '96', '99', '-102', '-586.5', '387', '512618', '-512618', '200',
                '11', '-11', '96', '-1024', '99', '-102', '-586', '986', '-0.0', '0'
            }
        
//? expect: boolean(true)
//? source: ...

set_equals<integer>(
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
//? test: set_equals<integer>-3
//? description: ...

            Integer subset of bags:
            { '11', '-11', '0', '-512618', '1024' },
            { '11', '-11', '0', '-512618', '1024', '986' }
        
//? expect: boolean(false)
//? source: ...

set_equals<integer>(
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
        0,
        -512618,
        1024,
        986
    )
)

//?
//? test: set_equals<integer>-4
//? description: ...

            Integer set equals of bags:
            { '11',  '1',  '1',  '1',  '1',  '1',  '1',  '1', '-1', '11', '1024', '-11', '-1024', '0', '-512618', '1' },
            { '-1', '11', '1024', '-11', '-1024', '0', '-512618', '1' }
        
//? expect: boolean(true)
//? source: ...

set_equals<integer>(
    array<integer>(
        11,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
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

