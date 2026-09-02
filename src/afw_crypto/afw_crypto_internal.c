// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework afw_crypto — keystore, resolve_key, helpers
 *
 * Copyright (c) 2010-2024 Clemson University
 */

/**
 * @file afw_crypto_internal.c
 * @brief Process keystore and polymorphic key resolution for afw_crypto.
 */

#include "afw.h"
#include "afw_crypto_internal.h"
#include "generated/afw_crypto_strings.h"

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include <string.h>
#include <stdlib.h>

/* Keystore entry */
typedef struct afw_crypto_key_entry_s {
    afw_integer_t key_id;
    afw_octet_t *material;
    afw_size_t material_size;
    afw_crypto_alg_kind_t alg;
    char alg_name_z[32];
    afw_integer_t length_bits;
    afw_boolean_t extractable;
    /* usages as bits */
    afw_boolean_t use_encrypt;
    afw_boolean_t use_decrypt;
    afw_boolean_t use_sign;
    afw_boolean_t use_verify;
    afw_boolean_t use_derive;
    struct afw_crypto_key_entry_s *next;
} afw_crypto_key_entry_t;

static afw_thread_mutex_t *impl_keystore_mutex = NULL;
static afw_crypto_key_entry_t *impl_keystore_head = NULL;
static afw_size_t impl_keystore_count = 0;
static afw_boolean_t impl_keystore_ready = false;

void
afw_crypto_internal_keystore_initialize(afw_xctx_t *xctx)
{
    apr_status_t rv;

    if (impl_keystore_ready) {
        return;
    }

    /* OpenSSL 3 default provider init */
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG, NULL);

    rv = apr_thread_mutex_create(&impl_keystore_mutex,
        APR_THREAD_MUTEX_UNNESTED,
        afw_pool_get_apr_pool(xctx->env->p));
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, apr, rv,
            "afw_crypto: apr_thread_mutex_create failed", xctx);
    }
    impl_keystore_head = NULL;
    impl_keystore_count = 0;
    impl_keystore_ready = true;
}

void
afw_crypto_internal_keystore_shutdown(afw_xctx_t *xctx)
{
    afw_crypto_key_entry_t *e;
    afw_crypto_key_entry_t *next;

    (void)xctx;
    if (!impl_keystore_ready || !impl_keystore_mutex) {
        return;
    }
    apr_thread_mutex_lock(impl_keystore_mutex);
    for (e = impl_keystore_head; e; e = next) {
        next = e->next;
        if (e->material) {
            OPENSSL_cleanse(e->material, e->material_size);
            free(e->material);
        }
        free(e);
    }
    impl_keystore_head = NULL;
    impl_keystore_count = 0;
    apr_thread_mutex_unlock(impl_keystore_mutex);
}

const afw_memory_t *
afw_crypto_internal_require_binary(
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    if (!value) {
        AFW_THROW_ERROR_Z(argument_error, "error:crypto:expected_binary: missing binary value", xctx);
    }
    if (AFW_VALUE_IS_DATA_TYPE(value, base64Binary)) {
        return &((const afw_value_base64Binary_t *)value)->internal;
    }
    if (AFW_VALUE_IS_DATA_TYPE(value, hexBinary)) {
        return &((const afw_value_hexBinary_t *)value)->internal;
    }
    AFW_THROW_ERROR_Z(argument_error, "error:crypto:expected_binary: expected base64Binary or hexBinary",
        xctx);
    return NULL; /* not reached */
}

afw_crypto_alg_kind_t
afw_crypto_internal_alg_from_name(
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    if (!name) {
        AFW_THROW_ERROR_Z(argument_error, "error:crypto:unknown_algorithm: missing algorithm name", xctx);
    }
    if (afw_utf8_equal_utf8_z(name, "AES-GCM")) {
        return afw_crypto_alg_aes_gcm;
    }
    if (afw_utf8_equal_utf8_z(name, "SHA-256")) {
        return afw_crypto_alg_sha256;
    }
    if (afw_utf8_equal_utf8_z(name, "SHA-512")) {
        return afw_crypto_alg_sha512;
    }
    if (afw_utf8_equal_utf8_z(name, "HMAC-SHA-256")) {
        return afw_crypto_alg_hmac_sha256;
    }
    if (afw_utf8_equal_utf8_z(name, "HMAC-SHA-512")) {
        return afw_crypto_alg_hmac_sha512;
    }
    if (afw_utf8_equal_utf8_z(name, "PBKDF2")) {
        return afw_crypto_alg_pbkdf2;
    }
    AFW_THROW_ERROR_Z(argument_error, "error:crypto:unknown_algorithm: unsupported algorithm name", xctx);
    return afw_crypto_alg_unknown;
}

