#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: whitespace.as
//? customPurpose: Part of test262
//? description: whitespace
//? sourceType: script
//?
//? test: between-form-feed
//? description: Insert real FORM FEED between tokens of var x=1
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


letx=1;

assert(x === 1);


//? test: between-horizontal-tab
//? description: Insert real HORIZONTAL TAB between tokens of var x=1
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


	let  x	=	1	;

assert(x === 1);


//? test: between-nbsp
//? description: Insert real NO-BREAK SPACE between tokens of var x=1
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


 let x = 2 ;

assert(x === 2);


//? test: between-space
//? description: Insert real SPACE between tokens of var x=1
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


 let x = 2 ;

assert(x === 2);


//? test: between-vertical-tab
//? description: Insert real VERTICAL TAB between tokens of var x=1
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


letx=1;

assert(x === 1);


//? test: comment-multi-form-feed
//? description: Use FORM FEED(\u000C)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


// CHECK#1
evaluate(script("/*\u000C multi line \u000C comment \u000C*/"));

//CHECK#2
let x = 0;
evaluate(script("/*\u000C multi line \u000C comment \u000C x = 1;*/"));
if (x !== 0) {
  throw '#1: let x = 0; evaluate(script("/*\\u000C multi line \\u000C comment \\u000C x = 1;*/")); x === 0. Actual: ' + (x);
}


//? test: comment-multi-horizontal-tab
//? description: Use HORIZONTAL TAB(\u0009)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


// CHECK#1
evaluate(script("/*\u0009 multi line \u0009 comment \u0009*/"));

//CHECK#2
let x = 0;
evaluate(script("/*\u0009 multi line \u0009 comment \u0009 x = 1;*/"));
if (x !== 0) {
  throw '#1: let x = 0; evaluate(script("/*\\u0009 multi line \\u0009 comment \\u0009 x = 1;*/")); x === 0. Actual: ' + (x);
}


//? test: comment-multi-nbsp
//? description: Use NO-BREAK SPACE(\u00A0)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


// CHECK#1
evaluate(script("/*\u00A0 multi line \u00A0 comment \u00A0*/"));

//CHECK#2
let x = 0;
evaluate(script("/*\u00A0 multi line \u00A0 comment \u00A0 x = 1;*/"));
if (x !== 0) {
  throw '#1: let x = 0; evaluate(script("/*\\u00A0 multi line \\u00A0 comment \\u00A0 x = 1;*/")); x === 0. Actual: ' + (x);
}


//? test: comment-multi-space
//? description: Use SPACE(\u0020)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


// CHECK#1
evaluate(script("/*\u0020 multi line \u0020 comment \u0020*/"));

//CHECK#2
let x = 0;
evaluate(script("/*\u0020 multi line \u0020 comment \u0020 x = 1;*/"));
if (x !== 0) {
  throw '#1: let x = 0; evaluate(script("/*\\u0020 multi line \\u0020 comment \\u0020 x = 1;*/")); x === 0. Actual: ' + (x);
}


//? test: comment-multi-vertical-tab
//? description: Use VERTICAL TAB(\u000B)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


// CHECK#1
evaluate(script("/*\u000B multi line \u000B comment \u000B*/"));

//CHECK#2
let x = 0;
evaluate(script("/*\u000B multi line \u000B comment \u000B x = 1;*/"));
if (x !== 0) {
  throw '#1: let x = 0; evaluate(script("/*\\u000B multi line \\u000B comment \\u000B x = 1;*/")); x === 0. Actual: ' + (x);
}


//? test: comment-single-form-feed
//? description: Use FORM FEED(\u000C)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


// CHECK#1
evaluate(script("//\u000C single line \u000C comment \u000C"));

//CHECK#2
let x = 0;
evaluate(script("//\u000C single line \u000C comment \u000C x = 1;"));
if (x !== 0) {
  throw '#1: let x = 0; evaluate(script("//\\u000C single line \\u000C comment \\u000C x = 1;")); x === 0. Actual: ' + (x);
}


//? test: comment-single-horizontal-tab
//? description: Use HORIZONTAL TAB(\u0009)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


// CHECK#1
evaluate(script("//\u0009 single line \u0009 comment \u0009"));

//CHECK#2
let x = 0;
evaluate(script("//\u0009 single line \u0009 comment \u0009 x = 1;"));
if (x !== 0) {
  throw '#1: let x = 0; evaluate(script("//\\u0009 single line \\u0009 comment \\u0009 x = 1;")); x === 0. Actual: ' + (x);
}


//? test: comment-single-nbsp
//? description: Use NO-BREAK SPACE(\u00A0)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


// CHECK#1
evaluate(script("//\u00A0 single line \u00A0 comment \u00A0"));

