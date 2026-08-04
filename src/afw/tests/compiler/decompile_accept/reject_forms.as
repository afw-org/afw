#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: reject_forms.as
//? customPurpose: Part of compiler decompile_accept tests
//? description: ...
Compiler-internal known rejects, unknown # forms, and bare '#' errors.
Design pad: designs/decompile-compiler-internal-inventory.md.
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

//?
//? test: reject-bare-pound-statement
//? description: bare # (pound_sign) is not legal statement syntax
//? expect: error
//? source: ...

/* Same generic path as any other unexpected token (not a special if). */
compile<script>(script("#;"));
return 0;

//?
//? test: reject-bare-pound-value
//? description: bare # is not legal value syntax
//? expect: error
//? source: ...

compile<script>(script("return #;"));
return 0;

//?
//? test: reject-pound-then-digit
//? description: #1 is pound_sign then number, not #Name (generic expect error)
//? expect: error
//? source: ...

compile<script>(script("#1;"));
return 0;

//?
//? test: reject-compile-no-operand
//? description: #compile requires at least one operand
//? expect: error
//? source: ...

compile<script>(script("#compile;"));
return 0;

//?
//? test: reject-compile-unknown-operand
//? description: unknown #compile operand name
//? expect: error
//? source: ...

compile<script>(script("#compile notAFlag;"));
return 0;

//?
//? test: reject-compile-missing-semicolon
//? description: #compile operands must end with semicolon
//? expect: error
//? source: ...

compile<script>(script("#compile typeCheck"));
return 0;