void
afw_crypto_internal_parse_algorithm_param(
    const afw_value_t *algorithm,
    afw_crypto_alg_kind_t *out_kind,
    const afw_utf8_t **out_name,
    afw_integer_t *out_length_bits,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *name;
    const afw_object_t *obj;
    const afw_value_t *v;

    *out_length_bits = 0;
    name = NULL;

    if (!algorithm) {
        AFW_THROW_ERROR_Z(argument_error, "error:crypto:unknown_algorithm: missing algorithm", xctx);
    }

    if (AFW_VALUE_IS_DATA_TYPE(algorithm, string)) {
        name = &((const afw_value_string_t *)algorithm)->internal;
    }
    else if (AFW_VALUE_IS_DATA_TYPE(algorithm, object)) {
        obj = ((const afw_value_object_t *)algorithm)->internal;
        v = afw_object_get_property(obj, afw_crypto_v_name, xctx);
        if (!v || !AFW_VALUE_IS_DATA_TYPE(v, string)) {
            AFW_THROW_ERROR_Z(argument_error, "error:crypto:unknown_algorithm: object requires string name",
                xctx);
        }
        name = &((const afw_value_string_t *)v)->internal;
        v = afw_object_get_property(obj, afw_crypto_v_length, xctx);
        if (v) {
            if (!AFW_VALUE_IS_DATA_TYPE(v, integer)) {
                AFW_THROW_ERROR_Z(argument_error, "error:crypto:unknown_algorithm: length must be integer",
                    xctx);
            }
            *out_length_bits =
                ((const afw_value_integer_t *)v)->internal;
        }
    }
    else {
        AFW_THROW_ERROR_Z(argument_error, "error:crypto:unknown_algorithm: expected string or object",
            xctx);
    }

    *out_kind = afw_crypto_internal_alg_from_name(name, xctx);
    *out_name = afw_utf8_clone(name, p, xctx);

    if (*out_kind == afw_crypto_alg_aes_gcm) {
        if (*out_length_bits == 0) {
            *out_length_bits = 256;
        }
        if (*out_length_bits != 128 && *out_length_bits != 256) {
            AFW_THROW_ERROR_Z(argument_error, "error:crypto:invalid_key_length: AES-GCM length must be 128 or 256",
                xctx);
        }
    }
}

static afw_boolean_t
impl_usage_allowed(
    const afw_crypto_key_entry_t *e,
    const afw_utf8_t *required_usage)
{
    if (!required_usage) {
        return true;
    }
    if (afw_utf8_equal_utf8_z(required_usage, "encrypt")) {
        return e->use_encrypt;
    }
    if (afw_utf8_equal_utf8_z(required_usage, "decrypt")) {
        return e->use_decrypt;
    }
    if (afw_utf8_equal_utf8_z(required_usage, "sign")) {
        return e->use_sign;
    }
    if (afw_utf8_equal_utf8_z(required_usage, "verify")) {
        return e->use_verify;
    }
    if (afw_utf8_equal_utf8_z(required_usage, "derive")) {
        return e->use_derive;
    }
    return false;
}

static void
impl_set_usages_on_entry(
    afw_crypto_key_entry_t *e,
    const afw_array_t *usages,
    afw_xctx_t *xctx)
{
    const afw_iterator_old_t *iterator;
    const afw_value_t *v;
    const afw_utf8_t *s;

    e->use_encrypt = e->use_decrypt = e->use_sign =
        e->use_verify = e->use_derive = false;

    iterator = NULL;
    for (;;) {
        v = afw_array_get_next_value(usages, &iterator, xctx->p, xctx);
        if (!v) {
            break;
        }
        if (!AFW_VALUE_IS_DATA_TYPE(v, string)) {
            AFW_THROW_ERROR_Z(argument_error, "error:crypto:invalid_usage: usages must be strings", xctx);
        }
        s = &((const afw_value_string_t *)v)->internal;
        if (afw_utf8_equal_utf8_z(s, "encrypt")) {
            e->use_encrypt = true;
        }
        else if (afw_utf8_equal_utf8_z(s, "decrypt")) {
            e->use_decrypt = true;
        }
        else if (afw_utf8_equal_utf8_z(s, "sign")) {
            e->use_sign = true;
        }
        else if (afw_utf8_equal_utf8_z(s, "verify")) {
            e->use_verify = true;
        }
        else if (afw_utf8_equal_utf8_z(s, "derive")) {
            e->use_derive = true;
        }
        else {
            AFW_THROW_ERROR_Z(argument_error, "error:crypto:invalid_usage: unknown usage string", xctx);
        }
    }
    if (!e->use_encrypt && !e->use_decrypt && !e->use_sign &&
        !e->use_verify && !e->use_derive) {
        AFW_THROW_ERROR_Z(argument_error, "error:crypto:invalid_usage: usages must be non-empty", xctx);
    }
}

