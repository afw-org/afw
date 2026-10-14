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
    acos 
    add 
    add_dayTimeDuration 
    add_yearMonthDuration 
    asin 
    at_least_one_member_of 
    atan 
    atan2 
    cbrt 
    ceil 
    clone 
    code_point_at 
    compile 
    cos 
    decode_to_string 
    divide 
    encode_as_base64Binary 
    encode_as_hexBinary 
    ends_with 
    eq 
    eq_ignore_case 
    eqx 
    eval 
    exp 
    floor 
    freeze 
    ge 
    gt 
    hypot 
    in_range 
    includes 
    index_of 
    intersection 
    is_in 
    last_index_of 
    le 
    length 
    log 
    log10 
    log2 
    lt 
    match 
    match_all 
    max 
    min 
    mod 
    multiply 
    ne 
    negative 
    nex 
    normalize 
    normalize_space 
    normalize_to_lower_case 
    one_and_only 
    pad_end 
    pad_start 
    pow 
    repeat 
    replace 
    replace_all 
    round 
    search 
    set_equals 
    sign 
    sin 
    split 
    sqrt 
    starts_with 
    subset 
    substring 
    subtract 
    subtract_dayTimeDuration 
    subtract_yearMonthDuration 
    tan 
    to_anyURI 
    to_base64Binary 
    to_date 
    to_dateTime 
    to_dayTimeDuration 
    to_dnsName 
    to_double 
    to_hexBinary 
    to_ia5String 
    to_integer 
    to_ipAddress 
    to_lower_case 
    to_objectId 
    to_objectPath 
    to_password 
    to_regexp 
    to_rfc822Name 
    to_string 
    to_time 
    to_upper_case 
    to_x500Name 
    to_yearMonthDuration 
    trim 
    trim_end 
    trim_start 
    trunc 
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


=head3 acos

Return the arccosine, in radians from 0 to #pi, of the `<dataType>` value, as
ECMAScript Math.acos does. A value outside -1.0 to 1.0 is an error. A result
that would be NaN, or a NaN argument, is an error. An integer is not accepted:
write to_double(v).
Arccosine

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


=head3 asin

Return the arcsine, in radians from -#pi/2 to #pi/2, of the `<dataType>`
value, as ECMAScript Math.asin does. A value outside -1.0 to 1.0 is an error.
A result that would be NaN, or a NaN argument, is an error. An integer is not
accepted: write to_double(v).
Arcsine

=head4 Parameters

    $value


=head3 at_least_one_member_of

Returns boolean true if at least one value in `<dataType>` array1 is in
`<dataType>` array2.
Checks for at least one value in common

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 atan

Return the arctangent, in radians from -#pi/2 to #pi/2, of the `<dataType>`
value, as ECMAScript Math.atan does. A result that would be NaN, or a NaN
argument, is an error. An integer is not accepted: write to_double(v).
Arctangent

=head4 Parameters

    $value


=head3 atan2

Return the angle, in radians from -#pi to #pi, from the positive x axis to the
point (x, y), as ECMAScript Math.atan2(y, x) does; y comes first. A result
that would be NaN, or a NaN argument, is an error. An integer is not accepted:
write to_double(v).
Arctangent of a quotient

=head4 Parameters

    $y

The y coordinate.

    $x

The x coordinate.

=head3 cbrt

Return the cube root of the `<dataType>` value, as ECMAScript Math.cbrt does.
A result that would be NaN, or a NaN argument, is an error. An integer is not
accepted: write to_double(v).
Cube root

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

=head3 code_point_at

Return the code point at a zero-based index of the `<dataType>` value as an
integer, as ECMAScript s.codePointAt(i) does. Indexes count code points, as
everywhere in Adaptive Script (not UTF-16 units). Negative indexes count from
the end, as at does. If the index is out of range, the result is undefined, as
for at.
Code point at an index

=head4 Parameters

    $value

The `<dataType>` value.

    $index

Zero-based code point index, or negative from the end.

=head3 compile

Compile `<dataType>` value and return either an unevaluated adaptive value or
a string containing the compiler listing. The listing is a human-oriented dump
(value tree interleaved with source, plus ---Symbols tables) for Fiddle and
debugging — not pure JSON (use stringify) and not Adaptive compiled-form text
(use decompile).
Compile <dataType> value

