#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: accept_forms.as
//? customPurpose: Part of compiler decompile_accept tests
//? description: ...
Compiler-private # forms: direct accept path (issue #28 / decompile inventory).
Each case compiles hand-written decompile-shaped text and evaluates when
meaningful. Emit/round-trip coverage is emit_and_roundtrip.as; rejects are
reject_forms.as. Design pad: designs/decompile-compiler-private-inventory.md.
//? sourceType: script
//?
//? test: accept-block-evaluate
//? description: #block as script root evaluates
//? expect: 0
//? source: ...

assert(evaluate(compile<script>(script("#block(return(1))"))) === 1);
return 0;

//?
//? test: accept-assignment-target-const
//? description: #assignment_target introduces binding under const()
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script(
    "#block(const(#assignment_target(\"const\",x),2,undefined),return(x))"
)));
assert(r === 2);
return 0;

//?
//? test: accept-script-function-simple
//? description: #script_function body + call
//? expect: 0
//? source: ...

const f = evaluate(compile<script>(script(
    "#block(return(#script_function(a,#block(return(a)))))"
)));
assert(f(7) === 7);
return 0;

//?
//? test: accept-script-function-typed-return
//? description: #script_function with param type and return type
//? expect: 0
//? source: ...

const f = evaluate(compile<script>(script(
    "#block(return(#script_function(a:integer,#block(return(a)),integer)))"
)));
assert(f(3) === 3);
return 0;

//?
//? test: accept-script-function-rest
//? description: #script_function rest parameter
//? expect: 0
//? source: ...

const f = evaluate(compile<script>(script(
    "#block(return(#script_function(...r,#block(return(r)))))"
)));
const a = f(1, 2);
assert(a[0] === 1);
assert(a[1] === 2);
return 0;

//?
//? test: accept-script-function-pattern
//? description: #script_function object Pattern parameter
//? expect: 0
//? source: ...

const f = evaluate(compile<script>(script(
    "#block(return(#script_function({a,b},#block(return(add(a,b))))))"
)));
assert(f({ a: 1, b: 2 }) === 3);
return 0;

//?
//? test: accept-template-definition
//? description: #template_definition parts evaluate to concatenated string
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script(
    "#block(return(#template_definition(\"a\",#block(1),\"b\")))"
)));
assert(r === "a1b");
return 0;

//?
//? test: accept-list-expression
//? description: #list_expression accepts (call-site decompile prefers surface ...)
//? expect: 0
//? source: ...

/*
 * Hand-written #list_expression compiles. Decompile of the same tree inside
 * a call often rewrites to surface ...expr (see inventory).
 */
const d = decompile(compile<script>(script(
    "#block(return(#list_expression([1,2])))"
)));
/* After decompile rewrite, recompile still works. */
assert(d == decompile(compile<script>(script(d))));
return 0;

//?
//? test: accept-switch-default-and-statements
//? description: #switch_default and #statements inside switch()
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script(
    "#block(switch(eqx,9,1,#statements(return(1))," +
    "#switch_default,#statements(return(99))))"
)));
assert(r === 99);
return 0;

//?
//? test: accept-block-as-statement
//? description: #block allowed in statement position (dispatch)
//? expect: 0
//? source: ...

/*
 * Decompile uses a single root #block. Statement-position dispatch also
 * accepts #block so a lone #block(...) statement recompiles.
 */
assert(evaluate(compile<script>(script("#block(return(5))"))) === 5);
return 0;
