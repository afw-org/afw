#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: less-than-or-equal.as
//? customPurpose: Part of test262
//? description: expressions/less-than-or-equal
//? sourceType: script
//?
//? test: S11.8.3_A1
//? description: Checking by using eval
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (evaluate(script("1\u0009<=\u00091")) !== true) {
  throw '#1: (1\\u0009<=\\u00091) === true';
}

//CHECK#2
if (evaluate(script("1\u000B<=\u000B1")) !== true) {
  throw '#2: (1\\u000B<=\\u000B1) === true';
}

//CHECK#3
if (evaluate(script("1\u000C<=\u000C1")) !== true) {
  throw '#3: (1\\u000C<=\\u000C1) === true';
}

//CHECK#4
if (evaluate(script("1\u0020<=\u00201")) !== true) {
  throw '#4: (1\\u0020<=\\u00201) === true';
}

//CHECK#5
if (evaluate(script("1\u00A0<=\u00A01")) !== true) {
  throw '#5: (1\\u00A0<=\\u00A01) === true';
}

//CHECK#6
if (evaluate(script("1\u000A<=\u000A1")) !== true) {
  throw '#6: (1\\u000A<=\\u000A1) === true';
}

//CHECK#7
if (evaluate(script("1\u000D<=\u000D1")) !== true) {
  throw '#7: (1\\u000D<=\\u000D1) === true';
}

//CHECK#8
if (evaluate(script("1\u2028<=\u20281")) !== true) {
  throw '#8: (1\\u2028<=\\u20281) === true';
}

//CHECK#9
if (evaluate(script("1\u2029<=\u20291")) !== true) {
  throw '#9: (1\\u2029<=\\u20291) === true';
}

//CHECK#10
if (evaluate(script("1\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u2029>\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u20290")) !== true) {
  throw '#10: (1\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u2029>\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u20290) === true';
}


//? test: S11.8.3_A2.1_T1
//? description: Either Type is not Reference or GetBase is not null
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (1 <= 1 !== true) {
  throw '#1: 1 <= 1 === true';
}

//CHECK#2
let x = 1;
if (x <= 1 !== true) {
  throw '#2: let x = 1; x <= 1 === true';
}

//CHECK#3
let y = 1;
if (1 <= y !== true) {
  throw '#3: let y = 1; 1 <= y === true';
}

//CHECK#4
x = 1;
y = 1;
if (x <= y !== true) {
  throw '#4: let x = 1; let y = 1; x <= y === true';
}


//? test: S11.8.3_A2.1_T2
//? description: If GetBase(x) is null, throw ReferenceError
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function x
//? source: ...
#!/usr/bin/env afw

x <= 1;



//? test: S11.8.3_A2.1_T3
//? description: If GetBase(y) is null, throw ReferenceError
//? expect: error:Parse error at offset 25 around line 3 column 6: Unknown built-in function y
//? source: ...
#!/usr/bin/env afw

1 <= y;


//? test: S11.8.3_A2.3_T1
//? description: Checking that operands of a "<=" evaluate left-to-right
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

//CHECK#1
let x = { valueOf: function () { throw "x"; } };
let y = { valueOf: function () { throw "y"; } };
try {
   let z = x <= y;
   throw '#1.1: Should have thrown';
} catch (e) {
   if (e.message === "y") {
     throw '#1.2: First expression should be evaluated first';
   } else {
     if (e.message !== "x") {
       throw '#1.3: Failed with: ' + e;
     }
   }
}


//? test: S11.8.3_A2.4_T2
//? description: Checking with "throw"
//? expect: null
//? source: ...
#!/usr/bin/env afw

//CHECK#1
let x = function () { throw "x"; };
let y = function () { throw "y"; };
try {
   let z = x() <= y();
   throw '#1.1: let x = function () { throw "x"; }; let y = function () { throw "y"; }; x() <= y() throw "x". Actual: ' + (x() <= y());
} catch (e) {
   if (e.message === "y") {
     throw '#1.2: First expression is evaluated first, and then second expression';
   } else {
     if (e.message !== "x") {
       throw '#1.3: let x = function () { throw "x"; }; let y = function () { throw "y"; }; x() <= y() throw "x". Actual: ' + (e);
     }
   }
}


//? test: S11.8.3_A2.4_T3
//? description: Checking with undeclarated variables
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function x
//? source: ...
#!/usr/bin/env afw

x <= (x = 1);



//? test: S11.8.3_A3.1_T1.2
//? description:  Type(Primitive(x)) and Type(Primitive(y)) vary between primitive number and Number object
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (1 <= 1 !== true) {
  throw '#1: 1 <= 1 === true';
}



