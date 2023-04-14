#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: variable_exists.as
//? customPurpose: Part of miscellaneous category tests
//? description: Test miscellaneous variable_exists function
//? sourceType: script
//?
//? test: variable_exists-true
//? description: Test variable_exists when it does
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc result: string = "x";

assert(variable_exists("result") === true);

return 0;

//? test: variable_exists-false
//? description: Test variable_exists when it does not
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

assert(variable_exists("result") === false);

return 0;