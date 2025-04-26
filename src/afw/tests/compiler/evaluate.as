#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: evaluate.as
//? customPurpose: Part of compiler category tests
//? description: Test evaluate function
//? sourceType: script
//?
//? test: evaluate-no-params
//? description: Test evaluate without any parameters
//? expect: error
//? source: ...

evaluate();