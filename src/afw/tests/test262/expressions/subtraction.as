#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: subtraction.as
//? customPurpose: Part of test262
//? description: expressions/subtraction
//? sourceType: script
//?
//? test: order-of-evaluation
//? description: Type coercion order of operations for subtraction operator
//? expect: success
//? skip: true
//? skipReason: ...
Never: Adaptive does not ToPrimitive/ToNumeric via valueOf;
left-to-right operand evaluation is S11.6.2_A2.4_T2
//? source: ...
#!/usr/bin/env afw


function MyError() {}
let trace;

// ?GetValue(lhs) throws.
trace = "";
assert.throws(MyError, function() {
  (function() {
    trace += "1";
    throw new MyError();
  })() - (function() {
    trace += "2";
    throw "should not be evaluated";
  })();
}, "?GetValue(lhs) throws.");
assert.sameValue(trace, "1", "?GetValue(lhs) throws.");

// ?GetValue(rhs) throws.
trace = "";
assert.throws(MyError, function() {
  (function() {
    trace += "1";
    return {
      valueOf: function() {
        trace += "3";
        throw "should not be evaluated";
      }
    };
  })() - (function() {
    trace += "2";
    throw new MyError();
  })();
}, "?GetValue(rhs) throws.");
assert.sameValue(trace, "12", "?GetValue(rhs) throws.");

// ?ToPrimive(lhs) throws.
trace = "";
assert.throws(MyError, function() {
  (function() {
    trace += "1";
    return {
      valueOf: function() {
        trace += "3";
        throw new MyError();
      }
    };
  })() - (function() {
    trace += "2";
    return {
      valueOf: function() {
        trace += "4";
        throw "should not be evaluated";
      }
    };
  })();
}, "?ToPrimive(lhs) throws.");
assert.sameValue(trace, "123", "?ToPrimive(lhs) throws.");

// ?ToPrimive(rhs) throws.
trace = "";
assert.throws(MyError, function() {
  (function() {
    trace += "1";
    return {
      valueOf: function() {
        trace += "3";
        return 1;
      }
    };
  })() - (function() {
    trace += "2";
    return {
      valueOf: function() {
        trace += "4";
        throw new MyError();
      }
    };
  })();
}, "?ToPrimive(rhs) throws.");
assert.sameValue(trace, "1234", "?ToPrimive(rhs) throws.");

// ?ToNumeric(lhs) throws.
trace = "";
assert.throws(TypeError, function() {
  (function() {
    trace += "1";
    return {
      valueOf: function() {
        trace += "3";
        return Symbol("1");
      }
    };
  })() - (function() {
    trace += "2";
    return {
      valueOf: function() {
        trace += "4";
        throw "should not be evaluated";
      }
    };
  })();
}, "?ToNumeric(lhs) throws.");
assert.sameValue(trace, "123", "?ToNumeric(lhs) throws.");

// GetValue(lhs) throws.
trace = "";
assert.throws(TypeError, function() {
  (function() {
    trace += "1";
    return {
      valueOf: function() {
        trace += "3";
        return 1;
      }
    };
  })() - (function() {
    trace += "2";
    return {
      valueOf: function() {
        trace += "4";
        return Symbol("1");
      }
    };
  })();
}, "GetValue(lhs) throws.");
assert.sameValue(trace, "1234", "GetValue(lhs) throws.");
//? test: S11.6.2_A1
//? description: Checking by using eval
//? expect: success
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (eval(script("1\u0009-\u00091")) !== 0) {
  throw '#1: 1\\u0009-\\u00091 === 0';
}

//CHECK#2
if (eval(script("1\u000B-\u000B1")) !== 0) {
  throw '#2: 1\\u000B-\\u000B1 === 0';
}

//CHECK#3
if (eval(script("1\u000C-\u000C1")) !== 0) {
  throw '#3: 1\\u000C-\\u000C1 === 0';
}

//CHECK#4
if (eval(script("1\u0020-\u00201")) !== 0) {
  throw '#4: 1\\u0020-\\u00201 === 0';
}

//CHECK#5
if (eval(script("1\u00A0-\u00A01")) !== 0) {
  throw '#5: 1\\u00A0-\\u00A01 === 0';
}

//CHECK#6
if (eval(script("1\u000A-\u000A1")) !== 0) {
  throw '#6: 1\\u000A-\\u000A1 === 0';
}

