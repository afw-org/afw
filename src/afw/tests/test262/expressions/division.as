#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: division.as
//? customPurpose: Part of test262
//? description: Tests division operator
//? sourceType: script
//?
//? test: line-terminator
//? description: Line terminator between the operands of a division operator
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


let x = 18

/

2

/

9
;

assert(x ===  1);


//? test: no-magic-asi-from-block-eval
//? description: division after block statement (no ASI)
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


let of = 4;
let g = 2;

let notRegExp = eval('{[42]}.8/of/g');

assert(notRegExp ===  .1);


//? test: no-magic-asi
//? description: No ASI happening in identifier used as operands
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


let instance = 60;
let of = 6;
let g = 2;

let notRegExp = instance/of/g;

assert(notRegExp ===  5);



//? test: S11.5.2_A1
//? description: Checking by using eval
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (evaluate(script("1\u0009/\u00091")) !== 1) {
  throw '#1: 1\\u0009/\\u00091 === 1';
}

//CHECK#2
if (evaluate(script("1\u000B/\u000B1")) !== 1) {
  throw '#2: 1\\u000B/\\u000B1 === 1';
}

//CHECK#3
if (evaluate(script("1\u000C/\u000C1")) !== 1) {
  throw '#3: 1\\u000C/\\u000C1 === 1';
}

//CHECK#4
if (evaluate(script("1\u0020/\u00201")) !== 1) {
  throw '#4: 1\\u0020/\\u00201 === 1';
}

//CHECK#5
if (evaluate(script("1\u00A0/\u00A01")) !== 1) {
  throw '#5: 1\\u00A0/\\u00A01 === 1';
}

//CHECK#6
if (evaluate(script("1\u000A/\u000A1")) !== 1) {
  throw '#6: 1\\u000A/\\u000A1 === 1';
}

//CHECK#7
if (evaluate(script("1\u000D/\u000D1")) !== 1) {
  throw '#7: 1\\u000D/\\u000D1 === 1';
}

//CHECK#8
if (evaluate(script("1\u2028/\u20281")) !== 1) {
  throw '#8: 1\\u2028/\\u20281 === 1';
}

//CHECK#9
if (evaluate(script("1\u2029/\u20291")) !== 1) {
  throw '#9: 1\\u2029/\\u20291 === 1';
}

//CHECK#10
if (evaluate(script("1\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u2029/\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u20291")) !== 1) {
  throw '#10: 1\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u2029/\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u20291 === 1';
}


//? test: S11.5.2_A2.1_T1
//? description: Either Type is not Reference or GetBase is not null
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (1 / 1 !== 1) {
  throw '#1: 1 / 1 === 1. Actual: ' + (1 / 1);
}

//CHECK#2
let x = 1;
if (x / 1 !== 1) {
  throw '#2: let x = 1; x / 1 === 1. Actual: ' + (x / 1);
}

//CHECK#3
let y = 1;
if (1 / y !== 1) {
  throw '#3: let y = 1; 1 / y === 1. Actual: ' + (1 / y);
}

//CHECK#4
x = 1;
y = 1;
if (x / y !== 1) {
  throw '#4: let x = 1; let y = 1; x / y === 1. Actual: ' + (x / y);
}


//? test: S11.5.2_A2.1_T2
//? description: If GetBase(x) is null, throw ReferenceError
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
try {
  x / 1;
  throw '#1.1: x / 1 throw ReferenceError. Actual: ' + (x / 1);
}
catch (e) {
  if ((e instanceof ReferenceError) !== true) {
    throw '#1.2: x / 1 throw ReferenceError. Actual: ' + (e);
  }
}


//? test: S11.5.2_A2.1_T3
//? description: If GetBase(y) is null, throw ReferenceError
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
try {
  1 / y;
  throw '#1.1: 1 / y throw ReferenceError. Actual: ' + (1 / y);
}
catch (e) {
  if ((e instanceof ReferenceError) !== true) {
    throw '#1.2: 1 / y throw ReferenceError. Actual: ' + (e);
  }
}


