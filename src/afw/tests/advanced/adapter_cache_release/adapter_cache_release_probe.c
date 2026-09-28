// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework adapter cache release probe
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/* Compiled with -DAFW_XCTX_INTERNAL_MEMBERS so xctx->cache is visible. */
#include "afw.h"
#include "afw_adapter_internal.h"

#include <stdio.h>
#include <string.h>

/**
 * @file adapter_cache_release_probe.c
 * @brief C probe for afw_adapter_session_commit_and_release_cache.
 *
 * Script cannot force one cached transaction to throw and still see
 * the others release. Stub infs count the calls.
 */

typedef struct impl_txn_s {
    afw_adapter_transaction_t pub;
    volatile int *commits;
    volatile int *releases;
    const char *throw_on_commit;
    const char *throw_on_release;
} impl_txn_t;

typedef struct impl_session_s {
    afw_adapter_session_t pub;
    volatile int *destroys;
    const char *throw_message;
} impl_session_t;


static afw_adapter_transaction_inf_t impl_txn_inf;
static afw_adapter_session_inf_t impl_session_inf;


static int
impl_fail(const char *label, const char *detail)
{
    fprintf(stderr, "%s: %s\n", label, detail);
    return 1;
}


static void
impl_txn_commit(
    const afw_adapter_transaction_t *instance,
    afw_xctx_t *xctx)
{
    impl_txn_t *self;

    self = (impl_txn_t *)instance;
    (*self->commits)++;
    if (self->throw_on_commit) {
        AFW_THROW_ERROR_Z(general, self->throw_on_commit, xctx);
    }
}


static void
impl_txn_release(
    const afw_adapter_transaction_t *instance,
    afw_xctx_t *xctx)
{
    impl_txn_t *self;

    self = (impl_txn_t *)instance;
    (*self->releases)++;
    if (self->throw_on_release) {
        AFW_THROW_ERROR_Z(general, self->throw_on_release, xctx);
    }
}


static void
impl_session_destroy(
    const afw_adapter_session_t *instance,
    afw_xctx_t *xctx)
{
    impl_session_t *self;

    self = (impl_session_t *)instance;
    (*self->destroys)++;
    if (self->throw_message) {
        AFW_THROW_ERROR_Z(general, self->throw_message, xctx);
    }
}


static void
impl_init_infs(void)
{
    memset(&impl_txn_inf, 0, sizeof(impl_txn_inf));
    impl_txn_inf.commit = impl_txn_commit;
    impl_txn_inf.release = impl_txn_release;
    memset(&impl_session_inf, 0, sizeof(impl_session_inf));
    impl_session_inf.destroy = impl_session_destroy;
}


static void
impl_prepare_txn(
    impl_txn_t *txn,
    afw_adapter_internal_transaction_t *slot,
    volatile int *commits,
    volatile int *releases,
    const char *throw_on_commit,
    const char *throw_on_release,
    afw_adapter_internal_cache_t *cache,
    afw_xctx_t *xctx)
{
    memset(txn, 0, sizeof(*txn));
    txn->pub.inf = &impl_txn_inf;
    txn->commits = commits;
    txn->releases = releases;
    txn->throw_on_commit = throw_on_commit;
    txn->throw_on_release = throw_on_release;
    memset(slot, 0, sizeof(*slot));
    slot->transaction = &txn->pub;
    afw_vector_push(cache->transactions, xctx) = slot;
}


