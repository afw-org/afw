package afw::regexp;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    compile_regexp 
    eq_regexp 
    eqx_regexp 
    ge_regexp 
    gt_regexp 
    le_regexp 
    lt_regexp 
    ne_regexp 
    nex_regexp 
    regexp 
);

=head1 NAME

afw::regexp - Perl module for regexp functions

=head2 Functions

The following functions are exported by default

=head3 compile_regexp

Compile regexp value and return either an unevaluated adaptive value or a
string containing the compiler listing.
Compile regexp value

=head4 Parameters

    $source

regexp string to compile

    $listing

If specified, a compiler listing is produced instead of an unevaluated
compiled value.

This parameter can be an integer between 0 and 10 of a string that is used for
indentation. If 0 is specified, no whitespace is added to the resulting
string. If 1 through 10 is specified, that number of spaces is used.

=head3 eq_regexp

Determine if regexp arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_regexp

Determine if for regexp arg1 is equal to the value and data type of arg2 then
return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_regexp

Checks for regexp arg1 is greater than or equal to regexp arg2 and return the
boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_regexp

Checks for regexp arg1 is greater than regexp arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 le_regexp

Checks for regexp arg1 is less than or equal to regexp arg2 and return the
boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_regexp

Checks for regexp arg1 is less that regexp arg2 and return the boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_regexp

Determine if regexp arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead if
you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_regexp

Determine if for regexp arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 regexp

Converts value to data type regexp returning regexp result.
Convert to data type regexp

=head4 Parameters

    $value

Value to convert

=cut

sub compile_regexp {
    my ($source, $listing) = @_;

    my $request = $session->request()

    $request->set("function" => "compile<regexp>");
    $request->set("source", $source);

    if (defined $listing)
        $request->set("listing", $listing);

    return $request->getResult();
}

sub eq_regexp {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<regexp>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_regexp {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<regexp>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_regexp {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<regexp>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_regexp {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<regexp>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub le_regexp {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<regexp>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_regexp {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<regexp>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_regexp {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<regexp>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_regexp {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<regexp>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub regexp {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "regexp");
    $request->set("value", $value);

    return $request->getResult();
}

42;