//? test: S11.5.2_A2.4_T2
//? description: Checking with "throw"
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
let x = function () { throw "x"; };
let y = function () { throw "y"; };
try {
   x() / y();
   throw '#1.1: let x = function () { throw "x"; }; let y = function () { throw "y"; }; x() / y() throw "x". Actual: ' + (x() / y());
} catch (e) {
   if (e === "y") {
     throw '#1.2: First expression is evaluated first, and then second expression';
   } else {
     if (e !== "x") {
       throw '#1.3: let x = function () { throw "x"; }; let y = function () { throw "y"; }; x() / y() throw "x". Actual: ' + (e);
     }
   }
}


//? test: S11.5.2_A3_T1.2
//? description: Type(x) and Type(y) vary between primitive number and Number object
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (1 / 1 !== 1) {
  throw '#1: 1 / 1 === 1. Actual: ' + (1 / 1);
}


//? test: S11.5.2_A3_T1.4
//? description: Type(x) and Type(y) vary between Null and Undefined
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN(null / undefined) !== true) {
  throw '#1: null / undefined === Not-a-Number. Actual: ' + (null / undefined);
}

//CHECK#2
if (is_NaN(undefined / null) !== true) {
  throw '#2: undefined / null === Not-a-Number. Actual: ' + (undefined / null);
}

//CHECK#3
if (is_NaN(undefined / undefined) !== true) {
  throw '#3: undefined / undefined === Not-a-Number. Actual: ' + (undefined / undefined);
}

//CHECK#4
if (is_NaN(null / null) !== true) {
  throw '#4: null / null === Not-a-Number. Actual: ' + (null / null);
}


//? test: S11.5.2_A3_T1.5
//? description: Type(x) and Type(y) vary between Object object and Function object
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN({} / function(){return 1}) !== true) {
  throw '#1: {} / function(){return 1} === Not-a-Number. Actual: ' + ({} / function(){return 1});
}

//CHECK#2
if (is_NaN(function(){return 1} / {}) !== true) {
  throw '#2: function(){return 1} / {} === Not-a-Number. Actual: ' + (function(){return 1} / {});
}

//CHECK#3
if (is_NaN(function(){return 1} / function(){return 1}) !== true) {
  throw '#3: function(){return 1} / function(){return 1} === Not-a-Number. Actual: ' + (function(){return 1} / function(){return 1});
}

//CHECK#4
if (is_NaN({} / {}) !== true) {
  throw '#4: {} / {} === Not-a-Number. Actual: ' + ({} / {});
}


//? test: S11.5.2_A3_T2.6
//? description: Type(x) is different from Type(y) and both types vary between String (primitive or object) and Undefined
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN("1" / undefined) !== true) {
  throw '#1: "1" / undefined === Not-a-Number. Actual: ' + ("1" / undefined);
}

//CHECK#2
if (is_NaN(undefined / "1") !== true) {
  throw '#2: undefined / "1" === Not-a-Number. Actual: ' + (undefined / "1");
}

//CHECK#3
if (is_NaN(new String("1") / undefined) !== true) {
  throw '#3: new String("1") / undefined === Not-a-Number. Actual: ' + (new String("1") / undefined);
}

//CHECK#4
if (is_NaN(undefined / new String("1")) !== true) {
  throw '#4: undefined / new String("1") === Not-a-Number. Actual: ' + (undefined / new String("1"));
}



//? test: S11.5.2_A4_T10
//? description: If both operands are finite and nonzero, the quotient is computed and rounded using IEEE 754 round-to-nearest mode.  If the magnitude is too small to represent, the result is then a zero of appropriate sign throw new Test262Error('#2.2: Number.MIN_VALUE / -2.1 === -0. Actual: +0'); throw new Test262Error('#4.2: Number.MIN_VALUE / -2.0 === -0. Actual: +0');
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (Number.MIN_VALUE / 2.1 !== 0) {
  throw '#1: Number.MIN_VALUE / 2.1 === 0. Actual: ' + (Number.MIN_VALUE / 2.1);
}

