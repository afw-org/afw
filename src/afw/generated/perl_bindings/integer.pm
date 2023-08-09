package afw::integer;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    abs_integer 
    add_integer 
    at_least_one_member_of_integer 
    bag_integer 
    bag_size_integer 
    divide_integer 
    eq_integer 
    eqx_integer 
    ge_integer 
    gt_integer 
    integer 
    intersection_integer 
    is_in_integer 
    is_integer 
    le_integer 
    lt_integer 
    max_integer 
    min_integer 
    mod_integer 
    multiply_integer 
    ne_integer 
    negative_integer 
    nex_integer 
    one_and_only_integer 
    set_equals_integer 
    subset_integer 
    subtract_integer 
    to_double_integer 
    to_string_integer 
    union_integer 
);

=head1 NAME

afw::integer - Perl module for integer functions

=head2 Functions

The following functions are exported by default

=head3 abs_integer

Compute the absolute value of the integer value and return the integer
result.
Absolute value

=head4 Parameters

    $value


=head3 add_integer

Add 2 or more integer values and return the integer result.
Add

=head4 Parameters

    $values


=head3 at_least_one_member_of_integer

Returns boolean true if at least one value in integer list1 is in integer
list2.
Checks for at least one value in common

=head4 Parameters

    $list1

The first array.

    $list2

The second array.

=head3 bag_integer

Takes any number of integer values and returns an array of array.
Makes an array from values

=head4 Parameters

    $values


=head3 bag_size_integer

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 divide_integer

Divide integer dividend by integer divisor and return the integer quotient.
Divide numbers

=head4 Parameters

    $dividend


    $divisor


=head3 eq_integer

Determine if integer arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_integer

Determine if for integer arg1 is equal to the value and data type of arg2
then return the boolean result. Use 'eq' ('==') instead if you want arg2 to
be converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_integer

Checks for integer arg1 is greater than or equal to integer arg2 and return
the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_integer

Checks for integer arg1 is greater than integer arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 integer

Converts value to data type integer returning integer result.
Convert to data type integer

=head4 Parameters

    $value

Value to convert

=head3 intersection_integer

Returns an array of integer with the values that are common to both array of
integer list1 and list2.
Returns intersection of two lists

=head4 Parameters

    $list1

The first array.

    $list2

The second array.

=head3 is_in_integer

Checks whether integer value is in array of integer array and returns the
boolean result.
Checks whether a value is in an array

=head4 Parameters

    $value


    $array


=head3 is_integer

Checks whether value is dataType integer and return the boolean result.
Checks whether value is dataType integer

=head4 Parameters

    $value

Value to check

=head3 le_integer

Checks for integer arg1 is less than or equal to integer arg2 and return the
boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_integer

Checks for integer arg1 is less that integer arg2 and return the boolean
result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 max_integer

Return the integer value that is greater than or equal to the others.
Maximum value

=head4 Parameters

    $values


=head3 min_integer

Return the integer value that is less than or equal to the others.
Minimum value

=head4 Parameters

    $values


=head3 mod_integer

Divide integer dividend by integer divisor and return the integer remainder.
Remainder of dividing numbers

=head4 Parameters

    $dividend


    $divisor


=head3 multiply_integer

Multiply 2 or more integer values and return the integer result.
Multiply numbers

=head4 Parameters

    $values


=head3 ne_integer

Determine if integer arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead
if you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 negative_integer

Return negative of integer value.
Negative of a number

=head4 Parameters

    $value


=head3 nex_integer

Determine if for integer arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to
be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_integer

This converts an array of integer values that contains one value to a single
integer value.
Converts a one value array to a integer value

=head4 Parameters

    $array


=head3 set_equals_integer

Returns boolean true if integer list1 and integer list2 are subsets of each
other and return the boolean result.
Checks whether two lists are subsets of each other

=head4 Parameters

    $list1


    $list2


=head3 subset_integer

Returns boolean true if the unique values in integer list1 are all in integer
list2.
Determines if the first array is a subset of second array

=head4 Parameters

    $list1

The first array.

    $list2

The second array.

=head3 subtract_integer

Subtract integer arg2 from integer arg1 and return the integer result.
Subtract numbers

=head4 Parameters

    $arg1


    $arg2


=head3 to_double_integer

Converts integer value to double and returns double result.
Converts to double

=head4 Parameters

    $value


=head3 to_string_integer

Converts integer value to string. For array values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A integer value.

=head3 union_integer

Returns an array of integer contains all of the unique values in two or more
array of integer values.
Returns union of two or more string lists

=head4 Parameters

    $lists

Two or more lists.

=cut

sub abs_integer {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "abs<integer>");
    $request->set("value", $value);

    return $request->getResult();
}

sub add_integer {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "add<integer>");
    $request->set("values", $values);

    return $request->getResult();
}

sub at_least_one_member_of_integer {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<integer>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub bag_integer {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<integer>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_integer {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<integer>");
    $request->set("value", $value);

    return $request->getResult();
}

sub divide_integer {
    my ($dividend, $divisor) = @_;

    my $request = $session->request()

    $request->set("function" => "divide<integer>");
    $request->set("dividend", $dividend);
    $request->set("divisor", $divisor);

    return $request->getResult();
}

sub eq_integer {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<integer>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_integer {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<integer>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_integer {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<integer>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_integer {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<integer>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub integer {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "integer");
    $request->set("value", $value);

    return $request->getResult();
}

sub intersection_integer {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<integer>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub is_in_integer {
    my ($value, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<integer>");
    $request->set("value", $value);
    $request->set("array", $array);

    return $request->getResult();
}

sub is_integer {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<integer>");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_integer {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<integer>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_integer {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<integer>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub max_integer {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "max<integer>");
    $request->set("values", $values);

    return $request->getResult();
}

sub min_integer {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "min<integer>");
    $request->set("values", $values);

    return $request->getResult();
}

sub mod_integer {
    my ($dividend, $divisor) = @_;

    my $request = $session->request()

    $request->set("function" => "mod<integer>");
    $request->set("dividend", $dividend);
    $request->set("divisor", $divisor);

    return $request->getResult();
}

sub multiply_integer {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "multiply<integer>");
    $request->set("values", $values);

    return $request->getResult();
}

sub ne_integer {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<integer>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub negative_integer {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "negative<integer>");
    $request->set("value", $value);

    return $request->getResult();
}

sub nex_integer {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<integer>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_integer {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<integer>");
    $request->set("array", $array);

    return $request->getResult();
}

sub set_equals_integer {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<integer>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub subset_integer {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<integer>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub subtract_integer {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "subtract<integer>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub to_double_integer {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_double<integer>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_string_integer {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<integer>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_integer {
    my ($lists) = @_;

    my $request = $session->request()

    $request->set("function" => "union<integer>");
    $request->set("lists", $lists);

    return $request->getResult();
}

42;