=head4 Parameters

    $source

`<dataType>` string to compile

    $listing

If specified, a human compiler listing is produced instead of an unevaluated
compiled value (tree + ---Symbols; not recompilable). Use decompile() for
Adaptive compiled-form text and stringify() for pure JSON of evaluated data.

This parameter can be an integer between 0 and 10 or a string that is used for
indentation. If 0 is specified, no whitespace is added to the resulting
string. If 1 through 10 is specified, that number of spaces is used.

=head3 cos

Return the cosine of the `<dataType>` value in radians, as ECMAScript Math.cos
does (#pi is π). An infinite value is an error. A result that would be NaN, or
a NaN argument, is an error. An integer is not accepted: write to_double(v).
Cosine

=head4 Parameters

    $value


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


=head3 eval

Compile and evaluate `<dataType>` value.
Compile and evaluate <dataType> value

=head4 Parameters

    $source

`<dataType>` to compile and evaluate

    $additionalUntrustedQualifiedVariables

This parameter supplies additional qualified variables that can be accessed
during evaluation. These variables will not be used by anything that needs to
ensure its qualified variables must come from a trusted source, such as
authorization. This parameter is intended to be used for testing only and
should not be used for anything running in production.

=head3 exp

Return #e raised to the `<dataType>` value, as ECMAScript Math.exp does. A
result that would be NaN, or a NaN argument, is an error. An integer is not
accepted: write to_double(v).
e raised to a power

=head4 Parameters

    $value


=head3 floor

Determine the largest integer that is smaller then or equal to the
`<dataType>` value and return the `<dataType>` result.
Round downwards to nearest integer

=head4 Parameters

    $number


=head3 freeze

Set a `<dataType>` value immutable so further mutation throws. If already
immutable, has no effect. Returns the same value.
Make <dataType> value immutable

=head4 Parameters

    $value

The `<dataType>` value to freeze.

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


=head3 hypot

Return the square root of the sum of the squares of the `<dataType>` values,
as ECMAScript Math.hypot does, without overflow or underflow in between. At
least one value is required (ECMAScript's hypot() is 0); an infinite value
makes the result Infinity. A result that would be NaN, or a NaN argument, is
an error. An integer is not accepted: write to_double(v).
Square root of the sum of squares

=head4 Parameters

    $values

The values.

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

=head3 is_in

Checks whether `<dataType>` value is in array of `<dataType>` array and
returns the boolean result. Empty array yields false.
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

Return the integer number of entries in a `<dataType>` array or the number of
codepoints in a `<dataType>` anyURI or string.
Returns number of codepoints or entries in value

=head4 Parameters

    $value

Returns the number of entries in an array or code points in others.

=head3 log

Return the natural logarithm (base #e) of the `<dataType>` value, as
ECMAScript Math.log does. log(0.0) is -Infinity; a negative value is an error.
A result that would be NaN, or a NaN argument, is an error. An integer is not
accepted: write to_double(v).
Natural logarithm

=head4 Parameters

    $value


=head3 log10

Return the base 10 logarithm of the `<dataType>` value, as ECMAScript
Math.log10 does. log10(0.0) is -Infinity; a negative value is an error. A
result that would be NaN, or a NaN argument, is an error. An integer is not
accepted: write to_double(v).
Base 10 logarithm

=head4 Parameters

    $value


=head3 log2

Return the base 2 logarithm of the `<dataType>` value, as ECMAScript Math.log2
does. log2(0.0) is -Infinity; a negative value is an error. A result that
would be NaN, or a NaN argument, is an error. An integer is not accepted:
write to_double(v).
Base 2 logarithm

=head4 Parameters

    $value


=head3 lt

Checks for `<dataType>` arg1 is less that `<dataType>` arg2 and return the
boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 match

Searches `<dataType>` value for the regular expression and returns its first
match with the text of its groups, or undefined if there is none, as
ECMAScript's s.match(re) does without the g flag. Use test for a boolean
result.
First match of a regular expression, with its groups

=head4 Parameters

    $value

The `<dataType>` value to search.

    $regexp

The regular expression. A regexp literal (/a+b/ or "a+b"re), to_regexp(s), or
`compile<regexp>(r`) is already compiled; any other regexp value is compiled
for this call.

=head3 match_all

Searches `<dataType>` value for the regular expression and returns each match
with the text of its groups, left to right, as ECMAScript's s.matchAll(re)
does. Matches do not overlap; after an empty match the next search starts one
code point later. length(match_all(s, re)) counts the matches.
Every match of a regular expression, with its groups

=head4 Parameters

    $value

The `<dataType>` value to search.

    $regexp

The regular expression. A regexp literal (/a+b/ or "a+b"re), to_regexp(s), or
`compile<regexp>(r`) is already compiled; any other regexp value is compiled
for this call.

    $limit

The maximum number of matches to return. The default, or any negative limit,
returns every match.

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


=head3 normalize

Return the `<dataType>` value in a Unicode normalization form, as ECMAScript
s.normalize(form) does. Every Adaptive string is already NFC, so form "NFC"
(the default) returns it as a string; "NFKC" also replaces compatibility
characters ("ﬁ" becomes "fi", "①" becomes "1"). "NFD" and "NFKD" are errors: a
string in Adaptive Script is always NFC, so a decomposed result can not be a
string.
Unicode normalization

=head4 Parameters

    $value

The `<dataType>` value.

    $form

"NFC" (the default) or "NFKC".

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


=head3 pad_end

Pad the `<dataType>` value at the end with fillString until it is maxLength
code points long, as ECMAScript s.padEnd() does. Lengths are code points, not
UTF-16 units. Unlike ECMAScript, an empty fillString when padding is needed is
an error, not a quiet return of a shorter value.
Pad at the end to a length

=head4 Parameters

    $value

The `<dataType>` value.

    $maxLength

The length of the result in code points, at most 1,000,000. If it is not more
than the length of value, value is returned (as a string).

    $fillString

The text to pad with, repeated and cut to fit (by code point). The default is
one space. An empty fillString is an error when padding is needed.

=head3 pad_start

Pad the `<dataType>` value at the start with fillString until it is maxLength
code points long, as ECMAScript s.padStart() does. Lengths are code points,
not UTF-16 units. Unlike ECMAScript, an empty fillString when padding is
needed is an error, not a quiet return of a shorter value.
Pad at the start to a length

=head4 Parameters

    $value

The `<dataType>` value.

    $maxLength

The length of the result in code points, at most 1,000,000. If it is not more
than the length of value, value is returned (as a string).

    $fillString

The text to pad with, repeated and cut to fit (by code point). The default is
one space. An empty fillString is an error when padding is needed.

=head3 pow

This returns the value of base raised to a power. Multiple exponents can be
specified to raise the previous exponent to the power of the latter exponent.
An integer result is exact: a negative exponent or a result too large for an
integer is an error.
Base raised to a power

=head4 Parameters

    $base

Base value.

    $exponent

Exponent value.

=head3 repeat

Repeat a `<dataType>` value a specified number of times.
Returns a repeated value

=head4 Parameters

    $value

The `<dataType>` value to repeat.

    $times

The number of times to repeat the value.

=head3 replace

Replace matches in a `<dataType>` value, left to right, up to limit times, as
ECMAScript's s.replace(search, r) does. search is a string, which is literal
text, or a regexp, which is a pattern. The replacement is a string, or a
function called with each match's _AdaptiveRegexpMatch_ object that returns
the string to put in. An empty match is a match at a code-point boundary. The
default limit replaces once (an empty string inserts at the start). Specify
limit -1 to replace every match.
Replace text or regular expression matches

=head4 Parameters

    $value

The original `<dataType>` value.

    $search

What to replace: a string is literal text, and an empty string matches at each
code-point boundary, including the start and the end; a regexp is a pattern. A
regexp literal (/a+b/ or "a+b"re), to_regexp(s), or `compile<regexp>(r`) is
already compiled; any other regexp value is compiled for this call.

    $replacement

