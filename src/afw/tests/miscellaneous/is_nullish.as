#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: is_nullish.as
//? customPurpose: Part of miscellaneous category tests
//? description: Test miscellaneous is_nullish function
//? sourceType: script
//?
//? test: is_nullish-booleans
//? description: Test is_nullish with booleans
//? expect: 0
//? source: ...
#!/usr/bin/env afw

assert(is_nullish(true) === false);
assert(is_nullish(false) === false);

return 0;

//? test: is_nullish-integers
//? description: Test is_nullish with integers
//? expect: 0
//? source: ...
#!/usr/bin/env afw

assert(is_nullish(0) === false);
assert(is_nullish(1) === false);

return 0;

//? test: is_nullish-strings
//? description: Test is_nullish with strings
//? expect: 0
//? source: ...
#!/usr/bin/env afw

assert(is_nullish("abc") === false);
assert(is_nullish("") === false);

return 0;

//? test: is_nullish-objects
//? description: Test is_nullish with objects
//? expect: 0
//? source: ...
#!/usr/bin/env afw

assert(is_nullish({}) === false);
assert(is_nullish({"x":"y"}) === false);

return 0;

//? test: is_nullish-lists
//? description: Test is_nullish with lists
//? expect: 0
//? source: ...
#!/usr/bin/env afw

assert(is_nullish(["abc"]) === false);
assert(is_nullish(["abc","def"]) === false);
assert(is_nullish([null]) === false);
assert(is_nullish([]) === false);

return 0;

//? test: is_nullish-null
//? description: Test is_nullish with null
//? expect: 0
//? source: ...
#!/usr/bin/env afw

assert(is_nullish(null) === true);

return 0;

//? test: is_nullish-undefined
//? description: Test is_nullish with undefined
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let x: integer;

assert(is_nullish(x) === true);

return 0;