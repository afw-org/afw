package afw::expression;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    bag_expression 
    bag_size_expression 
    compile_expression 
    eq_expression 
    eqx_expression 
    evaluate_expression 
    expression 
    ge_expression 
    gt_expression 
    is_expression 
    le_expression 
    lt_expression 
    ne_expression 
    nex_expression 
    to_string_expression 
);

=head1 NAME

afw::expression - Perl module for expression functions

=head2 Functions

The following functions are exported by default

=head3 bag_expression

Takes any number of expression values and returns an array of array.
Makes an array from values

=head4 Parameters

    $values


=head3 bag_size_expression

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 compile_expression

Compile expression value and return either an unevaluated adaptive value or a
string containing the compiler listing.
Compile expression value

=head4 Parameters

    $source

expression string to compile

    $listing

If specified, a compiler listing is produced instead of an unevaluated
expression value.

This parameter can be an integer between 0 and 10 of a string that is used for
indentation. If 0 is specified, no whitespace is added to the resulting
string. If 1 through 10 is specified, that number of spaces is used.

=head3 eq_expression

Determine if expression arg1 is equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'eqx' ('===') instead if
you want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_expression

Determine if for expression arg1 is equal to the value and data type of arg2
then return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 evaluate_expression

Compile and evaluate expression value.
Evaluate expression value

=head4 Parameters

    $source

expression string to compile and evaluate

    $additionalUntrustedQualifiedVariables

This parameter supplies additional qualified variables that can be accessed
during evaluation. These variables will not be used by anything that needs to
ensure its qualified variables must come from a trusted source, such as
authorization. This parameter is intended to be used for testing only and
should not be used for anything running in production.

=head3 expression

Converts value to data type expression returning expression result.
Convert to data type expression

=head4 Parameters

    $value

Value to convert

=head3 ge_expression

Checks for expression arg1 is greater than or equal to expression arg2 and
return the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_expression

Checks for expression arg1 is greater than expression arg2 and return the
boolean result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 is_expression

Checks whether value is dataType expression and return the boolean result.
Checks whether value is dataType expression

=head4 Parameters

    $value

Value to check

=head3 le_expression

Checks for expression arg1 is less than or equal to expression arg2 and return
the boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_expression

Checks for expression arg1 is less that expression arg2 and return the boolean
result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_expression

Determine if expression arg1 is not equal to the value of arg2 converted to
the data type of arg1 then return the boolean result. Use 'nex' ('!==')
instead if you want true to be returned if arg1 and arg2's data type don't
match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_expression

Determine if for expression arg1 is not equal to the value or data type of
arg2 then return the boolean result. Use 'ne' ('!=') instead if you want arg2
to be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 to_string_expression

Converts expression value to string. For array values, the to_string() value
for each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A expression value.

=cut

sub bag_expression {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<expression>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_expression {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<expression>");
    $request->set("value", $value);

    return $request->getResult();
}

sub compile_expression {
    my ($source, $listing) = @_;

    my $request = $session->request()

    $request->set("function" => "compile<expression>");
    $request->set("source", $source);

    if (defined $listing)
        $request->set("listing", $listing);

    return $request->getResult();
}

sub eq_expression {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<expression>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_expression {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<expression>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub evaluate_expression {
    my ($source, $additionalUntrustedQualifiedVariables) = @_;

    my $request = $session->request()

    $request->set("function" => "evaluate<expression>");
    $request->set("source", $source);

    if (defined $additionalUntrustedQualifiedVariables)
        $request->set("additionalUntrustedQualifiedVariables", $additionalUntrustedQualifiedVariables);

    return $request->getResult();
}

sub expression {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "expression");
    $request->set("value", $value);

    return $request->getResult();
}

sub ge_expression {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<expression>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_expression {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<expression>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub is_expression {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<expression>");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_expression {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<expression>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_expression {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<expression>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_expression {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<expression>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_expression {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<expression>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub to_string_expression {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<expression>");
    $request->set("value", $value);

    return $request->getResult();
}

42;
