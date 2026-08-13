#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: modulus.as
//? customPurpose: Part of test262
//? description: expressions/modulus
//? sourceType: script
//?
//? test: line-terminator
//? description: Line terminator between the operands of a modulus operator
//? expect: success
//? source: ...
#!/usr/bin/env afw


let x = 18

%

7

%

3
;

assert(x === 1);
//? test: order-of-evaluation
//? description: Type coercion order of operations for modulus operator
//? expect: 0
//? source: ...

let saw = "";
function xf() { saw = saw + "x"; throw "x"; }
function yf() { saw = saw + "y"; throw "y"; }
try {
    let unused = xf() % yf();
    assert(false);
} catch (e) {
    assert(e.message === "x");
    assert(saw === "x");
}
return 0;

//?
//? test: S11.5.3_A1
//? description: Checking by using eval
//? expect: success
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (eval(script("1\u0009%\u00091")) !== 0) {
  throw '#1: 1\\u0009%\\u00091 === 0';
}

//CHECK#2
if (eval(script("1\u000B%\u000B1")) !== 0) {
  throw '#2: 1\\u000B%\\u000B1 === 0';
}

//CHECK#3
if (eval(script("1\u000C%\u000C1")) !== 0) {
  throw '#3: 1\\u000C%\\u000C1 === 0';
}

//CHECK#4
if (eval(script("1\u0020%\u00201")) !== 0) {
  throw '#4: 1\\u0020%\\u00201 === 0';
}

//CHECK#5
if (eval(script("1\u00A0%\u00A01")) !== 0) {
  throw '#5: 1\\u00A0%\\u00A01 === 0';
}

//CHECK#6
if (eval(script("1\u000A%\u000A1")) !== 0) {
  throw '#6: 1\\u000A%\\u000A1 === 0';
}

//CHECK#7
if (eval(script("1\u000D%\u000D1")) !== 0) {
  throw '#7: 1\\u000D%\\u000D1 === 0';
}

//CHECK#8
if (eval(script("1\u2028%\u20281")) !== 0) {
  throw '#8: 1\\u2028%\\u20281 === 0';
}

//CHECK#9
if (eval(script("1\u2029%\u20291")) !== 0) {
  throw '#9: 1\\u2029%\\u20291 === 0';
}

//CHECK#10
if (eval(script("1\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u2029%\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u20291")) !== 0) {
  throw '#10: 1\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u2029%\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u20291 === 0';
}
//? test: S11.5.3_A2.1_T1
//? description: Either Type is not Reference or GetBase is not null
//? expect: success
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (1 % 2 !== 1) {
  throw '#1: 1 % 2 === 1. Actual: ' + (1 % 2);
}

//CHECK#2
let x = 1;
if (x % 2 !== 1) {
  throw '#2: let x = 1; x % 2 === 1. Actual: ' + (x % 2);
}

//CHECK#3
let y = 2;
if (1 % y !== 1) {
  throw '#3: let y = 2; 1 % y === 1. Actual: ' + (1 % y);
}

//CHECK#4
x = 1;
y = 2;
if (x % y !== 1) {
  throw '#4: let x = 1; let y = 2; x % y === 1. Actual: ' + (x % y);
}
//? test: S11.5.3_A2.1_T2
//? description: If GetBase(x) is null, throw ReferenceError
//? expect: error
//? source: ...
#!/usr/bin/env afw

// undeclared x is a compile error
x % 1;


//? test: S11.5.3_A2.1_T3
//? description: If GetBase(y) is null, throw ReferenceError
//? expect: error
//? source: ...
#!/usr/bin/env afw

// undeclared y is a compile error
1 % y;


//? test: S11.5.3_A2.3_T1
//? description: Checking with "throw"
//? expect: success
//? skip: true
//? skipReason: Never: Adaptive does not convert objects to numbers via valueOf
//? source: ...
#!/usr/bin/env afw


//CHECK#1
let x = { valueOf: function () { throw "x"; } };
let y = { valueOf: function () { throw "y"; } };
try {
   x % y;
   throw '#1.1: let x = { valueOf: function () { throw "x"; } }; let y = { valueOf: function () { throw "y"; } }; x % y throw "x". Actual: ' + (x % y);
} catch (e) {
   if (e === "y") {
     throw '#1.2: ToNumber(first expression) is called first, and then ToNumber(second expression)';
   } else {
     if (e !== "x") {
       throw '#1.3: let x = { valueOf: function () { throw "x"; } }; let y = { valueOf: function () { throw "y"; } }; x % y throw "x". Actual: ' + (e);
     }
   }
}
//? test: S11.5.3_A2.4_T2
//? description: Checking with "throw"
//? expect: 0
//? source: ...

