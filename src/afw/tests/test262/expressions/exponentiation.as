#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: exponentiation.as
//? customPurpose: Part of test262
//? description: expressions/exponentiation
//? sourceType: script
//?
//? test: applying-the-exp-operator_A10
//? description:If abs(base) < 1 and exponent is −∞, the result is +∞.
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


let exponent = -Infinity;
let bases = [
0.999999999999999,
0.5,
+0.0,
-0.0,
-0.5,
-0.999999999999999,
];

for (let i = 0; i < length(bases); i = i + 1) {
  if (pow(bases[i], exponent) !== +Infinity) {
    throw "(" + string(bases[i]) + " ** " + string(exponent) +") !== +Infinity";
  }
}


//? test: applying-the-exp-operator_A11
//? description:If base is +∞ and exponent > 0, the result is +∞.
//? expect: undefined
//? source: ...
#!/usr/bin/env afw



let base = +Infinity;
let exponents = [
Infinity,
1.7976931348623157E308, //largest (by module) finite number
1.0,
0.000000000000001
];

for (let i = 0; i < length(exponents); i = i + 1) {
	if (base ** exponents[i] !== +Infinity) {
		throw "(" + string(base) + " ** " + string(exponents[i]) + ") !== +Infinity";
	}
}


//? test: applying-the-exp-operator_A12
//? description:If base is +∞ and exponent < 0, the result is +0.
//? expect: undefined
//? source: ...
#!/usr/bin/env afw



let base = +Infinity;
let exponents = [
-Infinity,
-1.7976931348623157E308, //largest (by module) finite number
-1.0,
-0.000000000000001
];

for (let i = 0; i < length(exponents); i = i + 1) {
  if ((base ** exponents[i]) !== +0.0) {
    throw "(" + string(base) + " **  " + string(exponents[i]) + ") !== +0";
  }
}


//? test: applying-the-exp-operator_A13
//? description:If base is −∞ and exponent > 0 and exponent is an odd integer, the result is −∞.
//? expect: undefined
//? source: ...
#!/usr/bin/env afw



let base = -Infinity;
let exponents = [
1.0,
111.0,
111111.0
];

for (let i = 0; i < length(exponents); i = i + 1) {
  if ((base ** exponents[i]) !== -Infinity) {
    throw "(" + string(base) + " **  " + string(exponents[i]) + ") !== -Infinity";
  }
}


//? test: applying-the-exp-operator_A14
//? description:If base is −∞ and exponent > 0 and exponent is not an odd integer, the result is +∞.
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw



let base = -Infinity;
let exponents = [
0.000000000000001,
2,
Math.PI,
1.7976931348623157E308, //largest finite number
+Infinity
];

for (let i = 0; i < length(exponents); i = i + 1) {
  if ((base ** exponents[i]) !== +Infinity) {
    throw "(" + string(base) + " **  " + string(exponents[i]) + ") !== +Infinity";
  }
}


//? test: applying-the-exp-operator_A15
//? description:If base is −∞ and exponent < 0 and exponent is an odd integer, the result is −0.
//? expect: undefined
//? source: ...
#!/usr/bin/env afw



let base = -Infinity;
let exponents = [
-1.0,
-111.0,
-111111.0
];

for (let i = 0; i < length(exponents); i = i + 1) {
  assert(base ** exponents[i] ===  -0.0, string(base) + "** " + string(exponents[i]));
}


//? test: applying-the-exp-operator_A16
//? description:If base is −∞ and exponent < 0 and exponent is not an odd integer, the result is +0.
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw



let base = -Infinity;
let exponents = [
-0.000000000000001,
-2,
-Math.PI,
-1.7976931348623157E308, //largest (by module) finite number
-Infinity
];

for (let i = 0; i < length(exponents); i = i + 1) {
  if ((base ** exponents[i]) !== +0) {
    throw "(" + string(base) + " **  " + string(exponents[i]) + ") !== +0";
  }
}


