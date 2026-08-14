#!/usr/bin/env perl

#**
# @file indexes.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the indexes category.
#*

package afw::indexes;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    index_create 
    index_list 
    index_remove 
);

=head1 NAME

afw::indexes - Perl module for indexes functions

=head2 Functions

The following functions are exported by default

=head3 index_create

Create an index definition.
Create index definition

=head4 Parameters

    $adapterId

Id of adapter.

    $key

Name of the property index to be created.

    $value

Adaptive script (expression-like; must return a value) used to calculate the
index value(s). While evaluating, current::object, current::objectId,
current::objectType, and current::key are available (issue #54). If omitted,
the property named by key is indexed.

    $objectType

Object type id(s) this index may apply to.

    $filter

Adaptive script that must return a boolean to decide whether this index
applies to a particular object. Uses the same current:: variables as value
(issue #54). If omitted, the filter is always true.

    $options

Indexing options.

    $retroactive

Retroactively generate indexes for existing objects.

    $test

Test create (don't actually perform).

=head3 index_list

List property indexes
List property indexes

=head4 Parameters

    $adapterId

Id of adapter.

    $objectType

Id of adaptive object type indexes

=head3 index_remove

Remove an index definition.
Remove index definition

=head4 Parameters

    $adapterId

Id of adapter.

    $key

The index key to be removed.

=cut

sub index_create {
    my ($adapterId, $key, $value, $objectType, $filter, $options, $retroactive, $test) = @_;

    my $request = $session->request()

    $request->set("function" => "index_create");
    $request->set("key", $key);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    if (defined $value)
        $request->set("value", $value);

    if (defined $objectType)
        $request->set("objectType", $objectType);

    if (defined $filter)
        $request->set("filter", $filter);

    if (defined $options)
        $request->set("options", $options);

    if (defined $retroactive)
        $request->set("retroactive", $retroactive);

    if (defined $test)
        $request->set("test", $test);

    return $request->getResult();
}

sub index_list {
    my ($adapterId, $objectType) = @_;

    my $request = $session->request()

    $request->set("function" => "index_list");

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    if (defined $objectType)
        $request->set("objectType", $objectType);

    return $request->getResult();
}

sub index_remove {
    my ($adapterId, $key) = @_;

    my $request = $session->request()

    $request->set("function" => "index_remove");
    $request->set("key", $key);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    return $request->getResult();
}

42;
