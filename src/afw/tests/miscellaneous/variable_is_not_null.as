#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: variable_is_not_null.as
//? customPurpose: Part of miscellaneous category tests
//? description: Test miscellaneous variable_is_not_null function
//? sourceType: script
//?
//? test: variable_is_not_null-true
//? description: Test variable_is_not_null when it does
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let result: string = "x";

assert(variable_is_not_null("result") === true);

return 0;

//? test: variable_is_not_null-false
//? description: Test variable_is_not_null when it does not
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

assert(variable_is_not_null("result") === false);

return 0;