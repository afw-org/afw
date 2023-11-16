#!/usr/bin/env perl

#**
# @file polymorphic.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the polymorphic category.
#*

package afw::polymorphic;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    abs 
    add 
    add_dayTimeDuration 
    add_yearMonthDuration 
    at_least_one_member_of 
    bag 
    bag_size 
    ceil 
    clone 
    compile 
    decode_to_string 
    divide 
    encode_as_base64Binary 
    encode_as_hexBinary 
    ends_with 
    eq 
    eq_ignore_case 
    eqx 
    evaluate 
    floor 
    ge 
    gt 
    in_range 
    includes 
    index_of 
    intersection 
    is 
    is_in 
    last_index_of 
    le 
    length 
    lt 
    match 
    max 
    min 
    mod 
    multiply 
    ne 
    negative 
    nex 
    normalize_space 
    normalize_to_lower_case 
    one_and_only 
    pow 
    regexp_index_of 
    regexp_match 
    regexp_replace 
    repeat 
    replace 
    round 
    set_equals 
    split 
    starts_with 
    subset 
    substring 
    subtract 
    subtract_dayTimeDuration 
    subtract_yearMonthDuration 
    to_anyURI 
    to_boolean 
    to_date 
    to_dateTime 
    to_dayTimeDuration 
    to_dnsName 
    to_double 
    to_integer 
    to_ipAddress 
    to_rfc822Name 
    to_string 
    to_time 
    to_x500Name 
    to_yearMonthDuration 
    union 
    url_encode 
);

=head1 NAME

afw::polymorphic - Perl module for polymorphic functions

=head2 Functions

The following functions are exported by default

=head3 abs

Compute the absolute value of the `<dataType>` value and return the
`<dataType>` result.
Absolute value

=head4 Parameters

    $value


=head3 add

Add 2 or more `<dataType>` values and return the `<dataType>` result.
Add

=head4 Parameters

    $values


=head3 add_dayTimeDuration

Add `<dataType>` arg1 to dayTimeDuration arg2 and return the `<dataType>`
result.
Add duration

=head4 Parameters

    $arg1


    $arg2


=head3 add_yearMonthDuration

Add `<dataType>` arg1 to yearMonthDuration arg2 and return the `<dataType>`
result.
Add duration

=head4 Parameters

    $arg1


    $arg2


=head3 at_least_one_member_of

Returns boolean true if at least one value in `<dataType>` array1 is in
`<dataType>` array2.
Checks for at least one value in common

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 bag

Takes any number of `<dataType>` values and returns an array of array.
Makes an array from values

=head4 Parameters

    $values


=head3 bag_size

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 ceil

Determine the smallest integer that is greater then or equal to the
`<dataType>` value and return the `<dataType>` result.
Round upward to nearest integer

=head4 Parameters

    $value


=head3 clone

Deep clone a `<dataType>` value.
Clone <dataType> value

=head4 Parameters

    $value

The `<dataType>` value to clone.

=head3 compile

Compile `<dataType>` value and return either an unevaluated adaptive value or
a string containing the compiler listing.
Compile <dataType> value

=head4 Parameters

    $source

`<dataType>` string to compile

    $listing

If specified, a compiler listing is produced instead of an unevaluated
compiled value.

This parameter can be an integer between 0 and 10 of a string that is used for
indentation. If 0 is specified, no whitespace is added to the resulting
string. If 1 through 10 is specified, that number of spaces is used.

=head3 decode_to_string

Decode `<dataType>` value to string. An error is thrown if decoded value is
not valid UTF-8.
Decode <dataType> to string

=head4 Parameters

    $value

The `<dataType>` value to decode.

=head3 divide

Divide `<dataType>` dividend by `<dataType>` divisor and return the
`<dataType>` quotient.
Divide numbers

=head4 Parameters

    $dividend


    $divisor


=head3 encode_as_base64Binary

Encode a value as a base64Binary. The effect is to create a base64Binary value
with an internal value of the value passed.
Encode to a base64Binary value

