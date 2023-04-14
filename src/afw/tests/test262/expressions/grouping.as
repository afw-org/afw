#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: grouping.as
//? customPurpose: Part of test262
//? description: expressions/grouping
//? sourceType: script
//?
//? test: 11.1.6_A1
//? description: White Space and Line Terminator inside "grouping" operator are allowed
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (evaluate(script("(\u00091\u0009)")) != 1) {
    assert("#1: (\\u00091\\u0009) === 1");
}

if (evaluate(script("(\u000B1\u000B)")) != 1) {
    assert("#2: (\\u000B1\\u000B) === 1");
}

if (evaluate(script("(\u000C1\u000C)")) != 1) {
    assert("#3: (\\u000C1\\u000C) === 1");
}

if (evaluate(script("(\u00201\u0020)")) != 1) {
    assert("#4: (\\u00201\\u0020) === 1");
}

if (evaluate(script("(\u00A01\u00A0)")) != 1) {
    assert("#5: (\\u00A01\\u00A0) === 1");
}

if (evaluate(script("(\u000A1\u000A)")) != 1) {
    assert("#6: (\\u000A1\\u000A) === 1");
}

if (evaluate(script("(\u000D1\u000D)")) != 1) {
    assert("#7: (\\u000D1\\u000D) === 1");
}

if (evaluate(script("(\u20281\u2028)")) != 1) {
    assert("#8: (\\u20281\\u2028) === 1");
}

if (evaluate(script("(\u20291\u2029)")) != 1) {
    assert("#9: (\\u20291\\u2029) === 1");
}

if (evaluate(script("(\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u20291\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u2029)")) != 1) {
    assert("#10: (\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u2029) === 1");
}



//? test:  11.1.6_A3_T1
//? description: Applying grouping operator to boolean
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((true) !== true) {
    assert("#1: (true) === true");
}



//? test:  11.1.6_A3_T2
//? description: Applying grouping operator to integer
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((1) !== 1) {
    assert("#1: (1) === 1");
}



//? test:  11.1.6_A3_T3
//? description: Applying grouping operator to string
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (("1") !== "1") {
    assert("#1: (\"1\") === \"1\"");
}

if (("x") !== "x") {
    assert("#2: (\"x\") === \"x\"");
}



//? test:  11.1.6_A3_T4
//? description: Applying grouping operator to undefined/null
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((undefined) !== undefined) {
    assert("#1: (undefined) === undefined");
}

if ((null) !== null) {
    assert("#2: (null) === null");
}

