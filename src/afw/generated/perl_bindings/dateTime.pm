package afw::dateTime;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    add_dayTimeDuration_dateTime 
    add_yearMonthDuration_dateTime 
    at_least_one_member_of_dateTime 
    bag_dateTime 
    bag_size_dateTime 
    dateTime 
    eq_dateTime 
    eqx_dateTime 
    ge_dateTime 
    gt_dateTime 
    intersection_dateTime 
    is_dateTime 
    is_in_dateTime 
    le_dateTime 
    lt_dateTime 
    max_dateTime 
    min_dateTime 
    ne_dateTime 
    nex_dateTime 
    one_and_only_dateTime 
    set_equals_dateTime 
    subset_dateTime 
    subtract_dayTimeDuration_dateTime 
    subtract_yearMonthDuration_dateTime 
    to_string_dateTime 
    union_dateTime 
);

=head1 NAME

afw::dateTime - Perl module for dateTime functions

=head2 Functions

The following functions are exported by default

=head3 add_dayTimeDuration_dateTime

Add dateTime arg1 to dayTimeDuration arg2 and return the dateTime result.
Add duration

=head4 Parameters

    $arg1


    $arg2


=head3 add_yearMonthDuration_dateTime

Add dateTime arg1 to yearMonthDuration arg2 and return the dateTime result.
Add duration

=head4 Parameters

    $arg1


    $arg2


=head3 at_least_one_member_of_dateTime

Returns boolean true if at least one value in dateTime array1 is in dateTime
array2.
Checks for at least one value in common

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 bag_dateTime

Takes any number of dateTime values and returns an array of array.
Makes an array from values

=head4 Parameters

    $values


=head3 bag_size_dateTime

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 dateTime

Converts value to data type dateTime returning dateTime result.
Convert to data type dateTime

=head4 Parameters

    $value

Value to convert

=head3 eq_dateTime

Determine if dateTime arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_dateTime

Determine if for dateTime arg1 is equal to the value and data type of arg2
then return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_dateTime

Checks for dateTime arg1 is greater than or equal to dateTime arg2 and return
the boolean result.

If a dateTime value does not include a time-zone value, then the local
time-zone value will be assigned.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_dateTime

Checks for dateTime arg1 is greater than dateTime arg2 and return the boolean
result.

If a dateTime value does not include a time-zone value, then the local
time-zone value will be assigned.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 intersection_dateTime

Returns an array of dateTime with the values that are common to both array of
dateTime array1 and array2.
Returns intersection of two arrays

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 is_dateTime

Checks whether value is dataType dateTime and return the boolean result.
Checks whether value is dataType dateTime

=head4 Parameters

    $value

Value to check

=head3 is_in_dateTime

Checks whether dateTime value is in array of dateTime array and returns the
boolean result.
Checks whether a value is in an array

=head4 Parameters

    $value


    $array


=head3 le_dateTime

Checks for dateTime arg1 is less than or equal to dateTime arg2 and return the
boolean result.

If a dateTime value does not include a time-zone value, then the local
time-zone value will be assigned.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_dateTime

Checks for dateTime arg1 is less that dateTime arg2 and return the boolean
result.

If a dateTime value does not include a time-zone value, then the local
time-zone value will be assigned.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 max_dateTime

Return the dateTime value that is greater than or equal to the others.

If a dateTime value does not include a time-zone value, then the local
time-zone value will be assigned.
Maximum value

=head4 Parameters

    $values


=head3 min_dateTime

Return the dateTime value that is less than or equal to the others.

If a dateTime value does not include a time-zone value, then the local
time-zone value will be assigned.
Minimum value

=head4 Parameters

    $values


=head3 ne_dateTime

Determine if dateTime arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead if
you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_dateTime

Determine if for dateTime arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_dateTime

This converts an array of dateTime values that contains one value to a single
dateTime value.
Converts a one value array to a dateTime value

=head4 Parameters

    $array


=head3 set_equals_dateTime

Returns boolean true if dateTime array1 and dateTime array2 are subsets of
each other and return the boolean result.
Checks whether two arrays are subsets of each other

=head4 Parameters

    $array1


    $array2


=head3 subset_dateTime

Returns boolean true if the unique values in dateTime array1 are all in
dateTime array2.
Determines if the first array is a subset of second array

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 subtract_dayTimeDuration_dateTime

Subtract dayTimeDuration arg2 from dateTime arg1 and return the dateTime
result.
Subtract numbers

=head4 Parameters

    $arg1


    $arg2


=head3 subtract_yearMonthDuration_dateTime

Subtract yearMonthDuration arg2 from dateTime arg1 and return the dateTime
result.
Subtract numbers

=head4 Parameters

    $arg1


    $arg2


=head3 to_string_dateTime

Converts dateTime value to string. For array values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A dateTime value.

=head3 union_dateTime

Returns an array of dateTime contains all of the unique values in two or more
array of dateTime values.
Returns union of two or more string arrays

=head4 Parameters

    $arrays

Two or more arrays.

=cut

sub add_dayTimeDuration_dateTime {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "add_dayTimeDuration<dateTime>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub add_yearMonthDuration_dateTime {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "add_yearMonthDuration<dateTime>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub at_least_one_member_of_dateTime {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<dateTime>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub bag_dateTime {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<dateTime>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_dateTime {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<dateTime>");
    $request->set("value", $value);

    return $request->getResult();
}

sub dateTime {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "dateTime");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_dateTime {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<dateTime>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_dateTime {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<dateTime>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_dateTime {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<dateTime>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_dateTime {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<dateTime>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub intersection_dateTime {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<dateTime>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub is_dateTime {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<dateTime>");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_in_dateTime {
    my ($value, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<dateTime>");
    $request->set("value", $value);
    $request->set("array", $array);

    return $request->getResult();
}

sub le_dateTime {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<dateTime>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_dateTime {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<dateTime>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub max_dateTime {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "max<dateTime>");
    $request->set("values", $values);

    return $request->getResult();
}

sub min_dateTime {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "min<dateTime>");
    $request->set("values", $values);

    return $request->getResult();
}

sub ne_dateTime {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<dateTime>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_dateTime {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<dateTime>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_dateTime {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<dateTime>");
    $request->set("array", $array);

    return $request->getResult();
}

sub set_equals_dateTime {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<dateTime>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub subset_dateTime {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<dateTime>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub subtract_dayTimeDuration_dateTime {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "subtract_dayTimeDuration<dateTime>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub subtract_yearMonthDuration_dateTime {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "subtract_yearMonthDuration<dateTime>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub to_string_dateTime {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<dateTime>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_dateTime {
    my ($arrays) = @_;

    my $request = $session->request()

    $request->set("function" => "union<dateTime>");
    $request->set("arrays", $arrays);

    return $request->getResult();
}

42;
