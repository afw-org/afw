#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: assert.as
//? customPurpose: Part of compiler category tests
//? description: Test assert function
//? sourceType: script
//?
//? test: assert-no-params
//? description: Test assert without any parameters
//? expect: error
//? source: ...

assert();

//? test: assert-true
//? description: Test assert with true
//? expect: 0
//? source: ...

assert(true);

return 0;

//? test: assert-false
//? description: Test assert with false
//? expect: error:Assertion failed
//? source: ...

assert(false);

return 0;