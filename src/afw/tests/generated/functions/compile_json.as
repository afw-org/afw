#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/compile_json.as
//? customPurpose: Part of core function tests
//? description: Tests for compile_json function.
//? sourceType: expression
//?
//? test: compile_json-1
//? description: Test compile_json with an integer.
//? expect: integer(0)
//? source: ...

compile_json(
    string("0")
)

//?
//? test: compile_json-2
//? description: Test compile_json with a boolean true.
//? expect: boolean(true)
//? source: ...

compile_json(
    string("true")
)

//?
//? test: compile_json-3
//? description: Test compile_json with a string.
//? expect: string("xyz")
//? source: ...

compile_json(
    string("'xyz'")
)

//?
//? test: compile_json-4
//? skip: true
//? description: Test compile_json with a non-string (error).
//? expect: error
//? source: ...

compile_json(
    boolean(true)
)

