#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/boolean_set_equals.as
//? customPurpose: Part of core function tests
//? description: Tests for set_equals<boolean> function.
//? sourceType: expression
//?
//? test: set_equals<boolean>-1
//? description: ...

            Boolean set equals for bags:
            { },
            { }
        
//? expect: boolean(true)
//? source: ...

set_equals<boolean>(
    bag<boolean>(
    ),
    bag<boolean>(
    )
)

//?
//? test: set_equals<boolean>-2
//? description: ...

            Boolean set equals for bags:
            { 'true', 'true', 'false' },
            { 'false', 'true' }
        
//? expect: boolean(true)
//? source: ...

set_equals<boolean>(
    bag<boolean>(
        boolean(true),
        boolean(true),
        boolean(false)
    ),
    bag<boolean>(
        boolean(false),
        boolean(true)
    )
)

//?
//? test: set_equals<boolean>-3
//? description: ...

            Boolean set equals for bags:
            { 'false', 'false', 'false', 'true' },
            { }
        
//? expect: boolean(false)
//? source: ...

set_equals<boolean>(
    bag<boolean>(
        boolean(false),
        boolean(false),
        boolean(false),
        boolean(true)
    ),
    bag<boolean>(
    )
)

//?
//? test: set_equals<boolean>-4
//? description: ...

            Boolean set equals for bags:
            { 'false', 'false', 'false', 'false', 'false', 'false', 'false', 'false', 'false' },
            { 'false', 'false', 'false', 'false' }
        
//? expect: boolean(true)
//? source: ...

set_equals<boolean>(
    bag<boolean>(
        boolean(false),
        boolean(false),
        boolean(false),
        boolean(false),
        boolean(false),
        boolean(false),
        boolean(false),
        boolean(false),
        boolean(false)
    ),
    bag<boolean>(
        boolean(false),
        boolean(false),
        boolean(false),
        boolean(false)
    )
)

//?
//? test: set_equals<boolean>-5
//? description: ...

            Boolean set equals for bags:
            { 'true', 'true', 'true', 'true', 'true' },
            { 'true', 'true', 'true', 'true', 'true', 'false', 'false', 'false', 'false', 'false' }
        
//? expect: boolean(false)
//? source: ...

set_equals<boolean>(
    bag<boolean>(
        boolean(true),
        boolean(true),
        boolean(true),
        boolean(true),
        boolean(true)
    ),
    bag<boolean>(
        boolean(true),
        boolean(true),
        boolean(true),
        boolean(true),
        boolean(true),
        boolean(false),
        boolean(false),
        boolean(false),
        boolean(false),
        boolean(false)
    )
)