//? test: applying-the-exp-operator_A17
//? description:If base is +0 and exponent > 0, the result is +0.
//? expect: undefined
//? source: ...
#!/usr/bin/env afw



let base = +0.0;
let exponents = [
Infinity,
1.7976931348623157E308, //largest finite number
1.0,
0.000000000000001
];

for (let i = 0; i < length(exponents); i = i + 1) {
  if ((base ** exponents[i]) !== +0.0) {
    throw "(" + string(base) + " **  " + string(exponents[i]) + ") !== +0";
  }
}


//? test: applying-the-exp-operator_A18
//? description:If base is +0 and exponent < 0, the result is +∞.
//? expect: undefined
//? source: ...
#!/usr/bin/env afw



let base = +0.0;
let exponents = [
-Infinity,
-1.7976931348623157E308, //largest (by module) finite number
-1.0,
-0.000000000000001
];

for (let i = 0; i < length(exponents); i = i + 1) {
  if ((base ** exponents[i]) !== +Infinity) {
    throw "(" + string(base) + " **  " + string(exponents[i]) + ") !== +Infinity";
  }
}


//? test: applying-the-exp-operator_A19
//? description:If base is −0 and exponent > 0 and exponent is an odd integer, the result is −0.
//? expect: undefined
//? source: ...
#!/usr/bin/env afw



let base = -0.0;
let exponents = [
1.0,
111.0,
111111.0
];

for (let i = 0; i < length(exponents); i = i + 1) {
  assert(base ** exponents[i] ===  -0.0, string(base) + "**  " + string(exponents[i]));
}


//? test: applying-the-exp-operator_A1
//? description:If exponent is NaN, the result is NaN.
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


let exponent = NaN;
let bases = [
-Infinity,
-1.7976931348623157E308, //largest (by module) finite number
-0.000000000000001,
-0.0,
+0.0,
0.000000000000001,
1.7976931348623157E308, //largest finite number
+Infinity,
NaN
];

for (let i = 0; i < length(bases); i = i + 1) {
  assert(
    is_NaN(bases[i] ** exponent),    
    string(bases[i]) + " ** " + string(exponent)
  );
}


//? test: applying-the-exp-operator_A20
//? description:If base is −0 and exponent > 0 and exponent is not an odd integer, the result is +0.
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw



let base = -0;
let exponents = [
0.000000000000001,
2,
Math.PI,
1.7976931348623157E308, //largest finite number
+Infinity
];

for (let i = 0; i < length(exponents); i = i + 1) {
  if ((base ** exponents[i]) !== +0) {
    throw "(" + string(base) + " **  " + string(exponents[i]) + ") !== +0";
  }
}


//? test: applying-the-exp-operator_A21
//? description:If base is −0 and exponent < 0 and exponent is an odd integer, the result is −∞.
//? expect: undefined
//? source: ...
#!/usr/bin/env afw



let base = -0.0;
let exponents = [
-1.0,
-111.0,
-111111.0
];

for (let i = 0; i < length(exponents); i = i + 1) {
  if ((base ** exponents[i]) !== -Infinity) {
    throw "(" + string(base) + " **  " + string(exponents[i]) + ") !== -Infinity";
  }
}


//? test: applying-the-exp-operator_A22
//? description:If base is −0 and exponent < 0 and exponent is not an odd integer, the result is +∞.
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw



let base = -0;
let exponents = [
-0.000000000000001,
-2,
-Math.PI,
-1.7976931348623157E308, //largest (by module) finite number
-Infinity
];

for (let i = 0; i < length(exponents); i = i + 1) {
  if ((base ** exponents[i]) !== +Infinity) {
    throw "(" + string(base) + " **  " + string(exponents[i]) + ") !== +Infinity";
  }
}


//? test: applying-the-exp-operator_A23
//? description:If base < 0 and base is finite and exponent is finite and exponent is not an integer, the result is NaN.
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw



let bases = [];
-1.7976931348623157E308, //largest (by module) finite number
-Math.PI,
-1,
-0.000000000000001
];

