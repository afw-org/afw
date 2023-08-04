#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_intersection.as
//? customPurpose: Part of core function tests
//? description: Tests for intersection<string> function.
//? sourceType: expression
//?
//? test: intersection<string>-1
//? description: ...

            String intersection of bags:
            { },
            { }
        
//? expect: ...

bag<string>(
)

//? source: ...

intersection<string>(
    bag<string>(
    ),
    bag<string>(
    )
)

//?
//? test: intersection<string>-2
//? description: ...

            String intersection of bags:
            { "one", "two", "three" },
            { "one", "three" }
        
//? expect: ...

bag<string>(
    string("one"),
    string("three")
)

//? source: ...

intersection<string>(
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
//? test: intersection<string>-3
//? description: ...

            String intersection of bags:
            { "one", "two", "three" },
            { "four", "five" }
        
//? expect: ...

bag<string>(
)

//? source: ...

intersection<string>(
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
//? test: intersection<string>-4
//? description: ...

            String intersection of bags:
            { "one", "two", "three" },
            { "three", "four" }
        
//? expect: ...

bag<string>(
    string("three")
)

//? source: ...

intersection<string>(
    bag<string>(
        string("one"),
        string("two"),
        string("three")
    ),
    bag<string>(
        string("three"),
        string("four")
    )
)

//?
//? test: intersection<string>-5
//? description: ...

            String intersection of bags:
            {
               "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten",
               "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen", "twenty",
               "twenty-one", "twenty-two", "twenty-three", "twenty-four", "twenty-five"
            },
            {
               "two", "four", "six", "eight", "ten",
               "twelve", "fourteen", "sixteen", "eighteen", "twenty"
            }
        
//? expect: ...

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
)

//? source: ...

intersection<string>(
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

