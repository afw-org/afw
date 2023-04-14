#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: test_script_runtime_support.as
//? customPurpose: Part of compiler category tests
//? description: Test test_script_runtime_support function
//? sourceType: script
//?
//? test: test_script_runtime_support-no-params
//? description: Test test_script_runtime_support without any parameters
//? expect: error
//? source: ...
#!/usr/bin/env afw

test_script_runtime_support();