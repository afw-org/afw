#!/usr/bin/env perl

#**
# @file random.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the random category.
#*

package afw::random;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    random_base64Binary 
    random_digits 
    random_hexBinary 
    random_integer 
    random_number 
);

=head1 NAME

afw::random - Perl module for random functions

=head2 Functions

The following functions are exported by default

=head3 random_base64Binary

This returns a specified number of random octets as dataType base64Binary.
Generate random octets (base64Binary)

=head4 Parameters

    $numberOfOctets

The number of random octets to generate.

=head3 random_digits

Generate a string containing a specified number of random decimal digits.
Generate random decimal digits

=head4 Parameters

    $numberOfDigits

The number (1-18) of digits to generate.

=head3 random_hexBinary

This returns a specified number of random octets as dataType hexBinary.
Generate random octets (hexBinary)

=head4 Parameters

    $numberOfOctets

The number of random octets to generate.

=head3 random_integer

This returns a random integer between specified values inclusive.
Generate a random integer between specified values

=head4 Parameters

    $min

Minimum integer inclusive.

    $max

Maximum integer inclusive.

=head3 random_number

This returns a random double between specified values
Generate a random number between specified values

=head4 Parameters

    $min

Minimum double inclusive. The default is 0.0.

    $max

Maximum double exclusive. The default is 1.0.

=cut

sub random_base64Binary {
    my ($numberOfOctets) = @_;

    my $request = $session->request()

    $request->set("function" => "random_base64Binary");
    $request->set("numberOfOctets", $numberOfOctets);

    return $request->getResult();
}

sub random_digits {
    my ($numberOfDigits) = @_;

    my $request = $session->request()

    $request->set("function" => "random_digits");
    $request->set("numberOfDigits", $numberOfDigits);

    return $request->getResult();
}

sub random_hexBinary {
    my ($numberOfOctets) = @_;

    my $request = $session->request()

    $request->set("function" => "random_hexBinary");
    $request->set("numberOfOctets", $numberOfOctets);

    return $request->getResult();
}

sub random_integer {
    my ($min, $max) = @_;

    my $request = $session->request()

    $request->set("function" => "random_integer");
    $request->set("min", $min);
    $request->set("max", $max);

    return $request->getResult();
}

sub random_number {
    my ($min, $max) = @_;

    my $request = $session->request()

    $request->set("function" => "random_number");

    if (defined $min)
        $request->set("min", $min);

    if (defined $max)
        $request->set("max", $max);

    return $request->getResult();
}

42;
