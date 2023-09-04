package afw::array;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    add_entries 
    array 
    bag_array 
    bag_size_array 
    clone_array 
    eq_array 
    eqx_array 
    ge_array 
    gt_array 
    includes_array 
    is_array 
    join 
    le_array 
    length_array 
    lt_array 
    ne_array 
    nex_array 
    reverse 
    slice 
    to_string_array 
);

=head1 NAME

afw::array - Perl module for array functions

=head2 Functions

The following functions are exported by default

=head3 add_entries

Add the entries of one or more arrays to another.
Add entries of one or more arrays to another

=head4 Parameters

    $target

Target array. This array must not be immutable.

    $source

Source array(s).

=head3 array

Construct an array with 0 or more elements.
Construct an array with 0 or more elements

=head4 Parameters

    $values

A value can refer to any adaptable value belonging to any data type or an
array expression. In the case of an array expression, indicated by '...'
followed by an expression that results in an array, every element within that
array is included in the newly created array.

=head3 bag_array

Takes any number of array values and returns an array of array.
Makes an array from values

=head4 Parameters

    $values


=head3 bag_size_array

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 clone_array

Deep clone a array value.
Clone array value

=head4 Parameters

    $value

The array value to clone.

=head3 eq_array

Determine if array arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_array

Determine if for array arg1 is equal to the value and data type of arg2 then
return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_array

Checks for array arg1 is greater than or equal to array arg2 and return the
boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_array

Checks for array arg1 is greater than array arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 includes_array

Checks whether or not an array contains any value.
Checks whether or not an array contains any value

=head4 Parameters

    $array

The array to search.

    $searchElement

Element to find.

    $fromIndex

Index in the array to start search.

=head3 is_array

Checks whether value is dataType array and return the boolean result.
Checks whether value is dataType array

=head4 Parameters

    $value

Value to check

=head3 join

Concatenate the string values of the elements of an array with a separator.
Join the string values of array elements

=head4 Parameters

    $value

An array of values (statements) of any data type.

    $separator

The separator to use. If not specified, a comma (,) is used.

=head3 le_array

Checks for array arg1 is less than or equal to array arg2 and return the
boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 length_array

This is a polymorphic function where array can be any of the supported data
types. Return the integer number of entries in datatype array or codepoints in
others.
Returns number of codepoints or entries in value

=head4 Parameters

    $value

Returns the number of entries in an array or code points in others.

=head3 lt_array

Checks for array arg1 is less that array arg2 and return the boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_array

Determine if array arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead if
you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_array

Determine if for array arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 reverse

Reverse the order of the elements in an array. If the array is typed, the
resulting array will be the same type.
Return array with elements reversed

=head4 Parameters

    $array

An array to reverse.

=head3 slice

This function extracts a consecutive slice of values from an array.
Return a consecutive slice of values from an array

=head4 Parameters

    $array

The array to slice.

    $startIndex

This is the zero based starting index. Use negative number to index from the
end of the array. If not specified, an index of 0 is assumed.

    $endIndex

This is the zero based ending index. If positive, this is one more than the
index of the last value to include in the array. If negative, the index is
from the end of the array. If not specified, the slice is from startIndex up
to and including the end of the array.

=head3 to_string_array

Converts array value to string. For array values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A array value.

=cut

sub add_entries {
    my ($target, $source) = @_;

    my $request = $session->request()

    $request->set("function" => "add_entries");
    $request->set("target", $target);
    $request->set("source", $source);

    return $request->getResult();
}

sub array {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "array");
    $request->set("values", $values);

    return $request->getResult();
}

sub bag_array {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<array>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_array {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<array>");
    $request->set("value", $value);

    return $request->getResult();
}

sub clone_array {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "clone<array>");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_array {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<array>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_array {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<array>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_array {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<array>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_array {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<array>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub includes_array {
    my ($array, $searchElement, $fromIndex) = @_;

    my $request = $session->request()

    $request->set("function" => "includes<array>");
    $request->set("array", $array);
    $request->set("searchElement", $searchElement);

    if (defined $fromIndex)
        $request->set("fromIndex", $fromIndex);

    return $request->getResult();
}

sub is_array {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<array>");
    $request->set("value", $value);

    return $request->getResult();
}

sub join {
    my ($value, $separator) = @_;

    my $request = $session->request()

    $request->set("function" => "join");
    $request->set("value", $value);

    if (defined $separator)
        $request->set("separator", $separator);

    return $request->getResult();
}

sub le_array {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<array>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub length_array {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "length<array>");
    $request->set("value", $value);

    return $request->getResult();
}

sub lt_array {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<array>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_array {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<array>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_array {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<array>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub reverse {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "reverse");
    $request->set("array", $array);

    return $request->getResult();
}

sub slice {
    my ($array, $startIndex, $endIndex) = @_;

    my $request = $session->request()

    $request->set("function" => "slice");
    $request->set("array", $array);

    if (defined $startIndex)
        $request->set("startIndex", $startIndex);

    if (defined $endIndex)
        $request->set("endIndex", $endIndex);

    return $request->getResult();
}

sub to_string_array {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<array>");
    $request->set("value", $value);

    return $request->getResult();
}

42;
