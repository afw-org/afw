package afw::logical;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    and 
    n_of 
    not 
    or 
);

=head1 NAME

afw::logical - Perl module for logical functions

=head2 Functions

The following functions are exported by default

=head3 and

Evaluates 0 or more boolean conditions returning boolean true if there are no
conditions and boolean false if any condition evaluate to false. All
conditions after the first false remain unevaluated.
Logical and

=head4 Parameters

    $conditions


=head3 n_of

integer n specifies the number of boolean conditions that follow that must
evaluate to true for boolean true to be returned. If n is 0, true is returned.
Once n conditions evaluate to true, true is returned and the remaining
conditions remain unevaluated.
Check for n true values

=head4 Parameters

    $n


    $conditions


=head3 not

Evaluates boolean condition returning boolean true if condition evaluates to
false and false if condition evaluates to true.
Logical not

=head4 Parameters

    $condition


=head3 or

Evaluates 0 or more boolean conditions returning boolean false if there are no
conditions and boolean true if any condition evaluate to true. All conditions
after the first true remain unevaluated.
Logical or

=head4 Parameters

    $conditions


=cut

sub and_ {
    my ($conditions) = @_;

    my $request = $session->request()

    $request->set("function" => "and");
    $request->set("conditions", $conditions);

    return $request->getResult();
}

sub n_of {
    my ($n, $conditions) = @_;

    my $request = $session->request()

    $request->set("function" => "n_of");
    $request->set("n", $n);
    $request->set("conditions", $conditions);

    return $request->getResult();
}

sub not_ {
    my ($condition) = @_;

    my $request = $session->request()

    $request->set("function" => "not");
    $request->set("condition", $condition);

    return $request->getResult();
}

sub or_ {
    my ($conditions) = @_;

    my $request = $session->request()

    $request->set("function" => "or");
    $request->set("conditions", $conditions);

    return $request->getResult();
}

42;
