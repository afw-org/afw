#!/usr/bin/env perl

#**
# @file anyURI.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the anyURI category.
#*

package afw::anyURI;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    array_anyURI 
    as_anyURI 
    at_least_one_member_of_anyURI 
    ends_with_anyURI 
    eq_anyURI 
    eqx_anyURI 
    ge_anyURI 
    gt_anyURI 
    includes_anyURI 
    index_of_anyURI 
    intersection_anyURI 
    is_anyURI 
    is_in_anyURI 
    last_index_of_anyURI 
    le_anyURI 
    length_anyURI 
    lt_anyURI 
    match_all_anyURI 
    match_anyURI 
    ne_anyURI 
    nex_anyURI 
    one_and_only_anyURI 
    repeat_anyURI 
    replace_anyURI 
    search_anyURI 
    set_equals_anyURI 
    split_anyURI 
    starts_with_anyURI 
    subset_anyURI 
    substring_anyURI 
    to_string_anyURI 
    union_anyURI 
    url_encode_anyURI 
);

=head1 NAME

afw::anyURI - Perl module for anyURI functions

=head2 Functions

The following functions are exported by default

=head3 array_anyURI

Construct a new array of anyURI from the given values, the same as array()
except that the new array is typed: each value, and each entry of a spread
array, must be dataType anyURI, and adding a value of another data type to the
array later throws. A value of another data type is an error, never converted.
This is usually written `array<anyURI>(`...values).
Construct an array of anyURI from values or spread arrays

=head4 Parameters

    $values

A value of dataType anyURI, or ... followed by an expression that results in
an array, every entry of which is included.

=head3 as_anyURI

Returns value unchanged if it is dataType anyURI. Otherwise, including when
value is undefined, throws an error. It never converts: to get a anyURI from a
value of another data type, write a typed literal or call a to_T conversion
function where one exists. This is usually written value as anyURI.
Asserts that value is dataType anyURI

=head4 Parameters

    $value

Value to assert

=head3 at_least_one_member_of_anyURI

Returns boolean true if at least one value in anyURI array1 is in anyURI
array2.
Checks for at least one value in common

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 ends_with_anyURI

Checks whether anyURI value ends with a anyURI and return the boolean result.
Checks whether value ends with a string

=head4 Parameters

    $value


    $subString


=head3 eq_anyURI

Determine if anyURI arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_anyURI

Determine if for anyURI arg1 is equal to the value and data type of arg2 then
return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 ge_anyURI

Checks for anyURI arg1 is greater than or equal to anyURI arg2 and return the
boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_anyURI

Checks for anyURI arg1 is greater than anyURI arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 includes_anyURI

Checks whether the anyURI value includes a string and return the boolean
result.
Checks whether the anyURI value includes a string

=head4 Parameters

    $searchString

The anyURI to search.

    $subString

Substring to find.

    $position

Zero-based position in the search string to start search.

=head3 index_of_anyURI

Returns the zero-based index into anyURI value of subString. If subString is
not found, -1 is returned.
Returns index of first occurrence

=head4 Parameters

    $value

The anyURI value to search.

    $subString

Substring to search for.

    $startIndex

Optional start index for search if different than start of string.

=head3 intersection_anyURI

Returns an array of anyURI with the values that are common to both array of
anyURI array1 and array2.
Returns intersection of two arrays

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 is_anyURI

Checks whether value is dataType anyURI and returns the boolean result. It is
false for undefined. This can also be written `is<anyURI>(value`).
Checks whether value is dataType anyURI

=head4 Parameters

    $value

Value to check

=head3 is_in_anyURI

Checks whether anyURI value is in array of anyURI array and returns the
boolean result. Empty array yields false.
Checks whether a value is in an array

=head4 Parameters

    $value


    $array


=head3 last_index_of_anyURI

Returns the zero-based index into anyURI value of the last occurrence of a
subString. If subString is not found, -1 is returned.
Returns index of last occurrence

=head4 Parameters

    $value

The anyURI value to search.

    $subString

Substring to search for.

    $startIndex

Optional start index for search if different than start of string.

=head3 le_anyURI

Checks for anyURI arg1 is less than or equal to anyURI arg2 and return the
boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 length_anyURI

Return the integer number of entries in a anyURI array or the number of
codepoints in a anyURI anyURI or string.
Returns number of codepoints or entries in value

=head4 Parameters

    $value

Returns the number of entries in an array or code points in others.

=head3 lt_anyURI

Checks for anyURI arg1 is less that anyURI arg2 and return the boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 match_all_anyURI

Searches anyURI value for the regular expression and returns each match with
the text of its groups, left to right, as ECMAScript's s.matchAll(re) does.
Matches do not overlap; after an empty match the next search starts one code
point later. length(match_all(s, re)) counts the matches.
Every match of a regular expression, with its groups

