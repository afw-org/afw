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
//? test: compile_regexp-bad-literal
//? description: A regexp literal is compiled with the script, so a pattern that does not compile is a parse error at the literal.
//? expect: error:Parse error at offset 1 around line 2 column 1: Invalid regexp typed literal: U_REGEX_MISMATCHED_PAREN at line 1 offset 2
//? source: ...

"(a"re

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

//?
//? test: compile_regexp-literal-is-compiled
//? description: A regexp literal is already a compiled regexp; compile<regexp> of it is equal, and it can be returned and stored like any value.
//? expect: [true, true, "regexp"]
//? source: ...

function f(): regexp {
    return "a+b"re;
}
const kept = { r: f() };
return [
    compile<regexp>("a+b"re) == "a+b"re,
    kept.r == "a+b"re,
    meta(kept.r).dataType
];

//?
//? test: compile_regexp-used-by-regexp_match
//? description: regexp_match takes a literal, a to_regexp result, a compile<regexp> result, and a stored copy reused in a loop; each finds the pattern anywhere unless ^ and $ anchor it.
//? expect: [true, false, true, true, 3]
//? source: ...

const stored = to_regexp("^[0-9]+$");
let count: integer = 0;
for (const s of ["1", "22", "x", "333"]) {
    if (regexp_match(s, stored)) {
        count = count + 1;
    }
}
return [
    regexp_match("xaab", "a+b"re),
    regexp_match("xaab", "^a+b$"re),
    regexp_match("ÄÖx", to_regexp("^\\p{Lu}+x$")),
    regexp_match("abc", compile<regexp>(to_regexp("b"))),
    count
];

//?
//? test: to_regexp-bad-pattern
//? description: to_regexp of a pattern that does not compile is a conversion error.
//? expect: error:Invalid regular expression: U_REGEX_MISMATCHED_PAREN at line 1 offset 2
//? source: ...

to_regexp("(a")

//?
//? test: regexp-type-functions
//? description: is_regexp, as_regexp, to_string, and array<regexp> take regexp values like the other source types.
//? expect: [true, false, true, "(?i)a", "regexp", 2]
//? source: ...

const a = array<regexp>(/a/, "b"re);
return [
    is_regexp(/a/),
    is_regexp("a"),
    as_regexp(/a/) == "a"re,
    to_string(/a/i),
    meta(a[0]).dataType,
    length(a)
];

//?
//? test: compile-polymorphic-regexp
//? description: The polymorphic compile(v) dispatches on v's data type, so a regexp value compiles like compile<regexp>; a string is not a source type.
//? expect: ["regexp", true, true, true, "\n\n#\n#  Adaptive Value is evaluated at compile time\n#\n\"(?i)a+b\"regexp\n"]
//? source: ...

const r1 = compile("a+b"re);
const r2 = compile(to_regexp("^x\\d+$"));
return [
    meta(r1).dataType,
    r1 == "a+b"re,
    regexp_match("xaab", r1),
    regexp_match("x42", r2),
    compile(/a+b/i, true)
];

//?
//? test: compile-polymorphic-string
//? description: compile of a string is an error; a pattern held in a string goes through to_regexp first.
//? expect: error:'compile' is not a method of data type 'string'
//? source: ...

compile("a+b")
