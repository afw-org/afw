package afw::string;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    add_string 
    at_least_one_member_of_string 
    bag_size_string 
    bag_string 
    concat 
    encode_as_base64Binary_string 
    encode_as_hexBinary_string 
    ends_with_string 
    eq_ignore_case_string 
    eq_string 
    eqx_string 
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
    max_string 
    min_string 
    ne_string 
    nex_string 
    normalize_space_string 
    normalize_to_lower_case_string 
    one_and_only_string 
    regexp_index_of_string 
    regexp_match_string 
    regexp_replace_string 
    repeat_string 
    replace_string 
    set_equals_string 
    split_string 
    starts_with_string 
    string 
    subset_string 
    substring_string 
    to_anyURI_string 
    to_boolean_string 
    to_dateTime_string 
    to_date_string 
    to_dayTimeDuration_string 
    to_dnsName_string 
    to_double_string 
    to_integer_string 
    to_ipAddress_string 
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

Add (concatenate) a string with 1 or more values of any data type converted
to their string value and return the string result.
Add (concatenate) strings

=head4 Parameters

    $string


    $values


=head3 at_least_one_member_of_string

Returns boolean true if at least one value in string list1 is in string
list2.
Checks for at least one value in common

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 bag_size_string

This returns the integer number of values in list.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 bag_string

Takes any number of string values and returns a list of list.
Makes a list from values

=head4 Parameters

    $values


=head3 concat

Convert two or more values of any data type to string and return the
concatenated result. A value with an undefined value is represented by
`<undefined>`.
Convert two or more values to string and concatenate

=head4 Parameters

    $values

Value to convert

=head3 encode_as_base64Binary_string

Encode a value as a base64Binary. The effect is to create a base64Binary
value with an internal value of the value passed.
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
type of arg1 then return the boolean result. Use "eqx" ("===") instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_string

Determine if for string arg1 is equal to the value and data type of arg2 then
return the boolean result. Use "eq" ("==") instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


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

Returns a list of string with the values that are common to both list of
string list1 and list2.
Returns intersection of two lists

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 is_in_string

Checks whether string value is in list of string list and returns the boolean
result.
Checks whether a value is in a list

=head4 Parameters

    $value


    $list


=head3 is_string

Checks whether value is dataType string and return the boolean result.
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

This is a polymorphic function where string can be any of the supported data
types. Return the integer number of entries in datatype list or codepoints in
others.
Returns number of codepoints or entries in value

=head4 Parameters

    $value

Returns the number of entries in a list or code points in others.

=head3 lt_string

Checks for string arg1 is less that string arg2 and return the boolean
result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


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
data type of arg1 then return the boolean result. Use "nex" ("!==") instead
if you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_string

Determine if for string arg1 is not equal to the value or data type of arg2
then return the boolean result. Use "ne" ("!=") instead if you want arg2 to
be converted to the data type of arg1 before comparison.
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

This converts a list of string values that contains one value to a single
string value.
Converts a one value list to a string value

=head4 Parameters

    $list


=head3 regexp_index_of_string

Search string value for a regular expression and return index. If not found,
-1 is returned.
Returns index of first match of regular expression

=head4 Parameters

    $value

The string value to search.

    $regexp

A regular expression to use for search.

=head3 regexp_match_string

Checks whether string value matches the regular expression regexp and return
the boolean result.
Search for a match using a regular expression

=head4 Parameters

    $value


    $regexp


=head3 regexp_replace_string

Replace matched values for a regular expression in a string value.
Replace strings matching a regular expression

=head4 Parameters

    $value

The original string value.

    $regexp

A regular expression to use for search.

    $replacement

The replacement string.

    $limit

This is the maximum times to replace. The default is 1. Specify -1 to replace
all occurrences.

=head3 repeat_string

Repeat a string value a specified number of times.
Returns a repeated value

=head4 Parameters

    $value

The string value to repeat.

    $times

The number of times to repeat the value.

=head3 replace_string

Replace string(s) in a string value.
Replace strings

=head4 Parameters

    $value

The original string value.

    $match

The string to replace.

    $replacement

The replacement string.

    $limit

This is the maximum times to replace. The default is 1. Specify -1 to replace
all occurrences.

=head3 set_equals_string

Returns boolean true if string list1 and string list2 are subsets of each
other and return the boolean result.
Checks whether two lists are subsets of each other

=head4 Parameters

    $list1


    $list2


=head3 split_string

Split string value into an list of strings using a separator.
Split at a separator

=head4 Parameters

    $value

The string value to split.

    $separator

The separator to use. If this is an empty string or separator is not
specified, the value is split between characters.

    $limit

This is the maximum number of splits. Any remaining part of value is ignored.

=head3 starts_with_string

Checks whether string value starts with a subString and return the boolean
result.
Checks whether value starts with a string

=head4 Parameters

    $value


    $subString


=head3 string

Convert one or more values of any data type to string and return the
concatenated result. A value with an undefined value is represented by
`<undefined>`.
Convert one or more values to string and concatenate

=head4 Parameters

    $values

Value to convert

=head3 subset_string