//CHECK#7
if (eval(script("1\u000D-\u000D1")) !== 0) {
  throw '#7: 1\\u000D-\\u000D1 === 0';
}

//CHECK#8
if (eval(script("1\u2028-\u20281")) !== 0) {
  throw '#8: 1\\u2028-\\u20281 === 0';
}

//CHECK#9
if (eval(script("1\u2029-\u20291")) !== 0) {
  throw '#9: 1\\u2029-\\u20291 === 0';
}

//CHECK#10
if (eval(script("1\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u2029-\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u20291")) !== 0) {
  throw '#10: 1\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u2029-\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u20291 === 0';
}
//? test: S11.6.2_A2.1_T1
//? description: Either Type is not Reference or GetBase is not null
//? expect: success
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (1 - 1 !== 0) {
  throw '#1: 1 - 1 === 0. Actual: ' + (1 - 1);
}

//CHECK#2
let x = 1;
if (x - 1 !== 0) {
  throw '#2: let x = 1; x - 1 === 0. Actual: ' + (x - 1);
}

//CHECK#3
let y = 1;
if (1 - y !== 0) {
  throw '#3: let y = 1; 1 - y === 0. Actual: ' + (1 - y);
}

//CHECK#4
x = 1;
y = 1;
if (x - y !== 0) {
  throw '#4: let x = 1; let y = 1; x - y === 0. Actual: ' + (x - y);
}
//? test: S11.6.2_A2.1_T2
//? description: If GetBase(x) is null, throw ReferenceError
//? expect: error
//? source: ...
#!/usr/bin/env afw

// undeclared x is a compile error
x - 1;


//? test: S11.6.2_A2.1_T3
//? description: If GetBase(y) is null, throw ReferenceError
//? expect: error
//? source: ...
#!/usr/bin/env afw

// undeclared y is a compile error
1 - y;



//? test: S11.6.2_A2.3_T1
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
   x - y;
   throw '#1.1: let x = { valueOf: function () { throw "x"; } }; let y = { valueOf: function () { throw "y"; } }; x - y throw "x". Actual: ' + (x - y);
} catch (e) {
   if (e === "y") {
     throw '#1.2: ToNumber(first expression) is called first, and then ToNumber(second expression)';
   } else {
     if (e !== "x") {
       throw '#1.3: let x = { valueOf: function () { throw "x"; } }; let y = { valueOf: function () { throw "y"; } }; x - y throw "x". Actual: ' + (e);
     }
   }
}
//? test: S11.6.2_A2.4_T2
//? description: Checking with "throw"
//? expect: 0
//? source: ...

let saw = "";
function xf() { saw = saw + "x"; throw "x"; }
function yf() { saw = saw + "y"; throw "y"; }
try {
    let unused = xf() - yf();
    assert(false);
} catch (e) {
    assert(e.message === "x");
    assert(saw === "x");
}
return 0;
//? test: S11.6.2_A3_T1.2
//? description: Type(x) and Type(y) vary between primitive number and Number object
//? expect: success
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (1 - 1 !== 0) {
  throw '#1: 1 - 1 === 0. Actual: ' + (1 - 1);
}
//? test: S11.6.2_A4_T1
//? description: If either operand is NaN, the result is NaN
//? expect: success
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN(NaN - 1.0) !== true ) {
  throw '#1: NaN - 1 === Not-a-Number. Actual: ' + (NaN - 1);
}

//CHECK#2
if (is_NaN(1.0 - NaN) !== true ) {
  throw '#2: 1 - NaN === Not-a-Number. Actual: ' + (1 - NaN);
}

//CHECK#3
if (is_NaN(NaN - Infinity) !== true ) {
  throw '#3: NaN - Infinity === Not-a-Number. Actual: ' + (NaN - Infinity);
}

//CHECK#4
if (is_NaN(Infinity - NaN) !== true ) {
  throw '#4: Infinity - NaN === Not-a-Number. Actual: ' + (Infinity - NaN);
}

//CHECK#5
if (is_NaN(NaN - -Infinity) !== true ) {
  throw '#5: NaN - Infinity === Not-a-Number. Actual: ' + (NaN - Infinity);
}

//CHECK#6
if (is_NaN(-Infinity - NaN) !== true ) {
  throw '#6: Infinity - NaN === Not-a-Number. Actual: ' + (Infinity - NaN);
}
//? test: S11.6.2_A4_T2
//? description:...
    The difference of two infinities of opposite sign is the infinity
    of minuend sign
