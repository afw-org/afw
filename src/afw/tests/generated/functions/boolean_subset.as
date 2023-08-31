#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/boolean_subset.as
//? customPurpose: Part of core function tests
//? description: Tests for subset<boolean> function.
//? sourceType: expression
//?
//? test: subset<boolean>-1
//? description: ...

            Boolean subset for bags:
            { },
            { }
        
//? expect: boolean(true)
//? source: ...

subset<boolean>(
    bag<boolean>(
    ),
    bag<boolean>(
    )
)

//?
//? test: subset<boolean>-2
//? description: ...

            Boolean subset for bags:
            { 'true', 'true', 'false' },
            { 'false', 'true' }
        
//? expect: boolean(true)
//? source: ...

subset<boolean>(
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
//? test: subset<boolean>-3
//? description: ...

            Boolean subset for bags:
            { 'false', 'false', 'false', 'true' },
            { }
        
//? expect: boolean(false)
//? source: ...

subset<boolean>(
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
//? test: subset<boolean>-4
//? description: ...

            Boolean subset for bags:
            { 'false' },
            { 'false', 'false', 'false', 'false' }
        
//? expect: boolean(true)
//? source: ...

subset<boolean>(
    bag<boolean>(
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
//? test: subset<boolean>-5
//? description: ...

            Boolean subset for bags:
            { 'true', 'true', 'true', 'true', 'true' },
            { 'true', 'true', 'true', 'true', 'true', 'false', 'false', 'false', 'false', 'false' }
        
//? expect: boolean(true)
//? source: ...

subset<boolean>(
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

