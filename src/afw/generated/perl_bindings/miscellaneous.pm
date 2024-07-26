#!/usr/bin/env perl

#**
# @file miscellaneous.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the miscellaneous category.
#*

package afw::miscellaneous;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    compare_uri 
    debug 
    execution_start_time_local 
    execution_start_time_utc 
    generate_uuid 
    is_defined 
    is_nullish 
    log 
    now_local 
    now_utc 
    parse_uri 
    perform 
    trace 
    variable_exists 
    variable_get 
    variable_is_not_null 
);

=head1 NAME

afw::miscellaneous - Perl module for miscellaneous functions

=head2 Functions

The following functions are exported by default

=head3 compare_uri

Compare two URIs.
Compare URIs

=head4 Parameters

    $uri1

First URI for compare. This URI can not contain an asterisk ('*')

    $uri2

Second URI for compare. This URI can contain asterisk ('*') for substitution
if isValuePath is true and currentPath2 is specified.

    $isValuePath

The URIs are adaptive value paths. If one of the URIs begins with a single
slash '/', both must, and each URI will be parsed as an adaptive value path
(example: /adapterId/objectType/objectId.propertyNames).

    $currentPath

If isValuePath is true, this is the current path that is used to resolve a
relative path in the URIs. If isValuePath is not true, this parameter is
ignored.

=head3 debug

Conditionally, based on the detail parameter, write a value as a string to the
debug file descriptor (usually stderr).
Conditionally write a debug value

=head4 Parameters

    $value

This is the value that will be converted to its string representation and
written. An undefined value is represented by 'undefined'.

    $detail

If true, the string will only written if the debug:function_active:detail flag
is on. If false or not specified, the string will only written if the
debug:function_active flag is on.

=head3 execution_start_time_local

Return local dateTime when the execution context was created. This will
usually be the start of request time.
The execution context (xctx) local create dataTime

=head4 Parameters

=head3 execution_start_time_utc

Return UTC dateTime when the execution context was created. This will usually
be the start of request time.
The execution context (xctx) UTC create dataTime

=head4 Parameters

=head3 generate_uuid

Generate a UUID.
Generate UUID

=head4 Parameters

=head3 is_defined

Test value returning boolean True if it is not undefined.
Is defined

=head4 Parameters

    $value

Value to check

=head3 is_nullish

Test value returning boolean True if it is null or undefined.
Is nullish

=head4 Parameters

    $value

Value to check

=head3 log

Evaluate and convert value to String and log it.
Log value

=head4 Parameters

    $value

Value to log.

=head3 now_local

Return current local dateTime.
Local dataTime

=head4 Parameters

=head3 now_utc

Return current UTC dateTime.
UTC dataTime

=head4 Parameters

=head3 parse_uri

Parse a URI.
Parse URI

=head4 Parameters

    $uri

URI to parse

    $isValuePath

The URI is an adaptive value path. If the path begins with a single slash '/',
the URI will be parsed as an adaptive value path (example:
/adapterId/objectType/objectId.propertyNames)

    $currentPath

If isValuePath is true, this is the current path that is used to resolve
relative paths. If isValuePath is not true, this parameter is ignored.

=head3 perform

Perform actions right away.
Perform actions

=head4 Parameters

    $request

See /afw/_AdaptiveObjectType_/_AdaptiveActions_ for more information.

=head3 trace

Write a value to a trace log.
Write a value to the a trace log

=head4 Parameters

    $value

This is the value that will be converted to its string representation and
written the trace log. An undefined value is represented by 'undefined'.

    $filter

If this optional filter is false, nothing will be written to the trace log.
The default is true.

    $number

This is an optional number between 1 and 8 that is appended to 'trace' to
identify the trace log. The default is 1.

=head3 variable_exists

Return the true if the named variable exists.
Determine if a variable exists

=head4 Parameters

    $name

Name of variable to check. The name can optionally be preceded with a
qualifier followed by '::'.

=head3 variable_get

Return the value of a variable. If variable is not available, return a default
or null value.
Get a variable value

=head4 Parameters

    $name

Name of variable to get. The name can optionally be preceded with a qualifier
followed by '::'.

    $defaultValue

The default value of variable if it does not exist in object. If not
specified, null value is the default.

=head3 variable_is_not_null

Return the true if the named variable exists and is not null.
Determine if a variable exists and is not null

=head4 Parameters

    $name

Name of variable to check. The name can optionally be preceded with a
qualifier followed by '::'.

=cut

sub compare_uri {
    my ($uri1, $uri2, $isValuePath, $currentPath) = @_;

    my $request = $session->request()

    $request->set("function" => "compare_uri");
    $request->set("uri1", $uri1);
    $request->set("uri2", $uri2);

    if (defined $isValuePath)
        $request->set("isValuePath", $isValuePath);

    if (defined $currentPath)
        $request->set("currentPath", $currentPath);

    return $request->getResult();
}

sub debug {
    my ($value, $detail) = @_;

    my $request = $session->request()

    $request->set("function" => "debug");
    $request->set("value", $value);

    if (defined $detail)
        $request->set("detail", $detail);

    return $request->getResult();
}

sub execution_start_time_local {
    my () = @_;

    my $request = $session->request()

    $request->set("function" => "execution_start_time_local");

    return $request->getResult();
}

sub execution_start_time_utc {
    my () = @_;

    my $request = $session->request()

    $request->set("function" => "execution_start_time_utc");

    return $request->getResult();
}

sub generate_uuid {
    my () = @_;

    my $request = $session->request()

    $request->set("function" => "generate_uuid");

    return $request->getResult();
}

sub is_defined {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is_defined");
    $request->set("value", $value);

    return $request->getResult();
}

sub is_nullish {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is_nullish");
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

sub now_local {
    my () = @_;

    my $request = $session->request()

    $request->set("function" => "now_local");

    return $request->getResult();
}

sub now_utc {
    my () = @_;

    my $request = $session->request()

    $request->set("function" => "now_utc");

    return $request->getResult();
}

sub parse_uri {
    my ($uri, $isValuePath, $currentPath) = @_;

    my $request = $session->request()

    $request->set("function" => "parse_uri");
    $request->set("uri", $uri);

    if (defined $isValuePath)
        $request->set("isValuePath", $isValuePath);

    if (defined $currentPath)
        $request->set("currentPath", $currentPath);

    return $request->getResult();
}

sub perform {
    my ($request) = @_;

    my $request = $session->request()

    $request->set("function" => "perform");
    $request->set("request", $request);

    return $request->getResult();
}

sub trace {
    my ($value, $filter, $number) = @_;

    my $request = $session->request()

    $request->set("function" => "trace");
    $request->set("value", $value);

    if (defined $filter)
        $request->set("filter", $filter);

    if (defined $number)
        $request->set("number", $number);

    return $request->getResult();
}

sub variable_exists {
    my ($name) = @_;

    my $request = $session->request()

    $request->set("function" => "variable_exists");
    $request->set("name", $name);

    return $request->getResult();
}

sub variable_get {
    my ($name, $defaultValue) = @_;

    my $request = $session->request()

    $request->set("function" => "variable_get");
    $request->set("name", $name);

    if (defined $defaultValue)
        $request->set("defaultValue", $defaultValue);

    return $request->getResult();
}

sub variable_is_not_null {
    my ($name) = @_;

    my $request = $session->request()

    $request->set("function" => "variable_is_not_null");
    $request->set("name", $name);

    return $request->getResult();
}

42;
