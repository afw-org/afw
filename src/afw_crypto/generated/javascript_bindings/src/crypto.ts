// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for crypto
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file crypto.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the crypto category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Decrypt AES-GCM ciphertext. algorithm must include name, iv, and tag (16
 * octets). additionalData must match encrypt if used. Requires execute
 * access. Auth failure yields a generic decryption error.
 * 
 * @param {object} algorithm - AES-GCM parameters including required iv and
 *     tag.
 * 
 * @param {} key - Key material, CryptoKey, or key reference.
 * 
 * @param {} data - Ciphertext without tag (base64Binary or hexBinary).
 * 
 * @returns {base64Binary} Plaintext octets.
 */
export function afwCryptoDecrypt(client : any, algorithm : object, key : any, data : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "crypto_decrypt";
    _action["algorithm"] = algorithm;
    _action["key"] = key;
    _action["data"] = data;

    return client.perform(_action);
}

/**
 * Derive a key via PBKDF2-HMAC-SHA256 and store it in the keystore. algorithm
 * object requires name PBKDF2, salt (min 16 octets), length (output octets),
 * optional iterations (default 600000, min 100000), optional hash SHA-256.
 * baseKey may be polymorphic including utf8 encoding for passphrases.
 * Requires execute access.
 * 
 * @param {object} algorithm - PBKDF2 parameters object.
 * 
 * @param {} baseKey - Passphrase or key material (polymorphic; utf8 encoding
 *     allowed).
 * 
 * @param {array} usages - Default ["encrypt","decrypt"].
 * 
 * @param {boolean} extractable - Default false.
 * 
 * @returns {object} Derived CryptoKey (algorithm AES-GCM sized to length*8
 *     when 16 or 32 octets).
 */
export function afwCryptoDeriveKey(client : any, algorithm : object, baseKey : any, usages? : any[], extractable? : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "crypto_derive_key";
    _action["algorithm"] = algorithm;
    _action["baseKey"] = baseKey;

    if (usages !== undefined)
        _action["usages"] = usages;

    if (extractable !== undefined)
        _action["extractable"] = extractable;

    return client.perform(_action);
}

/**
 * Remove a key from the process keystore and cleanse its material. Requires
 * execute access.
 * 
 * @param {} key - CryptoKey object with keyId.
 * 
 * @returns {null} null
 */
export function afwCryptoDestroyKey(client : any, key : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "crypto_destroy_key";
    _action["key"] = key;

    return client.perform(_action);
}

/**
 * Compute a cryptographic digest of binary data. Algorithm must be SHA-256 or
 * SHA-512. Data may be base64Binary or hexBinary.
 * 
 * @param {string} algorithm - Digest algorithm name: SHA-256 or SHA-512.
 * 
 * @param {} data - Data to hash (base64Binary or hexBinary).
 * 
 * @returns {base64Binary} Digest octets as base64Binary.
 */
export function afwCryptoDigest(client : any, algorithm : string, data : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "crypto_digest";
    _action["algorithm"] = algorithm;
    _action["data"] = data;

    return client.perform(_action);
}

/**
 * Encrypt binary plaintext with AES-GCM. algorithm is an object { name:
 * "AES-GCM", length?, iv?, additionalData? }. If iv is omitted, a 12-octet IV
 * is generated. Returns ciphertext, iv, and tag. Requires execute access.
 * 
 * @param {object} algorithm - AES-GCM parameters object.
 * 
 * @param {} key - Key material, CryptoKey, or key reference.
 * 
 * @param {} data - Plaintext (base64Binary or hexBinary).
 * 
 * @returns {object} ciphertext, iv, tag, algorithm, keyLength.
 */
export function afwCryptoEncrypt(client : any, algorithm : object, key : any, data : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "crypto_encrypt";
    _action["algorithm"] = algorithm;
    _action["key"] = key;
    _action["data"] = data;

    return client.perform(_action);
}

/**
 * Export raw key octets from a CryptoKey if extractable is true. Requires
 * execute access.
 * 
 * @param {} key - CryptoKey object with keyId.
 * 
 * @returns {base64Binary} Raw key material.
 */
export function afwCryptoExportKey(client : any, key : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "crypto_export_key";
    _action["key"] = key;

    return client.perform(_action);
}

/**
 * Generate a random key in the process keystore. Algorithm string or object.
 * AES-GCM length defaults to 256 bits. Requires execute access.
 * 
 * @param {} algorithm - Algorithm string or object { name, length? }.
 * 
 * @param {array} usages - Optional usages; defaults depend on algorithm
 *     family.
 * 
 * @param {boolean} extractable - If true, crypto_export_key may export raw
 *     key. Default false.
 * 
 * @returns {object} Generated CryptoKey handle.
 */
export function afwCryptoGenerateKey(client : any, algorithm : any, usages? : any[], extractable? : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "crypto_generate_key";
    _action["algorithm"] = algorithm;

    if (usages !== undefined)
        _action["usages"] = usages;

    if (extractable !== undefined)
        _action["extractable"] = extractable;

    return client.perform(_action);
}

