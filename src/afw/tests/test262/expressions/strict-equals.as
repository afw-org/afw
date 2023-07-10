#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: strict-equals.as
//? customPurpose: Part of test262
//? description: Tests strict-equals expressions
//? sourceType: script
//?
//? test: 11.9.4_A1
//? description: White Space and Line Terminator between EqualityExpression and "===" or between "===" and RelationalExpression are allowed
//? expect: 0
//? source: ...
#!/usr/bin/env afw

if (!(evaluate(script("1\u0009===\u00091")))) {
    throw "#1: 1\\u0009===\\u00091";
}

if (!(evaluate(script("1\u000B===\u000B1")))) {
    throw "#2: 1\\u000B===\\u000B1";
}

if (!(evaluate(script("1\u000C===\u000C1")))) {
    throw "#3: 1\\u000C===\\u000C1";
}

if (!(evaluate(script("1\u0020===\u00201")))) {
    throw "#4: 1\\u0020===\\u00201";
}

if (!(evaluate(script("1\u00A0===\u00A01")))) {
    throw "#5: 1\\u00A0===\\u00A01";
}

if (!(evaluate(script("1\u000A===\u000A1")))) {
    throw "#6: 1\\u000A===\\u000A1";
}

if (!(evaluate(script("1\u000D===\u000D1")))) {
    throw "#7: 1\\u000D===\\u000D1";
}

if (!(evaluate(script("1\u2028===\u20281")))) {
    throw "#8: 1\\u2028===\\u20281";
}

if (!(evaluate(script("1\u2029===\u20291")))) {
    throw "#9: 1\\u2029===\\u20291";
}

if (!(evaluate(script("1\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u2029===\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u20291")))) {
    throw "#10: 1\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u20291";
}

return 0;