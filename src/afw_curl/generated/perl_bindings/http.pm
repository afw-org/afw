#!/usr/bin/env perl

#**
# @file http.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the http category.
#*

package afw::http;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    http_get 
    http_post 
);

=head1 NAME

afw::http - Perl module for http functions

=head2 Functions

The following functions are exported by default

=head3 http_get

Makes a HTTP GET Request.


=head4 Parameters

    $url

HTTP Url.

    $headers

HTTP Headers.

    $options

CURL Options

=head3 http_post

Makes a HTTP Post Request.


=head4 Parameters

    $url

HTTP Url.

    $payload

Data payload to POST to the url.

    $headers

HTTP Headers.

    $options

CURL Options

=cut

sub http_get {
    my ($url, $headers, $options) = @_;

    my $request = $session->request()

    $request->set("function" => "http_get");
    $request->set("url", $url);

    if (defined $headers)
        $request->set("headers", $headers);

    if (defined $options)
        $request->set("options", $options);

    return $request->getResult();
}

sub http_post {
    my ($url, $payload, $headers, $options) = @_;

    my $request = $session->request()

    $request->set("function" => "http_post");
    $request->set("url", $url);

    if (defined $payload)
        $request->set("payload", $payload);

    if (defined $headers)
        $request->set("headers", $headers);

    if (defined $options)
        $request->set("options", $options);

    return $request->getResult();
}

42;
