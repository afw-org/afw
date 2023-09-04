package afw::hybrid;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    bag_hybrid 
    bag_size_hybrid 
    compile_hybrid 
    eq_hybrid 
    eqx_hybrid 
    evaluate_hybrid 
    ge_hybrid 
    gt_hybrid 
    hybrid 
    is_hybrid 
    le_hybrid 
    lt_hybrid 
    ne_hybrid 
    nex_hybrid 
    to_string_hybrid 
);

=head1 NAME

afw::hybrid - Perl module for hybrid functions

=head2 Functions

The following functions are exported by default

=head3 bag_hybrid

Takes any number of hybrid values and returns an array of array.
Makes an array from values

=head4 Parameters

    $values


=head3 bag_size_hybrid

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 compile_hybrid

Compile hybrid value and return either an unevaluated adaptive value or a
string containing the compiler listing.
Compile hybrid value

=head4 Parameters

    $source

hybrid string to compile

    $listing

If specified, a compiler listing is produced instead of an unevaluated
expression value.

This parameter can be an integer between 0 and 10 of a string that is used for
indentation. If 0 is specified, no whitespace is added to the resulting
string. If 1 through 10 is specified, that number of spaces is used.

=head3 eq_hybrid

Determine if hybrid arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_hybrid

Determine if for hybrid arg1 is equal to the value and data type of arg2 then
return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 evaluate_hybrid

Compile and evaluate hybrid value.
Evaluate hybrid value

=head4 Parameters

    $source

hybrid string to compile and evaluate

    $additionalUntrustedQualifiedVariables

This parameter supplies additional qualified variables that can be accessed
during evaluation. These variables will not be used by anything that needs to
ensure its qualified variables must come from a trusted source, such as
authorization. This parameter is intended to be used for testing only and
should not be used for anything running in production.

=head3 ge_hybrid

Checks for hybrid arg1 is greater than or equal to hybrid arg2 and return the
boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_hybrid

Checks for hybrid arg1 is greater than hybrid arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 hybrid

Converts value to data type hybrid returning hybrid result.
Convert to data type hybrid

=head4 Parameters

    $value

Value to convert

=head3 is_hybrid

Checks whether value is dataType hybrid and return the boolean result.
Checks whether value is dataType hybrid

=head4 Parameters

    $value

Value to check

=head3 le_hybrid

Checks for hybrid arg1 is less than or equal to hybrid arg2 and return the
boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_hybrid

Checks for hybrid arg1 is less that hybrid arg2 and return the boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_hybrid

Determine if hybrid arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead if
you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_hybrid

Determine if for hybrid arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 to_string_hybrid

Converts hybrid value to string. For array values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A hybrid value.

=cut

sub bag_hybrid {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<hybrid>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_hybrid {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<hybrid>");
    $request->set("value", $value);

    return $request->getResult();
}

sub compile_hybrid {
    my ($source, $listing) = @_;

    my $request = $session->request()

    $request->set("function" => "compile<hybrid>");
    $request->set("source", $source);

    if (defined $listing)
        $request->set("listing", $listing);

    return $request->getResult();
}

sub eq_hybrid {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<hybrid>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_hybrid {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<hybrid>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub evaluate_hybrid {
    my ($source, $additionalUntrustedQualifiedVariables) = @_;

    my $request = $session->request()

    $request->set("function" => "evaluate<hybrid>");
    $request->set("source", $source);

    if (defined $additionalUntrustedQualifiedVariables)
        $request->set("additionalUntrustedQualifiedVariables", $additionalUntrustedQualifiedVariables);

    return $request->getResult();
}

sub ge_hybrid {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<hybrid>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_hybrid {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<hybrid>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub hybrid {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "hybrid");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_hybrid {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<hybrid>");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_hybrid {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<hybrid>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_hybrid {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<hybrid>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_hybrid {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<hybrid>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_hybrid {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<hybrid>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub to_string_hybrid {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<hybrid>");
    $request->set("value", $value);

    return $request->getResult();
}

42;
