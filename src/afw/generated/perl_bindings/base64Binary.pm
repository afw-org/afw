#!/usr/bin/env perl

#**
# @file base64Binary.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the base64Binary category.
#*

package afw::base64Binary;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    array_base64Binary 
    as_base64Binary 
    at_least_one_member_of_base64Binary 
    decode_to_string_base64Binary 
    eq_base64Binary 
    eqx_base64Binary 
    ge_base64Binary 
    gt_base64Binary 
    intersection_base64Binary 
    is_base64Binary 
    is_in_base64Binary 
    le_base64Binary 
    lt_base64Binary 
    ne_base64Binary 
    nex_base64Binary 
    one_and_only_base64Binary 
    set_equals_base64Binary 
    subset_base64Binary 
    to_string_base64Binary 
    union_base64Binary 
);

=head1 NAME

afw::base64Binary - Perl module for base64Binary functions

=head2 Functions

The following functions are exported by default

=head3 array_base64Binary

Construct a new array of base64Binary from the given values, the same as
array() except that the new array is typed: each value, and each entry of a
spread array, must be dataType base64Binary, and adding a value of another
data type to the array later throws. A value of another data type is an error,
never converted. This is usually written `array<base64Binary>(`...values).
Construct an array of base64Binary from values or spread arrays

=head4 Parameters

    $values

A value of dataType base64Binary, or ... followed by an expression that
results in an array, every entry of which is included.

=head3 as_base64Binary

Returns value unchanged if it is dataType base64Binary. Otherwise, including
when value is undefined, throws an error. It never converts: to get a
base64Binary from a value of another data type, write a typed literal or call
a to_T conversion function where one exists. This is usually written value as
base64Binary.
Asserts that value is dataType base64Binary

=head4 Parameters

    $value

Value to assert

=head3 at_least_one_member_of_base64Binary

Returns boolean true if at least one value in base64Binary array1 is in
base64Binary array2.
Checks for at least one value in common

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 decode_to_string_base64Binary

Decode base64Binary value to string. An error is thrown if decoded value is
not valid UTF-8.
Decode base64Binary to string

=head4 Parameters

    $value

The base64Binary value to decode.

=head3 eq_base64Binary

Determine if base64Binary arg1 is equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'eqx' ('===') instead if
you want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_base64Binary

Determine if for base64Binary arg1 is equal to the value and data type of arg2
then return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_base64Binary

Checks for base64Binary arg1 is greater than or equal to base64Binary arg2 and
return the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_base64Binary

Checks for base64Binary arg1 is greater than base64Binary arg2 and return the
boolean result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 intersection_base64Binary

Returns an array of base64Binary with the values that are common to both array
of base64Binary array1 and array2.
Returns intersection of two arrays

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 is_base64Binary

Checks whether value is dataType base64Binary and returns the boolean result.
It is false for undefined. This can also be written `is<base64Binary>(value`).
Checks whether value is dataType base64Binary

=head4 Parameters

    $value

Value to check

=head3 is_in_base64Binary

Checks whether base64Binary value is in array of base64Binary array and
returns the boolean result. Empty array yields false.
Checks whether a value is in an array

=head4 Parameters

    $value


    $array


=head3 le_base64Binary

Checks for base64Binary arg1 is less than or equal to base64Binary arg2 and
return the boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_base64Binary

Checks for base64Binary arg1 is less that base64Binary arg2 and return the
boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_base64Binary

Determine if base64Binary arg1 is not equal to the value of arg2 converted to
the data type of arg1 then return the boolean result. Use 'nex' ('!==')
instead if you want true to be returned if arg1 and arg2's data type don't
match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_base64Binary

Determine if for base64Binary arg1 is not equal to the value or data type of
arg2 then return the boolean result. Use 'ne' ('!=') instead if you want arg2
to be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_base64Binary

This converts an array of base64Binary values that contains one value to a
single base64Binary value.
Converts a one value array to a base64Binary value

=head4 Parameters

    $array


=head3 set_equals_base64Binary

Returns boolean true if base64Binary array1 and base64Binary array2 are
subsets of each other and return the boolean result.
Checks whether two arrays are subsets of each other

=head4 Parameters

    $array1


    $array2


=head3 subset_base64Binary

Returns boolean true if the unique values in base64Binary array1 are all in
base64Binary array2.
Determines if the first array is a subset of second array

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 to_string_base64Binary

Converts base64Binary value to string. For array values, the to_string() value
for each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A base64Binary value.

=head3 union_base64Binary

Returns an array of base64Binary contains all of the unique values in two or
more array of base64Binary values.
Returns union of two or more string arrays

=head4 Parameters

    $arrays

Two or more arrays.

=cut

sub array_base64Binary {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "array_base64Binary");
    $request->set("values", $values);

    return $request->getResult();
}

sub as_base64Binary {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "as_base64Binary");
    $request->set("value", $value);

    return $request->getResult();
}

sub at_least_one_member_of_base64Binary {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<base64Binary>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub decode_to_string_base64Binary {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "decode_to_string<base64Binary>");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_base64Binary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<base64Binary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_base64Binary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<base64Binary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_base64Binary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<base64Binary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_base64Binary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<base64Binary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub intersection_base64Binary {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<base64Binary>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub is_base64Binary {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is_base64Binary");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_in_base64Binary {
    my ($value, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<base64Binary>");
    $request->set("value", $value);
    $request->set("array", $array);

    return $request->getResult();
}

sub le_base64Binary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<base64Binary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_base64Binary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<base64Binary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_base64Binary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<base64Binary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_base64Binary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<base64Binary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_base64Binary {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<base64Binary>");
    $request->set("array", $array);

    return $request->getResult();
}

sub set_equals_base64Binary {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<base64Binary>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub subset_base64Binary {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<base64Binary>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub to_string_base64Binary {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<base64Binary>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_base64Binary {
    my ($arrays) = @_;

    my $request = $session->request()

    $request->set("function" => "union<base64Binary>");
    $request->set("arrays", $arrays);

    return $request->getResult();
}

42;
