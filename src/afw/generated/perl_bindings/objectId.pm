#!/usr/bin/env perl

#**
# @file objectId.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the objectId category.
#*

package afw::objectId;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    array_objectId 
    as_objectId 
    eq_objectId 
    eqx_objectId 
    ge_objectId 
    gt_objectId 
    is_objectId 
    le_objectId 
    lt_objectId 
    ne_objectId 
    nex_objectId 
    to_string_objectId 
);

=head1 NAME

afw::objectId - Perl module for objectId functions

=head2 Functions

The following functions are exported by default

=head3 array_objectId

Construct a new array of objectId from the given values, the same as array()
except that the new array is typed: each value, and each entry of a spread
array, must be dataType objectId, and adding a value of another data type to
the array later throws. A value of another data type is an error, never
converted. This is usually written `array<objectId>(`...values).
Construct an array of objectId from values or spread arrays

=head4 Parameters

    $values

A value of dataType objectId, or ... followed by an expression that results in
an array, every entry of which is included.

=head3 as_objectId

Returns value unchanged if it is dataType objectId. Otherwise, including when
value is undefined, throws an error. It never converts: to get a objectId from
a value of another data type, write a typed literal or call a to_T conversion
function where one exists. This is usually written value as objectId.
Asserts that value is dataType objectId

=head4 Parameters

    $value

Value to assert

=head3 eq_objectId

Determine if objectId arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_objectId

Determine if for objectId arg1 is equal to the value and data type of arg2
then return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_objectId

Checks for objectId arg1 is greater than or equal to objectId arg2 and return
the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_objectId

Checks for objectId arg1 is greater than objectId arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 is_objectId

Checks whether value is dataType objectId and returns the boolean result. It
is false for undefined. This can also be written `is<objectId>(value`).
Checks whether value is dataType objectId

=head4 Parameters

    $value

Value to check

=head3 le_objectId

Checks for objectId arg1 is less than or equal to objectId arg2 and return the
boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_objectId

Checks for objectId arg1 is less that objectId arg2 and return the boolean
result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_objectId

Determine if objectId arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead if
you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_objectId

Determine if for objectId arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 to_string_objectId

Converts objectId value to string. For array values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A objectId value.

=cut

sub array_objectId {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "array_objectId");
    $request->set("values", $values);

    return $request->getResult();
}

sub as_objectId {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "as_objectId");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_objectId {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<objectId>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_objectId {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<objectId>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_objectId {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<objectId>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_objectId {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<objectId>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub is_objectId {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is_objectId");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_objectId {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<objectId>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_objectId {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<objectId>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_objectId {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<objectId>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_objectId {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<objectId>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub to_string_objectId {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<objectId>");
    $request->set("value", $value);

    return $request->getResult();
}

42;
