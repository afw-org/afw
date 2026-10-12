#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/boolean_union.as
//? customPurpose: Part of core function tests
//? description: Tests for union<boolean> function.
//? sourceType: script
//?
//? test: union<boolean>-1
//? description: ...

            Boolean union for bags:
            { },
            { }
        
//? expect: ...

array<boolean>(
)

//? source: ...

union<boolean>(
    array<boolean>(
    ),
    array<boolean>(
    )
)

//?
//? test: union<boolean>-2
//? description: ...

            Boolean union for bags:
            { 'true', 'true', 'false' },
            { 'false', 'true' }
        
//? expect: ...

array<boolean>(
    boolean(true),
    boolean(false)
)

//? source: ...

union<boolean>(
    array<boolean>(
        boolean(true),
        boolean(true),
        boolean(false)
    ),
    array<boolean>(
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

array<boolean>(
    boolean(false),
    boolean(true)
)

//? source: ...

union<boolean>(
    array<boolean>(
        boolean(false),
        boolean(false),
        boolean(false),
        boolean(true)
    ),
    array<boolean>(
    )
)

//?
//? test: union<boolean>-4
//? description: ...

            Boolean union for bags:
            { 'false' },
            { 'false', 'false', 'false', 'false' }
        
//? expect: ...

array<boolean>(
    boolean(false)
)

//? source: ...

union<boolean>(
    array<boolean>(
        boolean(false)
    ),
    array<boolean>(
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

array<boolean>(
    boolean(true)
)

//? source: ...

union<boolean>(
    array<boolean>(
        boolean(true),
        boolean(true),
        boolean(true),
        boolean(true),
        boolean(true)
    ),
    array<boolean>(
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

