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

const d = "PT2147483647M"dayTimeDuration;
assert(d !== undefined);
return 0;

//?
//? test: minutes-int32-max-plus-one
//? description: PT2147483648M is out of range
//? expect: error
//? source: ...

"PT2147483648M"dayTimeDuration

//?
//? test: minutes-above-uint-range-example
//? description: PT3000000000M is out of range
//? expect: error
//? source: ...

"PT3000000000M"dayTimeDuration

//?
//? test: seconds-int32-max
//? description: PT2147483647S is accepted
//? expect: 0
//? source: ...

const d = "PT2147483647S"dayTimeDuration;
assert(d !== undefined);
return 0;

//?
//? test: seconds-int32-max-plus-one
//? description: PT2147483648S is out of range
//? expect: error
//? source: ...

"PT2147483648S"dayTimeDuration

//?
//? test: days-still-int32
//? description: days already used the signed 32-bit max
//? expect: error
//? source: ...

"P2147483648D"dayTimeDuration

//?
//? test: yearMonth-months-still-int32
//? description: yearMonth months already used the signed 32-bit max
//? expect: error
//? source: ...

"P2147483648M"yearMonthDuration
