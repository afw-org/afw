#!/usr/bin/env perl

#**
# @file dayTimeDuration.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the dayTimeDuration category.
#*

package afw::dayTimeDuration;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    array_dayTimeDuration 
    as_dayTimeDuration 
    at_least_one_member_of_dayTimeDuration 
    bag_dayTimeDuration 
    bag_size_dayTimeDuration 
    eq_dayTimeDuration 
    eqx_dayTimeDuration 
    ge_dayTimeDuration 
    gt_dayTimeDuration 
    intersection_dayTimeDuration 
    is_dayTimeDuration 
    is_in_dayTimeDuration 
    le_dayTimeDuration 
    lt_dayTimeDuration 
    ne_dayTimeDuration 
    nex_dayTimeDuration 
    one_and_only_dayTimeDuration 
    set_equals_dayTimeDuration 
    subset_dayTimeDuration 
    to_string_dayTimeDuration 
    union_dayTimeDuration 
);

=head1 NAME

afw::dayTimeDuration - Perl module for dayTimeDuration functions

=head2 Functions

The following functions are exported by default

=head3 array_dayTimeDuration

Construct a new array of dayTimeDuration from the given values, the same as
array() except that the new array is typed: each value, and each entry of a
spread array, must be dataType dayTimeDuration, and adding a value of another
data type to the array later throws. A value of another data type is an error,
never converted. This is usually written `array<dayTimeDuration>(`...values).
Construct an array of dayTimeDuration from values or spread arrays

=head4 Parameters

    $values

A value of dataType dayTimeDuration, or ... followed by an expression that
results in an array, every entry of which is included.

=head3 as_dayTimeDuration

Returns value unchanged if it is dataType dayTimeDuration. Otherwise,
including when value is undefined, throws an error. It never converts: to get
a dayTimeDuration from a value of another data type, write a typed literal or
call a to_T conversion function where one exists. This is usually written
value as dayTimeDuration.
Asserts that value is dataType dayTimeDuration

=head4 Parameters

    $value

Value to assert

=head3 at_least_one_member_of_dayTimeDuration

Returns boolean true if at least one value in dayTimeDuration array1 is in
dayTimeDuration array2.
Checks for at least one value in common

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 bag_dayTimeDuration

Takes any number of dayTimeDuration values and returns an array of
dayTimeDuration.
Makes an array from values

=head4 Parameters

    $values


=head3 bag_size_dayTimeDuration

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 eq_dayTimeDuration

Determine if dayTimeDuration arg1 is equal to the value of arg2 converted to
the data type of arg1 then return the boolean result. Use 'eqx' ('===')
instead if you want false to be returned if arg1 and arg2's data type don't
match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_dayTimeDuration

Determine if for dayTimeDuration arg1 is equal to the value and data type of
arg2 then return the boolean result. Use 'eq' ('==') instead if you want arg2
to be converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_dayTimeDuration

Checks for dayTimeDuration arg1 is greater than or equal to dayTimeDuration
arg2 and return the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_dayTimeDuration

Checks for dayTimeDuration arg1 is greater than dayTimeDuration arg2 and
return the boolean result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 intersection_dayTimeDuration

Returns an array of dayTimeDuration with the values that are common to both
array of dayTimeDuration array1 and array2.
Returns intersection of two arrays

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 is_dayTimeDuration

Checks whether value is dataType dayTimeDuration and returns the boolean
result. It is false for undefined. This can also be written
`is<dayTimeDuration>(value`).
Checks whether value is dataType dayTimeDuration

=head4 Parameters

    $value

Value to check

=head3 is_in_dayTimeDuration

Checks whether dayTimeDuration value is in array of dayTimeDuration array and
returns the boolean result. Empty array yields false.
Checks whether a value is in an array

=head4 Parameters

    $value


    $array


=head3 le_dayTimeDuration

Checks for dayTimeDuration arg1 is less than or equal to dayTimeDuration arg2
and return the boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_dayTimeDuration

Checks for dayTimeDuration arg1 is less that dayTimeDuration arg2 and return
the boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_dayTimeDuration

Determine if dayTimeDuration arg1 is not equal to the value of arg2 converted
to the data type of arg1 then return the boolean result. Use 'nex' ('!==')
instead if you want true to be returned if arg1 and arg2's data type don't
match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_dayTimeDuration

Determine if for dayTimeDuration arg1 is not equal to the value or data type
of arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
arg2 to be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_dayTimeDuration

This converts an array of dayTimeDuration values that contains one value to a
single dayTimeDuration value.
Converts a one value array to a dayTimeDuration value

=head4 Parameters

    $array


=head3 set_equals_dayTimeDuration

Returns boolean true if dayTimeDuration array1 and dayTimeDuration array2 are
subsets of each other and return the boolean result.
Checks whether two arrays are subsets of each other

=head4 Parameters

    $array1


    $array2


=head3 subset_dayTimeDuration

Returns boolean true if the unique values in dayTimeDuration array1 are all in
dayTimeDuration array2.
Determines if the first array is a subset of second array

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 to_string_dayTimeDuration

Converts dayTimeDuration value to string. For array values, the to_string()
value for each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A dayTimeDuration value.

=head3 union_dayTimeDuration

Returns an array of dayTimeDuration contains all of the unique values in two
or more array of dayTimeDuration values.
Returns union of two or more string arrays

=head4 Parameters

    $arrays

Two or more arrays.

=cut

sub array_dayTimeDuration {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "array_dayTimeDuration");
    $request->set("values", $values);

    return $request->getResult();
}

sub as_dayTimeDuration {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "as_dayTimeDuration");
    $request->set("value", $value);

    return $request->getResult();
}

sub at_least_one_member_of_dayTimeDuration {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<dayTimeDuration>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub bag_dayTimeDuration {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<dayTimeDuration>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_dayTimeDuration {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<dayTimeDuration>");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_dayTimeDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<dayTimeDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_dayTimeDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<dayTimeDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_dayTimeDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<dayTimeDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_dayTimeDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<dayTimeDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub intersection_dayTimeDuration {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<dayTimeDuration>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub is_dayTimeDuration {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is_dayTimeDuration");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_in_dayTimeDuration {
    my ($value, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<dayTimeDuration>");
    $request->set("value", $value);
    $request->set("array", $array);

    return $request->getResult();
}

sub le_dayTimeDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<dayTimeDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_dayTimeDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<dayTimeDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_dayTimeDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<dayTimeDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_dayTimeDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<dayTimeDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_dayTimeDuration {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<dayTimeDuration>");
    $request->set("array", $array);

    return $request->getResult();
}

sub set_equals_dayTimeDuration {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<dayTimeDuration>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub subset_dayTimeDuration {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<dayTimeDuration>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub to_string_dayTimeDuration {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<dayTimeDuration>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_dayTimeDuration {
    my ($arrays) = @_;

    my $request = $session->request()

    $request->set("function" => "union<dayTimeDuration>");
    $request->set("arrays", $arrays);

    return $request->getResult();
}

42;
