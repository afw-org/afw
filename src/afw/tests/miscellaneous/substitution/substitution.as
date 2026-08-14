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
//? test: hello_world_call_app_helloWorldFunctionArray
//? description: Call function app::helloWorldFunctionArray[0]()
//? expect: "Hello World!"
//? source: ...

const x = app::helloWorldFunctionArray;

return x[0]();

//?
//? test: hello_world_call_app_helloWorldFunctionArray_not_assigned
//? description: ...
Call function app::helloWorldFunctionArray[0]() without assigning to variable
//? expect: "Hello World!"
//? source: ...

return app::helloWorldFunctionArray[0]();

//?
//? test: hello_world_call_app_helloWorldFunctionArray_destructure
//? description: Call function app::helloWorldFunctionArray using destructure
//? expect: "Hello World!"
//? source: ...

const [x,y] = app::helloWorldFunctionArray;

return x();

//?
//? test: hello_world_call_app_helloWorldFunctionObject
//? description: Call function app::helloWorldFunctionObject[0]()
//? expect: "Hello World!"
//? source: ...

const x = app::helloWorldFunctionObject;

return x.a();

//?
//? test: hello_world_call_app_helloWorldFunctionObject_not_assigned
//? description: ...
Call function app::helloWorldFunctionArray[0]() without assigning to variable
//? expect: "Hello World!"
//? source: ...

return app::helloWorldFunctionObject.a();

//?
//? test: hello_world_call_app_helloWorldFunctionObject_destructure
//? description: ...
Call function app::helloWorldFunctionObject using destructure
//? expect: "Hello World!"
//? source: ...

const {a:x,b:y} = app::helloWorldFunctionObject;

return x();

//?
//? test: hello_world_call_app_helloWorldFunctionObject_destructure_concat
//? description: ...
Call function app::helloWorldFunctionObject using destructure and concat
//? expect: "Hello World!<<<Same to you!"
//? source: ...

const {a:x,b:y} = app::helloWorldFunctionObject;

return x() + '<<<' + y();

//?
//? test: hello_world_call_loaded_function
//? description: Call function loaded from file
//? expect: "Hello World!"
//? source: ...

const x = eval_from_file('includes/a.as');
return x();

//?
//? test: hello_world_call_app_helloWorldLoadedFunction
//? description: ...
Assign result of app::helloWorldLoadedFunction() to x and return x 
//? expect: "Hello World!"
//? source: ...

const x = app::helloWorldLoadedFunction();
return x;

//?
//? test: hello_world_call_app_helloWorldLoadedFunction_direct
//? description: ...
Call function returned from app::helloWorldLoadedFunction directly
//? expect: "Hello World!"
//? source: ...

// Call app::helloWorldLoadedFunction directly.
return app::helloWorldLoadedFunction();

//?
//? test: add_integers_from_qualified_variables_1
//? description: ...
Add integers from qualified variables where first is a string
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
//? description: ...
Concat integer strings from qualified variables where first is a string
//? expect: "123"
//? source: ...

// This will be concat of string because app::one is string.
return app::one + app::two + app::three;

//?
//? test: qualified_variable_compile_time_uuid
//? description: ...
Show that compile time substitution in template of qualified variable doesn't
reevaluate
//? expect: true
//? source: ...

return app::uuidCompileTime == app::uuidCompileTime;

//?
//? test: qualified_variable_eval_time_uuid
//? description: ...
Show that evaluation time substitution in template of qualified variable does
reevaluate
//? expect: false
//? source: ...

return app::uuidEvalTime == app::uuidEvalTime;

//?
//? test: qualified_variable_eval_time_uuid_assignment
//? description: ...
Show that evaluation time substitution in template from qualified variable
assigned to variable doesn't reevaluate
//? expect: true
//? source: ...

const x = app::uuidEvalTime;

return x == x;

//?
//? test: compile_time_template_literal_1
//? description: ...
Show that script in two compile time templates do get different results
//? expect: false
//? source: ...

return #{return generate_uuid();} == #{return generate_uuid();};

//?
//? test: compile_time_template_literal_2
//? description: ...
Show that that compile time template assigned to variable doesn't reevaluate
//? expect: true
//? source: ...

