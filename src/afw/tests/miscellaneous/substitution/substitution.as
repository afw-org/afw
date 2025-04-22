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
//? test: hello_world_call_app_helloWorldLoadedFunction
//? description: Call function returned from app::helloWorldLoadedFunction
//? expect: "Hello World!"
//? source: ...

const x = evaluate(app::helloWorldLoadedFunction);
return x();

//?
//? test: add_integers_from_qualified_variables
//? description: Add integers from qualified variables (make app::one is string)
//? expect: 6
//? source: ...

return integer(app::one) + app::two + app::three;

//?
//? test: add_integers_from_qualified_variables
//? description: Add integers from qualified variables (make app::one integer)
//? expect: 6
//? source: ...

return app::two + integer(app::one) + app::three;