=head4 Parameters

    $value

The anyURI value to search.

    $regexp

The regular expression. A regexp literal (/a+b/ or "a+b"re), to_regexp(s), or
`compile<regexp>(r`) is already compiled; any other regexp value is compiled
for this call.

    $limit

The maximum number of matches to return. The default, or any negative limit,
returns every match.

=head3 match_anyURI

Searches anyURI value for the regular expression and returns its first match
with the text of its groups, or undefined if there is none, as ECMAScript's
s.match(re) does without the g flag. Use test for a boolean result.
First match of a regular expression, with its groups

=head4 Parameters

    $value

The anyURI value to search.

    $regexp

The regular expression. A regexp literal (/a+b/ or "a+b"re), to_regexp(s), or
`compile<regexp>(r`) is already compiled; any other regexp value is compiled
for this call.

=head3 ne_anyURI

Determine if anyURI arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead if
you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_anyURI

Determine if for anyURI arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 one_and_only_anyURI

This converts an array of anyURI values that contains one value to a single
anyURI value.
Converts a one value array to a anyURI value

=head4 Parameters

    $array


=head3 repeat_anyURI

Repeat a anyURI value a specified number of times.
Returns a repeated value

=head4 Parameters

    $value

The anyURI value to repeat.

    $times

The number of times to repeat the value.

=head3 replace_anyURI

Replace matches in a anyURI value, left to right, up to limit times, as
ECMAScript's s.replace(search, r) does. search is a string, which is literal
text, or a regexp, which is a pattern. The replacement is a string, or a
function called with each match's _AdaptiveRegexpMatch_ object that returns
the string to put in. An empty match is a match at a code-point boundary. The
default limit replaces once (an empty string inserts at the start). Specify
limit -1 to replace every match.
Replace text or regular expression matches

=head4 Parameters

    $value

The original anyURI value.

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

=head3 search_anyURI

Searches anyURI value for the regular expression and returns the code point
index of the first match, or -1 if there is none, as ECMAScript's s.search(re)
does. Use index_of to find literal text.
Code point index of the first match of a regular expression

=head4 Parameters

    $value

The anyURI value to search.

    $regexp

The regular expression. A regexp literal (/a+b/ or "a+b"re), to_regexp(s), or
`compile<regexp>(r`) is already compiled; any other regexp value is compiled
for this call.

=head3 set_equals_anyURI

Returns boolean true if anyURI array1 and anyURI array2 are subsets of each
other and return the boolean result.
Checks whether two arrays are subsets of each other

=head4 Parameters

    $array1


    $array2


=head3 split_anyURI

Split anyURI value into an array of strings, as ECMAScript's
s.split(separator) does. separator is a string, which is literal text, or a
regexp, which is a pattern. With a regexp, the text of each group of a match
is put in the array after the piece before it (undefined when the group did
not take part), and an empty match splits between code points but not at the
start or end of the value.
Split at a separator or regular expression

=head4 Parameters

    $value

The anyURI value to split.

    $separator

Where to split: a string is literal text, a regexp is a pattern. If this is an
empty string or separator is not specified, the value is split between
characters. A regexp literal (/a+b/ or "a+b"re), to_regexp(s), or
`compile<regexp>(r`) is already compiled; any other regexp value is compiled
for this call.

    $limit

The maximum number of entries in the result. Any remaining part of value is
ignored. The default, or any negative limit, is no limit.

=head3 starts_with_anyURI

Checks whether anyURI value starts with a subString and return the boolean
result.
Checks whether value starts with a string

=head4 Parameters

    $value


    $subString


=head3 subset_anyURI

Returns boolean true if the unique values in anyURI array1 are all in anyURI
array2.
Determines if the first array is a subset of second array

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 substring_anyURI

Returns the string substring of value beginning at zero-based position integer
startIndex and ending at the position before integer endIndex. Specify -1 or
omit endIndex to return through the end of value. The result is always string
(a slice of anyURI is not an anyURI).
Extract a substring

=head4 Parameters

    $string


    $startIndex


    $endIndex


=head3 to_string_anyURI

Converts anyURI value to string. For array values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A anyURI value.

=head3 union_anyURI

Returns an array of anyURI contains all of the unique values in two or more
array of anyURI values.
Returns union of two or more string arrays

=head4 Parameters

    $arrays

Two or more arrays.

=head3 url_encode_anyURI

URL encode a anyURI value and return the string result.
URI encode

=head4 Parameters

    $unencoded

The anyURI value to URL encode.

=cut

sub array_anyURI {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "array_anyURI");
    $request->set("values", $values);

    return $request->getResult();
}

sub as_anyURI {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "as_anyURI");
    $request->set("value", $value);

    return $request->getResult();
}

