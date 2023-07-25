#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: function.as
//? customPurpose: Part of test262
//? description: expressions/function
//? sourceType: script
//?
//? test: 10.1.1_A1_T2
//? description: creating function dynamically
//? expect: null
//? source: ...
#!/usr/bin/env afw

let y: integer = function (): integer {return 2;}();
if (y !== 2) {
    throw "Create anonymous function dynamically failed";
}



//? test: dflt-params-abrupt
//? description: abrupt completion returned by evaluation of initializer (function expression)
//? expect: error:Parse error at offset 209 around line 9 column 38: Expecting a literal
//? source: ...
#!/usr/bin/env afw

let callCount: integer = 0;
function fn_assert(): any {
    throw "error";
};

// \fixme function parameter default assignment can't be a function call
let f: function = function (x: any = fn_assert()): any {
    callCount = callCount + 1;
};



//? test: dflt-params-arg-val-undefined
//? description: abrupt completion returned by evaluation of initializer (function expression)
//? expect: error:Assertion failed: fromExpr !== 45
//? source: ...
#!/usr/bin/env afw

let callCount = 0;
let ref = function (fromLiteral = 23, fromExpr = 45, fromHole = 99): any {
    assert(fromLiteral === 23, "fromLiteral !== 23");
    assert(fromExpr === 45, "fromExpr !== 45");
    assert(fromHole === 99, "fromHole !== 99");
    callCount = callCount + 1;
};

function fn(): any {
    return undefined;
};

// \fixme can't pass in undefined for parameters
ref(undefined, fn());

assert(callCount === 1, "callCount !== 1");



//? test: dflt-params-duplicates
//? description: It is a syntax error if function parameters contain duplicate parameter names
//? expect: error
//? source: ...
#!/usr/bin/env afw

function f(x: integer = 0, x: integer): any {};

//? test: dflt-params-ref-later
//? description: Referencing a parameter that occurs later in the parameter list
//? expect: error:Parse error at offset 85 around line 6 column 19: Expecting a literal
//? source: ...
#!/usr/bin/env afw

let x = 0;
let callCount = 0;
let f: function;
f = function (x = y, y?): any {
    callCount = callCount + 1;
};



//? test: dflt-params-ref-prior
//? description: Referencing a parameter that occurs earlier in the parameter list
//? expect: error:Parse error at offset 77 around line 5 column 28: Expecting a literal
//? source: ...
#!/usr/bin/env afw

let x = 0;
let callCount = 0;
let ref = function (x, y = x, z = y): any {
    assert(x === 3, "first argument value");
    assert(y === 3, "second argument value");
    assert(y === 3, "third argument value");
    callCount = callCount + 1;
};

ref(3);

assert(callCount === 1, "function invoked exactly once");



//? test: dflt-params-ref-self
//? description: Referencing a parameter from within its own initializer
//? expect: error
//? source: ...
#!/usr/bin/env afw

let x: integer = 0;
let callCount: integer = 0;
let f: function;

f = function (x: integer = x): any {    
    callCount = callCount + 1;
};

ref();

assert(callCount === 0, "function body not evaluated");



//? test: named-no-strict-reassign-fn-name-in-body-in-arrow
//? description: Reassignment of function name is silently ignored
//? expect: error:Assertion failed: ref() !== ref
//? source: ...
#!/usr/bin/env afw

let callCount = 0;
let ref = function BindingIdentifier() {
    callCount++;
    (function () {
        BindingIdentifier = 1;
    })();
    return BindingIdentifier;
};

assert(ref() === ref, "ref() !== ref");
assert(callCount === 1, "function invoked exactly once");



//? test: named-no-strict-reassign-fn-name-in-body
//? description: Reassignment of function name is silently ignored.
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

let callCount: integer = 0;
function BindingIdentifier(): any {
    callCount = callCount + 1;
    BindingIdentifier = 1;
    return BindingIdentifier;
};
let ref: function = BindingIdentifier;

assert(ref() === ref, "ref() !== ref");
assert(callCount === 1, "function invoked exactly once");



//? test: param-duplicated-strict-1
//? description: It is a syntax error if any identifier value occurs more than once within a parameter list
//? expect: error
//? source: ...
#!/usr/bin/env afw

function f(param: integer, param: integer): any {};



//? test: param-duplicated-strict-2
//? description: It is a syntax error if any identifier value occurs more than once within a parameter list
//? expect: error
//? source: ...
#!/usr/bin/env afw

function f(param1: integer, param2: integer, param1: integer): any {};



//? test: param-duplicated-strict-3
//? description: It is a syntax error if any identifier value occurs more than once within a parameter list
//? expect: error
//? source: ...
#!/usr/bin/env afw

function f(param: integer, param: integer, param: integer): any {};

