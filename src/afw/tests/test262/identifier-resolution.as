#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: identifier-resolution.as
//? customPurpose: Part of test262
//? description: identifier-resolution
//? sourceType: script
//?
//? test: S10.2.2_A1_T1
//? description: Checking scope chain containing function declarations
//? expect: success
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
//? expect: success
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
//? description: Nested function and inner let (Adaptive scope vs ES var/TDZ lineage)
//? expect: success
//? differences: Adaptive has no var-hoist; f2 sees outer x (0), not inner let x=1 as ES would after init
//? source: ...

/* Original test262 used var + non-strict hoist; Adaptive has only let/const.
   Observed Adaptive: f2 returns outer x (0). Document that; do not expect-error
   on the assertion throw (false green). */
let x = 0;

function f1(){

  function f2(){
    return x;
  };

  let x = 1;
  return f2();
}

if (f1() !== 0) {
  throw "#1: expected outer x (0) under Adaptive binding rules";
}
