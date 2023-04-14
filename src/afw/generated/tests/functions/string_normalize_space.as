#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_normalize_space.as
//? customPurpose: Part of core function tests
//? description: Tests for normalize_space<string> function.
//? sourceType: expression
//?
//? test: normalize_space<string>-1
//? description: One
//? expect: string("This is a sentence.")
//? source: ...

normalize_space<string>(
    string("This     is            a                  sentence.         ")
)

//?
//? test: normalize_space<string>-2
//? description: Two
//? expect: string("This is a sentence.")
//? source: ...

normalize_space<string>(
    string("This\n      is            \n      a                  \n      sentence.")
)

//?
//? test: normalize_space<string>-3
//? description: Two
//? expect: string("This is a sentence.")
//? source: ...

normalize_space<string>(
    string("\n        This                                                 is\n        a\n        sentence.\n      ")
)

