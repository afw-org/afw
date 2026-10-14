#!/usr/bin/env perl

#**
# @file double.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the double category.
#*

package afw::double;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    abs_double 
    acos_double 
    add_double 
    array_double 
    as_double 
    asin_double 
    at_least_one_member_of_double 
    atan2_double 
    atan_double 
    cbrt_double 
    ceil_double 
    cos_double 
    divide_double 
    eq_double 
    eqx_double 
    exp_double 
    floor_double 
    ge_double 
    gt_double 
    hypot_double 
    intersection_double 
    is_NaN 
    is_double 
    is_finite 
    is_in_double 
    le_double 
    log10_double 
    log2_double 
    log_double 
    lt_double 
    max_double 
    min_double 
    mod_double 
    multiply_double 
    ne_double 
    negative_double 
    nex_double 
    one_and_only_double 
    pow_double 
    round_double 
    set_equals_double 
    sign_double 
    sin_double 
    sqrt_double 
    subset_double 
    subtract_double 
    tan_double 
    to_integer_double 
    to_string_double 
    trunc_double 
    union_double 
);

=head1 NAME

afw::double - Perl module for double functions

=head2 Functions

The following functions are exported by default

=head3 abs_double

Compute the absolute value of the double value and return the double result.
Absolute value

=head4 Parameters

    $value


=head3 acos_double

Return the arccosine, in radians from 0 to #pi, of the double value, as
ECMAScript Math.acos does. A value outside -1.0 to 1.0 is an error. A result
that would be NaN, or a NaN argument, is an error. An integer is not accepted:
write to_double(v).
Arccosine

=head4 Parameters

    $value


=head3 add_double

Add 2 or more double values and return the double result.
Add

=head4 Parameters

    $values


=head3 array_double

Construct a new array of double from the given values, the same as array()
except that the new array is typed: each value, and each entry of a spread
array, must be dataType double, and adding a value of another data type to the
array later throws. A value of another data type is an error, never converted.
This is usually written `array<double>(`...values).
Construct an array of double from values or spread arrays

=head4 Parameters

    $values

A value of dataType double, or ... followed by an expression that results in
an array, every entry of which is included.

=head3 as_double

Returns value unchanged if it is dataType double. Otherwise, including when
value is undefined, throws an error. It never converts: to get a double from a
value of another data type, write a typed literal or call a to_T conversion
function where one exists. This is usually written value as double.
Asserts that value is dataType double

=head4 Parameters

    $value

Value to assert

=head3 asin_double

Return the arcsine, in radians from -#pi/2 to #pi/2, of the double value, as
ECMAScript Math.asin does. A value outside -1.0 to 1.0 is an error. A result
that would be NaN, or a NaN argument, is an error. An integer is not accepted:
write to_double(v).
Arcsine

=head4 Parameters

    $value


=head3 at_least_one_member_of_double

Returns boolean true if at least one value in double array1 is in double
array2.
Checks for at least one value in common

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 atan2_double

Return the angle, in radians from -#pi to #pi, from the positive x axis to the
point (x, y), as ECMAScript Math.atan2(y, x) does; y comes first. A result
that would be NaN, or a NaN argument, is an error. An integer is not accepted:
write to_double(v).
Arctangent of a quotient

=head4 Parameters

    $y

The y coordinate.

    $x

The x coordinate.

=head3 atan_double

Return the arctangent, in radians from -#pi/2 to #pi/2, of the double value,
as ECMAScript Math.atan does. A result that would be NaN, or a NaN argument,
is an error. An integer is not accepted: write to_double(v).
Arctangent

=head4 Parameters

    $value


=head3 cbrt_double

Return the cube root of the double value, as ECMAScript Math.cbrt does. A
result that would be NaN, or a NaN argument, is an error. An integer is not
accepted: write to_double(v).
Cube root

=head4 Parameters

    $value


=head3 ceil_double

Determine the smallest integer that is greater then or equal to the double
value and return the double result.
Round upward to nearest integer

=head4 Parameters

    $value


=head3 cos_double

