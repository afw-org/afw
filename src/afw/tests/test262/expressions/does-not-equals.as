#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: does-not-equals.as
//? customPurpose: Part of test262
//? description: Tests does not equals
//? sourceType: script
//? test: S11.9.2_A1
//? description:Checking by using eval
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (evaluate(script("true\u0009!=\u00091")) !== false) {
  assert(false, '#1: (true\\u0009!=\\u00091) === false');
}

//CHECK#2
if (evaluate(script("true\u000B!=\u000B1")) !== false) {
  assert(false, '#2: (true\\u000B!=\\u000B1) === false');
}

//CHECK#3
if (evaluate(script("true\u000C!=\u000C1")) !== false) {
  assert(false, '#3: (true\\u000C!=\\u000C1) === false');
}

//CHECK#4
if (evaluate(script("true\u0020!=\u00201")) !== false) {
  assert(false, '#4: (true\\u0020!=\\u00201) === false');
}

//CHECK#5
if (evaluate(script("true\u00A0!=\u00A01")) !== false) {
  assert(false, '#5: (true\\u00A0!=\\u00A01) === false');
}

//CHECK#6
if (evaluate(script("true\u000A!=\u000A1")) !== false) {
  assert(false, '#6: (true\\u000A!=\\u000A1) === false');
}

//CHECK#7
if (evaluate(script("true\u000D!=\u000D1")) !== false) {
  assert(false, '#7: (true\\u000D!=\\u000D1) === false');
}

//CHECK#8
if (evaluate(script("true\u2028!=\u20281")) !== false) {
  assert(false, '#8: (true\\u2028!=\\u20281) === false');
}

//CHECK#9
if (evaluate(script("true\u2029!=\u20291")) !== false) {
  assert(false, '#9: (true\\u2029!=\\u20291) === false');
}

//CHECK#10
if (evaluate(script("true\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u2029!=\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u20291")) !== false) {
  assert(false, '#10: (true\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u2029!=\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u20291) === false');
}


//? test: S11.9.2_A2.1_T1
//? description:Either Type is not Reference or GetBase is not null
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ((1 != 1) !== false) {
  assert(false, '#1: (1 != 1) === false');
}

//CHECK#2
loc x = 1;
if ((x != 1) !== false) {
  assert(false, '#2: loc x = 1; (x != 1) === false');
}

//CHECK#3
loc y = 1;
if ((1 != y) !== false) {
  assert(false, '#3: loc y = 1; (1 != y) === false');
}

//CHECK#4
x = 1;
y = 1;
if ((x != y) !== false) {
  assert(false, '#4: loc x = 1; loc y = 1; (x != y) === false');
}


//? test: S11.9.2_A2.1_T2
//? description:If GetBase(x) is null, throw ReferenceError
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function x
//? source: ...
#!/usr/bin/env afw

x != 1;
  


//? test: S11.9.2_A2.1_T3
//? description:If GetBase(y) is null, throw ReferenceError
//? expect: error:Parse error at offset 25 around line 3 column 6: Unknown built-in function y
//? source: ...
#!/usr/bin/env afw

1 != y;



//? test: S11.9.2_A2.4_T2
//? description:Checking with "throw"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
loc x = function () { throw "x"; };
loc y = function () { throw "y"; };
try {
   x() != y();
   assert(false, '#1.1: loc x = function () { throw "x"; }; loc y = function () { throw "y"; }; x() != y() throw "x". Actual: ' + (x() != y()));
} catch (e) {
   if (e === "y") {
     assert(false, '#1.2: First expression is evaluated first, and then second expression');
   } else {
     if (e !== "x") {
       assert(false, '#1.3: loc x = function () { throw "x"; }; loc y = function () { throw "y"; }; x() != y() throw "x". Actual: ' + (e));
     }
   }
}


//? test: S11.9.2_A3.1
//? description:x and y are boolean primitives
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ((true != true) !== false) {
  assert(false, '#1: (true != true) === false');
}

//CHECK#2
if ((false != false) !== false) {
  assert(false, '#2: (false != false) === false');
}

//CHECK#3
if ((true != false) !== true) {
  assert(false, '#3: (true != false) === true');
}

//CHECK#4
if ((false != true) !== true) {
  assert(false, '#4: (false != true) === true');
}


//? test: S11.9.2_A4.1_T1
//? description:x is NaN
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ((NaN != true) !== true) {
  assert(false, '#1: (NaN != true) === true');
}