//? test: S11.8.3_A3.2_T1.1
//? description:  Type(Primitive(x)) and Type(Primitive(y)) vary between primitive string and String object
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ("1" <= "1" !== true) {
  throw '#1: "1" <= "1" === true';
}

//CHECK#5
if ("x" <= "1" !== false) {
  throw '#5: "x" <= "1" === false';
}

//CHECK#6
if ("1" <= "x" !== true) {
  throw '#6: "1" <= "x" === true';
}


//? test: S11.8.3_A4.10
//? description: x and y are string primitives
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (("x " <= "x") !== false) {
  throw '#1: ("x " <= "x") === false';
}

//CHECK#2
if (("x" <= "") !== false) {
  throw '#2: ("x" <= "") === false';
}

//CHECK#3
if (("abcd" <= "ab") !== false) {
  throw '#3: ("abcd" <= ab") === false';
}

//CHECK#4
if (("abc\u0064" <= "abcd") !== true) {
  throw '#4: ("abc\\u0064" <= abcd") === true';
}

//CHECK#5
if (("x" + "y" <= "x") !== false) {
  throw '#5: ("x" + "y" <= "x") === false';
}

//CHECK#6
let x = "x";
if ((x + 'y' <= x) !== false) {
  throw '#6: let x = "x"; (x + "y" <= x) === false';
}


//? test: S11.8.3_A4.11
//? description: x and y are string primitives
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (("x" <= "x") !== true) {
  throw '#1: ("x" <= "x") === true';
}

//CHECK#2
if (("" <= "x") !== true) {
  throw '#2: ("" <= "x") === true';
}

//CHECK#3
if (("ab" <= "abcd") !== true) {
  throw '#3: ("ab" <= abcd") === true';
}

//CHECK#4
if (("abcd" <= "abc\u0064") !== true) {
  throw '#4: ("abcd" <= abc\\u0064") === true';
}

//CHECK#5
if (("x" <= "x" + "y") !== true) {
  throw '#5: ("x" <= "x" + "y") === true';
}

//CHECK#6
let x = "x";
if ((x <= x + "y") !== true) {
  throw '#6: let x = "x"; (x <= x + "y") === true';
}

//CHECK#7
if (("a\u0000" <= "a\u0000a") !== true) {
  throw '#7: ("a\\u0000" <= "a\\u0000a") === true';
}

//CHECK#8
if (("x" <= " x") !== false) {
  throw '#8: ("x" <= " x") === false';
}


//? test: S11.8.3_A4.12_T1
//? description: x and y are string primitives
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (("xx" <= "xy") !== true) {
  throw '#1: ("xx" <= "xy") === true';
}

//CHECK#2
if (("xy" <= "xx") !== false) {
  throw '#2: ("xy" <= "xx") === false';
}

//CHECK#3
if (("x" <= "y") !== true) {
  throw '#3: ("x" <= y") === true';
}

//CHECK#4
if (("aab" <= "aba") !== true) {
  throw '#4: ("aab" <= aba") === true';
}

//CHECK#5
if (("\u0061\u0061\u0061\u0062" <= "\u0061\u0061\u0061\u0061") !== false) {
  throw '#5: ("\\u0061\\u0061\\u0061\\u0062" <= \\u0061\\u0061\\u0061\\u0061") === false';
}

//CHECK#6
if (("a\u0000a" <= "a\u0000b") !== true) {
  throw '#6: ("a\\u0000a" <= "a\\u0000b") === true';
}

//CHECK#7
if (("aB" <= "aa") !== true) {
  throw '#7: ("aB" <= aa") === true';
}


//? test: S11.8.3_A4.12_T2
//? description: x and y are string primitives
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (("0" <= "x") !== true) {
  throw '#1: ("0" <= "x") !== true';
}

//CHECK#2
if (("-" <= "0") !== true) {
  throw '#2: ("-" <= "0") !== true';
}

//CHECK#3
if (("." <= "0") !== true) {
  throw '#3: ("." <= "0") !== true';
}

//CHECK#4
if (("+" <= "-") !== true) {
  throw '#4: ("+" <= "-") !== true';
}

//CHECK#5
if (("-0" <= "-1") !== true) {
  throw '#5: ("-0" <= "-1") !== true';
}

//CHECK#6
if (("+1" <= "-1") !== true) {
  throw '#6: ("+1" <= "-1") !== true';
}

//CHECK#7
if (("1" <= "1e-10") !== true) {
throw '#7: ("1" <= "1e-10") !== true';
}


//? test: S11.8.3_A4.1
//? description: y is number primitive
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ((NaN <= 0) !== false) {
  throw '#1: (NaN <= 0) === false';
}

//CHECK#2
if ((NaN <= 1.1) !== false) {
  throw '#2: (NaN <= 1.1) === false';
}