Return the cosine of the double value in radians, as ECMAScript Math.cos does
(#pi is π). An infinite value is an error. A result that would be NaN, or a
NaN argument, is an error. An integer is not accepted: write to_double(v).
Cosine

=head4 Parameters

    $value


=head3 divide_double

Divide double dividend by double divisor and return the double quotient.
Divide numbers

=head4 Parameters

    $dividend


    $divisor


=head3 eq_double

Determine if double arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_double

Determine if for double arg1 is equal to the value and data type of arg2 then
return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 exp_double

Return #e raised to the double value, as ECMAScript Math.exp does. A result
that would be NaN, or a NaN argument, is an error. An integer is not accepted:
write to_double(v).
e raised to a power

=head4 Parameters

    $value


=head3 floor_double

Determine the largest integer that is smaller then or equal to the double
value and return the double result.
Round downwards to nearest integer

=head4 Parameters

    $number


=head3 ge_double

Checks for double arg1 is greater than or equal to double arg2 and return the
boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_double

Checks for double arg1 is greater than double arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 hypot_double

Return the square root of the sum of the squares of the double values, as
ECMAScript Math.hypot does, without overflow or underflow in between. At least
one value is required (ECMAScript's hypot() is 0); an infinite value makes the
result Infinity. A result that would be NaN, or a NaN argument, is an error.
An integer is not accepted: write to_double(v).
Square root of the sum of squares

=head4 Parameters

    $values

The values.

=head3 intersection_double

Returns an array of double with the values that are common to both array of
double array1 and array2.
Returns intersection of two arrays

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 is_NaN

Checks if the argument 'number' is not a number(NaN) and returns the boolean
result.
Is not a number(NaN)

=head4 Parameters

    $number

Number to check

=head3 is_double

Checks whether value is dataType double and returns the boolean result. It is
false for undefined. This can also be written `is<double>(value`).
Checks whether value is dataType double

=head4 Parameters

    $value

Value to check

=head3 is_finite

Checks if the argument 'number' is finite and returns the boolean result.
Is a number finite

=head4 Parameters

    $number

Number to check

=head3 is_in_double

Checks whether double value is in array of double array and returns the
boolean result. Empty array yields false.
Checks whether a value is in an array

=head4 Parameters

    $value


    $array


=head3 le_double

Checks for double arg1 is less than or equal to double arg2 and return the
boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 log10_double

Return the base 10 logarithm of the double value, as ECMAScript Math.log10
does. log10(0.0) is -Infinity; a negative value is an error. A result that
would be NaN, or a NaN argument, is an error. An integer is not accepted:
write to_double(v).
Base 10 logarithm

=head4 Parameters

    $value


=head3 log2_double

Return the base 2 logarithm of the double value, as ECMAScript Math.log2 does.
log2(0.0) is -Infinity; a negative value is an error. A result that would be
NaN, or a NaN argument, is an error. An integer is not accepted: write
to_double(v).
Base 2 logarithm

=head4 Parameters

    $value


=head3 log_double

Return the natural logarithm (base #e) of the double value, as ECMAScript
Math.log does. log(0.0) is -Infinity; a negative value is an error. A result
that would be NaN, or a NaN argument, is an error. An integer is not accepted:
write to_double(v).
Natural logarithm

=head4 Parameters

    $value


=head3 lt_double

Checks for double arg1 is less that double arg2 and return the boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 max_double

Return the double value that is greater than or equal to the others. If any
value is NaN, the result is NaN, as ECMAScript Math.max does.
Maximum value

=head4 Parameters

    $values


=head3 min_double

Return the double value that is less than or equal to the others. If any value
is NaN, the result is NaN, as ECMAScript Math.min does.
Minimum value

=head4 Parameters

    $values


=head3 mod_double

Divide double dividend by double divisor and return the double remainder.
Remainder of dividing numbers

=head4 Parameters

    $dividend


    $divisor


=head3 multiply_double

Multiply 2 or more double values and return the double result.
Multiply numbers

=head4 Parameters

    $values


=head3 ne_double

Determine if double arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead if
you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 negative_double

Return negative of double value.
Negative of a number

=head4 Parameters

    $value


=head3 nex_double

Determine if for double arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_double

This converts an array of double values that contains one value to a single
double value.
Converts a one value array to a double value

=head4 Parameters

    $array


=head3 pow_double

This returns the value of base raised to a power. Multiple exponents can be
specified to raise the previous exponent to the power of the latter exponent.
Base raised to a power

=head4 Parameters

    $base

Base value.

    $exponent

Exponent value.

=head3 round_double

Determine the integer closest to double value and return the double result.
Round to nearest integer

=head4 Parameters

    $number


=head3 set_equals_double

Returns boolean true if double array1 and double array2 are subsets of each
other and return the boolean result.
Checks whether two arrays are subsets of each other

=head4 Parameters

    $array1


    $array2


=head3 sign_double

Return the sign of the double value, as ECMAScript Math.sign does: -1, 0 or 1
of the same data type (`sign<double>` keeps the sign of a zero: sign(-0.0) is
-0.0). A NaN argument is an error.
Sign of a number

=head4 Parameters

    $value


=head3 sin_double

Return the sine of the double value in radians, as ECMAScript Math.sin does
(#pi is π). An infinite value is an error. A result that would be NaN, or a
NaN argument, is an error. An integer is not accepted: write to_double(v).
Sine

=head4 Parameters

    $value


=head3 sqrt_double

Return the square root of the double value, as ECMAScript Math.sqrt does;
sqrt(-0.0) is -0.0. A negative value is an error. A result that would be NaN,
or a NaN argument, is an error. An integer is not accepted: write
to_double(v).
Square root

=head4 Parameters

    $value


=head3 subset_double

Returns boolean true if the unique values in double array1 are all in double
array2.
Determines if the first array is a subset of second array

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 subtract_double

Subtract double arg2 from double arg1 and return the double result.
Subtract numbers

=head4 Parameters

    $arg1


    $arg2


=head3 tan_double

Return the tangent of the double value in radians, as ECMAScript Math.tan does
(#pi is π). An infinite value is an error. A result that would be NaN, or a
NaN argument, is an error. An integer is not accepted: write to_double(v).
Tangent

=head4 Parameters

    $value


=head3 to_integer_double

Converts double value to integer. A double is truncated toward zero (1.9 is 1
and -1.9 is -1); use round(), floor(), or ceil() first for another choice. A
double that is NaN, infinite, or outside the integer range is an error. A
string is read as integer text, and text that is not an integer ("1.5", " 1",
"1x") is an error.
Converts to integer

=head4 Parameters

    $value


=head3 to_string_double

Converts double value to string. For array values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A double value.

=head3 trunc_double

Return the integer part of the double value, removing any fraction, as
ECMAScript Math.trunc does (toward zero: trunc(-1.5) is -1.0). The result is a
double: write to_integer(trunc(v)) for an integer. A result that would be NaN,
or a NaN argument, is an error. An integer is not accepted: write
to_double(v).
Integer part

=head4 Parameters

    $value


=head3 union_double

Returns an array of double contains all of the unique values in two or more
array of double values.
Returns union of two or more string arrays

=head4 Parameters

    $arrays

Two or more arrays.

=cut

sub abs_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "abs<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub acos_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "acos<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub add_double {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "add<double>");
    $request->set("values", $values);

    return $request->getResult();
}

sub array_double {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "array_double");
    $request->set("values", $values);

    return $request->getResult();
}

sub as_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "as_double");
    $request->set("value", $value);

    return $request->getResult();
}

