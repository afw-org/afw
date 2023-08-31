#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_abs.as
//? customPurpose: Part of core function tests
//? description: Tests for abs<double> function.
//? sourceType: expression
//?
//? test: abs<double>-1
//? description: 3.1416
//? expect: double(3.1416)
//? source: ...

abs<double>(
    double(3.1416)
)

//?
//? test: abs<double>-2
//? description: -3.1416
//? expect: double(3.1416)
//? source: ...

abs<double>(
    double(-3.1416)
)

//?
//? test: abs<double>-3
//? description: -92233720368547758
//? expect: double(92233720368547758.0)
//? source: ...

abs<double>(
    double(-92233720368547758.0)
)

//?
//? test: abs<double>-4
//? description: Invalid arguments test.
//? expect: error
//? source: ...

abs<double>(
    double(x.0)
)