const afw_array_t *
afw_crypto_internal_default_usages(
    afw_crypto_alg_kind_t alg,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_array_t *a;
    const afw_value_t *v;

    a = afw_array_create_unmanaged(p, xctx);
    if (alg == afw_crypto_alg_hmac_sha256 ||
        alg == afw_crypto_alg_hmac_sha512) {
        v = afw_value_create_unmanaged_string(
            afw_utf8_create("sign", AFW_UTF8_Z_LEN, p, xctx), p, xctx);
        afw_array_push_value(a, v, xctx);
        v = afw_value_create_unmanaged_string(
            afw_utf8_create("verify", AFW_UTF8_Z_LEN, p, xctx), p, xctx);
        afw_array_push_value(a, v, xctx);
    }
    else {
        /* AES-GCM, PBKDF2-derived, default encrypt+decrypt */
        v = afw_value_create_unmanaged_string(
            afw_utf8_create("encrypt", AFW_UTF8_Z_LEN, p, xctx), p, xctx);
        afw_array_push_value(a, v, xctx);
        v = afw_value_create_unmanaged_string(
            afw_utf8_create("decrypt", AFW_UTF8_Z_LEN, p, xctx), p, xctx);
        afw_array_push_value(a, v, xctx);
    }
    return a;
}

const afw_array_t *
afw_crypto_internal_parse_usages(
    const afw_value_t *usages_value,
    afw_crypto_alg_kind_t alg,
    afw_boolean_t use_defaults_if_absent,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    if (!usages_value) {
        if (use_defaults_if_absent) {
            return afw_crypto_internal_default_usages(alg, p, xctx);
        }
        AFW_THROW_ERROR_Z(argument_error, "error:crypto:invalid_usage: usages required", xctx);
    }
    if (!AFW_VALUE_IS_DATA_TYPE(usages_value, array)) {
        AFW_THROW_ERROR_Z(argument_error, "error:crypto:invalid_usage: usages must be array", xctx);
    }
    return ((const afw_value_array_t *)usages_value)->internal;
}

static afw_integer_t
impl_random_key_id(afw_xctx_t *xctx)
{
    afw_integer_t id;
    unsigned char buf[8];
    int i;

    for (i = 0; i < 16; i++) {
        if (RAND_bytes(buf, sizeof(buf)) != 1) {
            AFW_THROW_ERROR_Z(general, "error:crypto: openssl RAND_bytes failed", xctx);
        }
        memcpy(&id, buf, sizeof(id));
        /* avoid 0 */
        if (id != 0) {
            return id;
        }
    }
    AFW_THROW_ERROR_Z(general, "error:crypto: failed to allocate keyId", xctx);
    return 0;
}

static const afw_value_t *
impl_make_cryptokey_object(
    afw_crypto_key_entry_t *e,
    const afw_array_t *usages,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *obj;
    const afw_utf8_t *alg_name;

    obj = afw_object_create_unmanaged_new_p(p, xctx);
    afw_object_meta_set_object_type_id(obj,
        afw_crypto_s__AdaptiveCryptoKey_, xctx);

    afw_object_set_property_as_integer(obj, afw_crypto_v_keyId,
        e->key_id, xctx);

    alg_name = afw_utf8_create(e->alg_name_z, AFW_UTF8_Z_LEN, p, xctx);
    afw_object_set_property_as_string(obj, afw_crypto_v_algorithm,
        alg_name, xctx);

    afw_object_set_property(obj, afw_crypto_v_usages,
        afw_value_create_unmanaged_array(usages, p, xctx), xctx);

    afw_object_set_property_as_boolean(obj, afw_crypto_v_extractable,
        e->extractable, xctx);

    if (e->length_bits > 0) {
        afw_object_set_property_as_integer(obj, afw_crypto_v_length,
            e->length_bits, xctx);
    }

    afw_object_set_property_as_string(obj, afw_crypto_v_format,
        afw_utf8_create("raw", AFW_UTF8_Z_LEN, p, xctx), xctx);

    return afw_value_create_unmanaged_object(obj, p, xctx);
}

