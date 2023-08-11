#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: assignment.as
//? customPurpose: Part of test262
//? description: expressions/assignment
//? sourceType: script
//?
//? test: S11.13.1_A2.1_T1
//? description: Either AssigmentExpression is not Reference or GetBase is not null
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

let x;
let y;

//CHECK#1
x = 1;
if (x !== 1) {
  throw '#1: x = 1; x === 1. Actual: ' + (x);
}

//CHECK#2
x = 1;
if (x !== 1) {
  throw '#2: var x = 1; x === 1. Actual: ' + (x);
}

//CHECK#3
y = 1;
x = y;
if (x !== 1) {
  throw '#3: y = 1; x = y; x === 1. Actual: ' + (x);
}

//CHECK#4
y = 1;
x = y;
if (x !== 1) {
  throw '#4: var y = 1; var x = y; x === 1. Actual: ' + (x);
}

//? test: 11.13.1_A2.1_T2
//? description: If GetBase(AssigmentExpression) is null, throw ReferenceError
//? expect: error:Parse error at offset 28 around line 3 column 9: Unknown built-in function 'y'
//? source: ...
#!/usr/bin/env afw

let x = y;


//? test: 11.13.1_A3.1
//? description: Checking Expression and Variable statements
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

//CHECK#1
let x = 1;
if (x !== 1) {
  throw '#1: var x = 1; x === 1. Actual: ' + (x);
}

//CHECK#2
x = 1;
if (x !== 1) {
  throw '#2: x = 1; x === 1. Actual: ' + (x);
}


//? test: 11.13.1_A4_T2
//? description: Syntax check if "x = x" throws ReferenceError
//? expect: error
//? skip: true
//? source: ...
#!/usr/bin/env afw

// This seems to work when perhaps it shouldn't?
let x = x;


//? test: S8.12.5_A2
//? description: When the [[Put]] method of O is called with property P and value V, then set the value of the property to V. The attributes of the property are not changed
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

let _map={"1":"one", "two":2};

//////////////////////////////////////////////////////////////////////////////
//CHECK#1
_map["1"] = "uno";
if (_map["1"] !== "uno") {
  throw '#1: _map["1"] = "uno"; _map["1"] === "uno". Actual: ' + (_map["1"]);
}

//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//CHECK#2
_map["1"]=1;
if (_map["1"] !== 1) {
  throw '#2: var _map={1:"one",two:2}; _map[1]="uno"; _map["1"]=1; _map[1] === 1. Actual: ' + (_map[1]);
}
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//CHECK#3
_map["two"]="two";
if (_map["two"] !== "two") {
  throw '#3: var _map={1:"one",two:2}; _map[1]="uno"; _map["1"]=1; _map["two"]="two"; _map["two"] === "two". Actual: ' + (_map["two"]);
}
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//CHECK#4
_map.two="duo";
if (_map.two !== "duo") {
  throw '#4: var _map={1:"one",two:2}; _map[1]="uno"; _map["1"]=1; _map["two"]="two"; _map.two="duo"; _map.two === "duo". Actual: ' + (_map.two);
}
//
//////////////////////////////////////////////////////////////////////////////


//? test: line-terminator
//? description: White Space between LeftHandSideExpression and "=" or between "=" and AssignmentExpression is allowed
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

let x;

x
=
true;

if (x !== true) {
    throw '#6: (x\\u000A=\\u000Atrue) === true';
}


//? test: member-expr-ident-name-break-escaped
//? description: break is a valid identifier name, using escape (MemberExpression IdentifierName)
//? skip: true 
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

let obj = {};

// FIXME don't know if we should support this
obj.bre\u0061k = 42;

assert(property_exists(obj, "break"));


//? test: member-expr-ident-name-default
//? description: default is a valid identifier name (MemberExpression IdentifierName)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

let obj = {};

obj.default = 42;

assert(property_exists(obj, "default"));


//? test: non-simple-target
//? description: It is an early Syntax Error if LeftHandSideExpression is neither an ObjectLiteral nor an ArrayLiteral and AssignmentTargetType of LeftHandSideExpression is invalid or strict.
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 = 1;


//? test: target-assignment-inside-function
//? description: It is an early Syntax Error if LeftHandSideExpression is neither an ObjectLiteral nor an ArrayLiteral and AssignmentTargetType of LeftHandSideExpression is invalid or strict.
//? expect: error
//? source: ...
#!/usr/bin/env afw

