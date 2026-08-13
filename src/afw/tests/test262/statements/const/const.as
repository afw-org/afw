#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: const.as
//? customPurpose: Part of test262
//? description: Tests const
//? sourceType: script
//?
//? test: block-local-closure-get-before-initialization
//? description:...
    const: block local closure [[Get]] before initialization.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parse error at offset 46 around line 4 column 25: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

{
  function f() { return x + 1; }

    f();

  const x = 1;
}


//? test: block-local-use-before-initialization-in-declaration-statement
//? description:...
    const: block local use of name in its own initializer.
//? differences: Adaptive has no TDZ; RHS sees the new binding as undefined, so x + 1 errors on add — not ES ReferenceError
//? expect: error
//? source: ...
#!/usr/bin/env afw

const x = x + 1;


//? test: block-local-use-before-initialization-in-prior-statement
//? description:...
    const: block local use before initialization in prior statement.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parse error at offset 41 around line 5 column 5: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

function() {
  {
    x; const x = 1;
  }
}


//? test: cptn-value
//? description: Returns an empty completion
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

assert(
  eval(script('const test262id1 = 1;')) == undefined, 
  'Single declaration'
);
assert(
  eval(script('const test262id2 = 2, test262id3 = 3;')) == undefined,
  'Multiple declarations'
);
return;
//? test: fn-name-arrow
//? description: Assignment of function `name` attribute (ArrowFunction)
//? expect: error:Parse error at offset 35 around line 3 column 16: Expecting Value
//? source: ...
#!/usr/bin/env afw

const arrow = () => {};

assert(arrow.name === 'arrow');
verifyNotEnumerable(arrow, 'name');
verifyNotWritable(arrow, 'name');
verifyConfigurable(arrow, 'name');


//? test: fn-name-cover
//? description:...
    Assignment of function `name` attribute (CoverParenthesizedExpression)
//? expect: error:Parse error at offset 92 around line 4 column 18: Expecting ')'
//? source: ...
#!/usr/bin/env afw

// we do not support the comma operator in expressions
const xCover = (0, function() {});
const cover = (function() {});

assert(xCover.name !== 'xCover');

assert(cover.name === 'cover');
verifyNotEnumerable(cover, 'name');
verifyNotWritable(cover, 'name');
verifyConfigurable(cover, 'name');


//? test: fn-name-fn
//? description: Assignment of function `name` attribute (FunctionExpression)
//? expect: error:Parse error at offset 131 around line 9 column 1: Unknown built-in function 'verifyNotEnumerable'
//? source: ...
#!/usr/bin/env afw

const xFn = function x() {};
const fn = function() {};

assert(xFn.name !== 'xFn');

assert(fn.name === 'fn');
verifyNotEnumerable(fn, 'name');
verifyNotWritable(fn, 'name');
verifyConfigurable(fn, 'name');


//? test: fn-name-gen
//? description: Assignment of function `name` attribute (GeneratorExpression)
//? expect: error:Parse error at offset 41 around line 3 column 22: Expecting Value
//? source: ...
#!/usr/bin/env afw

const xGen = function* x() {};
const gen = function*() {};

assert(xGen.name !== 'xGen');

assert(gen.name === 'gen');
verifyNotEnumerable(gen, 'name');
verifyNotWritable(gen, 'name');
verifyConfigurable(gen, 'name');


//? test: function-local-closure-get-before-initialization
//? description:...
    const: function local closure [[Get]] before initialization.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parse error at offset 58 around line 4 column 25: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

(function() {
  function f() { return x + 1; }

  function() {
    f();
  }

  const x = 1;
}());


//? test: function-local-use-before-initialization-in-declaration-statement
//? description:...
    const: function local use before initialization in declaration statement.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parameter 1 of function 'add' can not be undefined
//? source: ...
#!/usr/bin/env afw

  (function() {
    const x = x + 1;
  }());


//? test: function-local-use-before-initialization-in-prior-statement
//? description:...
    const: function local use before initialization in prior statement.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parse error at offset 53 around line 5 column 5: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

function() {
  (function() {
    x; const x = 1;
  }());
}


//? test: global-closure-get-before-initialization
//? description:...
    const: global closure [[Get]] before initialization.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parse error at offset 42 around line 3 column 23: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

function f() { return x + 1; }

  f();

const x = 1;


//? test: global-use-before-initialization-in-declaration-statement
//? description:...
    const: global use before initialization in declaration statement.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parameter 1 of function 'add' can not be undefined
//? source: ...
#!/usr/bin/env afw

const x = x + 1;


//? test: global-use-before-initialization-in-prior-statement
//? description:...
    const: global use before initialization in prior statement.
    (TDZ, Temporal Dead Zone)
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

x; const x = 1;


//? test: redeclaration-error-from-within-strict-mode-function-const
//? description:...
    Redeclaration error within strict mode function inside non-strict code.
//? expect: error
//? source: ...
#!/usr/bin/env afw

(function() { 'use strict'; { const f = 1; let f; } })



//? test: static-init-await-binding-invalid
//? description: BindingIdentifier may not be `await` within class static blocks
//? expect: error
//? source: ...
#!/usr/bin/env afw

class C {
  static {
    const await = 0;
  }
}


//? test: static-init-await-binding-valid
//? description: The `await` keyword is interpreted as an identifier within arrow function bodies
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function 'class'
//? source: ...
#!/usr/bin/env afw

class C {
  static {
    (() => { const await = 0; });
  }
}


