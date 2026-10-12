#!/usr/bin/env perl

#**
# @file relaxedJson.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the relaxedJson category.
#*

package afw::relaxedJson;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    compile_relaxedJson 
    relaxedJson 
);

=head1 NAME

afw::relaxedJson - Perl module for relaxedJson functions

=head2 Functions

The following functions are exported by default

=head3 compile_relaxedJson

Compile relaxedJson value and return either an unevaluated adaptive value or a
string containing the compiler listing. The listing is a human-oriented dump
(value tree interleaved with source, plus ---Symbols tables) for Fiddle and
debugging — not pure JSON (use stringify) and not Adaptive compiled-form text
(use decompile).
Compile relaxedJson value

=head4 Parameters

    $source

relaxedJson string to compile

    $listing

If specified, a human compiler listing is produced instead of an unevaluated
compiled value (tree + ---Symbols; not recompilable). Use decompile() for
Adaptive compiled-form text and stringify() for pure JSON of evaluated data.

This parameter can be an integer between 0 and 10 or a string that is used for
indentation. If 0 is specified, no whitespace is added to the resulting
string. If 1 through 10 is specified, that number of spaces is used.

=head3 relaxedJson

Converts value to data type relaxedJson returning relaxedJson result. Holds
relaxed JSON source text as a relaxedJson value (does not parse/compile). Use
polymorphic compile to compile the source.
Convert to data type relaxedJson

=head4 Parameters

    $value

Value to convert

=cut

sub compile_relaxedJson {
    my ($source, $listing) = @_;

    my $request = $session->request()

    $request->set("function" => "compile<relaxedJson>");
    $request->set("source", $source);

    if (defined $listing)
        $request->set("listing", $listing);

    return $request->getResult();
}

sub relaxedJson {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "relaxedJson");
    $request->set("value", $value);

    return $request->getResult();
}

42;