=head4 Parameters

    $value

The `<dataType>` value to encode.

=head3 encode_as_hexBinary

Encode a value as a hexBinary. The effect is to create a hexBinary value with
an internal value of the value passed.
Encode to a hexBinary value

=head4 Parameters

    $value

The `<dataType>` value to encode.

=head3 ends_with

Checks whether `<dataType>` value ends with a `<dataType>` and return the
boolean result.
Checks whether value ends with a string

=head4 Parameters

    $value


    $subString


=head3 eq

Determine if `<dataType>` arg1 is equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'eqx' ('===') instead if
you want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eq_ignore_case

Checks for `<dataType>` arg1 is equal to `<dataType>` arg2 ignoring case and
return the boolean result.
Checks for equal ignoring case

=head4 Parameters

    $arg1


    $arg2


=head3 eqx

Determine if for `<dataType>` arg1 is equal to the value and data type of arg2
then return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 evaluate

Compile and evaluate `<dataType>` value.
Evaluate <dataType> value

=head4 Parameters

    $source

`<dataType>` string to compile and evaluate

    $additionalUntrustedQualifiedVariables

This parameter supplies additional qualified variables that can be accessed
during evaluation. These variables will not be used by anything that needs to
ensure its qualified variables must come from a trusted source, such as
authorization. This parameter is intended to be used for testing only and
should not be used for anything running in production.

=head3 floor

Determine the largest integer that is smaller then or equal to the
`<dataType>` value and return the `<dataType>` result.
Round downwards to nearest integer

=head4 Parameters

    $number


=head3 ge

Checks for `<dataType>` arg1 is greater than or equal to `<dataType>` arg2 and
return the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt

Checks for `<dataType>` arg1 is greater than `<dataType>` arg2 and return the
boolean result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 in_range

Checks if `<dataType>` is between startTime and endTime, inclusive. Regardless
of endTime value, it is always considered to be equal to, but less than 24
hours greater than startTime. If no `<dataType>` zone is specified for
`<dataType>,` the default `<dataType>` zone is used. If no `<dataType>` zone
is specified for startTime or endTime, the `<dataType>` zone of `<dataType>`
is used.
Checks if time in range

=head4 Parameters

    $time


    $startTime


    $endTime


=head3 includes

Checks whether the `<dataType>` value includes a string and return the boolean
result.
Checks whether the <dataType> value includes a string

=head4 Parameters

    $searchString

The `<dataType>` to search.

    $subString

Substring to find.

    $position

Zero-based position in the search string to start search.

=head3 index_of

Returns the zero-based index into `<dataType>` value of subString. If
subString is not found, -1 is returned.
Returns index of first occurrence

=head4 Parameters

    $value

The `<dataType>` value to search.

    $subString

Substring to search for.

    $startIndex

Optional start index for search if different than start of string.

=head3 intersection

Returns an array of `<dataType>` with the values that are common to both array
of `<dataType>` array1 and array2.
Returns intersection of two arrays

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 is

Checks whether value is dataType `<dataType>` and return the boolean result.
Checks whether value is dataType <dataType>

=head4 Parameters

    $value

Value to check

=head3 is_in

Checks whether `<dataType>` value is in array of `<dataType>` array and
returns the boolean result.
Checks whether a value is in an array

=head4 Parameters

    $value


    $array


=head3 last_index_of

Returns the zero-based index into `<dataType>` value of the last occurrence of
a subString. If subString is not found, -1 is returned.
Returns index of last occurrence

=head4 Parameters

    $value

The `<dataType>` value to search.

    $subString

Substring to search for.

    $startIndex

Optional start index for search if different than start of string.

=head3 le

Checks for `<dataType>` arg1 is less than or equal to `<dataType>` arg2 and
return the boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 length

This is a polymorphic function where `<dataType>` can be any of the supported
data types. Return the integer number of entries in datatype array or
codepoints in others.
Returns number of codepoints or entries in value

=head4 Parameters

    $value

Returns the number of entries in an array or code points in others.

=head3 lt

