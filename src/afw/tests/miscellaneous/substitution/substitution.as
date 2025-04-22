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
//? test: hello_world_evaluate_template
//? description: Evaluate mixed template string
//? expect: "Hello World!"
//? source: ...

const source = template("H#{'ello ' + 'Wor'}ld${'!'}");

return eval(source);

//?
//? test: hello_world_return_app_helloWorld
//? description: Return evaluated app::helloWorld string
//? expect: "Hello World!"
//? source: ...

return evaluate(app::helloWorld);

//?
//? test: hello_world_return_app_compileTime
//? description: Return evaluated app::compileTime string
//? expect: "Hello World!"
//? source: ...

return evaluate(app::compileTime);

//?
//? test: hello_world_return_app_evalTime
//? description: Return evaluated app::evalTime string
//? expect: "Hello World!"
//? source: ...

return evaluate(app::evalTime);

//?
//? test: hello_world_assign_app_helloWorld
//? description: Assign unevaluated app::helloWorld to x then return evaluated(x)
//? expect: "Hello World!"
//? source: ...

const x = app::helloWorld;
return evaluate(x);

//?
//? test: hello_world_assign_app_compileTime
//? description: Assign unevaluated app::compileTime to x then return evaluated(x)
//? expect: "Hello World!"
//? source: ...

const x = app::compileTime;
return evaluate(x);

//?
//? test: hello_world_assign_app_evalTime
//? description: Assign unevaluated app::evalTime to x then return evaluated(x)
//? expect: "Hello World!"
//? source: ...

const x = app::evalTime;
return evaluate(x);

//?
//? test: hello_world_call_app_helloWorldFunction
//? description: Call function returned from app::helloWorldFunction
//? expect: "Hello World!"
//? source: ...

const x = evaluate(app::helloWorldFunction);
return x();

//?
//? test: hello_world_call_loaded_function
//? description: Call function loaded from file
//? expect: "Hello World!"
//? source: ...

const x = evaluate(compile_from_file('includes/a.as'));
return x();

//?
//? test: hello_world_call_loaded_function_using_eval
//? description: Call function loaded from file using eval_from_file
//? expect: "Hello World!"
//? source: ...

const x = eval_from_file('includes/a.as');
return x();

//?
//? test: hello_world_call_app_helloWorldLoadedFunction
//? description: Call function returned from app::helloWorldLoadedFunction
//? expect: "Hello World!"
//? source: ...

// Evaluate is called so that x holds function reference instead of compiled
// value of app::helloWorldLoadedFunction template.
const x = evaluate(app::helloWorldLoadedFunction);
return x();

//?
//? test: hello_world_call_app_helloWorldLoadedFunction2_no_evaluate
//? description: Call function returned from app::helloWorldLoadedFunction with no evaluate
//? expect: "Hello World!"
//? source: ...

// If no evaluate call it should work too because of special case handled by
// value_call evaluate().
const x = app::helloWorldLoadedFunction;
return x();

//?
//? test: hello_world_call_app_helloWorldLoadedFunction_direct
//? description: Call function returned from app::helloWorldLoadedFunction direct
//? expect: "Hello World!"
//? source: ...

// Call app::helloWorldLoadedFunction directly.
return app::helloWorldLoadedFunction();

//?
//? test: hello_world_call_app_helloWorldLoadedFunction2
//? description: Call function returned from app::helloWorldLoadedFunction2
//? expect: "Hello World!"
//? source: ...

// Evaluate is called so that x holds function reference instead of compiled
// value of app::helloWorldLoadedFunction2 template.
const x = evaluate(app::helloWorldLoadedFunction2);
return x();

//?
//? test: hello_world_call_app_helloWorldLoadedFunction2_no_evaluate
//? description: Call function returned from app::helloWorldLoadedFunction2 with no evaluate
//? expect: "Hello World!"
//? source: ...

// If no evaluate call it should work too because of special case handled by
// value_call evaluate().
const x = app::helloWorldLoadedFunction2;
return x();

//?
//? test: hello_world_call_app_helloWorldLoadedFunction2_direct
//? description: Call function returned from app::helloWorldLoadedFunction2 direct
//? expect: "Hello World!"
//? source: ...

// Call app::helloWorldLoadedFunction2 directly.
return app::helloWorldLoadedFunction2();

//?
//? test: hello_world_call_app_helloWorldLoadedFunction2_error
//? description: Return compiled value of app::helloWorldLoadedFunction2 has to be skipped because unevaluated value in json test result causes error
//? expect: error:Function 'eval<script>' returned a value that is not evaluated. (closure_binding )
//? skip: true
//? source: ...

return app::helloWorldLoadedFunction2;

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
