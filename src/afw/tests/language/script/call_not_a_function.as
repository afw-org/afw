#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: call_not_a_function.as
//? customPurpose: Part of core language tests
//? description: Calling a value that is not a function says what was called and what it is (#625).
//? sourceType: script
//?
//? test: call-regexp
//? description: A regexp is not callable; use test, match, or another regexp function.
//? expect: error:'re' is not a function; it is data type 'regexp'
//? source: ...

let re = /a/;
return re("a");

//?
//? test: call-undefined
//? description: Calling an undefined variable is an undefined_value error.
//? expect: ["undefined_value", "'f' is not a function; it is undefined"]
//? source: ...

let f;
try {
    f(1);
}
catch (e) {
    return [e.id, e.message];
}
return "no error";

//?
//? test: call-property
//? description: The called expression is named as written (decompiled).
//? expect: ["argument_error", "'o[\"g\"]' is not a function; it is data type 'integer'"]
//? source: ...

let o = {g: 1};
try {
    o.g(1);
}
catch (e) {
    return [e.id, e.message];
}
return "no error";
