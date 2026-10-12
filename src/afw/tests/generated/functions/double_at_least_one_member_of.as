#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_at_least_one_member_of.as
//? customPurpose: Part of core function tests
//? description: Tests for at_least_one_member_of<double> function.
//? sourceType: script
//?
//? test: at_least_one_member_of<double>-1
//? description: ...

            Double at least one member of for bags:
            { '52.40215', '-15.8' },
            { }
        
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<double>(
    array<double>(
        52.40215,
        -15.8
    ),
    array<double>(
    )
)

//?
//? test: at_least_one_member_of<double>-2
//? description: ...

            Double at least one member of for bags:
            { },
            { '18.255' }
        
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<double>(
    array<double>(
    ),
    array<double>(
        18.255
    )
)

//?
//? test: at_least_one_member_of<double>-3
//? description: ...

            Double at least one member of for bags:
            {
                '2000.0215', '11', '-11.2151', '961518', '99', '-102.1651054', '-586.055', '98600', '0.15387',
                '512618', '-512618', '.0215200', '11516.105155', '-11', '51081', '0.99999999999', '-102',
                '-0.111115866', '986.3211111', '-0.0', '0.000000'
            },
            { '2001.0215', '-110.2151', '0.99999999999', '-102', '986.3211111', '0.000000' }
        
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<double>(
    array<double>(
        2000.0215,
        11.0,
        -11.2151,
        961518.0,
        99.0,
        -102.1651054,
        -586.055,
        98600.0,
        0.15387,
        512618.0,
        -512618.0,
        0.0215200,
        11516.105155,
        -11.0,
        51081.0,
        0.99999999999,
        -102.0,
        -0.111115866,
        986.3211111,
        -0.0,
        0.000000
    ),
    array<double>(
        2001.0215,
        -110.2151,
        0.99999999999,
        -102.0,
        986.3211111,
        0.000000
    )
)

//?
//? test: at_least_one_member_of<double>-4
//? description: ...

            Double at least one member of for bags:
            { '-0.0', '0.000000' },
            {
                '2000.0215', '-11.2151', '961518', '99', '-102.1651054', '-586.055', '0.15387', '512618', '-512618',
                '11516.105155', '51081', '0.99999999999', '-102', '986.3211111', '0.000000'
            }
        
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<double>(
    array<double>(
        -0.0,
        0.000000
    ),
    array<double>(
        2000.0215,
        -11.2151,
        961518.0,
        99.0,
        -102.1651054,
        -586.055,
        0.15387,
        512618.0,
        -512618.0,
        11516.105155,
        51081.0,
        0.99999999999,
        -102.0,
        986.3211111,
        0.000000
    )
)

