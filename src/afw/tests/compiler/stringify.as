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

//?
//? test: stringify-utf8-emoji
//? description: UTF-8 multi-byte code points pass through JSON stringify (not signed-char \\uffffff..)
//? skip: false
//? expect: 0
//? source: ...

let result: string;
let party: string;

/* Literal UTF-8 in source; result must keep real code points, not per-octet \\u escapes. */
party = "🎉 Happy 🥚🥳🎊";
result = stringify(party);
assert(result === "\"🎉 Happy 🥚🥳🎊\"");

/* Same via \\u{...} escapes in Adaptive string literals. */
result = stringify("\u{1F389} egg \u{1F95A}");
assert(result === "\"🎉 egg 🥚\"");

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


//? test: stringify-pure-json-binary-date
//? description: stringify is pure JSON (not Adaptive decompile forms)
//? skip: false
//? expect: 0
//? source: ...

const b = encode_as_base64Binary("hi");
/* base64 of "hi" is aGk= — as a JSON string, not base64Binary("...") */
assert(stringify(b) == "\"aGk=\"");
assert(stringify({ "iv": b }) == "{\"iv\":\"aGk=\"}");

const d = date("2020-01-01");
assert(stringify(d) == "\"2020-01-01\"");
assert(stringify({ "d": d }) == "{\"d\":\"2020-01-01\"}");

/* Adaptive compiled form remains decompile() */
assert(decompile(b) == "base64Binary(\"aGk=\")");
assert(starts_with(decompile(d), "date("));

/* UTF-8 of octets is decode_to_string, not stringify/string */
assert(decode_to_string(b) == "hi");
assert(string(b) == "aGk=");

return 0;


//? test: stringify-null
//? description: null stringifies as JSON null
//? skip: false
//? expect: "null"
//? source: ...

return stringify(null);


//? test: stringify-replacer-function
//? description: replacer function (key, value) can omit and rewrite
//? skip: false
//? expect: 0
//? source: ...

const obj = { "a": 1, "b": 2, "c": 3 };
const only_a = stringify(obj, function (key, value) {
    if (key == "b" || key == "c") {
        return undefined;
    }
    return value;
});
assert(only_a == "{\"a\":1}");

const doubled = stringify(obj, function (key, value) {
    if (key == "a") {
        return value * 2;
    }
    if (key == "b" || key == "c") {
        return undefined;
    }
    return value;
});
assert(doubled == "{\"a\":2}");

/* Root call uses empty key */
const root = stringify(5, function (key, value) {
    assert(key == "");
    return value + 1;
});
assert(root == "6");

return 0;


//? test: stringify-replacer-property-names
//? description: array of property names keeps only those keys
//? skip: false
//? expect: 0
//? source: ...

const obj = { "a": 1, "b": 2, "c": 3 };
assert(stringify(obj, ["a", "c"]) == "{\"a\":1,\"c\":3}");
assert(stringify(obj, ["b"]) == "{\"b\":2}");

return 0;


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