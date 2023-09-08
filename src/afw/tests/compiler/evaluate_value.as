#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: evaluate_value.as
//? customPurpose: Part of compiler category tests
//? description: Test evaluate_value function
//? sourceType: script
//?
//? test: evaluate_value-no-params
//? description: Test evaluate_value without any parameters
//? expect: error
//? source: ...

evaluate_value();