Checks for `<dataType>` arg1 is less that `<dataType>` arg2 and return the
boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 match

Returns true if arg1 matches some terminal sequence of RDNs from arg2 when
compared using `equal_<dataType>`.
Checks if x500Name matches

=head4 Parameters

    $arg1


    $arg2


=head3 max

Return the `<dataType>` value that is greater than or equal to the others.
Maximum value

=head4 Parameters

    $values


=head3 min

Return the `<dataType>` value that is less than or equal to the others.
Minimum value

=head4 Parameters

    $values


=head3 mod

Divide `<dataType>` dividend by `<dataType>` divisor and return the
`<dataType>` remainder.
Remainder of dividing numbers

=head4 Parameters

    $dividend


    $divisor


=head3 multiply

Multiply 2 or more `<dataType>` values and return the `<dataType>` result.
Multiply numbers

=head4 Parameters

    $values


=head3 ne

Determine if `<dataType>` arg1 is not equal to the value of arg2 converted to
the data type of arg1 then return the boolean result. Use 'nex' ('!==')
instead if you want true to be returned if arg1 and arg2's data type don't
match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 negative

Return negative of `<dataType>` value.
Negative of a number

=head4 Parameters

    $value


=head3 nex

Determine if for `<dataType>` arg1 is not equal to the value or data type of
arg2 then return the boolean result. Use 'ne' ('!=') instead if you want arg2
to be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 normalize_space

Remove whitespace from the beginning and end of a `<dataType>` value.
Trims whitespace from beginning and end

=head4 Parameters

    $string


=head3 normalize_to_lower_case

Normalize `<dataType>` value to lower case and returns `<dataType>` result.
Converts to lower case

=head4 Parameters

    $string


=head3 one_and_only

This converts an array of `<dataType>` values that contains one value to a
single `<dataType>` value.
Converts a one value array to a <dataType> value

=head4 Parameters

    $array


=head3 pow

This returns the value of base raised to a power. Multiple exponents can be
specified to raise the previous exponent to the power of the latter exponent.
Base raised to a power

=head4 Parameters

    $base

Base value.

    $exponent

Exponent value.

=head3 regexp_index_of

Search `<dataType>` value for a regular expression and return index. If not
found, -1 is returned.
Returns index of first match of regular expression

=head4 Parameters

    $value

The `<dataType>` value to search.

    $regexp

A regular expression to use for search.

=head3 regexp_match

Checks whether `<dataType>` value matches the regular expression regexp and
return the boolean result.
Search for a match using a regular expression

=head4 Parameters

    $value


    $regexp


=head3 regexp_replace

Replace matched values for a regular expression in a `<dataType>` value.
Replace strings matching a regular expression

=head4 Parameters

    $value

The original `<dataType>` value.

    $regexp

A regular expression to use for search.

    $replacement

The replacement string.

    $limit

This is the maximum times to replace. The default is 1. Specify -1 to replace
all occurrences.

=head3 repeat

Repeat a `<dataType>` value a specified number of times.
Returns a repeated value

=head4 Parameters

    $value

The `<dataType>` value to repeat.

    $times

The number of times to repeat the value.

=head3 replace

Replace string(s) in a `<dataType>` value.
Replace strings

=head4 Parameters

    $value

The original `<dataType>` value.

    $match

The string to replace.

    $replacement

The replacement string.

    $limit

This is the maximum times to replace. The default is 1. Specify -1 to replace
all occurrences.

=head3 round

Determine the integer closest to `<dataType>` value and return the
`<dataType>` result.
Round to nearest integer

=head4 Parameters

    $number


=head3 set_equals

Returns boolean true if `<dataType>` array1 and `<dataType>` array2 are
subsets of each other and return the boolean result.
Checks whether two arrays are subsets of each other

=head4 Parameters

    $array1


    $array2


=head3 split

Split `<dataType>` value into an array of strings using a separator.
Split at a separator

=head4 Parameters

    $value

The `<dataType>` value to split.

    $separator

