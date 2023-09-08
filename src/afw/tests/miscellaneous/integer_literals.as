#!/usr/bin/env -S afw --syntax test_script
//? testScript: integer_literals.as
//? customPurpose: Part of miscellaneous category tests
//? description: Test integer literals
//? sourceType: script
//?
//? test: DecimalIntegerLiteral-0
//? description: Test 0 decimal literal
//? expect: 0
//? source: ...

return 0;

//? test: DecimalIntegerLiteral-+0
//? description: Test +0 decimal literal
//? expect: 0
//? source: ...

return +0;

//? test: DecimalIntegerLiteral--0
//? description: Test -0 decimal literal
//? expect: 0
//? source: ...

return -0;

//?
//? test: DecimalIntegerLiteral-min
//? description: Test minimum decimal literal
//? expect: -9223372036854775808
//? source: ...

return -9223372036854775808;

//? test: DecimalIntegerLiteral-normal
//? description: Test normal decimal literal
//? expect: 123456789
//? source: ...

return 123456789;

//? test: DecimalIntegerLiteral-max
//? description: Test minimum decimal literal
//? expect: 9223372036854775807
//? source: ...

return 9223372036854775807;

//? test: DecimalIntegerLiteral-too-small
//? description: Test too small decimal literal
//? expect: error
//? source: ...

return -9223372036854775809;

//? test: DecimalIntegerLiteral-too-large
//? description: Test too large decimal literal
//? expect: error
//? source: ...

return 9223372036854775808;

//? test: BinaryIntegerLiteral-normal
//? description: Test normal binary literal
//? expect: 123456789
//? source: ...

return 0b0111010110111100110100010101;

//?
//? test: BinaryIntegerLiteral-min
//? description: Test minimum hex literal (one more than binary min)
//? expect: -9223372036854775807
//? source: ...

return -0b111111111111111111111111111111111111111111111111111111111111111;

//? test: BinaryIntegerLiteral-max
//? description: Test minimum binary literal
//? expect: 9223372036854775807
//? source: ...

return 0b111111111111111111111111111111111111111111111111111111111111111;

//? test: BinaryIntegerLiteral-too-small
//? description: Test too small binary literal
//? expect: error
//? source: ...

return -0b1111111111111111111111111111111111111111111111111111111111111111;

//? test: BinaryIntegerLiteral-too-large
//? description: Test too large binary literal
//? expect: error
//? source: ...

return 0b1111111111111111111111111111111111111111111111111111111111111111;

//? test: HexIntegerLiteral-normal
//? description: Test normal hex literal
//? expect: 123456789
//? source: ...

return 0X75BCD15;

//?
//? test: HexIntegerLiteral-min
//? description: Test minimum hex literal (one more than decimal min)
//? expect: -9223372036854775807
//? source: ...

return -0x7fffffffffffffff;

//? test: HexIntegerLiteral-max
//? description: Test minimum hex literal
//? expect: 9223372036854775807
//? source: ...

return 0x7fffffffffffffff;

//? test: HexIntegerLiteral-too-small
//? description: Test too small hex literal
//? expect: error
//? source: ...

return -0xffffffffffffffff;

//? test: HexIntegerLiteral-too-large
//? description: Test too large hex literal
//? expect: error
//? source: ...

return 0xffffffffffffffff;

//? test: OctalIntegerLiteral-normal
//? description: Test normal octal literal
//? expect: 123456789
//? source: ...

return 0o726746425;

//?
//? test: OctalIntegerLiteral-min
//? description: Test minimum octal literal (one more than decimal min)
//? expect: -9223372036854775807
//? source: ...

return -0o777777777777777777777;

//? test: OctalIntegerLiteral-max
//? description: Test minimum octal literal
//? expect: 9223372036854775807
//? source: ...

return 0o777777777777777777777;

//? test: octalIntegerLiteral-too-small
//? description: Test too small octal literal
//? expect: error
//? source: ...

return -0o7777777777777777777771;

//? test: OctalIntegerLiteral-too-large
//? description: Test too large octal literal
//? expect: error
//? source: ...

return 0o7777777777777777777771;

