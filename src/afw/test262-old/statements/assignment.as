#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: assignment.as
//? customPurpose: Part of test262
//? description: Tests assignments
//? sourceType: script
//?
//? test: assignment-1
//? description: tests for referencing undeclared variable in assignment
//? expect: error
//? source: ...
#!/usr/bin/env afw

x = 42;

return 0;