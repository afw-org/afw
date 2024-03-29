package afw::yearMonthDuration;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    at_least_one_member_of_yearMonthDuration 
    bag_size_yearMonthDuration 
    bag_yearMonthDuration 
    eq_yearMonthDuration 
    eqx_yearMonthDuration 
    ge_yearMonthDuration 
    gt_yearMonthDuration 
    intersection_yearMonthDuration 
    is_in_yearMonthDuration 
    is_yearMonthDuration 
    le_yearMonthDuration 
    lt_yearMonthDuration 
    ne_yearMonthDuration 
    nex_yearMonthDuration 
    one_and_only_yearMonthDuration 
    set_equals_yearMonthDuration 
    subset_yearMonthDuration 
    to_string_yearMonthDuration 
    union_yearMonthDuration 
    yearMonthDuration 
);

=head1 NAME

afw::yearMonthDuration - Perl module for yearMonthDuration functions

=head2 Functions

The following functions are exported by default

=head3 at_least_one_member_of_yearMonthDuration

Returns boolean true if at least one value in yearMonthDuration list1 is in
yearMonthDuration list2.
Checks for at least one value in common

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 bag_size_yearMonthDuration

This returns the integer number of values in list.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 bag_yearMonthDuration

Takes any number of yearMonthDuration values and returns a list of list.
Makes a list from values

=head4 Parameters

    $values


=head3 eq_yearMonthDuration

Determine if yearMonthDuration arg1 is equal to the value of arg2 converted
to the data type of arg1 then return the boolean result. Use "eqx" ("===")
instead if you want false to be returned if arg1 and arg2's data type don't
match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_yearMonthDuration

Determine if for yearMonthDuration arg1 is equal to the value and data type
of arg2 then return the boolean result. Use "eq" ("==") instead if you want
arg2 to be converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_yearMonthDuration

Checks for yearMonthDuration arg1 is greater than or equal to
yearMonthDuration arg2 and return the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_yearMonthDuration

Checks for yearMonthDuration arg1 is greater than yearMonthDuration arg2 and
return the boolean result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 intersection_yearMonthDuration

Returns a list of yearMonthDuration with the values that are common to both
list of yearMonthDuration list1 and list2.
Returns intersection of two lists

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 is_in_yearMonthDuration

Checks whether yearMonthDuration value is in list of yearMonthDuration list
and returns the boolean result.
Checks whether a value is in a list

=head4 Parameters

    $value


    $list


=head3 is_yearMonthDuration

Checks whether value is dataType yearMonthDuration and return the boolean
result.
Checks whether value is dataType yearMonthDuration

=head4 Parameters

    $value

Value to check

=head3 le_yearMonthDuration

Checks for yearMonthDuration arg1 is less than or equal to yearMonthDuration
arg2 and return the boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_yearMonthDuration

Checks for yearMonthDuration arg1 is less that yearMonthDuration arg2 and
return the boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_yearMonthDuration

Determine if yearMonthDuration arg1 is not equal to the value of arg2
converted to the data type of arg1 then return the boolean result. Use "nex"
("!==") instead if you want true to be returned if arg1 and arg2's data type
don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_yearMonthDuration

Determine if for yearMonthDuration arg1 is not equal to the value or data
type of arg2 then return the boolean result. Use "ne" ("!=") instead if you
want arg2 to be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_yearMonthDuration

This converts a list of yearMonthDuration values that contains one value to a
single yearMonthDuration value.
Converts a one value list to a yearMonthDuration value

=head4 Parameters

    $list


=head3 set_equals_yearMonthDuration

Returns boolean true if yearMonthDuration list1 and yearMonthDuration list2
are subsets of each other and return the boolean result.
Checks whether two lists are subsets of each other

=head4 Parameters

    $list1


    $list2


=head3 subset_yearMonthDuration

Returns boolean true if the unique values in yearMonthDuration list1 are all
in yearMonthDuration list2.
Determines if the first list is a subset of second list

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 to_string_yearMonthDuration

Converts yearMonthDuration value to string. For list values, the to_string()
value for each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A yearMonthDuration value.

=head3 union_yearMonthDuration

Returns a list of yearMonthDuration contains all of the unique values in two
or more list of yearMonthDuration values.
Returns union of two or more string lists

=head4 Parameters

    $lists

Two or more lists.

=head3 yearMonthDuration

Converts value to data type yearMonthDuration returning yearMonthDuration
result.
Convert to data type yearMonthDuration

=head4 Parameters

    $value

Value to convert

=cut

sub at_least_one_member_of_yearMonthDuration {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<yearMonthDuration>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub bag_size_yearMonthDuration {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<yearMonthDuration>");
    $request->set("value", $value);

    return $request->getResult();
}

sub bag_yearMonthDuration {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<yearMonthDuration>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub eq_yearMonthDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<yearMonthDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_yearMonthDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<yearMonthDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_yearMonthDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<yearMonthDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_yearMonthDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<yearMonthDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub intersection_yearMonthDuration {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<yearMonthDuration>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub is_in_yearMonthDuration {
    my ($value, $list) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<yearMonthDuration>");
    $request->set("value", $value);
    $request->set("list", $list);

    return $request->getResult();
}

sub is_yearMonthDuration {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<yearMonthDuration>");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_yearMonthDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<yearMonthDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_yearMonthDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<yearMonthDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_yearMonthDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<yearMonthDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_yearMonthDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<yearMonthDuration>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_yearMonthDuration {
    my ($list) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<yearMonthDuration>");
    $request->set("list", $list);

    return $request->getResult();
}

sub set_equals_yearMonthDuration {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<yearMonthDuration>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub subset_yearMonthDuration {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<yearMonthDuration>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub to_string_yearMonthDuration {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<yearMonthDuration>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_yearMonthDuration {
    my ($lists) = @_;

    my $request = $session->request()

    $request->set("function" => "union<yearMonthDuration>");
    $request->set("lists", $lists);

    return $request->getResult();
}

sub yearMonthDuration {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "yearMonthDuration");
    $request->set("value", $value);

    return $request->getResult();
}

42;
