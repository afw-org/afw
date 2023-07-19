#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: line-terminators.as
//? customPurpose: Part of test262
//? description: line-terminators
//? sourceType: script
//?
//? test: 7.3-15
//? description: 7.3 - ES5 recognize <BOM> (\uFFFF) as a whitespace character
//? expect: null
//? source: ...
#!/usr/bin/env afw


        let prop = "a\uFFFFa";

assert(length(prop) === 3,  'prop.length');
assert(prop !== "aa", 'prop');
//assert(prop[1], "\uFFFF",  'prop[1]');


//? test: 7.3-5
//? description:...
    7.3 - ES5 recognizes the character <LS> (\u2028) as terminating
    string literal
//? expect: null
//? source: ...
#!/usr/bin/env afw


        let prop = "66\u2028123";

assert(prop === "66\u2028123", 'prop');
//assert(prop[2] === "\u2028", 'prop[2]');
assert(length(prop) === 6, 'prop.length');


//? test: 7.3-6
//? description:...
    7.3 - ES5 recognizes the character <PS> (\u2029) as terminating
    string literal
//? expect: null
//? source: ...
#!/usr/bin/env afw


        let prop = "66\u2029123";

assert(prop === "66\u2029123",  'prop');
//assert(prop[2] === "\u2029",  'prop[2]');
assert(length(prop) === 6,  'prop.length');


//? test: between-tokens-cr
//? description: Insert real CARRIAGE RETURN between tokens of let x=1
//? expect: null
//? source: ...
#!/usr/bin/env afw


let
x
=
1;

if (x !== 1) {
  throw '#1: let\\nx\\n=\\n1\\n; x === 1. Actual: ' + (x);
}


//? test: between-tokens-lf
//? description: Insert real LINE FEED between tokens of let x=1
//? expect: null
//? source: ...
#!/usr/bin/env afw


let
x
=
1;

assert(x ===  1);


//? test: between-tokens-ls
//? description: Insert LINE SEPARATOR (\u2028) between tokens of let x=1
//? expect: null
//? source: ...
#!/usr/bin/env afw


let x = 1 ;

assert(x ===  1);


//? test: between-tokens-ps
//? description: Insert PARAGRAPH SEPARATOR (\u2029) between tokens of let x=1
//? expect: null
//? source: ...
#!/usr/bin/env afw


let x = 1 ;

assert(x === 1);


//? test: comment-multi-cr
//? description: Insert real CARRIAGE RETURN into multi line comment
//? expect: null
//? source: ...
#!/usr/bin/env afw


let x = 0;
/*
multi^Mline^Mcomment^Mx = 1;
*/
x = 1;
if (x !== 0) {
}


//? test: comment-multi-lf
//? description: Insert real LINE FEED into multi line comment
//? expect: error
//? source: ...
#!/usr/bin/env afw


// Because this test concerns the interpretation of non-executable character
// sequences within ECMAScript source code, special care must be taken to
// ensure that executable code is evaluated as expected.
//
// Express the intended behavior by intentionally throwing an error; this
// guarantees that test runners will only consider the test "passing" if
// executable sequences are correctly interpreted as such.

let x = 0;

/*
x = 1;
*/

if (x === 0) {
  assert(false);
}


//? test: comment-multi-ls
//? description: Insert LINE SEPARATOR (U+2028) into multi line comment
//? expect: error
//? source: ...
#!/usr/bin/env afw


// Because this test concerns the interpretation of non-executable character
// sequences within ECMAScript source code, special care must be taken to
// ensure that executable code is evaluated as expected.
//
// Express the intended behavior by intentionally throwing an error; this
// guarantees that test runners will only consider the test "passing" if
// executable sequences are correctly interpreted as such.

let x = 0;

/* x = 1; */

if (x === 0) {
  assert(false);
}


//? test: comment-multi-ps
//? description: Insert real PARAGRAPH SEPARATOR into multi line comment
//? expect: error
//? source: ...
#!/usr/bin/env afw


// Because this test concerns the interpretation of non-executable character
// sequences within ECMAScript source code, special care must be taken to
// ensure that executable code is evaluated as expected.
//
// Express the intended behavior by intentionally throwing an error; this
// guarantees that test runners will only consider the test "passing" if
// executable sequences are correctly interpreted as such.

let x = 0;

/* x = 1; */

if (x === 0) {
  assert(false);
}


//? test: comment-single-cr
//? description: Insert CARRIAGE RETURN (U+000D) into the end of single line comment
//? expect: error
//? source: ...
#!/usr/bin/env afw


