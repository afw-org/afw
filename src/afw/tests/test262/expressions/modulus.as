#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: modulus.as
//? customPurpose: Part of test262
//? description: expressions/modulus
//? sourceType: script
//?
//? test: line-terminator
//? description: Line terminator between the operands of a modulus operator
//? expect: null
//? source: ...
#!/usr/bin/env afw


loc x = 18

%

7

%

3
;

assert(x === 1);


//? test: order-of-evaluation
//? description: Type coercion order of operations for modulus operator
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
  })() % (function() {
    trace += "2";
    assert(false, "should not be evaluated");
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
        assert(false, "should not be evaluated");
      }
    };
  })() % (function() {
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
  })() % (function() {
    trace += "2";
    return {
      valueOf: function() {
        trace += "4";
        assert(false, "should not be evaluated");
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
  })() % (function() {
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
  })() % (function() {
    trace += "2";
    return {
      valueOf: function() {
        trace += "4";
        assert(false, "should not be evaluated");
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
  })() % (function() {
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


//? test: S11.5.3_A1
//? description: Checking by using eval
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (evaluate(script("1\u0009%\u00091")) !== 0) {
  assert(false, '#1: 1\\u0009%\\u00091 === 0');
}

//CHECK#2
if (evaluate(script("1\u000B%\u000B1")) !== 0) {
  assert(false, '#2: 1\\u000B%\\u000B1 === 0');
}

//CHECK#3
if (evaluate(script("1\u000C%\u000C1")) !== 0) {
  assert(false, '#3: 1\\u000C%\\u000C1 === 0');
}

//CHECK#4
if (evaluate(script("1\u0020%\u00201")) !== 0) {
  assert(false, '#4: 1\\u0020%\\u00201 === 0');
}

//CHECK#5
if (evaluate(script("1\u00A0%\u00A01")) !== 0) {
  assert(false, '#5: 1\\u00A0%\\u00A01 === 0');
}

//CHECK#6
if (evaluate(script("1\u000A%\u000A1")) !== 0) {
  assert(false, '#6: 1\\u000A%\\u000A1 === 0');
}

//CHECK#7
if (evaluate(script("1\u000D%\u000D1")) !== 0) {
  assert(false, '#7: 1\\u000D%\\u000D1 === 0');
}

//CHECK#8
if (evaluate(script("1\u2028%\u20281")) !== 0) {
  assert(false, '#8: 1\\u2028%\\u20281 === 0');
}

//CHECK#9
if (evaluate(script("1\u2029%\u20291")) !== 0) {
  assert(false, '#9: 1\\u2029%\\u20291 === 0');
}

//CHECK#10
if (evaluate(script("1\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u2029%\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u20291")) !== 0) {
  assert(false, '#10: 1\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u2029%\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u20291 === 0');
}


//? test: S11.5.3_A2.1_T1
//? description: Either Type is not Reference or GetBase is not null
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (1 % 2 !== 1) {
  assert(false, '#1: 1 % 2 === 1. Actual: ' + (1 % 2));
}

//CHECK#2
loc x = 1;
if (x % 2 !== 1) {
  assert(false, '#2: loc x = 1; x % 2 === 1. Actual: ' + (x % 2));
}

//CHECK#3
loc y = 2;
if (1 % y !== 1) {
  assert(false, '#3: loc y = 2; 1 % y === 1. Actual: ' + (1 % y));
}

//CHECK#4
x = 1;
y = 2;
if (x % y !== 1) {
  assert(false, '#4: loc x = 1; loc y = 2; x % y === 1. Actual: ' + (x % y));
}



//? test: S11.5.3_A2.1_T2
//? description: If GetBase(x) is null, throw ReferenceError
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function x
//? source: ...
#!/usr/bin/env afw

x % 1;


//? test: S11.5.3_A2.1_T3
//? description: If GetBase(y) is null, throw ReferenceError
//? expect: error:Parse error at offset 24 around line 3 column 5: Unknown built-in function y
//? source: ...
#!/usr/bin/env afw

1 % y;


//? test: S11.5.3_A2.3_T1
//? description: Checking with "throw"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
loc x = { valueOf: function () { throw "x"; } };
loc y = { valueOf: function () { throw "y"; } };
try {
   x % y;
   assert(false, '#1.1: loc x = { valueOf: function () { throw "x"; } }; loc y = { valueOf: function () { throw "y"; } }; x % y throw "x". Actual: ' + (x % y));
} catch (e) {
   if (e === "y") {
     assert(false, '#1.2: ToNumber(first expression) is called first, and then ToNumber(second expression)');
   } else {
     if (e !== "x") {
       assert(false, '#1.3: loc x = { valueOf: function () { throw "x"; } }; loc y = { valueOf: function () { throw "y"; } }; x % y throw "x". Actual: ' + (e));
     }
   }
}


//? test: S11.5.3_A2.4_T2
//? description: Checking with "throw"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
loc x = function () { throw "x"; };
loc y = function () { throw "y"; };
try {
   x() % y();
   assert(false, '#1.1: loc x = function () { throw "x"; }; loc y = function () { throw "y"; }; x() % y() throw "x". Actual: ' + (x() % y()));
} catch (e) {
   if (e === "y") {
     assert(false, '#1.2: First expression is evaluated first, and then second expression');
   } else {
     if (e !== "x") {
       assert(false, '#1.3: loc x = function () { throw "x"; }; loc y = function () { throw "y"; }; x() % y() throw "x". Actual: ' + (e));
     }
   }
}


//? test: S11.5.3_A2.4_T3
//? description: Checking with undeclarated variables
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function x
//? source: ...
#!/usr/bin/env afw

x % (x = 1);


//? test: S11.5.3_A3_T1.2
//? description: Type(x) and Type(y) vary between primitive number and Number object
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (1 % 1 !== 0) {
  assert(false, '#1: 1 % 1 === 0. Actual: ' + (1 % 1));
}


//? test: S11.5.3_A3_T1.3
//? description: Type(x) and Type(y) vary between primitive string and String object
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ("1" % "1" !== 0) {
  assert(false, '#1: "1" % "1" === 0. Actual: ' + ("1" % "1"));
}

//CHECK#2
if (new String("1") % "1" !== 0) {
  assert(false, '#2: new String("1") % "1" === 0. Actual: ' + (new String("1") % "1"));
}

//CHECK#3
if ("1" % new String("1") !== 0) {
  assert(false, '#3: "1" % new String("1") === 0. Actual: ' + ("1" % new String("1")));
}

//CHECK#4
if (new String("1") % new String("1") !== 0) {
  assert(false, '#4: new String("1") % new String("1") === 0. Actual: ' + (new String("1") % new String("1")));
}

//CHECK#5
if (is_NaN("x" % "1") !== true) {
  assert(false, '#5: "x" % "1" === Not-a-Number. Actual: ' + ("x" % "1"));
}

//CHECK#6
if (is_NaN("1" % "x") !== true) {
  assert(false, '#6: "1" % "x" === Not-a-Number. Actual: ' + ("1" % "x"));
}


//? test: S11.5.3_A3_T1.4
//? description: Type(x) and Type(y) vary between Null and Undefined
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN(null % undefined) !== true) {
  assert(false, '#1: null % undefined === Not-a-Number. Actual: ' + (null % undefined));
}

//CHECK#2
if (is_NaN(undefined % null) !== true) {
  assert(false, '#2: undefined % null === Not-a-Number. Actual: ' + (undefined % null));
}

//CHECK#3
if (is_NaN(undefined % undefined) !== true) {
  assert(false, '#3: undefined % undefined === Not-a-Number. Actual: ' + (undefined % undefined));
}

//CHECK#4
if (is_NaN(null % null) !== true) {
  assert(false, '#4: null % null === Not-a-Number. Actual: ' + (null % null));
}


//? test: S11.5.3_A3_T1.5
//? description: Type(x) and Type(y) vary between Object object and Function object
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN({} % function(){return 1}) !== true) {
  assert(false, '#1: {} % function(){return 1} === Not-a-Number. Actual: ' + ({} % function(){return 1}));
}

//CHECK#2
if (is_NaN(function(){return 1} % {}) !== true) {
  assert(false, '#2: function(){return 1} % {} === Not-a-Number. Actual: ' + (function(){return 1} % {}));
}

//CHECK#3
if (is_NaN(function(){return 1} % function(){return 1}) !== true) {
  assert(false, '#3: function(){return 1} % function(){return 1} === Not-a-Number. Actual: ' + (function(){return 1} % function(){return 1}));
}

//CHECK#4
if (is_NaN({} % {}) !== true) {
  assert(false, '#4: {} % {} === Not-a-Number. Actual: ' + ({} % {}));
}

//? test: S11.5.3_A3_T2.2
//? description:...
    Type(x) is different from Type(y) and both types vary between
    Number (primitive or object) and String (primitive and object)
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ("1" % 1 !== 0) {
  assert(false, '#1: "1" % 1 === 0. Actual: ' + ("1" % 1));
}

//CHECK#2
if (1 % "1" !== 0) {
  assert(false, '#2: 1 % "1" === 0. Actual: ' + (1 % "1"));
}

//CHECK#9
if (is_NaN("x" % 1) !== true) {
  assert(false, '#9: "x" % 1 === Not-a-Number. Actual: ' + ("x" % 1));
}

//CHECK#10
if (is_NaN(1 % "x") !== true) {
  assert(false, '#10: 1 % "x" === Not-a-Number. Actual: ' + (1 % "x"));
}


//? test: S11.5.3_A3_T2.3
//? description:...
    Type(x) is different from Type(y) and both types vary between
    Number (primitive or object) and Null
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN(1 % null) !== true) {
  assert(false, '#1: 1 % null === Not-a-Number. Actual: ' + (1 % null));
}

//CHECK#2
if (null % 1 !== 0) {
  assert(false, '#2: null % 1 === 0. Actual: ' + (null % 1));
}


//? test: S11.5.3_A3_T2.4
//? description:...
    Type(x) is different from Type(y) and both types vary between
    Number (primitive or object) and Undefined
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN(1 % undefined) !== true) {
  assert(false, '#1: 1 % undefined === Not-a-Number. Actual: ' + (1 % undefined));
}

//CHECK#2
if (is_NaN(undefined % 1) !== true) {
  assert(false, '#2: undefined % 1 === Not-a-Number. Actual: ' + (undefined % 1));
}


//? test: S11.5.3_A3_T2.6
//? description:...
    Type(x) is different from Type(y) and both types vary between
    String (primitive or object) and Undefined
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN("1" % undefined) !== true) {
  assert(false, '#1: "1" % undefined === Not-a-Number. Actual: ' + ("1" % undefined));
}

//CHECK#2
if (is_NaN(undefined % "1") !== true) {
  assert(false, '#2: undefined % "1" === Not-a-Number. Actual: ' + (undefined % "1"));
}

//CHECK#3
if (is_NaN(new String("1") % undefined) !== true) {
  assert(false, '#3: new String("1") % undefined === Not-a-Number. Actual: ' + (new String("1") % undefined));
}

//CHECK#4
if (is_NaN(undefined % new String("1")) !== true) {
  assert(false, '#4: undefined % new String("1") === Not-a-Number. Actual: ' + (undefined % new String("1")));
}


//? test: S11.5.3_A3_T2.7
//? description:...
    Type(x) is different from Type(y) and both types vary between
    String (primitive or object) and Null
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN("1" % null) !== true) {
  assert(false, '#1: "1" % null === Not-a-Number. Actual: ' + ("1" % null));
}