static void
impl_prepare_session(
    impl_session_t *session,
    afw_adapter_t *adapter,
    afw_adapter_internal_session_cache_t *slot,
    const afw_utf8_t *adapter_id,
    volatile int *destroys,
    const char *throw_message,
    afw_boolean_t in_hash,
    afw_adapter_internal_cache_t *cache,
    afw_xctx_t *xctx)
{
    memset(adapter, 0, sizeof(*adapter));
    adapter->adapter_id = *adapter_id;
    memset(session, 0, sizeof(*session));
    session->pub.inf = &impl_session_inf;
    session->pub.adapter = adapter;
    session->destroys = destroys;
    session->throw_message = throw_message;
    if (in_hash) {
        memset(slot, 0, sizeof(*slot));
        slot->session = &session->pub;
        afw_hash_table_set(cache->session_cache,
            adapter->adapter_id.s, adapter->adapter_id.len,
            slot, xctx);
    }
    else {
        cache->runtime_adapter_session = &session->pub;
    }
}


/* CATCH records this. The walk clears the cache before it rethrows. */
static int impl_cache_still_set;


static int
impl_check_message(
    const char *label,
    const char *want,
    const afw_error_t *error)
{
    if (!error->message_z || strcmp(error->message_z, want) != 0) {
        fprintf(stderr, "%s: message '%s'\n", label,
            error->message_z ? error->message_z : "(null)");
        return 1;
    }
    if (error->code != afw_error_code_general) {
        return impl_fail(label, "code");
    }
    if (!error->backtrace) {
        return impl_fail(label, "backtrace dropped");
    }
    if (impl_cache_still_set) {
        return impl_fail(label, "cache still set");
    }
    return 0;
}


static int
impl_run(
    const char *label,
    afw_boolean_t abort,
    const char *want_message,
    afw_xctx_t *xctx)
{
    int rc;
    int threw;

    afw_flag_set(afw_s_a_flag_response_error_backtrace, true, xctx);
    impl_cache_still_set = 0;
    threw = 0;
    rc = 0;
    AFW_TRY {
        afw_adapter_session_commit_and_release_cache(abort, xctx);
    }
    AFW_CATCH_UNHANDLED {
        impl_cache_still_set = (xctx->cache != NULL);
        threw = 1;
        rc = impl_check_message(label, want_message, AFW_ERROR_THROWN);
    }
    AFW_ENDTRY;

    if (rc != 0) {
        return rc;
    }
    if (!threw) {
        return impl_fail(label, "did not throw");
    }
    if (xctx->error_processing_count != 0) {
        return impl_fail(label, "error_processing_count");
    }
    if (xctx->cache != NULL) {
        return impl_fail(label, "cache restored");
    }
    return 0;
}


static int
impl_commit_keeps_first(afw_xctx_t *xctx)
{
    afw_adapter_internal_cache_t *cache;
    impl_txn_t txn_a;
    impl_txn_t txn_b;
    afw_adapter_internal_transaction_t slot_a;
    afw_adapter_internal_transaction_t slot_b;
    impl_session_t session_a;
    impl_session_t session_b;
    impl_session_t runtime;
    afw_adapter_t adapter_a;
    afw_adapter_t adapter_b;
    afw_adapter_t adapter_r;
    afw_adapter_internal_session_cache_t hash_a;
    afw_adapter_internal_session_cache_t hash_b;
    static const afw_utf8_t id_a = AFW_UTF8_LITERAL("a");
    static const afw_utf8_t id_b = AFW_UTF8_LITERAL("b");
    static const afw_utf8_t id_r = AFW_UTF8_LITERAL("r");
    volatile int commits_a = 0;
    volatile int commits_b = 0;
    volatile int releases_a = 0;
    volatile int releases_b = 0;
    volatile int destroys_a = 0;
    volatile int destroys_b = 0;
    volatile int destroys_r = 0;
    int rc;

    cache = afw_adapter_internal_get_cache(xctx);
    /* Begin A then B. The walk commits B first. */
    impl_prepare_txn(&txn_a, &slot_a, &commits_a, &releases_a,
        "second", NULL, cache, xctx);
    impl_prepare_txn(&txn_b, &slot_b, &commits_b, &releases_b,
        "first", NULL, cache, xctx);
    impl_prepare_session(&session_a, &adapter_a, &hash_a, &id_a,
        &destroys_a, "session", true, cache, xctx);
    impl_prepare_session(&session_b, &adapter_b, &hash_b, &id_b,
        &destroys_b, NULL, true, cache, xctx);
    impl_prepare_session(&runtime, &adapter_r, NULL, &id_r,
        &destroys_r, "runtime", false, cache, xctx);

    rc = impl_run("commit_keeps_first", false, "first", xctx);
    if (rc != 0) {
        return rc;
    }
    if (commits_a != 1 || commits_b != 1 ||
        releases_a != 1 || releases_b != 1 ||
        destroys_a != 1 || destroys_b != 1 || destroys_r != 1)
    {
        fprintf(stderr,
            "commit_keeps_first: counts c=%d/%d r=%d/%d d=%d/%d/%d\n",
            commits_a, commits_b, releases_a, releases_b,
            destroys_a, destroys_b, destroys_r);
        return 1;
    }
    return 0;
}