The separator to use. If this is an empty string or separator is not
specified, the value is split between characters.

    $limit

This is the maximum number of splits. Any remaining part of value is ignored.

=head3 starts_with

Checks whether `<dataType>` value starts with a subString and return the
boolean result.
Checks whether value starts with a string

=head4 Parameters

    $value


    $subString


=head3 subset

Returns boolean true if the unique values in `<dataType>` array1 are all in
`<dataType>` array2.
Determines if the first array is a subset of second array

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 substring

Returns the `<dataType>` substring of value beginning at zero-based position
integer startIndex and ending at the position before integer endIndex. Specify
-1 or omitting endIndex to return up to end of `<dataType>`.
Extract a substring

=head4 Parameters

    $string


    $startIndex


    $endIndex


=head3 subtract

Subtract `<dataType>` arg2 from `<dataType>` arg1 and return the `<dataType>`
result.
Subtract numbers

=head4 Parameters

    $arg1


    $arg2


=head3 subtract_dayTimeDuration

Subtract dayTimeDuration arg2 from `<dataType>` arg1 and return the
`<dataType>` result.
Subtract numbers

=head4 Parameters

    $arg1


    $arg2


=head3 subtract_yearMonthDuration

Subtract yearMonthDuration arg2 from `<dataType>` arg1 and return the
`<dataType>` result.
Subtract numbers

=head4 Parameters

    $arg1


    $arg2


=head3 to_anyURI

Converts `<dataType>` value to anyURI.
Converts string to anyURI

=head4 Parameters

    $value


=head3 to_boolean

Converts `<dataType>` value to boolean.
Converts string to boolean

=head4 Parameters

    $value


=head3 to_date

Converts `<dataType>` value to date.
Converts string to date

=head4 Parameters

    $value


=head3 to_dateTime

Converts `<dataType>` value to dateTime.
Converts string to dateTime

=head4 Parameters

    $value


=head3 to_dayTimeDuration

Converts `<dataType>` value to dayTimeDuration.
Converts string to dayTimeDuration

=head4 Parameters

    $value


=head3 to_dnsName

Converts `<dataType>` value to dnsName.
Converts string to dnsName

=head4 Parameters

    $value


=head3 to_double

Converts `<dataType>` value to double and returns double result.
Converts to double

=head4 Parameters

    $value


=head3 to_integer

Truncate `<dataType>` value to a whole number and returns integer result.
Converts to integer

=head4 Parameters

    $value


=head3 to_ipAddress

Converts `<dataType>` value to ipAddress.
Converts string to ipAddress

=head4 Parameters

    $value


=head3 to_rfc822Name

Converts `<dataType>` value to rfc822Name.
Converts string to rfc822Name

=head4 Parameters

    $value


=head3 to_string

Converts `<dataType>` value to string. For array values, the to_string() value
for each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A `<dataType>` value.

=head3 to_time

Converts `<dataType>` value to time.
Converts string to time

=head4 Parameters

    $value


=head3 to_x500Name

Converts `<dataType>` value to x500Name.
Converts string to x500Name

=head4 Parameters

    $value


=head3 to_yearMonthDuration

Converts `<dataType>` value to yearMonthDuration.
Converts string to yearMonthDuration

=head4 Parameters

    $value


=head3 union

Returns an array of `<dataType>` contains all of the unique values in two or
more array of `<dataType>` values.
Returns union of two or more string arrays

=head4 Parameters

    $arrays

Two or more arrays.

=head3 url_encode

URL encode a value or bag of values.
URI encode

=head4 Parameters

    $unencoded

URL encode a single value. See the url_encode method for the data type of more
details.

=cut

sub abs {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "abs");
    $request->set("value", $value);

    return $request->getResult();
}

sub add {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "add");
    $request->set("values", $values);

    return $request->getResult();
}

sub add_dayTimeDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "add_dayTimeDuration");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub add_yearMonthDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "add_yearMonthDuration");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub at_least_one_member_of {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub bag {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size");
    $request->set("value", $value);

    return $request->getResult();
}

sub ceil {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "ceil");
    $request->set("value", $value);

    return $request->getResult();
}

