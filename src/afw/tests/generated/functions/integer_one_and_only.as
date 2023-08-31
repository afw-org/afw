#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<integer> function.
//? sourceType: expression
//?
//? test: one_and_only<integer>-1
//? description: ...

            Integer one and only for bag:
            { 1 }
        
//? expect: integer(1)
//? source: ...

one_and_only<integer>(
    bag<integer>(
        integer(1)
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
    bag<integer>(
        integer(1),
        integer(2),
        integer(3),
        integer(4),
        integer(5)
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
    bag<integer>(
    )
)

