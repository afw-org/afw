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
    array_null 
    as_null 
    is_null 
    to_string_null 
);

=head1 NAME

afw::null - Perl module for null functions

=head2 Functions

The following functions are exported by default

=head3 array_null

Construct a new array of null from the given values, the same as array()
except that the new array is typed: each value, and each entry of a spread
array, must be dataType null, and adding a value of another data type to the
array later throws. A value of another data type is an error, never converted.
This is usually written `array<null>(`...values).
Construct an array of null from values or spread arrays

=head4 Parameters

    $values

A value of dataType null, or ... followed by an expression that results in an
array, every entry of which is included.

=head3 as_null

Returns value unchanged if it is dataType null. Otherwise, including when
value is undefined, throws an error. It never converts: to get a null from a
value of another data type, write a typed literal or call a to_T conversion
function where one exists. This is usually written value as null.
Asserts that value is dataType null

=head4 Parameters

    $value

Value to assert

=head3 is_null

Checks whether value is dataType null and returns the boolean result. It is
false for undefined. This can also be written `is<null>(value`).
Checks whether value is dataType null

=head4 Parameters

    $value

Value to check

=head3 to_string_null

Converts null value to string. For array values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A null value.

=cut

sub array_null {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "array_null");
    $request->set("values", $values);

    return $request->getResult();
}

sub as_null {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "as_null");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_null {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is_null");
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
