package afw::xml;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    compile_xml 
    xml 
);

=head1 NAME

afw::xml - Perl module for xml functions

=head2 Functions

The following functions are exported by default

=head3 compile_xml

Compile xml value and return either an unevaluated adaptive value or a string
containing the compiler listing.
Compile xml value

=head4 Parameters

    $source

xml string to compile

    $listing

If specified, a compiler listing is produced instead of an unevaluated
compiled value.

This parameter can be an integer between 0 and 10 of a string that is used for
indentation. If 0 is specified, no whitespace is added to the resulting
string. If 1 through 10 is specified, that number of spaces is used.

=head3 xml

Converts value to data type xml returning xml result.
Convert to data type xml

=head4 Parameters

    $value

Value to convert

=cut

sub compile_xml {
    my ($source, $listing) = @_;

    my $request = $session->request()

    $request->set("function" => "compile<xml>");
    $request->set("source", $source);

    if (defined $listing)
        $request->set("listing", $listing);

    return $request->getResult();
}

sub xml {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "xml");
    $request->set("value", $value);

    return $request->getResult();
}

42;
