package afw::base64Binary;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    at_least_one_member_of_base64Binary 
    bag_base64Binary 
    bag_size_base64Binary 
    base64Binary 
    decode_to_string_base64Binary 
    eq_base64Binary 
    eqx_base64Binary 
    ge_base64Binary 
    gt_base64Binary 
    intersection_base64Binary 
    is_base64Binary 
    is_in_base64Binary 
    le_base64Binary 
    lt_base64Binary 
    ne_base64Binary 
    nex_base64Binary 
    one_and_only_base64Binary 
    set_equals_base64Binary 
    subset_base64Binary 
    to_string_base64Binary 
    union_base64Binary 
);

=head1 NAME

afw::base64Binary - Perl module for base64Binary functions

=head2 Functions

The following functions are exported by default

=head3 at_least_one_member_of_base64Binary

Returns boolean true if at least one value in base64Binary list1 is in
base64Binary list2.
Checks for at least one value in common

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 bag_base64Binary

Takes any number of base64Binary values and returns a list of list.
Makes a list from values

=head4 Parameters

    $values


=head3 bag_size_base64Binary

This returns the integer number of values in list.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 base64Binary

Converts value to data type base64Binary returning base64Binary result.
Convert to data type base64Binary

=head4 Parameters

    $value

Value to convert

=head3 decode_to_string_base64Binary

Decode base64Binary value to string. An error is thrown if decoded value is
not valid UTF-8.
Decode base64Binary to string

=head4 Parameters

    $value

The base64Binary value to decode.

=head3 eq_base64Binary

Determine if base64Binary arg1 is equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
if you want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_base64Binary

Determine if for base64Binary arg1 is equal to the value and data type of
arg2 then return the boolean result. Use 'eq' ('==') instead if you want arg2
to be converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_base64Binary

Checks for base64Binary arg1 is greater than or equal to base64Binary arg2
and return the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_base64Binary

Checks for base64Binary arg1 is greater than base64Binary arg2 and return the
boolean result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 intersection_base64Binary

Returns a list of base64Binary with the values that are common to both list
of base64Binary list1 and list2.
Returns intersection of two lists

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 is_base64Binary

Checks whether value is dataType base64Binary and return the boolean result.
Checks whether value is dataType base64Binary

=head4 Parameters

    $value

Value to check

=head3 is_in_base64Binary

Checks whether base64Binary value is in list of base64Binary list and returns
the boolean result.
Checks whether a value is in a list

=head4 Parameters

    $value


    $array


=head3 le_base64Binary

Checks for base64Binary arg1 is less than or equal to base64Binary arg2 and
return the boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_base64Binary

Checks for base64Binary arg1 is less that base64Binary arg2 and return the
boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_base64Binary

Determine if base64Binary arg1 is not equal to the value of arg2 converted to
the data type of arg1 then return the boolean result. Use 'nex' ('!==')
instead if you want true to be returned if arg1 and arg2's data type don't
match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_base64Binary

Determine if for base64Binary arg1 is not equal to the value or data type of
arg2 then return the boolean result. Use 'ne' ('!=') instead if you want arg2
to be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_base64Binary

This converts a list of base64Binary values that contains one value to a
single base64Binary value.
Converts a one value list to a base64Binary value

=head4 Parameters

    $array


=head3 set_equals_base64Binary

Returns boolean true if base64Binary list1 and base64Binary list2 are subsets
of each other and return the boolean result.
Checks whether two lists are subsets of each other

=head4 Parameters

    $list1


    $list2


=head3 subset_base64Binary

Returns boolean true if the unique values in base64Binary list1 are all in
base64Binary list2.
Determines if the first list is a subset of second list

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 to_string_base64Binary

Converts base64Binary value to string. For list values, the to_string() value
for each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A base64Binary value.

=head3 union_base64Binary

Returns a list of base64Binary contains all of the unique values in two or
more list of base64Binary values.
Returns union of two or more string lists

=head4 Parameters

    $lists

Two or more lists.

=cut

sub at_least_one_member_of_base64Binary {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<base64Binary>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub bag_base64Binary {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<base64Binary>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_base64Binary {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<base64Binary>");
    $request->set("value", $value);

    return $request->getResult();
}

sub base64Binary {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "base64Binary");
    $request->set("value", $value);

    return $request->getResult();
}

sub decode_to_string_base64Binary {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "decode_to_string<base64Binary>");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_base64Binary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<base64Binary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_base64Binary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<base64Binary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_base64Binary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<base64Binary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_base64Binary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<base64Binary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub intersection_base64Binary {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<base64Binary>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub is_base64Binary {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<base64Binary>");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_in_base64Binary {
    my ($value, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<base64Binary>");
    $request->set("value", $value);
    $request->set("array", $array);

    return $request->getResult();
}

sub le_base64Binary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<base64Binary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_base64Binary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<base64Binary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_base64Binary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<base64Binary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_base64Binary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<base64Binary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_base64Binary {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<base64Binary>");
    $request->set("array", $array);

    return $request->getResult();
}

sub set_equals_base64Binary {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<base64Binary>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub subset_base64Binary {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<base64Binary>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub to_string_base64Binary {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<base64Binary>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_base64Binary {
    my ($lists) = @_;

    my $request = $session->request()

    $request->set("function" => "union<base64Binary>");
    $request->set("lists", $lists);

    return $request->getResult();
}

42;