let saw = "";
function xf() { saw = saw + "x"; throw "x"; }
function yf() { saw = saw + "y"; throw "y"; }
try {
    let unused = xf() % yf();
    assert(false);
} catch (e) {
    assert(e.message === "x");
    assert(saw === "x");
}
return 0;
//? test: S11.5.3_A2.4_T3
//? description: Checking with undeclarated variables
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

x % (x = 1);


//? test: S11.5.3_A3_T1.2
//? description: Type(x) and Type(y) vary between primitive number and Number object
//? expect: success
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (1 % 1 !== 0) {
  throw '#1: 1 % 1 === 0. Actual: ' + (1 % 1);
}
//? test: S11.5.3_A3_T1.3
//? description: Type(x) and Type(y) vary between primitive string and String object
//? skip: true
//? skipReason: ...
Never: Adaptive % does not ToNumber-coerce strings (no new String)
//? expect: success
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ("1" % "1" !== 0) {
  throw '#1: "1" % "1" === 0. Actual: ' + ("1" % "1");
}

//CHECK#2
if (new String("1") % "1" !== 0) {
  throw '#2: new String("1") % "1" === 0. Actual: ' + (new String("1") % "1");
}

//CHECK#3
if ("1" % new String("1") !== 0) {
  throw '#3: "1" % new String("1") === 0. Actual: ' + ("1" % new String("1"));
}

//CHECK#4
if (new String("1") % new String("1") !== 0) {
  throw '#4: new String("1") % new String("1") === 0. Actual: ' + (new String("1") % new String("1"));
}

//CHECK#5
if (is_NaN("x" % "1") !== true) {
  throw '#5: "x" % "1" === Not-a-Number. Actual: ' + ("x" % "1");
}

//CHECK#6
if (is_NaN("1" % "x") !== true) {
  throw '#6: "1" % "x" === Not-a-Number. Actual: ' + ("1" % "x");
}
//? test: S11.5.3_A3_T1.4
//? description: Type(x) and Type(y) vary between Null and Undefined
//? skip: true
//? skipReason: Incompatible: Adaptive % does not ToNumber-coerce (null/string/object); integer or double only
//? expect: success
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN(null % undefined) !== true) {
  throw '#1: null % undefined === Not-a-Number. Actual: ' + (null % undefined);
}

//CHECK#2
if (is_NaN(undefined % null) !== true) {
  throw '#2: undefined % null === Not-a-Number. Actual: ' + (undefined % null);
}

//CHECK#3
if (is_NaN(undefined % undefined) !== true) {
  throw '#3: undefined % undefined === Not-a-Number. Actual: ' + (undefined % undefined);
}

//CHECK#4
if (is_NaN(null % null) !== true) {
  throw '#4: null % null === Not-a-Number. Actual: ' + (null % null);
}
//? test: S11.5.3_A3_T1.5
//? description: Type(x) and Type(y) vary between Object object and Function object
//? skip: true
//? skipReason: Incompatible: Adaptive % does not ToNumber-coerce (null/string/object); integer or double only
//? expect: success
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN({} % function(){return 1}) !== true) {
  throw '#1: {} % function(){return 1} === Not-a-Number. Actual: ' + ({} % function(){return 1});
}

//CHECK#2
if (is_NaN(function(){return 1} % {}) !== true) {
  throw '#2: function(){return 1} % {} === Not-a-Number. Actual: ' + (function(){return 1} % {});
}

//CHECK#3
if (is_NaN(function(){return 1} % function(){return 1}) !== true) {
  throw '#3: function(){return 1} % function(){return 1} === Not-a-Number. Actual: ' + (function(){return 1} % function(){return 1});
}

//CHECK#4
if (is_NaN({} % {}) !== true) {
  throw '#4: {} % {} === Not-a-Number. Actual: ' + ({} % {});
}
//? test: S11.5.3_A3_T2.2
//? description:...
    Type(x) is different from Type(y) and both types vary between
    Number (primitive or object) and String (primitive and object)
//? skip: true
//? skipReason: Incompatible: Adaptive % does not ToNumber-coerce (null/string/object); integer or double only
//? expect: success
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ("1" % 1 !== 0) {
  throw '#1: "1" % 1 === 0. Actual: ' + ("1" % 1);
}

