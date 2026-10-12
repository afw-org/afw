#!/usr/bin/env perl

#**
# @file unevaluated.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the unevaluated category.
#*

package afw::unevaluated;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    as_unevaluated 
    is_unevaluated 
);

=head1 NAME

afw::unevaluated - Perl module for unevaluated functions

=head2 Functions

The following functions are exported by default

=head3 as_unevaluated

Returns value unchanged if it is dataType unevaluated. Otherwise, including
when value is undefined, throws an error. It never converts: to get a
unevaluated from a value of another data type, write a typed literal or call a
to_ function. This is usually written value as unevaluated.
Asserts that value is dataType unevaluated

=head4 Parameters

    $value

Value to assert

=head3 is_unevaluated

Checks whether value is dataType unevaluated and returns the boolean result.
It is false for undefined. This can also be written `is<unevaluated>(value`).
Checks whether value is dataType unevaluated

=head4 Parameters

    $value

Value to check

=cut

sub as_unevaluated {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "as_unevaluated");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_unevaluated {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is_unevaluated");
    $request->set("value", $value);

    return $request->getResult();
}

42;
