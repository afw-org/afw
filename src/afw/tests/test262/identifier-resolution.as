#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: identifier-resolution.as
//? customPurpose: Part of test262
//? description: identifier-resolution
//? sourceType: script
//?
//? test: S10.2.2_A1_T1
//? description: Checking scope chain containing function declarations
//? expect: undefined
//? source: ...


let x = 0;

function f1(){
  let x = 1;
  function f2(){
    return x;
  };
  return f2();
}

if(!(f1() === 1)){
  throw "#1: Scope chain disturbed";
}


//? test: S10.2.2_A1_T2
//? description: Checking scope chain containing function declarations
//? expect: undefined
//? source: ...


let x = 0;

function f1(){
  function f2(){
    return x;
  };
  return f2();
}

if(!(f1() === 0)){
  throw "#1: Scope chain disturbed";
}


//? test: S10.2.2_A1_T4
//? description: Checking scope chain containing function declarations
//? expect: error:#1: Scope chain disturbed
//? source: ...

/* This test originally used 'var' which is not supported by afw 
   Furthermore, ECMAScript "hoists" variables to the top of the 
   scope when not in "use strict" mode, which we also do not support.
   Therefore, it should fail.
*/
let x = 0;

function f1(){

  function f2(){
    return x;
  };

  let x = 1;
  return f2();
}

if(!(f1() === 1)){
  throw "#1: Scope chain disturbed";
}