//CHECK#2
if (Number.MIN_VALUE / -2.1 !== -0) {
  throw '#2.1: Number.MIN_VALUE / -2.1 === 0. Actual: ' + (Number.MIN_VALUE / -2.1);
} else {
  if (1 / (Number.MIN_VALUE / -2.1) !== -Infinity) {
    throw '#2.2: Number.MIN_VALUE / -2.1 === -0. Actual: +0';
  }
}

//CHECK#3
if (Number.MIN_VALUE / 2.0 !== 0) {
  throw '#3: Number.MIN_VALUE / 2.0 === 0. Actual: ' + (Number.MIN_VALUE / 2.0);
}

//CHECK#4
if (Number.MIN_VALUE / -2.0 !== -0) {
  throw '#4.1: Number.MIN_VALUE / -2.0 === -0. Actual: ' + (Number.MIN_VALUE / -2.0);
} else {
  if (1 / (Number.MIN_VALUE / -2.0) !== -Infinity) {
    throw '#4.2: Number.MIN_VALUE / -2.0 === -0. Actual: +0';
  }
}

//CHECK#5
if (Number.MIN_VALUE / 1.9 !== Number.MIN_VALUE) {
  throw '#5: Number.MIN_VALUE / 1.9 === Number.MIN_VALUE. Actual: ' + (Number.MIN_VALUE / 1.9);
}

//CHECK#6
if (Number.MIN_VALUE / -1.9 !== -Number.MIN_VALUE) {
  throw '#6: Number.MIN_VALUE / -1.9 === -Number.MIN_VALUE. Actual: ' + (Number.MIN_VALUE / -1.9);
}

//CHECK#7
if (Number.MIN_VALUE / 1.1 !== Number.MIN_VALUE) {
  throw '#7: Number.MIN_VALUE / 1.1 === Number.MIN_VALUE. Actual: ' + (Number.MIN_VALUE / 1.1);
}

//CHECK#8
if (Number.MIN_VALUE / -1.1 !== -Number.MIN_VALUE) {
  throw '#8: Number.MIN_VALUE / -1.1 === -Number.MIN_VALUE. Actual: ' + (Number.MIN_VALUE / -1.1);
}


//? test: S11.5.2_A4_T1.1
//? description: If left operand is NaN, the result is NaN
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN(NaN / NaN) !== true) {
  throw '#1: NaN / NaN === Not-a-Number. Actual: ' + (NaN / NaN);
}

//CHECK#2
if (is_NaN(NaN / +0.0) !== true) {
  throw '#2: NaN / +0 === Not-a-Number. Actual: ' + (NaN / +0);
}

//CHECK#3
if (is_NaN(NaN / -0.0) !== true) {
  throw '#3: NaN / -0 === Not-a-Number. Actual: ' + (NaN / -0);
}

//CHECK#4
if (is_NaN(NaN / Infinity) !== true) {
  throw '#4: NaN / Infinity === Not-a-Number. Actual: ' + (NaN / Infinity);
}

//CHECK#5
if (is_NaN(NaN / -Infinity) !== true) {
  throw '#5: NaN / -Infinity === Not-a-Number. Actual: ' + (NaN / -Infinity);
}

//CHECK#6
/*
if (is_NaN(NaN / Number.MAX_VALUE) !== true) {
  throw '#6: NaN / Number.MAX_VALUE === Not-a-Number. Actual: ' + (NaN / Number.MAX_VALUE);
}

//CHECK#7
if (is_NaN(NaN / Number.MIN_VALUE) !== true) {
  throw '#7: NaN / Number.MIN_VALUE === Not-a-Number. Actual: ' + (NaN / Number.MIN_VALUE);
}
*/

//CHECK#8
if (is_NaN(NaN / 1.0) !== true) {
  throw '#8: NaN / 1 === Not-a-Number. Actual: ' + (NaN / 1);
}


//? test: S11.5.2_A4_T1.2
//? description: If right operand is NaN, the result is NaN
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN(NaN / NaN) !== true) {
  throw '#1: NaN / NaN === Not-a-Number. Actual: ' + (NaN / NaN);
}

