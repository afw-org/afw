#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_normalize_to_lower_case.as
//? customPurpose: Part of core function tests
//? description: Tests for normalize_to_lower_case<string> function.
//? sourceType: script
//?
//? test: normalize_to_lower_case<string>-1
//? description: aBCDEFghijkLMNOpqrsTUVwxyz
//? expect: string("abcdefghijklmnopqrstuvwxyz")
//? source: ...

normalize_to_lower_case<string>(
    string("aBCDEFghijkLMNOpqrsTUVwxyz")
)

//?
//? test: normalize_to_lower_case<string>-2
//? description: aBC4
//? expect: string("abc4")
//? source: ...

normalize_to_lower_case<string>(
    string("aBC4")
)

//?
//? test: normalize_to_lower_case<string>-3
//? description: Invalid arguments test.
//? expect: error
//? source: normalize_to_lower_case<string>()
