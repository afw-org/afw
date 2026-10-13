#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_union.as
//? customPurpose: Part of core function tests
//? description: Tests for union<string> function.
//? sourceType: script
//?
//? test: union<string>-1
//? description: ...

            String union of two empty arrays.
        
//? expect: ...

array<string>(
)

//? source: ...

union<string>(
    array<string>(
    ),
    array<string>(
    )
)

//?
//? test: union<string>-2
//? description: ...

            String union of arrays:
            { 'one', 'two', 'three' },
            { 'one', 'three' }
        
//? expect: ...

array<string>(
    string("one"),
    string("two"),
    string("three")
)

//? source: ...

union<string>(
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
//? test: union<string>-3
//? description: ...

            String union of arrays:
            { 'one', 'two', 'three' },
            { 'four', 'five' }
        
//? expect: ...

array<string>(
    string("one"),
    string("two"),
    string("three"),
    string("four"),
    string("five")
)

//? source: ...

union<string>(
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
//? test: union<string>-4
//? description: ...

            String intersection of arrays:
            { 'one', 'two', 'three' },
            { 'three', 'four' }
        
//? expect: ...

array<string>(
    string("one"),
    string("two"),
    string("three"),
    string("four")
)

//? source: ...

union<string>(
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
//? test: union<string>-5
//? description: ...

            String union of arrays:
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

//? source: ...

union<string>(
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

