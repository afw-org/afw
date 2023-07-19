#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: variables.as
//? customPurpose: Part of afw core tests
//? description: Test variable functions 
//? sourceType: script
//?
//? test: variable_exists
//? description: Test variable_exists()
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const myvar: string = "abc";

assert(variable_exists("myvar"), "Could not find variable!");
assert(variable_exists("myvar2") === false, "Found variable that does not exist!");

return 0;

//?
//? test: variable_get
//? description: Test variable_get()
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const myvar: string = "def";

assert(variable_get("myvar") === "def", "variabl_get() returned incorrect value!");

return 0;

//?
//? test: variable_is_not_null
//? description: Test variable_is_not_null()
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const myvar: string = "def";
let myvar2: any;

assert(variable_is_not_null("myvar"), "variable_is_not_null() failed!");
assert(variable_is_not_null("myvar2") === false, "variable_is_not_null() failed for null variable!");

return 0;