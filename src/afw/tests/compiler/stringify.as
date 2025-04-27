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

let result: string;

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

let result: string;

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

let result: string;

result = stringify("abc");
assert(result == "\"abc\"");

result = stringify("");
assert(result == "\"\"");

return 0;

//? test: stringify-array
//? description: Test stringify of strings
//? skip: false
//? expect: 0
//? source: ...

let result: string;

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

let obj: object;
let result: string;

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

let obj: object;
let result: string;

obj = {
    "a": "b",
    "c": 1,
    "d": true
};

result = stringify(obj,,"    ");
assert(result == "{\n    \"a\": \"b\",\n    \"c\": 1,\n    \"d\": true\n}");

return 0;


//? test: stringify-object-property-escape-quote
//? description: ...
Test stringify of objects, containing properties with 
names that include quotes.
The expect matching is a bit of a mess, since there's 
so many layers of escape that need to happen in this 
test!
//? skip: false
//? expect: "{\"a\\\\\\\"b\":\"c\"}"
//? source: ...

const str = "a\\\"b";

let obj = {};
obj[str] = "c";

return stringify(obj);


//? test: stringify-object-property-escape-tab
//? description: ...
Test stringify of objects, containing properties with 
names that include tabs
//? skip: false
//? expect: "{\"a\\tb\":\"c\"}"
//? source: ...

const str = "a\tb";

let obj = {};
obj[str] = "c";

return stringify(obj);


//? test: stringify-object-property-escape-nl
//? description: ...
Test stringify of objects, containing properties with 
names that include newlines
//? skip: false
//? expect: "{\"a\\nb\":\"c\"}"
//? source: ...

const str = "a\nb";

let obj = {};
obj[str] = "c";

return stringify(obj);


//? test: stringify-object-property-escape-cr
//? description: ...
Test stringify of objects, containing properties with 
names that include carriage returns
//? skip: false
//? expect: "{\"a\\rb\":\"c\"}"
//? source: ...

const str = "a\rb";

let obj = {};
obj[str] = "c";

return stringify(obj);


//? test: stringify-object-property-escape-formfeed
//? description: ...
Test stringify of objects, containing properties with 
names that include formfeeds
//? skip: false
//? expect: "{\"a\\fb\":\"c\"}"
//? source: ...

const str = "a\fb";

let obj = {};
obj[str] = "c";

return stringify(obj);


//? test: stringify-object-property-escape-bell
//? description: ...
Test stringify of objects, containing properties with 
names that include bells
//? skip: false
//? expect: "{\"a\\bb\":\"c\"}"
//? source: ...

const str = "a\u{0007}b";

let obj = {};
obj[str] = "c";

return stringify(obj);


//? test: stringify-object-property-escape-control-char
//? description: ...
Test stringify of objects, containing properties with 
names that include other control characters
//? skip: false
//? expect: "{\"a\\u0005b\":\"c\"}"
//? source: ...

const str = "a\u{0005}b";

let obj = {};
obj[str] = "c";

return stringify(obj);