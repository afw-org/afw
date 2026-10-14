#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/regexp_escape.as
//? customPurpose: Part of core function tests
//? description: Tests for regexp_escape: text that matches itself in a regexp.
//? sourceType: script
//?
//? test: regexp_escape-special
//? description: Every special character matches itself.
//? expect: true
//? source: ...

let s = "^$\\.*+?()[]{}|/-,=<>#&!%:;@~'`\"";
return to_regexp("^" + regexp_escape(s) + "\\z")->test(s);

//?
//? test: regexp_escape-kept
//? description: Letters, digits after the first, _ and characters above ASCII are kept.
//? expect: "a1_Äé"
//? source: ...

regexp_escape("a1_Äé")

//?
//? test: regexp_escape-hex
//? description: A leading digit, white space and controls are written as \x{hh}.
//? expect: "\\x{31}2\\x{20}\\x{a}\\x{9}"
//? source: ...

regexp_escape("12 \n\t")

//?
//? test: regexp_escape-not-a-pattern
//? description: The escaped text is literal, not a pattern.
//? expect: [true, false]
//? source: ...

let re = to_regexp("^" + regexp_escape("a.b") + "\\z");
return [re->test("a.b"), re->test("axb")];

//?
//? test: regexp_escape-flag-x
//? description: White space and # stay literal with flag x.
//? expect: [true, false]
//? source: ...

let re = to_regexp("(?x)^" + regexp_escape("a b#c") + "\\z");
return [re->test("a b#c"), re->test("ab")];

//?
//? test: regexp_escape-after-back-reference
//? description: A leading digit does not continue a back reference.
//? expect: [true, false]
//? source: ...

let re = to_regexp("^(a)\\1" + regexp_escape("1") + "\\z");
return [re->test("aa1"), re->test("a")];

//?
//? test: regexp_escape-empty
//? description: An empty string escapes to an empty string.
//? expect: ""
//? source: ...

regexp_escape("")