static void
impl_validate_key_size_for_alg(
    afw_crypto_alg_kind_t alg,
    afw_size_t key_size,
    afw_integer_t length_bits,
    afw_xctx_t *xctx)
{
    if (key_size == 0 || key_size > AFW_CRYPTO_MAX_KEY_OCTETS) {
        AFW_THROW_ERROR_Z(payload_too_large, "error:crypto:key_too_large: key material size out of range",
            xctx);
    }
    if (alg == afw_crypto_alg_aes_gcm) {
        if (length_bits == 128 && key_size != 16) {
            AFW_THROW_ERROR_Z(argument_error, "error:crypto:invalid_key_length: AES-128-GCM needs 16 octets",
                xctx);
        }
        if (length_bits == 256 && key_size != 32) {
            AFW_THROW_ERROR_Z(argument_error, "error:crypto:invalid_key_length: AES-256-GCM needs 32 octets",
                xctx);
        }
        if (length_bits != 128 && length_bits != 256) {
            /* infer from size */
            if (key_size != 16 && key_size != 32) {
                AFW_THROW_ERROR_Z(argument_error, "error:crypto:invalid_key_length: AES-GCM key must be 16 or 32 octets",
                    xctx);
            }
        }
    }
}

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
    afw_xctx_t *xctx)
{
    afw_crypto_key_entry_t *e;
    afw_integer_t id;
    const afw_value_t *result;

    if (!impl_keystore_ready) {
        AFW_THROW_ERROR_Z(general, "error:crypto: keystore not initialized", xctx);
    }

    if (alg == afw_crypto_alg_aes_gcm && length_bits == 0) {
        if (key_size == 16) {
            length_bits = 128;
        }
        else if (key_size == 32) {
            length_bits = 256;
        }
    }
    impl_validate_key_size_for_alg(alg, key_size, length_bits, xctx);

    e = calloc(1, sizeof(*e));
    if (!e) {
        AFW_THROW_ERROR_Z(general, "error:crypto: out of memory", xctx);
    }
    e->material = malloc(key_size);
    if (!e->material) {
        free(e);
        AFW_THROW_ERROR_Z(general, "error:crypto: out of memory", xctx);
    }
    memcpy(e->material, key_ptr, key_size);
    e->material_size = key_size;
    e->alg = alg;
    e->length_bits = length_bits;
    e->extractable = extractable;
    if (alg_name && alg_name->len < sizeof(e->alg_name_z)) {
        memcpy(e->alg_name_z, alg_name->s, alg_name->len);
        e->alg_name_z[alg_name->len] = 0;
    }
    else {
        free(e->material);
        free(e);
        AFW_THROW_ERROR_Z(argument_error, "error:crypto:unknown_algorithm: algorithm name too long", xctx);
    }
    impl_set_usages_on_entry(e, usages, xctx);

    apr_thread_mutex_lock(impl_keystore_mutex);
    if (impl_keystore_count >= AFW_CRYPTO_MAX_KEYS) {
        apr_thread_mutex_unlock(impl_keystore_mutex);
        OPENSSL_cleanse(e->material, e->material_size);
        free(e->material);
        free(e);
        AFW_THROW_ERROR_Z(bad_request, "error:crypto:keystore_full: maximum keys exceeded", xctx);
    }
    /* unique id */
    for (;;) {
        id = impl_random_key_id(xctx);
        {
            afw_crypto_key_entry_t *scan;
            afw_boolean_t clash = false;
            for (scan = impl_keystore_head; scan; scan = scan->next) {
                if (scan->key_id == id) {
                    clash = true;
                    break;
                }
            }
            if (!clash) {
                break;
            }
        }
    }
    e->key_id = id;
    e->next = impl_keystore_head;
    impl_keystore_head = e;
    impl_keystore_count++;
    apr_thread_mutex_unlock(impl_keystore_mutex);

    result = impl_make_cryptokey_object(e, usages, p, xctx);
    return result;
}

