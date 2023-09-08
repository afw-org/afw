#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: safe_evaluate.as
//? customPurpose: Part of compiler category tests
//? description: Test safe_evaluate function
//? sourceType: script
//?
//? test: safe_evaluate-no-params
//? description: Test safe_evaluate without any parameters
//? expect: error
//? source: ...

safe_evaluate();