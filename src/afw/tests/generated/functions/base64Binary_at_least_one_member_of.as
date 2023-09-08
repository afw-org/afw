#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/base64Binary_at_least_one_member_of.as
//? customPurpose: Part of core function tests
//? description: Tests for at_least_one_member_of<base64Binary> function.
//? sourceType: script
//?
//? test: at_least_one_member_of<base64Binary>-1
//? description: ...

            base64Binary at least one member of test for bags:
            { },
            { }
        
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<base64Binary>(
    bag<base64Binary>(
    ),
    bag<base64Binary>(
    )
)

//?
//? test: at_least_one_member_of<base64Binary>-2
//? description: ...

            base64Binary at least one member of test for bags:
            { 0FA8, 0 fa8 0f+9, 0F+40A== },
            { 0FB8, 0 FB8 0F+9, 0F+41A== }
        
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<base64Binary>(
    bag<base64Binary>(
        base64Binary("0fa8"),
        base64Binary("0fa80f+9"),
        base64Binary("0F+40A==")
    ),
    bag<base64Binary>(
        base64Binary("0FB8"),
        base64Binary("0FB80F+9"),
        base64Binary("0F+41A==")
    )
)

//?
//? test: at_least_one_member_of<base64Binary>-3
//? description: ...

            base64Binary at least one member of case sensitivity test for bags:
            { 0F+41A==, 0 fa8 0f+9, 0fb8 },
            { 0FB8, 0 FB8 0F+9, 0F+40A== }
        
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<base64Binary>(
    bag<base64Binary>(
        base64Binary("0F+41A=="),
        base64Binary("0fa80f+9"),
        base64Binary("0FB8")
    ),
    bag<base64Binary>(
        base64Binary("0FB8"),
        base64Binary("0FB80F+9"),
        base64Binary("0F+40A==")
    )
)

//?
//? test: at_least_one_member_of<base64Binary>-4
//? description: ...

            base64Binary at least one member of case sensitivity test for bags:
            { 0F+41A==, 0 fb8 0f+9, 0FA8 },
            { 0FB8, 0 FB8 0F+9, 0F+40A== }
        
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<base64Binary>(
    bag<base64Binary>(
        base64Binary("0F+41A=="),
        base64Binary("0fb80f+9"),
        base64Binary("0FA8")
    ),
    bag<base64Binary>(
        base64Binary("0FA8"),
        base64Binary("0FB80F+9"),
        base64Binary("0F+40A==")
    )
)

//?
//? test: at_least_one_member_of<base64Binary>-5
//? skip: true
//? description: ...

            base64Binary at least one member of invalid argument test for bags:
            { 0f+40a== },
            { 0FB8, 0 FB8 0F+9, 0F+40A== }
        
//? expect: error
//? source: ...

at_least_one_member_of<base64Binary>(
    bag<base64Binary>(
        base64Binary("0f+40a==")
    ),
    bag<base64Binary>(
        base64Binary("0FB8"),
        base64Binary("0FB80F+9"),
        base64Binary("0f+40a==")
    )
)