//CHECK#2
if (is_NaN(+0.0 / NaN) !== true) {
  throw '#2: +0 / NaN === Not-a-Number. Actual: ' + (+0 / NaN);
}

//CHECK#3
if (is_NaN(-0.0 / NaN) !== true) {
  throw '#3: -0 / NaN === Not-a-Number. Actual: ' + (-0 / NaN);
}

//CHECK#4
if (is_NaN(Infinity / NaN) !== true) {
  throw '#4: Infinity / NaN === Not-a-Number. Actual: ' + (Infinity / NaN);
}

//CHECK#5
if (is_NaN(-Infinity / NaN) !== true) {
  throw '#5:  -Infinity / NaN === Not-a-Number. Actual: ' + ( -Infinity / NaN);
}

//CHECK#6
/*
if (is_NaN(Number.MAX_VALUE / NaN) !== true) {
  throw '#6: Number.MAX_VALUE / NaN === Not-a-Number. Actual: ' + (Number.MAX_VALUE / NaN);
}

//CHECK#7
if (is_NaN(Number.MIN_VALUE / NaN) !== true) {
  throw '#7: Number.MIN_VALUE / NaN === Not-a-Number. Actual: ' + (Number.MIN_VALUE / NaN);
}
*/

//CHECK#8
if (is_NaN(1.0 / NaN) !== true) {
  throw '#8: 1 / NaN === Not-a-Number. Actual: ' + (1 / NaN);
}


//? test: S11.5.2_A4_T2
//? description: The sign of the result is positive if both operands have the same sign, negative if the operands have different signs
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (1 / 1 !== 1) {
  throw '#1: 1 / 1 === 1. Actual: ' + (1 / 1);
}

//CHECK#2
if (1 / -1 !== -1) {
  throw '#2: 1 / -1 === -1. Actual: ' + (1 / -1);
}

//CHECK#3
if (-1 / 1 !== -1) {
  throw '#3: -1 / 1 === -1. Actual: ' + (-1 / 1);
}

//CHECK#4
if (-1 / -1 !== 1) {
  throw '#4: -1 / -1 === 1. Actual: ' + (-1 / -1);
}


//? test: S11.5.2_A4_T3
//? description: Division of an infinity by a zero results in an infinity of appropriate sign
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (-Infinity / 0.0 !== -Infinity) {
  throw '#1: Infinity / 0 === Infinity. Actual: ' + string(Infinity / 0.0);
}

//CHECK#2
if (-Infinity / -0.0 !== Infinity) {
  throw '#2: -Infinity / -0 === Infinity. Actual: ' + string(-Infinity / -0.0);
}

//CHECK#3
if (Infinity / 0.0 !== Infinity) {
  throw '#3: Infinity / 0 === Infinity. Actual: ' + string(Infinity / 0.0);
}

//CHECK#4
if (Infinity / -0.0 !== -Infinity) {
  throw '#4: Infinity / -0 === -Infinity. Actual: ' + string(Infinity / -0.0);
}


//? test: S11.5.2_A4_T4
//? description: Division of an infinity by an infinity results in NaN
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (is_NaN(-Infinity / -Infinity) !== true) {
  throw '#1: -Infinity / -Infinity === Not-a-Number. Actual: ' + (-Infinity / -Infinity);
}

//CHECK#2
if (is_NaN(Infinity / Infinity) !== true) {
  throw '#2: Infinity / Infinity === Not-a-Number. Actual: ' + (Infinity / Infinity);
}

//CHECK#3
if (is_NaN(-Infinity / Infinity) !== true) {
  throw '#3: -Infinity / Infinity === Not-a-Number. Actual: ' + (-Infinity / Infinity);
}

//CHECK#4
if (is_NaN(Infinity / -Infinity) !== true) {
  throw '#4: Infinity / -Infinity === Not-a-Number. Actual: ' + (Infinity / -Infinity);
}


//? test: S11.5.2_A4_T5
//? description: Division of an infinity by a finite non-zero value results in a signed infinity
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (-Infinity / 1 !== -Infinity) {
  throw '#1: -Infinity / 1 === -Infinity. Actual: ' + (-Infinity / 1);
}

