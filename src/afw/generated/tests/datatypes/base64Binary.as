#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: datatypes/base64Binary.as
//? customPurpose: Part of core dataType tests
//? description: Tests for base64Binary dataType.
//? sourceType: expression
//?
//? test: base64Binary-1
//? description: Test 1 for base64Binary dataType.
//? expect: boolean(true)
//? source: ...

eq<base64Binary>(
    base64Binary("0FB8 0F+9"),
    base64Binary("0FB8 0F+9")
)

//?
//? test: base64Binary-2
//? description: Test 2 for base64Binary dataType.
//? expect: boolean(true)
//? source: ...

eq<base64Binary>(
    base64Binary("0FB8"),
    base64Binary("0FB8")
)

//?
//? test: base64Binary-3
//? description: Test 3 for base64Binary dataType.
//? expect: boolean(true)
//? source: ...

eq<base64Binary>(
    base64Binary("0fb8"),
    base64Binary("0fb8")
)

//?
//? test: base64Binary-4
//? description: Test 4 for base64Binary dataType.
//? expect: boolean(true)
//? source: ...

eq<base64Binary>(
    base64Binary("0FB80F+9"),
    base64Binary("0FB80F+9")
)

//?
//? test: base64Binary-5
//? description: Test 5 for base64Binary dataType.
//? expect: boolean(true)
//? source: ...

eq<base64Binary>(
    base64Binary("0F+40A=="),
    base64Binary("0F+40A==")
)

//?
//? test: base64Binary-6
//? description: Test 6 for base64Binary dataType.
//? expect: boolean(true)
//? source: ...

eq<base64Binary>(
    base64Binary(""),
    base64Binary("")
)