//CHECK#2
let x = 0;
evaluate(script("//\u00A0 single line \u00A0 comment \u00A0 x = 1;"));
if (x !== 0) {
  throw '#1: let x = 0; evaluate(script("//\\u00A0 single line \\u00A0 comment \\u00A0 x = 1;")); x === 0. Actual: ' + (x);
}


//? test: comment-single-space
//? description: Use SPACE(\u0020)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


// CHECK#1
evaluate(script("//\u0020 single line \u0020 comment \u0020"));

//CHECK#2
let x = 0;
evaluate(script("//\u0020 single line \u0020 comment \u0020 x = 1;"));
if (x !== 0) {
  throw '#1: let x = 0; evaluate(script("//\\u0020 single line \\u0020 comment \\u0020 x = 1;")); x === 0. Actual: ' + (x);
}


//? test: comment-single-vertical-tab
//? description: Use VERTICAL TAB(\u000B)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


// CHECK#1
evaluate(script("//\u000B single line \u000B comment \u000B"));

//CHECK#2
let x = 0;
evaluate(script("//\u000B single line \u000B comment \u000B x = 1;"));
if (x !== 0) {
  throw '#1: let x = 0; evaluate(script("//\\u000B single line \\u000B comment \\u000B x = 1;")); x === 0. Actual: ' + (x);
}



//? test: mongolian-vowel-separator
//? description: mongolian-vowel-separator
//? expect: error
//? source: ...
#!/usr/bin/env afw

// U+180E between "let" and "foo"; UTF8(0x180E) = 0xE1 0xA0 0x8E
let᠎foo;


//? test: S7.2_A2.1_T2
//? description: Use real HORIZONTAL TAB
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ("	str	ing	" !== "\u0009str\u0009ing\u0009") {
  throw '#1: "	str	ing	" === "\\u0009str\\u0009ing\\u0009"';
}


//? test: S7.2_A2.2_T2
//? description: Use real VERTICAL TAB
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ("string" !== "\u000Bstr\u000Bing\u000B") {
  throw '#1: "string" === "\\u000Bstr\\u000Bing\\u000B"';
}


//? test: S7.2_A2.3_T2
//? description: Use real FORM FEED
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ("string" !== "\u000Cstr\u000Cing\u000C") {
  throw '#1: "string" === "\\u000Cstr\\u000Cing\\u000C"';
}


//? test: S7.2_A2.4_T2
//? description: Use real SPACE
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (" str ing " !== "\u0020str\u0020ing\u0020") {
  throw '#1: " str ing " === "\\u0020str\\u0020ing\\u0020"';
}


//? test: S7.2_A2.5_T2
//? description: Use real NO-BREAK SPACE
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (" str ing " !== "\u00A0str\u00A0ing\u00A0") {
  throw '#1: " str ing " === "\\u00A0str\\u00A0ing\\u00A0"';
}


//? test: S7.2_A3.1_T2
//? description: Use real HORIZONTAL TAB
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
let x = 0;
//	single	line	comment	x = 1;
if (x !== 0) {
  throw '#1: let x = 0; //	single	line	comment	x = 1; x === 0. Actual: ' + (x);
}


//? test: S7.2_A3.2_T2
//? description: Use real VERTICAL TAB
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
let x = 0;
//singlelinecommentx = 1;
if (x !== 0) {
  throw '#1: let x = 0; //singlelinecommentx = 1; x === 0. Actual: ' + (x);
}


//? test: S7.2_A3.3_T2
//? description: Use real FORM FEED
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
let x = 0;
//singlelinecommentx = 1;
if (x !== 0) {
  throw '#1: let x = 0; //singlelinecommentx = 1; x === 0. Actual: ' + (x);
}


//? test: S7.2_A3.4_T2
//? description: Use real SPACE
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
let x = 0;
// single line comment x = 1;
if (x !== 0) {
  throw '#1: let x = 0; // single line comment x = 1; x === 0. Actual: ' + (x);
}


//? test: S7.2_A3.5_T2
//? description: Use real NO-BREAK SPACE
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
let x = 0;
// single line comment x = 1;
if (x !== 0) {
  throw '#1: let x = 0; // single line comment x = 1; x === 0. Actual: ' + (x);
}


//? test: S7.2_A4.1_T2
//? description: Use real HORIZONTAL TAB
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


/*CHECK#1*/
let x = 0;
/*	multi	line	comment	x = 1;*/
if (x !== 0) {
  throw '#1: let x = 0; /*	multi	line	comment	x = 1;*/ x === 0. Actual: ' + (x);
}


//? test: S7.2_A4.2_T2
//? description: Use real VERTICAL TAB
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


/*CHECK#1*/
let x = 0;
/*multilinecommentx = 1;*/
if (x !== 0) {
  throw '#1: let x = 0; /*multilinecommentx = 1;*/ x === 0. Actual: ' + (x);
}