sub asin_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "asin<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub at_least_one_member_of_double {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<double>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub atan2_double {
    my ($y, $x) = @_;

    my $request = $session->request()

    $request->set("function" => "atan2<double>");
    $request->set("y", $y);
    $request->set("x", $x);

    return $request->getResult();
}

sub atan_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "atan<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub cbrt_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "cbrt<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub ceil_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "ceil<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub cos_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "cos<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub divide_double {
    my ($dividend, $divisor) = @_;

    my $request = $session->request()

    $request->set("function" => "divide<double>");
    $request->set("dividend", $dividend);
    $request->set("divisor", $divisor);

    return $request->getResult();
}

sub eq_double {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<double>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_double {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<double>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub exp_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "exp<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub floor_double {
    my ($number) = @_;

    my $request = $session->request()

    $request->set("function" => "floor<double>");
    $request->set("number", $number);

    return $request->getResult();
}

sub ge_double {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<double>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_double {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<double>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub hypot_double {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "hypot<double>");
    $request->set("values", $values);

    return $request->getResult();
}

sub intersection_double {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<double>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub is_NaN {
    my ($number) = @_;

    my $request = $session->request()

    $request->set("function" => "is_NaN");
    $request->set("number", $number);

    return $request->getResult();
}

sub is_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is_double");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_finite {
    my ($number) = @_;

    my $request = $session->request()

    $request->set("function" => "is_finite");
    $request->set("number", $number);

    return $request->getResult();
}

sub is_in_double {
    my ($value, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<double>");
    $request->set("value", $value);
    $request->set("array", $array);

    return $request->getResult();
}

sub le_double {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<double>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub log10_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "log10<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub log2_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "log2<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub log_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "log<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub lt_double {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<double>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub max_double {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "max<double>");
    $request->set("values", $values);

    return $request->getResult();
}

sub min_double {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "min<double>");
    $request->set("values", $values);

    return $request->getResult();
}

sub mod_double {
    my ($dividend, $divisor) = @_;

    my $request = $session->request()

    $request->set("function" => "mod<double>");
    $request->set("dividend", $dividend);
    $request->set("divisor", $divisor);

    return $request->getResult();
}

sub multiply_double {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "multiply<double>");
    $request->set("values", $values);

    return $request->getResult();
}

sub ne_double {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<double>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub negative_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "negative<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub nex_double {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<double>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_double {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<double>");
    $request->set("array", $array);

    return $request->getResult();
}

sub pow_double {
    my ($base, $exponent) = @_;

    my $request = $session->request()

    $request->set("function" => "pow<double>");
    $request->set("base", $base);
    $request->set("exponent", $exponent);

    return $request->getResult();
}

sub round_double {
    my ($number) = @_;

    my $request = $session->request()

    $request->set("function" => "round<double>");
    $request->set("number", $number);

    return $request->getResult();
}

sub set_equals_double {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<double>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub sign_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "sign<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub sin_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "sin<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub sqrt_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "sqrt<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub subset_double {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<double>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub subtract_double {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "subtract<double>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub tan_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "tan<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_integer_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_integer<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_string_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub trunc_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "trunc<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_double {
    my ($arrays) = @_;

    my $request = $session->request()

    $request->set("function" => "union<double>");
    $request->set("arrays", $arrays);

    return $request->getResult();
}

42;
