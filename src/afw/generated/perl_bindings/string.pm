#!/usr/bin/env perl

#**
# @file string.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the string category.
#*

package afw::string;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    add_string 
    array_string 
    as_string 
    at_least_one_member_of_string 
    concat 
    encode_as_base64Binary_string 
    encode_as_hexBinary_string 
    ends_with_string 
    eq_ignore_case_string 
    eq_string 
    eqx_string 
    eval_string 
    ge_string 
    gt_string 
    includes_string 
    index_of_string 
    intersection_string 
    is_in_string 
    is_string 
    last_index_of_string 
    le_string 
    length_string 
    lt_string 
    match_all_string 
    match_string 
    max_string 
    min_string 
    ne_string 
    nex_string 
    normalize_space_string 
    normalize_to_lower_case_string 
    one_and_only_string 
    repeat_string 
    replace_string 
    search_string 
    set_equals_string 
    split_string 
    starts_with_string 
    string 
    subset_string 
    substring_string 
    to_anyURI_string 
    to_base64Binary_string 
    to_dateTime_string 
    to_date_string 
    to_dayTimeDuration_string 
    to_dnsName_string 
    to_double_string 
    to_hexBinary_string 
    to_ia5String_string 
    to_integer_string 
    to_ipAddress_string 
    to_objectId_string 
    to_objectPath_string 
    to_password_string 
    to_regexp_string 
    to_rfc822Name_string 
    to_string_string 
    to_time_string 
    to_x500Name_string 
    to_yearMonthDuration_string 
    union_string 
    url_decode 
    url_encode_string 
);

=head1 NAME

afw::string - Perl module for string functions

=head2 Functions

The following functions are exported by default

=head3 add_string

Add (concatenate) 2 or more string values and return the string result. Every
value must be a string, the same as the + operator: to convert values while
concatenating, use concat() or string().
Add (concatenate) strings

=head4 Parameters

    $values


=head3 array_string

Construct a new array of string from the given values, the same as array()
except that the new array is typed: each value, and each entry of a spread
array, must be dataType string, and adding a value of another data type to the
array later throws. A value of another data type is an error, never converted.
This is usually written `array<string>(`...values).
Construct an array of string from values or spread arrays

=head4 Parameters

    $values

A value of dataType string, or ... followed by an expression that results in
an array, every entry of which is included.

=head3 as_string

Returns value unchanged if it is dataType string. Otherwise, including when
value is undefined, throws an error. It never converts: to get a string from a
value of another data type, write a typed literal or call a to_T conversion
function where one exists. This is usually written value as string.
Asserts that value is dataType string

=head4 Parameters

    $value

Value to assert

=head3 at_least_one_member_of_string

Returns boolean true if at least one value in string array1 is in string
array2.
Checks for at least one value in common

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 concat

concat follows its first value. If it is an array, return a new array of the
entries of every value in order, as ECMAScript array concat does, except that
every value must be an array (write [...a, v] or push(a, v) to add a single
value). Otherwise convert every value of any data type to string and return
the concatenated result; a value with an undefined value is represented by
'undefined'.
Concatenate arrays, or values as strings

=head4 Parameters

    $values

Arrays to concatenate, or values to convert to strings and concatenate.

=head3 encode_as_base64Binary_string

Encode a value as a base64Binary. The effect is to create a base64Binary value
with an internal value of the value passed.
Encode to a base64Binary value

=head4 Parameters

    $value

The string value to encode.

=head3 encode_as_hexBinary_string

Encode a value as a hexBinary. The effect is to create a hexBinary value with
an internal value of the value passed.
Encode to a hexBinary value

=head4 Parameters

    $value

The string value to encode.

=head3 ends_with_string

Checks whether string value ends with a string and return the boolean result.
Checks whether value ends with a string

=head4 Parameters

    $value


    $subString


=head3 eq_ignore_case_string

Checks for string arg1 is equal to string arg2 ignoring case and return the
boolean result.
Checks for equal ignoring case

