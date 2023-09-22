package afw::relaxed_json;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    compile_relaxed_json 
    relaxed_json 
);

=head1 NAME

afw::relaxed_json - Perl module for relaxed_json functions

=head2 Functions

The following functions are exported by default

=head3 compile_relaxed_json

Compile relaxed_json value and return either an unevaluated adaptive value or
a string containing the compiler listing.
Compile relaxed_json value

=head4 Parameters

    $source

relaxed_json string to compile

    $listing

If specified, a compiler listing is produced instead of an unevaluated
compiled value.

This parameter can be an integer between 0 and 10 of a string that is used for
indentation. If 0 is specified, no whitespace is added to the resulting
string. If 1 through 10 is specified, that number of spaces is used.

=head3 relaxed_json

Converts value to data type relaxed_json returning relaxed_json result.
Convert to data type relaxed_json

=head4 Parameters

    $value

Value to convert

=cut

sub compile_relaxed_json {
    my ($source, $listing) = @_;

    my $request = $session->request()

    $request->set("function" => "compile<relaxed_json>");
    $request->set("source", $source);

    if (defined $listing)
        $request->set("listing", $listing);

    return $request->getResult();
}

sub relaxed_json {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "relaxed_json");
    $request->set("value", $value);

    return $request->getResult();
}

42;