//CHECK#2
if (-Infinity / -1 !== Infinity) {
  throw '#2: -Infinity / -1 === Infinity. Actual: ' + (-Infinity / -1);
}

//CHECK#3
if (Infinity / 1 !== Infinity) {
  throw '#3: Infinity / 1 === Infinity. Actual: ' + (Infinity / 1);
}

//CHECK#4
if (Infinity / -1 !== -Infinity) {
  throw '#4: Infinity / -1 === -Infinity. Actual: ' + (Infinity / -1);
}

//CHECK#5
if (Infinity / -Number.MAX_VALUE !== -Infinity) {
  throw '#5: Infinity / -Number.MAX_VALUE === -Infinity. Actual: ' + (Infinity / -Number.MAX_VALUE);
}

//CHECK#6
if (-Infinity / Number.MIN_VALUE !== -Infinity) {
  throw '#6: -Infinity / Number.MIN_VALUE === -Infinity. Actual: ' + (-Infinity / Number.MIN_VALUE);
}


//? test: S11.5.2_A4_T6
//? description: Division of a finite value by an infinity results in zero of appropriate sign throw new Test262Error('#1.2: 1 / -Infinity === - 0. Actual: +0'); throw new Test262Error('#2.2: -1 / -Infinity === + 0. Actual: -0'); throw new Test262Error('#3.2: 1 / Infinity === + 0. Actual: -0'); throw new Test262Error('#4.2: -1 / Infinity === - 0. Actual: +0');
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (1.0 / -Infinity !== -0.0) {
  throw '#1.1: 1 / -Infinity === 0. Actual: ' + string(1.0 / -Infinity);
} else {
  if (1.0 / (1.0 / -Infinity) !== -Infinity) {
    throw '#1.2: 1 / -Infinity === - 0. Actual: +0';
  }
}

//CHECK#2
if (-1.0 / -Infinity !== +0.0) {
  throw '#2.1: -1 / -Infinity === 0. Actual: ' + string(-1.0 / -Infinity);
} else {
  if (1.0 / (-1.0 / -Infinity) !== Infinity) {
    throw '#2.2: -1 / -Infinity === + 0. Actual: -0';
  }
}

//CHECK#3
if (1.0 / Infinity !== +0.0) {
  throw '#3.1: 1 / Infinity === 0. Actual: ' + string(1.0 / Infinity);
} else {
  if (1.0 / (1.0 / Infinity) !== Infinity) {
    throw '#3.2: 1 / Infinity === + 0. Actual: -0';
  }
}

//CHECK#4
if (-1.0 / Infinity !== -0.0) {
  throw '#4.1: -1 / Infinity === 0. Actual: ' + string(-1.0 / Infinity);
} else {
  if (1.0 / (-1.0 / Infinity) !== -Infinity) {
    throw '#4.2: -1 / Infinity === - 0. Actual: +0';
  }
}


//? test: S11.5.2_A4_T7
//? description: Division of a zero by a zero results in NaN
//? expect: error:Integer divide by zero error
//? source: ...
#!/usr/bin/env afw

/* In JS, these result in NaN, but in AS they throw exceptions, because they can't return NaN (double) */

//CHECK#1
if (is_NaN(+0 / +0) !== true) {
  throw '#1: +0 / +0 === Not-a-Number. Actual: ' + (+0 / +0);
}

//CHECK#2
if (is_NaN(-0 / +0) !== true) {
  throw '#2: -0 / +0 === Not-a-Number. Actual: ' + (-0 / +0);
}

//CHECK#3
if (is_NaN(+0 / -0) !== true) {
  throw '#3: +0 / -0 === Not-a-Number. Actual: ' + (+0 / -0);
}

//CHECK#4
if (is_NaN(-0 / -0) !== true) {
  throw '#4: -0 / -0 === Not-a-Number. Actual: ' + (-0 / -0);
}


