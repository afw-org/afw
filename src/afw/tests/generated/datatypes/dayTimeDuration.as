#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: datatypes/dayTimeDuration.as
//? customPurpose: Part of core dataType tests
//? description: Tests for dayTimeDuration dataType.
//? sourceType: script
//?
//? test: dayTimeDuration-1
//? description: Test 1 for dayTimeDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<dayTimeDuration>(
    "P1DT2H"dayTimeDuration,
    "P1DT2H"dayTimeDuration
)

//?
//? test: dayTimeDuration-2
//? description: Test 2 for dayTimeDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<dayTimeDuration>(
    "PT20M"dayTimeDuration,
    "PT20M"dayTimeDuration
)

//?
//? test: dayTimeDuration-3
//? description: Test 3 for dayTimeDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<dayTimeDuration>(
    "PT120M"dayTimeDuration,
    "PT120M"dayTimeDuration
)

//?
//? test: dayTimeDuration-4
//? description: Test 4 for dayTimeDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<dayTimeDuration>(
    "P0DT1H"dayTimeDuration,
    "P0DT1H"dayTimeDuration
)

//?
//? test: dayTimeDuration-5
//? description: Test 5 for dayTimeDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<dayTimeDuration>(
    "P0D"dayTimeDuration,
    "P0D"dayTimeDuration
)

//?
//? test: dayTimeDuration-6
//? description: Test 6 for dayTimeDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<dayTimeDuration>(
    "-P60D"dayTimeDuration,
    "-P60D"dayTimeDuration
)

//?
//? test: dayTimeDuration-7
//? description: Test 7 for dayTimeDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<dayTimeDuration>(
    "PT1M30.5S"dayTimeDuration,
    "PT1M30.5S"dayTimeDuration
)

