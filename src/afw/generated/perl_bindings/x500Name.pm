#!/usr/bin/env perl

#**
# @file x500Name.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the x500Name category.
#*

package afw::x500Name;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    array_x500Name 
    as_x500Name 
    at_least_one_member_of_x500Name 
    eq_x500Name 
    eqx_x500Name 
    intersection_x500Name 
    is_in_x500Name 
    is_x500Name 
    ne_x500Name 
    nex_x500Name 
    one_and_only_x500Name 
    regexp_match_x500Name 
    set_equals_x500Name 
    subset_x500Name 
    to_string_x500Name 
    union_x500Name 
    x500Name_match 
);

=head1 NAME

afw::x500Name - Perl module for x500Name functions

=head2 Functions

The following functions are exported by default

=head3 array_x500Name

Construct a new array of x500Name from the given values, the same as array()
except that the new array is typed: each value, and each entry of a spread
array, must be dataType x500Name, and adding a value of another data type to
the array later throws. A value of another data type is an error, never
converted. This is usually written `array<x500Name>(`...values).
Construct an array of x500Name from values or spread arrays

=head4 Parameters

    $values

A value of dataType x500Name, or ... followed by an expression that results in
an array, every entry of which is included.

=head3 as_x500Name

Returns value unchanged if it is dataType x500Name. Otherwise, including when
value is undefined, throws an error. It never converts: to get a x500Name from
a value of another data type, write a typed literal or call a to_T conversion
function where one exists. This is usually written value as x500Name.
Asserts that value is dataType x500Name

=head4 Parameters

    $value

Value to assert

=head3 at_least_one_member_of_x500Name

Returns boolean true if at least one value in x500Name array1 is in x500Name
array2.
Checks for at least one value in common

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 eq_x500Name

Determine if x500Name arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_x500Name

Determine if for x500Name arg1 is equal to the value and data type of arg2
then return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 intersection_x500Name

Returns an array of x500Name with the values that are common to both array of
x500Name array1 and array2.
Returns intersection of two arrays

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 is_in_x500Name

Checks whether x500Name value is in array of x500Name array and returns the
boolean result. Empty array yields false.
Checks whether a value is in an array

=head4 Parameters

    $value


    $array


=head3 is_x500Name

Checks whether value is dataType x500Name and returns the boolean result. It
is false for undefined. This can also be written `is<x500Name>(value`).
Checks whether value is dataType x500Name

=head4 Parameters

    $value

Value to check

=head3 ne_x500Name

Determine if x500Name arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead if
you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_x500Name

Determine if for x500Name arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_x500Name

This converts an array of x500Name values that contains one value to a single
x500Name value.
Converts a one value array to a x500Name value

=head4 Parameters

    $array


=head3 regexp_match_x500Name

Checks whether x500Name value matches the regular expression regexp and return
the boolean result.
Search for a match using a regular expression

=head4 Parameters

    $value


    $regexp

The regular expression. A regexp literal ("a+b"re) or a `compile<regexp>`
result is already compiled; any other regexp value is compiled for this call.

=head3 set_equals_x500Name

Returns boolean true if x500Name array1 and x500Name array2 are subsets of
each other and return the boolean result.
Checks whether two arrays are subsets of each other

=head4 Parameters

    $array1


    $array2


=head3 subset_x500Name

Returns boolean true if the unique values in x500Name array1 are all in
x500Name array2.
Determines if the first array is a subset of second array

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 to_string_x500Name

Converts x500Name value to string. For array values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A x500Name value.

=head3 union_x500Name

Returns an array of x500Name contains all of the unique values in two or more
array of x500Name values.
Returns union of two or more string arrays

=head4 Parameters

    $arrays

Two or more arrays.

=head3 x500Name_match

Returns true if the RDNs of pattern are the last RDNs of value, compared as
x500Name equality compares them. For example, "O=Medico Corp,C=US"x500Name
matches "cn=John Smith,o=Medico Corp, c=US"x500Name.
Checks if an x500Name ends with the RDNs of another

=head4 Parameters

    $pattern

The RDNs value must end with.

    $value

The x500Name to check.

=cut

sub array_x500Name {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "array_x500Name");
    $request->set("values", $values);

    return $request->getResult();
}

sub as_x500Name {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "as_x500Name");
    $request->set("value", $value);

    return $request->getResult();
}

sub at_least_one_member_of_x500Name {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<x500Name>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub eq_x500Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<x500Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_x500Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<x500Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub intersection_x500Name {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<x500Name>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub is_in_x500Name {
    my ($value, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<x500Name>");
    $request->set("value", $value);
    $request->set("array", $array);

    return $request->getResult();
}

sub is_x500Name {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is_x500Name");
    $request->set("value", $value);

    return $request->getResult();
}

sub ne_x500Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<x500Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_x500Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<x500Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_x500Name {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<x500Name>");
    $request->set("array", $array);

    return $request->getResult();
}

sub regexp_match_x500Name {
    my ($value, $regexp) = @_;

    my $request = $session->request()

    $request->set("function" => "regexp_match<x500Name>");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

    return $request->getResult();
}

sub set_equals_x500Name {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<x500Name>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub subset_x500Name {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<x500Name>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub to_string_x500Name {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<x500Name>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_x500Name {
    my ($arrays) = @_;

    my $request = $session->request()

    $request->set("function" => "union<x500Name>");
    $request->set("arrays", $arrays);

    return $request->getResult();
}

sub x500Name_match {
    my ($pattern, $value) = @_;

    my $request = $session->request()

    $request->set("function" => "x500Name_match");
    $request->set("pattern", $pattern);
    $request->set("value", $value);

    return $request->getResult();
}

42;