=head4 Parameters

    $arg1


    $arg2


=head3 eq_string

Determine if string arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_string

Determine if for string arg1 is equal to the value and data type of arg2 then
return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 eval_string

Compile and evaluate string value as adaptive script.
Compile and evaluate string value as adaptive script

=head4 Parameters

    $source

string to compile and evaluate

    $additionalUntrustedQualifiedVariables

This parameter supplies additional qualified variables that can be accessed
during evaluation. These variables will not be used by anything that needs to
ensure its qualified variables must come from a trusted source, such as
authorization. This parameter is intended to be used for testing only and
should not be used for anything running in production.

=head3 ge_string

Checks for string arg1 is greater than or equal to string arg2 and return the
boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_string

Checks for string arg1 is greater than string arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 includes_string

Checks whether the string value includes a string and return the boolean
result.
Checks whether the string value includes a string

=head4 Parameters

    $searchString

The string to search.

    $subString

Substring to find.

    $position

Zero-based position in the search string to start search.

=head3 index_of_string

Returns the zero-based index into string value of subString. If subString is
not found, -1 is returned.
Returns index of first occurrence

=head4 Parameters

    $value

The string value to search.

    $subString

Substring to search for.

    $startIndex

Optional start index for search if different than start of string.

=head3 intersection_string

Returns an array of string with the values that are common to both array of
string array1 and array2.
Returns intersection of two arrays

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 is_in_string

Checks whether string value is in array of string array and returns the
boolean result. Empty array yields false.
Checks whether a value is in an array

=head4 Parameters

    $value


    $array


=head3 is_string

Checks whether value is dataType string and returns the boolean result. It is
false for undefined. This can also be written `is<string>(value`).
Checks whether value is dataType string

=head4 Parameters

    $value

Value to check

=head3 last_index_of_string

Returns the zero-based index into string value of the last occurrence of a
subString. If subString is not found, -1 is returned.
Returns index of last occurrence

=head4 Parameters

    $value

The string value to search.

    $subString

Substring to search for.

    $startIndex

Optional start index for search if different than start of string.

=head3 le_string

Checks for string arg1 is less than or equal to string arg2 and return the
boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 length_string

Return the integer number of entries in a string array or the number of
codepoints in a string anyURI or string.
Returns number of codepoints or entries in value

=head4 Parameters

    $value

Returns the number of entries in an array or code points in others.

=head3 lt_string

Checks for string arg1 is less that string arg2 and return the boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 match_all_string

Searches string value for the regular expression and returns each match with
the text of its groups, left to right, as ECMAScript's s.matchAll(re) does.
Matches do not overlap; after an empty match the next search starts one code
point later. length(match_all(s, re)) counts the matches.
Every match of a regular expression, with its groups

=head4 Parameters

    $value

The string value to search.

    $regexp

The regular expression. A regexp literal (/a+b/ or "a+b"re), to_regexp(s), or
`compile<regexp>(r`) is already compiled; any other regexp value is compiled
for this call.

    $limit

The maximum number of matches to return. The default, or any negative limit,
returns every match.

=head3 match_string

Searches string value for the regular expression and returns its first match
with the text of its groups, or undefined if there is none, as ECMAScript's
s.match(re) does without the g flag. Use test for a boolean result.
First match of a regular expression, with its groups

=head4 Parameters

    $value

The string value to search.

    $regexp

The regular expression. A regexp literal (/a+b/ or "a+b"re), to_regexp(s), or
`compile<regexp>(r`) is already compiled; any other regexp value is compiled
for this call.

=head3 max_string

Return the string value that is greater than or equal to the others.
Maximum value

=head4 Parameters

    $values


=head3 min_string

Return the string value that is less than or equal to the others.
Minimum value

=head4 Parameters

    $values


=head3 ne_string

Determine if string arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead if
you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_string

Determine if for string arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 normalize_space_string

Remove whitespace from the beginning and end of a string value.
Trims whitespace from beginning and end

=head4 Parameters

    $string