The replacement for each match: a string, or a function. A function is called
with one argument, the match's _AdaptiveRegexpMatch_ object ({match, index,
captures, groups}), and must return a string. A string replacement is literal
text when search is a string. When search is a regexp, $n is the text of group
n ($0 the whole match), ${name} the text of a named group, and a backslash
makes the next character literal (\$ for a dollar sign); any other $ is an
error.

    $limit

The maximum number of matches to replace. The default is 1. Specify -1 (or any
negative) to replace every match.

=head3 replace_all

Replace every match in a `<dataType>` value, left to right, as ECMAScript
s.replaceAll(search, r) does; the same as replace(value, search, replacement,
-1). search is a string, which is literal text, or a regexp, which is a
pattern (no g flag is needed). The replacement is a string, or a function
called with each match's _AdaptiveRegexpMatch_ object that returns the string
to put in.
Replace every match

=head4 Parameters

    $value

The `<dataType>` value.

    $search

What to replace: a string is literal text, and an empty string matches at each
code-point boundary, including the start and the end; a regexp is a pattern. A
regexp literal (/a+b/ or "a+b"re), to_regexp(s), or `compile<regexp>(r`) is
already compiled; any other regexp value is compiled for this call.

    $replacement

The replacement for each match: a string, or a function. A function is called
with one argument, the match's _AdaptiveRegexpMatch_ object ({match, index,
captures, groups}), and must return a string. A string replacement is literal
text when search is a string. When search is a regexp, $n is the text of group
n ($0 the whole match), ${name} the text of a named group, and a backslash
makes the next character literal (\$ for a dollar sign); any other $ is an
error.

