#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: compound-assignment.as
//? customPurpose: Part of test262
//? description: expressions/compound-assignment
//? sourceType: script
//?
//? test: 11.13.2-10-s
//? description:  ReferenceError is thrown if the LeftHandSideExpression of a Compound Assignment operator(^=) evaluates to an unresolvable reference 
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function _11_13_2_10
//? source: ...
#!/usr/bin/env afw

_11_13_2_10 ^= 1;


//? test: 11.13.2-11-s
//? description:  ReferenceError is thrown if the LeftHandSideExpression of a Compound Assignment operator(|=) evaluates to an unresolvable reference 
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function _11_13_2_11
//? source: ...
#!/usr/bin/env afw

_11_13_2_11 |= 1;


//? test: 11.13.2-1-s
//? description:  ReferenceError is thrown if the LeftHandSideExpression of a Compound Assignment operator(*=) evaluates to an unresolvable reference 
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function _11_13_2_1
//? source: ...
#!/usr/bin/env afw

_11_13_2_1 *= 1;


//? test: 11.13.2-2-s
//? description:  Strict Mode - ReferenceError is thrown if the LeftHandSideExpression of a Compound Assignment operator(/=) evaluates to an unresolvable reference 
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function _11_13_2_2
//? source: ...
#!/usr/bin/env afw

_11_13_2_2 /= 1;


//? test: 11.13.2-4-s
//? description:  Strict Mode - ReferenceError is thrown if the LeftHandSideExpression of a Compound Assignment operator(+=) 
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function _11_13_2_4
//? source: ...
#!/usr/bin/env afw

_11_13_2_4 += 1;


//? test: 11.13.2-5-s
//? description:  Strict Mode - ReferenceError is thrown if the LeftHandSideExpression of a Compound Assignment operator(-=) 
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function _11_13_2_5
//? source: ...
#!/usr/bin/env afw

_11_13_2_5 -= 1;


//? test: 11.13.2-9-s
//? description:  Strict Mode - ReferenceError is thrown if the LeftHandSideExpression of a Compound Assignment operator(&=) 
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function _11_13_2_9
//? source: ...
#!/usr/bin/env afw

_11_13_2_9 &= 1;


//? test: 11.13.2-12-s
//? description:  ReferenceError isn't thrown if the LeftHandSideExpression of a Compound Assignment operator(*=) evaluates to a resolvable reference 
//? expect: null
//? source: ...
#!/usr/bin/env afw

        let _11_13_2_12 = 5;
        _11_13_2_12 *= 2;

assert(_11_13_2_12 === 10, '_11_13_2_12');


//? test: 11.13.2-13-s
//? description:  ReferenceError isn't thrown if the LeftHandSideExpression of a Compound Assignment operator(/=) evaluates to a resolvable reference 
//? expect: null
//? source: ...
#!/usr/bin/env afw

        let _11_13_2_13 = 6;
        _11_13_2_13 /= 2;

assert(_11_13_2_13 === 3, '_11_13_2_13');


//? test: 11.13.2-14-s
//? description:  ReferenceError isn't thrown if the LeftHandSideExpression of a Compound Assignment operator(%=) evaluates to a resolvable reference 
//? expect: null
//? source: ...
#!/usr/bin/env afw

        let _11_13_2_14 = 5;
        _11_13_2_14 %= 2;

assert(_11_13_2_14 === 1, '_11_13_2_14');


//? test: 11.13.2-16-s
//? description:  ReferenceError isn't thrown if the LeftHandSideExpression of a Compound Assignment operator(-=) evaluates to a resolvable reference 
//? expect: null
//? source: ...
#!/usr/bin/env afw

        let _11_13_2_16 = 5;
        _11_13_2_16 -= 2;

assert(_11_13_2_16 === 3, '_11_13_2_16');


//? test: 11.13.2-19-s
//? description:  ReferenceError isn't thrown if the LeftHandSideExpression of a Compound Assignment operator(+=) evaluates to a resolvable reference 
//? expect: null
//? source: ...
#!/usr/bin/env afw

        let _11_13_2_19 = -1;
        _11_13_2_19 += 10;

assert(_11_13_2_19 === 9, '_11_13_2_19');


//? test: S11.13.2_A3.1_T1
//? description: Checking Expression and Variable statements for x *= y
//? expect: null
//? source: ...

