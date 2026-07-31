#!/usr/bin/env perl

#**
# @file crypto.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the crypto category.
#*

package afw::crypto;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    crypto_decrypt 
    crypto_derive_key 
    crypto_destroy_key 
    crypto_digest 
    crypto_encrypt 
    crypto_export_key 
    crypto_generate_key 
    crypto_hmac 
    crypto_hmac_verify 
    crypto_import_key 
    crypto_seal 
    crypto_unseal 
    crypto_version_info 
);

=head1 NAME

afw::crypto - Perl module for crypto functions

=head2 Functions

The following functions are exported by default

=head3 crypto_decrypt

Decrypt AES-GCM ciphertext. algorithm must include name, iv, and tag (16
octets). additionalData must match encrypt if used. Requires execute access.
Auth failure yields a generic decryption error.
Decrypt (AES-GCM)

=head4 Parameters

    $algorithm

AES-GCM parameters including required iv and tag.

    $key

Key material, CryptoKey, or key reference.

    $data

Ciphertext without tag (base64Binary or hexBinary).

=head3 crypto_derive_key

Derive a key via PBKDF2-HMAC-SHA256 and store it in the keystore. algorithm
object requires name PBKDF2, salt (min 16 octets), length (output octets),
optional iterations (default 600000, min 100000), optional hash SHA-256.
baseKey may be polymorphic including utf8 encoding for passphrases. Requires
execute access.
Derive key (PBKDF2)

=head4 Parameters

    $algorithm

PBKDF2 parameters object.

    $baseKey

Passphrase or key material (polymorphic; utf8 encoding allowed).

    $usages

Default ["encrypt","decrypt"].

    $extractable

Default false.

=head3 crypto_destroy_key

Remove a key from the process keystore and cleanse its material. Requires
execute access.
Destroy key

=head4 Parameters

    $key

CryptoKey object with keyId.

=head3 crypto_digest

Compute a cryptographic digest of binary data. Algorithm must be SHA-256 or
SHA-512. Data may be base64Binary or hexBinary.
Cryptographic digest

=head4 Parameters

    $algorithm

Digest algorithm name: SHA-256 or SHA-512.

    $data

Data to hash (base64Binary or hexBinary).

=head3 crypto_encrypt

Encrypt binary plaintext with AES-GCM. algorithm is an object { name:
"AES-GCM", length?, iv?, additionalData? }. If iv is omitted, a 12-octet IV is
generated. Returns ciphertext, iv, and tag. Requires execute access.
Encrypt (AES-GCM)

=head4 Parameters

    $algorithm

AES-GCM parameters object.

    $key

Key material, CryptoKey, or key reference.

    $data

Plaintext (base64Binary or hexBinary).

=head3 crypto_export_key

Export raw key octets from a CryptoKey if extractable is true. Requires
execute access.
Export key

=head4 Parameters

    $key

CryptoKey object with keyId.

=head3 crypto_generate_key

Generate a random key in the process keystore. Algorithm string or object.
AES-GCM length defaults to 256 bits. Requires execute access.
Generate key

=head4 Parameters

    $algorithm

Algorithm string or object { name, length? }.

    $usages

Optional usages; defaults depend on algorithm family.

    $extractable

If true, crypto_export_key may export raw key. Default false.

=head3 crypto_hmac

Compute HMAC over binary data. Algorithm must be HMAC-SHA-256 or HMAC-SHA-512.
Key is polymorphic (CryptoKey, raw binary, or reference object). Requires
execute access.
HMAC

=head4 Parameters

    $algorithm

HMAC algorithm: HMAC-SHA-256 or HMAC-SHA-512.

    $key

Key material, CryptoKey object, or key reference { from, ... }.

    $data

Data to MAC (base64Binary or hexBinary).

=head3 crypto_hmac_verify

Verify an HMAC in constant time. Algorithm must be HMAC-SHA-256 or
HMAC-SHA-512. Requires execute access.
Verify HMAC

=head4 Parameters

    $algorithm

HMAC algorithm: HMAC-SHA-256 or HMAC-SHA-512.

    $key

Key material, CryptoKey object, or key reference.

    $data

Data that was MAC'd (base64Binary or hexBinary).

    $mac

Expected MAC (base64Binary or hexBinary).

=head3 crypto_import_key

Import key material (or a key reference resolved to octets) into the process
keystore and return a CryptoKey handle. Algorithm is a string registry name or
object with name (and optional length for AES-GCM). Default extractable is
false. Requires execute access.
Import key into keystore

=head4 Parameters

    $keySource

Raw binary key, or reference object { from: environment|file|material, ... }.

    $algorithm

Algorithm string or object { name, length? }.

    $usages

Optional usages; defaults depend on algorithm family.

    $extractable

If true, crypto_export_key may export raw key. Default false.

=head3 crypto_seal