/**
 * Compute HMAC over binary data. Algorithm must be HMAC-SHA-256 or
 * HMAC-SHA-512. Key is polymorphic (CryptoKey, raw binary, or reference
 * object). Requires execute access.
 * 
 * @param {string} algorithm - HMAC algorithm: HMAC-SHA-256 or HMAC-SHA-512.
 * 
 * @param {} key - Key material, CryptoKey object, or key reference { from,
 *     ... }.
 * 
 * @param {} data - Data to MAC (base64Binary or hexBinary).
 * 
 * @returns {base64Binary} MAC octets as base64Binary.
 */
export function afwCryptoHmac(client : any, algorithm : string, key : any, data : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "crypto_hmac";
    _action["algorithm"] = algorithm;
    _action["key"] = key;
    _action["data"] = data;

    return client.perform(_action);
}

/**
 * Verify an HMAC in constant time. Algorithm must be HMAC-SHA-256 or
 * HMAC-SHA-512. Requires execute access.
 * 
 * @param {string} algorithm - HMAC algorithm: HMAC-SHA-256 or HMAC-SHA-512.
 * 
 * @param {} key - Key material, CryptoKey object, or key reference.
 * 
 * @param {} data - Data that was MAC'd (base64Binary or hexBinary).
 * 
 * @param {} mac - Expected MAC (base64Binary or hexBinary).
 * 
 * @returns {boolean} true if MAC matches.
 */
export function afwCryptoHmacVerify(client : any, algorithm : string, key : any, data : any, mac : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "crypto_hmac_verify";
    _action["algorithm"] = algorithm;
    _action["key"] = key;
    _action["data"] = data;
    _action["mac"] = mac;

    return client.perform(_action);
}

/**
 * Import key material (or a key reference resolved to octets) into the
 * process keystore and return a CryptoKey handle. Algorithm is a string
 * registry name or object with name (and optional length for AES-GCM).
 * Default extractable is false. Requires execute access.
 * 
 * @param {} keySource - Raw binary key, or reference object { from:
 *     environment|file|material, ... }.
 * 
 * @param {} algorithm - Algorithm string or object { name, length? }.
 * 
 * @param {array} usages - Optional usages; defaults depend on algorithm
 *     family.
 * 
 * @param {boolean} extractable - If true, crypto_export_key may export raw
 *     key. Default false.
 * 
 * @returns {object} Imported CryptoKey handle.
 */
export function afwCryptoImportKey(client : any, keySource : any, algorithm : any, usages? : any[], extractable? : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "crypto_import_key";
    _action["keySource"] = keySource;
    _action["algorithm"] = algorithm;

    if (usages !== undefined)
        _action["usages"] = usages;

    if (extractable !== undefined)
        _action["extractable"] = extractable;

    return client.perform(_action);
}

/**
 * Convenience for AES-GCM encryption: generates an IV, encrypts data, and
 * returns a sealed object { algorithm, keyLength, iv, tag, ciphertext }.
 * Equivalent to crypto_encrypt({ name: "AES-GCM" }, key, data) with an
 * auto-generated IV. Use stringify() (and optional pure-JSON field mapping)
 * to store the result. Requires execute access.
 * 
 * @param {} key - Key material, CryptoKey, or key reference.
 * 
 * @param {} data - Plaintext (base64Binary or hexBinary). Use
 *     encode_as_base64Binary() for UTF-8 text.
 * 
 * @returns {object} Sealed object with algorithm, keyLength, iv, tag, and
 *     ciphertext.
 */
export function afwCryptoSeal(client : any, key : any, data : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "crypto_seal";
    _action["key"] = key;
    _action["data"] = data;

    return client.perform(_action);
}

/**
 * Decrypt a sealed value from crypto_seal / crypto_encrypt. sealed may be:
 * (1) an object with iv, tag, and ciphertext as base64Binary/hexBinary or as
 * base64/hex strings; (2) a string of pure JSON with those properties as
 * base64 strings (e.g. after stringify of a JSON-friendly object). Returns
 * plaintext octets. Requires execute access.
 * 
 * @param {} key - Key material, CryptoKey, or key reference.
 * 
 * @param {} sealed - Sealed object or pure JSON string.
 * 
 * @returns {base64Binary} Plaintext octets. Use decode_to_string() for UTF-8
 *     text.
 */
export function afwCryptoUnseal(client : any, key : any, sealed : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "crypto_unseal";
    _action["key"] = key;
    _action["sealed"] = sealed;

    return client.perform(_action);
}

/**
 * Returns runtime OpenSSL and afw_crypto version information and the list of
 * supported algorithm names.
 * 
 * @returns {object} Version and algorithm information.
 */
export function afwCryptoVersionInfo(client : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "crypto_version_info";

    return client.perform(_action);
}

