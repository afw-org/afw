package afw::x500Name;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    at_least_one_member_of_x500Name 
    bag_size_x500Name 
    bag_x500Name 
    eq_x500Name 
    eqx_x500Name 
    ge_x500Name 
    gt_x500Name 
    intersection_x500Name 
    is_in_x500Name 
    is_x500Name 
    le_x500Name 
    lt_x500Name 
    match_x500Name 
    ne_x500Name 
    nex_x500Name 
    one_and_only_x500Name 
    regexp_match_x500Name 
    set_equals_x500Name 
    subset_x500Name 
    to_string_x500Name 
    union_x500Name 
    x500Name 
);

=head1 NAME

afw::x500Name - Perl module for x500Name functions

=head2 Functions

The following functions are exported by default

=head3 at_least_one_member_of_x500Name

Returns boolean true if at least one value in x500Name list1 is in x500Name
list2.
Checks for at least one value in common

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 bag_size_x500Name

This returns the integer number of values in list.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 bag_x500Name

Takes any number of x500Name values and returns a list of list.
Makes a list from values

=head4 Parameters

    $values


=head3 eq_x500Name

Determine if x500Name arg1 is equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
if you want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_x500Name

Determine if for x500Name arg1 is equal to the value and data type of arg2
then return the boolean result. Use 'eq' ('==') instead if you want arg2 to
be converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_x500Name

Checks for x500Name arg1 is greater than or equal to x500Name arg2 and return
the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_x500Name

Checks for x500Name arg1 is greater than x500Name arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 intersection_x500Name

Returns a list of x500Name with the values that are common to both list of
x500Name list1 and list2.
Returns intersection of two lists

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 is_in_x500Name

Checks whether x500Name value is in list of x500Name list and returns the
boolean result.
Checks whether a value is in a list

=head4 Parameters

    $value


    $list


=head3 is_x500Name

Checks whether value is dataType x500Name and return the boolean result.
Checks whether value is dataType x500Name

=head4 Parameters

    $value

Value to check

=head3 le_x500Name

Checks for x500Name arg1 is less than or equal to x500Name arg2 and return
the boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_x500Name

Checks for x500Name arg1 is less that x500Name arg2 and return the boolean
result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 match_x500Name

Returns true if arg1 matches some terminal sequence of RDNs from arg2 when
compared using equal_x500Name.
Checks if x500Name matches

=head4 Parameters

    $arg1


    $arg2


=head3 ne_x500Name

Determine if x500Name arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead
if you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_x500Name

Determine if for x500Name arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to
be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_x500Name

This converts a list of x500Name values that contains one value to a single
x500Name value.
Converts a one value list to a x500Name value

=head4 Parameters

    $list


=head3 regexp_match_x500Name

Checks whether x500Name value matches the regular expression regexp and
return the boolean result.
Search for a match using a regular expression

=head4 Parameters

    $value


    $regexp


=head3 set_equals_x500Name

Returns boolean true if x500Name list1 and x500Name list2 are subsets of each
other and return the boolean result.
Checks whether two lists are subsets of each other

=head4 Parameters

    $list1


    $list2


=head3 subset_x500Name

Returns boolean true if the unique values in x500Name list1 are all in
x500Name list2.
Determines if the first list is a subset of second list

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 to_string_x500Name

Converts x500Name value to string. For list values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A x500Name value.

=head3 union_x500Name

Returns a list of x500Name contains all of the unique values in two or more
list of x500Name values.
Returns union of two or more string lists

=head4 Parameters

    $lists

Two or more lists.

=head3 x500Name

Converts value to data type x500Name returning x500Name result.
Convert to data type x500Name

=head4 Parameters

    $value

Value to convert

=cut

sub at_least_one_member_of_x500Name {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<x500Name>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub bag_size_x500Name {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<x500Name>");
    $request->set("value", $value);

    return $request->getResult();
}

sub bag_x500Name {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<x500Name>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub eq_x500Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<x500Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_x500Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<x500Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_x500Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<x500Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_x500Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<x500Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub intersection_x500Name {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<x500Name>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub is_in_x500Name {
    my ($value, $list) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<x500Name>");
    $request->set("value", $value);
    $request->set("list", $list);

    return $request->getResult();
}

sub is_x500Name {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<x500Name>");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_x500Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<x500Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_x500Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<x500Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub match_x500Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "match<x500Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_x500Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<x500Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_x500Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<x500Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_x500Name {
    my ($list) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<x500Name>");
    $request->set("list", $list);

    return $request->getResult();
}

sub regexp_match_x500Name {
    my ($value, $regexp) = @_;

    my $request = $session->request()

    $request->set("function" => "regexp_match<x500Name>");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

    return $request->getResult();
}

sub set_equals_x500Name {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<x500Name>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub subset_x500Name {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<x500Name>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub to_string_x500Name {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<x500Name>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_x500Name {
    my ($lists) = @_;

    my $request = $session->request()

    $request->set("function" => "union<x500Name>");
    $request->set("lists", $lists);

    return $request->getResult();
}

sub x500Name {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "x500Name");
    $request->set("value", $value);

    return $request->getResult();
}

42;