=head3 round

Determine the integer closest to `<dataType>` value and return the
`<dataType>` result. A value halfway between two integers rounds toward
positive infinity, as ECMAScript Math.round and XPath fn:round do: round(2.5)
is 3 and round(-2.5) is -2.
Round to nearest integer

=head4 Parameters

    $number


=head3 search

Searches `<dataType>` value for the regular expression and returns the code
point index of the first match, or -1 if there is none, as ECMAScript's
s.search(re) does. Use index_of to find literal text.
Code point index of the first match of a regular expression

=head4 Parameters

    $value

The `<dataType>` value to search.

    $regexp

The regular expression. A regexp literal (/a+b/ or "a+b"re), to_regexp(s), or
`compile<regexp>(r`) is already compiled; any other regexp value is compiled
for this call.

=head3 set_equals

Returns boolean true if `<dataType>` array1 and `<dataType>` array2 are
subsets of each other and return the boolean result.
Checks whether two arrays are subsets of each other

=head4 Parameters

    $array1


    $array2


=head3 sign

Return the sign of the `<dataType>` value, as ECMAScript Math.sign does: -1, 0
or 1 of the same data type (`sign<double>` keeps the sign of a zero:
sign(-0.0) is -0.0). A NaN argument is an error.
Sign of a number

=head4 Parameters

    $value


=head3 sin

Return the sine of the `<dataType>` value in radians, as ECMAScript Math.sin
does (#pi is π). An infinite value is an error. A result that would be NaN, or
a NaN argument, is an error. An integer is not accepted: write to_double(v).
Sine

=head4 Parameters

    $value


=head3 split

Split `<dataType>` value into an array of strings, as ECMAScript's
s.split(separator) does. separator is a string, which is literal text, or a
regexp, which is a pattern. With a regexp, the text of each group of a match
is put in the array after the piece before it (undefined when the group did
not take part), and an empty match splits between code points but not at the
start or end of the value.
Split at a separator or regular expression

=head4 Parameters

    $value

The `<dataType>` value to split.

    $separator

Where to split: a string is literal text, a regexp is a pattern. If this is an
empty string or separator is not specified, the value is split between
characters. A regexp literal (/a+b/ or "a+b"re), to_regexp(s), or
`compile<regexp>(r`) is already compiled; any other regexp value is compiled
for this call.

    $limit

The maximum number of entries in the result. Any remaining part of value is
ignored. The default, or any negative limit, is no limit.

=head3 sqrt

Return the square root of the `<dataType>` value, as ECMAScript Math.sqrt
does; sqrt(-0.0) is -0.0. A negative value is an error. A result that would be
NaN, or a NaN argument, is an error. An integer is not accepted: write
to_double(v).
Square root

=head4 Parameters

    $value


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

Returns the string substring of value from zero-based code point position
integer startIndex up to, but not including, endIndex, as ECMAScript substring
does. A negative index is 0 and an index past the end is the end of value.
Omit endIndex to return through the end of value. If endIndex is less than
startIndex, the two are swapped, and equal indexes return an empty string. Use
slice to count from the end. The result is always string (a slice of anyURI is
not an anyURI).
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


