package afw::value;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    meta 
    metas 
);

=head1 NAME

afw::value - Perl module for value functions

=head2 Functions

The following functions are exported by default

=head3 meta

This function returns an object containing meta information for an adaptive
value.
Return the meta object about an adaptive value

=head4 Parameters

    $value

This is the adaptive value whose meta object is to be returned.

=head3 metas

This function returns a list of the result of calling meta() for each value
in a list or object adaptive value.
Return a list of the result of calling meta() for each value in a list or object adaptive value

=head4 Parameters

    $value

This is an adaptive list or object.

=cut

sub meta {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "meta");
    $request->set("value", $value);

    return $request->getResult();
}

sub metas {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "metas");
    $request->set("value", $value);

    return $request->getResult();
}

42;
