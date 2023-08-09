package afw::hexBinary;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    at_least_one_member_of_hexBinary 
    bag_hexBinary 
    bag_size_hexBinary 
    decode_to_string_hexBinary 
    eq_hexBinary 
    eqx_hexBinary 
    ge_hexBinary 
    gt_hexBinary 
    hexBinary 
    intersection_hexBinary 
    is_hexBinary 
    is_in_hexBinary 
    le_hexBinary 
    lt_hexBinary 
    ne_hexBinary 
    nex_hexBinary 
    one_and_only_hexBinary 
    set_equals_hexBinary 
    subset_hexBinary 
    to_string_hexBinary 
    union_hexBinary 
);

=head1 NAME

afw::hexBinary - Perl module for hexBinary functions

=head2 Functions

The following functions are exported by default

=head3 at_least_one_member_of_hexBinary

Returns boolean true if at least one value in hexBinary array1 is in
hexBinary array2.
Checks for at least one value in common

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 bag_hexBinary

Takes any number of hexBinary values and returns an array of array.
Makes an array from values

=head4 Parameters

    $values


=head3 bag_size_hexBinary

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 decode_to_string_hexBinary

Decode hexBinary value to string. An error is thrown if decoded value is not
valid UTF-8.
Decode hexBinary to string

=head4 Parameters

    $value

The hexBinary value to decode.

=head3 eq_hexBinary

Determine if hexBinary arg1 is equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
if you want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_hexBinary

Determine if for hexBinary arg1 is equal to the value and data type of arg2
then return the boolean result. Use 'eq' ('==') instead if you want arg2 to
be converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_hexBinary

Checks for hexBinary arg1 is greater than or equal to hexBinary arg2 and
return the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_hexBinary

Checks for hexBinary arg1 is greater than hexBinary arg2 and return the
boolean result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 hexBinary

Converts value to data type hexBinary returning hexBinary result.
Convert to data type hexBinary

=head4 Parameters

    $value

Value to convert

=head3 intersection_hexBinary

Returns an array of hexBinary with the values that are common to both array
of hexBinary array1 and array2.
Returns intersection of two arrays

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 is_hexBinary

Checks whether value is dataType hexBinary and return the boolean result.
Checks whether value is dataType hexBinary

=head4 Parameters

    $value

Value to check

=head3 is_in_hexBinary

Checks whether hexBinary value is in array of hexBinary array and returns the
boolean result.
Checks whether a value is in an array

=head4 Parameters

    $value


    $array


=head3 le_hexBinary

Checks for hexBinary arg1 is less than or equal to hexBinary arg2 and return
the boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_hexBinary

Checks for hexBinary arg1 is less that hexBinary arg2 and return the boolean
result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_hexBinary

Determine if hexBinary arg1 is not equal to the value of arg2 converted to
the data type of arg1 then return the boolean result. Use 'nex' ('!==')
instead if you want true to be returned if arg1 and arg2's data type don't
match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_hexBinary

Determine if for hexBinary arg1 is not equal to the value or data type of
arg2 then return the boolean result. Use 'ne' ('!=') instead if you want arg2
to be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_hexBinary

This converts an array of hexBinary values that contains one value to a
single hexBinary value.
Converts a one value array to a hexBinary value

=head4 Parameters

    $array


=head3 set_equals_hexBinary

Returns boolean true if hexBinary array1 and hexBinary array2 are subsets of
each other and return the boolean result.
Checks whether two arrays are subsets of each other

=head4 Parameters

    $array1


    $array2


=head3 subset_hexBinary

Returns boolean true if the unique values in hexBinary array1 are all in
hexBinary array2.
Determines if the first array is a subset of second array

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 to_string_hexBinary

Converts hexBinary value to string. For array values, the to_string() value
for each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A hexBinary value.

=head3 union_hexBinary

Returns an array of hexBinary contains all of the unique values in two or
more array of hexBinary values.
Returns union of two or more string arrays

=head4 Parameters

    $arrays

Two or more arrays.

=cut

sub at_least_one_member_of_hexBinary {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<hexBinary>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub bag_hexBinary {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<hexBinary>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_hexBinary {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<hexBinary>");
    $request->set("value", $value);

    return $request->getResult();
}

sub decode_to_string_hexBinary {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "decode_to_string<hexBinary>");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_hexBinary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<hexBinary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_hexBinary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<hexBinary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_hexBinary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<hexBinary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_hexBinary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<hexBinary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub hexBinary {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "hexBinary");
    $request->set("value", $value);

    return $request->getResult();
}

sub intersection_hexBinary {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<hexBinary>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub is_hexBinary {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<hexBinary>");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_in_hexBinary {
    my ($value, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<hexBinary>");
    $request->set("value", $value);
    $request->set("array", $array);

    return $request->getResult();
}

sub le_hexBinary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<hexBinary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_hexBinary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<hexBinary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_hexBinary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<hexBinary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_hexBinary {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<hexBinary>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_hexBinary {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<hexBinary>");
    $request->set("array", $array);

    return $request->getResult();
}

sub set_equals_hexBinary {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<hexBinary>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub subset_hexBinary {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<hexBinary>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub to_string_hexBinary {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<hexBinary>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_hexBinary {
    my ($arrays) = @_;

    my $request = $session->request()

    $request->set("function" => "union<hexBinary>");
    $request->set("arrays", $arrays);

    return $request->getResult();
}

42;
