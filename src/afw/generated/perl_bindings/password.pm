package afw::password;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    bag_password 
    bag_size_password 
    eq_password 
    eqx_password 
    ge_password 
    gt_password 
    is_password 
    le_password 
    lt_password 
    ne_password 
    nex_password 
    password 
    to_string_password 
);

=head1 NAME

afw::password - Perl module for password functions

=head2 Functions

The following functions are exported by default

=head3 bag_password

Takes any number of password values and returns a list of list.
Makes a list from values

=head4 Parameters

    $values


=head3 bag_size_password

This returns the integer number of values in list.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 eq_password

Determine if password arg1 is equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use "eqx" ("===") instead
if you want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_password

Determine if for password arg1 is equal to the value and data type of arg2
then return the boolean result. Use "eq" ("==") instead if you want arg2 to
be converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_password

Checks for password arg1 is greater than or equal to password arg2 and return
the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_password

Checks for password arg1 is greater than password arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 is_password

Checks whether value is dataType password and return the boolean result.
Checks whether value is dataType password

=head4 Parameters

    $value

Value to check

=head3 le_password

Checks for password arg1 is less than or equal to password arg2 and return
the boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_password

Checks for password arg1 is less that password arg2 and return the boolean
result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_password

Determine if password arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use "nex" ("!==") instead
if you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_password

Determine if for password arg1 is not equal to the value or data type of arg2
then return the boolean result. Use "ne" ("!=") instead if you want arg2 to
be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 password

Converts value to data type password returning password result.
Convert to data type password

=head4 Parameters

    $value

Value to convert

=head3 to_string_password

Converts password value to string. For list values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A password value.

=cut

sub bag_password {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<password>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_password {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<password>");
    $request->set("value", $value);

    return $request->getResult();
}

sub eq_password {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<password>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_password {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<password>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_password {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<password>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_password {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<password>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub is_password {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<password>");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_password {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<password>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_password {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<password>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_password {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<password>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_password {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<password>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub password {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "password");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_string_password {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<password>");
    $request->set("value", $value);

    return $request->getResult();
}

42;