//CHECK#2
if (null % "1" !== 0) {
  assert(false, '#2: null % "1" === 0. Actual: ' + (null % "1"));
}

//CHECK#3
if (is_NaN(new String("1") % null) !== true) {
  assert(false, '#3: new String("1") % null === Not-a-Number. Actual: ' + (new String("1") % null));
}

//CHECK#4
if (null % new String("1") !== 0) {
  assert(false, '#4: null % new String("1") === 0. Actual: ' + (null % new String("1")));
}


//? test: S11.5.3_A4_T1.1
//? description: If either operand is NaN, the result is NaN
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN(NaN % NaN) !== true) {
  assert(false, '#1: NaN % NaN === Not-a-Number. Actual: ' + (NaN % NaN));
}

//CHECK#2
if (is_NaN(NaN % +0) !== true) {
  assert(false, '#2: NaN % +0 === Not-a-Number. Actual: ' + (NaN % +0));
}

//CHECK#3
if (is_NaN(NaN % -0) !== true) {
  assert(false, '#3: NaN % -0 === Not-a-Number. Actual: ' + (NaN % -0));
}

//CHECK#4
if (is_NaN(NaN % Infinity) !== true) {
  assert(false, '#4: NaN % Infinity === Not-a-Number. Actual: ' + (NaN % Infinity));
}

