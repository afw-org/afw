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
    map 
    reduce 
    some 
    sort 
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

Returns true if the result of calling predicate with all of the combination of
values from array2 and any of the values of array1 returns true.
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

Return true if predicate returns true for every entry of the first array in
values (index order), or if that array is empty. Entries whose value is
undefined are included. Same single-array behavior as all_of.
Whether every array entry passes a test

=head4 Parameters

    $predicate

Called for each value in the first array in values or until false is returned.

    $values

Parameters passed to predicate with the first array passed one value at a
time.

=head3 filter

Return a new array of entries from the first array in values for which
predicate returns true. Every index is considered, including entries whose
value is undefined. Order of kept entries is preserved.
Select array entries that pass a test

=head4 Parameters

    $predicate

This is a boolean function that is called to determine if an array entry
should be included in the returned array.

    $values

These are the values passed to the predicate with the exception that the first
array is passed as the single current value from the array. At least one array
is required.

=head3 find

Call predicate for each entry of the first array in values, in index order,
until it returns true, then return that entry. Entries whose value is
undefined are included. If no entry passes, the result is undefined (the same
as a found undefined entry; use filter if you need to tell those apart).
First array entry that passes a test

=head4 Parameters

    $predicate

This is a boolean function that is called to determine if an array entry
passes the test.

    $values

These are the values passed to the predicate with the exception that the first
array is passed as the single current value from the array. At least one array
is required.

=head3 map

Call functor once for each entry of the first array in values, in index order
from 0 through length minus one, and return a new array of the same length
with the results. Entries whose value is undefined (including omitted elements
in array literals) are included; the functor receives undefined for those
indexes. Additional values parameters, if present, are passed through on every
call.
Map each array value through a function

=head4 Parameters

    $functor


    $values

The first array is walked one entry at a time as the first argument to
functor. Additional parameters are passed on every call. At least one array is
required.

=head3 reduce

Call functor for each entry of array, in index order, with the current
accumulator and that entry. The first call uses the accumulator argument; each
later call uses the previous return value. Every index is visited, including
undefined entries. If array is empty, the accumulator argument is returned
without calling functor.
Reduce array entries to a single value

=head4 Parameters

    $functor

This function is called for each value in an array. The returned value is
passed as the accumulator parameter on the next call to functor().

    $accumulator

This is an initial accumulator value passed to functor(). Normally, the
dataType of accumulator will be the data type for the reduce() return value,
but this is not required.

    $array

This is an array to be reduced.

=head3 some

Return true if predicate returns true for any entry of the first array in
values (index order). Entries whose value is undefined are included. Empty
array yields false. Same single-array behavior as any_of.
Whether any array entry passes a test

=head4 Parameters

    $predicate

Called for each value in the first array in values or until true is returned.

    $values

Parameters passed to predicate with the first array passed one value at a
time.

=head3 sort

Return a new array with the same entries as array, ordered using
compareFunction. The array must have a single element data type (for example
all integers or all strings); mixed or empty untyped arrays are not accepted.
compareFunction is called with two entries and must return true when the first
should sort before the second (boolean), not a numeric sort key.
Return a sorted copy of a single-type array

=head4 Parameters

    $compareFunction

Return true if value1 should be ordered before value2.

    $array

Array to sort. Must be single-type (all entries the same data type).

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
    my ($predicate, $values) = @_;

    my $request = $session->request()

    $request->set("function" => "every");
    $request->set("predicate", $predicate);
    $request->set("values", $values);

    return $request->getResult();
}

sub filter {
    my ($predicate, $values) = @_;

    my $request = $session->request()

    $request->set("function" => "filter");
    $request->set("predicate", $predicate);
    $request->set("values", $values);

    return $request->getResult();
}

sub find {
    my ($predicate, $values) = @_;

    my $request = $session->request()

    $request->set("function" => "find");
    $request->set("predicate", $predicate);
    $request->set("values", $values);

    return $request->getResult();
}

sub map {
    my ($functor, $values) = @_;

    my $request = $session->request()

    $request->set("function" => "map");
    $request->set("functor", $functor);
    $request->set("values", $values);

    return $request->getResult();
}

sub reduce {
    my ($functor, $accumulator, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "reduce");
    $request->set("functor", $functor);
    $request->set("accumulator", $accumulator);
    $request->set("array", $array);

    return $request->getResult();
}

sub some {
    my ($predicate, $values) = @_;

    my $request = $session->request()

    $request->set("function" => "some");
    $request->set("predicate", $predicate);
    $request->set("values", $values);

    return $request->getResult();
}

sub sort {
    my ($compareFunction, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "sort");
    $request->set("compareFunction", $compareFunction);
    $request->set("array", $array);

    return $request->getResult();
}

42;
