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
    pool_bytes_in_use 
    process_rss 
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

Return true if the value is not undefined. Does not check whether a variable
name is bound — use variable_exists for that. null is defined.
True if value is not undefined

=head4 Parameters

    $value

Value to check.

=head3 is_nullish

Return true if the value is null or undefined. Does not check whether a
variable name is bound — use variable_exists for that.
True if value is null or undefined

=head4 Parameters

    $value

Value to check.

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

=head3 pool_bytes_in_use

Return the environment running total of outstanding bytes AFW malloc/calloc
asked for and has not yet given back (heap/tracker free, or pool destroy).
Rounding and prefixes included. Not APR's private usage; see process_rss() for
current process RSS.
Outstanding AFW pool malloc/calloc bytes

=head4 Parameters

=head3 process_rss

Return the current resident set size of this process in kilobytes (Linux
/proc/self/statm). A hint at APR and OS usage; APR does not return pages to
the OS. Compare with pool_bytes_in_use() for AFW asked-for vs process RSS.
Current process RSS in kilobytes

=head4 Parameters

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

Return true if the named variable is bound: a lexical symbol in the current
scope chain, or a name defined on a visible qualifier frame. Still true when
the value is undefined (including an uninitialized let) or null. False only
when the name is not bound. Use is_defined / is_nullish for the value.
True if a variable name is bound

=head4 Parameters

    $name

Name of variable to check. Optionally qualifier::name.

=head3 variable_get

Return the value of a bound variable. Optional default applies only when the
name is not bound — not when the value is undefined. If unbound and no default
is given, the result is undefined. The default is the evaluated value at that
moment (identity).
Get a bound variable value

=head4 Parameters

    $name

Name of variable to get. Optionally qualifier::name.

    $defaultValue

Value to return only if the name is not bound. The evaluated value at that
moment (identity).

=head3 variable_is_not_null

Return true if the named variable is bound and its value is not Adaptive null.
Undefined (including an uninitialized let) counts as not null. False if the
name is not bound or the value is null. This is not the same as is_defined or
not is_nullish.
True if bound and not Adaptive null

=head4 Parameters

    $name

Name of variable to check. Optionally qualifier::name.

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

sub pool_bytes_in_use {
    my () = @_;

    my $request = $session->request()

    $request->set("function" => "pool_bytes_in_use");

    return $request->getResult();
}

sub process_rss {
    my () = @_;

    my $request = $session->request()

    $request->set("function" => "process_rss");

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
