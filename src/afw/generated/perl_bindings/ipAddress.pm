package afw::ipAddress;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    bag_ipAddress 
    bag_size_ipAddress 
    eq_ipAddress 
    eqx_ipAddress 
    ge_ipAddress 
    gt_ipAddress 
    ipAddress 
    is_in_ipAddress 
    is_ipAddress 
    le_ipAddress 
    lt_ipAddress 
    ne_ipAddress 
    nex_ipAddress 
    one_and_only_ipAddress 
    regexp_match_ipAddress 
    to_string_ipAddress 
);

=head1 NAME

afw::ipAddress - Perl module for ipAddress functions

=head2 Functions

The following functions are exported by default

=head3 bag_ipAddress

Takes any number of ipAddress values and returns a list of list.
Makes a list from values

=head4 Parameters

    $values


=head3 bag_size_ipAddress

This returns the integer number of values in list.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 eq_ipAddress

Determine if ipAddress arg1 is equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
if you want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_ipAddress

Determine if for ipAddress arg1 is equal to the value and data type of arg2
then return the boolean result. Use 'eq' ('==') instead if you want arg2 to
be converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_ipAddress

Checks for ipAddress arg1 is greater than or equal to ipAddress arg2 and
return the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_ipAddress

Checks for ipAddress arg1 is greater than ipAddress arg2 and return the
boolean result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 ipAddress

Converts value to data type ipAddress returning ipAddress result.
Convert to data type ipAddress

=head4 Parameters

    $value

Value to convert

=head3 is_in_ipAddress

Checks whether ipAddress value is in list of ipAddress list and returns the
boolean result.
Checks whether a value is in a list

=head4 Parameters

    $value


    $list


=head3 is_ipAddress

Checks whether value is dataType ipAddress and return the boolean result.
Checks whether value is dataType ipAddress

=head4 Parameters

    $value

Value to check

=head3 le_ipAddress

Checks for ipAddress arg1 is less than or equal to ipAddress arg2 and return
the boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_ipAddress

Checks for ipAddress arg1 is less that ipAddress arg2 and return the boolean
result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_ipAddress

Determine if ipAddress arg1 is not equal to the value of arg2 converted to
the data type of arg1 then return the boolean result. Use 'nex' ('!==')
instead if you want true to be returned if arg1 and arg2's data type don't
match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_ipAddress

Determine if for ipAddress arg1 is not equal to the value or data type of
arg2 then return the boolean result. Use 'ne' ('!=') instead if you want arg2
to be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_ipAddress

This converts a list of ipAddress values that contains one value to a single
ipAddress value.
Converts a one value list to a ipAddress value

=head4 Parameters

    $list


=head3 regexp_match_ipAddress

Checks whether ipAddress value matches the regular expression regexp and
return the boolean result.
Search for a match using a regular expression

=head4 Parameters

    $value


    $regexp


=head3 to_string_ipAddress

Converts ipAddress value to string. For list values, the to_string() value
for each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A ipAddress value.

=cut

sub bag_ipAddress {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<ipAddress>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_ipAddress {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<ipAddress>");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_ipAddress {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<ipAddress>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_ipAddress {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<ipAddress>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_ipAddress {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<ipAddress>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_ipAddress {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<ipAddress>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ipAddress {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "ipAddress");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_in_ipAddress {
    my ($value, $list) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<ipAddress>");
    $request->set("value", $value);
    $request->set("list", $list);

    return $request->getResult();
}

sub is_ipAddress {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<ipAddress>");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_ipAddress {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<ipAddress>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_ipAddress {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<ipAddress>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_ipAddress {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<ipAddress>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_ipAddress {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<ipAddress>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_ipAddress {
    my ($list) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<ipAddress>");
    $request->set("list", $list);

    return $request->getResult();
}

sub regexp_match_ipAddress {
    my ($value, $regexp) = @_;

    my $request = $session->request()

    $request->set("function" => "regexp_match<ipAddress>");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

    return $request->getResult();
}

sub to_string_ipAddress {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<ipAddress>");
    $request->set("value", $value);

    return $request->getResult();
}

42;