//CHECK#2
if ((NaN != 1) !== true) {
  assert(false, '#2: (NaN != 1) === true');
}

//CHECK#3
if ((NaN != NaN) !== true) {
  assert(false, '#3: (NaN != NaN) === true');
}

//CHECK#4
if ((NaN != Infinity) !== true) {
  assert(false, '#4: (NaN != +Infinity) === true');
}

//CHECK#5
if ((NaN != -Infinity) !== true) {
  assert(false, '#5: (NaN != -Infinity) === true');
}

//CHECK#6
/*
if ((NaN != Number.MAX_VALUE) !== true) {
  assert(false, '#6: (NaN != Number.MAX_VALUE) === true');
}
*/

//CHECK#7
/*
if ((NaN != Number.MIN_VALUE) !== true) {
  assert(false, '#7: (NaN != Number.MIN_VALUE) === true');
}
*/

//CHECK#8
if ((NaN != "string") !== true) {
  assert(false, '#8: (NaN != "string") === true');
}

//CHECK#9
if ((NaN != {}) !== true) {
  assert(false, '#9: (NaN != {}) === true');
}


//? test: S11.9.2_A4.1_T2
//? description:y is NaN
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ((true != NaN) !== true) {
  assert(false, '#1: (true != NaN) === true');
}

//CHECK#2
if ((-1 != NaN) !== true) {
  assert(false, '#2: (-1 != NaN) === true');
}

//CHECK#3
if ((NaN != NaN) !== true) {
  assert(false, '#3: (NaN != NaN) === true');
}

//CHECK#4
if ((Infinity != NaN) !== true) {
  assert(false, '#4: (+Infinity != NaN) === true');
}

//CHECK#5
if ((-Infinity != NaN) !== true) {
  assert(false, '#5: (-Infinity != NaN) === true');
}

//CHECK#6
/*
if ((Number.MAX_VALUE != NaN) !== true) {
  assert(false, '#6: (Number.MAX_VALUE != NaN) === true');
}
*/

//CHECK#7
/*
if ((Number.MIN_VALUE != NaN) !== true) {
  assert(false, '#7: (Number.MIN_VALUE != NaN) === true');
}
*/

//CHECK#8
if (("string" != NaN) !== true) {
  assert(false, '#8: ("string" != NaN) === true');
}



//? test: S11.9.2_A4.2
//? description:Checking all combinations
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ((+0 != -0) !== false) {
  assert(false, '#1: (+0 != -0) === false');
}

//CHECK#2
if ((-0 != +0) !== false) {
  assert(false, '#2: (-0 != +0) === false');
}


//? test: S11.9.2_A4.3
//? description:x and y are primitive numbers
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ((Infinity != Infinity) !== false) {
  assert(false, '#1: (+Infinity != +Infinity) === false');
}

//CHECK#2
if ((-Infinity != -Infinity) !== false) {
  assert(false, '#2: (-Infinity != -Infinity) === false');
}



//? test: S11.9.2_A5.1
//? description:x and y are primitive strings
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (("" != "") !== false) {
  assert(false, '#1: ("" != "") === false');
}

//CHECK#2
if ((" " != " ") !== false) {
  assert(false, '#2: " (" != " ") === false');
}

//CHECK#3
if ((" " != "") !== true) {
  assert(false, '#3: " (" != "") === true');
}

//CHECK#4
if (("string" != "string") !== false) {
  assert(false, '#4: ("string" != "string") === false');
}

//CHECK#5
if ((" string" != "string ") !== true) {
  assert(false, '#5: (" string" != "string ") === true');
}

//CHECK#6
if (("1.0" != "1") !== true) {
  assert(false, '#6: ("1.0" != "1") === true');
}

//CHECK#7
if (("0xff" != "255") !== true) {
  assert(false, '#7: ("0xff" != "255") === true');
}



//? test: S11.9.2_A6.1
//? description: Checking all combinations
//? expect: null
//? source: ...
#!/usr/bin/env afw

//CHECK#1
if ((undefined != undefined) !== false) {
  assert(false, '#1: (undefined != undefined) === false');
}

//CHECK#3
if ((undefined != evaluate(script("loc x;"))) !== false) {
  assert(false, '#3: (undefined != evaluate(script("loc x"))) === false');
}

//CHECK#4
if ((undefined != null) !== false) {
  assert(false, '#4: (undefined != null) === false');
}

//CHECK#6
if ((null != null) !== false) {
  assert(false, '#6: (null != null) === false');
}