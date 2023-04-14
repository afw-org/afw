#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: variable_get.as
//? customPurpose: Part of miscellaneous category tests
//? description: Test miscellaneous variable_get function
//? sourceType: script
//?
//? test: variable_get
//? description: Test variable_get
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc result: string = "x";

assert(variable_get("result") === "x");

return 0;
