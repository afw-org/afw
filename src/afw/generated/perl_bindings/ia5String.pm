#!/usr/bin/env perl

#**
# @file ia5String.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the ia5String category.
#*

package afw::ia5String;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    bag_ia5String 
    bag_size_ia5String 
    eq_ia5String 
    eqx_ia5String 
    ge_ia5String 
    gt_ia5String 
    ia5String 
    is_ia5String 
    le_ia5String 
    lt_ia5String 
    ne_ia5String 
    nex_ia5String 
    to_string_ia5String 
);

=head1 NAME

afw::ia5String - Perl module for ia5String functions

=head2 Functions

The following functions are exported by default

=head3 bag_ia5String

Takes any number of ia5String values and returns an array of array.
Makes an array from values

=head4 Parameters

    $values


=head3 bag_size_ia5String

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 eq_ia5String

Determine if ia5String arg1 is equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'eqx' ('===') instead if
you want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_ia5String

Determine if for ia5String arg1 is equal to the value and data type of arg2
then return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_ia5String

Checks for ia5String arg1 is greater than or equal to ia5String arg2 and
return the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_ia5String

Checks for ia5String arg1 is greater than ia5String arg2 and return the
boolean result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 ia5String

Converts value to data type ia5String returning ia5String result.
Convert to data type ia5String

=head4 Parameters

    $value

Value to convert

=head3 is_ia5String

Checks whether value is dataType ia5String and return the boolean result.
Checks whether value is dataType ia5String

=head4 Parameters

    $value

Value to check

=head3 le_ia5String

Checks for ia5String arg1 is less than or equal to ia5String arg2 and return
the boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_ia5String

Checks for ia5String arg1 is less that ia5String arg2 and return the boolean
result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_ia5String

Determine if ia5String arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead if
you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_ia5String

Determine if for ia5String arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 to_string_ia5String

Converts ia5String value to string. For array values, the to_string() value
for each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A ia5String value.

=cut

sub bag_ia5String {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<ia5String>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_ia5String {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<ia5String>");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_ia5String {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<ia5String>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_ia5String {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<ia5String>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_ia5String {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<ia5String>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_ia5String {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<ia5String>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ia5String {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "ia5String");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_ia5String {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<ia5String>");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_ia5String {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<ia5String>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_ia5String {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<ia5String>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_ia5String {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<ia5String>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_ia5String {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<ia5String>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub to_string_ia5String {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<ia5String>");
    $request->set("value", $value);

    return $request->getResult();
}

42;
