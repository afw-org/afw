#!/usr/bin/env perl

#**
# @file array.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the array category.
#*

package afw::array;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    add_entries 
    array 
    array_array 
    as_array 
    at 
    clone_array 
    create_array 
    eq_array 
    eqx_array 
    fill 
    flat 
    freeze_array 
    ge_array 
    gt_array 
    includes_array 
    index_of_array 
    is_array 
    join 
    last_index_of_array 
    le_array 
    length_array 
    lt_array 
    ne_array 
    nex_array 
    pop 
    push 
    reverse 
    shift 
    slice 
    splice 
    to_reversed 
    to_spliced 
    to_string_array 
    unshift 
    with_at 
);

=head1 NAME

afw::array - Perl module for array functions

=head2 Functions

The following functions are exported by default

=head3 add_entries

Append every entry of each source array onto the end of target, in order.
target must be mutable. Entries are copied by value reference; nested objects
and arrays are not deeply cloned.
Append entries from arrays onto a target array

=head4 Parameters

    $target

Target array. This array must not be immutable.

    $source

Source array(s).

=head3 array

Construct a new array from the given values (not a conversion function). Each
argument becomes one element, in order. If a value is written as ...expression
and the expression is an array, each of its entries is included in order. An
empty call produces an empty array. A non-spread array argument is nested as a
single element (array([1,2]) is [[1,2]]); use spread or a list literal to
flatten. For a length of undefined slots use create_array(n).
Construct an array from values or spread arrays

=head4 Parameters

    $values

A value can refer to any adaptable value belonging to any data type or an
array expression. In the case of an array expression, indicated by '...'
followed by an expression that results in an array, every element within that
array is included in the newly created array.

=head3 array_array

Construct a new array of array from the given values, the same as array()
except that the new array is typed: each value, and each entry of a spread
array, must be dataType array, and adding a value of another data type to the
array later throws. A value of another data type is an error, never converted.
This is usually written `array<array>(`...values).
Construct an array of array from values or spread arrays

=head4 Parameters

    $values

A value of dataType array, or ... followed by an expression that results in an
array, every entry of which is included.

=head3 as_array

Returns value unchanged if it is dataType array. Otherwise, including when
value is undefined, throws an error. It never converts: to get a array from a
value of another data type, write a typed literal or call a to_T conversion
function where one exists. This is usually written value as array.
Asserts that value is dataType array

=head4 Parameters

    $value

Value to assert

=head3 at

Return the value at a zero-based index in an array, or the code point at a
zero-based index in a string as a one-code-point string (as ECMAScript at does
for both). Negative indexes count from the end (-1 is the last). If the index
is out of range, the result is undefined. Bracket indexing a[index] and
s[index] use the same out-of-range result (undefined).
Value at array index, or code point at string index

=head4 Parameters

    $array

Array, or string, to index.

    $index

Zero-based index, or negative from the end.

=head3 clone_array

Deep clone a array value.
Clone array value

=head4 Parameters

    $value

The array value to clone.

=head3 create_array

Create a new mutable array of the given length where every entry is value, or
undefined when value is not passed (ECMAScript new Array(n).fill(v), with no
holes). Useful when you want a known length up front before assigning entries;
assigning one past the end appends, further is an error. A computed array is
create_array(n)->map(function(v, i) { ... }). Length must be a non-negative
integer and must not exceed 1,000,000. This is a length-based constructor, not
a conversion function (see also array(...), which builds from elements).
Create an array of a given length

=head4 Parameters

    $length

Number of entries (0 or more, up to the maximum).

    $value

The value of every entry. A function is stored as the entry, not called; write
create_array(n)->map(fn) for computed entries. If not passed, every entry is
undefined.

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


=head3 fill

Set the entries of array from start up to (not including) end to value, in
place, and return the same array, as ECMAScript fill does. Negative indexes
count from the end and indexes out of range are clamped, as for slice. The
length of the array does not change. A frozen array is an error. A function
passed as value is stored, not called.
Fill array entries with a value, in place

=head4 Parameters

    $array

The array to fill. Must not be frozen.

    $value

The value to store in each entry.

    $start

The first index to fill. The default is 0. Negative counts from the end.

    $end

One more than the last index to fill. The default is the length. Negative
counts from the end.

=head3 flat

Return a new array with the entries of array, where an entry that is an array
is replaced by its entries, down to depth levels, as ECMAScript flat does. The
default depth is 1; a negative depth flattens every level (ECMAScript's
flat(Infinity)); 0 copies the array. Undefined entries are kept (Adaptive
arrays have no holes).
Flatten nested arrays

=head4 Parameters

    $array

