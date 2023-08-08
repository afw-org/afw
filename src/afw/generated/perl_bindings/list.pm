package afw::list;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    add_entries 
    bag_list 
    bag_size_list 
    clone_list 
    eq_list 
    eqx_list 
    ge_list 
    gt_list 
    includes_list 
    is_list 
    join 
    le_list 
    length_list 
    list 
    lt_list 
    ne_list 
    nex_list 
    reverse 
    slice 
    to_string_list 
);

=head1 NAME

afw::list - Perl module for list functions

=head2 Functions

The following functions are exported by default

=head3 add_entries

Add the entries of one or more lists to another.
Add entries of one or more lists to another

=head4 Parameters

    $target

Target list. This list must not be immutable.

    $source

Source list(s).

=head3 bag_list

Takes any number of list values and returns a list of list.
Makes a list from values

=head4 Parameters

    $values


=head3 bag_size_list

This returns the integer number of values in list.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 clone_list

Deep clone a list value.
Clone list value

=head4 Parameters

    $value

The list value to clone.

=head3 eq_list

Determine if list arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_list

Determine if for list arg1 is equal to the value and data type of arg2 then
return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_list

Checks for list arg1 is greater than or equal to list arg2 and return the
boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_list

Checks for list arg1 is greater than list arg2 and return the boolean result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 includes_list

Checks whether or not a list contains any value.
Checks whether or not a list contains any value

=head4 Parameters

    $list

The list to search.

    $searchElement

Element to find.

    $fromIndex

Index in the list to start search.

=head3 is_list

Checks whether value is dataType list and return the boolean result.
Checks whether value is dataType list

=head4 Parameters

    $value

Value to check

=head3 join

Concatenate the string values of the elements of a list with a separator.
Join the string values of list elements

=head4 Parameters

    $value

A list of values (statements) of any data type.

    $separator

The separator to use. If not specified, a comma (,) is used.

=head3 le_list

Checks for list arg1 is less than or equal to list arg2 and return the
boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 length_list

This is a polymorphic function where list can be any of the supported data
types. Return the integer number of entries in datatype list or codepoints in
others.
Returns number of codepoints or entries in value

=head4 Parameters

    $value

Returns the number of entries in a list or code points in others.

=head3 list

Construct a list with 0 or more elements.
Construct a list with 0 or more elements

=head4 Parameters

    $values

A value can refer to any adaptable value belonging to any data type or a list
expression. In the case of a list expression, indicated by '...' followed by
an expression that results in a list, every element within that list is
included in the newly created list.

=head3 lt_list

Checks for list arg1 is less that list arg2 and return the boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_list

Determine if list arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead
if you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_list

Determine if for list arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to
be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 reverse

Reverse the order of the elements in a list. If the list is typed, the
resulting list will be the same type.
Return list with elements reversed

=head4 Parameters

    $list

A list to reverse.

=head3 slice

This function extracts a consecutive slice of values from a list.
Return a consecutive slice of values from a list

=head4 Parameters

    $list

The list to slice.

    $startIndex

This is the zero based starting index. Use negative number to index from the
end of the list. If not specified, an index of 0 is assumed.

    $endIndex

This is the zero based ending index. If positive, this is one more than the
index of the last value to include in the list. If negative, the index is
from the end of the list. If not specified, the slice is from startIndex up
to and including the end of the list.

=head3 to_string_list

Converts list value to string. For list values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A list value.

=cut

sub add_entries {
    my ($target, $source) = @_;

    my $request = $session->request()

    $request->set("function" => "add_entries");
    $request->set("target", $target);
    $request->set("source", $source);

    return $request->getResult();
}

sub bag_list {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<list>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_list {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<list>");
    $request->set("value", $value);

    return $request->getResult();
}

sub clone_list {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "clone<list>");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_list {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<list>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_list {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<list>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_list {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<list>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_list {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<list>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub includes_list {
    my ($list, $searchElement, $fromIndex) = @_;

    my $request = $session->request()

    $request->set("function" => "includes<list>");
    $request->set("list", $list);
    $request->set("searchElement", $searchElement);

    if (defined $fromIndex)
        $request->set("fromIndex", $fromIndex);

    return $request->getResult();
}

sub is_list {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<list>");
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

sub le_list {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<list>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub length_list {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "length<list>");
    $request->set("value", $value);

    return $request->getResult();
}

sub list {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "list");
    $request->set("values", $values);

    return $request->getResult();
}

sub lt_list {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<list>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_list {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<list>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_list {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<list>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub reverse {
    my ($list) = @_;

    my $request = $session->request()

    $request->set("function" => "reverse");
    $request->set("list", $list);

    return $request->getResult();
}

sub slice {
    my ($list, $startIndex, $endIndex) = @_;

    my $request = $session->request()

    $request->set("function" => "slice");
    $request->set("list", $list);

    if (defined $startIndex)
        $request->set("startIndex", $startIndex);

    if (defined $endIndex)
        $request->set("endIndex", $endIndex);

    return $request->getResult();
}

sub to_string_list {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<list>");
    $request->set("value", $value);

    return $request->getResult();
}

42;
