#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: syntax.as
//? customPurpose: Part of test262
//? description: Tests let syntax
//? sourceType: script
//?
//? test: escaped-let
//? description:...
//? expect: error:Parse error at offset 101 around line 4 column 1: Unknown built-in function 'this'
//? source: ...
#!/usr/bin/env afw

// Add a global property "let", so we won't get a runtime reference error below.
this.let = 0;

l\u0065t // ASI
a;

// If the parser treated the previous escaped "let" as a lexical declaration,
// this variable declaration will result an early syntax error.
let a;


//? test: identifier-let-allowed-as-lefthandside-expression-strict
//? description:...
    for declaration:
    identifier "let" disallowed as lefthandside expression in strict mode
//? expect: error
//? source: ...
#!/usr/bin/env afw

let o = { a: 1 };
for (let in o) { }


//? test: identifier-let-disallowed-as-boundname
//? description:...
    It is a Syntax Error if the BoundNames of ForDeclaration contains "let".
//? expect: error
//? source: ...
#!/usr/bin/env afw

for (let let in {}) { }


//? test: let-closure-inside-condition
//? description:...
    let: closure inside for loop condition
//? expect: error:Parse error at offset 150 around line 5 column 50: Expecting ';'
//? source: ...
#!/usr/bin/env afw

// we need push for this, but we also don't support ++ pre-increment and
// comma operator in the increment part of for iteration
let a = [];
for (let i = 0; a.push(function () { return i; }), i < 5; ++i) { }
for (let k = 0; k < 5; ++k) {
  assert(k === a[k]());
}


//? test: let-closure-inside-initialization
//? description:...
    let: closure inside for loop initialization
//? expect: error:Parse error at offset 114 around line 5 column 17: Unknown built-in function 'f'
//? source: ...
#!/usr/bin/env afw

let a = [];
// we can't use let with multiple assignments or ++ pre-increment
for (let i = 0, f = function() { return i }; i < 5; ++i) {
  a.push(f);
}
for (let k = 0; k < 5; ++k) {
  assert(0 === a[k]());
}


//? test: let-closure-inside-next-expression
//? description:...
    let: closure inside for loop next-expression
//? expect: error:Parse error at offset 156 around line 5 column 59: Invalid assignment target
//? source: ...
#!/usr/bin/env afw

let a = [];
// we can't use let with multiple assignments or ++ pre-increment
for (let i = 0; i < 5; a.push(function () { return i; }), ++i) { }
for (let k = 0; k < 5; ++k) {
  assert(k + 1 === a[k]());
}


//? test: let-iteration-variable-is-freshly-allocated-for-each-iteration-multi-let-binding
//? description:...
    In a normal for statement the iteration variable is freshly allocated for each iteration. Multi let binding
//? expect: error:Parse error at offset 126 around line 6 column 17: Unknown built-in function 'j'
//? source: ...
#!/usr/bin/env afw

let a = [];
let b = [];
// we can't use let with multiple assignments or ++ pre-increment
for (let i = 0, j = 10; i < 5; ++i, ++j) {
  a.push(function () { return i; });
  b.push(function () { return j; });
}
for (let k = 0; k < 5; ++k) {
  assert(k === a[k]());
  assert(k + 10 === b[k]());
}


//? test: let-iteration-variable-is-freshly-allocated-for-each-iteration-single-let-binding
//? description:...
    In a normal for statement the iteration variable is freshly allocated for each iteration. Single let binding
//? expect: error:Parse error at offset 90 around line 5 column 24: Invalid assignment target
//? source: ...
#!/usr/bin/env afw

let a = [];
// no support for ++ pre-increment
for (let i = 0; i < 5; ++i) {
  a.push(function () { return i; });
}
for (let j = 0; j < 5; ++j) {
  assert(j === a[j]());
}


//? test: let
//? description:...
    global and block scope let
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

let x;
let y = 2;

// Block local
{
  let y;
  let x = 3;
}

assert(x === undefined);
assert(y === 2);

if (true) {
  let y;
  assert(y === undefined);
}



//? test: let-let-declaration-split-across-two-lines
//? description:...
    let: |let let| split across two lines is not subject to automatic semicolon insertion.
//? expect: error
//? source: ...
#!/usr/bin/env afw




let  // start of a LexicalDeclaration, *not* an ASI opportunity
let;


//? test: let-let-declaration-with-initializer-split-across-two-lines
//? description:...
    let: |let let| split across two lines is not subject to automatic semicolon insertion.
//? expect: error
//? source: ...
#!/usr/bin/env afw




let  // start of a LexicalDeclaration, *not* an ASI opportunity
let = "irrelevant initializer";


//? test: let-newline-await-in-normal-function
//? description:...
//? expect: error
//? source: ...
#!/usr/bin/env afw




function f() {
    let
    await 0;
}