The array to flatten.

    $depth

How many levels of nested arrays to flatten. The default is 1; a negative
depth flattens every level.

=head3 freeze_array

Set a array value immutable so further mutation throws. If already immutable,
has no effect. Returns the same value.
Make array value immutable

=head4 Parameters

    $value

The array value to freeze.

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

Checks whether an array contains searchElement, as ECMAScript includes does:
the same as index_of(array, searchElement, fromIndex) >= 0. Values are
compared by value (equal arrays and objects match) and NaN is found.
Checks whether an array contains a value

=head4 Parameters

    $array

The array to search.

    $searchElement

Element to find.

    $fromIndex

Index to start the search at. Negative counts from the end. At or past the
end, nothing is found. Default 0.

=head3 index_of_array

Return the index of the first entry of array equal to searchElement, searching
forward from fromIndex, or -1, as ECMAScript indexOf does. Values are compared
as includes compares them: by value (equal arrays and objects match), and NaN
is found, so includes(a, v) is always index_of(a, v) >= 0.
Index of the first array entry equal to a value

=head4 Parameters

    $array

The array to search.

    $searchElement

The value to find.

    $fromIndex

Index to start the search at. Negative counts from the end. At or past the
end, nothing is found. Default 0.

=head3 is_array

Checks whether value is dataType array and returns the boolean result. It is
false for undefined. This can also be written `is<array>(value`).
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

=head3 last_index_of_array

Return the index of the last entry of array equal to searchElement, searching
backward from fromIndex, or -1, as ECMAScript lastIndexOf does. Values are
compared as includes compares them: by value (equal arrays and objects match),
and NaN is found.
Index of the last array entry equal to a value

=head4 Parameters

    $array

The array to search.

    $searchElement

The value to find.

    $fromIndex

Index to start the search at, searching backward. Negative counts from the
end. Default: the last index.

=head3 le_array

Checks for array arg1 is less than or equal to array arg2 and return the
boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 length_array

Return the integer number of entries in a array array or the number of
codepoints in a array anyURI or string.
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


=head3 pop

Remove the last value from a mutable array and return it. If the array is
empty, returns undefined.
Remove and return last array value

=head4 Parameters

    $array

Target array. Must not be immutable.

=head3 push

Append one or more values to the end of a mutable array. Returns the new
length of the array, as ECMAScript push does. The array must not be immutable
(for example after freeze).
Append values to an array

=head4 Parameters

    $array

Target array. Must not be immutable.

    $values

Values to append in order.

=head3 reverse

Reverse the order of the entries of array in place and return the same array,
as in ECMAScript. A frozen array is an error. Use to_reversed for a reversed
copy.
Reverse an array in place

=head4 Parameters

    $array

The array to reverse. Must not be frozen.

=head3 shift

Remove the first value from a mutable array and return it. If the array is
empty, returns undefined.
Remove and return first array value

=head4 Parameters

    $array

Target array. Must not be immutable.

=head3 slice

This function extracts a consecutive slice of values from an array, or of code
points from a string (as ECMAScript slice does for both). For a string, or
another value stored as UTF-8, the result is a string.
Return a consecutive slice of an array or a string

=head4 Parameters

    $array

The array, or the string, to slice.

    $startIndex

This is the zero based starting index. Use negative number to index from the
end. If not specified, an index of 0 is assumed.

    $endIndex

This is the zero based ending index. If positive, this is one more than the
index of the last value or code point to include. If negative, the index is
from the end. If not specified, the slice is from startIndex up to and
including the end.

=head3 splice

Remove zero or more values starting at an index from a mutable array and
optionally insert new values at that index. Returns an array of the removed
values. Negative startIndex counts from the end. If deleteCount is omitted,
all values from startIndex to the end are removed.
Change array contents by removing and/or inserting values

=head4 Parameters

    $array

Target array. Must not be immutable.

    $startIndex

Zero-based start index, or negative from the end.

    $deleteCount

Number of values to remove. If omitted, remove through the end of the array.
Negative is treated as zero.

    $values

Values to insert at startIndex after removals.

=head3 to_reversed

Return a new array with the entries of array in reverse order, leaving array
unchanged, as ECMAScript toReversed does. The new array is an `array<T>` only
if array is one.
Reversed copy of an array

=head4 Parameters

    $array

The array to copy reversed.

=head3 to_spliced

Return a new array that is array with the changes splice would make, leaving
array unchanged, as ECMAScript toSpliced does: deleteCount values removed at
startIndex and values inserted there. Negative startIndex counts from the end.
If deleteCount is omitted, all values from startIndex to the end are removed.
Copy of an array with values removed and/or inserted