static int
impl_abort_release_continues(afw_xctx_t *xctx)
{
    afw_adapter_internal_cache_t *cache;
    impl_txn_t txn_a;
    impl_txn_t txn_b;
    afw_adapter_internal_transaction_t slot_a;
    afw_adapter_internal_transaction_t slot_b;
    impl_session_t session_a;
    afw_adapter_t adapter_a;
    afw_adapter_internal_session_cache_t hash_a;
    static const afw_utf8_t id_a = AFW_UTF8_LITERAL("a");
    volatile int commits_a = 0;
    volatile int commits_b = 0;
    volatile int releases_a = 0;
    volatile int releases_b = 0;
    volatile int destroys_a = 0;
    int rc;

    cache = afw_adapter_internal_get_cache(xctx);
    impl_prepare_txn(&txn_a, &slot_a, &commits_a, &releases_a,
        "nope", NULL, cache, xctx);
    impl_prepare_txn(&txn_b, &slot_b, &commits_b, &releases_b,
        "nope", "rel", cache, xctx);
    impl_prepare_session(&session_a, &adapter_a, &hash_a, &id_a,
        &destroys_a, NULL, true, cache, xctx);

    rc = impl_run("abort_release_continues", true, "rel", xctx);
    if (rc != 0) {
        return rc;
    }
    if (commits_a != 0 || commits_b != 0 ||
        releases_a != 1 || releases_b != 1 || destroys_a != 1)
    {
        fprintf(stderr,
            "abort_release_continues: counts c=%d/%d r=%d/%d d=%d\n",
            commits_a, commits_b, releases_a, releases_b, destroys_a);
        return 1;
    }
    return 0;
}


static int
impl_runtime_throw(afw_xctx_t *xctx)
{
    afw_adapter_internal_cache_t *cache;
    impl_session_t runtime;
    afw_adapter_t adapter_r;
    static const afw_utf8_t id_r = AFW_UTF8_LITERAL("r");
    volatile int destroys_r = 0;
    int rc;

    cache = afw_adapter_internal_get_cache(xctx);
    impl_prepare_session(&runtime, &adapter_r, NULL, &id_r,
        &destroys_r, "runtime", false, cache, xctx);

    rc = impl_run("runtime_throw", false, "runtime", xctx);
    if (rc != 0) {
        return rc;
    }
    if (destroys_r != 1) {
        return impl_fail("runtime_throw", "destroy");
    }
    return 0;
}