//CHECK#5
if (is_NaN(NaN % -Infinity) !== true) {
  assert(false, '#5: NaN % -Infinity === Not-a-Number. Actual: ' + (NaN % -Infinity));
}

//CHECK#6
if (is_NaN(NaN % Number.MAX_VALUE) !== true) {
  assert(false, '#6: NaN % Number.MAX_VALUE === Not-a-Number. Actual: ' + (NaN % Number.MAX_VALUE));
}

//CHECK#7
if (is_NaN(NaN % Number.MIN_VALUE) !== true) {
  assert(false, '#7: NaN % Number.MIN_VALUE === Not-a-Number. Actual: ' + (NaN % Number.MIN_VALUE));
}

//CHECK#8
if (is_NaN(NaN % 1) !== true) {
  assert(false, '#8: NaN % 1 === Not-a-Number. Actual: ' + (NaN % 1));
}


//? test: S11.5.3_A4_T1.2
//? description: If either operand is NaN, the result is NaN
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN(NaN % NaN) !== true) {
  assert(false, '#1: NaN % NaN === Not-a-Number. Actual: ' + (NaN % NaN));
}

//CHECK#2
if (is_NaN(+0 % NaN) !== true) {
  assert(false, '#2: +0 % NaN === Not-a-Number. Actual: ' + (+0 % NaN));
}