function f() {
    let a, b = 2;
    (a = b) = 1;
}


//? test: target-assignment
//? description: It is an early Syntax Error if LeftHandSideExpression is neither an ObjectLiteral nor an ArrayLiteral and AssignmentTargetType of LeftHandSideExpression is invalid or strict.
//? expect: error
//? source: ...
#!/usr/bin/env afw

let a, b = 2;
(a = b) = 1;


//? test: target-boolean
//? description: It is an early Syntax Error if LeftHandSideExpression is neither an ObjectLiteral nor an ArrayLiteral and AssignmentTargetType of LeftHandSideExpression is invalid or strict.
//? expect: error
//? source: ...
#!/usr/bin/env afw

true = 42;


//? test: target-cover-id
//? description: It is an early Reference Error if LeftHandSideExpression is neither an ObjectLiteral nor an ArrayLiteral and IsValidSimpleAssignmentTarget of LeftHandSideExpression is false.
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

let x;

(x) = 1;

assert(x === 1);


//? test: target-member-identifier-reference-null
//? description: Assignment Operator evaluates the value prior validating a MemberExpression's reference (null)
//? expect: error
//? skip: true
//? source: ...
#!/usr/bin/env afw

let count = 0;
let base = null;

// we can't test this because assignment statement can't be used in expression
base.prop = count += 1;


//? test: target-member-computed-reference-undefined
//? description: Assignment Operator evaluates the value prior validating a MemberExpression's reference (undefined)
//? expect: error:Parse error at offset 175 around line 9 column 11: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw

// we can't do some of these
function DummyError() { }

assert.throws(DummyError, function() {
  let base = undefined;
  let prop = function() {
    throw new DummyError();
  };
  let expr = function() {
    throw new Test262Error("right-hand side expression evaluated");
  };

  base[prop()] = expr();
});

assert.throws(DummyError, function() {
  let base = undefined;
  let prop = {
    toString: function() {
      throw new Test262Error("property key evaluated");
    }
  };
  let expr = function() {
    throw new DummyError();
  };

  base[prop] = expr();
});


//? test: target-null
//? description: simple assignment throws SyntaxError if LeftHandSide is not a reference (null)
//? expect: error
//? source: ...
#!/usr/bin/env afw

null = 42;


//? test: target-number
//? description:  simple assignment throws SyntaxError if LeftHandSide is not a reference (number)
//? expect: error
//? source: ...
#!/usr/bin/env afw

42 = 42;


//? test: target-string
//? description:  simple assignment throws SyntaxError if LeftHandSide is not a reference (string)
//? expect: error
//? source: ...
#!/usr/bin/env afw

'x' = 42;


//? test: whitespace
//? description:  White Space between LeftHandSideExpression and "=" or between "=" and AssignmentExpression is allowed
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

let x;

x	=	'U+0009';
if (x !== 'U+0009') {
  throw '#1: (x\\u0009=\\u0009true) === true';
}

x='U+000B';
if (x !== 'U+000B') {
  throw'#2: (x\\u000B=\\u000Btrue) === true';
}

x='U+000C';
if (x !== 'U+000C') {
  throw'#3: (x\\u000C=\\u000Ctrue) === true';
}

x = 'U+0020';
if (x !== 'U+0020') {
  throw'#4: (x\\u0020=\\u0020true) === true';
}

x = 'U+00A0';
if (x !== 'U+00A0') {
  throw'#5: (x\\u00A0=\\u00A0true) === true';
}

x
=
'U+000D';
if (x !== 'U+000D') {
  throw'#7: (x\\u000D=\\u000Dtrue) === true';
}

x = 'U+2028';
if (x !== 'U+2028') {
  throw'#8: (x\\u2028=\\u2028true) === true';
}

x = 'U+2029';
if (x !== 'U+2029') {
  throw'#9: (x\\u2029=\\u2029true) === true';
}

x	  
  =	  
  'U+0009U+000BU+000CU+0020U+00A0U+000DU+2028U+2029';
if (x !== 'U+0009U+000BU+000CU+0020U+00A0U+000DU+2028U+2029') {
  throw'#10: (x\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000D\\u2028\\u2029=\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000D\\u2028\\u2029true) === true';
}