=head3 normalize_to_lower_case_string

Normalize string value to lower case and returns string result.
Converts to lower case

=head4 Parameters

    $string


=head3 one_and_only_string

This converts an array of string values that contains one value to a single
string value.
Converts a one value array to a string value

=head4 Parameters

    $array


=head3 repeat_string

Repeat a string value a specified number of times.
Returns a repeated value

=head4 Parameters

    $value

The string value to repeat.

    $times

The number of times to repeat the value.

=head3 replace_string

Replace matches in a string value, left to right, up to limit times, as
ECMAScript's s.replace(search, r) does. search is a string, which is literal
text, or a regexp, which is a pattern. An empty match is a match at a
code-point boundary. The default limit replaces once (an empty string inserts
at the start). Specify limit -1 to replace every match.
Replace text or regular expression matches

=head4 Parameters

    $value

The original string value.

    $search

What to replace: a string is literal text, and an empty string matches at each
code-point boundary, including the start and the end; a regexp is a pattern. A
regexp literal (/a+b/ or "a+b"re), to_regexp(s), or `compile<regexp>(r`) is
already compiled; any other regexp value is compiled for this call.

    $replacement

The replacement for each match. When search is a string, the replacement is
literal text. When search is a regexp, $n is the text of group n ($0 the whole
match), ${name} the text of a named group, and a backslash makes the next
character literal (\$ for a dollar sign); any other $ is an error.

    $limit

The maximum number of matches to replace. The default is 1. Specify -1 (or any
negative) to replace every match.

=head3 search_string

Searches string value for the regular expression and returns the code point
index of the first match, or -1 if there is none, as ECMAScript's s.search(re)
does. Use index_of to find literal text.
Code point index of the first match of a regular expression

=head4 Parameters

    $value

The string value to search.

    $regexp

The regular expression. A regexp literal (/a+b/ or "a+b"re), to_regexp(s), or
`compile<regexp>(r`) is already compiled; any other regexp value is compiled
for this call.

=head3 set_equals_string

Returns boolean true if string array1 and string array2 are subsets of each
other and return the boolean result.
Checks whether two arrays are subsets of each other

=head4 Parameters

    $array1


    $array2


=head3 split_string

Split string value into an array of strings, as ECMAScript's
s.split(separator) does. separator is a string, which is literal text, or a
regexp, which is a pattern. With a regexp, the text of each group of a match
is put in the array after the piece before it (undefined when the group did
not take part), and an empty match splits between code points but not at the
start or end of the value.
Split at a separator or regular expression

=head4 Parameters

    $value

The string value to split.

    $separator

Where to split: a string is literal text, a regexp is a pattern. If this is an
empty string or separator is not specified, the value is split between
characters. A regexp literal (/a+b/ or "a+b"re), to_regexp(s), or
`compile<regexp>(r`) is already compiled; any other regexp value is compiled
for this call.

    $limit

The maximum number of entries in the result. Any remaining part of value is
ignored.

=head3 starts_with_string

Checks whether string value starts with a subString and return the boolean
result.
Checks whether value starts with a string

=head4 Parameters

    $value


    $subString


=head3 string

Conversion function for string: convert one or more values of any data type to
string and return the concatenated result. With one argument this is
convert-to-string; with more arguments each is converted then concatenated in
order (no separator). A value with an undefined value is represented by
'undefined'.
Convert one or more values to string and concatenate

=head4 Parameters

    $values

Value to convert

=head3 subset_string

Returns boolean true if the unique values in string array1 are all in string
array2.
Determines if the first array is a subset of second array

=head4 Parameters

    $array1

The first array.

    $array2

The second array.

=head3 substring_string

Returns the string substring of value beginning at zero-based position integer
startIndex and ending at the position before integer endIndex. Specify -1 or
omit endIndex to return through the end of value. The result is always string
(a slice of anyURI is not an anyURI).
Extract a substring

=head4 Parameters

    $string


    $startIndex


    $endIndex


=head3 to_anyURI_string

