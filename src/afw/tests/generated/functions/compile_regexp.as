#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/compile_regexp.as
//? customPurpose: Part of core function tests
//? description: Tests for compile<regexp>: a value of data type regexp that holds its compiled form.
//? sourceType: script
//?
//? test: compile_regexp-data-type
//? description: The result is a regexp value.
//? expect: "regexp"
//? source: ...

meta(compile<regexp>("a+b"re)).dataType

//?
//? test: compile_regexp-equal-source
//? description: A compiled regexp equals its source.
//? expect: true
//? source: ...

compile<regexp>("a+b"re) == "a+b"re

//?
//? test: compile_regexp-bad-pattern
//? description: A pattern that does not compile is a syntax error.
//? expect: error:Invalid regular expression: U_REGEX_MISMATCHED_PAREN at line 1 offset 2
//? source: ...

compile<regexp>("(a"re)

//?
//? test: compile_regexp-listing
//? description: The compiler listing shows the source.
//? expect: "\n\n#\n#  Adaptive Value is evaluated at compile time\n#\n\"a+b\"regexp\n"
//? source: ...

compile<regexp>("a+b"re, true)

//?
//? test: compile_regexp-stored
//? description: Stored in a variable, an array, and an object, it stays a regexp equal to its source, and outlives the scope it was compiled in.
//? expect: [true, true, true, true]
//? source: ...

function make(source: regexp): object {
    const r = compile<regexp>(source);
    return { r: r, a: [r] };
}

let kept: object;
for (const s of ["x+y"re, "a+b"re]) {
    kept = make(s);
}
const r2 = kept.r;
return [
    kept.r == "a+b"re,
    kept.a[0] == "a+b"re,
    r2 === kept.r,
    meta(r2).dataType === "regexp"
];