Returns boolean true if the unique values in string list1 are all in string
list2.
Determines if the first list is a subset of second list

=head4 Parameters

    $list1

The first list.

    $list2

The second list.

=head3 substring_string

Returns the string substring of value beginning at zero-based position
integer startIndex and ending at the position before integer endIndex.
Specify -1 or omitting endIndex to return up to end of string.
Extract a substring

=head4 Parameters

    $string


    $startIndex


    $endIndex


=head3 to_anyURI_string

Converts string value to anyURI.
Converts string to anyURI

=head4 Parameters

    $value


=head3 to_boolean_string

Converts string value to boolean.
Converts string to boolean

=head4 Parameters

    $value


=head3 to_dateTime_string

Converts string value to dateTime.
Converts string to dateTime

=head4 Parameters

    $value


=head3 to_date_string

Converts string value to date.
Converts string to date

=head4 Parameters

    $value


=head3 to_dayTimeDuration_string

Converts string value to dayTimeDuration.
Converts string to dayTimeDuration

=head4 Parameters

    $value


=head3 to_dnsName_string

Converts string value to dnsName.
Converts string to dnsName

=head4 Parameters

    $value


=head3 to_double_string

Converts string value to double and returns double result.
Converts to double

=head4 Parameters

    $value


=head3 to_integer_string

Truncate string value to a whole number and returns integer result.
Converts to integer

=head4 Parameters

    $value


=head3 to_ipAddress_string

Converts string value to ipAddress.
Converts string to ipAddress

=head4 Parameters

    $value


=head3 to_rfc822Name_string

Converts string value to rfc822Name.
Converts string to rfc822Name

=head4 Parameters

    $value


=head3 to_string_string

Converts string value to string. For list values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A string value.

=head3 to_time_string

Converts string value to time.
Converts string to time

=head4 Parameters

    $value


=head3 to_x500Name_string

Converts string value to x500Name.
Converts string to x500Name

=head4 Parameters

    $value


=head3 to_yearMonthDuration_string

Converts string value to yearMonthDuration.
Converts string to yearMonthDuration

=head4 Parameters

    $value


=head3 union_string

Returns a list of string contains all of the unique values in two or more
list of string values.
Returns union of two or more string lists

=head4 Parameters

    $lists

Two or more lists.

=head3 url_decode

URL decode a value or bag of values.
URL decode

=head4 Parameters

    $encoded

URL decode a single string or a bag of string.

=head3 url_encode_string

URL encode a value or bag of values.
URI encode

=head4 Parameters

    $unencoded

URL encode a single value. See the url_encode method for the data type of
more details.

=cut

sub add_string {
    my ($string, $values) = @_;

    my $request = $session->request()

    $request->set("function" => "add<string>");
    $request->set("string", $string);
    $request->set("values", $values);

    return $request->getResult();
}

sub at_least_one_member_of_string {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "at_least_one_member_of<string>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub bag_size_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<string>");
    $request->set("value", $value);

    return $request->getResult();
}

sub bag_string {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<string>");

    if (defined $values)
        $request->set("values", $values);

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
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "intersection<string>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

    return $request->getResult();
}

sub is_in_string {
    my ($value, $list) = @_;

    my $request = $session->request()

    $request->set("function" => "is_in<string>");
    $request->set("value", $value);
    $request->set("list", $list);

    return $request->getResult();
}

sub is_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<string>");
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
    my ($list) = @_;

    my $request = $session->request()

    $request->set("function" => "one_and_only<string>");
    $request->set("list", $list);

    return $request->getResult();
}

sub regexp_index_of_string {
    my ($value, $regexp) = @_;

    my $request = $session->request()

    $request->set("function" => "regexp_index_of<string>");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

    return $request->getResult();
}

sub regexp_match_string {
    my ($value, $regexp) = @_;

    my $request = $session->request()

    $request->set("function" => "regexp_match<string>");
    $request->set("value", $value);
    $request->set("regexp", $regexp);

    return $request->getResult();
}

sub regexp_replace_string {
    my ($value, $regexp, $replacement, $limit) = @_;

    my $request = $session->request()

    $request->set("function" => "regexp_replace<string>");
    $request->set("value", $value);
    $request->set("regexp", $regexp);
    $request->set("replacement", $replacement);

    if (defined $limit)
        $request->set("limit", $limit);

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
    my ($value, $match, $replacement, $limit) = @_;

    my $request = $session->request()

    $request->set("function" => "replace<string>");
    $request->set("value", $value);
    $request->set("match", $match);
    $request->set("replacement", $replacement);

    if (defined $limit)
        $request->set("limit", $limit);

    return $request->getResult();
}

sub set_equals_string {
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "set_equals<string>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

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
    my ($list1, $list2) = @_;

    my $request = $session->request()

    $request->set("function" => "subset<string>");
    $request->set("list1", $list1);
    $request->set("list2", $list2);

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

sub to_boolean_string {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_boolean<string>");
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
    my ($lists) = @_;

    my $request = $session->request()

    $request->set("function" => "union<string>");
    $request->set("lists", $lists);

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
