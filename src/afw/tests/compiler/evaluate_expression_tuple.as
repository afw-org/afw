#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: evaluate_expression_tuple.as
//? customPurpose: Part of compiler category tests
//? description: Test evaluate_expression_tuple function
//? sourceType: script
//?
//? test: evaluate_expression_tuple-no-params
//? description: Test evaluate_expression_tuple without any parameters
//? expect: error
//? source: ...
#!/usr/bin/env afw

evaluate_expression_tuple();