package afw::time;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    at_least_one_member_of_time 
    bag_size_time 
    bag_time 
    eq_time 
    eqx_time 
    ge_time 
    gt_time 
    in_range_time 
    intersection_time 
    is_in_time 
    is_time 
    le_time 
    lt_time 
    max_time 
    min_time 
    ne_time 
    nex_time 
    one_and_only_time 
    set_equals_time 
    subset_time 
    time 
    to_string_time 
    union_time 
);

=head1 NAME

afw::time - Perl module for time functions

=head2 Functions

The following functions are exported by default

=head3 at_least_one_member_of_time

Returns boolean true if at least one value in time list1 is in time list2.
Checks for at least one value in common

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 bag_size_time

This returns the integer number of values in list.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 bag_time

Takes any number of time values and returns a list of list.
Makes a list from values

=head4 Parameters

    $values


=head3 eq_time

Determine if time arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use "eqx" ("===") instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_time

Determine if for time arg1 is equal to the value and data type of arg2 then
return the boolean result. Use "eq" ("==") instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_time

Checks for time arg1 is greater than or equal to time arg2 and return the
boolean result.

It is illegal to compare a time that includes a time-zone value with one that
does not. In such cases, the `in_range<time>` function should be used.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_time

Checks for time arg1 is greater than time arg2 and return the boolean result.

It is illegal to compare a time that includes a time-zone value with one that
does not. In such cases, the `in_range<time>` function should be used.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 in_range_time

Checks if time is between startTime and endTime, inclusive. Regardless of
endTime value, it is always considered to be equal to, but less than 24 hours
greater than startTime. If no time zone is specified for time, the default
time zone is used. If no time zone is specified for startTime or endTime, the
time zone of time is used.
Checks if time in range

=head4 Parameters

    $time


    $startTime


    $endTime


=head3 intersection_time

Returns a list of time with the values that are common to both list of time
list1 and list2.
Returns intersection of two lists

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 is_in_time

Checks whether time value is in list of time list and returns the boolean
result.
Checks whether a value is in a list

=head4 Parameters

    $value


    $list


=head3 is_time

Checks whether value is dataType time and return the boolean result.
Checks whether value is dataType time

=head4 Parameters

    $value

Value to check

=head3 le_time

Checks for time arg1 is less than or equal to time arg2 and return the
boolean result.

It is illegal to compare a time that includes a time-zone value with one that
does not. In such cases, the `in_range<time>` function should be used.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_time

Checks for time arg1 is less that time arg2 and return the boolean result.

It is illegal to compare a time that includes a time-zone value with one that
does not. In such cases, the `in_range<time>` function should be used.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 max_time

Return the time value that is greater than or equal to the others.

It is illegal to compare a time that includes a time-zone value with one that
does not. In such cases, the `in_range<time>` function should be used.
Maximum value

=head4 Parameters

    $values


=head3 min_time

Return the time value that is less than or equal to the others.

It is illegal to compare a time that includes a time-zone value with one that
does not. In such cases, the `in_range<time>` function should be used.
Minimum value

=head4 Parameters

    $values


=head3 ne_time

Determine if time arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use "nex" ("!==") instead
if you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_time

Determine if for time arg1 is not equal to the value or data type of arg2
then return the boolean result. Use "ne" ("!=") instead if you want arg2 to
be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_time

This converts a list of time values that contains one value to a single time
value.
Converts a one value list to a time value

=head4 Parameters

    $list


=head3 set_equals_time

Returns boolean true if time list1 and time list2 are subsets of each other
and return the boolean result.
Checks whether two lists are subsets of each other

=head4 Parameters

    $list1


    $list2


=head3 subset_time

Returns boolean true if the unique values in time list1 are all in time
list2.
Determines if the first list is a subset of second list

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 time

Converts value to data type time returning time result.
Convert to data type time

=head4 Parameters

    $value

Value to convert

=head3 to_string_time

Converts time value to string. For list values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A time value.

=head3 union_time

Returns a list of time contains all of the unique values in two or more list
of time values.
Returns union of two or more string lists

=head4 Parameters

    $lists

Two or more lists.

=cut

sub at_least_one_member_of_time {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<time>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub bag_size_time {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<time>");
    $request->set("value", $value);

    return $request->getResult();
}

sub bag_time {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<time>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub eq_time {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<time>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_time {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<time>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_time {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<time>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_time {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<time>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub in_range_time {
    my ($time, $startTime, $endTime) = @_;

    my $request = $session->request()

    $request->set("function" => "in_range<time>");
    $request->set("time", $time);
    $request->set("startTime", $startTime);
    $request->set("endTime", $endTime);

    return $request->getResult();
}

sub intersection_time {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<time>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub is_in_time {
    my ($value, $list) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<time>");
    $request->set("value", $value);
    $request->set("list", $list);

    return $request->getResult();
}

sub is_time {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<time>");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_time {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<time>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_time {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<time>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub max_time {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "max<time>");
    $request->set("values", $values);

    return $request->getResult();
}

sub min_time {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "min<time>");
    $request->set("values", $values);

    return $request->getResult();
}

sub ne_time {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<time>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_time {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<time>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_time {
    my ($list) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<time>");
    $request->set("list", $list);

    return $request->getResult();
}

sub set_equals_time {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<time>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub subset_time {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<time>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub time {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "time");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_string_time {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<time>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_time {
    my ($lists) = @_;

    my $request = $session->request()

    $request->set("function" => "union<time>");
    $request->set("lists", $lists);

    return $request->getResult();
}

42;