let exponents = [
-Math.PI,
-Math.E,
-1.000000000000001,
-0.000000000000001,
0.000000000000001,
1.000000000000001,
Math.E,
Math.PI
];

for (let i = 0; i < length(bases); i = i + 1) {
  for (let j = 0; j < length(exponents); j++) {
    assert(
      bases[i] ** exponents[j] == NaN,      
      bases[i] + " ** " + exponents[j]
    );
  }
}



//? test: applying-the-exp-operator_A2
//? description: If exponent is +0, the result is 1, even if base is NaN. throw new Test262Error("(" + bases[i] + " **  " + exponent + ") !== 1");
//? expect: undefined
//? source: ...
#!/usr/bin/env afw



let exponent = +0.0;
let bases = [
-Infinity,
-1.7976931348623157E308, //largest (by module) finite number
-0.000000000000001,
-0.0,
+0.0,
0.000000000000001,
1.7976931348623157E308, //largest finite number
+Infinity,
NaN
];

for (let i = 0; i < length(bases); i = i + 1) {
  if ((bases[i] ** exponent) !== 1.0) {
    throw "(" + string(bases[i]) + " **  " + string(exponent) +") !== 1";
  }
}


//? test: applying-the-exp-operator_A3
//? description: If exponent is −0, the result is 1, even if base is NaN. throw new Test262Error("(" + bases[i] + " ** -0) !== 1");
//? expect: undefined
//? source: ...
#!/usr/bin/env afw



let exponent = -0.0;
let bases = [
-Infinity,
-1.7976931348623157E308, //largest (by module) finite number
-0.000000000000001,
-0.0,
+0.0,
0.000000000000001,
1.7976931348623157E308, //largest finite number
+Infinity,
NaN
];

for (let i = 0; i < length(bases); i = i + 1) {
  if ((bases[i] ** exponent) !== 1.0) {
    throw "(" + string(bases[i]) + " ** -0) !== 1";
  }
}


//? test: applying-the-exp-operator_A4
//? description:If base is NaN and exponent is nonzero, the result is NaN.
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

let base = NaN;
let exponents = [
-Infinity,
-1.7976931348623157E308, //largest (by module) finite number
-0.000000000000001,
0.000000000000001,
1.7976931348623157E308, //largest finite number
+Infinity,
NaN
];

for (let i = 0; i < length(exponents); i = i + 1) {
  assert(
    is_NaN(base ** exponents[i]),
    string(base) + "** " + string(exponents[i])
  );
}


//? test: applying-the-exp-operator_A5
//? description:If abs(base) > 1 and exponent is +∞, the result is +∞.
//? expect: undefined
//? source: ...
#!/usr/bin/env afw



let exponent = +Infinity;
let bases = [
-Infinity,
-1.7976931348623157E308, //largest (by module) finite number
-1.000000000000001,
1.000000000000001,
1.7976931348623157E308, //largest finite number
+Infinity
];

for (let i = 0; i < length(bases); i = i + 1) {
  if ((bases[i] ** exponent) !== +Infinity) {
    throw "(" + string(bases[i]) + " **  " + string(exponent) +") !== +Infinity";
  }
}


//? test: applying-the-exp-operator_A6
//? description:If abs(base) > 1 and exponent is −∞, the result is +0.
//? expect: undefined
//? source: ...
#!/usr/bin/env afw



let exponent = -Infinity;
let bases = [
-Infinity,
-1.7976931348623157E308, //largest (by module) finite number
-1.000000000000001,
1.000000000000001,
1.7976931348623157E308, //largest finite number
+Infinity
];

for (let i = 0; i < length(bases); i = i + 1) {
  if ((bases[i] ** exponent) !== +0.0) {
    throw "(" + string(bases[i]) + " **  " + string(exponent) +") !== +0";
  }
}


//? test: applying-the-exp-operator_A7
//? description:If abs(base) is 1 and exponent is +∞, the result is NaN.
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

