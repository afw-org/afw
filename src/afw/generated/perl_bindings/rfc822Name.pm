#!/usr/bin/env perl

#**
# @file rfc822Name.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the rfc822Name category.
#*

package afw::rfc822Name;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    array_rfc822Name 
    as_rfc822Name 
    at_least_one_member_of_rfc822Name 
    eq_rfc822Name 
    eqx_rfc822Name 
    intersection_rfc822Name 
    is_in_rfc822Name 
    is_rfc822Name 
    ne_rfc822Name 
    nex_rfc822Name 
    one_and_only_rfc822Name 
    regexp_match_rfc822Name 
    rfc822Name_match 
    set_equals_rfc822Name 
    subset_rfc822Name 
    to_string_rfc822Name 
    union_rfc822Name 
);

=head1 NAME

afw::rfc822Name - Perl module for rfc822Name functions

=head2 Functions

The following functions are exported by default

=head3 array_rfc822Name

Construct a new array of rfc822Name from the given values, the same as array()
except that the new array is typed: each value, and each entry of a spread
array, must be dataType rfc822Name, and adding a value of another data type to
the array later throws. A value of another data type is an error, never
converted. This is usually written `array<rfc822Name>(`...values).
Construct an array of rfc822Name from values or spread arrays

=head4 Parameters

    $values

A value of dataType rfc822Name, or ... followed by an expression that results
in an array, every entry of which is included.

=head3 as_rfc822Name

Returns value unchanged if it is dataType rfc822Name. Otherwise, including
when value is undefined, throws an error. It never converts: to get a
rfc822Name from a value of another data type, write a typed literal or call a
to_T conversion function where one exists. This is usually written value as
rfc822Name.
Asserts that value is dataType rfc822Name

=head4 Parameters

    $value

Value to assert

=head3 at_least_one_member_of_rfc822Name

Returns boolean true if at least one value in rfc822Name array1 is in
rfc822Name array2.
Checks for at least one value in common

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 eq_rfc822Name

Determine if rfc822Name arg1 is equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'eqx' ('===') instead if
you want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_rfc822Name

Determine if for rfc822Name arg1 is equal to the value and data type of arg2
then return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 intersection_rfc822Name

Returns an array of rfc822Name with the values that are common to both array
of rfc822Name array1 and array2.
Returns intersection of two arrays

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 is_in_rfc822Name

Checks whether rfc822Name value is in array of rfc822Name array and returns
the boolean result. Empty array yields false.
Checks whether a value is in an array

=head4 Parameters

    $value


    $array


=head3 is_rfc822Name

Checks whether value is dataType rfc822Name and returns the boolean result. It
is false for undefined. This can also be written `is<rfc822Name>(value`).
Checks whether value is dataType rfc822Name

=head4 Parameters

    $value

Value to check

=head3 ne_rfc822Name

Determine if rfc822Name arg1 is not equal to the value of arg2 converted to
the data type of arg1 then return the boolean result. Use 'nex' ('!==')
instead if you want true to be returned if arg1 and arg2's data type don't
match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_rfc822Name

Determine if for rfc822Name arg1 is not equal to the value or data type of
arg2 then return the boolean result. Use 'ne' ('!=') instead if you want arg2
to be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_rfc822Name

This converts an array of rfc822Name values that contains one value to a
single rfc822Name value.
Converts a one value array to a rfc822Name value

=head4 Parameters

    $array


=head3 regexp_match_rfc822Name

Checks whether the regular expression matches anywhere in rfc822Name value and
returns the boolean result. Anchor with ^ and \z to match the whole value.
Search for a match using a regular expression

=head4 Parameters

    $value


    $regexp

The regular expression. A regexp literal (/a+b/ or "a+b"re), to_regexp(s), or
`compile<regexp>(r`) is already compiled; any other regexp value is compiled
for this call.

=head3 rfc822Name_match

Returns true if value matches pattern, a complete or partial rfc822Name.

If pattern is a complete mailbox, such as "Anderson@sun.com", it matches that
mailbox: the local part exactly and the domain part ignoring case.

If pattern is a domain, such as "sun.com", it matches every mailbox at exactly
that domain (ignoring case), not at its subdomains.

If pattern is a domain that starts with ".", such as ".east.sun.com", it
matches every mailbox in a subdomain of it, such as "Anderson@a.east.sun.com",
but not at "east.sun.com" itself.
Checks if an rfc822Name matches a complete or partial pattern

=head4 Parameters

    $pattern

A mailbox, a domain, or a domain that starts with ".".

    $value

The rfc822Name to check.

=head3 set_equals_rfc822Name

Returns boolean true if rfc822Name array1 and rfc822Name array2 are subsets of
each other and return the boolean result.
Checks whether two arrays are subsets of each other

=head4 Parameters

    $array1


    $array2


=head3 subset_rfc822Name

Returns boolean true if the unique values in rfc822Name array1 are all in
rfc822Name array2.
Determines if the first array is a subset of second array

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 to_string_rfc822Name

Converts rfc822Name value to string. For array values, the to_string() value
for each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A rfc822Name value.

=head3 union_rfc822Name

Returns an array of rfc822Name contains all of the unique values in two or
more array of rfc822Name values.
Returns union of two or more string arrays

=head4 Parameters

    $arrays

Two or more arrays.

=cut

sub array_rfc822Name {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "array_rfc822Name");
    $request->set("values", $values);

    return $request->getResult();
}

sub as_rfc822Name {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "as_rfc822Name");
    $request->set("value", $value);

    return $request->getResult();
}

sub at_least_one_member_of_rfc822Name {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<rfc822Name>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub eq_rfc822Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<rfc822Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_rfc822Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<rfc822Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub intersection_rfc822Name {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<rfc822Name>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub is_in_rfc822Name {
    my ($value, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<rfc822Name>");
    $request->set("value", $value);
    $request->set("array", $array);

    return $request->getResult();
}

sub is_rfc822Name {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is_rfc822Name");
    $request->set("value", $value);

    return $request->getResult();
}

sub ne_rfc822Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<rfc822Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_rfc822Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<rfc822Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_rfc822Name {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<rfc822Name>");
    $request->set("array", $array);

    return $request->getResult();
}

sub regexp_match_rfc822Name {
    my ($value, $regexp) = @_;

    my $request = $session->request()

    $request->set("function" => "regexp_match<rfc822Name>");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

    return $request->getResult();
}

sub rfc822Name_match {
    my ($pattern, $value) = @_;

    my $request = $session->request()

    $request->set("function" => "rfc822Name_match");
    $request->set("pattern", $pattern);
    $request->set("value", $value);

    return $request->getResult();
}

sub set_equals_rfc822Name {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<rfc822Name>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub subset_rfc822Name {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<rfc822Name>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub to_string_rfc822Name {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<rfc822Name>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_rfc822Name {
    my ($arrays) = @_;

    my $request = $session->request()

    $request->set("function" => "union<rfc822Name>");
    $request->set("arrays", $arrays);

    return $request->getResult();
}

42;
