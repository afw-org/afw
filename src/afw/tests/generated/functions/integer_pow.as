#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_pow.as
//? customPurpose: Part of core function tests
//? description: Tests for pow<integer> and integer ** (#513)
//? sourceType: script
//?
//? test: pow<integer>-1
//? description: 2 ** 3 is the integer 8
//? expect: 8
//? source: ...

2 ** 3

//?
//? test: pow<integer>-right-to-left
//? description: 2 ** 3 ** 2 is 2 ** 9
//? expect: 512
//? source: ...

2 ** 3 ** 2

//?
//? test: pow<integer>-function
//? description: pow<integer> takes more than one exponent, right to left
//? expect: 512
//? source: ...

pow<integer>(2, 3, 2)

//?
//? test: pow<integer>-zero
//? description: 0 ** 0 is 1, 0 ** 5 is 0, 7 ** 0 is 1
//? expect: [1,0,1]
//? source: ...

[0 ** 0, 0 ** 5, 7 ** 0]

//?
//? test: pow<integer>-minus-one
//? description: -1 to a large exponent does not loop
//? expect: [-1,1]
//? source: ...

[(-1) ** 9999999999, (-1) ** 10000000000]

//?
//? test: pow<integer>-min
//? description: (-2) ** 63 is the smallest integer
//? expect: true
//? source: ...

(-2) ** 63 === -9223372036854775807 - 1

//?
//? test: pow<integer>-max
//? description: the largest power of 3 that fits
//? expect: 4052555153018976267
//? source: ...

3 ** 39

//?
//? test: pow<integer>-overflow
//? description: 2 ** 63 does not fit
//? expect: error:Integer pow overflow
//? source: ...

2 ** 63

//?
//? test: pow<integer>-overflow-negative
//? description: (-2) ** 64 does not fit
//? expect: error:Integer pow overflow
//? source: ...

(-2) ** 64

//?
//? test: pow<integer>-negative-exponent
//? description: a negative exponent is an error, not a double
//? expect: error:Integer pow with a negative exponent
//? source: ...

2 ** -1

//?
//? test: pow<integer>-mixed
//? description: integer and double do not mix
//? expect: error
//? source: ...

2 ** 3.0

//?
//? test: pow<integer>-assign
//? description: **= on an integer element
//? expect: [9]
//? source: ...

let a = [3];
a[0] **= 2;
return a;

//?
//? test: pow-prefixed-base
//? description: as in ECMAScript, -2 ** 2 is a syntax error
//? expect: error:Parse error at offset 4 around line 2 column 4: A prefix operator or sign before '**' needs parentheses: write (-2) ** 2 or -(2 ** 2)
//? source: ...

-2 ** 2

//?
//? test: pow-prefixed-middle
//? description: 2 ** -3 ** 2 is a syntax error too
//? expect: error
//? source: ...

2 ** -3 ** 2

//?
//? test: pow-parenthesized
//? description: (-2) ** 2, -(2 ** 2), 2 ** -0 and 3 - 2 ** 2 are fine
//? expect: [4,-4,1,-1]
//? source: ...

[(-2) ** 2, -(2 ** 2), 2 ** -0, 3 - 2 ** 2]