let exponent = +Infinity;
let bases = [
-1.0,
1.0,
];

for (let i = 0; i < length(bases); i = i + 1) {
  assert(
    is_NaN(bases[i] ** exponent),
    string(bases[i]) + " ** " + string(exponent)
  );
}


//? test: applying-the-exp-operator_A8
//? description:If abs(base) is 1 and exponent is −∞, the result is NaN.
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

let exponent = -Infinity;
let bases = [
-1.0,
1.0,
];

for (let i = 0; i < length(bases); i = i + 1) {
  assert(
    is_NaN(bases[i] ** exponent),
    string(bases[i]) + " ** " + string(exponent)
  );
}


//? test: applying-the-exp-operator_A9
//? description:If abs(base) < 1 and exponent is +∞, the result is +0.
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


let exponent = +Infinity;
let bases = [
    0.999999999999999,
    0.5,
    +0.0,
    0.0,
    -0.5,
    -0.999999999999999
];

for (let i = 0; i < length(bases); i = i + 1) {
  if ((bases[i] ** exponent) !== +0.0) {
    throw "(" + string(bases[i]) + " **  " + string(exponent) +") !== +0";
  }
}


//? test: exp-operator-evaluation-order
//? description:Exponentiation Operator expression order of evaluation
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


let capture = [];
let leftValue = { valueOf() { capture.push("leftValue"); return 3; }};
let rightValue = { valueOf() { capture.push("rightValue"); return 2; }};

(capture.push("left"), leftValue) ** (capture.push("right"), rightValue);

// Expected per operator evaluation order: "left", "right", "leftValue", "rightValue"

assert(capture[0] ===  "left", "Expected the 1st element captured to be 'left'");
assert(capture[1] ===  "right", "Expected the 2nd element captured to be 'right'");
assert(capture[2] ===  "leftValue", "Expected the 3rd element captured to be 'leftValue'");
assert(capture[3] ===  "rightValue", "Expected the 4th element captured to be 'rightValue'");


//? test: exp-operator
//? description: Performs exponential calculation on operands. Same algorithm as %MathPow%(base, exponent)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


let exponent = 2.0;
assert(2.0 ** 3.0 ===  8.0, "(2 ** 3) === 8");
assert(3.0 * 2.0 ** 3.0 ===  24.0, "(3 * 2 ** 3) === 24");
//assert(2 ** ++exponent ===  8, "(2 ** ++exponent) === 8");
assert(2.0 ** -1.0 * 2.0 ===  1.0, "(2 ** -1 * 2) === 1");
assert(2.0 ** 2.0 * 4.0 ===  16.0, "(2 ** 2 * 4) === 16");
assert(2.0 ** 2.0 / 2.0 ===  2.0, "(2 ** 2 / 2) === 2");
assert(2.0 ** (3.0 ** 2.0) ===  512.0, "(2 ** (3 ** 2)) === 512");
assert(2.0 ** 3.0 ** 2.0 ===  512.0, "(2 ** 3 ** 2) === 512");
assert(16.0 / 2.0 ** 2.0 ===  4.0, "(16 / 2 ** 2) === 4");


//? test: exp-operator-precedence-unary-expression-semantics
//? description:Exponentiation Operator expression precedence of unary operators
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


assert(-(3.0 ** 2.0) === -9.0, "-(3 ** 2) === -9");
assert(+(3.0 ** 2.0) === 9.0, "+(3 ** 2) === 9");
//assert(~(3 ** 2) === -10, "~(3 ** 2) === -10");
//assert(!(3.0 ** 2.0) === false, "!(3 ** 2) === false");


assert(2.0 ** -2.0 === 0.25);

let o = { p: 1 };

let s = "2";
let n = 2.0;

//assert(2 ** +s ===  4, "+s -> +'2' -> 2 -> ToNumber(2) -> 2");
assert(2.0 ** +n ===  4.0, "+s -> +2 -> 2 -> ToNumber(2) -> 2");

