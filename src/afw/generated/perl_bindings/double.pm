package afw::double;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    abs_double 
    add_double 
    at_least_one_member_of_double 
    bag_double 
    bag_size_double 
    ceil_double 
    divide_double 
    double 
    eq_double 
    eqx_double 
    floor_double 
    ge_double 
    gt_double 
    intersection_double 
    is_NaN 
    is_double 
    is_finite 
    is_in_double 
    le_double 
    lt_double 
    max_double 
    min_double 
    multiply_double 
    ne_double 
    negative_double 
    nex_double 
    one_and_only_double 
    pow_double 
    round_double 
    set_equals_double 
    subset_double 
    subtract_double 
    to_integer_double 
    to_string_double 
    union_double 
);

=head1 NAME

afw::double - Perl module for double functions

=head2 Functions

The following functions are exported by default

=head3 abs_double

Compute the absolute value of the double value and return the double result.
Absolute value

=head4 Parameters

    $value


=head3 add_double

Add 2 or more double values and return the double result.
Add

=head4 Parameters

    $values


=head3 at_least_one_member_of_double

Returns boolean true if at least one value in double list1 is in double
list2.
Checks for at least one value in common

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 bag_double

Takes any number of double values and returns a list of list.
Makes a list from values

=head4 Parameters

    $values


=head3 bag_size_double

This returns the integer number of values in list.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 ceil_double

Determine the smallest integer that is greater then or equal to the double
value and return the double result.
Round upward to nearest integer

=head4 Parameters

    $value


=head3 divide_double

Divide double dividend by double divisor and return the double quotient.
Divide numbers

=head4 Parameters

    $dividend


    $divisor


=head3 double

Converts value to data type double returning double result.
Convert to data type double

=head4 Parameters

    $value

Value to convert

=head3 eq_double

Determine if double arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_double

Determine if for double arg1 is equal to the value and data type of arg2 then
return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 floor_double

Determine the largest integer that is smaller then or equal to the double
value and return the double result.
Round downwards to nearest integer

=head4 Parameters

    $number


=head3 ge_double

Checks for double arg1 is greater than or equal to double arg2 and return the
boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_double

Checks for double arg1 is greater than double arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 intersection_double

Returns a list of double with the values that are common to both list of
double list1 and list2.
Returns intersection of two lists

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 is_NaN

Checks if the argument 'number' is not a number(NaN) and returns the boolean
result.
Is not a number(NaN)

=head4 Parameters

    $number

Number to check

=head3 is_double

Checks whether value is dataType double and return the boolean result.
Checks whether value is dataType double

=head4 Parameters

    $value

Value to check

=head3 is_finite

Checks if the argument 'number' is finite and returns the boolean result.
Is a number finite

=head4 Parameters

    $number

Number to check

=head3 is_in_double

Checks whether double value is in list of double list and returns the boolean
result.
Checks whether a value is in a list

=head4 Parameters

    $value


    $array


=head3 le_double

Checks for double arg1 is less than or equal to double arg2 and return the
boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_double

Checks for double arg1 is less that double arg2 and return the boolean
result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 max_double

Return the double value that is greater than or equal to the others.
Maximum value

=head4 Parameters

    $values


=head3 min_double

Return the double value that is less than or equal to the others.
Minimum value

=head4 Parameters

    $values


=head3 multiply_double

Multiply 2 or more double values and return the double result.
Multiply numbers

=head4 Parameters

    $values


=head3 ne_double

Determine if double arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead
if you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 negative_double

Return negative of double value.
Negative of a number

=head4 Parameters

    $value


=head3 nex_double

Determine if for double arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to
be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_double

This converts a list of double values that contains one value to a single
double value.
Converts a one value list to a double value

=head4 Parameters

    $array


=head3 pow_double

This returns the value of base raised to a power. Multiple exponents can be
specified to raise the previous exponent to the power of the latter exponent.
Base raised to a power

=head4 Parameters

    $base

Base value.

    $exponent

Exponent value.

=head3 round_double

Determine the integer closest to double value and return the double result.
Round to nearest integer

=head4 Parameters

    $number


=head3 set_equals_double

Returns boolean true if double list1 and double list2 are subsets of each
other and return the boolean result.
Checks whether two lists are subsets of each other

=head4 Parameters

    $list1


    $list2


=head3 subset_double

Returns boolean true if the unique values in double list1 are all in double
list2.
Determines if the first list is a subset of second list

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 subtract_double

Subtract double arg2 from double arg1 and return the double result.
Subtract numbers

=head4 Parameters

    $arg1


    $arg2


=head3 to_integer_double

Truncate double value to a whole number and returns integer result.
Converts to integer

=head4 Parameters

    $value


=head3 to_string_double

Converts double value to string. For list values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A double value.

=head3 union_double

Returns a list of double contains all of the unique values in two or more
list of double values.
Returns union of two or more string lists

=head4 Parameters

    $lists

Two or more lists.

=cut

sub abs_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "abs<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub add_double {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "add<double>");
    $request->set("values", $values);

    return $request->getResult();
}

sub at_least_one_member_of_double {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<double>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub bag_double {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<double>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub ceil_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "ceil<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub divide_double {
    my ($dividend, $divisor) = @_;

    my $request = $session->request()

    $request->set("function" => "divide<double>");
    $request->set("dividend", $dividend);
    $request->set("divisor", $divisor);

    return $request->getResult();
}

sub double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "double");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_double {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<double>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_double {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<double>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub floor_double {
    my ($number) = @_;

    my $request = $session->request()

    $request->set("function" => "floor<double>");
    $request->set("number", $number);

    return $request->getResult();
}

sub ge_double {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<double>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_double {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<double>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub intersection_double {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<double>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub is_NaN {
    my ($number) = @_;

    my $request = $session->request()

    $request->set("function" => "is_NaN");
    $request->set("number", $number);

    return $request->getResult();
}

sub is_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_finite {
    my ($number) = @_;

    my $request = $session->request()

    $request->set("function" => "is_finite");
    $request->set("number", $number);

    return $request->getResult();
}

sub is_in_double {
    my ($value, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<double>");
    $request->set("value", $value);
    $request->set("array", $array);

    return $request->getResult();
}

sub le_double {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<double>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_double {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<double>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub max_double {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "max<double>");
    $request->set("values", $values);

    return $request->getResult();
}

sub min_double {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "min<double>");
    $request->set("values", $values);

    return $request->getResult();
}

sub multiply_double {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "multiply<double>");
    $request->set("values", $values);

    return $request->getResult();
}

sub ne_double {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<double>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub negative_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "negative<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub nex_double {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<double>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_double {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<double>");
    $request->set("array", $array);

    return $request->getResult();
}

sub pow_double {
    my ($base, $exponent) = @_;

    my $request = $session->request()

    $request->set("function" => "pow<double>");
    $request->set("base", $base);
    $request->set("exponent", $exponent);

    return $request->getResult();
}

sub round_double {
    my ($number) = @_;

    my $request = $session->request()

    $request->set("function" => "round<double>");
    $request->set("number", $number);

    return $request->getResult();
}

sub set_equals_double {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<double>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub subset_double {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<double>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub subtract_double {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "subtract<double>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub to_integer_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_integer<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_string_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<double>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_double {
    my ($lists) = @_;

    my $request = $session->request()

    $request->set("function" => "union<double>");
    $request->set("lists", $lists);

    return $request->getResult();
}

42;