=head3 tan

Return the tangent of the `<dataType>` value in radians, as ECMAScript
Math.tan does (#pi is π). An infinite value is an error. A result that would
be NaN, or a NaN argument, is an error. An integer is not accepted: write
to_double(v).
Tangent

=head4 Parameters

    $value


=head3 to_anyURI

Converts `<dataType>` value to anyURI. A string is read with the anyURI
parser, the same parser the typed literal "..."anyURI uses, and text the
parser rejects is an error.
Converts string to anyURI

=head4 Parameters

    $value


=head3 to_base64Binary

Converts `<dataType>` value to base64Binary. A string is read with the
base64Binary parser, the same parser the typed literal "..."base64Binary uses,
and text the parser rejects is an error.
Converts string to base64Binary

=head4 Parameters

    $value


=head3 to_date

Converts `<dataType>` value to date. A string is read with the date parser,
the same parser the typed literal "..."date uses, and text the parser rejects
is an error.
Converts string to date

=head4 Parameters

    $value


=head3 to_dateTime

Converts `<dataType>` value to dateTime. A string is read with the dateTime
parser, the same parser the typed literal "..."dateTime uses, and text the
parser rejects is an error.
Converts string to dateTime

=head4 Parameters

    $value


=head3 to_dayTimeDuration

Converts `<dataType>` value to dayTimeDuration. A string is read with the
dayTimeDuration parser, the same parser the typed literal "..."dayTimeDuration
uses, and text the parser rejects is an error.
Converts string to dayTimeDuration

=head4 Parameters

    $value


=head3 to_dnsName

Converts `<dataType>` value to dnsName. A string is read with the dnsName
parser, the same parser the typed literal "..."dnsName uses, and text the
parser rejects is an error.
Converts string to dnsName

=head4 Parameters

    $value


=head3 to_double

Converts `<dataType>` value to double. A string is read as XML Schema double
text: digits with an optional sign, decimal point, and exponent ("5", ".5",
"5.", "007.5", "1e3"), INF, +INF, -INF, or NaN. Other text, including leading
or trailing spaces, is an error. Only text is read this way: an integer value
is never silently a double.
Converts to double

=head4 Parameters

    $value


=head3 to_hexBinary

Converts `<dataType>` value to hexBinary. A string is read with the hexBinary
parser, the same parser the typed literal "..."hexBinary uses, and text the
parser rejects is an error.
Converts string to hexBinary

=head4 Parameters

    $value


=head3 to_ia5String

Converts `<dataType>` value to ia5String. A string is read with the ia5String
parser, the same parser the typed literal "..."ia5String uses, and text the
parser rejects is an error.
Converts string to ia5String

=head4 Parameters

    $value


=head3 to_integer

Converts `<dataType>` value to integer. A double is truncated toward zero (1.9
is 1 and -1.9 is -1); use round(), floor(), or ceil() first for another
choice. A double that is NaN, infinite, or outside the integer range is an
error. A string is read as XML Schema integer text: digits with an optional
sign, leading zeros allowed ("007" is 7). Text that is not an integer ("1.5",
" 1", "1x") is an error.
Converts to integer

=head4 Parameters

    $value


=head3 to_ipAddress

Converts `<dataType>` value to ipAddress. A string is read with the ipAddress
parser, the same parser the typed literal "..."ipAddress uses, and text the
parser rejects is an error.
Converts string to ipAddress

=head4 Parameters

    $value


=head3 to_lower_case

Return the `<dataType>` value in lower case, as ECMAScript s.toLowerCase()
does: Unicode full case mapping, the same in every locale ("İ" becomes "i̇").
The result is a string. normalize_to_lower_case maps one code point at a time.
Lower case

=head4 Parameters

    $value

The `<dataType>` value.

=head3 to_objectId

Converts `<dataType>` value to objectId. A string is read with the objectId
parser, and text the parser rejects is an error.
Converts string to objectId

=head4 Parameters

    $value


=head3 to_objectPath

Converts `<dataType>` value to objectPath. A string is read with the
objectPath parser, and text the parser rejects is an error.
Converts string to objectPath

=head4 Parameters

    $value


=head3 to_password

Converts `<dataType>` value to password. A string is read with the password
parser, the same parser the typed literal "..."password uses, and text the
parser rejects is an error.
Converts string to password

=head4 Parameters

    $value


=head3 to_regexp

Converts `<dataType>` value to a compiled regexp, the same value as a regexp
literal. A pattern ICU does not accept is a conversion_error.
Converts string to regexp

=head4 Parameters

    $value


=head3 to_rfc822Name

Converts `<dataType>` value to rfc822Name. A string is read with the
rfc822Name parser, the same parser the typed literal "..."rfc822Name uses, and
text the parser rejects is an error.
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

Converts `<dataType>` value to time. A string is read with the time parser,
the same parser the typed literal "..."time uses, and text the parser rejects
is an error.
Converts string to time

=head4 Parameters

    $value


=head3 to_upper_case

Return the `<dataType>` value in upper case, as ECMAScript s.toUpperCase()
does: Unicode full case mapping, the same in every locale, so one code point
can become more than one ("ß" becomes "SS"). The result is a string.
Upper case

=head4 Parameters

    $value

The `<dataType>` value.

=head3 to_x500Name

Converts `<dataType>` value to x500Name. A string is read with the x500Name
parser, the same parser the typed literal "..."x500Name uses, and text the
parser rejects is an error.
Converts string to x500Name

=head4 Parameters

    $value


=head3 to_yearMonthDuration

Converts `<dataType>` value to yearMonthDuration. A string is read with the
yearMonthDuration parser, the same parser the typed literal
"..."yearMonthDuration uses, and text the parser rejects is an error.
Converts string to yearMonthDuration

=head4 Parameters

    $value


=head3 trim

Remove white space and line terminators as ECMAScript counts them (tab,
vertical tab, form feed, space, no-break space, U+FEFF, every Unicode space
separator, LF, CR, U+2028 and U+2029) from the beginning and end of the
`<dataType>` value, as ECMAScript s.trim() does. White space inside the value
is kept; normalize_space also collapses it. The result is a string.
Remove white space from both ends

=head4 Parameters

    $value

The `<dataType>` value.

=head3 trim_end

Remove white space and line terminators as ECMAScript counts them (tab,
vertical tab, form feed, space, no-break space, U+FEFF, every Unicode space
separator, LF, CR, U+2028 and U+2029) from the end of the `<dataType>` value,
as ECMAScript s.trimEnd() does. The result is a string.
Remove white space from the end

=head4 Parameters

    $value

The `<dataType>` value.

=head3 trim_start

Remove white space and line terminators as ECMAScript counts them (tab,
vertical tab, form feed, space, no-break space, U+FEFF, every Unicode space
separator, LF, CR, U+2028 and U+2029) from the beginning of the `<dataType>`
value, as ECMAScript s.trimStart() does. The result is a string.
Remove white space from the start

=head4 Parameters

    $value

The `<dataType>` value.

=head3 trunc

Return the integer part of the `<dataType>` value, removing any fraction, as
ECMAScript Math.trunc does (toward zero: trunc(-1.5) is -1.0). The result is a
double: write to_integer(trunc(v)) for an integer. A result that would be NaN,
or a NaN argument, is an error. An integer is not accepted: write
to_double(v).
Integer part

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

URL encode a `<dataType>` value and return the string result, as ECMAScript
encodeURI does: every character is percent-encoded as UTF-8 except A-Z a-z 0-9
; , / ? : @ & = + $ - _ . ! ~ * ' ( ) #. Characters that give a URI its
structure are kept, so this encodes a whole URI, not a single query value or
path segment.
URI encode

=head4 Parameters

    $unencoded

The `<dataType>` value to URL encode.

=cut

sub abs {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "abs");
    $request->set("value", $value);

    return $request->getResult();
}

