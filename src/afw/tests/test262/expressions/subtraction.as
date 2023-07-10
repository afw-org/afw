#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: subtraction.as
//? customPurpose: Part of test262
//? description: expressions/subtraction
//? sourceType: script
//?
//? test: order-of-evaluation
//? description: Type coercion order of operations for subtraction operator
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


function MyError() {}
loc trace;

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
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (evaluate(script("1\u0009-\u00091")) !== 0) {
  throw '#1: 1\\u0009-\\u00091 === 0';
}

//CHECK#2
if (evaluate(script("1\u000B-\u000B1")) !== 0) {
  throw '#2: 1\\u000B-\\u000B1 === 0';
}

//CHECK#3
if (evaluate(script("1\u000C-\u000C1")) !== 0) {
  throw '#3: 1\\u000C-\\u000C1 === 0';
}

//CHECK#4
if (evaluate(script("1\u0020-\u00201")) !== 0) {
  throw '#4: 1\\u0020-\\u00201 === 0';
}

//CHECK#5
if (evaluate(script("1\u00A0-\u00A01")) !== 0) {
  throw '#5: 1\\u00A0-\\u00A01 === 0';
}

//CHECK#6
if (evaluate(script("1\u000A-\u000A1")) !== 0) {
  throw '#6: 1\\u000A-\\u000A1 === 0';
}

//CHECK#7
if (evaluate(script("1\u000D-\u000D1")) !== 0) {
  throw '#7: 1\\u000D-\\u000D1 === 0';
}

//CHECK#8
if (evaluate(script("1\u2028-\u20281")) !== 0) {
  throw '#8: 1\\u2028-\\u20281 === 0';
}

//CHECK#9
if (evaluate(script("1\u2029-\u20291")) !== 0) {
  throw '#9: 1\\u2029-\\u20291 === 0';
}

//CHECK#10
if (evaluate(script("1\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u2029-\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u20291")) !== 0) {
  throw '#10: 1\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u2029-\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u20291 === 0';
}


//? test: S11.6.2_A2.1_T1
//? description: Either Type is not Reference or GetBase is not null
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (1 - 1 !== 0) {
  throw '#1: 1 - 1 === 0. Actual: ' + (1 - 1);
}

//CHECK#2
loc x = 1;
if (x - 1 !== 0) {
  throw '#2: loc x = 1; x - 1 === 0. Actual: ' + (x - 1);
}

//CHECK#3
loc y = 1;
if (1 - y !== 0) {
  throw '#3: loc y = 1; 1 - y === 0. Actual: ' + (1 - y);
}

//CHECK#4
x = 1;
y = 1;
if (x - y !== 0) {
  throw '#4: loc x = 1; loc y = 1; x - y === 0. Actual: ' + (x - y);
}


//? test: S11.6.2_A2.1_T2
//? description: If GetBase(x) is null, throw ReferenceError
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function x
//? source: ...
#!/usr/bin/env afw

x - 1;


//? test: S11.6.2_A2.1_T3
//? description: If GetBase(y) is null, throw ReferenceError
//? expect: error:Parse error at offset 24 around line 3 column 5: Unknown built-in function y
//? source: ...
#!/usr/bin/env afw

1 - y;



//? test: S11.6.2_A2.3_T1
//? description: Checking with "throw"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
loc x = { valueOf: function () { throw "x"; } };
loc y = { valueOf: function () { throw "y"; } };
try {
   x - y;
   throw '#1.1: loc x = { valueOf: function () { throw "x"; } }; loc y = { valueOf: function () { throw "y"; } }; x - y throw "x". Actual: ' + (x - y);
} catch (e) {
   if (e === "y") {
     throw '#1.2: ToNumber(first expression) is called first, and then ToNumber(second expression)';
   } else {
     if (e !== "x") {
       throw '#1.3: loc x = { valueOf: function () { throw "x"; } }; loc y = { valueOf: function () { throw "y"; } }; x - y throw "x". Actual: ' + (e);
     }
   }
}



//? test: S11.6.2_A2.4_T2
//? description: Checking with "throw"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
loc x = function () { throw "x"; };
loc y = function () { throw "y"; };
try {
   x() - y();
   throw '#1.1: loc x = function () { throw "x"; }; loc y = function () { throw "y"; }; x() - y() throw "x". Actual: ' + (x() - y());
} catch (e) {
   if (e === "y") {
     throw '#1.2: First expression is evaluated first, and then second expression';
   } else {
     if (e !== "x") {
       throw '#1.3: loc x = function () { throw "x"; }; loc y = function () { throw "y"; }; x() - y() throw "x". Actual: ' + (e);
     }
   }
}



//? test: S11.6.2_A3_T1.2
//? description: Type(x) and Type(y) vary between primitive number and Number object
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (1 - 1 !== 0) {
  throw '#1: 1 - 1 === 0. Actual: ' + (1 - 1);
}


//? test: S11.6.2_A4_T1
//? description: If either operand is NaN, the result is NaN
//? expect: null
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
//? expect: null
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
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


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
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (Infinity - 1 !== Infinity ) {
  throw '#1: Infinity - 1 === Infinity. Actual: ' + (Infinity - 1);
}

//CHECK#2
if (-1 - Infinity !== -Infinity ) {
  throw '#2: -1 - Infinity === -Infinity. Actual: ' + (-1 - Infinity);
}

//CHECK#3
if (-Infinity - 1 !== -Infinity ) {
  throw '#3: -Infinity - 1 === -Infinity. Actual: ' + (-Infinity - 1);
}

//CHECK#4
if (-1 - -Infinity !== Infinity ) {
  throw '#4: -1 - -Infinity === Infinity. Actual: ' + (-1 - -Infinity);
}