const afw_value_t *
afw_crypto_internal_generate_key(
    afw_crypto_alg_kind_t alg,
    const afw_utf8_t *alg_name,
    afw_integer_t length_bits,
    const afw_array_t *usages,
    afw_boolean_t extractable,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_size_t nbytes;
    afw_octet_t *buf;
    const afw_value_t *result;

    if (alg == afw_crypto_alg_aes_gcm) {
        if (length_bits == 0) {
            length_bits = 256;
        }
        nbytes = (afw_size_t)(length_bits / 8);
    }
    else if (alg == afw_crypto_alg_hmac_sha256) {
        nbytes = 32;
        length_bits = 256;
    }
    else if (alg == afw_crypto_alg_hmac_sha512) {
        nbytes = 64;
        length_bits = 512;
    }
    else {
        AFW_THROW_ERROR_Z(argument_error, "error:crypto:unknown_algorithm: cannot generate for this algorithm",
            xctx);
        return NULL;
    }

    buf = malloc(nbytes);
    if (!buf) {
        AFW_THROW_ERROR_Z(general, "error:crypto: out of memory", xctx);
    }
    if (RAND_bytes(buf, (int)nbytes) != 1) {
        OPENSSL_cleanse(buf, nbytes);
        free(buf);
        AFW_THROW_ERROR_Z(general, "error:crypto: RAND_bytes failed", xctx);
    }
    result = afw_crypto_internal_import_key(buf, nbytes, alg, alg_name,
        length_bits, usages, extractable, p, xctx);
    OPENSSL_cleanse(buf, nbytes);
    free(buf);
    return result;
}

static afw_crypto_key_entry_t *
impl_find_entry(afw_integer_t key_id)
{
    afw_crypto_key_entry_t *e;
    for (e = impl_keystore_head; e; e = e->next) {
        if (e->key_id == key_id) {
            return e;
        }
    }
    return NULL;
}

static afw_integer_t
impl_key_id_from_value(const afw_value_t *key_value, afw_xctx_t *xctx)
{
    const afw_object_t *obj;
    const afw_value_t *v;

    if (!key_value || !AFW_VALUE_IS_DATA_TYPE(key_value, object)) {
        AFW_THROW_ERROR_Z(argument_error, "error:crypto:unknown_key: expected CryptoKey object", xctx);
    }
    obj = ((const afw_value_object_t *)key_value)->internal;
    v = afw_object_get_property(obj, afw_crypto_v_keyId, xctx);
    if (!v || !AFW_VALUE_IS_DATA_TYPE(v, integer)) {
        AFW_THROW_ERROR_Z(not_found, "error:crypto:unknown_key: missing keyId", xctx);
    }
    return ((const afw_value_integer_t *)v)->internal;
}

const afw_value_t *
afw_crypto_internal_export_key(
    const afw_value_t *key_value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_integer_t id;
    afw_crypto_key_entry_t *e;
    afw_memory_t mem;
    afw_octet_t *copy;

    id = impl_key_id_from_value(key_value, xctx);
    apr_thread_mutex_lock(impl_keystore_mutex);
    e = impl_find_entry(id);
    if (!e) {
        apr_thread_mutex_unlock(impl_keystore_mutex);
        AFW_THROW_ERROR_Z(not_found, "error:crypto:unknown_key: keyId not in keystore", xctx);
    }
    if (!e->extractable) {
        apr_thread_mutex_unlock(impl_keystore_mutex);
        AFW_THROW_ERROR_Z(argument_error, "error:crypto:not_extractable: key is not extractable", xctx);
    }
    copy = afw_pool_malloc(p, e->material_size, xctx);
    memcpy(copy, e->material, e->material_size);
    mem.ptr = copy;
    mem.size = e->material_size;
    apr_thread_mutex_unlock(impl_keystore_mutex);
    return afw_value_create_unmanaged_base64Binary(&mem, p, xctx);
}

void
afw_crypto_internal_destroy_key(
    const afw_value_t *key_value,
    afw_xctx_t *xctx)
{
    afw_integer_t id;
    afw_crypto_key_entry_t *e;
    afw_crypto_key_entry_t **pp;

    id = impl_key_id_from_value(key_value, xctx);
    apr_thread_mutex_lock(impl_keystore_mutex);
    for (pp = &impl_keystore_head; *pp; pp = &(*pp)->next) {
        if ((*pp)->key_id == id) {
            e = *pp;
            *pp = e->next;
            impl_keystore_count--;
            apr_thread_mutex_unlock(impl_keystore_mutex);
            OPENSSL_cleanse(e->material, e->material_size);
            free(e->material);
            free(e);
            return;
        }
    }
    apr_thread_mutex_unlock(impl_keystore_mutex);
    AFW_THROW_ERROR_Z(not_found, "error:crypto:unknown_key: keyId not in keystore", xctx);
}

