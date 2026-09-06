<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for crypto
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file crypto.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category crypto
 */
class crypto
{
    private $session;

    /**
     * Constructs the crypto class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * crypto_decrypt()
     *
     * Decrypt AES-GCM ciphertext. algorithm must include name, iv, and tag
     * (16 octets). additionalData must match encrypt if used. Requires
     * execute access. Auth failure yields a generic decryption error.
     *
     * @param object $algorithm AES-GCM parameters including required iv and
     *                          tag.
     * @param  $key Key material, CryptoKey, or key reference.
     * @param  $data Ciphertext without tag (base64Binary or hexBinary).
     *
     * @return base64Binary Plaintext octets.
     */
    public function crypto_decrypt(, $algorithm, $key, $data)
    {
        $request = $this->$session->request();

        $request->set("function", "crypto_decrypt");

        /* pass along required parameters to the request payload */
        $request->set("algorithm", $algorithm);
        $request->set("key", $key);
        $request->set("data", $data);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * crypto_derive_key()
     *
     * Derive a key via PBKDF2-HMAC-SHA256 and store it in the keystore.
     * algorithm object requires name PBKDF2, salt (min 16 octets), length
     * (output octets), optional iterations (default 600000, min 100000),
     * optional hash SHA-256. baseKey may be polymorphic including utf8
     * encoding for passphrases. Requires execute access.
     *
     * @param object $algorithm PBKDF2 parameters object.
     * @param  $baseKey Passphrase or key material (polymorphic; utf8 encoding
     *                  allowed).
     * @param array $usages Default ["encrypt","decrypt"].
     * @param boolean $extractable Default false.
     *
     * @return object Derived CryptoKey (algorithm AES-GCM sized to length*8
     *                when 16 or 32 octets).
     */
    public function crypto_derive_key(, $algorithm, $baseKey, $usages = null, $extractable = null)
    {
        $request = $this->$session->request();

        $request->set("function", "crypto_derive_key");

        /* pass along required parameters to the request payload */
        $request->set("algorithm", $algorithm);
        $request->set("baseKey", $baseKey);

        /* pass along any optional parameters to the request payload */
        if ($usages != null)
            $request->set('usages', $usages);

        if ($extractable != null)
            $request->set('extractable', $extractable);

        return $request->get_result();
    }

    /**
     * crypto_destroy_key()
     *
     * Remove a key from the process keystore and cleanse its material.
     * Requires execute access.
     *
     * @param  $key CryptoKey object with keyId.
     *
     * @return null null
     */
    public function crypto_destroy_key(, $key)
    {
        $request = $this->$session->request();

        $request->set("function", "crypto_destroy_key");

        /* pass along required parameters to the request payload */
        $request->set("key", $key);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * crypto_digest()
     *
     * Compute a cryptographic digest of binary data. Algorithm must be
     * SHA-256 or SHA-512. Data may be base64Binary or hexBinary.
     *
     * @param string $algorithm Digest algorithm name: SHA-256 or SHA-512.
     * @param  $data Data to hash (base64Binary or hexBinary).
     *
     * @return base64Binary Digest octets as base64Binary.
     */
    public function crypto_digest(, $algorithm, $data)
    {
        $request = $this->$session->request();

        $request->set("function", "crypto_digest");

        /* pass along required parameters to the request payload */
        $request->set("algorithm", $algorithm);
        $request->set("data", $data);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * crypto_encrypt()
     *
     * Encrypt binary plaintext with AES-GCM. algorithm is an object { name:
     * "AES-GCM", length?, iv?, additionalData? }. If iv is omitted, a
     * 12-octet IV is generated. Returns ciphertext, iv, and tag. Requires
     * execute access.
     *
     * @param object $algorithm AES-GCM parameters object.
     * @param  $key Key material, CryptoKey, or key reference.
     * @param  $data Plaintext (base64Binary or hexBinary).
     *
     * @return object ciphertext, iv, tag, algorithm, keyLength.
     */
    public function crypto_encrypt(, $algorithm, $key, $data)
    {
        $request = $this->$session->request();

        $request->set("function", "crypto_encrypt");

        /* pass along required parameters to the request payload */
        $request->set("algorithm", $algorithm);
        $request->set("key", $key);
        $request->set("data", $data);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * crypto_export_key()
     *
     * Export raw key octets from a CryptoKey if extractable is true. Requires
     * execute access.
     *
     * @param  $key CryptoKey object with keyId.
     *
     * @return base64Binary Raw key material.
     */
    public function crypto_export_key(, $key)
    {
        $request = $this->$session->request();

        $request->set("function", "crypto_export_key");

        /* pass along required parameters to the request payload */
        $request->set("key", $key);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * crypto_generate_key()
     *
     * Generate a random key in the process keystore. Algorithm string or
     * object. AES-GCM length defaults to 256 bits. Requires execute access.
     *
     * @param  $algorithm Algorithm string or object { name, length? }.
     * @param array $usages Optional usages; defaults depend on algorithm
     *                      family.
     * @param boolean $extractable If true, crypto_export_key may export raw
     *                             key. Default false.
     *
     * @return object Generated CryptoKey handle.
     */
    public function crypto_generate_key(, $algorithm, $usages = null, $extractable = null)
    {
        $request = $this->$session->request();

        $request->set("function", "crypto_generate_key");

        /* pass along required parameters to the request payload */
        $request->set("algorithm", $algorithm);

        /* pass along any optional parameters to the request payload */
        if ($usages != null)
            $request->set('usages', $usages);

        if ($extractable != null)
            $request->set('extractable', $extractable);

        return $request->get_result();
    }

    /**
     * crypto_hmac()
     *
     * Compute HMAC over binary data. Algorithm must be HMAC-SHA-256 or
     * HMAC-SHA-512. Key is polymorphic (CryptoKey, raw binary, or reference
     * object). Requires execute access.
     *
     * @param string $algorithm HMAC algorithm: HMAC-SHA-256 or HMAC-SHA-512.
     * @param  $key Key material, CryptoKey object, or key reference { from,
     *              ... }.
     * @param  $data Data to MAC (base64Binary or hexBinary).
     *
     * @return base64Binary MAC octets as base64Binary.
     */
    public function crypto_hmac(, $algorithm, $key, $data)
    {
        $request = $this->$session->request();

        $request->set("function", "crypto_hmac");

        /* pass along required parameters to the request payload */
        $request->set("algorithm", $algorithm);
        $request->set("key", $key);
        $request->set("data", $data);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * crypto_hmac_verify()
     *
     * Verify an HMAC in constant time. Algorithm must be HMAC-SHA-256 or
     * HMAC-SHA-512. Requires execute access.
     *
     * @param string $algorithm HMAC algorithm: HMAC-SHA-256 or HMAC-SHA-512.
     * @param  $key Key material, CryptoKey object, or key reference.
     * @param  $data Data that was MAC'd (base64Binary or hexBinary).
     * @param  $mac Expected MAC (base64Binary or hexBinary).
     *
     * @return boolean true if MAC matches.
     */
    public function crypto_hmac_verify(, $algorithm, $key, $data, $mac)
    {
        $request = $this->$session->request();

        $request->set("function", "crypto_hmac_verify");

        /* pass along required parameters to the request payload */
        $request->set("algorithm", $algorithm);
        $request->set("key", $key);
        $request->set("data", $data);
        $request->set("mac", $mac);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * crypto_import_key()
     *
     * Import key material (or a key reference resolved to octets) into the
     * process keystore and return a CryptoKey handle. Algorithm is a string
     * registry name or object with name (and optional length for AES-GCM).
     * Default extractable is false. Requires execute access.
     *
     * @param  $keySource Raw binary key, or reference object { from:
     *                    environment|file|material, ... }.
     * @param  $algorithm Algorithm string or object { name, length? }.
     * @param array $usages Optional usages; defaults depend on algorithm
     *                      family.
     * @param boolean $extractable If true, crypto_export_key may export raw
     *                             key. Default false.
     *
     * @return object Imported CryptoKey handle.
     */
    public function crypto_import_key(, $keySource, $algorithm, $usages = null, $extractable = null)
    {
        $request = $this->$session->request();

        $request->set("function", "crypto_import_key");

        /* pass along required parameters to the request payload */
        $request->set("keySource", $keySource);
        $request->set("algorithm", $algorithm);

        /* pass along any optional parameters to the request payload */
        if ($usages != null)
            $request->set('usages', $usages);

        if ($extractable != null)
            $request->set('extractable', $extractable);

        return $request->get_result();
    }

    /**
     * crypto_seal()
     *
     * Convenience for AES-GCM encryption: generates an IV, encrypts data, and
     * returns a sealed object { algorithm, keyLength, iv, tag, ciphertext }.
     * Equivalent to crypto_encrypt({ name: "AES-GCM" }, key, data) with an
     * auto-generated IV. Use stringify() (and optional pure-JSON field
     * mapping) to store the result. Requires execute access.
     *
     * @param  $key Key material, CryptoKey, or key reference.
     * @param  $data Plaintext (base64Binary or hexBinary). Use
     *               encode_as_base64Binary() for UTF-8 text.
     *
     * @return object Sealed object with algorithm, keyLength, iv, tag, and
     *                ciphertext.
     */
    public function crypto_seal(, $key, $data)
    {
        $request = $this->$session->request();

        $request->set("function", "crypto_seal");

        /* pass along required parameters to the request payload */
        $request->set("key", $key);
        $request->set("data", $data);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * crypto_unseal()
     *
     * Decrypt a sealed value from crypto_seal / crypto_encrypt. sealed may
     * be: (1) an object with iv, tag, and ciphertext as
     * base64Binary/hexBinary or as base64/hex strings; (2) a string of pure
     * JSON with those properties as base64 strings (e.g. after stringify of a
     * JSON-friendly object). Returns plaintext octets. Requires execute
     * access.
     *
     * @param  $key Key material, CryptoKey, or key reference.
     * @param  $sealed Sealed object or pure JSON string.
     *
     * @return base64Binary Plaintext octets. Use decode_to_string() for UTF-8
     *                      text.
     */
    public function crypto_unseal(, $key, $sealed)
    {
        $request = $this->$session->request();

        $request->set("function", "crypto_unseal");

        /* pass along required parameters to the request payload */
        $request->set("key", $key);
        $request->set("sealed", $sealed);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * crypto_version_info()
     *
     * Returns runtime OpenSSL and afw_crypto version information and the list
     * of supported algorithm names.
     *
     *
     * @return object Version and algorithm information.
     */
    public function crypto_version_info()
    {
        $request = $this->$session->request();

        $request->set("function", "crypto_version_info");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
