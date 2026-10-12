#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_intersection.as
//? customPurpose: Part of core function tests
//? description: Tests for intersection<date> function.
//? sourceType: script
//?
//? test: intersection<date>-1
//? description: duplicates
//? expect: ...

bag<date>(
    "2004-04-12"date,
    "1970-01-01"date
)

//? source: ...

intersection<date>(
    bag<date>(
        "2004-04-12"date,
        "1970-01-01"date,
        "2004-04-12"date,
        "2004-04-12"date
    ),
    bag<date>(
        "1970-01-01"date,
        "2004-04-12"date
    )
)

//?
//? test: intersection<date>-2
//? description: empty
//? expect: ...

bag<date>(
)

//? source: ...

intersection<date>(
    bag<date>(
        "2004-04-12"date
    ),
    bag<date>(
        "1970-01-01"date
    )
)

