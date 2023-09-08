#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_set_equals.as
//? customPurpose: Part of core function tests
//? description: Tests for set_equals<string> function.
//? sourceType: script
//?
//? test: set_equals<string>-1
//? description: ...

            String set equals of bags:
            { },
            { }
        
//? expect: boolean(true)
//? source: ...

set_equals<string>(
    bag<string>(
    ),
    bag<string>(
    )
)

//?
//? test: set_equals<string>-2
//? description: ...

            String set equals of bags:
            { 'one', 'two', 'three' },
            { 'one', 'two', 'three' }
        
//? expect: boolean(true)
//? source: ...

set_equals<string>(
    bag<string>(
        string("one"),
        string("two"),
        string("three")
    ),
    bag<string>(
        string("one"),
        string("two"),
        string("three")
    )
)

//?
//? test: set_equals<string>-3
//? description: ...

            String set equals of bags:
            { 'one', 'two', 'three' },
            { 'two', 'three' }
        
//? expect: boolean(false)
//? source: ...

set_equals<string>(
    bag<string>(
        string("one"),
        string("two"),
        string("three")
    ),
    bag<string>(
        string("two"),
        string("three")
    )
)

//?
//? test: set_equals<string>-4
//? description: ...

            String set equals of bags:
            { 'one', 'three', 'one', 'three' },
            { 'one', 'one', 'one', 'three', 'three', 'three' }
        
//? expect: boolean(true)
//? source: ...

set_equals<string>(
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
        string("three"),
        string("three"),
        string("three")
    )
)

//?
//? test: set_equals<string>-5
//? description: ...

            String set equals of bags:
            {
                'two', 'four', 'six', 'eight', 'ten',
                'twelve', 'fourteen', 'sixteen', 'eighteen', 'twenty'
            },
            {
                'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'ten',
                'eleven', 'twelve', 'thirteen', 'fourteen', 'fifteen', 'sixteen', 'seventeen', 'eighteen', 'nineteen', 'twenty',
                'twenty-one', 'twenty-two', 'twenty-three', 'twenty-four', 'twenty-five'
            }
        
//? expect: boolean(false)
//? source: ...

set_equals<string>(
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

//?
//? test: set_equals<string>-6
//? description: ...

            String set equals of bags:
            {
                'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'ten',
                'eleven', 'twelve', 'thirteen', 'fourteen', 'fifteen', 'sixteen', 'seventeen', 'eighteen', 'nineteen', 'twenty',
                'twenty-one', 'twenty-two', 'twenty-three', 'twenty-four', 'twenty-five',
                'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'ten',
                'eleven', 'twelve', 'thirteen', 'fourteen', 'fifteen', 'sixteen', 'seventeen', 'eighteen', 'nineteen', 'twenty',
                'twenty-one', 'twenty-two', 'twenty-three', 'twenty-four', 'twenty-five'
            },
            {
                'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'ten',
                'eleven', 'twelve', 'thirteen', 'fourteen', 'fifteen', 'sixteen', 'seventeen', 'eighteen', 'nineteen', 'twenty',
                'twenty-one', 'twenty-two', 'twenty-three', 'twenty-four', 'twenty-five'
            }
        
//? expect: boolean(true)
//? source: ...

set_equals<string>(
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
        string("twenty-five"),
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