sub acos {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "acos");
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

sub asin {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "asin");
    $request->set("value", $value);

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

sub atan {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "atan");
    $request->set("value", $value);

    return $request->getResult();
}

sub atan2 {
    my ($y, $x) = @_;

    my $request = $session->request()

    $request->set("function" => "atan2");
    $request->set("y", $y);
    $request->set("x", $x);

    return $request->getResult();
}

sub cbrt {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "cbrt");
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

sub code_point_at {
    my ($value, $index) = @_;

    my $request = $session->request()

    $request->set("function" => "code_point_at");
    $request->set("value", $value);
    $request->set("index", $index);

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

sub cos {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "cos");
    $request->set("value", $value);

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

sub eval {
    my ($source, $additionalUntrustedQualifiedVariables) = @_;

    my $request = $session->request()

    $request->set("function" => "eval");
    $request->set("source", $source);

    if (defined $additionalUntrustedQualifiedVariables)
        $request->set("additionalUntrustedQualifiedVariables", $additionalUntrustedQualifiedVariables);

    return $request->getResult();
}

sub exp {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "exp");
    $request->set("value", $value);

    return $request->getResult();
}

sub floor {
    my ($number) = @_;

    my $request = $session->request()

    $request->set("function" => "floor");
    $request->set("number", $number);

    return $request->getResult();
}