=head4 Parameters

    $array

The array to copy.

    $startIndex

Zero-based start index, or negative from the end.

    $deleteCount

Number of values to remove. If omitted, remove through the end of the array.
Negative is treated as zero.

    $values

Values to insert at startIndex after removals.

=head3 to_string_array

Converts array value to string. For array values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A array value.

=head3 unshift

Insert one or more values at the beginning of a mutable array, preserving the
relative order of the inserted values. Returns the new length of the array, as
ECMAScript unshift does. The array must not be immutable (for example after
freeze).
Insert values at the front of an array

=head4 Parameters

    $array

Target array. Must not be immutable.

    $values

Values to insert at the front, in order.

=head3 with_at

Return a new array that is array with the value at index replaced by value,
leaving array unchanged, as ECMAScript with does (with is a reserved word).
Negative index counts from the end, as in at. An index out of range is an
error.
Copy of an array with one value replaced

=head4 Parameters

    $array

The array to copy.

    $index

Zero-based index, or negative from the end.

    $value

The value to put at index.

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

sub array_array {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "array_array");
    $request->set("values", $values);

    return $request->getResult();
}

sub as_array {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "as_array");
    $request->set("value", $value);

    return $request->getResult();
}

sub at {
    my ($array, $index) = @_;

    my $request = $session->request()

    $request->set("function" => "at");
    $request->set("array", $array);
    $request->set("index", $index);

    return $request->getResult();
}

sub clone_array {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "clone<array>");
    $request->set("value", $value);

    return $request->getResult();
}

sub create_array {
    my ($length, $value) = @_;

    my $request = $session->request()

    $request->set("function" => "create_array");
    $request->set("length", $length);

    if (defined $value)
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

sub fill {
    my ($array, $value, $start, $end) = @_;

    my $request = $session->request()

    $request->set("function" => "fill");
    $request->set("array", $array);
    $request->set("value", $value);

    if (defined $start)
        $request->set("start", $start);

    if (defined $end)
        $request->set("end", $end);

    return $request->getResult();
}

sub flat {
    my ($array, $depth) = @_;

    my $request = $session->request()

    $request->set("function" => "flat");
    $request->set("array", $array);

    if (defined $depth)
        $request->set("depth", $depth);

    return $request->getResult();
}

sub freeze_array {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "freeze<array>");
    $request->set("value", $value);

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

sub index_of_array {
    my ($array, $searchElement, $fromIndex) = @_;

    my $request = $session->request()

    $request->set("function" => "index_of<array>");
    $request->set("array", $array);
    $request->set("searchElement", $searchElement);

    if (defined $fromIndex)
        $request->set("fromIndex", $fromIndex);

    return $request->getResult();
}

sub is_array {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is_array");
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

sub last_index_of_array {
    my ($array, $searchElement, $fromIndex) = @_;

    my $request = $session->request()

    $request->set("function" => "last_index_of<array>");
    $request->set("array", $array);
    $request->set("searchElement", $searchElement);

    if (defined $fromIndex)
        $request->set("fromIndex", $fromIndex);

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

sub pop {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "pop");
    $request->set("array", $array);

    return $request->getResult();
}

sub push {
    my ($array, $values) = @_;

    my $request = $session->request()

    $request->set("function" => "push");
    $request->set("array", $array);

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub reverse {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "reverse");
    $request->set("array", $array);

    return $request->getResult();
}

sub shift {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "shift");
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

sub splice {
    my ($array, $startIndex, $deleteCount, $values) = @_;

    my $request = $session->request()

    $request->set("function" => "splice");
    $request->set("array", $array);
    $request->set("startIndex", $startIndex);

    if (defined $deleteCount)
        $request->set("deleteCount", $deleteCount);

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub to_reversed {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "to_reversed");
    $request->set("array", $array);

    return $request->getResult();
}

sub to_spliced {
    my ($array, $startIndex, $deleteCount, $values) = @_;

    my $request = $session->request()

    $request->set("function" => "to_spliced");
    $request->set("array", $array);
    $request->set("startIndex", $startIndex);

    if (defined $deleteCount)
        $request->set("deleteCount", $deleteCount);

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub to_string_array {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<array>");
    $request->set("value", $value);

    return $request->getResult();
}

sub unshift {
    my ($array, $values) = @_;

    my $request = $session->request()

    $request->set("function" => "unshift");
    $request->set("array", $array);

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub with_at {
    my ($array, $index, $value) = @_;

    my $request = $session->request()

    $request->set("function" => "with_at");
    $request->set("array", $array);
    $request->set("index", $index);
    $request->set("value", $value);

    return $request->getResult();
}

42;
