// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework index sargability decision probe
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#include "afw.h"
#include "afw_adapter_impl_index.h"
#include "afw_query_criteria.h"

#include <stdio.h>
#include <string.h>

/**
 * @file index_sargable_probe.c
 * @brief C probe for afw_adapter_impl_index_sargable().
 *
 * afw_adapter_impl_index_sargable() is the sole gate deciding whether
 * retrieve_objects() uses an index-accelerated query or falls back to a
 * full scan (see impl_afw_adapter_session_retrieve_objects() in
 * src/afw_lmdb/afw_lmdb_adapter_session.c). It's a core, adapter-agnostic
 * function -- afw_adapter_impl_index_t is just { inf; indexDefinitions; },
 * and sargable()/is_property_indexed() only ever read indexDefinitions,
 * never call through inf -- so it's testable directly with a fake instance
 * and no live session/adapter/extension at all (issue #57).
 */

static const afw_object_t *
impl_index_definitions(const char *script_z, const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_utf8_t script;
    const afw_value_t *compiled;
    const afw_value_t *evaluated;

    script.s = (const afw_utf8_octet_t *)script_z;
    script.len = strlen(script_z);

    compiled = afw_compile_to_value(&script, NULL, afw_compile_type_script,
        NULL, NULL, p, xctx);
    evaluated = afw_value_evaluate(compiled, p, xctx);

    return afw_value_as_object(evaluated, xctx);
}

static afw_boolean_t
impl_check_sargable(
    const char *indexDefinitions_script_z,
    const char *object_type_id_z,
    const char *property_name_z,
    afw_query_criteria_filter_op_id_t op_id,
    afw_boolean_t has_criteria,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_adapter_impl_index_t instance;
    afw_query_criteria_filter_entry_t entry;
    afw_query_criteria_t criteria;
    afw_utf8_t object_type_id;
    afw_utf8_t property_name;

    afw_memory_clear(&instance);
    instance.indexDefinitions =
        impl_index_definitions(indexDefinitions_script_z, p, xctx);

    object_type_id.s = (const afw_utf8_octet_t *)object_type_id_z;
    object_type_id.len = strlen(object_type_id_z);

    afw_memory_clear(&entry);
    property_name.s = (const afw_utf8_octet_t *)property_name_z;
    property_name.len = strlen(property_name_z);
    entry.property_name = &property_name;
    entry.op_id = op_id;
    entry.on_true = AFW_QUERY_CRITERIA_TRUE;
    entry.on_false = AFW_QUERY_CRITERIA_FALSE;

    afw_memory_clear(&criteria);
    criteria.filter = has_criteria ? &entry : NULL;

    return afw_adapter_impl_index_sargable(
        &instance, &object_type_id, &criteria, xctx);
}

static int
impl_expect(
    afw_boolean_t actual,
    afw_boolean_t expected,
    const char *label)
{
    if (actual != expected) {
        fprintf(stderr, "%s: expected sargable=%s, got %s\n", label,
            expected ? "true" : "false", actual ? "true" : "false");
        return 1;
    }
    return 0;
}

static int
impl_sargable_indexed_eq(const afw_pool_t *p, afw_xctx_t *xctx)
{
    /* "surname" is indexed for Person; querying Person.surname with eq
       is exactly the case retrieve_objects() should accelerate. */
    afw_boolean_t sargable = impl_check_sargable(
        "return {surname: {objectType: [\"Person\"]}};",
        "Person", "surname", afw_query_criteria_filter_op_id_eq, true,
        p, xctx);
    return impl_expect(sargable, true, "indexed-eq");
}

static int
impl_not_sargable_unindexed(const afw_pool_t *p, afw_xctx_t *xctx)
{
    /* No definition at all for "email". */
    afw_boolean_t sargable = impl_check_sargable(
        "return {surname: {objectType: [\"Person\"]}};",
        "Person", "email", afw_query_criteria_filter_op_id_eq, true,
        p, xctx);
    return impl_expect(sargable, false, "unindexed-property");
}

