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

    $adaptorId

Id of adaptor.

    $key

Name of the property index to be created.

    $value

Expression to calculate the index value(s).

    $objectType

Object Type(s) this index may apply to.

    $filter

Expression to determine if this index applies to a particular object.

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

    $adaptorId

Id of adaptor.

    $objectType

Id of adaptive object type indexes

=head3 index_remove

Remove an index definition.
Remove index definition

=head4 Parameters

    $adaptorId

Id of adaptor.

    $key

The index key to be removed.

=cut

sub index_create {
    my ($adaptorId, $key, $value, $objectType, $filter, $options, $retroactive, $test) = @_;

    my $request = $session->request()

    $request->set("function" => "index_create");
    $request->set("key", $key);

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

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
    my ($adaptorId, $objectType) = @_;

    my $request = $session->request()

    $request->set("function" => "index_list");

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    if (defined $objectType)
        $request->set("objectType", $objectType);

    return $request->getResult();
}

sub index_remove {
    my ($adaptorId, $key) = @_;

    my $request = $session->request()

    $request->set("function" => "index_remove");
    $request->set("key", $key);

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    return $request->getResult();
}

42;