sub clone {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "clone");
    $request->set("value", $value);

    return $request->getResult();
}

sub compile {
    my ($source, $listing) = @_;

    my $request = $session->request()

    $request->set("function" => "compile");
    $request->set("source", $source);

    if (defined $listing)
        $request->set("listing", $listing);

    return $request->getResult();
}

sub decode_to_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "decode_to_string");
    $request->set("value", $value);

    return $request->getResult();
}

sub divide {
    my ($dividend, $divisor) = @_;

    my $request = $session->request()

    $request->set("function" => "divide");
    $request->set("dividend", $dividend);
    $request->set("divisor", $divisor);

    return $request->getResult();
}

sub encode_as_base64Binary {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "encode_as_base64Binary");
    $request->set("value", $value);

    return $request->getResult();
}

sub encode_as_hexBinary {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "encode_as_hexBinary");
    $request->set("value", $value);

    return $request->getResult();
}

sub ends_with {
    my ($value, $subString) = @_;

    my $request = $session->request()

    $request->set("function" => "ends_with");
    $request->set("value", $value);
    $request->set("subString", $subString);

    return $request->getResult();
}

sub eq {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eq_ignore_case {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq_ignore_case");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub evaluate {
    my ($source, $additionalUntrustedQualifiedVariables) = @_;

    my $request = $session->request()

    $request->set("function" => "evaluate");
    $request->set("source", $source);

    if (defined $additionalUntrustedQualifiedVariables)
        $request->set("additionalUntrustedQualifiedVariables", $additionalUntrustedQualifiedVariables);

    return $request->getResult();
}

sub floor {
    my ($number) = @_;

    my $request = $session->request()

    $request->set("function" => "floor");
    $request->set("number", $number);

    return $request->getResult();
}

sub ge {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub in_range {
    my ($time, $startTime, $endTime) = @_;

    my $request = $session->request()

    $request->set("function" => "in_range");
    $request->set("time", $time);
    $request->set("startTime", $startTime);
    $request->set("endTime", $endTime);

    return $request->getResult();
}

sub includes {
    my ($searchString, $subString, $position) = @_;

    my $request = $session->request()

    $request->set("function" => "includes");
    $request->set("searchString", $searchString);
    $request->set("subString", $subString);

    if (defined $position)
        $request->set("position", $position);

    return $request->getResult();
}

sub index_of {
    my ($value, $subString, $startIndex) = @_;

    my $request = $session->request()

    $request->set("function" => "index_of");
    $request->set("value", $value);
    $request->set("subString", $subString);

    if (defined $startIndex)
        $request->set("startIndex", $startIndex);

    return $request->getResult();
}

sub intersection {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub is_ {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_in {
    my ($value, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in");
    $request->set("value", $value);
    $request->set("array", $array);

    return $request->getResult();
}

sub last_index_of {
    my ($value, $subString, $startIndex) = @_;

    my $request = $session->request()

    $request->set("function" => "last_index_of");
    $request->set("value", $value);
    $request->set("subString", $subString);

    if (defined $startIndex)
        $request->set("startIndex", $startIndex);

    return $request->getResult();
}

sub le {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub length {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "length");
    $request->set("value", $value);

    return $request->getResult();
}

sub lt {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub match {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "match");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub max {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "max");
    $request->set("values", $values);

    return $request->getResult();
}

sub min {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "min");
    $request->set("values", $values);

    return $request->getResult();
}

sub mod {
    my ($dividend, $divisor) = @_;

    my $request = $session->request()

    $request->set("function" => "mod");
    $request->set("dividend", $dividend);
    $request->set("divisor", $divisor);

    return $request->getResult();
}

sub multiply {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "multiply");
    $request->set("values", $values);

    return $request->getResult();
}

sub ne {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub negative {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "negative");
    $request->set("value", $value);

    return $request->getResult();
}

sub nex {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub normalize_space {
    my ($string) = @_;

    my $request = $session->request()

    $request->set("function" => "normalize_space");
    $request->set("string", $string);

    return $request->getResult();
}

sub normalize_to_lower_case {
    my ($string) = @_;

    my $request = $session->request()

    $request->set("function" => "normalize_to_lower_case");
    $request->set("string", $string);

    return $request->getResult();
}

sub one_and_only {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only");
    $request->set("array", $array);

    return $request->getResult();
}

sub pow {
    my ($base, $exponent) = @_;

    my $request = $session->request()

    $request->set("function" => "pow");
    $request->set("base", $base);
    $request->set("exponent", $exponent);

    return $request->getResult();
}

sub regexp_index_of {
    my ($value, $regexp) = @_;

    my $request = $session->request()

    $request->set("function" => "regexp_index_of");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

    return $request->getResult();
}

sub regexp_match {
    my ($value, $regexp) = @_;

    my $request = $session->request()

    $request->set("function" => "regexp_match");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

    return $request->getResult();
}

sub regexp_replace {
    my ($value, $regexp, $replacement, $limit) = @_;

    my $request = $session->request()

    $request->set("function" => "regexp_replace");
    $request->set("value", $value);
    $request->set("regexp", $regexp);
    $request->set("replacement", $replacement);

    if (defined $limit)
        $request->set("limit", $limit);

    return $request->getResult();
}

sub repeat {
    my ($value, $times) = @_;

    my $request = $session->request()

    $request->set("function" => "repeat");
    $request->set("value", $value);
    $request->set("times", $times);

    return $request->getResult();
}

sub replace {
    my ($value, $match, $replacement, $limit) = @_;

    my $request = $session->request()

    $request->set("function" => "replace");
    $request->set("value", $value);
    $request->set("match", $match);
    $request->set("replacement", $replacement);

    if (defined $limit)
        $request->set("limit", $limit);

    return $request->getResult();
}

sub round {
    my ($number) = @_;

    my $request = $session->request()

    $request->set("function" => "round");
    $request->set("number", $number);

    return $request->getResult();
}

sub set_equals {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub split {
    my ($value, $separator, $limit) = @_;

    my $request = $session->request()

    $request->set("function" => "split");
    $request->set("value", $value);

    if (defined $separator)
        $request->set("separator", $separator);

    if (defined $limit)
        $request->set("limit", $limit);

    return $request->getResult();
}

sub starts_with {
    my ($value, $subString) = @_;

    my $request = $session->request()

    $request->set("function" => "starts_with");
    $request->set("value", $value);
    $request->set("subString", $subString);

    return $request->getResult();
}

sub subset {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub substring {
    my ($string, $startIndex, $endIndex) = @_;

    my $request = $session->request()

    $request->set("function" => "substring");
    $request->set("string", $string);
    $request->set("startIndex", $startIndex);

    if (defined $endIndex)
        $request->set("endIndex", $endIndex);

    return $request->getResult();
}

sub subtract {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "subtract");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub subtract_dayTimeDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "subtract_dayTimeDuration");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub subtract_yearMonthDuration {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "subtract_yearMonthDuration");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub to_anyURI {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_anyURI");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_boolean {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_boolean");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_date {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_date");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_dateTime {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_dateTime");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_dayTimeDuration {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_dayTimeDuration");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_dnsName {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_dnsName");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_double {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_double");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_integer {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_integer");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_ipAddress {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_ipAddress");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_rfc822Name {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_rfc822Name");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_time {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_time");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_x500Name {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_x500Name");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_yearMonthDuration {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_yearMonthDuration");
    $request->set("value", $value);

    return $request->getResult();
}

sub union {
    my ($arrays) = @_;

    my $request = $session->request()

    $request->set("function" => "union");
    $request->set("arrays", $arrays);

    return $request->getResult();
}

sub url_encode {
    my ($unencoded) = @_;

    my $request = $session->request()

    $request->set("function" => "url_encode");
    $request->set("unencoded", $unencoded);

    return $request->getResult();
}

42;
