#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: stringify.as
//? customPurpose: Part of compiler category tests
//? description: Test compiler stringify function
//? sourceType: script
//?
//? test: stringify-boolean
//? description: Test stringify of booleans
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc result: string;

result = stringify(true);
assert(result == "true");

result = stringify(false);
assert(result == "false");

return 0;

//? test: stringify-integer
//? description: Test stringify of integers
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc result: string;

result = stringify(0);
assert(result == "0");

result = stringify(-1);
assert(result == "-1");

result = stringify(1);
assert(result == "1");

result = stringify(99999999);
assert(result == "99999999");

return 0;

//? test: stringify-string
//? description: Test stringify of strings
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc result: string;

result = stringify("abc");
assert(result == "\"abc\"");

result = stringify("");
assert(result == "\"\"");

return 0;

//? test: stringify-list
//? description: Test stringify of strings
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc result: string;

result = stringify([]);
assert(result == "[]");

result = stringify(["abc"]);
assert(result == "[\"abc\"]");

result = stringify(["abc", "def"]);
assert(result == "[\"abc\",\"def\"]");

return 0;

//? test: stringify-object
//? description: Test stringify of strings
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc obj: object;
loc result: string;

obj = {
    "a": "b",
    "c": 1,
    "d": true
};

result = stringify(obj);
assert(result == "{\"a\":\"b\",\"c\":1,\"d\":true}");

return 0;

//? test: stringify-whitespace
//? description: Test stringify of strings
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc obj: object;
loc result: string;

obj = {
    "a": "b",
    "c": 1,
    "d": true
};

result = stringify(obj,,"    ");
assert(result == "{\n    \"a\": \"b\",\n    \"c\": 1,\n    \"d\": true\n}");

return 0;