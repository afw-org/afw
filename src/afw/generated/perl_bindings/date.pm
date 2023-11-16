#!/usr/bin/env perl

#**
# @file date.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the date category.
#*

package afw::date;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    add_yearMonthDuration_date 
    at_least_one_member_of_date 
    bag_date 
    bag_size_date 
    date 
    eq_date 
    eqx_date 
    ge_date 
    gt_date 
    intersection_date 
    is_date 
    is_in_date 
    le_date 
    lt_date 
    max_date 
    min_date 
    ne_date 
    nex_date 
    one_and_only_date 
    set_equals_date 
    subset_date 
    subtract_yearMonthDuration_date 
    to_string_date 
    union_date 
);

=head1 NAME

afw::date - Perl module for date functions

=head2 Functions

The following functions are exported by default

=head3 add_yearMonthDuration_date

Add date arg1 to yearMonthDuration arg2 and return the date result.
Add duration

=head4 Parameters

    $arg1


    $arg2


=head3 at_least_one_member_of_date

Returns boolean true if at least one value in date array1 is in date array2.
Checks for at least one value in common

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 bag_date

Takes any number of date values and returns an array of array.
Makes an array from values

=head4 Parameters

    $values


=head3 bag_size_date

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 date

Converts value to data type date returning date result.
Convert to data type date

=head4 Parameters

    $value

Value to convert

=head3 eq_date

Determine if date arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_date

Determine if for date arg1 is equal to the value and data type of arg2 then
return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_date

Checks for date arg1 is greater than or equal to date arg2 and return the
boolean result.

If a date value does not include a time-zone value, then the local time-zone
value will be assigned.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_date

Checks for date arg1 is greater than date arg2 and return the boolean result.

If a date value does not include a time-zone value, then the local time-zone
value will be assigned.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 intersection_date

Returns an array of date with the values that are common to both array of date
array1 and array2.
Returns intersection of two arrays

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 is_date

Checks whether value is dataType date and return the boolean result.
Checks whether value is dataType date

=head4 Parameters

    $value

Value to check

=head3 is_in_date

Checks whether date value is in array of date array and returns the boolean
result.
Checks whether a value is in an array

=head4 Parameters

    $value


    $array


=head3 le_date

Checks for date arg1 is less than or equal to date arg2 and return the boolean
result.

If a date value does not include a time-zone value, then the local time-zone
value will be assigned.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_date

Checks for date arg1 is less that date arg2 and return the boolean result.

If a date value does not include a time-zone value, then the local time-zone
value will be assigned.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 max_date

Return the date value that is greater than or equal to the others..

If a date value does not include a time-zone value, then the local time-zone
value will be assigned.
Maximum value

=head4 Parameters

    $values


=head3 min_date

Return the date value that is less than or equal to the others..

If a date value does not include a time-zone value, then the local time-zone
value will be assigned.
Minimum value

=head4 Parameters

    $values


=head3 ne_date

Determine if date arg1 is not equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'nex' ('!==') instead if you
want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_date

Determine if for date arg1 is not equal to the value or data type of arg2 then
return the boolean result. Use 'ne' ('!=') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_date

This converts an array of date values that contains one value to a single date
value.
Converts a one value array to a date value

=head4 Parameters

    $array


=head3 set_equals_date

Returns boolean true if date array1 and date array2 are subsets of each other
and return the boolean result.
Checks whether two arrays are subsets of each other

=head4 Parameters

    $array1


    $array2


=head3 subset_date

Returns boolean true if the unique values in date array1 are all in date
array2.
Determines if the first array is a subset of second array

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 subtract_yearMonthDuration_date

Subtract yearMonthDuration arg2 from date arg1 and return the date result.
Subtract numbers

=head4 Parameters

    $arg1


    $arg2


=head3 to_string_date

Converts date value to string. For array values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A date value.

=head3 union_date

Returns an array of date contains all of the unique values in two or more
array of date values.
Returns union of two or more string arrays

=head4 Parameters

    $arrays

Two or more arrays.

=cut

sub add_yearMonthDuration_date {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "add_yearMonthDuration<date>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub at_least_one_member_of_date {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<date>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub bag_date {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<date>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_date {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<date>");
    $request->set("value", $value);

    return $request->getResult();
}

sub date {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "date");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_date {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<date>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_date {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<date>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_date {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<date>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_date {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<date>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub intersection_date {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<date>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub is_date {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<date>");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_in_date {
    my ($value, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<date>");
    $request->set("value", $value);
    $request->set("array", $array);

    return $request->getResult();
}

sub le_date {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<date>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_date {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<date>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub max_date {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "max<date>");
    $request->set("values", $values);

    return $request->getResult();
}

sub min_date {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "min<date>");
    $request->set("values", $values);

    return $request->getResult();
}

sub ne_date {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<date>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_date {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<date>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_date {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<date>");
    $request->set("array", $array);

    return $request->getResult();
}

sub set_equals_date {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<date>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub subset_date {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<date>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub subtract_yearMonthDuration_date {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "subtract_yearMonthDuration<date>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub to_string_date {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<date>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_date {
    my ($arrays) = @_;

    my $request = $session->request()

    $request->set("function" => "union<date>");
    $request->set("arrays", $arrays);

    return $request->getResult();
}

42;
