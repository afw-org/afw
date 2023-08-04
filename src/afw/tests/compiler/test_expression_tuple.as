#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: test_expression_tuple.as
//? customPurpose: Part of compiler category tests
//? description: Test test_expression_tuple function
//? sourceType: script
//?
//? test: test_expression_tuple-no-params
//? description: Test test_expression_tuple without any parameters
//? expect: error
//? source: ...
#!/usr/bin/env afw

test_expression_tuple();