//CHECK#3
if (is_NaN(-0 % NaN) !== true) {
  assert(false, '#3: -0 % NaN === Not-a-Number. Actual: ' + (-0 % NaN));
}

//CHECK#4
if (is_NaN(Infinity % NaN) !== true) {
  assert(false, '#4: Infinity % NaN === Not-a-Number. Actual: ' + (Infinity % NaN));
}

//CHECK#5
if (is_NaN(-Infinity % NaN) !== true) {
  assert(false, '#5:  -Infinity % NaN === Not-a-Number. Actual: ' + ( -Infinity % NaN));
}

//CHECK#6
if (is_NaN(Number.MAX_VALUE % NaN) !== true) {
  assert(false, '#6: Number.MAX_VALUE % NaN === Not-a-Number. Actual: ' + (Number.MAX_VALUE % NaN));
}

//CHECK#7
if (is_NaN(Number.MIN_VALUE % NaN) !== true) {
  assert(false, '#7: Number.MIN_VALUE % NaN === Not-a-Number. Actual: ' + (Number.MIN_VALUE % NaN));
}

//CHECK#8
if (is_NaN(1 % NaN) !== true) {
  assert(false, '#8: 1 % NaN === Not-a-Number. Actual: ' + (1 % NaN));
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
  assert(false, '#1.1: 1 % 1 === 0. Actual: ' + (1 % 1));
} else {
  if (1 / (1 % 1) !== Infinity) {
    assert(false, '#1.2: 1 % 1 === + 0. Actual: -0');
  }
}

//CHECK#2
if (-1 % -1 !== -0) {
  assert(false, '#2.1: -1 % -1 === 0. Actual: ' + (-1 % -1));
} else {
  if (1 / (-1 % -1) !== -Infinity) {
    assert(false, '#2.2: -1 % -1 === - 0. Actual: +0');
  }
}

//CHECK#3
if (-1 % 1 !== -0) {
  assert(false, '#3.1: -1 % 1 === 0. Actual: ' + (-1 % 1));
} else {
  if (1 / (-1 % 1) !== -Infinity) {
    assert(false, '#3.2: -1 % 1 === - 0. Actual: +0');
  }
}

