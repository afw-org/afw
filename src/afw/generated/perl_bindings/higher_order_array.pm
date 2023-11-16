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
    filter 
    find 
    map 
    reduce 
    sort 
);

=head1 NAME

afw::higher_order_array - Perl module for higher_order_array functions

=head2 Functions

The following functions are exported by default

=head3 all_of

Returns true if all values in an array pass the predicate test.
All values pass a test

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

Returns true if any value in an array pass the predicate test.
Any values pass a test

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


=head3 filter

This produces an array containing only values from another array that pass a
predicate test.
Filter an array

=head4 Parameters

    $predicate

This is a boolean function that is called to determine if an array entry
should be included in the returned array.

    $values

These are the values passed to the predicate with the exception that the first
array is passed as the single current value from the array. At least one array
is required.

=head3 find

The predicate is called for each value in the first array in values until true
is returned, then that value is returned.
Returns the first value in an array that passes a test

=head4 Parameters

    $predicate

This is a boolean function that is called to determine if an array entry
passes the test.

    $values

These are the values passed to the predicate with the exception that the first
array is passed as the single current value from the array. At least one array
is required.

=head3 map

This function creates an array of the results of calling functor with each
value of the first array in values
Maps values of an array

=head4 Parameters

    $functor


    $values

These are the parameters passed to functor with the exception that the first
array is passed one value at a time. At least one array is required.

=head3 reduce

Reduce calls functor for each value in array with two parameters, accumulator
and value, and must return a value of any dataType. Parameter accumulator is
the reduce() accumulator parameter value on first call and the return value of
previous functor() call on subsequent calls. The dataType of the return value
should normally be the same as accumulator, but this is not required.
Reduce values of an array to a single value

=head4 Parameters

    $functor

This function is called for each value in an array. The returned value is
passed as the accumulator parameter on the next call to functor().

    $accumulator

This is an initial accumulator value passed to functor(). Normally, the
dataType of accumulator will be the dataTape for the reduce() return value,
but this is not required.

    $array

This is an array to be reduced.

=head3 sort

This produces an array with values sorted based on result of compareFunction.
The compareFunction is passed two values from the array and must return an
integer less than 0 if the first value is less than the second value, 0 if
they are equal, and a integer greater than 0 if the first value is greater
than the second value.
Sort values in an array

=head4 Parameters

    $compareFunction

This function is called with two value from array.

    $array

This is the array to sort.

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

sub sort {
    my ($compareFunction, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "sort");
    $request->set("compareFunction", $compareFunction);
    $request->set("array", $array);

    return $request->getResult();
}

42;