Converts string value to anyURI. A string is read with the anyURI parser, the
same parser the typed literal "..."anyURI uses, and text the parser rejects is
an error.
Converts string to anyURI

=head4 Parameters

    $value


=head3 to_base64Binary_string

Converts string value to base64Binary. A string is read with the base64Binary
parser, the same parser the typed literal "..."base64Binary uses, and text the
parser rejects is an error.
Converts string to base64Binary

=head4 Parameters

    $value


=head3 to_dateTime_string

Converts string value to dateTime. A string is read with the dateTime parser,
the same parser the typed literal "..."dateTime uses, and text the parser
rejects is an error.
Converts string to dateTime

=head4 Parameters

    $value


=head3 to_date_string

Converts string value to date. A string is read with the date parser, the same
parser the typed literal "..."date uses, and text the parser rejects is an
error.
Converts string to date

=head4 Parameters

    $value


=head3 to_dayTimeDuration_string

Converts string value to dayTimeDuration. A string is read with the
dayTimeDuration parser, the same parser the typed literal "..."dayTimeDuration
uses, and text the parser rejects is an error.
Converts string to dayTimeDuration

=head4 Parameters

    $value


=head3 to_dnsName_string

Converts string value to dnsName. A string is read with the dnsName parser,
the same parser the typed literal "..."dnsName uses, and text the parser
rejects is an error.
Converts string to dnsName

=head4 Parameters

    $value


=head3 to_double_string

Converts string value to double. A string is read as double text, the same
text a double literal accepts, and other text is an error: integer text such
as "5" is not a double, so use to_double(to_integer(s)) for it.
Converts to double

=head4 Parameters

    $value


=head3 to_hexBinary_string

Converts string value to hexBinary. A string is read with the hexBinary
parser, the same parser the typed literal "..."hexBinary uses, and text the
parser rejects is an error.
Converts string to hexBinary

=head4 Parameters

    $value


=head3 to_ia5String_string

Converts string value to ia5String. A string is read with the ia5String
parser, the same parser the typed literal "..."ia5String uses, and text the
parser rejects is an error.
Converts string to ia5String

=head4 Parameters

    $value


=head3 to_integer_string

Converts string value to integer. A double is truncated toward zero (1.9 is 1
and -1.9 is -1); use round(), floor(), or ceil() first for another choice. A
double that is NaN, infinite, or outside the integer range is an error. A
string is read as integer text, and text that is not an integer ("1.5", " 1",
"1x") is an error.
Converts to integer

=head4 Parameters

    $value


=head3 to_ipAddress_string

Converts string value to ipAddress. A string is read with the ipAddress
parser, the same parser the typed literal "..."ipAddress uses, and text the
parser rejects is an error.
Converts string to ipAddress

=head4 Parameters

    $value


=head3 to_objectId_string

Converts string value to objectId. A string is read with the objectId parser,
and text the parser rejects is an error.
Converts string to objectId

=head4 Parameters

    $value


=head3 to_objectPath_string

Converts string value to objectPath. A string is read with the objectPath
parser, and text the parser rejects is an error.
Converts string to objectPath

=head4 Parameters

    $value


=head3 to_password_string

Converts string value to password. A string is read with the password parser,
the same parser the typed literal "..."password uses, and text the parser
rejects is an error.
Converts string to password

=head4 Parameters

    $value


=head3 to_regexp_string

Converts string value to a compiled regexp, the same value as a regexp
literal. A pattern ICU does not accept is a conversion_error.
Converts string to regexp

=head4 Parameters

    $value


=head3 to_rfc822Name_string

Converts string value to rfc822Name. A string is read with the rfc822Name
parser, the same parser the typed literal "..."rfc822Name uses, and text the
parser rejects is an error.
Converts string to rfc822Name

=head4 Parameters

    $value


=head3 to_string_string

Converts string value to string. For array values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A string value.

=head3 to_time_string

Converts string value to time. A string is read with the time parser, the same
parser the typed literal "..."time uses, and text the parser rejects is an
error.
Converts string to time

