#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/boolean_intersection.as
//? customPurpose: Part of core function tests
//? description: Tests for intersection<boolean> function.
//? sourceType: script
//?
//? test: intersection<boolean>-1
//? description: ...

            Boolean intersection of two empty bags.
        
//? expect: ...

bag<boolean>(
)

//? source: ...

intersection<boolean>(
    bag<boolean>(
    ),
    bag<boolean>(
    )
)

//?
//? test: intersection<boolean>-2
//? description: ...

            Boolean intersection of bags:
            { 'true', 'true', 'false' },
            { 'false', 'true' }
        
//? expect: ...

bag<boolean>(
    boolean(true),
    boolean(false)
)

//? source: ...

intersection<boolean>(
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
//? test: intersection<boolean>-3
//? description: ...

            Boolean intersection of bags:
            { 'false', 'false', 'false', 'false' },
            { 'false', 'true' }
        
//? expect: ...

bag<boolean>(
    boolean(false)
)

//? source: ...

intersection<boolean>(
    bag<boolean>(
        boolean(false),
        boolean(false),
        boolean(false),
        boolean(false)
    ),
    bag<boolean>(
        boolean(false),
        boolean(true)
    )
)

//?
//? test: intersection<boolean>-4
//? description: ...

            Boolean intersection of bags:
            { 'true' },
            { 'false', 'false', 'false', 'false' }
        
//? expect: ...

bag<boolean>(
)

//? source: ...

intersection<boolean>(
    bag<boolean>(
        boolean(true)
    ),
    bag<boolean>(
        boolean(false),
        boolean(false),
        boolean(false),
        boolean(false)
    )
)

