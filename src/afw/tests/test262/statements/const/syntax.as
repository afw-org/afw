#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: syntax.as
//? customPurpose: Part of test262
//? description: Tests const syntax
//? sourceType: script
//?
//? test: block-scope-syntax-const-declarations-mixed-without-with-initialiser
//? description:...
    const declarations mixed: without, with initialiser
//? expect: error
//? source: ...
#!/usr/bin/env afw

const x, y = 1;


//? test: block-scope-syntax-const-declarations-mixed-with-without-initialiser
//? description:...
    const declarations mixed: with, without initialiser
//? expect: error
//? source: ...
#!/usr/bin/env afw

const x = 1, y;


//? test: block-scope-syntax-const-declarations-without-initialiser
//? description:...
    const declarations without initialiser
//? expect: error
//? source: ...
#!/usr/bin/env afw

const x;


//? test: const-declaring-let-split-across-two-lines
//? description:...
    const: |const let| split across two lines is a static semantics early error.
//? expect: error
//? source: ...
#!/usr/bin/env afw

const
let = "irrelevant initializer";


//? test: const-invalid-assignment-next-expression-for
//? description:...
    const: invalid assignment in next expression
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw

// this throws the JSON closure message
// when fixed, the expect should be an error because i++ on const
function() {
  for (const i = 0; i < 1; i++) {}
}


//? test: const-invalid-assignment-statement-body-for-in
//? description:...
    const: invalid assignment in Statement body
//? expect: error:Parse error at offset 75 around line 5 column 16: Expecting '='
//? source: ...
#!/usr/bin/env afw

function() {
  // we don't support 'in'
  for (const x in [1, 2, 3]) { x++; }
}


//? test: const-invalid-assignment-statement-body-for-of
//? description:...
    const: invalid assignment in Statement body
//? expect: error
//? skip: true
//? source: ...
#!/usr/bin/env afw

// fixme this produces a JSON closure error
function() {
  for (const x of [1, 2, 3]) { x++; }
}


//? test: const
//? description:...
    global and block scope const
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

const z = 4;
// Block local
{
  const z = 5;
}
assert(z === 4);
if (true) {
  const z = 1;
  assert(z === 1);
}


//? test: const-outer-inner-let-bindings
//? description:...
    outer const binding unchanged by for-loop const binding
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

//
const x = "outer_x";
const y = "outer_y";
let i = 0;
for (const x = "inner_x"; i < 1; i++) {
  const y = "inner_y";
  assert(x === "inner_x");
  assert(y === "inner_y");
}
assert(x === "outer_x");
assert(y === "outer_y");


//? test: with-initializer-case-expression-statement-list
//? description:...
    const declarations with initialisers in statement positions:
    case Expression : StatementList
//? expect: 1
//? source: ...
#!/usr/bin/env afw

switch (true) { case true: const x = 1; }


//? test: with-initializer-default-statement-list
//? description:...
    const declarations with initialisers in statement positions:
    default : StatementList
//? expect: 1
//? source: ...
#!/usr/bin/env afw

switch (true) { default: const x = 1; }


//? test: with-initializer-do-statement-while-expression
//? description:...
    const declarations with initialisers in statement positions:
    do Statement while ( Expression )
//? expect: error
//? source: ...
#!/usr/bin/env afw

do const x = 1; while (false)


//? test: with-initializer-for-statement
//? description:...
    const declarations with initialisers in statement positions:
    for ( ;;) Statement
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

// this is OK in Adaptive Script for now
for (;false;) const x = 1;


//? test: with-initializer-if-expression-statement-else-statement
//? description:...
    const declarations with initialisers in statement positions:
    if ( Expression ) Statement else Statement
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

// this is OK in Adaptive Script for now
if (true) {} else const x = 1;


//? test: with-initializer-if-expression-statement
//? description:...
    const declarations with initialisers in statement positions:
    if ( Expression ) Statement
//? expect: 1
//? source: ...
#!/usr/bin/env afw

// this is OK in Adaptive Script for now
if (true) const x = 1;


//? test: with-initializer-label-statement
//? description:...
    const declarations with initialisers in statement positions:
    label: Statement
//? expect: error
//? source: ...
#!/usr/bin/env afw

label: const x = 1;


//? test: with-initializer-while-expression-statement
//? description:...
    const declarations with initialisers in statement positions:
    while ( Expression ) Statement
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

// This is OK in Adaptive Script for now
while (false) const x = 1;


//? test: without-initializer-case-expression-statement-list
//? description:...
    const declarations without initialisers in statement positions:
    case Expression : StatementList
//? expect: error
//? source: ...
#!/usr/bin/env afw

switch (true) { case true: const x; }


//? test: without-initializer-default-statement-list
//? description:...
    const declarations without initialisers in statement positions:
    default : StatementList
//? expect: error
//? source: ...
#!/usr/bin/env afw

switch (true) { default: const x; }


//? test: without-initializer-do-statement-while-expression
//? description:...
    const declarations without initialisers in statement positions:
    do Statement while ( Expression )
//? expect: error
//? source: ...
#!/usr/bin/env afw

do const x; while (false)


//? test: without-initializer-for-statement
//? description:...
    const declarations without initialisers in statement positions:
    for ( ;;) Statement
//? expect: error
//? source: ...
#!/usr/bin/env afw

for (;false;) const x;


//? test: without-initializer-if-expression-statement-else-statement
//? description:...
    const declarations without initialisers in statement positions:
    if ( Expression ) Statement else Statement
//? expect: error
//? source: ...
#!/usr/bin/env afw

if (true) {} else const x;


//? test: without-initializer-if-expression-statement
//? description:...
    const declarations without initialisers in statement positions:
    if ( Expression ) Statement
//? expect: error
//? source: ...
#!/usr/bin/env afw

if (true) const x;


//? test: without-initializer-label-statement
//? description:...
    const declarations without initialisers in statement positions:
    label: Statement
//? expect: error
//? source: ...
#!/usr/bin/env afw

label: const x;


//? test: without-initializer-while-expression-statement
//? description:...
    const declarations without initialisers in statement positions:
    while ( Expression ) Statement
//? expect: error:Parse error at offset 41 around line 3 column 22: Expecting '='
//? source: ...
#!/usr/bin/env afw

while (false) const x;
