#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<double> function.
//? sourceType: script
//?
//? test: bag_size<double>-1
//? description: Double bag size of 0.
//? expect: integer(0)
//? source: ...

bag_size<double>(
    bag<double>(
    )
)

//?
//? test: bag_size<double>-2
//? description: Double bag size of 2.
//? expect: integer(2)
//? source: ...

bag_size<double>(
    bag<double>(
        double(52.40215),
        double(-15.8)
    )
)

//?
//? test: bag_size<double>-3
//? description: Double bag size of 21.
//? expect: integer(21)
//? source: ...

bag_size<double>(
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
        double(5126189454.0),
        double(-5126189454.0),
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
    )
)

