#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: let.as
//? customPurpose: Part of test262
//? description: Tests let
//? sourceType: script
//?
//? test: block-local-closure-get-before-initialization
//? description:...
    let: block local closure [[Get]] before initialization.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parse error at offset 46 around line 4 column 25: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

{
  function f() { return x + 1; }

  let x;
}


//? test: block-local-closure-set-before-initialization
//? description:...
    let: block local closure [[Set]] before initialization.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parse error at offset 39 around line 4 column 18: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

{
  function f() { x = 1; }

  let x;
}


//? test: block-local-use-before-initialization-in-declaration-statement
//? description:...
    let: block local use before initialization in declaration statement.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parameter 1 of function 'add' can not be undefined
//? source: ...
#!/usr/bin/env afw

  {
    let x = x + 1;
  }


//? test: block-local-use-before-initialization-in-prior-statement
//? description:...
    let: block local use before initialization in prior statement.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parse error at offset 28 around line 4 column 5: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

  {
    x; let x;
  }


//? test: cptn-value
//? description: Returns an empty completion
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

assert(
  eval(script('let test262id1;')) == undefined, 
  'Single declaration without initializer'
);

assert(
  eval(script('let test262id2 = 2;')) == undefined,
  'Single declaration bearing initializer'
);
assert(
  eval(script('let test262id3 = 3, test262id4;')) == undefined,
  'Multiple declarations, final without initializer'
);
assert(
  eval(script('let test262id5, test262id6 = 6;')) == undefined,
  'Multiple declarations, final bearing initializer'
);
return;


//? test: fn-name-arrow
//? description: Assignment of function `name` attribute (ArrowFunction)
//? expect: error:Parse error at offset 80 around line 4 column 14: Expecting Value
//? source: ...
#!/usr/bin/env afw

// we do not currently support arrow functions
let arrow = () => {};

assert(arrow.name === 'arrow');
verifyNotEnumerable(arrow, 'name');
verifyNotWritable(arrow, 'name');
verifyConfigurable(arrow, 'name');


//? test: fn-name-cover
//? description:...
    Assignment of function `name` attribute (CoverParenthesizedExpression)
//? expect: error:Parse error at offset 36 around line 4 column 16: Expecting ')'
//? source: ...
#!/usr/bin/env afw


let xCover = (0, function() {});
let cover = (function() {});

assert(xCover.name !== 'xCover');

assert(cover.name === 'cover');
verifyNotEnumerable(cover, 'name');
verifyNotWritable(cover, 'name');
verifyConfigurable(cover, 'name');


//? test: fn-name-fn
//? description: Assignment of function `name` attribute (FunctionExpression)
//? expect: error:Parse error at offset 128 around line 10 column 1: Unknown built-in function 'verifyNotEnumerable'
//? source: ...
#!/usr/bin/env afw


let xFn = function x() {};
let fn = function() {};

assert(xFn.name !== 'xFn');

assert(fn.name === 'fn');
verifyNotEnumerable(fn, 'name');
verifyNotWritable(fn, 'name');
verifyConfigurable(fn, 'name');


//? test: fn-name-gen
//? description: Assignment of function `name` attribute (GeneratorExpression)
//? expect: error:Parse error at offset 40 around line 4 column 20: Expecting Value
//? source: ...
#!/usr/bin/env afw


let xGen = function* x() {};
let gen = function*() {};

assert(xGen.name !== 'xGen');

assert(gen.name === 'gen');
verifyNotEnumerable(gen, 'name');
verifyNotWritable(gen, 'name');
verifyConfigurable(gen, 'name');


//? test: function-local-closure-get-before-initialization
//? description:...
    let: function local closure [[Get]] before initialization.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parse error at offset 58 around line 4 column 25: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

(function() {
  function f() { return x + 1; }

  f();

  let x;
}());


//? test: function-local-closure-set-before-initialization
//? description:...
    let: function local closure [[Set]] before initialization.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parse error at offset 51 around line 4 column 18: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

(function() {
  function f() { x = 1; }

    f();

  let x;
}());


//? test: function-local-use-before-initialization-in-declaration-statement
//? description:...
    let: function local use of name in its own initializer
//? differences: Adaptive has no TDZ; let x = x binds x to undefined (ES would ReferenceError)
//? expect: 0
//? source: ...
#!/usr/bin/env afw

(function () {
  let x = x;
  assert(x === undefined);
})();
return 0;

//? test: function-local-use-before-initialization-in-prior-statement
//? description:...
    let: function local use before initialization in prior statement.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parse error at offset 53 around line 5 column 5: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

function() {
  (function() {
    x; let x;
  }());
}


//? test: global-closure-get-before-initialization
//? description:...
    let: global closure [[Get]] before initialization.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parse error at offset 42 around line 3 column 23: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

function f() { return x + 1; }

f();

let x;


//? test: global-closure-set-before-initialization
//? description:...
    let: global closure [[Set]] before initialization.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parse error at offset 35 around line 3 column 16: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

function f() { x = 1; }

f();

let x;


//? test: global-use-before-initialization-in-declaration-statement
//? description:...
    let: global use before initialization in declaration statement.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parameter 1 of function 'add' can not be undefined
//? source: ...
#!/usr/bin/env afw

let x = x + 1;


//? test: global-use-before-initialization-in-prior-statement
//? description:...
    let: global use before initialization in prior statement.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

x; let x;


//? test: redeclaration-error-from-within-strict-mode-function
//? description:...
    Redeclaration error within strict mode function inside non-strict code.
//? expect: error
//? source: ...
#!/usr/bin/env afw


# we 'use strict' rules by default, so it's not necessary
(function() { /*'use strict';*/ { let f; let f; } })



//? test: static-init-await-binding-invalid
//? description: BindingIdentifier may not be `await` within class static blocks
//? expect: error
//? source: ...
#!/usr/bin/env afw

class C {
  static {
    let await;
  }
}


//? test: static-init-await-binding-valid
//? description: The `await` keyword is interpreted as an identifier within arrow function bodies
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function 'class'
//? source: ...
#!/usr/bin/env afw

class C {
  static {
    (() => { let await; });
  }
}