//CHECK#2
if (1 % "1" !== 0) {
  throw '#2: 1 % "1" === 0. Actual: ' + (1 % "1");
}

//CHECK#9
if (is_NaN("x" % 1) !== true) {
  throw '#9: "x" % 1 === Not-a-Number. Actual: ' + ("x" % 1);
}

//CHECK#10
if (is_NaN(1 % "x") !== true) {
  throw '#10: 1 % "x" === Not-a-Number. Actual: ' + (1 % "x");
}
//? test: S11.5.3_A3_T2.3
//? description:...
    Type(x) is different from Type(y) and both types vary between
    Number (primitive or object) and Null
//? skip: true
//? skipReason: Incompatible: Adaptive % does not ToNumber-coerce (null/string/object); integer or double only
//? expect: success
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN(1 % null) !== true) {
  throw '#1: 1 % null === Not-a-Number. Actual: ' + (1 % null);
}

//CHECK#2
if (null % 1 !== 0) {
  throw '#2: null % 1 === 0. Actual: ' + (null % 1);
}
//? test: S11.5.3_A3_T2.4
//? description:...
    Type(x) is different from Type(y) and both types vary between
    Number (primitive or object) and Undefined
//? skip: true
//? skipReason: Incompatible: Adaptive % does not ToNumber-coerce (null/string/object); integer or double only
//? expect: success
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN(1 % undefined) !== true) {
  throw '#1: 1 % undefined === Not-a-Number. Actual: ' + (1 % undefined);
}

//CHECK#2
if (is_NaN(undefined % 1) !== true) {
  throw '#2: undefined % 1 === Not-a-Number. Actual: ' + (undefined % 1);
}
//? test: S11.5.3_A3_T2.6
//? description:...
    Type(x) is different from Type(y) and both types vary between
    String (primitive or object) and Undefined
//? skip: true
//? skipReason: ...
Never: Adaptive % does not ToNumber-coerce strings or undefined
(no new String / valueOf)
//? expect: success
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN("1" % undefined) !== true) {
  throw '#1: "1" % undefined === Not-a-Number. Actual: ' + ("1" % undefined);
}

//CHECK#2
if (is_NaN(undefined % "1") !== true) {
  throw '#2: undefined % "1" === Not-a-Number. Actual: ' + (undefined % "1");
}

//CHECK#3
if (is_NaN(new String("1") % undefined) !== true) {
  throw '#3: new String("1") % undefined === Not-a-Number. Actual: ' + (new String("1") % undefined);
}

//CHECK#4
if (is_NaN(undefined % new String("1")) !== true) {
  throw '#4: undefined % new String("1") === Not-a-Number. Actual: ' + (undefined % new String("1"));
}
//? test: S11.5.3_A3_T2.7
//? description:...
    Type(x) is different from Type(y) and both types vary between
    String (primitive or object) and Null
//? skip: true
//? skipReason: ...
Never: Adaptive % does not ToNumber-coerce strings or undefined
(no new String / valueOf)
//? expect: success
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN("1" % null) !== true) {
  throw '#1: "1" % null === Not-a-Number. Actual: ' + ("1" % null);
}

//CHECK#2
if (null % "1" !== 0) {
  throw '#2: null % "1" === 0. Actual: ' + (null % "1");
}

//CHECK#3
if (is_NaN(new String("1") % null) !== true) {
  throw '#3: new String("1") % null === Not-a-Number. Actual: ' + (new String("1") % null);
}

//CHECK#4
if (null % new String("1") !== 0) {
  throw '#4: null % new String("1") === 0. Actual: ' + (null % new String("1"));
}
//? test: S11.5.3_A4_T1.1
//? description: If either operand is NaN, the result is NaN (left NaN; double mod)
//? expect: success
//? differences: Adaptive double % uses fmod (IEEE); integer % still throws on divide-by-zero
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN(NaN % NaN) !== true) {
  throw '#1: NaN % NaN === Not-a-Number';
}

//CHECK#2
if (is_NaN(NaN % 0.0) !== true) {
  throw '#2: NaN % 0.0 === Not-a-Number';
}

//CHECK#3
if (is_NaN(NaN % -0.0) !== true) {
  throw '#3: NaN % -0.0 === Not-a-Number';
}

//CHECK#4
if (is_NaN(NaN % Infinity) !== true) {
  throw '#4: NaN % Infinity === Not-a-Number';
}

//CHECK#5
if (is_NaN(NaN % -Infinity) !== true) {
  throw '#5: NaN % -Infinity === Not-a-Number';
}

