#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: redeclaration.as
//? customPurpose: Part of test262
//? description: block-scope/syntax/redeclaration
//? sourceType: script
//?
//? test: const-name-redeclaration-attempt-with-function
//? description: redeclaration with FunctionDeclaration (LexicalDeclaration (const) in BlockStatement)
//? expect: error
//? source: ...

{ const f = 0; function f() {} }


//? test: const-name-redeclaration-attempt-with-let
//? description: redeclaration with let-LexicalDeclaration (LexicalDeclaration (const) in BlockStatement)
//? expect: error
//? source: ...

{ const f = 0; let f; }


//? test: function-name-redeclaration-attempt-with-const
//? description: redeclaration with const-LexicalDeclaration (FunctionDeclaration in BlockStatement)
//? expect: error
//? source: ...

{ function f() {}; const f = 0; }


//? test: function-name-redeclaration-attempt-with-function
//? description: redeclaration with FunctionDeclaration (FunctionDeclaration in BlockStatement)
//? expect: error
//? source: ...

{ function f() {}; function f() {}; }


//? test: function-name-redeclaration-attempt-with-let
//? description: redeclaration with let-LexicalDeclaration (FunctionDeclaration in BlockStatement)
//? expect: error
//? source: ...

{ function f() {}; let f; }


//? test: let-name-redeclaration-attempt-with-const
//? description: redeclaration with const-LexicalDeclaration (LexicalDeclaration (let) in BlockStatement)
//? expect: error
//? source: ...

{ let f; const f = 0; }


//? test: let-name-redeclaration-attempt-with-function
//? description: redeclaration with FunctionDeclaration (LexicalDeclaration (let) in BlockStatement)
//? expect: error
//? source: ...

{ let f; function f() {}; }


//? test: let-name-redeclaration-attempt-with-let
//? description: redeclaration with let-LexicalDeclaration (LexicalDeclaration (let) in BlockStatement)
//? expect: error
//? source: ...

{ let f; let f; }