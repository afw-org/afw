#!/usr/bin/env perl

#**
# @file null.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the null category.
#*

package afw::null;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    bag_null 
    bag_size_null 
    is_null 
    null 
    to_string_null 
);

=head1 NAME

afw::null - Perl module for null functions

=head2 Functions

The following functions are exported by default

=head3 bag_null

Takes any number of null values and returns an array of array.
Makes an array from values

=head4 Parameters

    $values


=head3 bag_size_null

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 is_null

Checks whether value is dataType null and return the boolean result.
Checks whether value is dataType null

=head4 Parameters

    $value

Value to check

=head3 null

Converts value to data type null returning null result.
Convert to data type null

=head4 Parameters

    $value

Value to convert

=head3 to_string_null

Converts null value to string. For array values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A null value.

=cut

sub bag_null {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<null>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_null {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<null>");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_null {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<null>");
    $request->set("value", $value);

    return $request->getResult();
}

sub null {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "null");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_string_null {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<null>");
    $request->set("value", $value);

    return $request->getResult();
}

42;
