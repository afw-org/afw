#!/usr/bin/env perl

#**
# @file higher_order_array.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the higher_order_array category.
#*

package afw::higher_order_array;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    all_of 
    all_of_all 
    all_of_any 
    any_of 
    any_of_all 
    any_of_any 
    every 
    filter 
    find 
    find_index 
    map 
    reduce 
    some 
    sort 
    to_sorted 
);

=head1 NAME

afw::higher_order_array - Perl module for higher_order_array functions

=head2 Functions

The following functions are exported by default

=head3 all_of

Return true if predicate returns true for every entry of the first array in
values (index order), or if that array is empty. Entries whose value is
undefined are included. every() is an alias for the common single-array form.
Whether every array entry passes a test

=head4 Parameters

    $predicate

This function is called for each value in the first array in values or until
false is returned. If no calls return false, the result is true.

    $values

These are the parameters passed to predicate with the exception that the first
array is passed one value at a time. At least one array is required.

=head3 all_of_all

Returns true if the result of calling predicate with all of the combination of
values from array1 and array2 returns true.
All combinations true

=head4 Parameters

    $predicate

The predicate is passed two parameters, the first is a value from array1 and
the second is a value from array2.

    $array1


    $array2


=head3 all_of_any

This function returns true if the result of calling predicate with all of the
combination of values from array1 and any of the values of array2 returns
true.
Combinations of all in array1 and any in array2 true

=head4 Parameters

    $predicate

The predicate is passed two parameters, the first is a value from array1 and
the second is a value from array2.

    $array1


    $array2


=head3 any_of

Return true if predicate returns true for any entry of the first array in
values (index order). Entries whose value is undefined are included. Empty
array yields false. some() is an alias for the common single-array form.
Whether any array entry passes a test

=head4 Parameters

    $predicate

This function is called for each value in the first array in values or until
true is returned. If no calls return true, the result is false.

    $values

These are the parameters passed to predicate with the exception that the first
array is passed one value at a time. At least one array is required.

=head3 any_of_all

Returns true if, for at least one value of array1, the predicate returns true
with every value of array2 (the predicate gets the value from array1 first).
Combinations of any in array1 and all in array2 true

=head4 Parameters

    $predicate

The predicate is passed two parameters, the first is a value from array1 and
the second is a value from array2.

    $array1


    $array2


=head3 any_of_any

This function returns true if the result of calling predicate with any of the
combination of values from array1 and array2 returns true.
Any combinations true

=head4 Parameters

    $predicate

The predicate is passed two parameters, the first is a value from array1 and
the second is a value from array2.

    $array1


    $array2


=head3 every

Return true if predicate returns true for every entry of array (index order),
or if array is empty. Entries whose value is undefined are included.
Whether every array entry passes a test

=head4 Parameters

    $array

The array to walk. A string is not walked as an array: write [...value] to
walk its characters.

    $predicate

Called for each entry of array until it returns false. A script function is
called with (value, index, array); a built-in function is called with the
value alone, since its other parameters have their own meanings.

=head3 filter

Return a new array of the entries of array for which predicate returns true.
Every index is considered, including entries whose value is undefined. Order
of kept entries is preserved.
Select array entries that pass a test

=head4 Parameters

    $array

The array to walk. A string is not walked as an array: write [...value] to
walk its characters.

    $predicate

Returns true to keep the entry. A script function is called with (value,
index, array); a built-in function is called with the value alone, since its
other parameters have their own meanings.

=head3 find

Call predicate for each entry of array, in index order, until it returns true,
then return that entry. Entries whose value is undefined are included. If no
entry passes, the result is undefined (the same as a found undefined entry;
use find_index if you need to tell those apart).
First array entry that passes a test

=head4 Parameters

    $array

The array to walk. A string is not walked as an array: write [...value] to
walk its characters.

    $predicate

Returns true for the entry to return. A script function is called with (value,
index, array); a built-in function is called with the value alone, since its
other parameters have their own meanings.

=head3 find_index

Call predicate for each entry of array, in index order, until it returns true,
then return that entry's index. Entries whose value is undefined are included.
If no entry passes, the result is -1.
Index of the first array entry that passes a test

=head4 Parameters

    $array

The array to walk. A string is not walked as an array: write [...value] to
walk its characters.

    $predicate

Returns true for the entry whose index to return. A script function is called
with (value, index, array); a built-in function is called with the value
alone, since its other parameters have their own meanings.

=head3 map

Call callbackfn once for each entry of array, in index order from 0 through
length minus one, and return a new array of the same length with the results.
Entries whose value is undefined (including omitted elements in array
literals) are included; callbackfn receives undefined for those indexes.
Map each array value through a function

=head4 Parameters

    $array

The array to walk. A string is not walked as an array: write [...value] to
walk its characters.

    $callbackfn

Called for each entry of array; its result is the entry of the new array at
the same index. A script function is called with (value, index, array); a
built-in function is called with the value alone, since its other parameters
have their own meanings.

=head3 reduce

Call callbackfn for each entry of array, in index order, with the accumulator
and that entry; each call's result is the accumulator for the next call. If
initialValue is passed, it is the first accumulator and every entry is
visited; otherwise the first entry is the first accumulator and the walk
starts at the second. Undefined entries are visited. An empty array with no
initialValue is an error.
Reduce array entries to a single value

=head4 Parameters

    $array

The array to walk. A string is not walked as an array: write [...value] to
walk its characters.

    $callbackfn

