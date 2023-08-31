#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: datatypes/string.as
//? customPurpose: Part of core dataType tests
//? description: Tests for string dataType.
//? sourceType: expression
//?
//? test: string-1
//? description: Test 1 for string dataType.
//? expect: boolean(true)
//? source: ...

eq<string>(
    string("This is a string!"),
    string("This is a string!")
)

//?
//? test: string-2
//? description: Test 2 for string dataType.
//? expect: boolean(true)
//? source: ...

eq<string>(
    string("Édition française."),
    string("Édition française.")
)

//?
//? test: string-3
//? description: Test 3 for string dataType.
//? expect: boolean(true)
//? source: ...

eq<string>(
    string("これは日本の文字列です。"),
    string("これは日本の文字列です。")
)

//?
//? test: string-4
//? description: Test 4 for string dataType.
//? expect: boolean(true)
//? source: ...

eq<string>(
    string("12.5"),
    string("12.5")
)

//?
//? test: string-5
//? description: Test 5 for string dataType.
//? expect: boolean(true)
//? source: ...

eq<string>(
    string("None"),
    string("None")
)

//?
//? test: string-6
//? description: Test 6 for string dataType.
//? expect: boolean(true)
//? source: ...

eq<string>(
    string("PB&J"),
    string("PB&J")
)

//?
//? test: string-7
//? description: Test 7 for string dataType.
//? expect: boolean(true)
//? source: ...

eq<string>(
    string("Separated   by   3   spaces."),
    string("Separated   by   3   spaces.")
)

//?
//? test: string-8
//? description: Test 8 for string dataType.
//? expect: boolean(true)
//? source: ...

eq<string>(
    string("This\n\t\t\tis on two lines."),
    string("This\n\t\t\tis on two lines.")
)

