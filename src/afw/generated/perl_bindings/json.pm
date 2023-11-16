#!/usr/bin/env perl

#**
# @file json.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the json category.
#*

package afw::json;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    compile_json 
    json 
);

=head1 NAME

afw::json - Perl module for json functions

=head2 Functions

The following functions are exported by default

=head3 compile_json

Compile json value and return either an unevaluated adaptive value or a string
containing the compiler listing.
Compile json value

=head4 Parameters

    $source

json string to compile

    $listing

If specified, a compiler listing is produced instead of an unevaluated
compiled value.

This parameter can be an integer between 0 and 10 of a string that is used for
indentation. If 0 is specified, no whitespace is added to the resulting
string. If 1 through 10 is specified, that number of spaces is used.

=head3 json

Converts value to data type json returning json result.
Convert to data type json

=head4 Parameters

    $value

Value to convert

=cut

sub compile_json {
    my ($source, $listing) = @_;

    my $request = $session->request()

    $request->set("function" => "compile<json>");
    $request->set("source", $source);

    if (defined $listing)
        $request->set("listing", $listing);

    return $request->getResult();
}

sub json {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "json");
    $request->set("value", $value);

    return $request->getResult();
}

42;
