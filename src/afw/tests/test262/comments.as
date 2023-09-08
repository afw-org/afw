#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: comments.as
//? customPurpose: Part of test262
//? description: comments
//? sourceType: script
//?
//? test: S7.4_A1_T1
//? description: Create comments with any code
//? expect: undefined
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
//? expect: undefined
//? source: ...

//CHECK#1
///


//? test: S7.4_A2_T1
//? description: Create comments with any code
//? expect: undefined
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
//? expect: undefined
//? source: ...

/*CHECK#1*/

/* let
*///x*/


//? test: S7.4_A4_T3
//? description: Insert Single line comment into Multi line comment
//? expect: undefined
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
//? expect: undefined
//? source: ...

/*CHECK#1*/

// let /* x */


//? test: S7.4_A4_T6
//? description: Insert Multi line comment with two closed tags into Single line comment
//? expect: undefined
//? source: ...

/*CHECK#1*/

// let /* x / = */ 1 */

//? test: S7.4_A4_T7
//? description: Insert Multi line comment into Single line comments
//? expect: undefined
//? source: ...

/*CHECK#1*/

// let /*
// x
// =
// 1*/

//? test: S7.4_A5
//? description: //var " + xx + "yy = -1", insert instead of xx all Unicode characters
//? expect: undefined
//? source: ...

let hex = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"];
for (let i1 = 0; i1 < 14; i1 = i1 + 1) {
    for (let i2 = 0; i2 < 8; i2 = i2 + 1) {
        for (let i3 = 0; i3 < 16; i3 = i3 + 1) {
            for (let i4 = 0; i4 < 16; i4 = i4 + 1) {                
                let uu = hex[i1] + hex[i2] + hex[i3] + hex[i4];                
                let xx = evaluate<script>(script("\"\\u" + uu + "\""));
                let LineTerminators = ((uu === "000A") || (uu === "000D") || (uu === "2028") || (uu === "2029"));                
                
                let strToEval = script("let yy = 0;\n//let " + xx + "yy = -1;");                  
                let e = evaluate<script>(strToEval);                
                if (LineTerminators) {
                    assert(e === -1, "The value of 'e' is expected to equal -1");
                } else {
                    assert(e === 0, "The value of 'e' is expected to equal 0");
                }                
            }
        }
    }
}


//? test: S7.4_A6
//? description: "\"var\"+ yy+ \"xx = 1\", insert instead of yy all Unicode characters"
//? expect: undefined
//? source: ...

let hex = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"];
for (let i1 = 0; i1 < 14; i1 = i1 + 1) {
    for (let i2 = 0; i2 < 8; i2 = i2 + 1) {
        for (let i3 = 0; i3 < 16; i3 = i3 + 1) {
            for (let i4 = 0; i4 < 16; i4 = i4 + 1) {                
                let uu = hex[i1] + hex[i2] + hex[i3] + hex[i4];                
                let xx = evaluate<script>(script("\"\\u" + uu + "\""));

                let y = evaluate(script("/*let " + xx + "y = 1*/"));
                if (y !== undefined) {
                    throw "Expected y to be undefined";
                }
            }
        }
    }
}