// Because this test concerns the interpretation of non-executable character
// sequences within ECMAScript source code, special care must be taken to
// ensure that executable code is evaluated as expected.
//
// Express the intended behavior by intentionally throwing an error; this
// guarantees that test runners will only consider the test "passing" if
// executable sequences are correctly interpreted as such.

// single line
assert(false);


//? test: comment-single-lf
//? description: Insert LINE FEED (\u000A) into single line comment
//? expect: error
//? source: ...
#!/usr/bin/env afw


// Because this test concerns the interpretation of non-executable character
// sequences within ECMAScript source code, special care must be taken to
// ensure that executable code is evaluated as expected.
//
// Express the intended behavior by intentionally throwing an error; this
// guarantees that test runners will only consider the test "passing" if
// executable sequences are correctly interpreted as such.

// single line
assert(false);


//? test: comment-single-ls
//? description: Insert LINE SEPARATOR (U+2028) into the end of single line comment
//? expect: error
//? source: ...
#!/usr/bin/env afw


// Because this test concerns the interpretation of non-executable character
// sequences within ECMAScript source code, special care must be taken to
// ensure that executable code is evaluated as expected.
//
// Express the intended behavior by intentionally throwing an error; this
// guarantees that test runners will only consider the test "passing" if
// executable sequences are correctly interpreted as such.

// single line assert(false);


//? test: comment-single-ps
//? description:...
    Insert PARAGRAPH SEPARATOR (U+2029) into the end of single line
    comment
//? expect: error
//? source: ...
#!/usr/bin/env afw


// Because this test concerns the interpretation of non-executable character
// sequences within ECMAScript source code, special care must be taken to
// ensure that executable code is evaluated as expected.
//
// Express the intended behavior by intentionally throwing an error; this
// guarantees that test runners will only consider the test "passing" if
// executable sequences are correctly interpreted as such.

// single line assert(false);


//? test: invalid-comment-single-cr
//? description: Insert CARRIAGE RETURN (\u000D) into begin of single line comment
//? expect: error
//? source: ...
#!/usr/bin/env afw




//
 this text is not included in the single-line comment that precedes it


//? test: invalid-comment-single-lf
//? description: Insert real LINE FEED into single line comment
//? expect: error
//? source: ...
#!/usr/bin/env afw




// CHECK#1
//single
line comment


//? test: invalid-comment-single-ls
//? description: Insert LINE SEPARATOR (\u2028) into single line comment
//? expect: error
//? source: ...
#!/usr/bin/env afw




// single line LS> ??? (invalid)


//? test: invalid-comment-single-ps
//? description: Insert PARAGRAPH SEPARATOR (\u2029) into single line comment
//? expect: error
//? source: ...
#!/usr/bin/env afw




// single line PS> ??? (invalid)


//? test: invalid-regexp-cr
//? description:...
    7.3 - ES5 recognizes the character <CR> (\u000D) as terminating
    regular expression literals
//? expect: error
//? source: ...
#!/usr/bin/env afw




/
/


//? test: invalid-regexp-lf
//? description:...
    7.3 - ES5 recognizes the character <LF> (\u000A) as terminating
    regular expression literals
//? expect: error
//? source: ...
#!/usr/bin/env afw




/
/


//? test: invalid-regexp-ls
//? description:...
    7.3 - ES5 recognizes the character <LS> (\u2028) as terminating
    regular expression literals
//? expect: error
//? source: ...
#!/usr/bin/env afw




/ /


//? test: invalid-regexp-ps
//? description:...
    7.3 - ES5 recognizes the character <PS> (\u2029) as terminating
    regular expression literals
//? expect: error
//? source: ...
#!/usr/bin/env afw




/ /


//? test: invalid-string-cr
//? description: Insert CARRIAGE RETURN (\u000D) into string
//? expect: error
//? skip: true
//? source: ...
#!/usr/bin/env afw




'
'


//? test: invalid-string-lf
//? description: Insert LINE FEED (\u000A) into string
//? expect: error
//? skip: true
//? source: ...
#!/usr/bin/env afw




'
'


//? test: S7.3_A2.1_T2
//? description: Use real LINE FEED into string
//? expect: error
//? source: ...
#!/usr/bin/env afw




//CHECK#1
"
str
ing
";


//? test: S7.3_A2.2_T2
//? description: Insert real CARRIAGE RETURN into string
//? expect: error
//? source: ...
#!/usr/bin/env afw




//CHECK#1
"
str
ing
";


