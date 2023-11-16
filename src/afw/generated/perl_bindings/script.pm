#!/usr/bin/env perl

#**
# @file script.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the script category.
#*

package afw::script;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    bag_script 
    bag_size_script 
    compile_script 
    eq_script 
    eqx_script 
    evaluate_script 
    ge_script 
    gt_script 
    is_script 
    le_script 
    lt_script 
    ne_script 
    nex_script 
    script 
);

=head1 NAME

afw::script - Perl module for script functions

=head2 Functions

The following functions are exported by default

=head3 bag_script

Takes any number of script values and returns an array of array.
Makes an array from values

=head4 Parameters

    $values


=head3 bag_size_script

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 compile_script

Compile script value and return either an unevaluated adaptive value or a
string containing the compiler listing.
Compile script value

=head4 Parameters

    $source

script string to compile

    $listing

If specified, a compiler listing is produced instead of an unevaluated
compiled value.

This parameter can be an integer between 0 and 10 of a string that is used for
indentation. If 0 is specified, no whitespace is added to the resulting
string. If 1 through 10 is specified, that number of spaces is used.

=head3 eq_script

Determine if script arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_script

Determine if for script arg1 is equal to the value and data type of arg2 then
return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 evaluate_script

Compile and evaluate script value.
Evaluate script value

=head4 Parameters

    $source

script string to compile and evaluate

    $additionalUntrustedQualifiedVariables

This parameter supplies additional qualified variables that can be accessed
during evaluation. These variables will not be used by anything that needs to
ensure its qualified variables must come from a trusted source, such as
authorization. This parameter is intended to be used for testing only and
should not be used for anything running in production.

=head3 ge_script

Checks for script arg1 is greater than or equal to script arg2 and return the
boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_script

Checks for script arg1 is greater than script arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 is_script

Checks whether value is dataType script and return the boolean result.
Checks whether value is dataType script

=head4 Parameters

    $value

Value to check

=head3 le_script

Checks for script arg1 is less than or equal to script arg2 and return the
boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_script

Checks for script arg1 is less that script arg2 and return the boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_script

Determine if script arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead if
you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_script

Determine if for script arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 script

Converts value to data type script returning script result.
Convert to data type script

=head4 Parameters

    $value

Value to convert

=cut

sub bag_script {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<script>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_script {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<script>");
    $request->set("value", $value);

    return $request->getResult();
}

sub compile_script {
    my ($source, $listing) = @_;

    my $request = $session->request()

    $request->set("function" => "compile<script>");
    $request->set("source", $source);

    if (defined $listing)
        $request->set("listing", $listing);

    return $request->getResult();
}

sub eq_script {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<script>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_script {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<script>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub evaluate_script {
    my ($source, $additionalUntrustedQualifiedVariables) = @_;

    my $request = $session->request()

    $request->set("function" => "evaluate<script>");
    $request->set("source", $source);

    if (defined $additionalUntrustedQualifiedVariables)
        $request->set("additionalUntrustedQualifiedVariables", $additionalUntrustedQualifiedVariables);

    return $request->getResult();
}

sub ge_script {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<script>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_script {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<script>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub is_script {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<script>");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_script {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<script>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_script {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<script>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_script {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<script>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_script {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<script>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub script {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "script");
    $request->set("value", $value);

    return $request->getResult();
}

42;
