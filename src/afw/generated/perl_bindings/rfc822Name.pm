package afw::rfc822Name;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    at_least_one_member_of_rfc822Name 
    bag_rfc822Name 
    bag_size_rfc822Name 
    eq_rfc822Name 
    eqx_rfc822Name 
    ge_rfc822Name 
    gt_rfc822Name 
    intersection_rfc822Name 
    is_in_rfc822Name 
    is_rfc822Name 
    le_rfc822Name 
    lt_rfc822Name 
    match_rfc822Name 
    ne_rfc822Name 
    nex_rfc822Name 
    one_and_only_rfc822Name 
    regexp_match_rfc822Name 
    rfc822Name 
    set_equals_rfc822Name 
    subset_rfc822Name 
    to_string_rfc822Name 
    union_rfc822Name 
);

=head1 NAME

afw::rfc822Name - Perl module for rfc822Name functions

=head2 Functions

The following functions are exported by default

=head3 at_least_one_member_of_rfc822Name

Returns boolean true if at least one value in rfc822Name list1 is in
rfc822Name list2.
Checks for at least one value in common

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 bag_rfc822Name

Takes any number of rfc822Name values and returns a list of list.
Makes a list from values

=head4 Parameters

    $values


=head3 bag_size_rfc822Name

This returns the integer number of values in list.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 eq_rfc822Name

Determine if rfc822Name arg1 is equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
if you want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_rfc822Name

Determine if for rfc822Name arg1 is equal to the value and data type of arg2
then return the boolean result. Use 'eq' ('==') instead if you want arg2 to
be converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_rfc822Name

Checks for rfc822Name arg1 is greater than or equal to rfc822Name arg2 and
return the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_rfc822Name

Checks for rfc822Name arg1 is greater than rfc822Name arg2 and return the
boolean result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 intersection_rfc822Name

Returns a list of rfc822Name with the values that are common to both list of
rfc822Name list1 and list2.
Returns intersection of two lists

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 is_in_rfc822Name

Checks whether rfc822Name value is in list of rfc822Name list and returns the
boolean result.
Checks whether a value is in a list

=head4 Parameters

    $value


    $array


=head3 is_rfc822Name

Checks whether value is dataType rfc822Name and return the boolean result.
Checks whether value is dataType rfc822Name

=head4 Parameters

    $value

Value to check

=head3 le_rfc822Name

Checks for rfc822Name arg1 is less than or equal to rfc822Name arg2 and
return the boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_rfc822Name

Checks for rfc822Name arg1 is less that rfc822Name arg2 and return the
boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 match_rfc822Name

Returns true if arg1 matches some terminal sequence of RDNs from arg2 when
compared using equal_rfc822Name.
Checks if x500Name matches

=head4 Parameters

    $arg1


    $arg2


=head3 ne_rfc822Name

Determine if rfc822Name arg1 is not equal to the value of arg2 converted to
the data type of arg1 then return the boolean result. Use 'nex' ('!==')
instead if you want true to be returned if arg1 and arg2's data type don't
match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_rfc822Name

Determine if for rfc822Name arg1 is not equal to the value or data type of
arg2 then return the boolean result. Use 'ne' ('!=') instead if you want arg2
to be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_rfc822Name

This converts a list of rfc822Name values that contains one value to a single
rfc822Name value.
Converts a one value list to a rfc822Name value

=head4 Parameters

    $array


=head3 regexp_match_rfc822Name

Checks whether rfc822Name value matches the regular expression regexp and
return the boolean result.
Search for a match using a regular expression

=head4 Parameters

    $value


    $regexp


=head3 rfc822Name

Converts value to data type rfc822Name returning rfc822Name result.
Convert to data type rfc822Name

=head4 Parameters

    $value

Value to convert

=head3 set_equals_rfc822Name

Returns boolean true if rfc822Name list1 and rfc822Name list2 are subsets of
each other and return the boolean result.
Checks whether two lists are subsets of each other

=head4 Parameters

    $list1


    $list2


=head3 subset_rfc822Name

Returns boolean true if the unique values in rfc822Name list1 are all in
rfc822Name list2.
Determines if the first list is a subset of second list

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 to_string_rfc822Name

Converts rfc822Name value to string. For list values, the to_string() value
for each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A rfc822Name value.

=head3 union_rfc822Name

Returns a list of rfc822Name contains all of the unique values in two or more
list of rfc822Name values.
Returns union of two or more string lists

=head4 Parameters

    $lists

Two or more lists.

=cut

sub at_least_one_member_of_rfc822Name {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<rfc822Name>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub bag_rfc822Name {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<rfc822Name>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_rfc822Name {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<rfc822Name>");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_rfc822Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<rfc822Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_rfc822Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<rfc822Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_rfc822Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<rfc822Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_rfc822Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<rfc822Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub intersection_rfc822Name {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<rfc822Name>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub is_in_rfc822Name {
    my ($value, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<rfc822Name>");
    $request->set("value", $value);
    $request->set("array", $array);

    return $request->getResult();
}

sub is_rfc822Name {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<rfc822Name>");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_rfc822Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<rfc822Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_rfc822Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<rfc822Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub match_rfc822Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "match<rfc822Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_rfc822Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<rfc822Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_rfc822Name {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<rfc822Name>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_rfc822Name {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<rfc822Name>");
    $request->set("array", $array);

    return $request->getResult();
}

sub regexp_match_rfc822Name {
    my ($value, $regexp) = @_;

    my $request = $session->request()

    $request->set("function" => "regexp_match<rfc822Name>");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

    return $request->getResult();
}

sub rfc822Name {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "rfc822Name");
    $request->set("value", $value);

    return $request->getResult();
}

sub set_equals_rfc822Name {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<rfc822Name>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub subset_rfc822Name {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<rfc822Name>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub to_string_rfc822Name {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<rfc822Name>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_rfc822Name {
    my ($lists) = @_;

    my $request = $session->request()

    $request->set("function" => "union<rfc822Name>");
    $request->set("lists", $lists);

    return $request->getResult();
}

42;
