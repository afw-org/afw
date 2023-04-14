package afw::objectId;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    bag_objectId 
    bag_size_objectId 
    eq_objectId 
    eqx_objectId 
    ge_objectId 
    gt_objectId 
    is_objectId 
    le_objectId 
    lt_objectId 
    ne_objectId 
    nex_objectId 
    objectId 
    to_string_objectId 
);

=head1 NAME

afw::objectId - Perl module for objectId functions

=head2 Functions

The following functions are exported by default

=head3 bag_objectId

Takes any number of objectId values and returns a list of list.
Makes a list from values

=head4 Parameters

    $values


=head3 bag_size_objectId

This returns the integer number of values in list.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 eq_objectId

Determine if objectId arg1 is equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use "eqx" ("===") instead
if you want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_objectId

Determine if for objectId arg1 is equal to the value and data type of arg2
then return the boolean result. Use "eq" ("==") instead if you want arg2 to
be converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_objectId

Checks for objectId arg1 is greater than or equal to objectId arg2 and return
the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_objectId

Checks for objectId arg1 is greater than objectId arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 is_objectId

Checks whether value is dataType objectId and return the boolean result.
Checks whether value is dataType objectId

=head4 Parameters

    $value

Value to check

=head3 le_objectId

Checks for objectId arg1 is less than or equal to objectId arg2 and return
the boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_objectId

Checks for objectId arg1 is less that objectId arg2 and return the boolean
result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_objectId

Determine if objectId arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use "nex" ("!==") instead
if you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_objectId

Determine if for objectId arg1 is not equal to the value or data type of arg2
then return the boolean result. Use "ne" ("!=") instead if you want arg2 to
be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 objectId

Converts value to data type objectId returning objectId result.
Convert to data type objectId

=head4 Parameters

    $value

Value to convert

=head3 to_string_objectId

Converts objectId value to string. For list values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A objectId value.

=cut

sub bag_objectId {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<objectId>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_objectId {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<objectId>");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_objectId {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<objectId>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_objectId {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<objectId>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_objectId {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<objectId>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_objectId {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<objectId>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub is_objectId {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<objectId>");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_objectId {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<objectId>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_objectId {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<objectId>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_objectId {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<objectId>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_objectId {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<objectId>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub objectId {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "objectId");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_string_objectId {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<objectId>");
    $request->set("value", $value);

    return $request->getResult();
}

42;
