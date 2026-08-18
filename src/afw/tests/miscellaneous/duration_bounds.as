#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: duration_bounds.as
//? customPurpose: Part of miscellaneous category tests
//? description: dayTimeDuration minutes and seconds reject values above signed 32-bit max
//? sourceType: script
//?
//? test: minutes-int32-max
//? description: PT2147483647M is accepted
//? expect: 0
//? source: ...

const d = dayTimeDuration("PT2147483647M");
assert(d !== undefined);
return 0;

//?
//? test: minutes-int32-max-plus-one
//? description: PT2147483648M is out of range
//? expect: error
//? source: ...

dayTimeDuration("PT2147483648M")

//?
//? test: minutes-above-uint-range-example
//? description: PT3000000000M is out of range
//? expect: error
//? source: ...

dayTimeDuration("PT3000000000M")

//?
//? test: seconds-int32-max
//? description: PT2147483647S is accepted
//? expect: 0
//? source: ...

const d = dayTimeDuration("PT2147483647S");
assert(d !== undefined);
return 0;

//?
//? test: seconds-int32-max-plus-one
//? description: PT2147483648S is out of range
//? expect: error
//? source: ...

dayTimeDuration("PT2147483648S")

//?
//? test: days-still-int32
//? description: days already used the signed 32-bit max
//? expect: error
//? source: ...

dayTimeDuration("P2147483648D")

//?
//? test: yearMonth-months-still-int32
//? description: yearMonth months already used the signed 32-bit max
//? expect: error
//? source: ...

yearMonthDuration("P2147483648M")