//? test: S7.2_A4.3_T2
//? description: Use real FORM FEED
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


/*CHECK#1*/
let x = 0;
/*multilinecommentx = 1;*/
if (x !== 0) {
  throw '#1: let x = 0; /*multilinecommentx = 1;*/ x === 0. Actual: ' + (x);
}


//? test: S7.2_A4.4_T2
//? description: Use real SPACE
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


/*CHECK#1*/
let x = 0;
/* multi line comment x = 1;*/
if (x !== 0) {
  throw '#1: let x = 0; /* multi line comment x = 1;*/ x === 0. Actual: ' + (x);
}


//? test: S7.2_A4.5_T2
//? description: Use real NO-BREAK SPACE
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


/*CHECK#1*/
let x = 0;
/* multi line comment x = 1;*/
if (x !== 0) {
  throw '#1: let x = 0; /* multi line comment x = 1;*/ x === 0. Actual: ' + (x);
}


//? test: S7.2_A5_T1
//? description: Use TAB (U+0009)
//? expect: error
//? source: ...
#!/usr/bin/env afw




let\u0009x;


//? test: S7.2_A5_T2
//? description: Use VERTICAL TAB (U+000B)
//? expect: error
//? source: ...
#!/usr/bin/env afw




let\u000Bx;


//? test: S7.2_A5_T3
//? description: Use FORM FEED (U+000C)
//? expect: error
//? source: ...
#!/usr/bin/env afw




let\u000Cx;


//? test: S7.2_A5_T4
//? description: Use SPACE (U+0020)
//? expect: error
//? source: ...
#!/usr/bin/env afw




let\u0020x;


//? test: S7.2_A5_T5
//? description: Use NO-BREAK SPACE (U+00A0)
//? expect: error
//? source: ...
#!/usr/bin/env afw




let\u00A0x;


//? test: string-form-feed
//? description: Use FORM FEED(\u000C and \f)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


// CHECK#1
if (evaluate(script("'\u000Cstr\u000Cing\u000C'")) !== "\u000Cstr\u000Cing\u000C") {
  throw '#1: evaluate(script("\'\\u000Cstr\\u000Cing\\u000C\'")) === "\\u000Cstr\\u000Cing\\u000C"';
}

//CHECK#2
if (evaluate(script("'\fstr\fing\f'")) !== "\fstr\fing\f") {
  throw '#2: evaluate(script("\'\\fstr\\fing\\f\'")) === "\\fstr\\fing\\f"';
}


//? test: string-horizontal-tab
//? description: Use HORIZONTAL TAB(\u0009 and \t)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


// CHECK#1
if (evaluate(script("'\u0009str\u0009ing\u0009'")) !== "\u0009str\u0009ing\u0009") {
  throw '#1: evaluate(script("\'\\u0009str\\u0009ing\\u0009\'")) === "\\u0009str\\u0009ing\\u0009"';
}

//CHECK#2
if (evaluate(script("'\tstr\ting\t'")) !== "\tstr\ting\t") {
  throw '#2: evaluate(script("\'\\tstr\\ting\\t\'")) === "\\tstr\\ting\\t"';
}


//? test: string-nbsp
//? description: Use NO-BREAK SPACE(\u00A0)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


// CHECK#1
if (evaluate(script("'\u00A0str\u00A0ing\u00A0'")) !== "\u00A0str\u00A0ing\u00A0") {
  throw '#1: evaluate(script("\'\\u00A0str\\u00A0ing\\u00A0\'")) === "\\u00A0str\\u00A0ing\\u00A0"';
}


//? test: string-space
//? description: Use SPACE(\u0020)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


// CHECK#1
if (evaluate(script("'\u0020str\u0020ing\u0020'")) !== "\u0020str\u0020ing\u0020") {
  throw '#1: evaluate(script("\'\\u0020str\\u0020ing\\u0020\'")) === "\\u0020str\\u0020ing\\u0020"';
}

//CHECK#2
if (evaluate(script("' str ing '")) !== " str ing ") {
  throw '#2: evaluate(script("\' str ing \'")) === " str ing "';
}


//? test: string-vertical-tab
//? description: Use VERTICAL TAB(\u000B and \v)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


// CHECK#1
if (evaluate(script("'\u000Bstr\u000Bing\u000B'")) !== "\u000Bstr\u000Bing\u000B") {
  throw '#1: evaluate(script("\'\\u000Bstr\\u000Bing\\u000B\'")) === "\\u000Bstr\\u000Bing\\u000B"';
}

//CHECK#2
if (evaluate(script("'\vstr\ving\v'")) !== "\vstr\ving\v") {
  throw '#2: evaluate(script("\'\\vstr\\ving\\v\'")) === "\\vstr\\ving\\v"';
}