sub at_least_one_member_of_anyURI {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<anyURI>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub ends_with_anyURI {
    my ($value, $subString) = @_;

    my $request = $session->request()

    $request->set("function" => "ends_with<anyURI>");
    $request->set("value", $value);
    $request->set("subString", $subString);

    return $request->getResult();
}

sub eq_anyURI {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<anyURI>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_anyURI {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<anyURI>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ge_anyURI {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<anyURI>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_anyURI {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<anyURI>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub includes_anyURI {
    my ($searchString, $subString, $position) = @_;

    my $request = $session->request()

    $request->set("function" => "includes<anyURI>");
    $request->set("searchString", $searchString);
    $request->set("subString", $subString);

    if (defined $position)
        $request->set("position", $position);

    return $request->getResult();
}

sub index_of_anyURI {
    my ($value, $subString, $startIndex) = @_;

    my $request = $session->request()

    $request->set("function" => "index_of<anyURI>");
    $request->set("value", $value);
    $request->set("subString", $subString);

    if (defined $startIndex)
        $request->set("startIndex", $startIndex);

    return $request->getResult();
}

sub intersection_anyURI {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<anyURI>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub is_anyURI {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is_anyURI");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_in_anyURI {
    my ($value, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<anyURI>");
    $request->set("value", $value);
    $request->set("array", $array);

    return $request->getResult();
}

sub last_index_of_anyURI {
    my ($value, $subString, $startIndex) = @_;

    my $request = $session->request()

    $request->set("function" => "last_index_of<anyURI>");
    $request->set("value", $value);
    $request->set("subString", $subString);

    if (defined $startIndex)
        $request->set("startIndex", $startIndex);

    return $request->getResult();
}

sub le_anyURI {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<anyURI>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub length_anyURI {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "length<anyURI>");
    $request->set("value", $value);

    return $request->getResult();
}

sub lt_anyURI {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<anyURI>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub match_all_anyURI {
    my ($value, $regexp, $limit) = @_;

    my $request = $session->request()

    $request->set("function" => "match_all<anyURI>");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

    if (defined $limit)
        $request->set("limit", $limit);

    return $request->getResult();
}

sub match_anyURI {
    my ($value, $regexp) = @_;

    my $request = $session->request()

    $request->set("function" => "match<anyURI>");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

    return $request->getResult();
}

sub ne_anyURI {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<anyURI>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_anyURI {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<anyURI>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub one_and_only_anyURI {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<anyURI>");
    $request->set("array", $array);

    return $request->getResult();
}

sub repeat_anyURI {
    my ($value, $times) = @_;

    my $request = $session->request()

    $request->set("function" => "repeat<anyURI>");
    $request->set("value", $value);
    $request->set("times", $times);

    return $request->getResult();
}

sub replace_anyURI {
    my ($value, $search, $replacement, $limit) = @_;

    my $request = $session->request()

    $request->set("function" => "replace<anyURI>");
    $request->set("value", $value);
    $request->set("search", $search);
    $request->set("replacement", $replacement);

    if (defined $limit)
        $request->set("limit", $limit);

    return $request->getResult();
}

sub search_anyURI {
    my ($value, $regexp) = @_;

    my $request = $session->request()

    $request->set("function" => "search<anyURI>");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

    return $request->getResult();
}

sub set_equals_anyURI {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<anyURI>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub split_anyURI {
    my ($value, $separator, $limit) = @_;

    my $request = $session->request()

    $request->set("function" => "split<anyURI>");
    $request->set("value", $value);

    if (defined $separator)
        $request->set("separator", $separator);

    if (defined $limit)
        $request->set("limit", $limit);

    return $request->getResult();
}

sub starts_with_anyURI {
    my ($value, $subString) = @_;

    my $request = $session->request()

    $request->set("function" => "starts_with<anyURI>");
    $request->set("value", $value);
    $request->set("subString", $subString);

    return $request->getResult();
}

sub subset_anyURI {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<anyURI>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub substring_anyURI {
    my ($string, $startIndex, $endIndex) = @_;

    my $request = $session->request()

    $request->set("function" => "substring<anyURI>");
    $request->set("string", $string);
    $request->set("startIndex", $startIndex);

    if (defined $endIndex)
        $request->set("endIndex", $endIndex);

    return $request->getResult();
}

sub to_string_anyURI {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<anyURI>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_anyURI {
    my ($arrays) = @_;

    my $request = $session->request()

    $request->set("function" => "union<anyURI>");
    $request->set("arrays", $arrays);

    return $request->getResult();
}

sub url_encode_anyURI {
    my ($unencoded) = @_;

    my $request = $session->request()

    $request->set("function" => "url_encode<anyURI>");
    $request->set("unencoded", $unencoded);

    return $request->getResult();
}

42;
