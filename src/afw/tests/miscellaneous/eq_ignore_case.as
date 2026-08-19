#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: eq_ignore_case.as
//? customPurpose: Part of miscellaneous category tests
//? description: eq_ignore_case mixed-width UTF-8 (issue #206 leftover)
//? sourceType: script
//?
//? test: ascii
//? description: ASCII ignore-case still matches
//? expect: 0
//? source: ...

assert(eq_ignore_case("AbC", "aBc") === true);
assert(eq_ignore_case("abc", "abd") === false);
return 0;

//?
//? test: dotted-i-mixed-width
//? description: iX vs İX — U+0130 is two bytes, i is one (issue #206)
//? expect: 0
//? source: ...

assert(eq_ignore_case("i", "\u0130") === true);
assert(eq_ignore_case("iX", "\u0130X") === true);
assert(eq_ignore_case("\u0130X", "iX") === true);
return 0;

//?
//? test: a-vs-agrave
//? description: A vs À are not ignore-case equal
//? expect: 0
//? source: ...

assert(eq_ignore_case("A", "\u00c0") === false);
return 0;
