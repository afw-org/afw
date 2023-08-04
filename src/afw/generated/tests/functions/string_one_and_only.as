#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<string> function.
//? sourceType: expression
//?
//? test: one_and_only<string>-1
//? description: ...

            String one and only for bag:
            { "one" }
        
//? expect: string("one")
//? source: ...

one_and_only<string>(
    bag<string>(
        string("one")
    )
)

//?
//? test: one_and_only<string>-2
//? description: ...

            String one and only for bag:
            { "one", "two", "three", "four", five" }
        
//? expect: error
//? source: ...

one_and_only<string>(
    bag<string>(
        string("one"),
        string("two"),
        string("three"),
        string("four"),
        string("five")
    )
)

//?
//? test: one_and_only<string>-3
//? description: ...

            String one and only for bag:
            { }
        
//? expect: error
//? source: ...

one_and_only<string>(
    bag<string>(
    )
)