=head4 Parameters

    $value


=head3 to_x500Name_string

Converts string value to x500Name. A string is read with the x500Name parser,
the same parser the typed literal "..."x500Name uses, and text the parser
rejects is an error.
Converts string to x500Name

=head4 Parameters

    $value


=head3 to_yearMonthDuration_string

Converts string value to yearMonthDuration. A string is read with the
yearMonthDuration parser, the same parser the typed literal
"..."yearMonthDuration uses, and text the parser rejects is an error.
Converts string to yearMonthDuration

=head4 Parameters

    $value


=head3 union_string

Returns an array of string contains all of the unique values in two or more
array of string values.
Returns union of two or more string arrays

=head4 Parameters

    $arrays

Two or more arrays.

=head3 url_decode

URL decode a string.
URL decode

=head4 Parameters

    $encoded

String to URL decode.

=head3 url_encode_string

URL encode a string value and return the string result.
URI encode

=head4 Parameters

    $unencoded

The string value to URL encode.

=cut

sub add_string {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "add<string>");
    $request->set("values", $values);

    return $request->getResult();
}

sub array_string {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "array_string");
    $request->set("values", $values);

    return $request->getResult();
}

sub as_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "as_string");
    $request->set("value", $value);

    return $request->getResult();
}

sub at_least_one_member_of_string {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<string>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub concat {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "concat");
    $request->set("values", $values);

    return $request->getResult();
}

sub encode_as_base64Binary_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "encode_as_base64Binary<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub encode_as_hexBinary_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "encode_as_hexBinary<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub ends_with_string {
    my ($value, $subString) = @_;

    my $request = $session->request()

    $request->set("function" => "ends_with<string>");
    $request->set("value", $value);
    $request->set("subString", $subString);

    return $request->getResult();
}

