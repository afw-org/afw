#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<integer> function.
//? sourceType: script
//?
//? test: one_and_only<integer>-1
//? description: ...

            Integer one and only for bag:
            { 1 }
        
//? expect: 1
//? source: ...

one_and_only<integer>(
    array<integer>(
        1
    )
)

//?
//? test: one_and_only<integer>-2
//? description: ...

            Integer one and only for bag:
            { 1, 2, 3, 4, 5 }
        
//? expect: error
//? source: ...

one_and_only<integer>(
    array<integer>(
        1,
        2,
        3,
        4,
        5
    )
)

//?
//? test: one_and_only<integer>-3
//? description: ...

            Integer one and only for bag:
            { }
        
//? expect: error
//? source: ...

one_and_only<integer>(
    array<integer>(
    )
)