//? expect: success
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (Infinity - -Infinity !== Infinity ) {
  throw '#1: Infinity - -Infinity === Infinity. Actual: ' + (Infinity - -Infinity);
}

//CHECK#2
if (-Infinity - Infinity !== -Infinity ) {
  throw '#2: -Infinity - Infinity === -Infinity. Actual: ' + (-Infinity - Infinity);
}
//? test: S11.6.2_A4_T3
//? description: The difference of two infinities of the same sign is NaN
//? expect: success
//? source: ...

//CHECK#1
if (is_NaN(Infinity - Infinity) !== true ) {
  throw '#1: Infinity - Infinity === Not-a-Number. Actual: ' + (Infinity - Infinity);
}

//CHECK#2
if (is_NaN(-Infinity - -Infinity) !== true ) {
  throw '#2: -Infinity - -Infinity === Not-a-Number. Actual: ' + (-Infinity - -Infinity);
}
//? test: S11.6.2_A4_T4
//? description:...
    The difference of an infinity and a finite value is equal to
    infinity of appropriate sign
//? expect: success
//? source: ...

//CHECK#1
if (Infinity - 1.0 !== Infinity ) {
  throw '#1: Infinity - 1 === Infinity. Actual: ' + (Infinity - 1.0);
}

//CHECK#2
if (-1.0 - Infinity !== -Infinity ) {
  throw '#2: -1 - Infinity === -Infinity. Actual: ' + (-1.0 - Infinity);
}

//CHECK#3
if (-Infinity - 1.0 !== -Infinity ) {
  throw '#3: -Infinity - 1 === -Infinity. Actual: ' + (-Infinity - 1.0);
}

//CHECK#4
if (-1.0 - -Infinity !== Infinity ) {
  throw '#4: -1 - -Infinity === Infinity. Actual: ' + (-1.0 - -Infinity);
}

