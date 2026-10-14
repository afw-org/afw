#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: prefix_operators.as
//? customPurpose: Part of language/script tests
//? description: Prefix operators + - ! void nest, as in ECMAScript (#513)
//? sourceType: script
//?
//? test: not-not
//? description: !!x and !!!x parse
//? expect: [true,false,true]
//? source: ...

let x = true;
return [!!x, !!!x, !!!false];

//?
//? test: minus-minus
//? description: - -x is x (a space keeps it from being --)
//? expect: [3,3,-3]
//? source: ...

let x = 3;
return [- -x, -(-x), - - -x];

//?
//? test: plus-minus
//? description: + and - mix
//? expect: [-3,-3]
//? source: ...

let x = 3;
return [+-x, -+x];

//?
//? test: void-void
//? description: void void x is undefined
//? expect: undefined
//? source: ...

let x = 3;
return void void x;

//?
//? test: not-binds-tighter
//? description: a nested prefix still binds tighter than a binary operator
//? expect: true
//? source: ...

let x = false;
return !!x || !!!x;

//?
//? test: too-deep
//? description: pathological prefix nesting is an error, not a crash
//? expect: error
//? source: ...

evaluate(compile<script>(script("return " + repeat("!", 1000000) + "true;")));
