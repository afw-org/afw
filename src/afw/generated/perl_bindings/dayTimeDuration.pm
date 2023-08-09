package afw::dayTimeDuration;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    at_least_one_member_of_dayTimeDuration 
    bag_dayTimeDuration 
    bag_size_dayTimeDuration 
    dayTimeDuration 
    eq_dayTimeDuration 
    eqx_dayTimeDuration 
    ge_dayTimeDuration 
    gt_dayTimeDuration 
    intersection_dayTimeDuration 
    is_dayTimeDuration 
    is_in_dayTimeDuration 
    le_dayTimeDuration 
    lt_dayTimeDuration 
    ne_dayTimeDuration 
    nex_dayTimeDuration 
    one_and_only_dayTimeDuration 
    set_equals_dayTimeDuration 
    subset_dayTimeDuration 
    to_string_dayTimeDuration 
    union_dayTimeDuration 
);

=head1 NAME

afw::dayTimeDuration - Perl module for dayTimeDuration functions

=head2 Functions

The following functions are exported by default

=head3 at_least_one_member_of_dayTimeDuration

Returns boolean true if at least one value in dayTimeDuration list1 is in
dayTimeDuration list2.
Checks for at least one value in common

=head4 Parameters

    $list1

The first array.

    $list2

The second array.

=head3 bag_dayTimeDuration

Takes any number of dayTimeDuration values and returns an array of array.
Makes an array from values

=head4 Parameters

    $values


=head3 bag_size_dayTimeDuration

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 dayTimeDuration

Converts value to data type dayTimeDuration returning dayTimeDuration result.
Convert to data type dateTimeDuration

=head4 Parameters

    $value

Value to convert

=head3 eq_dayTimeDuration

Determine if dayTimeDuration arg1 is equal to the value of arg2 converted to
the data type of arg1 then return the boolean result. Use 'eqx' ('===')
instead if you want false to be returned if arg1 and arg2's data type don't
match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_dayTimeDuration

Determine if for dayTimeDuration arg1 is equal to the value and data type of
arg2 then return the boolean result. Use 'eq' ('==') instead if you want arg2
to be converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_dayTimeDuration

Checks for dayTimeDuration arg1 is greater than or equal to dayTimeDuration
arg2 and return the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_dayTimeDuration

Checks for dayTimeDuration arg1 is greater than dayTimeDuration arg2 and
return the boolean result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 intersection_dayTimeDuration

Returns an array of dayTimeDuration with the values that are common to both
array of dayTimeDuration list1 and list2.
Returns intersection of two lists

=head4 Parameters

    $list1

The first array.

    $list2

The second array.

=head3 is_dayTimeDuration

Checks whether value is dataType dayTimeDuration and return the boolean
result.
Checks whether value is dataType dayTimeDuration

=head4 Parameters

    $value

Value to check

=head3 is_in_dayTimeDuration

Checks whether dayTimeDuration value is in array of dayTimeDuration array and
returns the boolean result.
Checks whether a value is in an array

=head4 Parameters

    $value


    $array


=head3 le_dayTimeDuration

Checks for dayTimeDuration arg1 is less than or equal to dayTimeDuration arg2
and return the boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_dayTimeDuration

Checks for dayTimeDuration arg1 is less that dayTimeDuration arg2 and return
the boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_dayTimeDuration

Determine if dayTimeDuration arg1 is not equal to the value of arg2 converted
to the data type of arg1 then return the boolean result. Use 'nex' ('!==')
instead if you want true to be returned if arg1 and arg2's data type don't
match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_dayTimeDuration

Determine if for dayTimeDuration arg1 is not equal to the value or data type
of arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
arg2 to be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_dayTimeDuration

This converts an array of dayTimeDuration values that contains one value to a
single dayTimeDuration value.
Converts a one value array to a dayTimeDuration value

=head4 Parameters

    $array


=head3 set_equals_dayTimeDuration

Returns boolean true if dayTimeDuration list1 and dayTimeDuration list2 are
subsets of each other and return the boolean result.
Checks whether two lists are subsets of each other

=head4 Parameters

    $list1


    $list2


=head3 subset_dayTimeDuration

Returns boolean true if the unique values in dayTimeDuration list1 are all in
dayTimeDuration list2.
Determines if the first array is a subset of second array

=head4 Parameters

    $list1

The first array.

    $list2

The second array.

=head3 to_string_dayTimeDuration

Converts dayTimeDuration value to string. For array values, the to_string()
value for each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A dayTimeDuration value.

=head3 union_dayTimeDuration

Returns an array of dayTimeDuration contains all of the unique values in two
or more array of dayTimeDuration values.
Returns union of two or more string lists

=head4 Parameters

    $lists

Two or more lists.

=cut

sub at_least_one_member_of_dayTimeDuration {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<dayTimeDuration>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub bag_dayTimeDuration {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<dayTimeDuration>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_dayTimeDuration {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<dayTimeDuration>");
    $request->set("value", $value);

    return $request->getResult();
}

sub dayTimeDuration {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "dayTimeDuration");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_dayTimeDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<dayTimeDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_dayTimeDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<dayTimeDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_dayTimeDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<dayTimeDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_dayTimeDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<dayTimeDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub intersection_dayTimeDuration {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<dayTimeDuration>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub is_dayTimeDuration {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<dayTimeDuration>");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_in_dayTimeDuration {
    my ($value, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<dayTimeDuration>");
    $request->set("value", $value);
    $request->set("array", $array);

    return $request->getResult();
}

sub le_dayTimeDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<dayTimeDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_dayTimeDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<dayTimeDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_dayTimeDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<dayTimeDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_dayTimeDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<dayTimeDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_dayTimeDuration {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<dayTimeDuration>");
    $request->set("array", $array);

    return $request->getResult();
}

sub set_equals_dayTimeDuration {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<dayTimeDuration>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub subset_dayTimeDuration {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<dayTimeDuration>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub to_string_dayTimeDuration {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<dayTimeDuration>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_dayTimeDuration {
    my ($lists) = @_;

    my $request = $session->request()

    $request->set("function" => "union<dayTimeDuration>");
    $request->set("lists", $lists);

    return $request->getResult();
}

42;
