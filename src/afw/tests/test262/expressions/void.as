#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: void.as
//? customPurpose: Part of test262
//? description: expressions/void
//? sourceType: script
//?
//? test: S11.4.2_A1
//? description: Checking by using eval
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

//CHECK#1
if (evaluate(script("void\u00090")) !== undefined) {
  throw '#1: void\\u00090 === undefined';
}

//CHECK#2
if (evaluate(script("void\u000B0")) !== undefined) {
  throw '#2: void\\u000B0 === undefined';
}

//CHECK#3
if (evaluate(script("void\u000C0")) !== undefined) {
  throw '#3: void\\u000C0 === undefined';
}

//CHECK#4
if (evaluate(script("void\u00200")) !== undefined) {
  throw '#4: void\\u00200 === undefined';
}

//CHECK#5
if (evaluate(script("void\u00A00")) !== undefined) {
  throw '#5: void\\u00A00 === undefined';
}

//CHECK#6
if (evaluate(script("void\u000A0")) !== undefined) {
  throw '#6: void\\u000A0 === undefined';
}

//CHECK#7
if (evaluate(script("void\u000D0")) !== undefined) {
  throw '#7: void\\u000D0 === undefined';
}

//CHECK#8
if (evaluate(script("void\u20280")) !== undefined) {
  throw '#8: void\\u20280 === undefined';
}

//CHECK#9
if (evaluate(script("void\u20290")) !== undefined) {
  throw '#9: void\\u20290 === undefined';
}

//CHECK#10
if (evaluate(script("void\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u20290")) !== undefined) {
  throw '#10: void\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u20290 === undefined';
}


//? test: S11.4.2_A2_T1
//? description: Either Type(x) is not Reference or GetBase(x) is not null
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

//CHECK#1
if (void 0 !== undefined) {
  throw '#1: void 0 === undefined. Actual: ' + (void 0);
}

//CHECK#2
let x = 0;
if (void x !== undefined) {
  throw '#2: let x = 0; void x === undefined. Actual: ' + (void x);
}

//CHECK#3
/*
let x = new Object();
if (void x !== undefined) {
  throw '#3: let x = new Object(); void x === undefined. Actual: ' + (void x);
}
*/


//? test: S11.4.2_A2_T2
//? description: If GetBase(x) is null, throw ReferenceError
//? expect: error:Parse error at offset 25 around line 3 column 6: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

void x;


//? test: S11.4.2_A4_T1
//? description: Type(x) is boolean primitive or Boolean object
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

//CHECK#1
let x = false;
if (void x !== undefined) {
  throw '#1: let x = false; void x === undefined. Actual: ' + (void x);
}

//CHECK#2
/*
let x = new Boolean(true);
if (void x !== undefined) {
  throw '#2: let x = new Boolean(true); void x === undefined. Actual: ' + (void x);
}
*/


//? test: S11.4.2_A4_T2
//? description: Type(x) is number primitive or Number object
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

//CHECK#1
let x = 0.1;
if (void x !== undefined) {
  throw '#1: let x = 0.1; void x === undefined. Actual: ' + (void x);
}

//CHECK#2
/*
let x = new Number(-1.1);
if (void x !== undefined) {
  throw '#2: let x = new Number(-1.1); void x === undefined. Actual: ' + (void x);
}
*/


//? test: S11.4.2_A4_T3
//? description: Type(x) is string primitive of String object
//? skip: true
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

//CHECK#1
let x = "1";
if (void x !== undefined) {
  throw '#1: let x = "1"; void x === undefined. Actual: ' + (void x);
}

//CHECK#2
x = "x";
if (isNaN(void x) !== true) {
  throw '#2: let x = "x"; void x === undefined. Actual: ' + (void x);
}

//CHECK#3
/*
let x = new String("-1");
if (void x !== undefined) {
  throw '#3: let x = new String("-1"); void x === undefined. Actual: ' + (void x);
}
*/


//? test: S11.4.2_A4_T4
//? description: Type(x) is undefined or null
//? skip: true
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

//CHECK#1
let x;
if (isNaN(void x) !== true) {
  throw '#1: let x; void x === undefined. Actual: ' + (void x);
}

//CHECK#2
x = null;
if (void x !== undefined) {
  throw '#2: let x = null; void x === undefined. Actual: ' + (void x);
}


//? test: S11.4.2_A4_T5
//? description: Type(x) is Object object or Function object
//? skip: true
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

//CHECK#1
let x = {};
if (isNaN(void x) !== true) {
  throw '#1: let x = {}; void x === undefined. Actual: ' + (void x);
}

//CHECK#2
x = function(){return 1};
if (isNaN(void x) !== true) {
  throw '#2: let x = function(){return 1}; void x === undefined. Actual: ' + (void x);
}


//? test: S11.4.2_A4_T6
//? description: Checking Simple Assignment operator
//? expect: error:Parse error at offset 103 around line 6 column 13: Expecting ')'
//? source: ...
#!/usr/bin/env afw

//CHECK#1
let x = 0;
// we do not allow for assignments in expressions
if (void (x = 1) !== undefined) {
  throw '#1: let x = 0; void (x = 1) === undefined. Actual: ' + (void (x = 1));
} else {
  if (x !== 1) {
    throw '#1: let x = 0; void (x = 1); x === 1. Actual: ' + (x);
  }
}