void
afw_crypto_internal_resolved_key_release(
    afw_crypto_resolved_key_t *rk,
    afw_xctx_t *xctx)
{
    (void)xctx;
    if (!rk) {
        return;
    }
    if (rk->must_cleanse && rk->ptr && rk->size) {
        OPENSSL_cleanse(rk->ptr, rk->size);
        free(rk->ptr);
    }
    free(rk);
}

static afw_crypto_resolved_key_t *
impl_resolved_from_octets(
    const afw_octet_t *src,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_crypto_resolved_key_t *rk;

    if (size > AFW_CRYPTO_MAX_KEY_OCTETS) {
        AFW_THROW_ERROR_Z(payload_too_large, "error:crypto:key_too_large: key material exceeds limit", xctx);
    }
    rk = calloc(1, sizeof(*rk));
    if (!rk) {
        AFW_THROW_ERROR_Z(general, "error:crypto: out of memory", xctx);
    }
    rk->ptr = malloc(size ? size : 1);
    if (!rk->ptr) {
        free(rk);
        AFW_THROW_ERROR_Z(general, "error:crypto: out of memory", xctx);
    }
    if (size) {
        memcpy(rk->ptr, src, size);
    }
    rk->size = size;
    rk->must_cleanse = true;
    rk->keystore_alg = afw_crypto_alg_unknown;
    rk->key_length_bits = 0;
    return rk;
}

