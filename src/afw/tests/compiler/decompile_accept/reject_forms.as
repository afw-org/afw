#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: reject_forms.as
//? customPurpose: Part of compiler decompile_accept tests
//? description: ...
Compiler-private known rejects and unknown # forms (decompile inventory).
Separate file so expect:error cases stay focused. Design pad:
designs/decompile-compiler-private-inventory.md.
//? sourceType: script
//?
//? test: reject-closure-binding-statement
//? description: #closure_binding is runtime-only (statement position)
//? expect: error
//? source: ...

compile<script>(script("#closure_binding(x);"));
return 0;

//?
//? test: reject-closure-binding-value
//? description: #closure_binding is runtime-only (value position)
//? expect: error
//? source: ...

compile<script>(script("return #closure_binding(x);"));
return 0;

//?
//? test: reject-function-thunk-statement
//? description: #function_thunk is C-side only (statement position)
//? expect: error
//? source: ...

compile<script>(script("#function_thunk(\"detail\");"));
return 0;

//?
//? test: reject-function-thunk-value
//? description: #function_thunk is C-side only (value position)
//? expect: error
//? source: ...

compile<script>(script("return #function_thunk(\"detail\");"));
return 0;

//?
//? test: reject-unknown-statement
//? description: unknown #Name in statement position
//? expect: error
//? source: ...

compile<script>(script("#no_such_form(1);"));
return 0;

//?
//? test: reject-unknown-value
//? description: unknown #Name in value position
//? expect: error
//? source: ...

compile<script>(script("return #no_such_form(1);"));
return 0;

//?
//? test: reject-assignment-target-as-statement
//? description: #assignment_target is value-only, not a statement production
//? expect: error
//? source: ...

compile<script>(script("#assignment_target(\"const\",x);"));
return 0;

//?
//? test: reject-script-function-as-statement
//? description: #script_function is value-only
//? expect: error
//? source: ...

compile<script>(script("#script_function(a,#block(return(a)));"));
return 0;
