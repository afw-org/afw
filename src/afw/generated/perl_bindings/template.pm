package afw::template;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    bag_size_template 
    bag_template 
    compile_template 
    eq_template 
    eqx_template 
    evaluate_template 
    ge_template 
    gt_template 
    is_template 
    le_template 
    lt_template 
    ne_template 
    nex_template 
    template 
    to_string_template 
);

=head1 NAME

afw::template - Perl module for template functions

=head2 Functions

The following functions are exported by default

=head3 bag_size_template

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 bag_template

Takes any number of template values and returns an array of array.
Makes an array from values

=head4 Parameters

    $values


=head3 compile_template

Compile template value and return either an unevaluated adaptive value or a
string containing the compiler listing.
Compile template value

=head4 Parameters

    $source

template string to compile

    $listing

If specified, a compiler listing is produced instead of an unevaluated
expression value.

This parameter can be an integer between 0 and 10 of a string that is used
for indentation. If 0 is specified, no whitespace is added to the resulting
string. If 1 through 10 is specified, that number of spaces is used.

=head3 eq_template

Determine if template arg1 is equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
if you want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_template

Determine if for template arg1 is equal to the value and data type of arg2
then return the boolean result. Use 'eq' ('==') instead if you want arg2 to
be converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 evaluate_template

Compile and evaluate template value.
Evaluate template value

=head4 Parameters

    $source

template string to compile and evaluate

    $additionalUntrustedQualifiedVariables

This parameter supplies additional qualified variables that can be accessed
during evaluation. These variables will not be used by anything that needs to
ensure its qualified variables must come from a trusted source, such as
authorization. This parameter is intended to be used for testing only and
should not be used for anything running in production.

=head3 ge_template

Checks for template arg1 is greater than or equal to template arg2 and return
the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_template

Checks for template arg1 is greater than template arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 is_template

Checks whether value is dataType template and return the boolean result.
Checks whether value is dataType template

=head4 Parameters

    $value

Value to check

=head3 le_template

Checks for template arg1 is less than or equal to template arg2 and return
the boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_template

Checks for template arg1 is less that template arg2 and return the boolean
result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_template

Determine if template arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead
if you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_template

Determine if for template arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to
be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 template

Converts value to data type template returning template result.
Convert to data type template

=head4 Parameters

    $value

Value to convert

=head3 to_string_template

Converts template value to string. For array values, the to_string() value
for each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A template value.

=cut

sub bag_size_template {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<template>");
    $request->set("value", $value);

    return $request->getResult();
}

sub bag_template {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<template>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub compile_template {
    my ($source, $listing) = @_;

    my $request = $session->request()

    $request->set("function" => "compile<template>");
    $request->set("source", $source);

    if (defined $listing)
        $request->set("listing", $listing);

    return $request->getResult();
}

sub eq_template {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<template>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_template {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<template>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub evaluate_template {
    my ($source, $additionalUntrustedQualifiedVariables) = @_;

    my $request = $session->request()

    $request->set("function" => "evaluate<template>");
    $request->set("source", $source);

    if (defined $additionalUntrustedQualifiedVariables)
        $request->set("additionalUntrustedQualifiedVariables", $additionalUntrustedQualifiedVariables);

    return $request->getResult();
}

sub ge_template {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<template>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_template {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<template>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub is_template {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<template>");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_template {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<template>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_template {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<template>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_template {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<template>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_template {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<template>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub template {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "template");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_string_template {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<template>");
    $request->set("value", $value);

    return $request->getResult();
}

42;