Called for each entry visited; its result is the next accumulator. A script
function is called with (accumulator, value, index, array); a built-in
function is called with (accumulator, value).

    $initialValue

The first accumulator. When it is not passed, the first entry of array is.

=head3 some

Return true if predicate returns true for any entry of array (index order).
Entries whose value is undefined are included. An empty array yields false.
Whether any array entry passes a test

=head4 Parameters

    $array

The array to walk. A string is not walked as an array: write [...value] to
walk its characters.

    $predicate

Called for each entry of array until it returns true. A script function is
called with (value, index, array); a built-in function is called with the
value alone, since its other parameters have their own meanings.

=head3 sort

Sort the entries of array in place, ordered by compareFn, or, with no
compareFn, as < orders them, and return the same array, as in ECMAScript. The
sort is stable: entries that compare equal keep their order. With no
compareFn, every entry must be of one data type (a mix is an error, as it is
for <); unlike ECMAScript, entries are not converted to strings, so [10, 9, 1]
sorts to [1, 9, 10]. NaN sorts after every other double. With compareFn, the
array may hold any mix of data types. Undefined entries go last and are not
compared, as in ECMAScript. A frozen array is an error before compareFn is
called. Use to_sorted for a sorted copy.
Sort an array in place

=head4 Parameters

    $array

The array to sort. Must not be frozen.

    $compareFn

Called with two defined entries, once per comparison. If omitted, entries are
ordered as < orders them. Return true, or a negative integer, when a sorts
before b; false, zero or a positive integer otherwise. Entries that compare
equal keep their order. Any other result, a double included, is an error.

=head3 to_sorted

Return a new array with the entries of array ordered by compareFn, or, with no
compareFn, as < orders them, leaving array unchanged, as ECMAScript toSorted
does. The sort is stable: entries that compare equal keep their order.
Undefined entries go last and are not compared. With no compareFn, every entry
must be of one data type, and NaN sorts after every other double. The new
array is an `array<T>` only if array is one.
Sorted copy of an array

=head4 Parameters

    $array

The array to sort.

    $compareFn

Called with two defined entries, once per comparison. If omitted, entries are
ordered as < orders them. Return true, or a negative integer, when a sorts
before b; false, zero or a positive integer otherwise. Entries that compare
equal keep their order. Any other result, a double included, is an error.

=cut

sub all_of {
    my ($predicate, $values) = @_;

    my $request = $session->request()

    $request->set("function" => "all_of");
    $request->set("predicate", $predicate);
    $request->set("values", $values);

    return $request->getResult();
}

sub all_of_all {
    my ($predicate, $array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "all_of_all");
    $request->set("predicate", $predicate);
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub all_of_any {
    my ($predicate, $array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "all_of_any");
    $request->set("predicate", $predicate);
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub any_of {
    my ($predicate, $values) = @_;

    my $request = $session->request()

    $request->set("function" => "any_of");
    $request->set("predicate", $predicate);
    $request->set("values", $values);

    return $request->getResult();
}

sub any_of_all {
    my ($predicate, $array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "any_of_all");
    $request->set("predicate", $predicate);
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub any_of_any {
    my ($predicate, $array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "any_of_any");
    $request->set("predicate", $predicate);
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub every {
    my ($array, $predicate) = @_;

    my $request = $session->request()

    $request->set("function" => "every");
    $request->set("array", $array);
    $request->set("predicate", $predicate);

    return $request->getResult();
}

sub filter {
    my ($array, $predicate) = @_;

    my $request = $session->request()

    $request->set("function" => "filter");
    $request->set("array", $array);
    $request->set("predicate", $predicate);

    return $request->getResult();
}

sub find {
    my ($array, $predicate) = @_;

    my $request = $session->request()

    $request->set("function" => "find");
    $request->set("array", $array);
    $request->set("predicate", $predicate);

    return $request->getResult();
}

sub find_index {
    my ($array, $predicate) = @_;

    my $request = $session->request()

    $request->set("function" => "find_index");
    $request->set("array", $array);
    $request->set("predicate", $predicate);

    return $request->getResult();
}

sub map {
    my ($array, $callbackfn) = @_;

    my $request = $session->request()

    $request->set("function" => "map");
    $request->set("array", $array);
    $request->set("callbackfn", $callbackfn);

    return $request->getResult();
}

sub reduce {
    my ($array, $callbackfn, $initialValue) = @_;

    my $request = $session->request()

    $request->set("function" => "reduce");
    $request->set("array", $array);
    $request->set("callbackfn", $callbackfn);

    if (defined $initialValue)
        $request->set("initialValue", $initialValue);

    return $request->getResult();
}

sub some {
    my ($array, $predicate) = @_;

    my $request = $session->request()

    $request->set("function" => "some");
    $request->set("array", $array);
    $request->set("predicate", $predicate);

    return $request->getResult();
}

sub sort {
    my ($array, $compareFn) = @_;

    my $request = $session->request()

    $request->set("function" => "sort");
    $request->set("array", $array);

    if (defined $compareFn)
        $request->set("compareFn", $compareFn);

    return $request->getResult();
}

sub to_sorted {
    my ($array, $compareFn) = @_;

    my $request = $session->request()

    $request->set("function" => "to_sorted");
    $request->set("array", $array);

    if (defined $compareFn)
        $request->set("compareFn", $compareFn);

    return $request->getResult();
}

42;
