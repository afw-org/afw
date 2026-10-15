#!/usr/bin/env perl

#**
# @file regexp.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the regexp category.
#*

package afw::regexp;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    array_regexp 
    as_regexp 
    compile_regexp 
    eq_regexp 
    eqx_regexp 
    ge_regexp 
    gt_regexp 
    is_regexp 
    le_regexp 
    lt_regexp 
    ne_regexp 
    nex_regexp 
    regexp 
    regexp_escape 
    test 
    to_string_regexp 
);

=head1 NAME

afw::regexp - Perl module for regexp functions

=head2 Functions

The following functions are exported by default

=head3 array_regexp

Construct a new array of regexp from the given values, the same as array()
except that the new array is typed: each value, and each entry of a spread
array, must be dataType regexp, and adding a value of another data type to the
array later throws. A value of another data type is an error, never converted.
This is usually written `array<regexp>(`...values).
Construct an array of regexp from values or spread arrays

=head4 Parameters

    $values

A value of dataType regexp, or ... followed by an expression that results in
an array, every entry of which is included.

=head3 as_regexp

Returns value unchanged if it is dataType regexp. Otherwise, including when
value is undefined, throws an error. It never converts: to get a regexp from a
value of another data type, write a typed literal or call a to_T conversion
function where one exists. This is usually written value as regexp.
Asserts that value is dataType regexp

=head4 Parameters

    $value

Value to assert

=head3 compile_regexp

Compile regexp value and return either an unevaluated adaptive value or a
string containing the compiler listing. The listing is a human-oriented dump
(value tree interleaved with source, plus ---Symbols tables) for Fiddle and
debugging — not pure JSON (use stringify) and not Adaptive compiled-form text
(use decompile).
Compile regexp value

=head4 Parameters

    $source

regexp string to compile

    $listing

If specified, a human compiler listing is produced instead of an unevaluated
compiled value (tree + ---Symbols; not recompilable). Use decompile() for
Adaptive compiled-form text and stringify() for pure JSON of evaluated data.

This parameter can be an integer between 0 and 10 or a string that is used for
indentation. If 0 is specified, no whitespace is added to the resulting
string. If 1 through 10 is specified, that number of spaces is used.

=head3 eq_regexp

Determine if regexp arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_regexp

Determine if for regexp arg1 is equal to the value and data type of arg2 then
return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_regexp

Checks for regexp arg1 is greater than or equal to regexp arg2 and return the
boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_regexp

Checks for regexp arg1 is greater than regexp arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 is_regexp

Checks whether value is dataType regexp and returns the boolean result. It is
false for undefined. This can also be written `is<regexp>(value`).
Checks whether value is dataType regexp

=head4 Parameters

    $value

Value to check

=head3 le_regexp

Checks for regexp arg1 is less than or equal to regexp arg2 and return the
boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_regexp

Checks for regexp arg1 is less that regexp arg2 and return the boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_regexp

Determine if regexp arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead if
you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_regexp

Determine if for regexp arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 regexp

Converts value to data type regexp returning regexp result. Holds a regular
expression source string as a regexp value (does not compile the pattern for
matching by itself). Use polymorphic compile when a compiled form is required.
Convert to data type regexp

=head4 Parameters

    $value

Value to convert

=head3 regexp_escape

Returns value with every character that could have a meaning in a regular
expression escaped, so that the result matches value itself. Use it to put
data in a pattern: to_regexp("^" + regexp_escape(name) + "\\z"). ASCII
letters, digits, _ and characters above ASCII are kept, except a leading digit
and white space, which are written as \x{hh}, as are control characters; every
other ASCII character gets a backslash. The result means the same with any
flags.
Escape text for use in a regular expression

=head4 Parameters

    $value

The text to escape.

=head3 test

Checks whether the regular expression matches anywhere in value and returns
the boolean result; the method form is re->test(s). Anchor with ^ and \z to
match the whole value. value can be any value whose data type holds text, such
as string, anyURI, dnsName, ipAddress, rfc822Name and x500Name.
Test whether a regular expression matches

=head4 Parameters

    $regexp

The regular expression. A regexp literal (/a+b/ or "a+b"re), to_regexp(s), or
`compile<regexp>(r`) is already compiled; any other regexp value is compiled
for this call.

    $value

The value to search. Its data type must hold text.

=head3 to_string_regexp

Converts regexp value to string. For array values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A regexp value.

=cut

sub array_regexp {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "array_regexp");
    $request->set("values", $values);

    return $request->getResult();
}

sub as_regexp {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "as_regexp");
    $request->set("value", $value);

    return $request->getResult();
}

sub compile_regexp {
    my ($source, $listing) = @_;

    my $request = $session->request()

    $request->set("function" => "compile<regexp>");
    $request->set("source", $source);

    if (defined $listing)
        $request->set("listing", $listing);

    return $request->getResult();
}

sub eq_regexp {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<regexp>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_regexp {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<regexp>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_regexp {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<regexp>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_regexp {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<regexp>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub is_regexp {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is_regexp");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_regexp {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<regexp>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_regexp {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<regexp>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_regexp {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<regexp>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_regexp {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<regexp>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub regexp {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "regexp");
    $request->set("value", $value);

    return $request->getResult();
}

sub regexp_escape {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "regexp_escape");
    $request->set("value", $value);

    return $request->getResult();
}

sub test {
    my ($regexp, $value) = @_;

    my $request = $session->request()

    $request->set("function" => "test");
    $request->set("regexp", $regexp);
    $request->set("value", $value);

    return $request->getResult();
}

sub to_string_regexp {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<regexp>");
    $request->set("value", $value);

    return $request->getResult();
}

42;
