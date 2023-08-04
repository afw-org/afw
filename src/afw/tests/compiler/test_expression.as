#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: test_expression.as
//? customPurpose: Part of compiler category tests
//? description: Test test_expression function
//? sourceType: script
//?
//? test: test_expression-no-params
//? description: Test test_expression without any parameters
//? expect: error
//? source: ...
#!/usr/bin/env afw

test_expression();