//CHECK#6
if (is_NaN(NaN % 1.0) !== true) {
  throw '#6: NaN % 1.0 === Not-a-Number';
}
//? test: S11.5.3_A4_T1.2
//? description: If either operand is NaN, the result is NaN (right NaN; double mod)
//? expect: success
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN(NaN % NaN) !== true) {
  throw '#1: NaN % NaN === Not-a-Number';
}

//CHECK#2
if (is_NaN(0.0 % NaN) !== true) {
  throw '#2: 0.0 % NaN === Not-a-Number';
}

//CHECK#3
if (is_NaN(-0.0 % NaN) !== true) {
  throw '#3: -0.0 % NaN === Not-a-Number';
}

//CHECK#4
if (is_NaN(Infinity % NaN) !== true) {
  throw '#4: Infinity % NaN === Not-a-Number';
}

//CHECK#5
if (is_NaN(-Infinity % NaN) !== true) {
  throw '#5: -Infinity % NaN === Not-a-Number';
}

//CHECK#6
if (is_NaN(1.0 % NaN) !== true) {
  throw '#6: 1.0 % NaN === Not-a-Number';
}
//? test: S11.5.3_A4_T2
//? description:...
    The sign of the finite non-zero value result equals the sign of
    the divided
//? expect: error:Integer divide by zero error
//? source: ...
#!/usr/bin/env afw

// test262 expects 1/0 to be Infinity, where we throw errors instead.

//CHECK#1
if (1 % 1 !== 0) {
  throw '#1.1: 1 % 1 === 0. Actual: ' + (1 % 1);
} else {
  if (1 / (1 % 1) !== Infinity) {
    throw '#1.2: 1 % 1 === + 0. Actual: -0';
  }
}

//CHECK#2
if (-1 % -1 !== -0) {
  throw '#2.1: -1 % -1 === 0. Actual: ' + (-1 % -1);
} else {
  if (1 / (-1 % -1) !== -Infinity) {
    throw '#2.2: -1 % -1 === - 0. Actual: +0';
  }
}

//CHECK#3
if (-1 % 1 !== -0) {
  throw '#3.1: -1 % 1 === 0. Actual: ' + (-1 % 1);
} else {
  if (1 / (-1 % 1) !== -Infinity) {
    throw '#3.2: -1 % 1 === - 0. Actual: +0';
  }
}

//CHECK#4
if (1 % -1 !== 0) {
  throw '#4.1: 1 % -1 === 0. Actual: ' + (1 % -1);
} else {
  if (1 / (1 % -1) !== Infinity) {
    throw '#4.2: 1 % -1 === + 0. Actual: -0';
  }
}

//CHECK#5
if (101 % 51 !== 50) {
  throw '#5: 101 % 51 === 50. Actual: ' + (101 % 51);
}

//CHECK#6
if (101 % -51 !== 50) {
  throw '#6: 101 % -51 === 50. Actual: ' + (101 % -51);
}

//CHECK#7
if (-101 % 51 !== -50) {
  throw '#7: -101 % 51 === -50. Actual: ' + (-101 % 51);
}

//CHECK#8
if (-101 % -51 !== -50) {
  throw '#8: -101 % -51 === -50. Actual: ' + (-101 % -51);
}


//? test: S11.5.3_A4_T3
//? description: If the dividend is an infinity results is NaN (double mod)
//? expect: success
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN(-Infinity % Infinity) !== true) {
  throw '#1: -Infinity % Infinity === Not-a-Number';
}

//CHECK#2
if (is_NaN(-Infinity % -Infinity) !== true) {
  throw '#2: -Infinity % -Infinity === Not-a-Number';
}

//CHECK#3
if (is_NaN(Infinity % Infinity) !== true) {
  throw '#3: Infinity % Infinity === Not-a-Number';
}

//CHECK#4
if (is_NaN(Infinity % -Infinity) !== true) {
  throw '#4: Infinity % -Infinity === Not-a-Number';
}

//CHECK#5
if (is_NaN(-Infinity % 1.0) !== true) {
  throw '#5: -Infinity % 1.0 === Not-a-Number';
}

//CHECK#6
if (is_NaN(-Infinity % -1.0) !== true) {
  throw '#6: -Infinity % -1.0 === Not-a-Number';
}

//CHECK#7
if (is_NaN(Infinity % 1.0) !== true) {
  throw '#7: Infinity % 1.0 === Not-a-Number';
}

