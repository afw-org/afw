// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_crypto_function_execute_* for category crypto
 *
 * Copyright (c) 2010-2024 Clemson University
 */

/**
 * @file afw_crypto_function_crypto.c
 * @brief Adaptive function execute implementations for crypto (afw_crypto).
 */

#include "afw.h"
#include "afw_json.h"
#include "afw_crypto_internal.h"
#include "generated/afw_crypto_strings.h"
#include "generated/afw_crypto_version_info.h"

#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include <string.h>

/* Local usage string constants */
static const afw_utf8_t impl_usage_encrypt =
    AFW_UTF8_LITERAL("encrypt");
static const afw_utf8_t impl_usage_decrypt =
    AFW_UTF8_LITERAL("decrypt");
static const afw_utf8_t impl_usage_sign =
    AFW_UTF8_LITERAL("sign");
static const afw_utf8_t impl_usage_verify =
    AFW_UTF8_LITERAL("verify");

static void
impl_check_input_size(afw_size_t size, afw_xctx_t *xctx)
{
    if (size > AFW_CRYPTO_MAX_INPUT_OCTETS) {
        AFW_THROW_ERROR_Z(payload_too_large,
            "error:crypto:input_too_large: exceeds 16 MiB limit", xctx);
    }
}

static const EVP_MD *
impl_md_for_digest(afw_crypto_alg_kind_t k, afw_xctx_t *xctx)
{
    if (k == afw_crypto_alg_sha256 || k == afw_crypto_alg_hmac_sha256) {
        return EVP_sha256();
    }
    if (k == afw_crypto_alg_sha512 || k == afw_crypto_alg_hmac_sha512) {
        return EVP_sha512();
    }
    AFW_THROW_ERROR_Z(arg_error,
        "error:crypto:unknown_algorithm: digest/HMAC algorithm", xctx);
    return NULL;
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
    const afw_object_t *object;
    const afw_array_t *algs;
    const afw_value_t *v;
    const afw_pool_t *p = x->p;
    afw_xctx_t *xctx = x->xctx;
    const char *ossl;

    object = afw_object_create(p, xctx);
    afw_object_meta_set_object_type_id(object,
        afw_crypto_s__AdaptiveCryptoVersionInfo_, xctx);

    ossl = OpenSSL_version(OPENSSL_VERSION);
    afw_object_set_property_as_string_from_utf8_z(object,
        afw_crypto_s_opensslVersion, ossl, xctx);
    afw_object_set_property_as_string_from_utf8_z(object,
        afw_crypto_s_extensionVersion, AFW_CRYPTO_VERSION_STRING, xctx);

    algs = afw_array_create_generic(p, xctx);
#define PUSH_ALG(Z) \
    v = afw_value_create_unmanaged_string( \
        afw_utf8_create(Z, AFW_UTF8_Z_LEN, p, xctx), p, xctx); \
    afw_array_push_value(algs, v, xctx)
    PUSH_ALG("AES-GCM");
    PUSH_ALG("SHA-256");
    PUSH_ALG("SHA-512");
    PUSH_ALG("HMAC-SHA-256");
    PUSH_ALG("HMAC-SHA-512");
    PUSH_ALG("PBKDF2");
#undef PUSH_ALG

    afw_object_set_property(object, afw_crypto_s_algorithms,
        afw_value_create_unmanaged_array(algs, p, xctx), xctx);

    return afw_value_create_unmanaged_object(object, p, xctx);
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
    const afw_value_string_t *algorithm;
    const afw_value_t *data_v;
    const afw_memory_t *data;
    afw_crypto_alg_kind_t kind;
    const EVP_MD *md;
    unsigned char out[EVP_MAX_MD_SIZE];
    unsigned int out_len = 0;
    afw_memory_t mem;
    afw_octet_t *copy;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(algorithm, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(data_v, 2);

    kind = afw_crypto_internal_alg_from_name(&algorithm->internal, x->xctx);
    if (kind != afw_crypto_alg_sha256 && kind != afw_crypto_alg_sha512) {
        AFW_THROW_ERROR_Z(arg_error,
            "error:crypto:unknown_algorithm: digest requires SHA-256 or SHA-512",
            x->xctx);
    }
    data = afw_crypto_internal_require_binary(data_v, x->xctx);
    impl_check_input_size(data->size, x->xctx);
    md = impl_md_for_digest(kind, x->xctx);

    if (EVP_Digest(data->ptr, data->size, out, &out_len, md, NULL) != 1) {
        AFW_THROW_ERROR_Z(general,
            "error:crypto: EVP_Digest failed", x->xctx);
    }
    copy = afw_pool_malloc(x->p, out_len, x->xctx);
    memcpy(copy, out, out_len);
    OPENSSL_cleanse(out, sizeof(out));
    mem.ptr = copy;
    mem.size = out_len;
    return afw_value_create_unmanaged_base64Binary(&mem, x->p, x->xctx);
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
    const afw_value_string_t *algorithm;
    const afw_value_t *key_v;
    const afw_value_t *data_v;
    const afw_memory_t *data;
    afw_crypto_alg_kind_t kind;
    const EVP_MD *md;
    afw_crypto_resolved_key_t *rk;
    unsigned char out[EVP_MAX_MD_SIZE];
    unsigned int out_len = 0;
    afw_memory_t mem;
    afw_octet_t *copy;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(algorithm, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(key_v, 2);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(data_v, 3);

    kind = afw_crypto_internal_alg_from_name(&algorithm->internal, x->xctx);
    if (kind != afw_crypto_alg_hmac_sha256 &&
        kind != afw_crypto_alg_hmac_sha512) {
        AFW_THROW_ERROR_Z(arg_error,
            "error:crypto:unknown_algorithm: hmac requires HMAC-SHA-256 or HMAC-SHA-512",
            x->xctx);
    }
    data = afw_crypto_internal_require_binary(data_v, x->xctx);
    impl_check_input_size(data->size, x->xctx);
    md = impl_md_for_digest(kind, x->xctx);

    rk = afw_crypto_internal_resolve_key(key_v, &impl_usage_sign,
        false, x->p, x->xctx);

    if (HMAC(md, rk->ptr, (int)rk->size, data->ptr, data->size,
            out, &out_len) == NULL) {
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general, "error:crypto: HMAC failed", x->xctx);
    }
    afw_crypto_internal_resolved_key_release(rk, x->xctx);

    copy = afw_pool_malloc(x->p, out_len, x->xctx);
    memcpy(copy, out, out_len);
    OPENSSL_cleanse(out, sizeof(out));
    mem.ptr = copy;
    mem.size = out_len;
    return afw_value_create_unmanaged_base64Binary(&mem, x->p, x->xctx);
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
    const afw_value_string_t *algorithm;
    const afw_value_t *key_v;
    const afw_value_t *data_v;
    const afw_value_t *mac_v;
    const afw_memory_t *data;
    const afw_memory_t *mac;
    afw_crypto_alg_kind_t kind;
    const EVP_MD *md;
    afw_crypto_resolved_key_t *rk;
    unsigned char out[EVP_MAX_MD_SIZE];
    unsigned int out_len = 0;
    int ok;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(algorithm, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(key_v, 2);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(data_v, 3);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(mac_v, 4);

    kind = afw_crypto_internal_alg_from_name(&algorithm->internal, x->xctx);
    if (kind != afw_crypto_alg_hmac_sha256 &&
        kind != afw_crypto_alg_hmac_sha512) {
        AFW_THROW_ERROR_Z(arg_error,
            "error:crypto:unknown_algorithm: hmac_verify requires HMAC-SHA-*",
            x->xctx);
    }
    data = afw_crypto_internal_require_binary(data_v, x->xctx);
    mac = afw_crypto_internal_require_binary(mac_v, x->xctx);
    impl_check_input_size(data->size, x->xctx);
    md = impl_md_for_digest(kind, x->xctx);

    rk = afw_crypto_internal_resolve_key(key_v, &impl_usage_verify,
        false, x->p, x->xctx);

    if (HMAC(md, rk->ptr, (int)rk->size, data->ptr, data->size,
            out, &out_len) == NULL) {
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general, "error:crypto: HMAC failed", x->xctx);
    }
    afw_crypto_internal_resolved_key_release(rk, x->xctx);

    ok = 0;
    if (out_len == mac->size) {
        ok = (CRYPTO_memcmp(out, mac->ptr, out_len) == 0);
    }
    OPENSSL_cleanse(out, sizeof(out));
    return ok ? afw_boolean_v_true : afw_boolean_v_false;
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
    const afw_value_t *key_source;
    const afw_value_t *algorithm;
    const afw_value_t *usages_v = NULL;
    const afw_value_boolean_t *extractable_v = NULL;
    afw_crypto_alg_kind_t kind;
    const afw_utf8_t *alg_name;
    afw_integer_t length_bits;
    const afw_array_t *usages;
    afw_boolean_t extractable;
    afw_crypto_resolved_key_t *rk;
    const afw_value_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(key_source, 1);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(algorithm, 2);
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(usages_v, 3);
    }
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(
            extractable_v, 4, boolean);
    }

    afw_crypto_internal_parse_algorithm_param(algorithm, &kind, &alg_name,
        &length_bits, x->p, x->xctx);
    if (kind != afw_crypto_alg_aes_gcm &&
        kind != afw_crypto_alg_hmac_sha256 &&
        kind != afw_crypto_alg_hmac_sha512) {
        AFW_THROW_ERROR_Z(arg_error,
            "error:crypto:unknown_algorithm: import supports AES-GCM and HMAC-SHA-*",
            x->xctx);
    }

    usages = afw_crypto_internal_parse_usages(usages_v, kind, true,
        x->p, x->xctx);
    extractable = extractable_v ? extractable_v->internal : false;

    rk = afw_crypto_internal_resolve_key(key_source, NULL, false,
        x->p, x->xctx);
    result = afw_crypto_internal_import_key(rk->ptr, rk->size, kind, alg_name,
        length_bits, usages, extractable, x->p, x->xctx);
    afw_crypto_internal_resolved_key_release(rk, x->xctx);
    return result;
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
    const afw_value_t *algorithm;
    const afw_value_t *usages_v = NULL;
    const afw_value_boolean_t *extractable_v = NULL;
    afw_crypto_alg_kind_t kind;
    const afw_utf8_t *alg_name;
    afw_integer_t length_bits;
    const afw_array_t *usages;
    afw_boolean_t extractable;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(algorithm, 1);
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(usages_v, 2);
    }
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(
            extractable_v, 3, boolean);
    }

    afw_crypto_internal_parse_algorithm_param(algorithm, &kind, &alg_name,
        &length_bits, x->p, x->xctx);
    usages = afw_crypto_internal_parse_usages(usages_v, kind, true,
        x->p, x->xctx);
    extractable = extractable_v ? extractable_v->internal : false;

    return afw_crypto_internal_generate_key(kind, alg_name, length_bits,
        usages, extractable, x->p, x->xctx);
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
    const afw_value_t *key_v;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(key_v, 1);
    return afw_crypto_internal_export_key(key_v, x->p, x->xctx);
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
    const afw_value_t *key_v;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(key_v, 1);
    afw_crypto_internal_destroy_key(key_v, x->xctx);
    return afw_value_null;
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
    const afw_value_object_t *algorithm;
    const afw_value_t *key_v;
    const afw_value_t *data_v;
    const afw_object_t *alg_obj;
    const afw_value_t *name_v;
    const afw_memory_t *plaintext;
    const afw_memory_t *iv_in;
    const afw_memory_t *aad;
    afw_crypto_resolved_key_t *rk;
    EVP_CIPHER_CTX *ctx = NULL;
    const EVP_CIPHER *cipher;
    unsigned char iv_buf[AFW_CRYPTO_AES_GCM_IV_LEN];
    unsigned char tag[AFW_CRYPTO_AES_GCM_TAG_LEN];
    unsigned char *out = NULL;
    int out_len = 0;
    int len = 0;
    afw_integer_t key_bits;
    const afw_object_t *result_obj;
    afw_memory_t mem;
    afw_octet_t *copy;
    afw_size_t iv_len;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(algorithm, 1, object);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(key_v, 2);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(data_v, 3);

    alg_obj = algorithm->internal;
    name_v = afw_object_get_property(alg_obj, afw_crypto_s_name, x->xctx);
    if (!name_v || !AFW_VALUE_IS_DATA_TYPE(name_v, string) ||
        !afw_utf8_equal_utf8_z(
            &((const afw_value_string_t *)name_v)->internal, "AES-GCM")) {
        AFW_THROW_ERROR_Z(arg_error,
            "error:crypto:unknown_algorithm: encrypt requires AES-GCM",
            x->xctx);
    }

    plaintext = afw_crypto_internal_require_binary(data_v, x->xctx);
    impl_check_input_size(plaintext->size, x->xctx);

    iv_in = afw_crypto_internal_object_get_binary(alg_obj, afw_crypto_s_iv,
        false, NULL, x->xctx);
    aad = afw_crypto_internal_object_get_binary(alg_obj,
        afw_crypto_s_additionalData, false, NULL, x->xctx);

    rk = afw_crypto_internal_resolve_key(key_v, &impl_usage_encrypt,
        false, x->p, x->xctx);

    if (rk->size == 16) {
        key_bits = 128;
        cipher = EVP_aes_128_gcm();
    }
    else if (rk->size == 32) {
        key_bits = 256;
        cipher = EVP_aes_256_gcm();
    }
    else {
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(arg_error,
            "error:crypto:invalid_key_length: AES-GCM key must be 16 or 32 octets",
            x->xctx);
        return NULL;
    }

    if (iv_in) {
        if (iv_in->size < AFW_CRYPTO_AES_GCM_IV_LEN) {
            afw_crypto_internal_resolved_key_release(rk, x->xctx);
            AFW_THROW_ERROR_Z(arg_error,
                "error:crypto:missing_iv: iv must be at least 12 octets",
                x->xctx);
        }
        iv_len = iv_in->size;
        if (iv_len > sizeof(iv_buf)) {
            /* use provided IV from memory directly via pointer below */
        }
    }
    else {
        if (RAND_bytes(iv_buf, AFW_CRYPTO_AES_GCM_IV_LEN) != 1) {
            afw_crypto_internal_resolved_key_release(rk, x->xctx);
            AFW_THROW_ERROR_Z(general,
                "error:crypto: RAND_bytes failed for IV", x->xctx);
        }
        iv_len = AFW_CRYPTO_AES_GCM_IV_LEN;
    }

    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general, "error:crypto: EVP_CIPHER_CTX_new failed",
            x->xctx);
    }

    out = malloc(plaintext->size + 16);
    if (!out) {
        EVP_CIPHER_CTX_free(ctx);
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general, "error:crypto: out of memory", x->xctx);
    }

    if (EVP_EncryptInit_ex(ctx, cipher, NULL, NULL, NULL) != 1 ||
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, (int)iv_len,
            NULL) != 1 ||
        EVP_EncryptInit_ex(ctx, NULL, NULL, rk->ptr,
            iv_in ? iv_in->ptr : iv_buf) != 1) {
        free(out);
        EVP_CIPHER_CTX_free(ctx);
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general, "error:crypto: EncryptInit failed",
            x->xctx);
    }

    if (aad && aad->size > 0) {
        if (EVP_EncryptUpdate(ctx, NULL, &len, aad->ptr, (int)aad->size)
            != 1) {
            free(out);
            EVP_CIPHER_CTX_free(ctx);
            afw_crypto_internal_resolved_key_release(rk, x->xctx);
            AFW_THROW_ERROR_Z(general, "error:crypto: AAD update failed",
                x->xctx);
        }
    }

    if (EVP_EncryptUpdate(ctx, out, &out_len, plaintext->ptr,
            (int)plaintext->size) != 1) {
        free(out);
        EVP_CIPHER_CTX_free(ctx);
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general, "error:crypto: EncryptUpdate failed",
            x->xctx);
    }
    len = out_len;
    if (EVP_EncryptFinal_ex(ctx, out + len, &out_len) != 1) {
        free(out);
        EVP_CIPHER_CTX_free(ctx);
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general, "error:crypto: EncryptFinal failed",
            x->xctx);
    }
    len += out_len;
    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG,
            AFW_CRYPTO_AES_GCM_TAG_LEN, tag) != 1) {
        free(out);
        EVP_CIPHER_CTX_free(ctx);
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general, "error:crypto: GET_TAG failed", x->xctx);
    }

    EVP_CIPHER_CTX_free(ctx);
    afw_crypto_internal_resolved_key_release(rk, x->xctx);

    result_obj = afw_object_create(x->p, x->xctx);
    afw_object_meta_set_object_type_id(result_obj,
        afw_crypto_s__AdaptiveCryptoEncryptResult_, x->xctx);
    afw_object_set_property_as_string_from_utf8_z(result_obj,
        afw_crypto_s_algorithm, "AES-GCM", x->xctx);
    afw_object_set_property_as_integer(result_obj, afw_crypto_s_keyLength,
        key_bits, x->xctx);

    copy = afw_pool_malloc(x->p, (afw_size_t)len, x->xctx);
    memcpy(copy, out, (size_t)len);
    free(out);
    mem.ptr = copy;
    mem.size = (afw_size_t)len;
    afw_object_set_property_as_base64Binary(result_obj,
        afw_crypto_s_ciphertext, &mem, x->xctx);

    if (iv_in) {
        mem.ptr = afw_pool_malloc(x->p, iv_in->size, x->xctx);
        memcpy((void *)mem.ptr, iv_in->ptr, iv_in->size);
        mem.size = iv_in->size;
    }
    else {
        mem.ptr = afw_pool_malloc(x->p, AFW_CRYPTO_AES_GCM_IV_LEN, x->xctx);
        memcpy((void *)mem.ptr, iv_buf, AFW_CRYPTO_AES_GCM_IV_LEN);
        mem.size = AFW_CRYPTO_AES_GCM_IV_LEN;
    }
    afw_object_set_property_as_base64Binary(result_obj, afw_crypto_s_iv,
        &mem, x->xctx);

    mem.ptr = afw_pool_malloc(x->p, AFW_CRYPTO_AES_GCM_TAG_LEN, x->xctx);
    memcpy((void *)mem.ptr, tag, AFW_CRYPTO_AES_GCM_TAG_LEN);
    mem.size = AFW_CRYPTO_AES_GCM_TAG_LEN;
    OPENSSL_cleanse(tag, sizeof(tag));
    afw_object_set_property_as_base64Binary(result_obj, afw_crypto_s_tag,
        &mem, x->xctx);

    return afw_value_create_unmanaged_object(result_obj, x->p, x->xctx);
}

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
    const afw_value_object_t *algorithm;
    const afw_value_t *key_v;
    const afw_value_t *data_v;
    const afw_object_t *alg_obj;
    const afw_value_t *name_v;
    const afw_memory_t *ciphertext;
    const afw_memory_t *iv;
    const afw_memory_t *tag;
    const afw_memory_t *aad;
    afw_crypto_resolved_key_t *rk;
    EVP_CIPHER_CTX *ctx = NULL;
    const EVP_CIPHER *cipher;
    unsigned char *out = NULL;
    int out_len = 0;
    int len = 0;
    afw_memory_t mem;
    afw_octet_t *copy;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(algorithm, 1, object);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(key_v, 2);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(data_v, 3);

    alg_obj = algorithm->internal;
    name_v = afw_object_get_property(alg_obj, afw_crypto_s_name, x->xctx);
    if (!name_v || !AFW_VALUE_IS_DATA_TYPE(name_v, string) ||
        !afw_utf8_equal_utf8_z(
            &((const afw_value_string_t *)name_v)->internal, "AES-GCM")) {
        AFW_THROW_ERROR_Z(arg_error,
            "error:crypto:unknown_algorithm: decrypt requires AES-GCM",
            x->xctx);
    }

    ciphertext = afw_crypto_internal_require_binary(data_v, x->xctx);
    impl_check_input_size(ciphertext->size, x->xctx);

    iv = afw_crypto_internal_object_get_binary(alg_obj, afw_crypto_s_iv,
        true, "error:crypto:missing_iv: iv required for decrypt", x->xctx);
    tag = afw_crypto_internal_object_get_binary(alg_obj, afw_crypto_s_tag,
        true, "error:crypto:missing_tag: tag required for decrypt", x->xctx);
    if (tag->size != AFW_CRYPTO_AES_GCM_TAG_LEN) {
        AFW_THROW_ERROR_Z(arg_error,
            "error:crypto:invalid_tag_length: tag must be 16 octets",
            x->xctx);
    }
    aad = afw_crypto_internal_object_get_binary(alg_obj,
        afw_crypto_s_additionalData, false, NULL, x->xctx);

    rk = afw_crypto_internal_resolve_key(key_v, &impl_usage_decrypt,
        false, x->p, x->xctx);

    if (rk->size == 16) {
        cipher = EVP_aes_128_gcm();
    }
    else if (rk->size == 32) {
        cipher = EVP_aes_256_gcm();
    }
    else {
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(arg_error,
            "error:crypto:invalid_key_length: AES-GCM key must be 16 or 32 octets",
            x->xctx);
        return NULL;
    }

    ctx = EVP_CIPHER_CTX_new();
    out = malloc(ciphertext->size + 16);
    if (!ctx || !out) {
        free(out);
        if (ctx) {
            EVP_CIPHER_CTX_free(ctx);
        }
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general, "error:crypto: decrypt setup failed",
            x->xctx);
    }

    if (EVP_DecryptInit_ex(ctx, cipher, NULL, NULL, NULL) != 1 ||
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, (int)iv->size,
            NULL) != 1 ||
        EVP_DecryptInit_ex(ctx, NULL, NULL, rk->ptr, iv->ptr) != 1) {
        free(out);
        EVP_CIPHER_CTX_free(ctx);
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general,
            "error:crypto:decryption_failed", x->xctx);
    }

    if (aad && aad->size > 0) {
        if (EVP_DecryptUpdate(ctx, NULL, &len, aad->ptr, (int)aad->size)
            != 1) {
            free(out);
            EVP_CIPHER_CTX_free(ctx);
            afw_crypto_internal_resolved_key_release(rk, x->xctx);
            AFW_THROW_ERROR_Z(general,
                "error:crypto:decryption_failed", x->xctx);
        }
    }

    if (EVP_DecryptUpdate(ctx, out, &out_len, ciphertext->ptr,
            (int)ciphertext->size) != 1) {
        free(out);
        EVP_CIPHER_CTX_free(ctx);
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general,
            "error:crypto:decryption_failed", x->xctx);
    }
    len = out_len;

    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG,
            AFW_CRYPTO_AES_GCM_TAG_LEN, (void *)tag->ptr) != 1) {
        free(out);
        EVP_CIPHER_CTX_free(ctx);
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general,
            "error:crypto:decryption_failed", x->xctx);
    }

    if (EVP_DecryptFinal_ex(ctx, out + len, &out_len) != 1) {
        free(out);
        EVP_CIPHER_CTX_free(ctx);
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general,
            "error:crypto:decryption_failed", x->xctx);
    }
    len += out_len;

    EVP_CIPHER_CTX_free(ctx);
    afw_crypto_internal_resolved_key_release(rk, x->xctx);

    copy = afw_pool_malloc(x->p, (afw_size_t)len, x->xctx);
    memcpy(copy, out, (size_t)len);
    OPENSSL_cleanse(out, (size_t)len);
    free(out);
    mem.ptr = copy;
    mem.size = (afw_size_t)len;
    return afw_value_create_unmanaged_base64Binary(&mem, x->p, x->xctx);
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
    const afw_value_object_t *algorithm;
    const afw_value_t *base_key_v;
    const afw_value_t *usages_v = NULL;
    const afw_value_boolean_t *extractable_v = NULL;
    const afw_object_t *alg_obj;
    const afw_value_t *v;
    const afw_memory_t *salt;
    afw_integer_t iterations;
    afw_integer_t length_octets;
    afw_crypto_resolved_key_t *rk;
    unsigned char *out;
    const afw_array_t *usages;
    afw_boolean_t extractable;
    const afw_utf8_t *alg_name;
    afw_crypto_alg_kind_t store_alg;
    afw_integer_t length_bits;
    const afw_value_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(algorithm, 1, object);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(base_key_v, 2);
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(usages_v, 3);
    }
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(
            extractable_v, 4, boolean);
    }

    alg_obj = algorithm->internal;
    v = afw_object_get_property(alg_obj, afw_crypto_s_name, x->xctx);
    if (!v || !AFW_VALUE_IS_DATA_TYPE(v, string) ||
        !afw_utf8_equal_utf8_z(
            &((const afw_value_string_t *)v)->internal, "PBKDF2")) {
        AFW_THROW_ERROR_Z(arg_error,
            "error:crypto:unknown_algorithm: derive_key requires PBKDF2",
            x->xctx);
    }

    salt = afw_crypto_internal_object_get_binary(alg_obj, afw_crypto_s_salt,
        true, "error:crypto:expected_binary: salt required", x->xctx);
    if (salt->size < 16) {
        AFW_THROW_ERROR_Z(arg_error,
            "error:crypto:invalid_key_length: salt must be at least 16 octets",
            x->xctx);
    }

    v = afw_object_get_property(alg_obj, afw_crypto_s_length, x->xctx);
    if (!v || !AFW_VALUE_IS_DATA_TYPE(v, integer)) {
        AFW_THROW_ERROR_Z(arg_error,
            "error:crypto:invalid_key_length: PBKDF2 length (octets) required",
            x->xctx);
    }
    length_octets = ((const afw_value_integer_t *)v)->internal;
    if (length_octets <= 0 || length_octets > AFW_CRYPTO_MAX_KEY_OCTETS) {
        AFW_THROW_ERROR_Z(arg_error,
            "error:crypto:invalid_key_length: PBKDF2 length out of range",
            x->xctx);
    }

    iterations = AFW_CRYPTO_PBKDF2_DEFAULT_ITERATIONS;
    v = afw_object_get_property(alg_obj, afw_crypto_s_iterations, x->xctx);
    if (v) {
        if (!AFW_VALUE_IS_DATA_TYPE(v, integer)) {
            AFW_THROW_ERROR_Z(arg_error,
                "error:crypto:invalid_usage: iterations must be integer",
                x->xctx);
        }
        iterations = ((const afw_value_integer_t *)v)->internal;
    }
    if (iterations < AFW_CRYPTO_PBKDF2_MIN_ITERATIONS) {
        AFW_THROW_ERROR_Z(arg_error,
            "error:crypto:invalid_usage: iterations below minimum 100000",
            x->xctx);
    }

    /* hash optional; only SHA-256 in v1 */
    v = afw_object_get_property(alg_obj, afw_crypto_s_hash, x->xctx);
    if (v) {
        if (!AFW_VALUE_IS_DATA_TYPE(v, string) ||
            !afw_utf8_equal_utf8_z(
                &((const afw_value_string_t *)v)->internal, "SHA-256")) {
            AFW_THROW_ERROR_Z(arg_error,
                "error:crypto:unknown_algorithm: PBKDF2 hash must be SHA-256",
                x->xctx);
        }
    }

    rk = afw_crypto_internal_resolve_key(base_key_v, NULL, true,
        x->p, x->xctx);

    out = malloc((size_t)length_octets);
    if (!out) {
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general, "error:crypto: out of memory", x->xctx);
    }

    if (PKCS5_PBKDF2_HMAC(
            (const char *)rk->ptr, (int)rk->size,
            salt->ptr, (int)salt->size,
            (int)iterations,
            EVP_sha256(),
            (int)length_octets, out) != 1) {
        OPENSSL_cleanse(out, (size_t)length_octets);
        free(out);
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general, "error:crypto: PBKDF2 failed", x->xctx);
    }
    afw_crypto_internal_resolved_key_release(rk, x->xctx);

    /* Store derived key as AES-GCM if 16/32 octets, else HMAC-SHA-256 material */
    if (length_octets == 16 || length_octets == 32) {
        store_alg = afw_crypto_alg_aes_gcm;
        alg_name = afw_utf8_create("AES-GCM", AFW_UTF8_Z_LEN, x->p, x->xctx);
        length_bits = length_octets * 8;
        usages = afw_crypto_internal_parse_usages(usages_v,
            afw_crypto_alg_aes_gcm, true, x->p, x->xctx);
    }
    else {
        store_alg = afw_crypto_alg_hmac_sha256;
        alg_name = afw_utf8_create("HMAC-SHA-256", AFW_UTF8_Z_LEN,
            x->p, x->xctx);
        length_bits = length_octets * 8;
        usages = afw_crypto_internal_parse_usages(usages_v,
            afw_crypto_alg_hmac_sha256, true, x->p, x->xctx);
    }
    extractable = extractable_v ? extractable_v->internal : false;

    result = afw_crypto_internal_import_key(out, (afw_size_t)length_octets,
        store_alg, alg_name, length_bits, usages, extractable, x->p, x->xctx);
    OPENSSL_cleanse(out, (size_t)length_octets);
    free(out);
    return result;
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
    const afw_value_t *key_v;
    const afw_value_t *data_v;
    const afw_memory_t *plaintext;
    afw_crypto_resolved_key_t *rk;
    EVP_CIPHER_CTX *ctx = NULL;
    const EVP_CIPHER *cipher;
    unsigned char iv_buf[AFW_CRYPTO_AES_GCM_IV_LEN];
    unsigned char tag[AFW_CRYPTO_AES_GCM_TAG_LEN];
    unsigned char *out = NULL;
    int out_len = 0;
    int len = 0;
    afw_integer_t key_bits;
    const afw_object_t *result_obj;
    afw_memory_t mem;
    afw_octet_t *copy;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(key_v, 1);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(data_v, 2);

    plaintext = afw_crypto_internal_require_binary(data_v, x->xctx);
    impl_check_input_size(plaintext->size, x->xctx);

    rk = afw_crypto_internal_resolve_key(key_v, &impl_usage_encrypt,
        false, x->p, x->xctx);

    if (rk->size == 16) {
        key_bits = 128;
        cipher = EVP_aes_128_gcm();
    }
    else if (rk->size == 32) {
        key_bits = 256;
        cipher = EVP_aes_256_gcm();
    }
    else {
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(arg_error,
            "error:crypto:invalid_key_length: AES-GCM key must be 16 or 32 octets",
            x->xctx);
        return NULL;
    }

    if (RAND_bytes(iv_buf, AFW_CRYPTO_AES_GCM_IV_LEN) != 1) {
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general,
            "error:crypto: RAND_bytes failed for IV", x->xctx);
    }

    ctx = EVP_CIPHER_CTX_new();
    out = malloc(plaintext->size + 16);
    if (!ctx || !out) {
        free(out);
        if (ctx) {
            EVP_CIPHER_CTX_free(ctx);
        }
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general, "error:crypto: out of memory", x->xctx);
    }

    if (EVP_EncryptInit_ex(ctx, cipher, NULL, NULL, NULL) != 1 ||
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN,
            AFW_CRYPTO_AES_GCM_IV_LEN, NULL) != 1 ||
        EVP_EncryptInit_ex(ctx, NULL, NULL, rk->ptr, iv_buf) != 1 ||
        EVP_EncryptUpdate(ctx, out, &out_len, plaintext->ptr,
            (int)plaintext->size) != 1)
    {
        free(out);
        EVP_CIPHER_CTX_free(ctx);
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general, "error:crypto: seal encrypt failed",
            x->xctx);
    }
    len = out_len;
    if (EVP_EncryptFinal_ex(ctx, out + len, &out_len) != 1 ||
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG,
            AFW_CRYPTO_AES_GCM_TAG_LEN, tag) != 1)
    {
        free(out);
        EVP_CIPHER_CTX_free(ctx);
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general, "error:crypto: seal encrypt final failed",
            x->xctx);
    }
    len += out_len;
    EVP_CIPHER_CTX_free(ctx);
    afw_crypto_internal_resolved_key_release(rk, x->xctx);

    result_obj = afw_object_create(x->p, x->xctx);
    afw_object_meta_set_object_type_id(result_obj,
        afw_crypto_s__AdaptiveCryptoEncryptResult_, x->xctx);
    afw_object_set_property_as_string_from_utf8_z(result_obj,
        afw_crypto_s_algorithm, "AES-GCM", x->xctx);
    afw_object_set_property_as_integer(result_obj, afw_crypto_s_keyLength,
        key_bits, x->xctx);

    copy = afw_pool_malloc(x->p, (afw_size_t)len, x->xctx);
    memcpy(copy, out, (size_t)len);
    free(out);
    mem.ptr = copy;
    mem.size = (afw_size_t)len;
    afw_object_set_property_as_base64Binary(result_obj,
        afw_crypto_s_ciphertext, &mem, x->xctx);

    mem.ptr = afw_pool_malloc(x->p, AFW_CRYPTO_AES_GCM_IV_LEN, x->xctx);
    memcpy((void *)mem.ptr, iv_buf, AFW_CRYPTO_AES_GCM_IV_LEN);
    mem.size = AFW_CRYPTO_AES_GCM_IV_LEN;
    afw_object_set_property_as_base64Binary(result_obj, afw_crypto_s_iv,
        &mem, x->xctx);

    mem.ptr = afw_pool_malloc(x->p, AFW_CRYPTO_AES_GCM_TAG_LEN, x->xctx);
    memcpy((void *)mem.ptr, tag, AFW_CRYPTO_AES_GCM_TAG_LEN);
    mem.size = AFW_CRYPTO_AES_GCM_TAG_LEN;
    OPENSSL_cleanse(tag, sizeof(tag));
    afw_object_set_property_as_base64Binary(result_obj, afw_crypto_s_tag,
        &mem, x->xctx);

    return afw_value_create_unmanaged_object(result_obj, x->p, x->xctx);
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
    const afw_value_t *key_v;
    const afw_value_t *sealed_v;
    const afw_object_t *sealed_obj;
    const afw_memory_t *iv;
    const afw_memory_t *tag;
    const afw_memory_t *ciphertext;
    afw_crypto_resolved_key_t *rk;
    EVP_CIPHER_CTX *ctx = NULL;
    const EVP_CIPHER *cipher;
    unsigned char *out = NULL;
    int out_len = 0;
    int len = 0;
    afw_memory_t mem;
    afw_octet_t *copy;
    const afw_utf8_t *json_s;
    const afw_value_t *parsed;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(key_v, 1);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(sealed_v, 2);

    if (AFW_VALUE_IS_DATA_TYPE(sealed_v, string)) {
        json_s = &((const afw_value_string_t *)sealed_v)->internal;
        parsed = afw_json_to_value(json_s, NULL, x->p, x->xctx);
        if (!parsed || !AFW_VALUE_IS_DATA_TYPE(parsed, object)) {
            AFW_THROW_ERROR_Z(arg_error,
                "error:crypto:expected_binary: sealed string must be JSON object",
                x->xctx);
        }
        sealed_obj = ((const afw_value_object_t *)parsed)->internal;
    }
    else if (AFW_VALUE_IS_DATA_TYPE(sealed_v, object)) {
        sealed_obj = ((const afw_value_object_t *)sealed_v)->internal;
    }
    else {
        AFW_THROW_ERROR_Z(arg_error,
            "error:crypto:expected_binary: sealed must be object or JSON string",
            x->xctx);
        return NULL;
    }

    iv = afw_crypto_internal_object_get_binary_or_b64string(sealed_obj,
        afw_crypto_s_iv, true, "error:crypto:missing_iv: iv required",
        x->p, x->xctx);
    tag = afw_crypto_internal_object_get_binary_or_b64string(sealed_obj,
        afw_crypto_s_tag, true, "error:crypto:missing_tag: tag required",
        x->p, x->xctx);
    ciphertext = afw_crypto_internal_object_get_binary_or_b64string(sealed_obj,
        afw_crypto_s_ciphertext, true,
        "error:crypto:expected_binary: ciphertext required",
        x->p, x->xctx);

    if (tag->size != AFW_CRYPTO_AES_GCM_TAG_LEN) {
        AFW_THROW_ERROR_Z(arg_error,
            "error:crypto:invalid_tag_length: tag must be 16 octets",
            x->xctx);
    }
    impl_check_input_size(ciphertext->size, x->xctx);

    rk = afw_crypto_internal_resolve_key(key_v, &impl_usage_decrypt,
        false, x->p, x->xctx);

    if (rk->size == 16) {
        cipher = EVP_aes_128_gcm();
    }
    else if (rk->size == 32) {
        cipher = EVP_aes_256_gcm();
    }
    else {
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(arg_error,
            "error:crypto:invalid_key_length: AES-GCM key must be 16 or 32 octets",
            x->xctx);
        return NULL;
    }

    ctx = EVP_CIPHER_CTX_new();
    out = malloc(ciphertext->size + 16);
    if (!ctx || !out) {
        free(out);
        if (ctx) {
            EVP_CIPHER_CTX_free(ctx);
        }
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general, "error:crypto: unseal setup failed",
            x->xctx);
    }

    if (EVP_DecryptInit_ex(ctx, cipher, NULL, NULL, NULL) != 1 ||
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, (int)iv->size,
            NULL) != 1 ||
        EVP_DecryptInit_ex(ctx, NULL, NULL, rk->ptr, iv->ptr) != 1 ||
        EVP_DecryptUpdate(ctx, out, &out_len, ciphertext->ptr,
            (int)ciphertext->size) != 1 ||
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG,
            AFW_CRYPTO_AES_GCM_TAG_LEN, (void *)tag->ptr) != 1)
    {
        free(out);
        EVP_CIPHER_CTX_free(ctx);
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general, "error:crypto:decryption_failed", x->xctx);
    }
    len = out_len;
    if (EVP_DecryptFinal_ex(ctx, out + len, &out_len) != 1) {
        free(out);
        EVP_CIPHER_CTX_free(ctx);
        afw_crypto_internal_resolved_key_release(rk, x->xctx);
        AFW_THROW_ERROR_Z(general, "error:crypto:decryption_failed", x->xctx);
    }
    len += out_len;
    EVP_CIPHER_CTX_free(ctx);
    afw_crypto_internal_resolved_key_release(rk, x->xctx);

    copy = afw_pool_malloc(x->p, (afw_size_t)len, x->xctx);
    memcpy(copy, out, (size_t)len);
    OPENSSL_cleanse(out, (size_t)len);
    free(out);
    mem.ptr = copy;
    mem.size = (afw_size_t)len;
    return afw_value_create_unmanaged_base64Binary(&mem, x->p, x->xctx);
}