//CHECK#4
if (1 % -1 !== 0) {
  assert(false, '#4.1: 1 % -1 === 0. Actual: ' + (1 % -1));
} else {
  if (1 / (1 % -1) !== Infinity) {
    assert(false, '#4.2: 1 % -1 === + 0. Actual: -0');
  }
}

//CHECK#5
if (101 % 51 !== 50) {
  assert(false, '#5: 101 % 51 === 50. Actual: ' + (101 % 51));
}

//CHECK#6
if (101 % -51 !== 50) {
  assert(false, '#6: 101 % -51 === 50. Actual: ' + (101 % -51));
}

//CHECK#7
if (-101 % 51 !== -50) {
  assert(false, '#7: -101 % 51 === -50. Actual: ' + (-101 % 51));
}

//CHECK#8
if (-101 % -51 !== -50) {
  assert(false, '#8: -101 % -51 === -50. Actual: ' + (-101 % -51));
}


//? test: S11.5.3_A4_T3
//? description: If the dividend is an infinity results is NaN
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN(-Infinity % Infinity) !== true) {
  assert(false, '#1: -Infinity % Infinity === Not-a-Number. Actual: ' + (-Infinity % Infinity));
}

//CHECK#2
if (is_NaN(-Infinity % -Infinity) !== true) {
  assert(false, '#2: -Infinity % -Infinity === Not-a-Number. Actual: ' + (-Infinity % -Infinity));
}

//CHECK#3
if (is_NaN(Infinity % Infinity) !== true) {
  assert(false, '#3: Infinity % Infinity === Not-a-Number. Actual: ' + (Infinity % Infinity));
}

//CHECK#4
if (is_NaN(Infinity % -Infinity) !== true) {
  assert(false, '#4: Infinity % -Infinity === Not-a-Number. Actual: ' + (Infinity % -Infinity));
}

//CHECK#5
if (is_NaN(-Infinity % 1) !== true) {
  assert(false, '#5: Infinity % 1 === Not-a-Number. Actual: ' + (Infinity % 1));
}

//CHECK#6
if (is_NaN(-Infinity % -1) !== true) {
  assert(false, '#6: -Infinity % -1 === Not-a-Number. Actual: ' + (-Infinity % -1));
}

//CHECK#7
if (is_NaN(Infinity % 1) !== true) {
  assert(false, '#7: Infinity % 1 === Not-a-Number. Actual: ' + (Infinity % 1));
}

//CHECK#8
if (is_NaN(Infinity % -1) !== true) {
  assert(false, '#8: Infinity % -1 === Not-a-Number. Actual: ' + (Infinity % -1));
}

//CHECK#9
if (is_NaN(-Infinity % Number.MAX_VALUE) !== true) {
  assert(false, '#9: Infinity % Number.MAX_VALUE === Not-a-Number. Actual: ' + (Infinity % Number.MAX_VALUE));
}

//CHECK#10
if (is_NaN(-Infinity % -Number.MAX_VALUE) !== true) {
  assert(false, '#10: -Infinity % -Number.MAX_VALUE === Not-a-Number. Actual: ' + (-Infinity % -Number.MAX_VALUE));
}

//CHECK#11
if (is_NaN(Infinity % Number.MAX_VALUE) !== true) {
  assert(false, '#11: Infinity % Number.MAX_VALUE === Not-a-Number. Actual: ' + (Infinity % Number.MAX_VALUE));
}

//CHECK#12
if (is_NaN(Infinity % -Number.MAX_VALUE) !== true) {
  assert(false, '#12: Infinity % -Number.MAX_VALUE === Not-a-Number. Actual: ' + (Infinity % -Number.MAX_VALUE));
}


//? test: S11.5.3_A4_T4
//? description: If the divisor is zero results is NaN
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN(-0 % 0) !== true) {
  assert(false, '#1: -0 % 0 === Not-a-Number. Actual: ' + (-0 % 0));
}

//CHECK#2
if (is_NaN(-0 % -0) !== true) {
  assert(false, '#2: -0 % -0 === Not-a-Number. Actual: ' + (-0 % -0));
}

//CHECK#3
if (is_NaN(0 % 0) !== true) {
  assert(false, '#3: 0 % 0 === Not-a-Number. Actual: ' + (0 % 0));
}

