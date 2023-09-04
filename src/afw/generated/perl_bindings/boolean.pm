package afw::boolean;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    at_least_one_member_of_boolean 
    bag_boolean 
    bag_size_boolean 
    boolean 
    eq_boolean 
    eqx_boolean 
    ge_boolean 
    gt_boolean 
    intersection_boolean 
    is_boolean 
    is_in_boolean 
    le_boolean 
    lt_boolean 
    ne_boolean 
    nex_boolean 
    one_and_only_boolean 
    set_equals_boolean 
    subset_boolean 
    to_string_boolean 
    union_boolean 
);

=head1 NAME

afw::boolean - Perl module for boolean functions

=head2 Functions

The following functions are exported by default

=head3 at_least_one_member_of_boolean

Returns boolean true if at least one value in boolean array1 is in boolean
array2.
Checks for at least one value in common

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 bag_boolean

Takes any number of boolean values and returns an array of array.
Makes an array from values

=head4 Parameters

    $values


=head3 bag_size_boolean

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 boolean

Converts value to data type boolean returning boolean result.
Convert to data type boolean

=head4 Parameters

    $value

Value to convert

=head3 eq_boolean

Determine if boolean arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_boolean

Determine if for boolean arg1 is equal to the value and data type of arg2 then
return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_boolean

Checks for boolean arg1 is greater than or equal to boolean arg2 and return
the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_boolean

Checks for boolean arg1 is greater than boolean arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 intersection_boolean

Returns an array of boolean with the values that are common to both array of
boolean array1 and array2.
Returns intersection of two arrays

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 is_boolean

Checks whether value is dataType boolean and return the boolean result.
Checks whether value is dataType boolean

=head4 Parameters

    $value

Value to check

=head3 is_in_boolean

Checks whether boolean value is in array of boolean array and returns the
boolean result.
Checks whether a value is in an array

=head4 Parameters

    $value


    $array


=head3 le_boolean

Checks for boolean arg1 is less than or equal to boolean arg2 and return the
boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_boolean

Checks for boolean arg1 is less that boolean arg2 and return the boolean
result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_boolean

Determine if boolean arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead if
you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_boolean

Determine if for boolean arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_boolean

This converts an array of boolean values that contains one value to a single
boolean value.
Converts a one value array to a boolean value

=head4 Parameters

    $array


=head3 set_equals_boolean

Returns boolean true if boolean array1 and boolean array2 are subsets of each
other and return the boolean result.
Checks whether two arrays are subsets of each other

=head4 Parameters

    $array1


    $array2


=head3 subset_boolean

Returns boolean true if the unique values in boolean array1 are all in boolean
array2.
Determines if the first array is a subset of second array

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 to_string_boolean

Converts boolean value to string. For array values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A boolean value.

=head3 union_boolean

Returns an array of boolean contains all of the unique values in two or more
array of boolean values.
Returns union of two or more string arrays

=head4 Parameters

    $arrays

Two or more arrays.

=cut

sub at_least_one_member_of_boolean {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<boolean>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub bag_boolean {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<boolean>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_boolean {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<boolean>");
    $request->set("value", $value);

    return $request->getResult();
}

sub boolean {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "boolean");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_boolean {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<boolean>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_boolean {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<boolean>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_boolean {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<boolean>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_boolean {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<boolean>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub intersection_boolean {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<boolean>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub is_boolean {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<boolean>");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_in_boolean {
    my ($value, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<boolean>");
    $request->set("value", $value);
    $request->set("array", $array);

    return $request->getResult();
}

sub le_boolean {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<boolean>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_boolean {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<boolean>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_boolean {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<boolean>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_boolean {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<boolean>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_boolean {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<boolean>");
    $request->set("array", $array);

    return $request->getResult();
}

sub set_equals_boolean {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<boolean>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub subset_boolean {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<boolean>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub to_string_boolean {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<boolean>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_boolean {
    my ($arrays) = @_;

    my $request = $session->request()

    $request->set("function" => "union<boolean>");
    $request->set("arrays", $arrays);

    return $request->getResult();
}

42;
