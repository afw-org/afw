#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/perform.as
//? customPurpose: Part of core function tests
//? description: Tests for perform function.
//? sourceType: script
//?
//? test: perform-1
//? description: Test perform function with single top-level action.
//? expect: ...

object({
    "result": 2,
    "resultDataType": "integer",
    "status": "success"
})

//? source: ...

perform(
    object({
    "function": "add",
    "values": [
        1,
        1
    ]
})
)

//?
//? test: perform-2
//? description: Test perform function with actions property
//? expect: ...

object({
    "actions": [
        {
            "result": 2,
            "resultDataType": "integer"
        }
    ],
    "status": "success"
})

//? source: ...

perform(
    object({
    "actions": [
        {
            "function": "add",
            "values": [
                1,
                1
            ]
        }
    ]
})
)

//?
//? test: perform-3
//? description: Test perform function with two actions
//? expect: ...

object({
    "actions": [
        {
            "result": 2,
            "resultDataType": "integer"
        },
        {
            "result": 1,
            "resultDataType": "integer"
        }
    ],
    "status": "success"
})

//? source: ...

perform(
    object({
    "actions": [
        {
            "function": "add",
            "values": [
                1,
                1
            ]
        },
        {
            "function": "subtract",
            "arg1": 2,
            "arg2": 1
        }
    ]
})
)