//CHECK#4
if (is_NaN(0 % -0) !== true) {
  assert(false, '#4: 0 % -0 === Not-a-Number. Actual: ' + (0 % -0));
}

//CHECK#5
if (is_NaN(-1 % 0) !== true) {
  assert(false, '#5: 1 % 0 === Not-a-Number. Actual: ' + (1 % 0));
}

//CHECK#6
if (is_NaN(-1 % -0) !== true) {
  assert(false, '#6: -1 % -0 === Not-a-Number. Actual: ' + (-1 % -0));
}

//CHECK#7
if (is_NaN(1 % 0) !== true) {
  assert(false, '#7: 1 % 0 === Not-a-Number. Actual: ' + (1 % 0));
}

//CHECK#8
if (is_NaN(1 % -0) !== true) {
  assert(false, '#8: 1 % -0 === Not-a-Number. Actual: ' + (1 % -0));
}

//CHECK#9
if (is_NaN(-Infinity % 0) !== true) {
  assert(false, '#9: Infinity % 0 === Not-a-Number. Actual: ' + (Infinity % 0));
}

//CHECK#10
if (is_NaN(-Infinity % -0) !== true) {
  assert(false, '#10: -Infinity % -0 === Not-a-Number. Actual: ' + (-Infinity % -0));
}

//CHECK#11
if (is_NaN(Infinity % 0) !== true) {
  assert(false, '#11: Infinity % 0 === Not-a-Number. Actual: ' + (Infinity % 0));
}

//CHECK#12
if (is_NaN(Infinity % -0) !== true) {
  assert(false, '#12: Infinity % -0 === Not-a-Number. Actual: ' + (Infinity % -0));
}

/*
//CHECK#13
if (is_NaN(Number.MIN_VALUE % 0) !== true) {
  assert(false, '#13: Number.MIN_VALUE % 0 === Not-a-Number. Actual: ' + (Number.MIN_VALUE % 0));
}

//CHECK#14
if (is_NaN(Number.MIN_VALUE % -0) !== true) {
  assert(false, '#14: -Number.MIN_VALUE % -0 === Not-a-Number. Actual: ' + (-Number.MIN_VALUE % -0));
}

//CHECK#15
if (is_NaN(Number.MAX_VALUE % 0) !== true) {
  assert(false, '#15: Number.MAX_VALUE % 0 === Not-a-Number. Actual: ' + (Number.MAX_VALUE % 0));
}

//CHECK#16
if (is_NaN(Number.MAX_VALUE % -0) !== true) {
  assert(false, '#16: Number.MAX_VALUE % -0 === Not-a-Number. Actual: ' + (Number.MAX_VALUE % -0));
}
*/

//? test: S11.5.3_A4_T5
//? description:...
    If dividend is finite and the divisor is an infinity, the result
    equals the dividend
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (1 % -Infinity !== 1) {
  assert(false, '#1: 1 % -Infinity === 1. Actual: ' + (1 % -Infinity));
}
//CHECK#2
if (1 % Infinity !==1) {
  assert(false, '#2: 1 % Infinity === 1. Actual: ' + (1 % Infinity));
}

//CHECK#3
if (-1 % Infinity !== -1) {
  assert(false, '#3: -1 % Infinity === -1. Actual: ' + (-1 % Infinity));
}

//CHECK#4
if (-1 % -Infinity !== -1) {
  assert(false, '#4: -1 % -Infinity === -1. Actual: ' + (-1 % -Infinity));
}

//CHECK#5
if (0 % Infinity !== 0) {
  assert(false, '#5.1: 0 % Infinity === 0. Actual: ' + (0 % Infinity));
} else {
  if (1 / (0 % Infinity) !== Infinity) {
    assert(false, '#5.2: 0 % Infinity === + 0. Actual: -0');
  }
}

//CHECK#6
if (0 % -Infinity !== 0) {
  assert(false, '#6.1: 0 % -Infinity === 0. Actual: ' + (0 % -Infinity));
} else {
  if (1 / (0 %  -Infinity) !== Infinity) {
    assert(false, '#6.2: 0 % -Infinity === + 0. Actual: -0');
  }
}

