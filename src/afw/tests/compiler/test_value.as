#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: test_value.as
//? customPurpose: Part of compiler category tests
//? description: Test test_value function
//? sourceType: script
//?
//? test: test_value-no-params
//? description: Test test_value without any parameters
//? expect: error
//? source: ...

test_value();