sub eq_ignore_case_string {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq_ignore_case<string>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eq_string {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<string>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_string {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<string>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eval_string {
    my ($source, $additionalUntrustedQualifiedVariables) = @_;

    my $request = $session->request()

    $request->set("function" => "eval<string>");
    $request->set("source", $source);

    if (defined $additionalUntrustedQualifiedVariables)
        $request->set("additionalUntrustedQualifiedVariables", $additionalUntrustedQualifiedVariables);

    return $request->getResult();
}

sub ge_string {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<string>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_string {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<string>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub includes_string {
    my ($searchString, $subString, $position) = @_;

    my $request = $session->request()

    $request->set("function" => "includes<string>");
    $request->set("searchString", $searchString);
    $request->set("subString", $subString);

    if (defined $position)
        $request->set("position", $position);

    return $request->getResult();
}

sub index_of_string {
    my ($value, $subString, $startIndex) = @_;

    my $request = $session->request()

    $request->set("function" => "index_of<string>");
    $request->set("value", $value);
    $request->set("subString", $subString);

    if (defined $startIndex)
        $request->set("startIndex", $startIndex);

    return $request->getResult();
}

sub intersection_string {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<string>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub is_in_string {
    my ($value, $array) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<string>");
    $request->set("value", $value);
    $request->set("array", $array);

    return $request->getResult();
}

sub is_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is_string");
    $request->set("value", $value);

    return $request->getResult();
}

sub last_index_of_string {
    my ($value, $subString, $startIndex) = @_;

    my $request = $session->request()

    $request->set("function" => "last_index_of<string>");
    $request->set("value", $value);
    $request->set("subString", $subString);

    if (defined $startIndex)
        $request->set("startIndex", $startIndex);

    return $request->getResult();
}

sub le_string {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<string>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub length_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "length<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub lt_string {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<string>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub match_all_string {
    my ($value, $regexp, $limit) = @_;

    my $request = $session->request()

    $request->set("function" => "match_all<string>");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

    if (defined $limit)
        $request->set("limit", $limit);

    return $request->getResult();
}

sub match_string {
    my ($value, $regexp) = @_;

    my $request = $session->request()

    $request->set("function" => "match<string>");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

    return $request->getResult();
}

sub max_string {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "max<string>");
    $request->set("values", $values);

    return $request->getResult();
}

sub min_string {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "min<string>");
    $request->set("values", $values);

    return $request->getResult();
}

sub ne_string {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<string>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_string {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<string>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub normalize_space_string {
    my ($string) = @_;

    my $request = $session->request()

    $request->set("function" => "normalize_space<string>");
    $request->set("string", $string);

    return $request->getResult();
}

sub normalize_to_lower_case_string {
    my ($string) = @_;

    my $request = $session->request()

    $request->set("function" => "normalize_to_lower_case<string>");
    $request->set("string", $string);

    return $request->getResult();
}

sub one_and_only_string {
    my ($array) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<string>");
    $request->set("array", $array);

    return $request->getResult();
}

sub repeat_string {
    my ($value, $times) = @_;

    my $request = $session->request()

    $request->set("function" => "repeat<string>");
    $request->set("value", $value);
    $request->set("times", $times);

    return $request->getResult();
}

sub replace_string {
    my ($value, $search, $replacement, $limit) = @_;

    my $request = $session->request()

    $request->set("function" => "replace<string>");
    $request->set("value", $value);
    $request->set("search", $search);
    $request->set("replacement", $replacement);

    if (defined $limit)
        $request->set("limit", $limit);

    return $request->getResult();
}

sub search_string {
    my ($value, $regexp) = @_;

    my $request = $session->request()

    $request->set("function" => "search<string>");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

    return $request->getResult();
}

sub set_equals_string {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<string>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub split_string {
    my ($value, $separator, $limit) = @_;

    my $request = $session->request()

    $request->set("function" => "split<string>");
    $request->set("value", $value);

    if (defined $separator)
        $request->set("separator", $separator);

    if (defined $limit)
        $request->set("limit", $limit);

    return $request->getResult();
}

sub starts_with_string {
    my ($value, $subString) = @_;

    my $request = $session->request()

    $request->set("function" => "starts_with<string>");
    $request->set("value", $value);
    $request->set("subString", $subString);

    return $request->getResult();
}

sub string {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "string");
    $request->set("values", $values);

    return $request->getResult();
}

sub subset_string {
    my ($array1, $array2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<string>");
    $request->set("array1", $array1);
    $request->set("array2", $array2);

    return $request->getResult();
}

sub substring_string {
    my ($string, $startIndex, $endIndex) = @_;

    my $request = $session->request()

    $request->set("function" => "substring<string>");
    $request->set("string", $string);
    $request->set("startIndex", $startIndex);

    if (defined $endIndex)
        $request->set("endIndex", $endIndex);

    return $request->getResult();
}

sub to_anyURI_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_anyURI<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_base64Binary_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_base64Binary<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_dateTime_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_dateTime<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_date_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_date<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_dayTimeDuration_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_dayTimeDuration<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_dnsName_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_dnsName<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_double_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_double<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_hexBinary_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_hexBinary<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_ia5String_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_ia5String<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_integer_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_integer<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_ipAddress_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_ipAddress<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_objectId_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_objectId<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_objectPath_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_objectPath<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_password_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_password<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_regexp_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_regexp<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_rfc822Name_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_rfc822Name<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_string_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_time_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_time<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_x500Name_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_x500Name<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub to_yearMonthDuration_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_yearMonthDuration<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub union_string {
    my ($arrays) = @_;

    my $request = $session->request()

    $request->set("function" => "union<string>");
    $request->set("arrays", $arrays);

    return $request->getResult();
}

sub url_decode {
    my ($encoded) = @_;

    my $request = $session->request()

    $request->set("function" => "url_decode");
    $request->set("encoded", $encoded);

    return $request->getResult();
}

sub url_encode_string {
    my ($unencoded) = @_;

    my $request = $session->request()

    $request->set("function" => "url_encode<string>");
    $request->set("unencoded", $unencoded);

    return $request->getResult();
}

42;
