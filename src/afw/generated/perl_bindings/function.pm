package afw::function;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    bag_function 
    bag_size_function 
    eq_function 
    eqx_function 
    function 
    ge_function 
    gt_function 
    is_function 
    le_function 
    lt_function 
    ne_function 
    nex_function 
);

=head1 NAME

afw::function - Perl module for function functions

=head2 Functions

The following functions are exported by default

=head3 bag_function

Takes any number of function values and returns an array of array.
Makes an array from values

=head4 Parameters

    $values


=head3 bag_size_function

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 eq_function

Determine if function arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_function

Determine if for function arg1 is equal to the value and data type of arg2
then return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 function

Converts value to data type function returning function result.


=head4 Parameters

    $value

Value to convert

=head3 ge_function

Checks for function arg1 is greater than or equal to function arg2 and return
the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_function

Checks for function arg1 is greater than function arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 is_function

Checks whether value is dataType function and return the boolean result.
Checks whether value is dataType function

=head4 Parameters

    $value

Value to check

=head3 le_function

Checks for function arg1 is less than or equal to function arg2 and return the
boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_function

Checks for function arg1 is less that function arg2 and return the boolean
result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_function

Determine if function arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead if
you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_function

Determine if for function arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=cut

sub bag_function {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<function>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_function {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<function>");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_function {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<function>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_function {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<function>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub function {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "function");
    $request->set("value", $value);

    return $request->getResult();
}

sub ge_function {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<function>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_function {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<function>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub is_function {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<function>");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_function {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<function>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_function {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<function>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_function {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<function>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_function {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<function>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

42;
