#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: pragma.as
//? customPurpose: Part of compiler category tests
//? description: PragmaStatement / PragmaValue (#block and unknown)
//? sourceType: script
//?
//? test: pragma-unknown-value
//? description: Unknown #name in expression position
//? expect: error
//? source: ...

return #nosuch;

//? test: pragma-unknown-value-call-like
//? description: Unknown #name() in expression position
//? expect: error
//? source: ...

return #nosuch(1);

//? test: pragma-unknown-statement
//? description: Unknown #name as statement
//? expect: error
//? source: ...

#typecheck;
return 0;

//? test: pragma-block-literal
//? description: #block(add(1,2)) compiles and evaluates to 3
//? skip: false
//? expect: 0
//? source: ...

assert(evaluate(compile<script>(script("#block(add(1,2))"))) == 3);
return 0;

//? test: pragma-block-empty
//? description: Empty #block() compiles
//? skip: false
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script("#block()")));
assert(!is_defined(r));
return 0;

//? test: pragma-block-roundtrip-expr
//? description: decompile/compile round-trip for expression script 1+2
//? skip: false
//? expect: 0
//? source: ...

const d = decompile(compile<script>(script("1+2")));
assert(d == "#block(add(1,2))");
assert(evaluate(compile<script>(script(d))) == 3);
/* Outer script block unwraps sole #block so decompile is stable. */
assert(decompile(compile<script>(script(d))) == d);
return 0;

//? test: pragma-block-roundtrip-return
//? description: decompile/compile round-trip for return 1+2
//? skip: false
//? expect: 0
//? source: ...

const d = decompile(compile<script>(script("return 1+2;")));
assert(d == "#block(return(add(1,2)))");
assert(evaluate(compile<script>(script(d))) == 3);
assert(decompile(compile<script>(script(d))) == d);
return 0;

//? test: pragma-block-as-value
//? description: #block as expression value (parenthesized / assigned path)
//? skip: false
//? expect: 0
//? source: ...

// Nested use: outer script evaluates an expression that is #block(...)
assert(evaluate(compile<script>(script(
    "const b = #block(add(10, 20));\nreturn evaluate(b);"
))) == 30);
return 0;
