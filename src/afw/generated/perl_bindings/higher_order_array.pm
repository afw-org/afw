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

Returns true if all values in a list pass the predicate test.
All values pass a test

=head4 Parameters

    $predicate

This function is called for each value in the first list in values or until
false is returned. If no calls return false, the result is true.

    $values

These are the parameters passed to predicate with the exception that the
first list is passed one value at a time. At least one list is required.

=head3 all_of_all

Returns true if the result of calling predicate with all of the combination
of values from list1 and list2 returns true.
All combinations true

=head4 Parameters

    $predicate

The predicate is passed two parameters, the first is a value from list1 and
the second is a value from list2.

    $list1


    $list2


=head3 all_of_any

This function returns true if the result of calling predicate with all of the
combination of values from list1 and any of the values of list2 returns true.
Combinations of all in list1 and any in list2 true

=head4 Parameters

    $predicate

The predicate is passed two parameters, the first is a value from list1 and
the second is a value from list2.

    $list1


    $list2


=head3 any_of

Returns true if any value in a list pass the predicate test.
Any values pass a test

=head4 Parameters

    $predicate

This function is called for each value in the first list in values or until
true is returned. If no calls return true, the result is false.

    $values

These are the parameters passed to predicate with the exception that the
first list is passed one value at a time. At least one list is required.

=head3 any_of_all

Returns true if the result of calling predicate with all of the combination
of values from list2 and any of the values of list1 returns true.
Combinations of any in list1 and all in list2 true

=head4 Parameters

    $predicate

The predicate is passed two parameters, the first is a value from list1 and
the second is a value from list2.

    $list1


    $list2


=head3 any_of_any

This function returns true if the result of calling predicate with any of the
combination of values from list1 and list2 returns true.
Any combinations true

=head4 Parameters

    $predicate

The predicate is passed two parameters, the first is a value from list1 and
the second is a value from list2.

    $list1


    $list2


=head3 filter

This produces a list containing only values from another list that pass a
predicate test.
Filter a list

=head4 Parameters

    $predicate

This is a boolean function that is called to determine if a list entry should
be included in the returned list.

    $values

These are the values passed to the predicate with the exception that the
first list is passed as the single current value from the list. At least one
list is required.

=head3 find

The predicate is called for each value in the first list in values until true
is returned, then that value is returned.
Returns the first value in a list that passes a test

=head4 Parameters

    $predicate

This is a boolean function that is called to determine if a list entry passes
the test.

    $values

These are the values passed to the predicate with the exception that the
first list is passed as the single current value from the list. At least one
list is required.

=head3 map

This function creates a list of the results of calling functor with each
value of the first list in values
Maps values of a list

=head4 Parameters

    $functor


    $values

These are the parameters passed to functor with the exception that the first
list is passed one value at a time. At least one list is required.

=head3 reduce

Reduce calls functor for each value in list with two parameters, accumulator
and value, and must return a value of any dataType. Parameter accumulator is
the reduce() accumulator parameter value on first call and the return value
of previous functor() call on subsequent calls. The dataType of the return
value should normally be the same as accumulator, but this is not required.
Reduce values of a list to a single value

=head4 Parameters

    $functor

This function is called for each value in a list. The returned value is
passed as the accumulator parameter on the next call to functor().

    $accumulator

This is an initial accumulator value passed to functor(). Normally, the
dataType of accumulator will be the dataTape for the reduce() return value,
but this is not required.

    $array

This is a list to be reduced.

=head3 sort

This produces a list with values sorted based on result of compareFunction.
The compareFunction is passed two values from the list and must return an
integer less than 0 if the first value is less than the second value, 0 if
they are equal, and a integer greater than 0 if the first value is greater
than the second value.
Sort values in a list

=head4 Parameters

    $compareFunction

This function is called with two value from list.

    $array

This is the list to sort.

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
    my ($predicate, $list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "all_of_all");
    $request->set("predicate", $predicate);
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub all_of_any {
    my ($predicate, $list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "all_of_any");
    $request->set("predicate", $predicate);
    $request->set("list1", $list1);
    $request->set("list2", $list2);

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
    my ($predicate, $list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "any_of_all");
    $request->set("predicate", $predicate);
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub any_of_any {
    my ($predicate, $list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "any_of_any");
    $request->set("predicate", $predicate);
    $request->set("list1", $list1);
    $request->set("list2", $list2);

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
