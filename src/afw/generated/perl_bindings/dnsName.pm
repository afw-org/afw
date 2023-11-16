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
    bag_dnsName 
    bag_size_dnsName 
    dnsName 
    eq_dnsName 
    eqx_dnsName 
    ge_dnsName 
    gt_dnsName 
    is_dnsName 
    is_in_dnsName 
    le_dnsName 
    lt_dnsName 
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

=head3 bag_dnsName

Takes any number of dnsName values and returns an array of array.
Makes an array from values

=head4 Parameters

    $values


=head3 bag_size_dnsName

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 dnsName

Converts value to data type dnsName returning dnsName result.
Convert to data type dnsName

=head4 Parameters

    $value

Value to convert

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


=head3 ge_dnsName

Checks for dnsName arg1 is greater than or equal to dnsName arg2 and return
the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_dnsName

Checks for dnsName arg1 is greater than dnsName arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 is_dnsName

Checks whether value is dataType dnsName and return the boolean result.
Checks whether value is dataType dnsName

=head4 Parameters

    $value

Value to check

=head3 is_in_dnsName

Checks whether dnsName value is in array of dnsName array and returns the
boolean result.
Checks whether a value is in an array

=head4 Parameters

    $value


    $array


=head3 le_dnsName

Checks for dnsName arg1 is less than or equal to dnsName arg2 and return the
boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_dnsName

Checks for dnsName arg1 is less that dnsName arg2 and return the boolean
result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


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

sub bag_dnsName {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<dnsName>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_dnsName {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<dnsName>");
    $request->set("value", $value);

    return $request->getResult();
}

sub dnsName {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "dnsName");
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

sub ge_dnsName {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<dnsName>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_dnsName {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<dnsName>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub is_dnsName {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<dnsName>");
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

sub le_dnsName {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<dnsName>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_dnsName {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<dnsName>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

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