//? test: S7.3_A3.2_T1
//? description: Insert CARRIAGE RETURN (\u000D) into single line comment
//? expect: error
//? source: ...
#!/usr/bin/env afw




// single line comment
 ??? (invalid)


//? test: S7.3_A5.4
//? description: Insert PARAGRAPH SEPARATOR (U+2029) into multi line comment
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1

//CHECK#2
let x = 0;
if (x !== 0) {
}


//? test: S7.3_A6_T1
//? description: Insert LINE FEED (U+000A) in let x
//? expect: error
//? source: ...
#!/usr/bin/env afw




let\u000Ax;


//? test: S7.3_A6_T2
//? description: Insert CARRIAGE RETURN (U+000D) in let x
//? expect: error
//? source: ...
#!/usr/bin/env afw




let\u000Dx;


//? test: S7.3_A6_T3
//? description: Insert LINE SEPARATOR (U+2028) in let x
//? expect: error
//? source: ...
#!/usr/bin/env afw




let\u2028x;


//? test: S7.3_A6_T4
//? description: Insert PARAGRAPH SEPARATOR (U+2029) in let x
//? expect: error
//? source: ...
#!/usr/bin/env afw




let\u2029x;


//? test: S7.3_A7_T1
//? description: Insert Line Terminator in let x=y+z
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
let y=2;
let z=3;
let
x
=
y
+
z
;
if (x !== 5) {
  throw '#1: let\\nx\\n=\\ny\\n+\\nz\\n; x === 5. Actual: ' + (x);
}
x=0;

// CHECK#2
y=2;
z=3;
x
=
y
+
z
;
if (x !== 5) {
  throw '#2: let\\nx\\n=\\ny\\n+\\nz\\n; x === 5. Actual: ' + (x);
}


//? test: S7.3_A7_T2
//? description: Insert Line Terminator in let x=y-z
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
let y=3;
let z=2;
let
x
=
y
-
z
;
if (x !== 1) {
  throw '#1: let\\nx\\n=\\ny\\n-\\nz\\n; x === 1. Actual: ' + (x);
}
x=0;

// CHECK#2
y=3;
z=2;
x
=
y
-
z
;
if (x !== 1) {
  throw '#2: let\\nx\\n=\\ny\\n-\\nz\\n; x === 1. Actual: ' + (x);
}


//? test: S7.3_A7_T3
//? description: Insert Line Terminator in let x=y*z
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
let y=3;
let z=2;
let
x
=
y
*
z
;
if (x !== 6) {
  throw '#1: let\\nx\\n=\\ny\\n*\\nz\\n; x === 6. Actual: ' + (x);
}
x=0;

// CHECK#2
y=3;
z=2;
x
=
y
*
z
;
if (x !== 6) {
  throw '#2: let\\nx\\n=\\ny\\n*\\nz\\n; x === 6. Actual: ' + (x);
}


//? test: S7.3_A7_T4
//? description: Insert Line Terminator in let x=y/z
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
let y=12;
let z=2;
let
x
=
y
/
z
;
if (x !== 6) {
  throw '#1: let\\nx\\n=\\ny\\n/\\nz\\n; x === 6. Actual: ' + (x);
}
x=0;

// CHECK#2
y=12;
z=2;
x
=
y
/
z
;
if (x !== 6) {
  throw '#2: let\\nx\\n=\\ny\\n/\\nz\\n; x === 6. Actual: ' + (x);
}


//? test: S7.3_A7_T5
//? description: Insert Line Terminator in let x=y%z
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
let y=16;
let z=10;
let
x
=
y
%
z
;
if (x !== 6) {
  throw '#1: let\\nx\\n=\\ny\\n%\\nz\\n; x === 6. Actual: ' + (x);
}
x=0;

// CHECK#2
y=16;
z=10;
x
=
y
%
z
;
if (x !== 6) {
  throw '#2: let\\nx\\n=\\ny\\n%\\nz\\n; x === 6. Actual: ' + (x);
}




//? test: S7.3_A7_T8
//? description: Insert Line Terminator in let x=y<z
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
let y=2;
let z=3;
let
x
=
y
<
z
;
if (x !== true) {
  throw '#1: let\\nx\\n=\\ny\\n<\\nz\\n; x === true. Actual: ' + (x);
}
x=0;

// CHECK#2
y=2;
z=3;
x
=
y
<
z
;
if (x !== true) {
  throw '#2: let\\nx\\n=\\ny\\n<\\nz\\n; x === true. Actual: ' + (x);
}