static int
impl_not_sargable_wrong_objecttype(const afw_pool_t *p, afw_xctx_t *xctx)
{
    /* "surname" is indexed, but only for Person -- querying it on a
       different object type must not claim sargability. */
    afw_boolean_t sargable = impl_check_sargable(
        "return {surname: {objectType: [\"Person\"]}};",
        "Vehicle", "surname", afw_query_criteria_filter_op_id_eq, true,
        p, xctx);
    return impl_expect(sargable, false, "wrong-objectType");
}

static int
impl_not_sargable_unsupported_op(const afw_pool_t *p, afw_xctx_t *xctx)
{
    /* "surname" is indexed, but "ne" (not-equal) is outside the
       eq/lt/le/gt/ge whitelist afw_adapter_impl_index_sargable_entry()
       checks -- an index cannot accelerate a negative match. */
    afw_boolean_t sargable = impl_check_sargable(
        "return {surname: {objectType: [\"Person\"]}};",
        "Person", "surname", afw_query_criteria_filter_op_id_ne, true,
        p, xctx);
    return impl_expect(sargable, false, "unsupported-operator");
}

static int
impl_not_sargable_no_filter(const afw_pool_t *p, afw_xctx_t *xctx)
{
    /* No criteria/filter at all -- nothing for an index to accelerate. */
    afw_boolean_t sargable = impl_check_sargable(
        "return {surname: {objectType: [\"Person\"]}};",
        "Person", "surname", afw_query_criteria_filter_op_id_eq, false,
        p, xctx);
    return impl_expect(sargable, false, "no-filter");
}

static int
impl_not_sargable_key_value_mismatch(const afw_pool_t *p, afw_xctx_t *xctx)
{
    /*
     * Documents a real, non-obvious design limitation rather than a bug:
     * afw_adapter_impl_index_is_property_indexed() looks up
     * indexDefinitions by property_name used AS the definition's key --
     * it never consults the definition's "value" script to see what
     * property it actually indexes. So an index keyed "idx1" whose value
     * script indexes "surname" is invisible to a query filtering on
     * "surname": is_property_indexed("surname") does a direct key lookup
     * for "surname", finds nothing (the key is "idx1"), and reports not
     * sargable even though an index over that data effectively exists.
     */
    afw_boolean_t sargable = impl_check_sargable(
        "return {idx1: {objectType: [\"Person\"], "
        "value: \"property_get(current::object, 'surname')\"}};",
        "Person", "surname", afw_query_criteria_filter_op_id_eq, true,
        p, xctx);
    return impl_expect(sargable, false, "key-value-mismatch (known limitation)");
}

int
main(int argc, char **argv)
{
    const afw_error_t *create_error;
    afw_xctx_t *xctx;
    const afw_pool_t *p;
    const char *case_name;
    int rc;

    xctx = afw_environment_create(afw_version(), argc,
        (const char * const *)argv, &create_error);
    if (!xctx) {
        fprintf(stderr, "environment create failed\n");
        return 2;
    }

    case_name = (argc > 1) ? argv[1] : "";
    p = afw_pool_create(xctx->p, xctx);
    rc = 0;

    if (strcmp(case_name, "indexed-eq") == 0) {
        rc = impl_sargable_indexed_eq(p, xctx);
    }
    else if (strcmp(case_name, "unindexed-property") == 0) {
        rc = impl_not_sargable_unindexed(p, xctx);
    }
    else if (strcmp(case_name, "wrong-objecttype") == 0) {
        rc = impl_not_sargable_wrong_objecttype(p, xctx);
    }
    else if (strcmp(case_name, "unsupported-operator") == 0) {
        rc = impl_not_sargable_unsupported_op(p, xctx);
    }
    else if (strcmp(case_name, "no-filter") == 0) {
        rc = impl_not_sargable_no_filter(p, xctx);
    }
    else if (strcmp(case_name, "key-value-mismatch") == 0) {
        rc = impl_not_sargable_key_value_mismatch(p, xctx);
    }
    else {
        fprintf(stderr, "usage: index_sargable_probe "
            "indexed-eq|unindexed-property|wrong-objecttype|"
            "unsupported-operator|no-filter|key-value-mismatch\n");
        rc = 2;
    }

    afw_pool_release(p, xctx);
    afw_environment_release(xctx);
    return rc;
}
