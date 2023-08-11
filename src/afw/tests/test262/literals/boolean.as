#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: boolean.as
//? customPurpose: Part of test262
//? description: literals/boolean
//? sourceType: script
//?
//? test: S7.8.2_A1_T1
//? description: "BooleanLiteral :: true"
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (true !== true) {
  throw '#1: Boolean(true) === true. Actual: Boolean(true) === ' + (string(true));
}


//? test: S7.8.2_A1_T2
//? description: "BooleanLiteral :: false"
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (false !== false) {
  throw '#1: Boolean(false) === false. Actual: Boolean(false) === ' + (string(false));
}