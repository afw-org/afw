#!/usr/bin/env perl

#**
# @file smtp.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the smtp category.
#*

package afw::smtp;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    smtp_send 
);

=head1 NAME

afw::smtp - Perl module for smtp functions

=head2 Functions

The following functions are exported by default

=head3 smtp_send

Sends an eMail using SMTP protocol with cURL.


=head4 Parameters

    $url

SMTP Server Url.

    $mail_from

From Address.

    $mail_recipients

To Addresses.

    $payload

Email Payload, or body of the message.

    $options

CURL Options

=cut

sub smtp_send {
    my ($url, $mail_from, $mail_recipients, $payload, $options) = @_;

    my $request = $session->request()

    $request->set("function" => "smtp_send");
    $request->set("url", $url);
    $request->set("mail_from", $mail_from);
    $request->set("mail_recipients", $mail_recipients);
    $request->set("payload", $payload);

    if (defined $options)
        $request->set("options", $options);

    return $request->getResult();
}

42;
