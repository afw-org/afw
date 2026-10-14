#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/to_upper_case.as
//? customPurpose: Part of core function tests
//? description: Tests for to_upper_case: Unicode full case mapping, the same in every locale.
//? sourceType: script
//?
//? test: to_upper_case-ascii
//? description: ASCII letters.
//? expect: "ABC1"
//? source: ...

"abC1"->to_upper_case()

//?
//? test: to_upper_case-full
//? description: One code point can become more than one (ß becomes SS).
//? expect: "STRASSE"
//? source: ...

to_upper_case("straße")

//?
//? test: to_upper_case-unchanged
//? description: A value already in upper case is the same string.
//? expect: "ABC"
//? source: ...

"ABC"->to_upper_case()

//?
//? test: to_upper_case-anyURI
//? description: An anyURI gives a string.
//? expect: "HTTP://A.B/C"
//? source: ...

to_upper_case("http://a.b/c"anyURI)

//?
//? test: to_upper_case-integer
//? description: An integer is an error, not converted.
//? expect: error:'to_upper_case' is not a method of data type 'integer'
//? source: ...

to_upper_case(1)