//CHECK#5
if (Infinity - Number.MAX_VALUE !== Infinity ) {
  throw '#5: Infinity - Number.MAX_VALUE === Infinity. Actual: ' + (Infinity - Number.MAX_VALUE);
}

//CHECK#6
if (-Number.MAX_VALUE - Infinity !== -Infinity ) {
  throw '#6: -Number.MAX_VALUE - Infinity === I-nfinity. Actual: ' + (-Number.MAX_VALUE - Infinity);
}

//CHECK#7
if (-Infinity - Number.MAX_VALUE !== -Infinity ) {
  throw '#7: -Infinity - Number.MAX_VALUE === -Infinity. Actual: ' + (-Infinity - Number.MAX_VALUE);
}

//CHECK#8
if (-Number.MAX_VALUE - -Infinity !== Infinity ) {
  throw '#8: -Number.MAX_VALUE - -Infinity === Infinity. Actual: ' + (-Number.MAX_VALUE - -Infinity);
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
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (1 - -0 !== 1 ) {
  throw '#1: 1 - -0 === 1. Actual: ' + (1 - -0);
}

//CHECK#2
if (1 - 0 !== 1 ) {
  throw '#2: 1 - 0 === 1. Actual: ' + (1 - 0);
}

//CHECK#3
if (-0 - 1 !== -1 ) {
  throw '#3: -0 - 1 === -1. Actual: ' + (-0 - 1);
}

//CHECK#4
if (0 - 1 !== -1 ) {
  throw '#4: 0 - 1 === -1. Actual: ' + (0 - 1);
}

//CHECK#5
if (Number.MAX_VALUE - -0 !== Number.MAX_VALUE ) {
  throw '#5: Number.MAX_VALUE - -0 === Number.MAX_VALUE. Actual: ' + (Number.MAX_VALUE - -0);
}

//CHECK#6
if (Number.MAX_VALUE - 0 !== Number.MAX_VALUE ) {
  throw '#6: Number.MAX_VALUE - 0 === Number.MAX_VALUE. Actual: ' + (Number.MAX_VALUE - 0);
}

//CHECK#7
if (-0 - Number.MIN_VALUE !== -Number.MIN_VALUE ) {
  throw '#7: -0 - Number.MIN_VALUE === -Number.MIN_VALUE. Actual: ' + (-0 - Number.MIN_VALUE);
}

//CHECK#8
if (0 - Number.MIN_VALUE !== -Number.MIN_VALUE ) {
  throw '#8: 0 - Number.MIN_VALUE === -Number.MIN_VALUE. Actual: ' + (0 - Number.MIN_VALUE);
}


//? test: S11.6.2_A4_T7
//? description:...
    The mathematical difference of two nonzero finite values of the
    same magnitude and same sign is +0
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (Number.MIN_VALUE - Number.MIN_VALUE !== +0) {
  throw '#1.1: Number.MIN_VALUE - Number.MIN_VALUE === 0. Actual: ' + (Number.MIN_VALUE - Number.MIN_VALUE);
} else {
  if (1 / (Number.MIN_VALUE - Number.MIN_VALUE) !== Infinity) {
    throw '#1.2: Number.MIN_VALUE - Number.MIN_VALUE === + 0. Actual: -0';
  }
}

//CHECK#2
if (-Number.MAX_VALUE - -Number.MAX_VALUE !== +0) {
  throw '#2.2: -Number.MAX_VALUE - -Number.MAX_VALUE === 0. Actual: ' + (-Number.MAX_VALUE - -Number.MAX_VALUE);
} else {
  if (1 / (-Number.MAX_VALUE - -Number.MAX_VALUE) !== Infinity) {
    throw '#2.1: -Number.MAX_VALUE - -Number.MAX_VALUE === + 0. Actual: -0';
  }
}

//CHECK#3
if (1 / Number.MAX_VALUE - 1 / Number.MAX_VALUE !== +0) {
  throw '#3.1: 1 / Number.MAX_VALUE - 1 / Number.MAX_VALUE === 0. Actual: ' + (1 / Number.MAX_VALUE - 1 / Number.MAX_VALUE);
} else {
  if (1 / (1 / Number.MAX_VALUE - 1 / Number.MAX_VALUE) !== Infinity) {
    throw '#3.2: 1 / Number.MAX_VALUE - 1 / Number.MAX_VALUE === + 0. Actual: -0';
  }
}


//? test: S11.6.2_A4_T8
//? description:...
    If the magnitude is too large to represent, the operation
    overflows and the result is then an infinity of appropriate sign
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (Number.MAX_VALUE - -Number.MAX_VALUE !== Infinity) {
  throw '#1: Number.MAX_VALUE - -Number.MAX_VALUE === Infinity. Actual: ' + (Number.MAX_VALUE - -Number.MAX_VALUE);
}

//CHECK#2
if (-Number.MAX_VALUE - Number.MAX_VALUE !== -Infinity) {
  throw '#2: -Number.MAX_VALUE - umber.MAX_VALUE === -Infinity. Actual: ' + (-Number.MAX_VALUE - Number.MAX_VALUE);
}

//CHECK#3
if (1e+308 - -1e+308 !== Infinity) {
  throw '#3: 1e+308 - -1e+308 === Infinity. Actual: ' + (1e+308 - -1e+308);
}

//CHECK#4
if (-8.99e+307 - 8.99e+307 !== -Infinity) {
  throw '#4: -8.99e+307 - 8.99e+307 === -Infinity. Actual: ' + (-8.99e+307 - 8.99e+307);
}


