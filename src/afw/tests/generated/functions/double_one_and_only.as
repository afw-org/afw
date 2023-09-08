#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<double> function.
//? sourceType: script
//?
//? test: one_and_only<double>-1
//? description: ...

            Double one and only for bag:
            { 1.111 }
        
//? expect: double(1.111)
//? source: ...

one_and_only<double>(
    bag<double>(
        double(1.111)
    )
)

//?
//? test: one_and_only<double>-2
//? description: ...

            Double one and only for bag:
            { 1.111, 2.222, 3.333, 4.444, 5.555 }
        
//? expect: error
//? source: ...

one_and_only<double>(
    bag<double>(
        double(1.111),
        double(2.222),
        double(3.333),
        double(4.444),
        double(5.555)
    )
)

//?
//? test: one_and_only<double>-3
//? description: ...

            Double one and only for bag:
            { }
        
//? expect: error
//? source: ...

one_and_only<double>(
    bag<double>(
    )
)