//assert(2.0 ** -s ===  0.25, "-s -> -'2' -> -2 -> ToNumber(-2) -> -2");
assert(2.0 ** -n ===  0.25, "-s -> -2 -> -2 -> ToNumber(-2) -> -2");

//assert(2 ** ~s ===  0.125, "~s -> ~'2' -> -3 -> ToNumber(-3) -> -3");
//assert(2 ** ~n ===  0.125, "~s -> ~2 -> -3 -> ToNumber(-3) -> -3");

//assert(2 ** !s ===  1, "!s -> !'2' -> false -> ToNumber(false) -> 0");
//assert(2.0 ** !n ===  1.0, "!s -> !2 -> false -> ToNumber(false) -> 0");




//? test: exp-operator-syntax-error-logical-not-unary-expression-base
//? description:Exponentiation Expression syntax error, `!` UnaryExpression
//? expect: error
//? source: ...
#!/usr/bin/env afw

!1 ** 2;


//? test: exp-operator-syntax-error-negate-unary-expression-base
//? description:Exponentiation Expression syntax error, `-` UnaryExpression
//? expect: error
//? source: ...
#!/usr/bin/env afw

-3 ** 2;


//? test: exp-operator-syntax-error-plus-unary-expression-base
//? description:Exponentiation Expression syntax error, `+` UnaryExpression
//? expect: error
//? source: ...
#!/usr/bin/env afw

+1 ** 2;


//? test: exp-operator-syntax-error-typeof-unary-expression-base
//? description:Exponentiation Expression syntax error, `typeof` UnaryExpression
//? expect: error
//? source: ...
#!/usr/bin/env afw

typeof 1 ** 2;


//? test: int32_min-exponent
//? description: Using -(2**31) as exponent with the exponentiation operator should behave as expected. "2**-(gonzo huge exponent > 1074) should be +0 because " + "2**-1074 is the smallest positive IEEE-754 number"); "1**-(gonzo huge exponent > 1074) should be 1");
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


const INT32_MIN = -2147483648.0;

assert(2.0**INT32_MIN === +0.0,
                 "2**-(gonzo huge exponent > 1074) should be +0 because " +
                 "2**-1074 is the smallest positive IEEE-754 number");

assert(1.0**INT32_MIN === 1.0,
                 "1**-(gonzo huge exponent > 1074) should be 1");


//? test: order-of-evaluation
//? description:Type coercion order of operations for exponentiation operator
//? expect: undefined
//? skip: true
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
  })() ** (function() {
    trace += "2";
    throw "should not be evaluated";
  })();
}, "?GetValue(lhs) throws.");
assert(trace ===  "1", "?GetValue(lhs) throws.");

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
  })() ** (function() {
    trace += "2";
    throw new MyError();
  })();
}, "?GetValue(rhs) throws.");
assert(trace ===  "12", "?GetValue(rhs) throws.");

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
  })() ** (function() {
    trace += "2";
    return {
      valueOf: function() {
        trace += "4";
        throw "should not be evaluated";
      }
    };
  })();
}, "?ToPrimive(lhs) throws.");
assert(trace ===  "123", "?ToPrimive(lhs) throws.");

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
  })() ** (function() {
    trace += "2";
    return {
      valueOf: function() {
        trace += "4";
        throw new MyError();
      }
    };
  })();
}, "?ToPrimive(rhs) throws.");
assert(trace ===  "1234", "?ToPrimive(rhs) throws.");

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
  })() ** (function() {
    trace += "2";
    return {
      valueOf: function() {
        trace += "4";
        throw "should not be evaluated";
      }
    };
  })();
}, "?ToNumeric(lhs) throws.");
assert(trace ===  "123", "?ToNumeric(lhs) throws.");

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
  })() ** (function() {
    trace += "2";
    return {
      valueOf: function() {
        trace += "4";
        return Symbol("1");
      }
    };
  })();
}, "GetValue(lhs) throws.");
assert(trace ===  "1234", "GetValue(lhs) throws.");