#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/concat.as
//? customPurpose: Part of core function tests
//? description: Tests for concat function.
//? sourceType: expression
//?
//? test: concat-1
//? description: string concatenate test a bc def 
//? expect: string("abcdef")
//? source: ...

concat(
    string("a"),
    string("bc"),
    string("def")
)

