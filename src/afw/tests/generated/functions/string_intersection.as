#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_intersection.as
//? customPurpose: Part of core function tests
//? description: Tests for intersection<string> function.
//? sourceType: script
//?
//? test: intersection<string>-1
//? description: ...

            String intersection of arrays:
            { },
            { }
        
//? expect: ...

array<string>(
)

//? source: ...

intersection<string>(
    array<string>(
    ),
    array<string>(
    )
)

//?
//? test: intersection<string>-2
//? description: ...

            String intersection of arrays:
            { 'one', 'two', 'three' },
            { 'one', 'three' }
        
//? expect: ...

array<string>(
    string("one"),
    string("three")
)

//? source: ...

intersection<string>(
    array<string>(
        string("one"),
        string("two"),
        string("three")
    ),
    array<string>(
        string("one"),
        string("three")
    )
)

//?
//? test: intersection<string>-3
//? description: ...

            String intersection of arrays:
            { 'one', 'two', 'three' },
            { 'four', 'five' }
        
//? expect: ...

array<string>(
)

//? source: ...

intersection<string>(
    array<string>(
        string("one"),
        string("two"),
        string("three")
    ),
    array<string>(
        string("four"),
        string("five")
    )
)

//?
//? test: intersection<string>-4
//? description: ...

            String intersection of arrays:
            { 'one', 'two', 'three' },
            { 'three', 'four' }
        
//? expect: ...

array<string>(
    string("three")
)

//? source: ...

intersection<string>(
    array<string>(
        string("one"),
        string("two"),
        string("three")
    ),
    array<string>(
        string("three"),
        string("four")
    )
)

//?
//? test: intersection<string>-5
//? description: ...

            String intersection of arrays:
            {
               'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'ten',
               'eleven', 'twelve', 'thirteen', 'fourteen', 'fifteen', 'sixteen', 'seventeen', 'eighteen', 'nineteen', 'twenty',
               'twenty-one', 'twenty-two', 'twenty-three', 'twenty-four', 'twenty-five'
            },
            {
               'two', 'four', 'six', 'eight', 'ten',
               'twelve', 'fourteen', 'sixteen', 'eighteen', 'twenty'
            }
        
//? expect: ...

array<string>(
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
)

//? source: ...

intersection<string>(
    array<string>(
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
    array<string>(
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
    )
)