Convenience for AES-GCM encryption: generates an IV, encrypts data, and
returns a sealed object { algorithm, keyLength, iv, tag, ciphertext }.
Equivalent to crypto_encrypt({ name: "AES-GCM" }, key, data) with an
auto-generated IV. Use stringify() (and optional pure-JSON field mapping) to
store the result. Requires execute access.
Seal binary data (AES-GCM)

=head4 Parameters

    $key

Key material, CryptoKey, or key reference.

    $data

Plaintext (base64Binary or hexBinary). Use encode_as_base64Binary() for UTF-8
text.

=head3 crypto_unseal

Decrypt a sealed value from crypto_seal / crypto_encrypt. sealed may be: (1)
an object with iv, tag, and ciphertext as base64Binary/hexBinary or as
base64/hex strings; (2) a string of pure JSON with those properties as base64
strings (e.g. after stringify of a JSON-friendly bag). Returns plaintext
octets. Requires execute access.
Unseal AES-GCM sealed data

=head4 Parameters

    $key

Key material, CryptoKey, or key reference.

    $sealed

Sealed object or pure JSON string.

=head3 crypto_version_info

Returns runtime OpenSSL and afw_crypto version information and the list of
supported algorithm names.
Crypto extension version info

=head4 Parameters

=cut

sub crypto_decrypt {
    my ($algorithm, $key, $data) = @_;

    my $request = $session->request()

    $request->set("function" => "crypto_decrypt");
    $request->set("algorithm", $algorithm);
    $request->set("key", $key);
    $request->set("data", $data);

    return $request->getResult();
}

sub crypto_derive_key {
    my ($algorithm, $baseKey, $usages, $extractable) = @_;

    my $request = $session->request()

    $request->set("function" => "crypto_derive_key");
    $request->set("algorithm", $algorithm);
    $request->set("baseKey", $baseKey);

    if (defined $usages)
        $request->set("usages", $usages);

    if (defined $extractable)
        $request->set("extractable", $extractable);

    return $request->getResult();
}

sub crypto_destroy_key {
    my ($key) = @_;

    my $request = $session->request()

    $request->set("function" => "crypto_destroy_key");
    $request->set("key", $key);

    return $request->getResult();
}

sub crypto_digest {
    my ($algorithm, $data) = @_;

    my $request = $session->request()

    $request->set("function" => "crypto_digest");
    $request->set("algorithm", $algorithm);
    $request->set("data", $data);

    return $request->getResult();
}

sub crypto_encrypt {
    my ($algorithm, $key, $data) = @_;

    my $request = $session->request()

    $request->set("function" => "crypto_encrypt");
    $request->set("algorithm", $algorithm);
    $request->set("key", $key);
    $request->set("data", $data);

    return $request->getResult();
}

sub crypto_export_key {
    my ($key) = @_;

    my $request = $session->request()

    $request->set("function" => "crypto_export_key");
    $request->set("key", $key);

    return $request->getResult();
}

sub crypto_generate_key {
    my ($algorithm, $usages, $extractable) = @_;

    my $request = $session->request()

    $request->set("function" => "crypto_generate_key");
    $request->set("algorithm", $algorithm);

    if (defined $usages)
        $request->set("usages", $usages);

    if (defined $extractable)
        $request->set("extractable", $extractable);

    return $request->getResult();
}

sub crypto_hmac {
    my ($algorithm, $key, $data) = @_;

    my $request = $session->request()

    $request->set("function" => "crypto_hmac");
    $request->set("algorithm", $algorithm);
    $request->set("key", $key);
    $request->set("data", $data);

    return $request->getResult();
}

sub crypto_hmac_verify {
    my ($algorithm, $key, $data, $mac) = @_;

    my $request = $session->request()

    $request->set("function" => "crypto_hmac_verify");
    $request->set("algorithm", $algorithm);
    $request->set("key", $key);
    $request->set("data", $data);
    $request->set("mac", $mac);

    return $request->getResult();
}

sub crypto_import_key {
    my ($keySource, $algorithm, $usages, $extractable) = @_;

    my $request = $session->request()

    $request->set("function" => "crypto_import_key");
    $request->set("keySource", $keySource);
    $request->set("algorithm", $algorithm);

    if (defined $usages)
        $request->set("usages", $usages);

    if (defined $extractable)
        $request->set("extractable", $extractable);

    return $request->getResult();
}

sub crypto_seal {
    my ($key, $data) = @_;

    my $request = $session->request()

    $request->set("function" => "crypto_seal");
    $request->set("key", $key);
    $request->set("data", $data);

    return $request->getResult();
}

sub crypto_unseal {
    my ($key, $sealed) = @_;

    my $request = $session->request()

    $request->set("function" => "crypto_unseal");
    $request->set("key", $key);
    $request->set("sealed", $sealed);

    return $request->getResult();
}

sub crypto_version_info {
    my () = @_;

    my $request = $session->request()

    $request->set("function" => "crypto_version_info");

    return $request->getResult();
}

42;