static int
impl_all_ok(afw_xctx_t *xctx)
{
    afw_adapter_internal_cache_t *cache;
    impl_txn_t txn_a;
    impl_txn_t txn_b;
    afw_adapter_internal_transaction_t slot_a;
    afw_adapter_internal_transaction_t slot_b;
    impl_session_t session_a;
    impl_session_t runtime;
    afw_adapter_t adapter_a;
    afw_adapter_t adapter_r;
    afw_adapter_internal_session_cache_t hash_a;
    static const afw_utf8_t id_a = AFW_UTF8_LITERAL("a");
    static const afw_utf8_t id_r = AFW_UTF8_LITERAL("r");
    volatile int commits_a = 0;
    volatile int commits_b = 0;
    volatile int releases_a = 0;
    volatile int releases_b = 0;
    volatile int destroys_a = 0;
    volatile int destroys_r = 0;
    afw_size_t count;

    cache = afw_adapter_internal_get_cache(xctx);
    impl_prepare_txn(&txn_a, &slot_a, &commits_a, &releases_a,
        NULL, NULL, cache, xctx);
    impl_prepare_txn(&txn_b, &slot_b, &commits_b, &releases_b,
        NULL, NULL, cache, xctx);
    impl_prepare_session(&session_a, &adapter_a, &hash_a, &id_a,
        &destroys_a, NULL, true, cache, xctx);
    impl_prepare_session(&runtime, &adapter_r, NULL, &id_r,
        &destroys_r, NULL, false, cache, xctx);

    count = xctx->error_processing_count;
    afw_adapter_session_commit_and_release_cache(false, xctx);
    if (xctx->cache != NULL) {
        return impl_fail("all_ok", "cache");
    }
    if (xctx->error_processing_count != count) {
        return impl_fail("all_ok", "error_processing_count");
    }
    if (commits_a != 1 || commits_b != 1 ||
        releases_a != 1 || releases_b != 1 ||
        destroys_a != 1 || destroys_r != 1)
    {
        fprintf(stderr,
            "all_ok: counts c=%d/%d r=%d/%d d=%d/%d\n",
            commits_a, commits_b, releases_a, releases_b,
            destroys_a, destroys_r);
        return 1;
    }
    return 0;
}


static int
impl_both_throw(afw_xctx_t *xctx)
{
    afw_adapter_internal_cache_t *cache;
    impl_txn_t txn_a;
    impl_txn_t txn_b;
    afw_adapter_internal_transaction_t slot_a;
    afw_adapter_internal_transaction_t slot_b;
    volatile int commits_a = 0;
    volatile int commits_b = 0;
    volatile int releases_a = 0;
    volatile int releases_b = 0;
    int rc;

    cache = afw_adapter_internal_get_cache(xctx);
    /* Begin A then B. The walk runs B first. */
    impl_prepare_txn(&txn_a, &slot_a, &commits_a, &releases_a,
        "later", NULL, cache, xctx);
    impl_prepare_txn(&txn_b, &slot_b, &commits_b, &releases_b,
        "commit", "release", cache, xctx);

    rc = impl_run("both_throw", false, "commit", xctx);
    if (rc != 0) {
        return rc;
    }
    if (commits_a != 1 || commits_b != 1 ||
        releases_a != 1 || releases_b != 1)
    {
        fprintf(stderr, "both_throw: counts c=%d/%d r=%d/%d\n",
            commits_a, commits_b, releases_a, releases_b);
        return 1;
    }
    return 0;
}