//? test: S11.5.2_A4_T8
//? description: Division of a zero by any non-zero finite value -0 results in zero of appropriate sign throw new Test262Error('#1.2: -0 / 1 === - 0. Actual: +0'); throw new Test262Error('#2.2: -0 / -1 === + 0. Actual: -0'); throw new Test262Error('#3.2: +0 / 1 === + 0. Actual: -0'); throw new Test262Error('#4.2: +0 / -1 === - 0. Actual: +0'); throw new Test262Error('#5.2: +0 / -Number.MAX_VALUE === - 0. Actual: +0'); throw new Test262Error('#6.2: -0 / Number.MIN_VALUE === - 0. Actual: +0');
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (-0 / 1 !== -0) {
  throw '#1.1: -0 / 1 === 0. Actual: ' + (-0 / 1);
} else {
  if (1 / (-0 / 1) !== -Infinity) {
    throw '#1.2: -0 / 1 === - 0. Actual: +0';
  }
}

//CHECK#2
if (-0 / -1 !== +0) {
  throw '#2.1: -0 / -1 === 0. Actual: ' + (-0 / -1);
} else {
  if (1 / (-0 / -1) !== Infinity) {
    throw '#2.2: -0 / -1 === + 0. Actual: -0';
  }
}

//CHECK#3
if (+0 / 1 !== +0) {
  throw '#3.1: +0 / 1 === 0. Actual: ' + (+0 / 1);
} else {
  if (1 / (+0 / 1) !== Infinity) {
    throw '#3.2: +0 / 1 === + 0. Actual: -0';
  }
}

//CHECK#4
if (+0 / -1 !== -0) {
  throw '#4.1: +0 / -1 === 0. Actual: ' + (+0 / -1);
} else {
  if (1 / (+0 / -1) !== -Infinity) {
    throw '#4.2: +0 / -1 === - 0. Actual: +0';
  }
}

//CHECK#5
if (+0 / -Number.MAX_VALUE !== -0) {
  throw '#5.1: 0 / -Number.MAX_VALUE === 0. Actual: ' + (0 / -Number.MAX_VALUE);
} else {
  if (1 / (+0 / -Number.MAX_VALUE) !== -Infinity) {
    throw '#5.2: +0 / -Number.MAX_VALUE === - 0. Actual: +0';
  }
}

//CHECK#6
if (-0 / Number.MIN_VALUE !== -0) {
  throw '#6.1: -0 / Number.MIN_VALUE === 0. Actual: ' + (-0 / Number.MIN_VALUE);
} else {
  if (1 / (-0 / Number.MIN_VALUE) !== -Infinity) {
    throw '#6.2: -0 / Number.MIN_VALUE === - 0. Actual: +0';
  }
}


//? test: S11.5.2_A4_T9
//? description: If the magnitude is too large to represent, the result is then an infinity of appropriate sign
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (Number.MAX_VALUE / 0.9 !== Infinity) {
  throw '#1: Number.MAX_VALUE / 0.9 === Infinity. Actual: ' + (Number.MAX_VALUE / 0.9);
}

//CHECK#2
if (Number.MAX_VALUE / -0.9 !== -Infinity) {
  throw '#2: Number.MAX_VALUE / -0.9 === -Infinity. Actual: ' + (Number.MAX_VALUE / -0.9);
}

//CHECK#3
if (Number.MAX_VALUE / 1 !== Number.MAX_VALUE) {
  throw '#3: Number.MAX_VALUE / 1 === Number.MAX_VALUE. Actual: ' + (Number.MAX_VALUE / 1);
}

//CHECK#4
if (Number.MAX_VALUE / -1 !== -Number.MAX_VALUE) {
  throw '#4: Number.MAX_VALUE / -1 === -Number.MAX_VALUE. Actual: ' + (Number.MAX_VALUE / -1);
}

//CHECK#5
if (Number.MAX_VALUE / (Number.MAX_VALUE / 0.9) === (Number.MAX_VALUE / Number.MAX_VALUE) / 0.9) {
  throw '#5: Number.MAX_VALUE / (Number.MAX_VALUE / 0.9) !== (Number.MAX_VALUE / Number.MAX_VALUE) / 0.9';
}


