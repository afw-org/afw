// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework afw_crypto — internal keystore and helpers
 *
 * Copyright (c) 2010-2024 Clemson University
 */

#ifndef __AFW_CRYPTO_INTERNAL_H__
#define __AFW_CRYPTO_INTERNAL_H__

#include "afw.h"

AFW_BEGIN_DECLARES

/** Max key material octets (env/file/material). */
#define AFW_CRYPTO_MAX_KEY_OCTETS 8192

/** Max plaintext/ciphertext per call. */
#define AFW_CRYPTO_MAX_INPUT_OCTETS (16 * 1024 * 1024)

/** Max process keystore entries. */
#define AFW_CRYPTO_MAX_KEYS 1024

/** Default PBKDF2 iterations (OWASP-aligned provisional). */
#define AFW_CRYPTO_PBKDF2_DEFAULT_ITERATIONS 600000

/** Minimum PBKDF2 iterations. */
#define AFW_CRYPTO_PBKDF2_MIN_ITERATIONS 100000

/** AES-GCM recommended IV length. */
#define AFW_CRYPTO_AES_GCM_IV_LEN 12

/** AES-GCM tag length. */
#define AFW_CRYPTO_AES_GCM_TAG_LEN 16

/**
 * @brief Algorithm kind for keystore entries.
 */
typedef enum {
    afw_crypto_alg_unknown = 0,
    afw_crypto_alg_aes_gcm,
    afw_crypto_alg_hmac_sha256,
    afw_crypto_alg_hmac_sha512,
    afw_crypto_alg_sha256,
    afw_crypto_alg_sha512,
    afw_crypto_alg_pbkdf2
} afw_crypto_alg_kind_t;

/**
 * @brief Resolved key material for one crypto operation.
 *
 * When must_cleanse is true, call afw_crypto_internal_resolved_key_release().
 * Key octets are always a private copy the caller may use until release.
 */
typedef struct afw_crypto_resolved_key_s {
    afw_octet_t *ptr;
    afw_size_t size;
    afw_boolean_t must_cleanse;
    afw_crypto_alg_kind_t keystore_alg; /* if from CryptoKey; else unknown */
    afw_integer_t key_length_bits;      /* when known from keystore */
} afw_crypto_resolved_key_t;

/**
 * @brief Initialize process-global keystore (call from extension initialize).
 */
void
afw_crypto_internal_keystore_initialize(afw_xctx_t *xctx);

void
afw_crypto_internal_keystore_shutdown(afw_xctx_t *xctx);

const afw_memory_t *
afw_crypto_internal_require_binary(
    const afw_value_t *value,
    afw_xctx_t *xctx);

afw_crypto_alg_kind_t
afw_crypto_internal_alg_from_name(
    const afw_utf8_t *name,
    afw_xctx_t *xctx);

void
afw_crypto_internal_parse_algorithm_param(
    const afw_value_t *algorithm,
    afw_crypto_alg_kind_t *out_kind,
    const afw_utf8_t **out_name,
    afw_integer_t *out_length_bits,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

afw_crypto_resolved_key_t *
afw_crypto_internal_resolve_key(
    const afw_value_t *key_value,
    const afw_utf8_t *required_usage,
    afw_boolean_t allow_utf8_material,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

void
afw_crypto_internal_resolved_key_release(
    afw_crypto_resolved_key_t *rk,
    afw_xctx_t *xctx);

const afw_value_t *
afw_crypto_internal_import_key(
    const afw_octet_t *key_ptr,
    afw_size_t key_size,
    afw_crypto_alg_kind_t alg,
    const afw_utf8_t *alg_name,
    afw_integer_t length_bits,
    const afw_array_t *usages,
    afw_boolean_t extractable,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

const afw_value_t *
afw_crypto_internal_generate_key(
    afw_crypto_alg_kind_t alg,
    const afw_utf8_t *alg_name,
    afw_integer_t length_bits,
    const afw_array_t *usages,
    afw_boolean_t extractable,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

const afw_value_t *
afw_crypto_internal_export_key(
    const afw_value_t *key_value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

void
afw_crypto_internal_destroy_key(
    const afw_value_t *key_value,
    afw_xctx_t *xctx);

const afw_array_t *
afw_crypto_internal_default_usages(
    afw_crypto_alg_kind_t alg,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

const afw_array_t *
afw_crypto_internal_parse_usages(
    const afw_value_t *usages_value,
    afw_crypto_alg_kind_t alg,
    afw_boolean_t use_defaults_if_absent,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

const afw_memory_t *
afw_crypto_internal_object_get_binary(
    const afw_object_t *obj,
    const afw_utf8_t *name,
    afw_boolean_t required,
    const char *missing_prefix,
    afw_xctx_t *xctx);

AFW_END_DECLARES

#endif /* __AFW_CRYPTO_INTERNAL_H__ */