const x = #{return generate_uuid();};
return x == x;

//?
//? test: run_time_template_literal_1
//? description: Show that two runtime template literals each evaluate
//? expect: false
//? source: ...

return `${return generate_uuid();}` == `${return generate_uuid();}`;

//?
//? test: run_time_template_literal_2
//? description: ...
Show that a runtime template literal assigned to variable doesn't reevaluate
//? expect: true
//? source: ...

const x = `${return generate_uuid();}`;
return x == x;

//?
//? test: run_time_template_literal_3
//? description: ...
Show that a runtime template literal assigned to a variable with unevaluated
data type is evaluated before assignment because a literal is not a compiled
value and thus is part of normal expression evaluation
//? expect: true
//? source: ...

const x:unevaluated = `${return generate_uuid();}`;
return x == x;

//?
//? test: run_time_template_literal_4
//? description: ...
Show that a compiled template assigned to a variable with unevaluated
data type does reevaluate each time accessed because that is the purpose
of the unevaluated data type.
//? expect: false
//? source: ...

const x:unevaluated = compile(template("${return generate_uuid();}"));
return x == x;

//?
//? test: run_time_template_literal_5
//? description: ...
Show that a compiled template assigned to a variable without unevaluated
data type is evaluated at assignment time.
//? expect: true
//? source: ...

const x = compile(template("${return generate_uuid();}"));
return x == x;

//?
//? test: qualified_variable_compile_time_uuid_stored_unevaluated
//? description: ...
Show that compile time substitution in template of qualified variable doesn't
reevaluate even when assigned to a variable with data type unevaluated
//? expect: true
//? source: ...

const x:unevaluated = app::uuidCompileTime;
return x == x;

//?
//? test: qualified_variable_eval_time_uuid_stored_unevaluated
//? description: ...
Show that runtime substitution in template of qualified variable doesn't
evaluate when assigned to a variable with data type unevaluated
//? expect: false
//? source: ...

const x:unevaluated = app::uuidEvalTime;
return x == x;

//?
//? test: qualified_variable_eval_time_uuid_assignment_with_evaluate_stored_unevaluated
//? description: ...
Show that evaluation time substitution in template from qualified variable that
is evaluated and assigned to variable does not reevaluate
//? expect: true
//? source: ...

const x:unevaluated = evaluate(app::uuidEvalTime);

return x == x;

//?
//? test: compile_time_outer_local_not_visible
//? description: Outer locals are not visible inside compile-time substitution
//? expect: error:Parse error at offset 0 around line 1 column 1: Unknown built-in function 'outer'
//? source: ...

const outer = 42;
return #{outer};

//?
//? test: compile_time_throw
//? description: throw inside compile-time substitution fails at compile time
//? expect: error:Error during compile at offset 7 around line 2 column 7: boom
//? source: ...

return #{ throw "boom"; };

//?
//? test: template_string_escape_hash
//? description: \# suppresses #{ opener in template string
//? expect: "#{lit}"
//? source: ...

return `\#{lit}`;

//?
//? test: template_string_escape_dollar
//? description: \$ suppresses ${ opener in template string
//? expect: "${lit}"
//? source: ...

return `\${lit}`;

//?
//? test: template_string_sub_still_works
//? description: #{ and ${ still substitute in template strings
//? expect: "ab"
//? source: ...

return `#{'a'}${'b'}`;

//?
//? test: template_string_backslash
//? description: \\ remains a normal string escape in template strings
//? expect: "\\"
//? source: ...

return `\\`;

//?
//? test: template_string_mixed_escape_and_sub
//? description: Mix opener-suppress and real substitution
//? expect: "x#{y}z"
//? source: ...

return `x\#{y}${'z'}`;

//?
//? test: compile_time_value_integer
//? description: Bare compile-time substitution as a Value
//? expect: 3
//? source: ...

return #{1 + 2};

//?
//? test: compile_time_nested
//? description: Nested compile-time substitutions
//? expect: 3
//? source: ...

return #{ return #{1 + 2;}; };

//?
//? test: eval_time_outer_local_visible
//? description: Outer locals are visible inside evaluation-time substitution
//? expect: 42
//? source: ...

const outer = 42;
return `${outer}`;