sub freeze {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "freeze");
    $request->set("value", $value);

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

sub hypot {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "hypot");
    $request->set("values", $values);

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

sub log {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "log");
    $request->set("value", $value);

    return $request->getResult();
}

sub log10 {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "log10");
    $request->set("value", $value);

    return $request->getResult();
}

sub log2 {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "log2");
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
    my ($value, $regexp) = @_;

    my $request = $session->request()

    $request->set("function" => "match");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

    return $request->getResult();
}

sub match_all {
    my ($value, $regexp, $limit) = @_;

    my $request = $session->request()

    $request->set("function" => "match_all");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

    if (defined $limit)
        $request->set("limit", $limit);

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

sub normalize {
    my ($value, $form) = @_;

    my $request = $session->request()

    $request->set("function" => "normalize");
    $request->set("value", $value);

    if (defined $form)
        $request->set("form", $form);

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

sub pad_end {
    my ($value, $maxLength, $fillString) = @_;

    my $request = $session->request()

    $request->set("function" => "pad_end");
    $request->set("value", $value);
    $request->set("maxLength", $maxLength);

    if (defined $fillString)
        $request->set("fillString", $fillString);

    return $request->getResult();
}

sub pad_start {
    my ($value, $maxLength, $fillString) = @_;

    my $request = $session->request()

    $request->set("function" => "pad_start");
    $request->set("value", $value);
    $request->set("maxLength", $maxLength);

    if (defined $fillString)
        $request->set("fillString", $fillString);

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

sub repeat {
    my ($value, $times) = @_;

    my $request = $session->request()

    $request->set("function" => "repeat");
    $request->set("value", $value);
    $request->set("times", $times);

    return $request->getResult();
}

sub replace {
    my ($value, $search, $replacement, $limit) = @_;

    my $request = $session->request()

    $request->set("function" => "replace");
    $request->set("value", $value);
    $request->set("search", $search);
    $request->set("replacement", $replacement);

    if (defined $limit)
        $request->set("limit", $limit);

    return $request->getResult();
}

sub replace_all {
    my ($value, $search, $replacement) = @_;

    my $request = $session->request()

    $request->set("function" => "replace_all");
    $request->set("value", $value);
    $request->set("search", $search);
    $request->set("replacement", $replacement);

    return $request->getResult();
}

sub round {
    my ($number) = @_;

    my $request = $session->request()

    $request->set("function" => "round");
    $request->set("number", $number);

    return $request->getResult();
}

sub search {
    my ($value, $regexp) = @_;

    my $request = $session->request()

    $request->set("function" => "search");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

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

sub sign {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "sign");
    $request->set("value", $value);

    return $request->getResult();
}

sub sin {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "sin");
    $request->set("value", $value);

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

sub sqrt {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "sqrt");
    $request->set("value", $value);

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

sub tan {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "tan");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_anyURI {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_anyURI");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_base64Binary {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_base64Binary");
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

sub to_hexBinary {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_hexBinary");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_ia5String {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_ia5String");
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

sub to_lower_case {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_lower_case");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_objectId {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_objectId");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_objectPath {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_objectPath");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_password {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_password");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_regexp {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_regexp");
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

sub to_upper_case {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_upper_case");
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

sub trim {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "trim");
    $request->set("value", $value);

    return $request->getResult();
}

sub trim_end {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "trim_end");
    $request->set("value", $value);

    return $request->getResult();
}

sub trim_start {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "trim_start");
    $request->set("value", $value);

    return $request->getResult();
}

sub trunc {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "trunc");
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