//CHECK#8
if (is_NaN(Infinity % -1.0) !== true) {
  throw '#8: Infinity % -1.0 === Not-a-Number';
}

//CHECK#9
if (is_NaN(Infinity % 1.7976931348623157e+308) !== true) {
  throw '#9: Infinity % max-double === Not-a-Number';
}
//? test: S11.5.3_A4_T4
//? description: If the divisor is zero results is NaN (double mod; not integer %)
//? expect: success
//? differences: Integer % still throws Integer divide by zero; double % yields NaN via fmod
//? source: ...
#!/usr/bin/env afw


// Use double literals so % resolves to mod<double> (fmod), not integer
// which throws on divide-by-zero.

//CHECK#1
if (is_NaN(-0.0 % 0.0) !== true) {
  throw '#1: -0.0 % 0.0 === Not-a-Number';
}

//CHECK#2
if (is_NaN(-0.0 % -0.0) !== true) {
  throw '#2: -0.0 % -0.0 === Not-a-Number';
}

//CHECK#3
if (is_NaN(0.0 % 0.0) !== true) {
  throw '#3: 0.0 % 0.0 === Not-a-Number';
}

//CHECK#4
if (is_NaN(0.0 % -0.0) !== true) {
  throw '#4: 0.0 % -0.0 === Not-a-Number';
}

//CHECK#5
if (is_NaN(-1.0 % 0.0) !== true) {
  throw '#5: -1.0 % 0.0 === Not-a-Number';
}

//CHECK#6
if (is_NaN(-1.0 % -0.0) !== true) {
  throw '#6: -1.0 % -0.0 === Not-a-Number';
}

//CHECK#7
if (is_NaN(1.0 % 0.0) !== true) {
  throw '#7: 1.0 % 0.0 === Not-a-Number';
}

//CHECK#8
if (is_NaN(1.0 % -0.0) !== true) {
  throw '#8: 1.0 % -0.0 === Not-a-Number';
}

//CHECK#9
if (is_NaN(Infinity % 0.0) !== true) {
  throw '#9: Infinity % 0.0 === Not-a-Number';
}

//CHECK#10
if (is_NaN(-Infinity % 0.0) !== true) {
  throw '#10: -Infinity % 0.0 === Not-a-Number';
}
//? test: S11.5.3_A4_T5
//? description:...
    If dividend is finite and the divisor is an infinity, the result
    equals the dividend
//? expect: success
//? source: ...
#!/usr/bin/env afw


// Double literals so % uses mod<double>

//CHECK#1
if (1.0 % -Infinity !== 1.0) {
  throw '#1: 1.0 % -Infinity === 1.0. Actual: ' + string(1.0 % -Infinity);
}
//CHECK#2
if (1.0 % Infinity !== 1.0) {
  throw '#2: 1.0 % Infinity === 1.0. Actual: ' + string(1.0 % Infinity);
}

//CHECK#3
if (-1.0 % Infinity !== -1.0) {
  throw '#3: -1.0 % Infinity === -1.0. Actual: ' + string(-1.0 % Infinity);
}

//CHECK#4
if (-1.0 % -Infinity !== -1.0) {
  throw '#4: -1.0 % -Infinity === -1.0. Actual: ' + string(-1.0 % -Infinity);
}

//CHECK#5
if (0.0 % Infinity !== 0.0) {
  throw '#5.1: 0.0 % Infinity === 0.0. Actual: ' + string(0.0 % Infinity);
} else {
  if (1.0 / (0.0 % Infinity) !== Infinity) {
    throw '#5.2: 0.0 % Infinity === +0';
  }
}

//CHECK#6
if (0.0 % -Infinity !== 0.0) {
  throw '#6.1: 0.0 % -Infinity === 0.0. Actual: ' + string(0.0 % -Infinity);
} else {
  if (1.0 / (0.0 % -Infinity) !== Infinity) {
    throw '#6.2: 0.0 % -Infinity === +0';
  }
}

//CHECK#7
if (-0.0 % Infinity !== -0.0) {
  throw '#7.1: -0.0 % Infinity === -0. Actual: ' + string(-0.0 % Infinity);
} else {
  if (1.0 / (-0.0 % Infinity) !== -Infinity) {
    throw '#7.2: -0.0 % Infinity === -0';
  }
}

