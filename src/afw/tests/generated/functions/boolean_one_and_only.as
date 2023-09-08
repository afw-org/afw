#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/boolean_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<boolean> function.
//? sourceType: script
//?
//? test: one_and_only<boolean>-1
//? description: ...

            Boolean one and only for bag:
            { 'true' }
        
//? expect: boolean(true)
//? source: ...

one_and_only<boolean>(
    bag<boolean>(
        boolean(true)
    )
)

//?
//? test: one_and_only<boolean>-2
//? description: ...

            Boolean one and only for bag:
            { 'true', 'false', 'true', 'false', true' }
        
//? expect: error
//? source: ...

one_and_only<boolean>(
    bag<boolean>(
        boolean(true),
        boolean(false),
        boolean(true),
        boolean(false),
        boolean(true)
    )
)

//?
//? test: one_and_only<boolean>-3
//? description: ...

            Boolean one and only for bag:
            { }
        
//? expect: error
//? source: ...

one_and_only<boolean>(
    bag<boolean>(
    )
)

