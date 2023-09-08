#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: evaluate_with_retry.as
//? customPurpose: Part of compiler category tests
//? description: Test evaluate_with_retry function
//? sourceType: script
//?
//? test: evaluate_with_retry-no-params
//? description: Test evaluate_with_retry without any parameters
//? expect: error
//? source: ...

evaluate_with_retry();