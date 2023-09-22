#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/compile_json.as
//? customPurpose: Part of core function tests
//? description: Tests for compile_json function.
//? sourceType: script
//?
//? test: compile_json-1
//? description: Test compile_json with an integer.
//? expect: integer(0)
//? source: ...

compile_json(
    json("0")
)

//?
//? test: compile_json-2
//? description: Test polymorphic compile json with a boolean true.
//? expect: boolean(true)
//? source: ...

compile(
    json("true")
)

//?
//? test: compile_json-3
//? description: Test compile_json with a json.
//? expect: "xyz"
//? source: ...

compile_json(
    json("'xyz'")
)

//?
//? test: compile_json-4
//? skip: true
//? description: Test compile_json with a non-json (error).
//? expect: error
//? source: ...

compile_json(
    boolean(true)
)

