#!/usr/bin/env perl

#**
# @file lmdb.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the lmdb category.
#*

package afw::lmdb;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    reader_check 
    reader_list 
);

=head1 NAME

afw::lmdb - Perl module for lmdb functions

=head2 Functions

The following functions are exported by default

=head3 reader_check

Check for stale entries in the reader lock table.


=head4 Parameters

    $adaptorId

Adaptor Id.

=head3 reader_list

Dump the entries in the reader lock table.


=head4 Parameters

    $adaptorId

Adaptor Id.

=cut

sub reader_check {
    my ($adaptorId) = @_;

    my $request = $session->request()

    $request->set("function" => "reader_check");

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    return $request->getResult();
}

sub reader_list {
    my ($adaptorId) = @_;

    my $request = $session->request()

    $request->set("function" => "reader_list");

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    return $request->getResult();
}

42;
