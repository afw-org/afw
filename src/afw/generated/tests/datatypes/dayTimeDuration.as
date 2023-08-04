#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: datatypes/dayTimeDuration.as
//? customPurpose: Part of core dataType tests
//? description: Tests for dayTimeDuration dataType.
//? sourceType: expression
//?
//? test: dayTimeDuration-1
//? description: Test 1 for dayTimeDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<dayTimeDuration>(
    dayTimeDuration("P1DT2H"),
    dayTimeDuration("P1DT2H")
)

//?
//? test: dayTimeDuration-2
//? description: Test 2 for dayTimeDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<dayTimeDuration>(
    dayTimeDuration("PT20M"),
    dayTimeDuration("PT20M")
)

//?
//? test: dayTimeDuration-3
//? description: Test 3 for dayTimeDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<dayTimeDuration>(
    dayTimeDuration("PT120M"),
    dayTimeDuration("PT120M")
)

//?
//? test: dayTimeDuration-4
//? description: Test 4 for dayTimeDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<dayTimeDuration>(
    dayTimeDuration("P0DT1H"),
    dayTimeDuration("P0DT1H")
)

//?
//? test: dayTimeDuration-5
//? description: Test 5 for dayTimeDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<dayTimeDuration>(
    dayTimeDuration("P0D"),
    dayTimeDuration("P0D")
)

//?
//? test: dayTimeDuration-6
//? description: Test 6 for dayTimeDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<dayTimeDuration>(
    dayTimeDuration("-P60D"),
    dayTimeDuration("-P60D")
)

//?
//? test: dayTimeDuration-7
//? description: Test 7 for dayTimeDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<dayTimeDuration>(
    dayTimeDuration("PT1M30.5S"),
    dayTimeDuration("PT1M30.5S")
)

