#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_is_in.as
//? customPurpose: Part of core function tests
//? description: Tests for is_in<string> function.
//? sourceType: script
//?
//? test: is_in<string>-1
//? description: ...

		String 'one' is in empty bag.
		Should return boolean false.
	
//? expect: boolean(false)
//? source: ...

is_in<string>(
    string("one"),
    array<string>(
    )
)

//?
//? test: is_in<string>-2
//? description: ...

		String 'two' is in bag: 'one', 'two', 'three'.
		Should return boolean true.
	
//? expect: boolean(true)
//? source: ...

is_in<string>(
    string("two"),
    array<string>(
        string("one"),
        string("two"),
        string("three")
    )
)

//?
//? test: is_in<string>-3
//? description: ...

		String 'four' is in bag: 'one', 'two', 'three'.
		Should return boolean false.
	
//? expect: boolean(false)
//? source: ...

is_in<string>(
    string("four"),
    array<string>(
        string("one"),
        string("two"),
        string("three")
    )
)

//?
//? test: is_in<string>-4
//? description: ...

		String 'four' is in bag: 'one', 'two', 'three'.
		Should return boolean false.
	
//? expect: boolean(true)
//? source: ...

is_in<string>(
    string("four"),
    array<string>(
        string("one"),
        string("two"),
        string("three"),
        string("four")
    )
)

