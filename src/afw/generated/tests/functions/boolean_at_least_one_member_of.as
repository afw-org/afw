#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/boolean_at_least_one_member_of.as
//? customPurpose: Part of core function tests
//? description: Tests for at_least_one_member_of<boolean> function.
//? sourceType: expression
//?
//? test: at_least_one_member_of<boolean>-1
//? description: ...

            Boolean at least one member of for bags:
            { },
            { }
        
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<boolean>(
    bag<boolean>(
    ),
    bag<boolean>(
    )
)

//?
//? test: at_least_one_member_of<boolean>-2
//? description: ...

            Boolean at least one member of for bags:
            { "true", "true", "false" },
            { "false", "true" }
        
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<boolean>(
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
//? test: at_least_one_member_of<boolean>-3
//? description: ...

            Boolean at least one member of for bags:
            { "false", "false", "false", "false" },
            { "false", "true" }
        
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<boolean>(
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
//? test: at_least_one_member_of<boolean>-4
//? description: ...

            Boolean at least one member of for bags:
            { "true" },
            { "false", "false", "false", "false" }
        
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<boolean>(
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