static afw_crypto_resolved_key_t *
impl_resolve_reference_object(
    const afw_object_t *obj,
    afw_boolean_t allow_utf8_material,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *v;
    const afw_utf8_t *from;
    const afw_utf8_t *encoding;
    afw_boolean_t has_encoding;
    afw_memory_t decoded;
    const afw_memory_t *mem;
    const char *name_z;
    const char *env_z;
    const afw_utf8_t *path;
    const afw_utf8_t *resolved;
    const afw_memory_t *file_mem;
    afw_utf8_t as_utf8;

    v = afw_object_get_property(obj, afw_crypto_v_from, xctx);
    if (!v || !AFW_VALUE_IS_DATA_TYPE(v, string)) {
        AFW_THROW_ERROR_Z(argument_error, "error:crypto:expected_binary: key reference requires string from",
            xctx);
    }
    from = &((const afw_value_string_t *)v)->internal;

    encoding = NULL;
    has_encoding = false;
    v = afw_object_get_property(obj, afw_crypto_v_encoding, xctx);
    if (v) {
        if (!AFW_VALUE_IS_DATA_TYPE(v, string)) {
            AFW_THROW_ERROR_Z(argument_error, "error:crypto:invalid_encoding: encoding must be string",
                xctx);
        }
        encoding = &((const afw_value_string_t *)v)->internal;
        has_encoding = true;
    }

    if (afw_utf8_equal_utf8_z(from, "material")) {
        v = afw_object_get_property(obj, afw_crypto_v_data, xctx);
        if (!v) {
            AFW_THROW_ERROR_Z(argument_error, "error:crypto:expected_binary: material reference needs data",
                xctx);
        }
        mem = afw_crypto_internal_require_binary(v, xctx);
        return impl_resolved_from_octets(mem->ptr, mem->size, xctx);
    }

    if (afw_utf8_equal_utf8_z(from, "environment")) {
        v = afw_object_get_property(obj, afw_crypto_v_name, xctx);
        if (!v || !AFW_VALUE_IS_DATA_TYPE(v, string)) {
            AFW_THROW_ERROR_Z(argument_error, "error:crypto:env_not_found: environment ref needs name",
                xctx);
        }
        name_z = afw_utf8_z_create(
            ((const afw_value_string_t *)v)->internal.s,
            ((const afw_value_string_t *)v)->internal.len,
            p, xctx);
        env_z = getenv(name_z);
        if (!env_z || !*env_z) {
            AFW_THROW_ERROR_Z(not_found, "error:crypto:env_not_found: environment variable missing or empty",
                xctx);
        }
        as_utf8.s = env_z;
        as_utf8.len = strlen(env_z);
        if (!has_encoding || afw_utf8_equal_utf8_z(encoding, "utf8")) {
            if (!allow_utf8_material && has_encoding &&
                afw_utf8_equal_utf8_z(encoding, "utf8")) {
                /* utf8 only allowed when caller permits (derive) */
            }
            if (has_encoding && afw_utf8_equal_utf8_z(encoding, "utf8")) {
                if (!allow_utf8_material) {
                    AFW_THROW_ERROR_Z(argument_error, "error:crypto:invalid_encoding: utf8 not allowed for raw keys",
                        xctx);
                }
                return impl_resolved_from_octets(
                    (const afw_octet_t *)as_utf8.s, as_utf8.len, xctx);
            }
            if (!has_encoding) {
                /* default for env: treat as raw octets of the C string */
                return impl_resolved_from_octets(
                    (const afw_octet_t *)as_utf8.s, as_utf8.len, xctx);
            }
        }
        if (afw_utf8_equal_utf8_z(encoding, "base64")) {
            afw_memory_decode_base64(&decoded, &as_utf8, p, xctx);
            return impl_resolved_from_octets(decoded.ptr, decoded.size, xctx);
        }
        if (afw_utf8_equal_utf8_z(encoding, "hex")) {
            afw_memory_decode_printable_hex(&decoded, &as_utf8, p, xctx);
            return impl_resolved_from_octets(decoded.ptr, decoded.size, xctx);
        }
        if (afw_utf8_equal_utf8_z(encoding, "raw")) {
            return impl_resolved_from_octets(
                (const afw_octet_t *)as_utf8.s, as_utf8.len, xctx);
        }
        AFW_THROW_ERROR_Z(argument_error, "error:crypto:invalid_encoding: unsupported encoding", xctx);
    }

    if (afw_utf8_equal_utf8_z(from, "file")) {
        v = afw_object_get_property(obj, afw_crypto_v_path, xctx);
        if (!v || !AFW_VALUE_IS_DATA_TYPE(v, string)) {
            AFW_THROW_ERROR_Z(argument_error, "error:crypto:path_not_allowed: file ref needs path string",
                xctx);
        }
        path = &((const afw_value_string_t *)v)->internal;
        /* Throws on missing rootFilePaths / traversal — surface as path_not_allowed. */
        resolved = afw_file_path_resolve_rootFilePaths(path, p, xctx);
        file_mem = afw_file_to_memory(resolved, 0, p, xctx);
        if (file_mem->size > AFW_CRYPTO_MAX_KEY_OCTETS) {
            AFW_THROW_ERROR_Z(payload_too_large, "error:crypto:key_too_large: key file exceeds limit", xctx);
        }
        if (has_encoding && afw_utf8_equal_utf8_z(encoding, "base64")) {
            as_utf8.s = (const char *)file_mem->ptr;
            as_utf8.len = file_mem->size;
            /* trim trailing newline if present */
            while (as_utf8.len > 0 &&
                (as_utf8.s[as_utf8.len - 1] == '\n' ||
                 as_utf8.s[as_utf8.len - 1] == '\r')) {
                as_utf8.len--;
            }
            afw_memory_decode_base64(&decoded, &as_utf8, p, xctx);
            return impl_resolved_from_octets(decoded.ptr, decoded.size, xctx);
        }
        if (has_encoding && afw_utf8_equal_utf8_z(encoding, "hex")) {
            as_utf8.s = (const char *)file_mem->ptr;
            as_utf8.len = file_mem->size;
            while (as_utf8.len > 0 &&
                (as_utf8.s[as_utf8.len - 1] == '\n' ||
                 as_utf8.s[as_utf8.len - 1] == '\r')) {
                as_utf8.len--;
            }
            afw_memory_decode_printable_hex(&decoded, &as_utf8, p, xctx);
            return impl_resolved_from_octets(decoded.ptr, decoded.size, xctx);
        }
        if (has_encoding && afw_utf8_equal_utf8_z(encoding, "utf8")) {
            if (!allow_utf8_material) {
                AFW_THROW_ERROR_Z(argument_error, "error:crypto:invalid_encoding: utf8 not allowed for raw keys",
                    xctx);
            }
            return impl_resolved_from_octets(file_mem->ptr, file_mem->size,
                xctx);
        }
        /* raw / default */
        return impl_resolved_from_octets(file_mem->ptr, file_mem->size, xctx);
    }

    AFW_THROW_ERROR_Z(argument_error, "error:crypto:expected_binary: unsupported key reference from",
        xctx);
    return NULL;
}