//? test: let-newline-yield-in-generator-function
//? description:...
//? expect: error
//? source: ...
#!/usr/bin/env afw




function* f() {
    let
    yield 0;
}


//? test: let-newline-yield-in-normal-function
//? description:...
//? expect: error
//? source: ...
#!/usr/bin/env afw




function f() {
    let
    yield 0;
}


//? test: let-outer-inner-let-bindings
//? description:...
    outer let binding unchanged by for-loop let binding
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw

let x = "outer_x";
let y = "outer_y";

// fixme can't really test this without allowing multiple let decls
for (let x = "inner_x", i = 0; i < 1; i++) {
  let y = "inner_y";

  assert(x === "inner_x");
  assert(y === "inner_y");
}
assert(x === "outer_x");
assert(y === "outer_y");


//? test: with-initialisers-in-statement-positions-case-expression-statement-list
//? description:...
    let declarations with initialisers in statement positions:
    case Expression : StatementList
//? expect: 1
//? source: ...
#!/usr/bin/env afw

// if we let declarations return undefined, the expect could change later
switch (true) { case true: let x = 1; }


//? test: with-initialisers-in-statement-positions-default-statement-list
//? description:...
    let declarations with initialisers in statement positions:
    default : StatementList
//? expect: 1
//? source: ...
#!/usr/bin/env afw

// if we let declarations return undefined, the expect could change later
switch (true) { default: let x = 1; }


//? test: with-initialisers-in-statement-positions-do-statement-while-expression
//? description:...
    let declarations with initialisers in statement positions:
    do Statement while ( Expression )
//? expect: error
//? source: ...
#!/usr/bin/env afw

do let x = 1; while (false)


//? test: with-initialisers-in-statement-positions-for-statement
//? description:...
    let declarations with initialisers in statement positions:
    for ( ;;) Statement
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

// ecmascript does not allow this, but we don't fail on it
for (;false;) let x = 1;


//? test: with-initialisers-in-statement-positions-if-expression-statement-else-statement
//? description:...
    let declarations with initialisers in statement positions:
    if ( Expression ) Statement else Statement
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

// ecmascript does not allow this, but we don't fail on it
if (true) {} else let x = 1;


//? test: with-initialisers-in-statement-positions-if-expression-statement
//? description:...
    let declarations with initialisers in statement positions:
    if ( Expression ) Statement
//? expect: 1
//? source: ...
#!/usr/bin/env afw

// ecmascript does not allow this, but we don't fail on it
if (true) let x = 1;


//? test: with-initialisers-in-statement-positions-label-statement
//? description:...
    let declarations with initialisers in statement positions:
    label: Statement
//? expect: error
//? source: ...
#!/usr/bin/env afw

label: let x = 1;


//? test: with-initialisers-in-statement-positions-while-expression-statement
//? description:...
    let declarations with initialisers in statement positions:
    while ( Expression ) Statement
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

// ecmascript does not allow this, but we don't fail on it
while (false) let x = 1;


//? test: without-initialisers-in-statement-positions-case-expression-statement-list
//? description:...
    let declarations without initialisers in statement positions:
    case Expression : StatementList
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

switch (true) { case true: let x; }


//? test: without-initialisers-in-statement-positions-default-statement-list
//? description:...
    let declarations without initialisers in statement positions:
    default : StatementList
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

switch (true) { default: let x; }


//? test: without-initialisers-in-statement-positions-do-statement-while-expression
//? description:...
    let declarations without initialisers in statement positions:
    do Statement while ( Expression )
//? expect: error
//? source: ...
#!/usr/bin/env afw



do let x; while (false)


//? test: without-initialisers-in-statement-positions-for-statement
//? description:...
    let declarations without initialisers in statement positions:
    for ( ;;) Statement
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

// ecmascript does not allow this, but we don't fail on it
for (;false;) let x;


//? test: without-initialisers-in-statement-positions-if-expression-statement-else-statement
//? description:...
    let declarations without initialisers in statement positions:
    if ( Expression ) Statement else Statement
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

// ecmascript does not allow this, but we don't fail on it
if (true) {} else let x;


//? test: without-initialisers-in-statement-positions-if-expression-statement
//? description:...
    let declarations without initialisers in statement positions:
    if ( Expression ) Statement
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

// ecmascript does not allow this, but we don't fail on it
if (true) let x;


//? test: without-initialisers-in-statement-positions-label-statement
//? description:...
    let declarations without initialisers in statement positions:
    label: Statement
//? expect: error
//? source: ...
#!/usr/bin/env afw

label: let x;


//? test: without-initialisers-in-statement-positions-while-expression-statement
//? description:...
    let declarations without initialisers in statement positions:
    while ( Expression ) Statement
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

// ecmascript does not allow this, but we don't fail on it
while (false) let x;
