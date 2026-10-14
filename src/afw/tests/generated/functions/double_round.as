#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_round.as
//? customPurpose: Part of core function tests
//? description: Tests for round<double> function.
//? sourceType: script
//?
//? test: round<double>-1
//? description: x
//? expect: 5.0
//? source: ...

round<double>(
    5.2
)

//?
//? test: round<double>-2
//? description: x
//? expect: 6.0
//? source: ...

round<double>(
    5.5
)

//?
//? test: round<double>-3
//? description: x
//? expect: 888888888888888886.0
//? source: ...

round<double>(
    888888888888888885.5
)

//?
//? test: round<double>-4
//? description: Invalid arguments test.
//? expect: error
//? source: ...

round<double>(
    to_double(x.0)
)

//?
//? test: round<double>-tie-negative
//? description: -2.5 rounds toward positive infinity, as ECMAScript Math.round and XPath fn:round (#543)
//? expect: -2.0
//? source: ...

round<double>(-2.5)

//?
//? test: round<double>-tie-negative-one-and-half
//? description: -1.5 rounds to -1
//? expect: -1.0
//? source: ...

round<double>(-1.5)

//?
//? test: round<double>-tie-positive
//? description: 2.5 rounds to 3
//? expect: 3.0
//? source: ...

round<double>(2.5)

//?
//? test: round<double>-below-half
//? description: 0.49999999999999994 rounds to 0, not 1
//? expect: 0.0
//? source: ...

round<double>(0.49999999999999994)

//?
//? test: round<double>-negative-past-tie
//? description: -2.5000000000000004 rounds to -3
//? expect: -3.0
//? source: ...

round<double>(-2.5000000000000004)

//?
//? test: round<double>-infinity
//? description: Infinity rounds to Infinity
//? expect: Infinity
//? source: ...

round<double>(Infinity)

//?
//? test: round<double>-negative-zero
//? description: -0.5 and -0.4 round to -0, as ECMAScript Math.round
//? expect: true
//? source: ...

1.0 / round<double>(-0.5) === -Infinity && 1.0 / round<double>(-0.4) === -Infinity
