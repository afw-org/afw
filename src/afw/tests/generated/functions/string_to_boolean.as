#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_to_boolean.as
//? customPurpose: Part of core function tests
//? description: Tests for the string_to_boolean function (#588).
//? sourceType: script
//?
//? test: string_to_boolean-true
//? description: true text in any case, 1, t, and T are true
//? expect: 0
//? source: ...

for (const s of ["true", "TRUE", "True", "tRuE", "1", "t", "T"]) {
    assert(string_to_boolean(s) === true, s);
}
return 0;

//?
//? test: string_to_boolean-false
//? description: false text in any case, 0, f, and F are false
//? expect: 0
//? source: ...

for (const s of ["false", "FALSE", "False", "fAlSe", "0", "f", "F"]) {
    assert(string_to_boolean(s) === false, s);
}
return 0;

//?
//? test: string_to_boolean-bad-text
//? description: any other text is an error, including a single unknown character and the empty string
//? expect: 0
//? source: ...

for (const s of ["", "x", "2", " ", "y", "n", "maybe", "yes", "no",
    "tru", "fals", " true", "true ", "truee", "falsey", "10"])
{
    let message = "returned";
    try {
        string_to_boolean(s);
    }
    catch (e) {
        assert(e.id === "conversion_error", s + " " + e.id);
        message = e.message;
    }
    assert(message === "Not a valid boolean value '" + s + "'.",
        s + ": " + message);
}
return 0;

//?
//? test: string_to_boolean-not-truthiness
//? description: unlike boolean(), which is ECMAScript Boolean() truthiness
//? expect: [true, false, true, false]
//? source: ...

[boolean("false"), string_to_boolean("false"),
    boolean("0"), string_to_boolean("0")]

//?
//? test: string_to_boolean-string-only
//? description: the parameter is a string
//? expect: error:Parameter 1 of function 'string_to_boolean' must evaluate to data type 'string' but evaluated to be 'integer'; write to_string(v)
//? source: ...

string_to_boolean(1)

//?
//? test: string_to_boolean-to_boolean-gone
//? description: to_boolean was renamed string_to_boolean
//? expect: error:Parse error at offset 1 around line 2 column 1: Unknown built-in function 'to_boolean'
//? source: ...

to_boolean("true")
