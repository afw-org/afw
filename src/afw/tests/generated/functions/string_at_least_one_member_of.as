#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_at_least_one_member_of.as
//? customPurpose: Part of core function tests
//? description: Tests for at_least_one_member_of<string> function.
//? sourceType: script
//?
//? test: at_least_one_member_of<string>-1
//? description: ...

            String at least one member of for two empty bags.
        
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<string>(
    bag<string>(
    ),
    bag<string>(
    )
)

//?
//? test: at_least_one_member_of<string>-2
//? description: ...

            String at least one member of for bags:
            { 'one', 'two', 'three' },
            { 'one', 'three' }
        
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<string>(
    bag<string>(
        string("one"),
        string("two"),
        string("three")
    ),
    bag<string>(
        string("one"),
        string("three")
    )
)

//?
//? test: at_least_one_member_of<string>-3
//? description: ...

            String at least one member of for bags:
            { 'one', 'two', 'three' },
            { 'four', 'five' }
        
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<string>(
    bag<string>(
        string("one"),
        string("two"),
        string("three")
    ),
    bag<string>(
        string("four"),
        string("five")
    )
)

//?
//? test: at_least_one_member_of<string>-4
//? description: ...

            String intersection of bags:
            {
                'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'ten',
                'eleven', 'twelve', 'thirteen', 'fourteen', 'fifteen', 'sixteen', 'seventeen', 'eighteen', 'nineteen', 'twenty',
                'twenty-one', 'twenty-two', 'twenty-three', 'twenty-four', 'twenty-five'
            },
            { 'twenty-five' }
        
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<string>(
    bag<string>(
        string("one"),
        string("two"),
        string("three"),
        string("four"),
        string("five"),
        string("six"),
        string("seven"),
        string("eight"),
        string("nine"),
        string("ten"),
        string("eleven"),
        string("twelve"),
        string("thirteen"),
        string("fourteen"),
        string("fifteen"),
        string("sixteen"),
        string("seventeen"),
        string("eighteen"),
        string("nineteen"),
        string("twenty"),
        string("twenty-one"),
        string("twenty-two"),
        string("twenty-three"),
        string("twenty-four"),
        string("twenty-five")
    ),
    bag<string>(
        string("twenty-five")
    )
)