static int
impl_quiet_inside_catch(afw_xctx_t *xctx)
{
    afw_adapter_internal_cache_t *cache;
    impl_txn_t txn_a;
    afw_adapter_internal_transaction_t slot_a;
    impl_session_t session_a;
    afw_adapter_t adapter_a;
    afw_adapter_internal_session_cache_t hash_a;
    static const afw_utf8_t id_a = AFW_UTF8_LITERAL("a");
    volatile int commits_a = 0;
    volatile int releases_a = 0;
    volatile int destroys_a = 0;
    volatile int rc = 0;
    volatile afw_size_t count_in_catch = 0;
    int saw;

    cache = afw_adapter_internal_get_cache(xctx);
    impl_prepare_txn(&txn_a, &slot_a, &commits_a, &releases_a,
        "should-not-commit", NULL, cache, xctx);
    impl_prepare_session(&session_a, &adapter_a, &hash_a, &id_a,
        &destroys_a, NULL, true, cache, xctx);
    afw_flag_set(afw_s_a_flag_response_error_backtrace, true, xctx);

    saw = 0;
    AFW_TRY {
        AFW_TRY {
            AFW_THROW_ERROR_Z(general, "request", xctx);
        }
        AFW_CATCH_UNHANDLED {
            afw_adapter_session_commit_and_release_cache(true, xctx);
            count_in_catch = xctx->error_processing_count;
            if (count_in_catch != 1) {
                rc = 1;
            }
            else if (xctx->cache != NULL) {
                rc = 2;
            }
            else if (!AFW_ERROR_THROWN->message_z ||
                strcmp(AFW_ERROR_THROWN->message_z, "request") != 0)
            {
                rc = 3;
            }
            else if (!AFW_ERROR_THROWN->backtrace) {
                rc = 4;
            }
            AFW_ERROR_RETHROW;
        }
        AFW_ENDTRY;
    }
    AFW_CATCH_UNHANDLED {
        saw = 1;
        if (rc == 0 &&
            (!AFW_ERROR_THROWN->message_z ||
            strcmp(AFW_ERROR_THROWN->message_z, "request") != 0))
        {
            rc = 5;
        }
    }
    AFW_ENDTRY;

    if (rc == 1) {
        fprintf(stderr, "quiet_inside_catch: count %d\n",
            (int)count_in_catch);
        return 1;
    }
    if (rc != 0) {
        fprintf(stderr, "quiet_inside_catch: check %d\n", rc);
        return 1;
    }
    if (!saw) {
        return impl_fail("quiet_inside_catch", "did not throw");
    }
    if (xctx->error_processing_count != 0) {
        return impl_fail("quiet_inside_catch", "error_processing_count");
    }
    if (commits_a != 0 || releases_a != 1 || destroys_a != 1) {
        fprintf(stderr, "quiet_inside_catch: counts c=%d r=%d d=%d\n",
            commits_a, releases_a, destroys_a);
        return 1;
    }
    return 0;
}


static int
impl_no_cache(afw_xctx_t *xctx)
{
    afw_size_t count;

    xctx->cache = NULL;
    count = xctx->error_processing_count;
    afw_adapter_session_commit_and_release_cache(false, xctx);
    if (xctx->cache != NULL) {
        return impl_fail("no_cache", "cache");
    }
    if (xctx->error_processing_count != count) {
        return impl_fail("no_cache", "error_processing_count");
    }
    return 0;
}


int
main(int argc, char **argv)
{
    const afw_error_t *create_error;
    afw_xctx_t *xctx;
    const char *case_name;
    int rc;

    xctx = afw_environment_create(afw_version(), argc,
        (const char * const *)argv, &create_error);
    if (!xctx) {
        fprintf(stderr, "environment create failed\n");
        return 2;
    }

    impl_init_infs();
    case_name = (argc > 1) ? argv[1] : "";
    rc = 0;

    if (strcmp(case_name, "commit_keeps_first") == 0) {
        rc = impl_commit_keeps_first(xctx);
    }
    else if (strcmp(case_name, "abort_release_continues") == 0) {
        rc = impl_abort_release_continues(xctx);
    }
    else if (strcmp(case_name, "runtime_throw") == 0) {
        rc = impl_runtime_throw(xctx);
    }
    else if (strcmp(case_name, "all_ok") == 0) {
        rc = impl_all_ok(xctx);
    }
    else if (strcmp(case_name, "no_cache") == 0) {
        rc = impl_no_cache(xctx);
    }
    else if (strcmp(case_name, "both_throw") == 0) {
        rc = impl_both_throw(xctx);
    }
    else if (strcmp(case_name, "quiet_inside_catch") == 0) {
        rc = impl_quiet_inside_catch(xctx);
    }
    else {
        fprintf(stderr, "unknown case '%s'\n", case_name);
        rc = 2;
    }

    return rc;
}
