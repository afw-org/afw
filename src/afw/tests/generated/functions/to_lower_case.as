#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/to_lower_case.as
//? customPurpose: Part of core function tests
//? description: Tests for to_lower_case: Unicode full case mapping, the same in every locale.
//? sourceType: script
//?
//? test: to_lower_case-ascii
//? description: ASCII letters.
//? expect: "abc1"
//? source: ...

"AbC1"->to_lower_case()

//?
//? test: to_lower_case-final-sigma
//? description: A final capital sigma becomes ς, as in ECMAScript.
//? expect: "σας"
//? source: ...

to_lower_case("ΣΑΣ")

//?
//? test: to_lower_case-dotted-i
//? description: İ becomes i followed by a combining dot, in every locale.
//? expect: "i\u0307"
//? source: ...

to_lower_case("İ")
