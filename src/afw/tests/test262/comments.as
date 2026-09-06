#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: comments.as
//? customPurpose: Part of test262
//? description: comments
//? sourceType: script
//?
//? test: S7.4_A1_T1
//? description: Create comments with any code
//? expect: success
//? source: ...

//CHECK#1
// throw "#1: Correct interpretation of single line comments"

//CHECK#2
let x = 0;
assert(x === 0, "the value of 'x' is 0");

//CHECK#3
let // y = 1;
y;
assert(y === undefined, "The value of 'y' is expected to equal undefined");

//CHECK#4
//throw "#4: Correct interpretation of single line comments") // throw "#4: Correct interpretation of single line comments"; //

////CHECK#5
//let x = 1;
//if (x === 1) {
//  throw "#5: Correct interpretation of single line comments";
//}
//? test: S7.4_A1_T2
//? description: Simple test, create empty comment: ///
//? expect: success
//? source: ...

//CHECK#1
///
//? test: S7.4_A2_T1
//? description: Create comments with any code
//? expect: success
//? source: ...

/*CHECK#1*/
/* throw "#1: Correct interpretation of multi line comments"; 
*/

/*CHECK#2*/
let x = 0;
/* x = 1;*/
assert(x === 0, "the value of 'x' is 0");

//CHECK#3
let /* y = 1;*/
y;
assert(y === undefined, "The value of 'y' is expected to equal undefined");

//CHECK#4
let /* y2 = 1;*/ y2;
assert(y2 === undefined, "The value of 'y2' is expected to equal undefined");

/*CHECK#5*/
/*let x2 = 1;
if (x2 === 1) {
  throw "#5: Correct interpretation of multi line comments";
}*/

/*CHECK#6*/

//CHECK#7
let string = "/*var y = 0*/"; /* y = 1;*/
assert(string === "/*var y = 0*/", "The value of 'string' is expected to equal '/*var y = 0*/'");

//CHECK#8
let string2 = "/*var y = 0"; /* y = 1;*/
assert(string2 === "/*var y = 0", "The value of 'string2' is expected to equal '/*var y = 0*/'");

/*CHECK#9*/
/** throw "#9: Correct interpretation of multi line comments"; 
*/

/*CHECK#10*/
/* throw "#10: Correct interpretation of multi line comments";
**/

/*CHECK#11*/
/****** throw '#11: Correct interpretation multi line comments';*********
***********
*
**********
**/
//? test: S7.4_A2_T2
//? description: Try use /*CHECK#1/. This is not closed multi line comment
//? expect: error
//? source: ...

/*CHECK#1/


//? test: S7.4_A3
//? description: Try use nested comments
//? expect: error
//? source: ...

/*CHECK#1*/

/*
let
/* x */
= 1;
*/


//? test: S7.4_A4_T1
//? description: Try use 2 close comment tags
//? expect: error
//? source: ...

/*CHECK#1*/

/* let*/
x*/


//? test: S7.4_A4_T2
//? description: Fist Multi line comment, then Single line comment
//? expect: success
//? source: ...

/*CHECK#1*/

/* let
*///x*/
//? test: S7.4_A4_T3
//? description: Insert Single line comment into Multi line comment
//? expect: success
//? source: ...

/*CHECK#1*/

/* let
//x1
*/
//? test: S7.4_A4_T4
//? description: Try to open Multi line comment at the end of Single comment
//? expect: error
//? source: ...

/*CHECK#1*/

// let /*
x*/


//? test: S7.4_A4_T5
//? description: Insert Multi line comment into Single line comment
//? expect: success
//? source: ...

/*CHECK#1*/

// let /* x */
//? test: S7.4_A4_T6
//? description: Insert Multi line comment with two closed tags into Single line comment
//? expect: success
//? source: ...

/*CHECK#1*/

// let /* x / = */ 1 */
//? test: S7.4_A4_T7
//? description: Insert Multi line comment into Single line comments
//? expect: success
//? source: ...

/*CHECK#1*/

// let /*
// x
// =
// 1*/
//? test: S7.4_A5_beyond_bmp
//? description: UTF-8 supplementary code point in a // comment
//? differences: Adaptive Script stores UTF-8. BMP via ES \\uXXXX is comments-bmp-slash-*.as.
//? expect: success
//? source: ...

let e = eval<script>(script("let yy = 0;\n//let " + "😀" + "yy = -1;"));
assert(e === undefined, "supplementary UTF-8 stays in // comment");
//? test: S7.4_A6_beyond_bmp
//? description: UTF-8 supplementary code point inside /* */
//? differences: Adaptive Script stores UTF-8. BMP via ES \\uXXXX is comments-bmp-block-*.as.
//? expect: success
//? source: ...

let y = eval(script("/*let " + "😀" + "y = 1*/"));
assert(y === undefined, "supplementary UTF-8 stays in block comment");
