#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: null.as
//? customPurpose: Part of test262
//? description: literals/null
//? sourceType: script
//?
//? test: S7.8.1_A1_T1
//? description: Check null === null
//? expect: undefined
//? source: ...


//CHECK#1
if (null !== null) {
  throw '#1: null === null';
}
