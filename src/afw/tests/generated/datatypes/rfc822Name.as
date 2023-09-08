#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: datatypes/rfc822Name.as
//? customPurpose: Part of core dataType tests
//? description: Tests for rfc822Name dataType.
//? sourceType: script
//?
//? test: rfc822Name-1
//? description: Test 1 for rfc822Name dataType.
//? expect: boolean(true)
//? source: ...

eq<rfc822Name>(
    rfc822Name("username@domainname"),
    rfc822Name("username@domainname")
)

//?
//? test: rfc822Name-2
//? description: Test 2 for rfc822Name dataType.
//? expect: boolean(true)
//? source: ...

eq<rfc822Name>(
    rfc822Name("foo@whatever.com"),
    rfc822Name("foo@whatever.com")
)

//?
//? test: rfc822Name-3
//? description: Test 3 for rfc822Name dataType.
//? expect: boolean(true)
//? source: ...

eq<rfc822Name>(
    rfc822Name("fooemail@bar.com"),
    rfc822Name("fooemail@bar.com")
)

//?
//? test: rfc822Name-4
//? description: Test 4 for rfc822Name dataType.
//? expect: boolean(true)
//? source: ...

eq<rfc822Name>(
    rfc822Name("userxyz@192.168.0.5"),
    rfc822Name("userxyz@192.168.0.5")
)

//?
//? test: rfc822Name-5
//? description: Test 5 for rfc822Name dataType.
//? expect: boolean(true)
//? source: ...

eq<rfc822Name>(
    rfc822Name("userxyz@FEDC:BA98:7654:3210:FEDC:BA98:7654:3210"),
    rfc822Name("userxyz@FEDC:BA98:7654:3210:FEDC:BA98:7654:3210")
)

