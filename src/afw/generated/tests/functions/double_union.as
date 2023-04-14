#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_union.as
//? customPurpose: Part of core function tests
//? description: Tests for union<double> function.
//? sourceType: expression
//?
//? test: union<double>-1
//? description: ...

            Double union of bags:
            { "52.40215", "-15.8" },
            { "18.255" }
        
//? expect: ...

bag<double>(
    double(52.40215),
    double(-15.8),
    double(18.255)
)

//? source: ...

union<double>(
    bag<double>(
        double(52.40215),
        double(-15.8)
    ),
    bag<double>(
        double(18.255)
    )
)

//?
//? test: union<double>-2
//? description: ...

            Double union of bags:
            { },
            { "18.255" }
        
//? expect: ...

bag<double>(
    double(18.255)
)

//? source: ...

union<double>(
    bag<double>(
    ),
    bag<double>(
        double(18.255)
    )
)

//?
//? test: union<double>-3
//? description: ...

            Double union of bags:
            {
                "2000.0215", "11", "-11.2151", "961518", "99", "-102.1651054", "-586.055", "98600", "0.15387",
                "512618", "-512618", ".0215200", "11516.105155", "-11", "51081", "0.99999999999", "-102",
                "-0.111115866", "986.3211111", "-0.0", "0.000000"
            },
            {
                "2000.0215", "-11.2151", "961518", "99", "-102.1651054", "-586.055", "0.15387", "512618", "-512618",
                "11516.105155", "51081", "0.99999999999", "-102", "986.3211111", "0.000000"
            }
        
//? expect: ...

bag<double>(
    double(2000.0215),
    double(11.0),
    double(-11.2151),
    double(961518.0),
    double(99.0),
    double(-102.1651054),
    double(-586.055),
    double(98600.0),
    double(0.15387),
    double(512618.0),
    double(-512618.0),
    double(0.0215200),
    double(11516.105155),
    double(-11.0),
    double(51081.0),
    double(0.99999999999),
    double(-102.0),
    double(-0.111115866),
    double(986.3211111),
    double(0.0)
)

//? source: ...

union<double>(
    bag<double>(
        double(2000.0215),
        double(11.0),
        double(-11.2151),
        double(961518.0),
        double(99.0),
        double(-102.1651054),
        double(-586.055),
        double(98600.0),
        double(0.15387),
        double(512618.0),
        double(-512618.0),
        double(0.0215200),
        double(11516.105155),
        double(-11.0),
        double(51081.0),
        double(0.99999999999),
        double(-102.0),
        double(-0.111115866),
        double(986.3211111),
        double(-0.0),
        double(0.000000)
    ),
    bag<double>(
        double(2000.0215),
        double(-11.2151),
        double(961518.0),
        double(99.0),
        double(-102.1651054),
        double(-586.055),
        double(0.15387),
        double(512618.0),
        double(-512618.0),
        double(11516.105155),
        double(51081.0),
        double(0.99999999999),
        double(-102.0),
        double(986.3211111),
        double(0.000000)
    )
)

