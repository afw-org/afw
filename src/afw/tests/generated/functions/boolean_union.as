#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/boolean_union.as
//? customPurpose: Part of core function tests
//? description: Tests for union<boolean> function.
//? sourceType: expression
//?
//? test: union<boolean>-1
//? description: ...

            Boolean union for bags:
            { },
            { }
        
//? expect: ...

bag<boolean>(
)

//? source: ...

union<boolean>(
    bag<boolean>(
    ),
    bag<boolean>(
    )
)

//?
//? test: union<boolean>-2
//? description: ...

            Boolean union for bags:
            { 'true', 'true', 'false' },
            { 'false', 'true' }
        
//? expect: ...

bag<boolean>(
    boolean(true),
    boolean(false)
)

//? source: ...

union<boolean>(
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
//? test: union<boolean>-3
//? description: ...

            Boolean union for bags:
            { 'false', 'false', 'false', 'true' },
            { }
        
//? expect: ...

bag<boolean>(
    boolean(false),
    boolean(true)
)

//? source: ...

union<boolean>(
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
//? test: union<boolean>-4
//? description: ...

            Boolean union for bags:
            { 'false' },
            { 'false', 'false', 'false', 'false' }
        
//? expect: ...

bag<boolean>(
    boolean(false)
)

//? source: ...

union<boolean>(
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
//? test: union<boolean>-5
//? description: ...

            Boolean union for bags:
            { 'true', 'true', 'true', 'true', 'true' },
            { 'true', 'true', 'true', 'true', 'true', 'true', 'true', 'true', 'true', 'true' }
        
//? expect: ...

bag<boolean>(
    boolean(true)
)

//? source: ...

union<boolean>(
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
        boolean(true),
        boolean(true),
        boolean(true),
        boolean(true),
        boolean(true)
    )
)

