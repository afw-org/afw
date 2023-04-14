#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: test_template.as
//? customPurpose: Part of compiler category tests
//? description: Test test_template function
//? sourceType: script
//?
//? test: test_template-no-params
//? description: Test test_template without any parameters
//? expect: error
//? source: ...
#!/usr/bin/env afw

test_template();