//CHECK#3
if ((NaN <= -1.1) !== false) {
  throw '#3: (NaN <= -1.1) === false';
}

//CHECK#4
if ((NaN <= NaN) !== false) {
  throw '#4: (NaN <= NaN) === false';
}

//CHECK#5
if ((NaN <= Infinity) !== false) {
  throw '#5: (NaN <= +Infinity) === false';
}

//CHECK#6
if ((NaN <= -Infinity) !== false) {
  throw '#6: (NaN <= -Infinity) === false';
}

//CHECK#7
/*
if ((NaN <= Number.MAX_VALUE) !== false) {
  throw '#7: (NaN <= Number.MAX_VALUE) === false';
}
*/

//CHECK#8
/*
if ((NaN <= Number.MIN_VALUE) !== false) {
  throw '#8: (NaN <= Number.MIN_VALUE) === false';
}
*/


//? test: S11.8.3_A4.2
//? description: x is number primitive
//? expect: null
//? source: ...
#!/usr/bin/env afw

//CHECK#1
if ((0 <= NaN) !== false) {
  throw '#1: (0 <= NaN) === false';
}

//CHECK#2
if ((1.1 <= NaN) !== false) {
  throw '#2: (1.1 <= NaN) === false';
}

//CHECK#3
if ((-1.1 <= NaN) !== false) {
  throw '#3: (-1.1 <= NaN) === false';
}

//CHECK#4
if ((NaN <= NaN) !== false) {
  throw '#4: (NaN <= NaN) === false';
}

//CHECK#5
if ((Infinity <= NaN) !== false) {
  throw '#5: (+Infinity <= NaN) === false';
}

//CHECK#6
if ((-Infinity <= NaN) !== false) {
  throw '#6: (-Infinity <= NaN) === false';
}

/*
//CHECK#7
if ((Number.MAX_VALUE <= NaN) !== false) {
  throw '#7: (Number.MAX_VALUE <= NaN) === false';
}

//CHECK#8
if ((Number.MIN_VALUE <= NaN) !== false) {
  throw '#8: (Number.MIN_VALUE <= NaN) === false';
}
*/


//? test: S11.8.3_A4.3
//? description: x and y are number primitives
//? expect: null
//? source: ...
#!/usr/bin/env afw

//CHECK#1
if ((1 <= 1) !== true) {
  throw '#1: (1 <= 1) === true';
}

//CHECK#2
if ((1.1 <= 1.1) !== true) {
  throw '#2: (1.1 <= 1.1) === true';
}

//CHECK#3
if ((-1.1 <= -1.1) !== true) {
  throw '#3: (-1.1 <= -1.1) === true';
}

//CHECK#4
if ((-Infinity <= -Infinity) !== true) {
  throw '#4: (-Infinity <= -Infinity) === true';
}

//CHECK#5
if ((Infinity <= Infinity) !== true) {
  throw '#5: (+Infinity <= +Infinity) === true';
}

/*
//CHECK#6
if ((Number.MAX_VALUE <= Number.MAX_VALUE) !== true) {
  throw '#6: (Number.MAX_VALUE <= Number.MAX_VALUE) === true';
}

//CHECK#7
if ((Number.MIN_VALUE <= Number.MIN_VALUE) !== true) {
  throw '#7: (Number.MIN_VALUE <= Number.MIN_VALUE) === true';
}
*/


//? test: S11.8.3_A4.4
//? description: Checking all combinations
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ((0 <= 0) !== true) {
  throw '#1: (0 <= 0) === true';
}

//CHECK#2
if ((-0 <= -0) !== true) {
  throw '#2: (-0 <= -0) === true';
}

//CHECK#3
if ((+0 <= -0) !== true) {
  throw '#3: (+0 <= -0) === true';
}

//CHECK#4
if ((-0 <= +0) !== true) {
  throw '#4: (-0 <= +0) === true';
}


//? test: S11.8.3_A4.5
//? description: y is number primitive
//? expect: null
//? source: ...
#!/usr/bin/env afw

//CHECK#1
if ((Infinity <= 0) !== false) {
  throw '#1: (+Infinity <= 0) === false';
}

//CHECK#2
if ((Infinity <= 1.1) !== false) {
  throw '#2: (+Infinity <= 1.1) === false';
}

//CHECK#3
if ((Infinity <= -1.1) !== false) {
  throw '#3: (+Infinity <= -1.1) === false';
}

//CHECK#4
if ((Infinity <= -Infinity) !== false) {
  throw '#4: (+Infinity <= -Infinity) === false';
}

