#!/usr/bin/env perl

#**
# @file dnsName.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the dnsName category.
#*

package afw::dnsName;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    array_dnsName 
    as_dnsName 
    eq_dnsName 
    eqx_dnsName 
    is_dnsName 
    is_in_dnsName 
    ne_dnsName 
    nex_dnsName 
    one_and_only_dnsName 
    regexp_match_dnsName 
    to_string_dnsName 
);

=head1 NAME

afw::dnsName - Perl module for dnsName functions

=head2 Functions

The following functions are exported by default

=head3 array_dnsName

Construct a new array of dnsName from the given values, the same as array()
except that the new array is typed: each value, and each entry of a spread
array, must be dataType dnsName, and adding a value of another data type to
the array later throws. A value of another data type is an error, never
converted. This is usually written `array<dnsName>(`...values).
Construct an array of dnsName from values or spread arrays

=head4 Parameters

    $values

A value of dataType dnsName, or ... followed by an expression that results in
an array, every entry of which is included.

=head3 as_dnsName

Returns value unchanged if it is dataType dnsName. Otherwise, including when
value is undefined, throws an error. It never converts: to get a dnsName from
a value of another data type, write a typed literal or call a to_T conversion
function where one exists. This is usually written value as dnsName.
Asserts that value is dataType dnsName

=head4 Parameters

    $value

Value to assert

=head3 eq_dnsName

Determine if dnsName arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_dnsName

Determine if for dnsName arg1 is equal to the value and data type of arg2 then
return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 is_dnsName

Checks whether value is dataType dnsName and returns the boolean result. It is
false for undefined. This can also be written `is<dnsName>(value`).
Checks whether value is dataType dnsName

=head4 Parameters

    $value

Value to check

=head3 is_in_dnsName

Checks whether dnsName value is in array of dnsName array and returns the
boolean result. Empty array yields false.
Checks whether a value is in an array

=head4 Parameters

    $value


    $array


=head3 ne_dnsName

Determine if dnsName arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead if
you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_dnsName

Determine if for dnsName arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_dnsName

This converts an array of dnsName values that contains one value to a single
dnsName value.
Converts a one value array to a dnsName value

=head4 Parameters

    $array


=head3 regexp_match_dnsName

Checks whether dnsName value matches the regular expression regexp and return
the boolean result.
Search for a match using a regular expression

=head4 Parameters

    $value


    $regexp


=head3 to_string_dnsName

Converts dnsName value to string. For array values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A dnsName value.

=cut

sub array_dnsName {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "array_dnsName");
    $request->set("values", $values);

    return $request->getResult();
}

sub as_dnsName {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "as_dnsName");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_dnsName {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<dnsName>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_dnsName {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<dnsName>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub is_dnsName {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is_dnsName");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_in_dnsName {
    my ($value, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<dnsName>");
    $request->set("value", $value);
    $request->set("array", $array);

    return $request->getResult();
}

sub ne_dnsName {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<dnsName>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_dnsName {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<dnsName>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_dnsName {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<dnsName>");
    $request->set("array", $array);

    return $request->getResult();
}

sub regexp_match_dnsName {
    my ($value, $regexp) = @_;

    my $request = $session->request()

    $request->set("function" => "regexp_match<dnsName>");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

    return $request->getResult();
}

sub to_string_dnsName {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<dnsName>");
    $request->set("value", $value);

    return $request->getResult();
}

42;
