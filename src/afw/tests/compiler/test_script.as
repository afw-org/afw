#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: test_script.as
//? customPurpose: Part of compiler category tests
//? description: Test test_script function
//? sourceType: script
//?
//? test: test_script-no-params
//? description: Test test_script without any parameters
//? expect: error
//? source: ...
#!/usr/bin/env afw

test_script();