//CHECK#1
let x = 1;
x *= -1; 
if (x !== -1) {
 throw '#1: let x = 1; x *= -1; x === -1. Actual: ' + (x);
}

//CHECK#2
let y;
y = 1;
y *= -1;
if (y !== -1) {
  throw '#2: y = 1; y *= -1; y === -1. Actual: ' + (y);
}


//? test: S11.13.2_A3.1_T2
//? description: Checking Expression and Variable statements for x /= y
//? expect: null
//? source: ...

//CHECK#1
let x = 1;
x /= -1; 
if (x !== -1) {
  throw '#1: let x = 1; x /= -1; x === -1. Actual: ' + (x);
}

//CHECK#2
let y;
y = 1;
y /= -1;
if (y !== -1) {
  throw '#2: y = 1; y /= -1; y === -1. Actual: ' + (y);
}


//? test: S11.13.2_A3.1_T3
//? description: Checking Expression and Variable statements for x %= y
//? expect: null
//? source: ...

//CHECK#1
let x = -1;
x %= 2; 
if (x !== -1) {
  throw '#1: let x = -1; x %= 2; x === -1. Actual: ' + (x);
}

//CHECK#2
let y;
y = -1;
y %= 2;
if (y !== -1) {
  throw '#2: y = -1; y %= 2; y === -1. Actual: ' + (y);
}


//? test: S11.13.2_A3.1_T4
//? description: Checking Expression and Variable statements for x += y
//? expect: null
//? source: ...

//CHECK#1
let x = 1;
x += 1; 
if (x !== 2) {
  throw '#1: let x = 1; x += 1; x === 2. Actual: ' + (x);
}

//CHECK#2
let y;
y = 1;
y += 1;
if (y !== 2) {
  throw '#2: y = 1; y += 1; y === 2. Actual: ' + (y);
}


//? test: S11.13.2_A3.1_T5
//? description: Checking Expression and Variable statements for x -= y
//? expect: null
//? source: ...

//CHECK#1
let x = -1;
x -= 1; 
if (x !== -2) {
  throw '#1: let x = -1; x -= 1; x === -2. Actual: ' + (x);
}

//CHECK#2
let y;
y = -1;
y -= 1;
if (y !== -2) {
  throw '#2: y = -1; y -= 1; y === -2. Actual: ' + (y);
}


//? test: S11.13.2_A4.1_T1.2
//? description: The production x *= y is the same as x = x I y
//? expect: null
//? source: ...

let x;

//CHECK#1
x = 1;
x *= 1;
if (x !== 1) {
  throw '#1: x = 1; x *= 1; x === 1. Actual: ' + (x);
}

//? test: S11.13.2_A4.3_T1.2
//? description: The production x %= y is the same as x = x % y
//? expect: null
//? source: ...

let x;

//CHECK#1
x = 1;
x %= 1;
if (x !== 0) {
  throw '#1: x = 1; x %= 1; x === 0. Actual: ' + (x);
}


//? test: add-non-simple
//? description: Compound addition assignment with non-simple target
//? expect: error:Invalid assignment target
//? source: ...

1 += 1;


//? test: btws-and-non-simple
//? description: It is an early Syntax Error if AssignmentTargetType of LeftHandSideExpression is invalid or strict.
//? expect: error
//? source: ...

1 &= 1;


//? test: btws-or-non-simple
//? description: It is an early Syntax Error if AssignmentTargetType of LeftHandSideExpression is invalid or strict.
//? expect: error
//? source: ...

1 |= 1;


//? test: div-non-simple
//? description: It is an early Syntax Error if AssignmentTargetType of LeftHandSideExpression is invalid or strict.
//? expect: error
//? source: ...

1 /= 1;


//? test: mod-div-non-simple
//? description: It is an early Syntax Error if AssignmentTargetType of LeftHandSideExpression is invalid or strict.
//? expect: error
//? source: ...

1 %= 1;


//? test: mult-non-simple
//? description: It is an early Syntax Error if AssignmentTargetType of LeftHandSideExpression is invalid or strict.
//? expect: error
//? source: ...

1 *= 1;


//? test: subtrace-non-simple
//? description: It is an early Syntax Error if AssignmentTargetType of LeftHandSideExpression is invalid or strict.
//? expect: error
//? source: ...

1 -= 1;