package afw::unevaluated;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    is_unevaluated 
);

=head1 NAME

afw::unevaluated - Perl module for unevaluated functions

=head2 Functions

The following functions are exported by default

=head3 is_unevaluated

Checks whether value is dataType unevaluated and return the boolean result.
Checks whether value is dataType unevaluated

=head4 Parameters

    $value

Value to check

=cut

sub is_unevaluated {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<unevaluated>");
    $request->set("value", $value);

    return $request->getResult();
}

42;
