package afw::objectPath;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    bag_objectPath 
    bag_size_objectPath 
    eq_objectPath 
    eqx_objectPath 
    ge_objectPath 
    gt_objectPath 
    is_objectPath 
    le_objectPath 
    lt_objectPath 
    ne_objectPath 
    nex_objectPath 
    objectPath 
    to_string_objectPath 
);

=head1 NAME

afw::objectPath - Perl module for objectPath functions

=head2 Functions

The following functions are exported by default

=head3 bag_objectPath

Takes any number of objectPath values and returns an array of array.
Makes an array from values

=head4 Parameters

    $values


=head3 bag_size_objectPath

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 eq_objectPath

Determine if objectPath arg1 is equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'eqx' ('===') instead if
you want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_objectPath

Determine if for objectPath arg1 is equal to the value and data type of arg2
then return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_objectPath

Checks for objectPath arg1 is greater than or equal to objectPath arg2 and
return the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_objectPath

Checks for objectPath arg1 is greater than objectPath arg2 and return the
boolean result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 is_objectPath

Checks whether value is dataType objectPath and return the boolean result.
Checks whether value is dataType objectPath

=head4 Parameters

    $value

Value to check

=head3 le_objectPath

Checks for objectPath arg1 is less than or equal to objectPath arg2 and return
the boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_objectPath

Checks for objectPath arg1 is less that objectPath arg2 and return the boolean
result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_objectPath

Determine if objectPath arg1 is not equal to the value of arg2 converted to
the data type of arg1 then return the boolean result. Use 'nex' ('!==')
instead if you want true to be returned if arg1 and arg2's data type don't
match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_objectPath

Determine if for objectPath arg1 is not equal to the value or data type of
arg2 then return the boolean result. Use 'ne' ('!=') instead if you want arg2
to be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 objectPath

Converts value to data type objectPath returning objectPath result.
Convert to data type objectPath

=head4 Parameters

    $value

Value to convert

=head3 to_string_objectPath

Converts objectPath value to string. For array values, the to_string() value
for each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A objectPath value.

=cut

sub bag_objectPath {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<objectPath>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_objectPath {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<objectPath>");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_objectPath {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<objectPath>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_objectPath {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<objectPath>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_objectPath {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<objectPath>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_objectPath {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<objectPath>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub is_objectPath {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<objectPath>");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_objectPath {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<objectPath>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_objectPath {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<objectPath>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_objectPath {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<objectPath>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_objectPath {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<objectPath>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub objectPath {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "objectPath");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_string_objectPath {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<objectPath>");
    $request->set("value", $value);

    return $request->getResult();
}

42;
