#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/trim.as
//? customPurpose: Part of core function tests
//? description: Tests for trim, trim_start and trim_end: ECMAScript's white space and line terminators.
//? sourceType: script
//?
//? test: trim-both
//? description: Both ends; inner white space is kept.
//? expect: "a  b"
//? source: ...

"  a  b \n"->trim()

//?
//? test: trim-unicode
//? description: No-break space, U+FEFF, ideographic space and U+2028 are white space.
//? expect: "a"
//? source: ...

"\u00A0\uFEFF\u3000a\u2028"->trim()

//?
//? test: trim-start
//? description: trim_start keeps the end.
//? expect: "a  "
//? source: ...

"  a  "->trim_start()

//?
//? test: trim-end
//? description: trim_end keeps the start.
//? expect: "  a"
//? source: ...

"  a  "->trim_end()

//?
//? test: trim-all
//? description: Only white space gives the empty string.
//? expect: ""
//? source: ...

" \t\r\n "->trim()

//?
//? test: trim-none
//? description: Nothing to trim.
//? expect: "a b"
//? source: ...

trim("a b")

//?
//? test: trim-zero-width
//? description: A zero width space (U+200B) is not white space, as in ECMAScript.
//? expect: "\u200Ba"
//? source: ...

"\u200Ba "->trim()

//?
//? test: trim-anyURI
//? description: An anyURI gives a string.
//? expect: "http://a"
//? source: ...

trim(" http://a "anyURI)
//?
//? test: anyURI-string-functions
//? description: the new string functions take an anyURI and give a string
//? expect: 0
//? source: ...

const u = " http://Example.COM/a-b "anyURI;
const t = u->trim();
assert(is_string(t) && t === "http://Example.COM/a-b", "trim");
assert(is_string(u->trim_start()) && u->trim_start() === "http://Example.COM/a-b ", "trim_start");
assert(u->trim_end() === " http://Example.COM/a-b", "trim_end");
assert(to_lower_case(u) === " http://example.com/a-b ", "to_lower_case");
assert(to_upper_case(u) === " HTTP://EXAMPLE.COM/A-B ", "to_upper_case");
assert(pad_start("a:b"anyURI, 5, "x") === "xxa:b" && pad_end("a:b"anyURI, 4) === "a:b ", "pad");
assert(replace_all("a-b-c"anyURI, "-", "/") === "a/b/c", "replace_all");
assert(code_point_at("a:b"anyURI, 1) === 58, "code_point_at");
assert(is_string(normalize("a:b"anyURI)) && normalize("a:b"anyURI) === "a:b", "normalize");
return 0;
