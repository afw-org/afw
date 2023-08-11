#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: concatenation.as
//? customPurpose: Part of test262
//? description: expressions/concatenation
//? sourceType: script
//?
//? test: S9.8_A1_T2
//? description: result of string conversion from undefined value is "<undefined>"
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

// When using "+" operator which calls polymorphic function "add", the first
// parameter must be integer, double, or string. Because of this, "string"
// must be used to convert undefined to a string.

if (concat(string(undefined) + "") !== "<undefined>") {
    throw "undefined + \"\" === \"<undefined>\"";
}



//? test: S9.8_A2_T2
//? description: result of string conversion from null value is "null"
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

if (concat(null, "") !== "null") {
    throw "null + \"\" === \"null\"";
}



//? test: S9.8_A3_T2
//? description: result of string conversion from boolean value is "true" if the argument is "true", else is "false"
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

if (concat(false, "") !== "false") {
    throw "false + \"\" === \"false\"";
}

if (concat(true, "") !== "true") {
    throw "true + \"\" === \"true\"";
}



//? test: S9.8_A4_T2
//? description: result of string conversion from string value is input argument (no conversion)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

let x1: string = "abc";
if (concat(x1, "") !== x1) {
    throw "x1 + \"\" === \"abc\"";
}

let x2: string = "abc";
if (concat(meta(x2).dataType, "") !== meta(x2).dataType) {
    throw "meta(x2).dataType + \"\" !== meta(x2).dataType";
}

