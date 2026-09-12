#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/is_in.as
//? customPurpose: Part of core function tests
//? description: Edge-case tests for the is_in polymorphic function. Happy-path coverage lives in the per-dataType files (e.g. integer_is_in.as).
//? sourceType: script
//?
//? test: is_in-mismatched-types-false
//? description: ...

		is_in no longer requires the array's aggregate data type to match
		value's data type (#286). A mismatched, per-entry comparison simply
		returns false instead of throwing.

//? expect: boolean(false)
//? source: ...

is_in<integer>(
    integer(6),
    bag<string>(string("6"))
)

//?
//? test: is_in-untyped-empty-array-false
//? description: ...

		A generic untyped empty array ([]) has no aggregate data type, but
		is_in should still return false rather than throw (#286).

//? expect: boolean(false)
//? source: ...

is_in<integer>(
    integer(6),
    []
)