afw_crypto_resolved_key_t *
afw_crypto_internal_resolve_key(
    const afw_value_t *key_value,
    const afw_utf8_t *required_usage,
    afw_boolean_t allow_utf8_material,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_memory_t *mem;
    const afw_object_t *obj;
    const afw_value_t *v;
    afw_integer_t id;
    afw_crypto_key_entry_t *e;
    afw_crypto_resolved_key_t *rk;

    if (!key_value) {
        AFW_THROW_ERROR_Z(argument_error, "error:crypto:expected_binary: missing key", xctx);
    }

    /* Raw binary material — no usage check */
    if (AFW_VALUE_IS_DATA_TYPE(key_value, base64Binary) ||
        AFW_VALUE_IS_DATA_TYPE(key_value, hexBinary)) {
        mem = afw_crypto_internal_require_binary(key_value, xctx);
        return impl_resolved_from_octets(mem->ptr, mem->size, xctx);
    }

    if (!AFW_VALUE_IS_DATA_TYPE(key_value, object)) {
        AFW_THROW_ERROR_Z(argument_error, "error:crypto:expected_binary: key must be binary or object",
            xctx);
    }

    obj = ((const afw_value_object_t *)key_value)->internal;

    /* CryptoKey: has keyId */
    v = afw_object_get_property(obj, afw_crypto_v_keyId, xctx);
    if (v && AFW_VALUE_IS_DATA_TYPE(v, integer)) {
        id = ((const afw_value_integer_t *)v)->internal;
        if (!impl_keystore_ready) {
            AFW_THROW_ERROR_Z(general, "error:crypto: keystore not initialized", xctx);
        }
        apr_thread_mutex_lock(impl_keystore_mutex);
        e = impl_find_entry(id);
        if (!e) {
            apr_thread_mutex_unlock(impl_keystore_mutex);
            AFW_THROW_ERROR_Z(not_found, "error:crypto:unknown_key: keyId not in keystore", xctx);
        }
        if (required_usage && !impl_usage_allowed(e, required_usage)) {
            apr_thread_mutex_unlock(impl_keystore_mutex);
            AFW_THROW_ERROR_Z(argument_error, "error:crypto:usage_not_permitted: key missing required usage",
                xctx);
        }
        rk = impl_resolved_from_octets(e->material, e->material_size, xctx);
        rk->keystore_alg = e->alg;
        rk->key_length_bits = e->length_bits;
        apr_thread_mutex_unlock(impl_keystore_mutex);
        return rk;
    }

    /* Reference object with from */
    v = afw_object_get_property(obj, afw_crypto_v_from, xctx);
    if (v) {
        return impl_resolve_reference_object(obj, allow_utf8_material, p,
            xctx);
    }

    AFW_THROW_ERROR_Z(argument_error, "error:crypto:expected_binary: object is neither CryptoKey nor key reference",
        xctx);
    return NULL;
}

const afw_memory_t *
afw_crypto_internal_object_get_binary(
    const afw_object_t *obj,
    const afw_utf8_t *name,
    afw_boolean_t required,
    const char *missing_prefix,
    afw_xctx_t *xctx)
{
    const afw_value_t *v;
    const afw_value_string_t name_value = AFW_VALUE_STRING_UNMANAGED(name);

    v = afw_object_get_property(obj, &name_value.pub, xctx);
    if (!v) {
        if (required) {
            AFW_THROW_ERROR_Z(argument_error, missing_prefix, xctx);
        }
        return NULL;
    }
    return afw_crypto_internal_require_binary(v, xctx);
}

const afw_memory_t *
afw_crypto_internal_object_get_binary_or_b64string(
    const afw_object_t *obj,
    const afw_utf8_t *name,
    afw_boolean_t required,
    const char *missing_prefix,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *v;
    afw_memory_t *mem;
    const afw_utf8_t *s;
    const afw_value_string_t name_value = AFW_VALUE_STRING_UNMANAGED(name);

    v = afw_object_get_property(obj, &name_value.pub, xctx);
    if (!v) {
        if (required) {
            AFW_THROW_ERROR_Z(argument_error, missing_prefix, xctx);
        }
        return NULL;
    }
    if (AFW_VALUE_IS_DATA_TYPE(v, base64Binary) ||
        AFW_VALUE_IS_DATA_TYPE(v, hexBinary))
    {
        return afw_crypto_internal_require_binary(v, xctx);
    }
    if (AFW_VALUE_IS_DATA_TYPE(v, string)) {
        s = &((const afw_value_string_t *)v)->internal;
        mem = afw_pool_calloc_type(p, afw_memory_t, xctx);
        /* JSON-friendly bags use base64 text in string properties. */
        afw_memory_decode_base64(mem, s, p, xctx);
        return mem;
    }
    AFW_THROW_ERROR_Z(argument_error,
        "error:crypto:expected_binary: sealed field must be binary or base64/hex string",
        xctx);
    return NULL; /* not reached */
}

