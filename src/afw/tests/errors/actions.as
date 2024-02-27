#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: actions.as
//? customPurpose: Part of core function tests
//? description: Exercises errors that can occur with actions
//? sourceType: script
//?
//? test: minArgs_last_parameter
//? description: minArgs must be on last parameter
//? expect: "minArgs must be on last parameter"
//? skip: true
//? source: ...

/* \fixme */


//? test: missing_parameter
//? description: If arg is NULL and not optional, throw error
//? expect: "Missing parameter 'arg2'"
//? source: ...

const result = perform({
    function: "subtract",
    arg1: 3
});

assert(result.status === "error");

return result.error.message;


//? test: function_return_fully_evaluated
//? description: Make sure function result is evaluated
//? expect: "Function %s returned a value that is not evaluated. (%s)"
//? skip: true
//? source: ...

/* \fixme */


//? test: actions_or_function_property
//? description: Either actions or function property must be specified
//? expect: "Either actions or function property must be specified"
//? source: ...

const result = perform({
    arg1: 3
});

assert(result.status === "error");

return result.error.message;


//? test: unknown_function
//? description: Unknown function specified
//? expect: "Unknown function 'blah'"
//? source: ...

const result = perform({
    function: "blah"
});

assert(result.status === "error");

return result.error.message;


//? test: property_of_actions_missing_or_invalid
//? description: Property of actions is missing or invalid
//? expect: "Property 'actions' of actions is missing or invalid"
//? source: ...

const result = perform({
    actions: "blah"
});

assert(result.status === "error");

return result.error.message;


//? test: entries_in_actions_array
//? description: Entries in actions array must be objects
//? expect: "Entries in actions array must be objects"
//? source: ...

const result = perform({
    actions: [
        "blah"
    ]
});

assert(result.status === "error");

return result.error.message;


//? test: property_of_action_missing_or_invalid
//? description: Property of action is missing or invalid
//? expect: "Property 'function' of action 1 is missing or invalid"
//? source: ...

const result = perform({
    actions: [
        {
            blah: "abc"
        }
    ]
});

assert(result.status === "error");

return result.error.message;


//? test: unknown_function_in_action
//? description: Unknown function in action
//? expect: "Unknown function 'blah' in action 1"
//? source: ...

const result = perform({
    actions: [
        {
            function: "blah"
        }
    ]
});

assert(result.status === "error");

return result.error.message;