/*
//CHECK#5
if ((Infinity <= Number.MAX_VALUE) !== false) {
  throw '#5: (+Infinity <= Number.MAX_VALUE) === false';
}

//CHECK#6
if ((Infinity <= Number.MIN_VALUE) !== false) {
  throw '#6: (+Infinity <= Number.MIN_VALUE) === false';
}
*/


//? test: S11.8.3_A4.6
//? description: x is number primitive
//? expect: null
//? source: ...
#!/usr/bin/env afw

//CHECK#1
if ((0 <= Infinity) !== true) {
  throw '#1: (0 <= +Infinity) === true';
}

//CHECK#2
if ((1.1 <= Infinity) !== true) {
  throw '#2: (1.1 <= +Infinity) === true';
}

//CHECK#3
if ((-1.1 <= Infinity) !== true) {
  throw '#3: (-1.1 <= +Infinity) === true';
}

//CHECK#4
if ((-Infinity <= Infinity) !== true) {
  throw '#4: (-Infinity <= +Infinity) === true';
}

/*
//CHECK#5
if ((Number.MAX_VALUE <= Infinity) !== true) {
  throw '#5: (Number.MAX_VALUE <= +Infinity) === true';
}

//CHECK#6
if ((Number.MIN_VALUE <= Infinity) !== true) {
  throw '#6: (Number.MIN_VALUE <= +Infinity) === true';
}
*/


//? test: S11.8.3_A4.7
//? description: y is number primitive
//? expect: null
//? source: ...
#!/usr/bin/env afw

//CHECK#1
if ((-Infinity <= 0) !== true) {
  throw '#1: (-Infinity <= 0) === true';
}

//CHECK#2
if ((-Infinity <= 1.1) !== true) {
  throw '#2: (-Infinity <= 1.1) === true';
}

//CHECK#3
if ((-Infinity <= -1.1) !== true) {
  throw '#3: (-Infinity <= -1.1) === true';
}

//CHECK#4
if ((-Infinity <= Infinity) !== true) {
  throw '#4: (-Infinity <= +Infinity) === true';
}

/*
//CHECK#5
if ((-Infinity <= Number.MAX_VALUE) !== true) {
  throw '#5: (-Infinity <= Number.MAX_VALUE) === true';
}

//CHECK#6
if ((-Infinity <= Number.MIN_VALUE) !== true) {
  throw '#6: (-Infinity <= Number.MIN_VALUE) === true';
}
*/


//? test: S11.8.3_A4.8
//? description: x is number primitive
//? expect: null
//? source: ...
#!/usr/bin/env afw

//CHECK#1
if ((0 <= -Infinity) !== false) {
  throw '#1: (0 <= -Infinity) === false';
}

//CHECK#2
if ((1.1 <= -Infinity) !== false) {
  throw '#2: (1.1 <= -Infinity) === false';
}

//CHECK#3
if ((-1.1 <= -Infinity) !== false) {
  throw '#3: (-1.1 <= -Infinity) === false';
}

//CHECK#4
if ((Infinity <= -Infinity) !== false) {
  throw '#4: (+Infinity <= -Infinity) === false';
}

/*
//CHECK#5
if ((Number.MAX_VALUE <= -Infinity) !== false) {
  throw '#5: (Number.MAX_VALUE <= -Infinity) === false';
}

//CHECK#6
if ((Number.MIN_VALUE <= -Infinity) !== false) {
  throw '#6: (Number.MIN_VALUE <= -Infinity) === false';
}
*/


//? test: S11.8.3_A4.9
//? description: x and y are number primitives
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

//CHECK#1
if ((1.1 <= 1) !== false) {
  throw '#1: (1.1 <= 1) === false';
}

//CHECK#2
if ((1 <= 1.1) !== true) {
  throw '#2: (1 <= 1.1) === true';
}

//CHECK#3
if ((-1.1 <= -1) !== true) {
  throw '#3: (-1.1 <= -1) === true';
}

//CHECK#4
// fixme this won't pass because of integer/double conversion
if ((-1 <= -1.1) !== false) {
  throw '#4: (-1 <= -1.1) === false';
}

//CHECK#5
if ((0 <= 0.1) !== true) {
  throw '#5: (0 <= 0.1) === true';
}

//CHECK#6
if ((-0.1 <= 0) !== true) {
  throw '#6: (-0.1 <= 0) === true';
}

/*
//CHECK#7
if ((Number.MAX_VALUE/2 <= Number.MAX_VALUE) !== true) {
  throw '#7: (Number.MAX_VALUE/2 <= Number.MAX_VALUE) === true';
}

//CHECK#8
if ((Number.MIN_VALUE <= Number.MIN_VALUE*2) !== true) {
  throw '#8: (Number.MIN_VALUE <= Number.MIN_VALUE*2) === true';
}
*/