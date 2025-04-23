#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: substitution.as
//? customPurpose: Part of authorization tests
//? description: Test substitution
//? sourceType: script
//?
//? test: hello_world_mixed_template_string
//? description: Mixed template string
//? expect: "Hello World!"
//? source: ...

return `H#{'ello ' + 'Wor'}ld${'!'}`;

//?
//? test: hello_world_compile_time_value
//? description: Compile time substitution value
//? expect: "Hello World!"
//? source: ...

return #{"Hello World!"};

//?
//? test: hello_world_eval_template
//? description: Eval mixed template string
//? expect: "Hello World!"
//? source: ...

const source = template("H#{'ello ' + 'Wor'}ld${'!'}");

return eval(source);

//?
//? test: hello_world_return_app_helloWorld
//? description: Return app::helloWorld string
//? expect: "Hello World!"
//? source: ...

return app::helloWorld;

//?
//? test: hello_world_return_app_compileTime
//? description: Return app::compileTime string
//? expect: "Hello World!"
//? source: ...

return app::compileTime;

//?
//? test: hello_world_return_app_evalTime
//? description: Return app::evalTime string
//? expect: "Hello World!"
//? source: ...

return app::evalTime;

//?
//? test: hello_world_assign_app_helloWorld
//? description: Assign app::helloWorld to x then return x
//? expect: "Hello World!"
//? source: ...

const x = app::helloWorld;
return x;

//?
//? test: hello_world_assign_app_compileTime
//? description: Assign app::compileTime to x then return x
//? expect: "Hello World!"
//? source: ...

const x = app::compileTime;
return x;

//?
//? test: hello_world_assign_app_evalTime
//? description: Assign app::evalTime to x then return x
//? expect: "Hello World!"
//? source: ...

const x = app::evalTime;
return x;

//?
//? test: hello_world_call_app_helloWorldFunction
//? description: Call function app::helloWorldFunction()
//? expect: "Hello World!"
//? source: ...

return app::helloWorldFunction();

//?
//? test: hello_world_call_loaded_function
//? description: Call function loaded from file
//? expect: "Hello World!"
//? source: ...

const x = eval_from_file('includes/a.as');
return x();

//?
//? test: hello_world_call_app_helloWorldLoadedFunction
//? description: Assign result of app::helloWorldLoadedFunction() to x and return x 
//? expect: "Hello World!"
//? source: ...

const x = app::helloWorldLoadedFunction();
return x;

//?
//? test: hello_world_call_app_helloWorldLoadedFunction_direct
//? description: Call function returned from app::helloWorldLoadedFunction direct
//? expect: "Hello World!"
//? source: ...

// Call app::helloWorldLoadedFunction directly.
return app::helloWorldLoadedFunction();

//?
//? test: add_integers_from_qualified_variables_1
//? description: Add integers from qualified variables where first is a string
//? expect: 6
//? source: ...

// Cast app::one to integer because a 1 as the body of a template is a string.
return integer(app::one) + app::two + app::three;

//?
//? test: add_integers_from_qualified_variables_2
//? description: Add integers from qualified variables where first is an integer
//? expect: 6
//? source: ...

return app::two + integer(app::one) + app::three;

//?
//? test: concat_integer_strings_from_qualified_variables
//? description: Concat integer strings from qualified variables where first is a string
//? expect: "123"
//? source: ...

// This will be concat of string because app::one is string.
return app::one + app::two + app::three;

//?
//? test: qualified_variable_compile_time_uuid
//? description: Show that compile time substitution in template of qualified variable doesn't reevaluate
//? expect: true
//? source: ...

return app::uuidCompileTime == app::uuidCompileTime;

//?
//? test: qualified_variable_eval_time_uuid
//? description: Show that evaluation time substitution in template of qualified variable does reevaluate
//? expect: false
//? source: ...

return app::uuidEvalTime == app::uuidEvalTime;

//?
//? test: qualified_variable_eval_time_uuid_assignment
//? description: Show that evaluation time substitution in template from qualified variable assigned to variable doesn't reevaluate
//? expect: true
//? source: ...

const x = app::uuidEvalTime;

return x == x;
