// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_crypto_function_execute_* functions for crypto
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_crypto_function_crypto.c
 * @brief afw_crypto_function_execute_* functions for crypto.
 */

#include "afw.h"



/*
 * Adaptive function: crypto_decrypt
 *
 * afw_crypto_function_execute_crypto_decrypt
 *
 * See afw_crypto_function_bindings.h for more information.
 *
 * Decrypt AES-GCM ciphertext. algorithm must include name, iv, and tag (16
 * octets). additionalData must match encrypt if used. Requires execute access.
 * Auth failure yields a generic decryption error.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function crypto_decrypt(
 *       algorithm: object,
 *       key: any,
 *       data: any
 *   ): base64Binary;
 * ```
 *
 * Parameters:
 *
 *   algorithm - (object) AES-GCM parameters including required iv and tag.
 *
 *   key - (any dataType) Key material, CryptoKey, or key reference.
 *
 *   data - (any dataType) Ciphertext without tag (base64Binary or hexBinary).
 *
 * Returns:
 *
 *   (base64Binary) Plaintext octets.
 */
const afw_value_t *
afw_crypto_function_execute_crypto_decrypt(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: crypto_derive_key
 *
 * afw_crypto_function_execute_crypto_derive_key
 *
 * See afw_crypto_function_bindings.h for more information.
 *
 * Derive a key via PBKDF2-HMAC-SHA256 and store it in the keystore. algorithm
 * object requires name PBKDF2, salt (min 16 octets), length (output octets),
 * optional iterations (default 600000, min 100000), optional hash SHA-256.
 * baseKey may be polymorphic including utf8 encoding for passphrases. Requires
 * execute access.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function crypto_derive_key(
 *       algorithm: object,
 *       baseKey: any,
 *       usages?: (array string),
 *       extractable?: boolean
 *   ): (object _AdaptiveCryptoKey_);
 * ```
 *
 * Parameters:
 *
 *   algorithm - (object) PBKDF2 parameters object.
 *
 *   baseKey - (any dataType) Passphrase or key material (polymorphic; utf8
 *       encoding allowed).
 *
 *   usages - (optional array string) Default ["encrypt","decrypt"].
 *
 *   extractable - (optional boolean) Default false.
 *
 * Returns:
 *
 *   (object _AdaptiveCryptoKey_) Derived CryptoKey (algorithm AES-GCM sized to
 *       length*8 when 16 or 32 octets).
 */
const afw_value_t *
afw_crypto_function_execute_crypto_derive_key(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: crypto_destroy_key
 *
 * afw_crypto_function_execute_crypto_destroy_key
 *
 * See afw_crypto_function_bindings.h for more information.
 *
 * Remove a key from the process keystore and cleanse its material. Requires
 * execute access.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function crypto_destroy_key(
 *       key: any
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   key - (any dataType) CryptoKey object with keyId.
 *
 * Returns:
 *
 *   (null) null.
 */
const afw_value_t *
afw_crypto_function_execute_crypto_destroy_key(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: crypto_digest
 *
 * afw_crypto_function_execute_crypto_digest
 *
 * See afw_crypto_function_bindings.h for more information.
 *
 * Compute a cryptographic digest of binary data. Algorithm must be SHA-256 or
 * SHA-512. Data may be base64Binary or hexBinary.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function crypto_digest(
 *       algorithm: string,
 *       data: any
 *   ): base64Binary;
 * ```
 *
 * Parameters:
 *
 *   algorithm - (string) Digest algorithm name: SHA-256 or SHA-512.
 *
 *   data - (any dataType) Data to hash (base64Binary or hexBinary).
 *
 * Returns:
 *
 *   (base64Binary) Digest octets as base64Binary.
 */
const afw_value_t *
afw_crypto_function_execute_crypto_digest(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: crypto_encrypt
 *
 * afw_crypto_function_execute_crypto_encrypt
 *
 * See afw_crypto_function_bindings.h for more information.
 *
 * Encrypt binary plaintext with AES-GCM. algorithm is an object { name:
 * "AES-GCM", length?, iv?, additionalData? }. If iv is omitted, a 12-octet IV
 * is generated. Returns ciphertext, iv, and tag. Requires execute access.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function crypto_encrypt(
 *       algorithm: object,
 *       key: any,
 *       data: any
 *   ): (object _AdaptiveCryptoEncryptResult_);
 * ```
 *
 * Parameters:
 *
 *   algorithm - (object) AES-GCM parameters object.
 *
 *   key - (any dataType) Key material, CryptoKey, or key reference.
 *
 *   data - (any dataType) Plaintext (base64Binary or hexBinary).
 *
 * Returns:
 *
 *   (object _AdaptiveCryptoEncryptResult_) ciphertext, iv, tag, algorithm,
 *       keyLength.
 */
const afw_value_t *
afw_crypto_function_execute_crypto_encrypt(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: crypto_export_key
 *
 * afw_crypto_function_execute_crypto_export_key
 *
 * See afw_crypto_function_bindings.h for more information.
 *
 * Export raw key octets from a CryptoKey if extractable is true. Requires
 * execute access.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function crypto_export_key(
 *       key: any
 *   ): base64Binary;
 * ```
 *
 * Parameters:
 *
 *   key - (any dataType) CryptoKey object with keyId.
 *
 * Returns:
 *
 *   (base64Binary) Raw key material.
 */
const afw_value_t *
afw_crypto_function_execute_crypto_export_key(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: crypto_generate_key
 *
 * afw_crypto_function_execute_crypto_generate_key
 *
 * See afw_crypto_function_bindings.h for more information.
 *
 * Generate a random key in the process keystore. Algorithm string or object.
 * AES-GCM length defaults to 256 bits. Requires execute access.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function crypto_generate_key(
 *       algorithm: any,
 *       usages?: (array string),
 *       extractable?: boolean
 *   ): (object _AdaptiveCryptoKey_);
 * ```
 *
 * Parameters:
 *
 *   algorithm - (any dataType) Algorithm string or object { name, length? }.
 *
 *   usages - (optional array string) Optional usages; defaults depend on
 *       algorithm family.
 *
 *   extractable - (optional boolean) If true, crypto_export_key may export raw
 *       key. Default false.
 *
 * Returns:
 *
 *   (object _AdaptiveCryptoKey_) Generated CryptoKey handle.
 */
const afw_value_t *
afw_crypto_function_execute_crypto_generate_key(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: crypto_hmac
 *
 * afw_crypto_function_execute_crypto_hmac
 *
 * See afw_crypto_function_bindings.h for more information.
 *
 * Compute HMAC over binary data. Algorithm must be HMAC-SHA-256 or
 * HMAC-SHA-512. Key is polymorphic (CryptoKey, raw binary, or reference
 * object). Requires execute access.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function crypto_hmac(
 *       algorithm: string,
 *       key: any,
 *       data: any
 *   ): base64Binary;
 * ```
 *
 * Parameters:
 *
 *   algorithm - (string) HMAC algorithm: HMAC-SHA-256 or HMAC-SHA-512.
 *
 *   key - (any dataType) Key material, CryptoKey object, or key reference {
 *       from, ... }.
 *
 *   data - (any dataType) Data to MAC (base64Binary or hexBinary).
 *
 * Returns:
 *
 *   (base64Binary) MAC octets as base64Binary.
 */
const afw_value_t *
afw_crypto_function_execute_crypto_hmac(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: crypto_hmac_verify
 *
 * afw_crypto_function_execute_crypto_hmac_verify
 *
 * See afw_crypto_function_bindings.h for more information.
 *
 * Verify an HMAC in constant time. Algorithm must be HMAC-SHA-256 or
 * HMAC-SHA-512. Requires execute access.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function crypto_hmac_verify(
 *       algorithm: string,
 *       key: any,
 *       data: any,
 *       mac: any
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   algorithm - (string) HMAC algorithm: HMAC-SHA-256 or HMAC-SHA-512.
 *
 *   key - (any dataType) Key material, CryptoKey object, or key reference.
 *
 *   data - (any dataType) Data that was MAC'd (base64Binary or hexBinary).
 *
 *   mac - (any dataType) Expected MAC (base64Binary or hexBinary).
 *
 * Returns:
 *
 *   (boolean) true if MAC matches.
 */
const afw_value_t *
afw_crypto_function_execute_crypto_hmac_verify(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: crypto_import_key
 *
 * afw_crypto_function_execute_crypto_import_key
 *
 * See afw_crypto_function_bindings.h for more information.
 *
 * Import key material (or a key reference resolved to octets) into the process
 * keystore and return a CryptoKey handle. Algorithm is a string registry name
 * or object with name (and optional length for AES-GCM). Default extractable is
 * false. Requires execute access.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function crypto_import_key(
 *       keySource: any,
 *       algorithm: any,
 *       usages?: (array string),
 *       extractable?: boolean
 *   ): (object _AdaptiveCryptoKey_);
 * ```
 *
 * Parameters:
 *
 *   keySource - (any dataType) Raw binary key, or reference object { from:
 *       environment|file|material, ... }.
 *
 *   algorithm - (any dataType) Algorithm string or object { name, length? }.
 *
 *   usages - (optional array string) Optional usages; defaults depend on
 *       algorithm family.
 *
 *   extractable - (optional boolean) If true, crypto_export_key may export raw
 *       key. Default false.
 *
 * Returns:
 *
 *   (object _AdaptiveCryptoKey_) Imported CryptoKey handle.
 */
const afw_value_t *
afw_crypto_function_execute_crypto_import_key(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: crypto_seal
 *
 * afw_crypto_function_execute_crypto_seal
 *
 * See afw_crypto_function_bindings.h for more information.
 *
 * Convenience for AES-GCM encryption: generates an IV, encrypts data, and
 * returns a sealed object { algorithm, keyLength, iv, tag, ciphertext }.
 * Equivalent to crypto_encrypt({ name: "AES-GCM" }, key, data) with an
 * auto-generated IV. Use stringify() (and optional pure-JSON field mapping) to
 * store the result. Requires execute access.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function crypto_seal(
 *       key: any,
 *       data: any
 *   ): (object _AdaptiveCryptoEncryptResult_);
 * ```
 *
 * Parameters:
 *
 *   key - (any dataType) Key material, CryptoKey, or key reference.
 *
 *   data - (any dataType) Plaintext (base64Binary or hexBinary). Use
 *       encode_as_base64Binary() for UTF-8 text.
 *
 * Returns:
 *
 *   (object _AdaptiveCryptoEncryptResult_) Sealed object with algorithm,
 *       keyLength, iv, tag, and ciphertext.
 */
const afw_value_t *
afw_crypto_function_execute_crypto_seal(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: crypto_unseal
 *
 * afw_crypto_function_execute_crypto_unseal
 *
 * See afw_crypto_function_bindings.h for more information.
 *
 * Decrypt a sealed value from crypto_seal / crypto_encrypt. sealed may be: (1)
 * an object with iv, tag, and ciphertext as base64Binary/hexBinary or as
 * base64/hex strings; (2) a string of pure JSON with those properties as base64
 * strings (e.g. after stringify of a JSON-friendly bag). Returns plaintext
 * octets. Requires execute access.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function crypto_unseal(
 *       key: any,
 *       sealed: any
 *   ): base64Binary;
 * ```
 *
 * Parameters:
 *
 *   key - (any dataType) Key material, CryptoKey, or key reference.
 *
 *   sealed - (any dataType) Sealed object or pure JSON string.
 *
 * Returns:
 *
 *   (base64Binary) Plaintext octets. Use decode_to_string() for UTF-8 text.
 */
const afw_value_t *
afw_crypto_function_execute_crypto_unseal(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: crypto_version_info
 *
 * afw_crypto_function_execute_crypto_version_info
 *
 * See afw_crypto_function_bindings.h for more information.
 *
 * Returns runtime OpenSSL and afw_crypto version information and the list of
 * supported algorithm names.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function crypto_version_info(
 *   
 *   ): (object _AdaptiveCryptoVersionInfo_);
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (object _AdaptiveCryptoVersionInfo_) Version and algorithm information.
 */
const afw_value_t *
afw_crypto_function_execute_crypto_version_info(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
