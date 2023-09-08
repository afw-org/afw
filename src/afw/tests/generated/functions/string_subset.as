#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_subset.as
//? customPurpose: Part of core function tests
//? description: Tests for subset<string> function.
//? sourceType: script
//?
//? test: subset<string>-1
//? description: ...

            String subset of bags:
            { },
            { }
        
//? expect: boolean(true)
//? source: ...

subset<string>(
    bag<string>(
    ),
    bag<string>(
    )
)

//?
//? test: subset<string>-2
//? description: ...

            String subset of bags:
            { 'one', 'three', 'one', 'three' },
            { 'one', 'one', 'one', 'two', 'two', 'two', 'three', 'three', 'three' }
        
//? expect: boolean(true)
//? source: ...

subset<string>(
    bag<string>(
        string("one"),
        string("three"),
        string("one"),
        string("three")
    ),
    bag<string>(
        string("one"),
        string("one"),
        string("one"),
        string("two"),
        string("two"),
        string("two"),
        string("three"),
        string("three"),
        string("three")
    )
)

//?
//? test: subset<string>-3
//? description: ...

            String subset of bags:
            { 'one', 'two', 'three' },
            { 'four', 'five' }
        
//? expect: boolean(false)
//? source: ...

subset<string>(
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
//? test: subset<string>-4
//? description: ...

            String subset of bags:
            { 'one', 'two', 'three' },
            { 'one', 'two', 'four' }
        
//? expect: boolean(false)
//? source: ...

subset<string>(
    bag<string>(
        string("one"),
        string("two"),
        string("three")
    ),
    bag<string>(
        string("one"),
        string("two"),
        string("four")
    )
)

//?
//? test: subset<string>-5
//? description: ...

            String subset of bags:
            {
                'two', 'four', 'six', 'eight', 'ten',
                'twelve', 'fourteen', 'sixteen', 'eighteen', 'twenty'
            },
            {
                'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'ten',
                'eleven', 'twelve', 'thirteen', 'fourteen', 'fifteen', 'sixteen', 'seventeen', 'eighteen', 'nineteen', 'twenty',
                'twenty-one', 'twenty-two', 'twenty-three', 'twenty-four', 'twenty-five'
            }
        
//? expect: boolean(true)
//? source: ...

subset<string>(
    bag<string>(
        string("two"),
        string("four"),
        string("six"),
        string("eight"),
        string("ten"),
        string("twelve"),
        string("fourteen"),
        string("sixteen"),
        string("eighteen"),
        string("twenty")
    ),
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
    )
)