//CHECK#7
if (-0 % Infinity !== -0) {
  assert(false, '#7.1: -0 % Infinity === 0. Actual: ' + (-0 % Infinity));
} else {
  if (1 / (-0 % Infinity) !== -Infinity) {
    assert(false, '#7.2: -0 % Infinity === - 0. Actual: +0');
  }
}

//CHECK#8
if (-0 %  -Infinity !== -0) {
  assert(false, '#8.1: -0 % -Infinity === 0. Actual: ' + (-0 % -Infinity));
} else {
  if (1 / (-0 %  -Infinity) !== -Infinity) {
    assert(false, '#8.2: -0 % -Infinity === - 0. Actual: +0');
  }
}

//CHECK#9
if (Number.MAX_VALUE % -Infinity !== Number.MAX_VALUE) {
  assert(false, '#9: Number.MAX_VALUE % -Infinity === Number.MAX_VALUE. Actual: ' + (Number.MAX_VALUE % -Infinity));
}

//CHECK#10
if (Number.MAX_VALUE % Infinity !== Number.MAX_VALUE) {
  assert(false, '#10: Number.MAX_VALUE % Infinity === Number.MAX_VALUE. Actual: ' + (Number.MAX_VALUE % Infinity));
}

//CHECK#11
if (-Number.MAX_VALUE % Infinity !== -Number.MAX_VALUE) {
  assert(false, '#11: -Number.MAX_VALUE % Infinity === -Number.MAX_VALUE. Actual: ' + (-Number.MAX_VALUE % Infinity));
}

//CHECK#12
if (-Number.MAX_VALUE % -Infinity !== -Number.MAX_VALUE) {
  assert(false, '#12: -Number.MAX_VALUE % -Infinity === -Number.MAX_VALUE. Actual: ' + (-Number.MAX_VALUE % -Infinity));
}

//CHECK#13
if (Number.MIN_VALUE % -Infinity !== Number.MIN_VALUE) {
  assert(false, '#13: Number.MIN_VALUE % -Infinity === Number.MIN_VALUE. Actual: ' + (Number.MIN_VALUE % -Infinity));
}
//CHECK#14
if (Number.MIN_VALUE % Infinity !== Number.MIN_VALUE) {
  assert(false, '#14: Number.MIN_VALUE % Infinity === Number.MIN_VALUE. Actual: ' + (Number.MIN_VALUE % Infinity));
}

//CHECK#15
if (-Number.MIN_VALUE % Infinity !== -Number.MIN_VALUE) {
  assert(false, '#15: -Number.MIN_VALUE % Infinity === -Number.MIN_VALUE. Actual: ' + (-Number.MIN_VALUE % Infinity));
}

//CHECK#16
if (-Number.MIN_VALUE % -Infinity !== -Number.MIN_VALUE) {
  assert(false, '#16: -Number.MIN_VALUE % -Infinity === -Number.MIN_VALUE. Actual: ' + (-Number.MIN_VALUE % -Infinity));
}


//? test: S11.5.3_A4_T6
//? description:...
    If dividend is a zero and the divisor is nonzero finite, the
    result equals the dividend
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (0 % 1 !== 0) {
  assert(false, '#1.1: 0 % 1 === 0. Actual: ' + (0 % 1));
} else {
  if (1 / (0 % 1) !== Infinity) {
    assert(false, '#1.2: 0 % 1 === + 0. Actual: -0');
  }
}

//CHECK#2
if (0 % -1 !== 0) {
  assert(false, '#2.1: 0 % -1 === 0. Actual: ' + (0 % -1));
} else {
  if (1 / (0 %  -1) !== Infinity) {
    assert(false, '#2.2: 0 % -1 === + 0. Actual: -0');
  }
}

//CHECK#3
if (-0 % 1 !== -0) {
  assert(false, '#3.1: -0 % 1 === 0. Actual: ' + (-0 % 1));
} else {
  if (1 / (-0 % 1) !== -Infinity) {
    assert(false, '#3.2: -0 % 1 === - 0. Actual: +0');
  }
}

