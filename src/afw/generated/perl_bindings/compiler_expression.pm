#!/usr/bin/env perl

#**
# @file compiler_expression.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the compiler_expression category.
#*

package afw::compiler_expression;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    nullish_coalescing 
    optional_chaining 
    void_operator 
);

=head1 NAME

afw::compiler_expression - Perl module for compiler_expression functions

=head2 Functions

The following functions are exported by default

=head3 nullish_coalescing

Returns the first value of values that is not null or undefined leaving the
remaining values unevaluated.
Nullish coalescing

=head4 Parameters

    $values


=head3 optional_chaining

Returns undefined if arg1 is null or undefined without evaluating arg2, but
otherwise returns evaluated value of arg2.
Optional chaining

=head4 Parameters

    $arg1


    $arg2


=head3 void_operator

This is the support function for the void operator which evaluates value and
returns undefined.
Support for the void operator

=head4 Parameters

    $value

This is the value to evaluate.

=cut

sub nullish_coalescing {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "nullish_coalescing");
    $request->set("values", $values);

    return $request->getResult();
}

sub optional_chaining {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "optional_chaining");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub void_operator {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "void_operator");
    $request->set("value", $value);

    return $request->getResult();
}

42;