//CHECK#8
if (-0.0 % -Infinity !== -0.0) {
  throw '#8.1: -0.0 % -Infinity === -0. Actual: ' + string(-0.0 % -Infinity);
} else {
  if (1.0 / (-0.0 % -Infinity) !== -Infinity) {
    throw '#8.2: -0.0 % -Infinity === -0';
  }
}
//? test: S11.5.3_A4_T6
//? description:...
    If dividend is a zero and the divisor is nonzero finite, the
    result equals the dividend
//? expect: success
//? source: ...
#!/usr/bin/env afw


// Double literals for signed-zero and fmod path

//CHECK#1
if (0.0 % 1.0 !== 0.0) {
  throw '#1.1: 0.0 % 1.0 === 0.0';
} else {
  if (1.0 / (0.0 % 1.0) !== Infinity) {
    throw '#1.2: 0.0 % 1.0 === +0';
  }
}

//CHECK#2
if (0.0 % -1.0 !== 0.0) {
  throw '#2.1: 0.0 % -1.0 === 0.0';
} else {
  if (1.0 / (0.0 % -1.0) !== Infinity) {
    throw '#2.2: 0.0 % -1.0 === +0';
  }
}

//CHECK#3
if (-0.0 % 1.0 !== -0.0) {
  throw '#3.1: -0.0 % 1.0 === -0';
} else {
  if (1.0 / (-0.0 % 1.0) !== -Infinity) {
    throw '#3.2: -0.0 % 1.0 === -0';
  }
}

//CHECK#4
if (-0.0 % -1.0 !== -0.0) {
  throw '#4.1: -0.0 % -1.0 === -0';
} else {
  if (1.0 / (-0.0 % -1.0) !== -Infinity) {
    throw '#4.2: -0.0 % -1.0 === -0';
  }
}

//CHECK#5
if (0.0 % 1.7976931348623157e+308 !== 0.0) {
  throw '#5.1: 0.0 % max-double === 0.0';
} else {
  if (1.0 / (0.0 % 1.7976931348623157e+308) !== Infinity) {
    throw '#5.2: 0.0 % max-double === +0';
  }
}

//CHECK#6
if (-0.0 % 1.7976931348623157e+308 !== -0.0) {
  throw '#6.1: -0.0 % max-double === -0';
} else {
  if (1.0 / (-0.0 % 1.7976931348623157e+308) !== -Infinity) {
    throw '#6.2: -0.0 % max-double === -0';
  }
}
//? test: S11.5.3_A4_T7
//? description:...
    If operands neither an infinity, nor a zero, nor NaN, return x -
    truncate(x / y) * y
//? expect: success
//? source: ...
#!/usr/bin/env afw


function truncate(x) {
  if (x > 0.0) {
    return floor(x);
  } else {
    return ceil(x);
  }
}

let x;
let y;

//CHECK#1
x = 1.3;
y = 1.1;
if (x % y !== 0.19999999999999996) {
  throw '#1: 1.3 % 1.1 === 0.19999999999999996. Actual: ' + string(x % y);
}

//CHECK#2
x = -1.3;
y = 1.1;
if (x % y !== -0.19999999999999996) {
  throw '#2: -1.3 % 1.1 === -0.19999999999999996. Actual: ' + string(x % y);
}

//CHECK#3
x = 1.3;
y = -1.1;
if (x % y !== 0.19999999999999996) {
  throw '#3: 1.3 % -1.1 === 0.19999999999999996. Actual: ' + string(x % y);
}

//CHECK#4
x = -1.3;
y = -1.1;
if (x % y !== -0.19999999999999996) {
  throw '#4: -1.3 % -1.1 === -0.19999999999999996. Actual: ' + string(x % y);
}

//CHECK#5
x = 1.3;
y = 1.1;
if (x % y !== x - truncate(x / y) * y) {
  throw '#5: 1.3 % 1.1 === x - truncate(x / y) * y. Actual: ' + string(x % y);
}

//CHECK#6
x = -1.3;
y = 1.1;
if (x % y !== x - truncate(x / y) * y) {
  throw '#6: -1.3 % 1.1 === x - truncate(x / y) * y. Actual: ' + string(x % y);
}

//CHECK#7
x = 1.3;
y = -1.1;
if (x % y !== x - truncate(x / y) * y) {
  throw '#7: 1.3 % -1.1 === x - truncate(x / y) * y. Actual: ' + string(x % y);
}

//CHECK#8
x = -1.3;
y = -1.1;
if (x % y !== x - truncate(x / y) * y) {
  throw '#8: -1.3 % -1.1 === x - truncate(x / y) * y. Actual: ' + string(x % y);
}