//CHECK#4
if (-0 %  -1 !== -0) {
  assert(false, '#4.1: -0 % -1 === 0. Actual: ' + (-0 % -1));
} else {
  if (1 / (-0 %  -1) !== -Infinity) {
    assert(false, '#4.2: 0 % -1 === - 0. Actual: +0');
  }
}

//CHECK#5
if (0 % Number.MAX_VALUE !== 0) {
  assert(false, '#5.1: 0 % Number.MAX_VALUE === 0. Actual: ' + (0 % Number.MAX_VALUE));
} else {
  if (1 / (0 % Number.MAX_VALUE) !== Infinity) {
    assert(false, '#5.2: 0 % Number.MAX_VALUE === + 0. Actual: -0');
  }
}

//CHECK#6
if (0 % Number.MIN_VALUE !== 0) {
  assert(false, '#6.1: 0 % Number.MIN_VALUE === 0. Actual: ' + (0 % Number.MIN_VALUE));
} else {
  if (1 / (0 % Number.MIN_VALUE) !== Infinity) {
    assert(false, '#6.2: 0 % Number.MIN_VALUE === + 0. Actual: -0');
  }
}

//CHECK#7
if (-0 % Number.MAX_VALUE !== -0) {
  assert(false, '#7.1: -0 % Number.MAX_VALUE === 0. Actual: ' + (-0 % Number.MAX_VALUE));
} else {
  if (1 / (-0 % Number.MAX_VALUE) !== -Infinity) {
    assert(false, '#7.2: -0 % Number.MAX_VALUE === - 0. Actual: +0');
  }
}

//CHECK#8
if (-0 % Number.MIN_VALUE !== -0) {
  assert(false, '#8.1: -0 % Number.MIN_VALUE === 0. Actual: ' + (-0 % Number.MIN_VALUE));
} else {
  if (1 / (-0 % Number.MIN_VALUE) !== -Infinity) {
    assert(false, '#8.2: 0 % Number.MIN_VALUE === - 0. Actual: +0');
  }
}


//? test: S11.5.3_A4_T7
//? description:...
    If operands neither an infinity, nor a zero, nor NaN, return x -
    truncate(x / y) * y
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


function truncate(x) {
  if (x > 0) {
    return Math.floor(x);
  } else {
    return Math.ceil(x);
  }
}

loc x, y;

//CHECK#1
x = 1.3;
y = 1.1;
if (x % y !== 0.19999999999999996) {
  assert(false, '#1: x = 1.3; y = 1.1; x % y === 0.19999999999999996. Actual: ' + (x % y));
}

//CHECK#2
x = -1.3;
y = 1.1;
if (x % y !== -0.19999999999999996) {
  assert(false, '#2: x = -1.3; y = 1.1; x % y === -0.19999999999999996. Actual: ' + (x % y));
}

//CHECK#3
x = 1.3;
y = -1.1;
if (x % y !== 0.19999999999999996) {
  assert(false, '#3: x = 1.3; y = -1.1; x % y === 0.19999999999999996. Actual: ' + (x % y));
}

//CHECK#4
x = -1.3;
y = -1.1;
if (x % y !== -0.19999999999999996) {
  assert(false, '#4: x = -1.3; y = -1.1; x % y === -0.19999999999999996. Actual: ' + (x % y));
}

//CHECK#5
x = 1.3;
y = 1.1;
if (x % y !== x - truncate(x / y) * y) {
  assert(false, '#5: x = 1.3; y = 1.1; x % y === x - truncate(x / y) * y. Actual: ' + (x % y));
}

//CHECK#6
x = -1.3;
y = 1.1;
if (x % y !== x - truncate(x / y) * y) {
  assert(false, '#6: x = -1.3; y = 1.1; x % y === x - truncate(x / y) * y. Actual: ' + (x % y));
}

//CHECK#7
x = 1.3;
y = -1.1;
if (x % y !== x - truncate(x / y) * y) {
  assert(false, '#7: x = 1.3; y = -1.1; x % y === x - truncate(x / y) * y. Actual: ' + (x % y));
}

//CHECK#8
x = -1.3;
y = -1.1;
if (x % y !== x - truncate(x / y) * y) {
  assert(false, '#8: x = -1.3; y = -1.1; x % y === x - truncate(x / y) * y. Actual: ' + (x % y));
}

