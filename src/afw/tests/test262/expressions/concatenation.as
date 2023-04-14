#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: concatenation.as
//? customPurpose: Part of test262
//? description: expressions/concatenation
//? sourceType: script
//?
//? test: S9.8_A1_T2
//? description: result of string conversion from undefined value is "<undefined>"
//? expect: null
//? source: ...
#!/usr/bin/env afw

// When using "+" operator which calls polymorphic function "add", the first
// parameter must be integer, double, or string. Because of this, "string"
// must be used to convert undefined to a string.

if (concat(string(undefined) + "") !== "<undefined>") {
    assert(false, "undefined + \"\" === \"<undefined>\"");
}



//? test: S9.8_A2_T2
//? description: result of string conversion from null value is "null"
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (concat(null, "") !== "null") {
    assert(false, "null + \"\" === \"null\"");
}



//? test: S9.8_A3_T2
//? description: result of string conversion from boolean value is "true" if the argument is "true", else is "false"
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (concat(false, "") !== "false") {
    assert(false, "false + \"\" === \"false\"");
}

if (concat(true, "") !== "true") {
    assert(false, "true + \"\" === \"true\"");
}



//? test: S9.8_A4_T2
//? description: result of string conversion from string value is input argument (no conversion)
//? expect: null
//? source: ...
#!/usr/bin/env afw

loc x1: string = "abc";
if (concat(x1, "") !== x1) {
    assert(false, "x1 + \"\" === \"abc\"");
}

loc x2: string = "abc";
if (concat(meta(x2).dataType, "") !== meta(x2).dataType) {
    assert(false, "meta(x2).dataType + \"\" !== meta(x2).dataType");
}

