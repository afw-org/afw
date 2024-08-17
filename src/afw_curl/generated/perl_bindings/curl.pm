#!/usr/bin/env perl

#**
# @file curl.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the curl category.
#*

package afw::curl;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    curl_version_info 
);

=head1 NAME

afw::curl - Perl module for curl functions

=head2 Functions

The following functions are exported by default

=head3 curl_version_info

Returns run-time libcurl version info.


=head4 Parameters

=cut

sub curl_version_info {
    my () = @_;

    my $request = $session->request()

    $request->set("function" => "curl_version_info");

    return $request->getResult();
}

42;