//CHECK#5
if (Infinity - #doubleMax !== Infinity ) {
  throw '#5: Infinity - #doubleMax === Infinity. Actual: ' + (Infinity - #doubleMax);
}

//CHECK#6
if (-#doubleMax - Infinity !== -Infinity ) {
  throw '#6: -#doubleMax - Infinity === -Infinity. Actual: ' + (-#doubleMax - Infinity);
}

//CHECK#7
if (-Infinity - #doubleMax !== -Infinity ) {
  throw '#7: -Infinity - #doubleMax === -Infinity. Actual: ' + (-Infinity - #doubleMax);
}

//CHECK#8
if (-#doubleMax - -Infinity !== Infinity ) {
  throw '#8: -#doubleMax - -Infinity === Infinity. Actual: ' + (-#doubleMax - -Infinity);
}
//? test: S11.6.2_A4_T5
//? description:...
    Using the rule of sum of two zeroes and the fact that a - b = a +
    (-b)
//? expect: error:Integer divide by zero error
//? source: ...
#!/usr/bin/env afw

// test262 expects 1 / 0 to be Infinity, but we throw errors instead in AFW

//CHECK#1
if (-0 - -0 !== 0 ) {
  throw '#1.1: -0 - -0 === 0. Actual: ' + (-0 - -0);
} else {
  if (1 / (-0 - -0) !== Infinity) {
    throw '#1.2: -0 - -0 === + 0. Actual: -0';
  }
}

//CHECK#2
if (0 - -0 !== 0 ) {
  throw '#2.1: 0 - -0 === 0. Actual: ' + (0 - -0);
} else {
  if (1 / (0 - -0) !== Infinity) {
    throw '#2.2: 0 - -0 === + 0. Actual: -0';
  }
}

//CHECK#3
if (-0 - 0 !== -0 ) {
  throw '#3.1: -0 - 0 === 0. Actual: ' + (-0 - 0);
} else {
  if (1 / (-0 - 0) !== -Infinity) {
    throw '#3.2: -0 - 0 === - 0. Actual: +0';
  }
}

//CHECK#4
if (0 - 0 !== 0 ) {
  throw '#4.1: 0 - 0 === 0. Actual: ' + (0 - 0);
} else {
  if (1 / (0 - 0) !== Infinity) {
    throw '#4.2: 0 - 0 === + 0. Actual: -0';
  }
}


//? test: S11.6.2_A4_T6
//? description:...
    Using the rule of sum of a zero and a nonzero finite value and the
    fact that a - b = a + (-b)
//? expect: success
//? source: ...

//CHECK#1
if (1.0 - -0.0 !== 1.0 ) {
  throw '#1: 1 - -0 === 1. Actual: ' + (1.0 - -0.0);
}

//CHECK#2
if (1.0 - 0.0 !== 1.0 ) {
  throw '#2: 1 - 0 === 1. Actual: ' + (1.0 - 0.0);
}

//CHECK#3
if (-0.0 - 1.0 !== -1.0 ) {
  throw '#3: -0 - 1 === -1. Actual: ' + (-0.0 - 1.0);
}

//CHECK#4
if (0.0 - 1.0 !== -1.0 ) {
  throw '#4: 0 - 1 === -1. Actual: ' + (0.0 - 1.0);
}

//CHECK#5
if (#doubleMax - -0.0 !== #doubleMax ) {
  throw '#5: #doubleMax - -0 === #doubleMax. Actual: ' + (#doubleMax - -0.0);
}

//CHECK#6
if (#doubleMax - 0.0 !== #doubleMax ) {
  throw '#6: #doubleMax - 0 === #doubleMax. Actual: ' + (#doubleMax - 0.0);
}

//CHECK#7
if (-0.0 - #doubleMin !== -#doubleMin ) {
  throw '#7: -0 - #doubleMin === -#doubleMin. Actual: ' + (-0.0 - #doubleMin);
}

//CHECK#8
if (0.0 - #doubleMin !== -#doubleMin ) {
  throw '#8: 0 - #doubleMin === -#doubleMin. Actual: ' + (0.0 - #doubleMin);
}
//? test: S11.6.2_A4_T7
//? description:...
    The mathematical difference of two nonzero finite values of the
    same magnitude and same sign is +0
//? expect: success
//? source: ...

//CHECK#1
if (#doubleMin - #doubleMin !== +0.0) {
  throw '#1.1: #doubleMin - #doubleMin === 0. Actual: ' + (#doubleMin - #doubleMin);
} else {
  if (1.0 / (#doubleMin - #doubleMin) !== Infinity) {
    throw '#1.2: #doubleMin - #doubleMin === + 0. Actual: -0';
  }
}

//CHECK#2
if (-#doubleMax - -#doubleMax !== +0.0) {
  throw '#2.2: -#doubleMax - -#doubleMax === 0. Actual: ' + (-#doubleMax - -#doubleMax);
} else {
  if (1.0 / (-#doubleMax - -#doubleMax) !== Infinity) {
    throw '#2.1: -#doubleMax - -#doubleMax === + 0. Actual: -0';
  }
}

//CHECK#3
if (1.0 / #doubleMax - 1.0 / #doubleMax !== +0.0) {
  throw '#3.1: 1 / #doubleMax - 1 / #doubleMax === 0. Actual: ' + (1.0 / #doubleMax - 1.0 / #doubleMax);
} else {
  if (1.0 / (1.0 / #doubleMax - 1.0 / #doubleMax) !== Infinity) {
    throw '#3.2: 1 / #doubleMax - 1 / #doubleMax === + 0. Actual: -0';
  }
}
//? test: S11.6.2_A4_T8
//? description:...
    If the magnitude is too large to represent, the operation
    overflows and the result is then an infinity of appropriate sign
//? expect: success
//? source: ...

//CHECK#1
if (#doubleMax - -#doubleMax !== Infinity) {
  throw '#1: #doubleMax - -#doubleMax === Infinity. Actual: ' + (#doubleMax - -#doubleMax);
}

//CHECK#2
if (-#doubleMax - #doubleMax !== -Infinity) {
  throw '#2: -#doubleMax - #doubleMax === -Infinity. Actual: ' + (-#doubleMax - #doubleMax);
}

//CHECK#3
if (1.0E308 - -1.0E308 !== Infinity) {
  throw '#3: 1e+308 - -1e+308 === Infinity. Actual: ' + (1.0E308 - -1.0E308);
}

//CHECK#4
if (-8.99E307 - 8.99E307 !== -Infinity) {
  throw '#4: -8.99e+307 - 8.99e+307 === -Infinity. Actual: ' + (-8.99E307 - 8.99E307);
}
