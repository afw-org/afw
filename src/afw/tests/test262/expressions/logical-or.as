#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: logical-or.as
//? customPurpose: Part of test262
//? description: expressions/logical-or
//? sourceType: script
//?
//? test: S11.11.2_A1
//? description: Checking by using eval
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ((evaluate(script("false\u0009||\u0009true"))) !== true) {
  throw '#1: (false\\u0009||\\u0009true) === true';
}

//CHECK#2
if ((evaluate(script("false\u000B||\u000Btrue"))) !== true) {
  throw '#2: (false\\u000B||\\u000Btrue) === true';
}

//CHECK#3
if ((evaluate(script("false\u000C||\u000Ctrue"))) !== true) {
  throw '#3: (false\\u000C||\\u000Ctrue) === true';
}

//CHECK#4
if ((evaluate(script("false\u0020||\u0020true"))) !== true) {
  throw '#4: (false\\u0020||\\u0020true) === true';
}

//CHECK#5
if ((evaluate(script("false\u00A0||\u00A0true"))) !== true) {
  throw '#5: (false\\u00A0||\\u00A0true) === true';
}

//CHECK#6
if ((evaluate(script("false\u000A||\u000Atrue"))) !== true) {
  throw '#6: (false\\u000A||\\u000Atrue) === true';
}

//CHECK#7
if ((evaluate(script("false\u000D||\u000Dtrue"))) !== true) {
  throw '#7: (false\\u000D||\\u000Dtrue) === true';
}

//CHECK#8
if ((evaluate(script("false\u2028||\u2028true"))) !== true) {
  throw '#8: (false\\u2028||\\u2028true) === true';
}

//CHECK#9
if ((evaluate(script("false\u2029||\u2029true"))) !== true) {
  throw '#9: (false\\u2029||\\u2029true) === true';
}


//CHECK#10
if ((evaluate(script("false\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u2029||\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u2029true"))) !== true) {
  throw '#10: (false\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u2029||\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u2029true) === true';
}


//? test: S11.11.2_A2.1_T1
//? description: Either Type is not Reference or GetBase is not null
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ((true || false) !== true) {
  throw '#1: (true || false) === true';
}

//CHECK#2
if ((false || true) !== true) {
  throw '#2: (false || true) === true';
}



//? test: S11.11.2_A2.1_T2
//? description: If GetBase(x) is null, throw ReferenceError
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

x || true;



//? test: S11.11.2_A2.1_T3
//? description:  If ToBoolean(x) is false and GetBase(y) is null, throw ReferenceError throw new Test262Error('#1.2: false || y throw ReferenceError. Actual: ' + (e));
//? expect: error:Parse error at offset 29 around line 3 column 10: Unknown built-in function 'y'
//? source: ...
#!/usr/bin/env afw

false || y;


//? test: S11.11.2_A2.1_T4
//? description: If ToBoolean(x) is true and GetBase(y) is null, return true
//? expect: error:Parse error at offset 43 around line 4 column 14: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

//CHECK#1
if ((true || x) !== true) {
  throw '#1: (true || x) === true';
}


//? test: S11.11.2_A2.4_T2
//? description: Checking with "throw"
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
let x = function () { throw "x"; };
let y = function () { throw "y"; };
try {
   x() || y();
   throw '#1.1: let x = function () { throw "x"; }; let y = function () { throw "y"; }; x() || y() throw "x". Actual: ' + (x() || y());
} catch (e) {
   if (e === "y") {
     throw '#1.2: First expression is evaluated first, and then second expression';
   } else {
     if (e !== "x") {
       throw '#1.3: let x = function () { throw "x"; }; let y = function () { throw "y"; }; x() || y() throw "x". Actual: ' + (e);
     }
   }
}


//? test: S11.11.2_A2.4_T3
//? description: Checking with undeclared variables
//? expect: error:Parse error at offset 38 around line 5 column 3: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw

//CHECK#1
try {
  x || (x = true);
  throw '#1.1: x || (x = true) throw ReferenceError. Actual: ' + (x || (x = true));
}
catch (e) {
  if ((e instanceof ReferenceError) !== true) {
    throw '#1.2: x || (x = true) throw ReferenceError. Actual: ' + (e);
  }
}

//CHECK#2
if (((y = true) || y) !== true) {
  throw '#2: ((y = true) || y) === true';
}


//? test: S11.11.2_A3_T1
//? description:  Type(x) and Type(y) vary between primitive boolean and Boolean object
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ((false || true) !== true) {
  throw '#1: (false || true) === true';
}

//CHECK#2
if ((false || false) !== false) {
  throw '#2: (false || false) === false';
}


//? test: S11.11.2_A4_T1
//? description:  Type(x) and Type(y) vary between primitive boolean and Boolean object
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (((true || true)) !== true) {
  throw '#1: (true || true) === true';
}

//CHECK#2
if ((true || false) !== true) {
  throw '#2: (true || false) === true';
}


//? test: S11.11.2_A4_T4
//? description: Type(x) or Type(y) vary between Null and Undefined
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ((true || undefined) !== true) {
  throw '#1: (true || undefined) === true';
}

//CHECK#2
if ((true || null) !== true) {
  throw '#2: (true || null) === true';
}




