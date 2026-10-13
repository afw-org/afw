// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for compiled_regexp
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */


/**
 * @file afw_value_compiled_regexp.c
 * @brief Implementation of afw_value interface for a compiled regexp
 *
 * A value of data type regexp that also holds its compiled form, so the
 * regexp functions use it without compiling the source again. It is
 * what compile<regexp> returns and what a regexp literal is.
 *
 * Two kinds (designs/lifetime-principles.md, Four kinds):
 *
 * - Pooled (afw_value_pooled_compiled_regexp_inf): lives in a pool it
 *   does not own, dest p of compile<regexp> or the compile unit's pool
 *   for a literal. Its ICU resources are closed with that pool.
 *
 * - Reference counted (afw_value_counted_compiled_regexp_inf): what
 *   get_assignable_value of a pooled one returns. A copy in its own
 *   pool (parent p->managed_p), whose count is its count. The copy
 *   clones the compiled form (ICU shares the compiled pattern between
 *   clones with its own atomic count), so it points into nothing it
 *   does not own, and a literal in a unit shared by several threads (a
 *   model's on* script) is only read: each thread's store makes its own
 *   copy.
 *
 * Neither changes after it is created. A find matches on a clone of the
 * ICU matcher (afw_utf8_regexp_find()), so one is safe on any thread.
 */

#include "afw_internal.h"



/* Methods both infs share. */

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default

/* Fully evaluated, so optional_evaluate is NULL. */
#define impl_afw_value_optional_evaluate NULL

#define impl_afw_value_create_iterator NULL

/* A scalar holds no references and can not lead back (no cycles). */
#define impl_afw_value_for_each_reference afw_value_no_references_for_each
#define impl_afw_value_release_references \
    afw_value_no_references_release_references
#define impl_afw_value_get_counted afw_value_not_counted_get_counted

/* Inf specific is always data type. */
#define AFW_IMPLEMENTATION_SPECIFIC (const void *)&afw_data_type_regexp_direct

/* Inf variables: data_type, is_evaluated_of_data_type, is_managed. */
#define AFW_IMPLEMENTATION_INF_VARIABLES \
    (const void *)&afw_data_type_regexp_direct, \
    (const void *)&afw_data_type_regexp_direct, \
    false

#define AFW_VALUE_SELF_T afw_value_compiled_regexp_t


/* Pooled: get_reference / release throw, as for any pooled scalar. */

static void
impl_pooled_release(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)self;
    AFW_THROW_ERROR_Z(general,
        "release of pooled compiled regexp", xctx);
}


static const afw_value_t *
impl_pooled_get_reference(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)self;
    AFW_THROW_ERROR_Z(general,
        "get_reference of pooled compiled regexp", xctx);
}


/* A counted copy in its own pool. Caller releases. */
static const afw_value_t *
impl_pooled_get_assignable_value(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_pool_t *own_p;
    afw_value_compiled_regexp_t * volatile copy;

    own_p = afw_pool_heap_create(p->managed_p,
        xctx->env->small_chunk_min, xctx);
    AFW_TRY {
        copy = afw_pool_calloc_type(own_p, afw_value_compiled_regexp_t,
            xctx);
        copy->inf = &afw_value_counted_compiled_regexp_inf;
        afw_utf8_set(&copy->internal, self->internal.s, self->internal.len,
            own_p, xctx);
        copy->regexp = afw_utf8_regexp_clone(self->regexp, own_p, xctx);
        copy->p = own_p;
    }
    AFW_CATCH_UNHANDLED {
        afw_pool_release(own_p, xctx);
        AFW_ERROR_RETHROW;
    }
    AFW_ENDTRY;

    return &copy->pub;
}


static afw_size_t
impl_pooled_get_reference_count(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)self;
    (void)xctx;
    return 0;
}


#define impl_afw_value_release impl_pooled_release
#define impl_afw_value_get_reference impl_pooled_get_reference
#define impl_afw_value_get_assignable_value impl_pooled_get_assignable_value
#define impl_afw_value_get_reference_count impl_pooled_get_reference_count
#define impl_afw_value_get_for_p_lifetime afw_value_pooled_get_for_p_lifetime

#define AFW_IMPLEMENTATION_ID "pooled_compiled_regexp"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_pooled_compiled_regexp_inf
#include "afw_value_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef impl_afw_value_release
#undef impl_afw_value_get_reference
#undef impl_afw_value_get_assignable_value
#undef impl_afw_value_get_reference_count
#undef impl_afw_value_get_for_p_lifetime



/* Counted: the count is its own pool's count. */

static void
impl_counted_release(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    afw_pool_release(self->p, xctx);
}


static const afw_value_t *
impl_counted_get_reference(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    afw_pool_get_reference(self->p, xctx);
    return &self->pub;
}


static const afw_value_t *
impl_counted_get_assignable_value(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    (void)p;
    return impl_counted_get_reference(self, xctx);
}


static afw_size_t
impl_counted_get_reference_count(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    return afw_pool_get_reference_count(self->p, xctx);
}


#define impl_afw_value_release impl_counted_release
#define impl_afw_value_get_reference impl_counted_get_reference
#define impl_afw_value_get_assignable_value impl_counted_get_assignable_value
#define impl_afw_value_get_reference_count impl_counted_get_reference_count
#define impl_afw_value_get_for_p_lifetime afw_value_counted_get_for_p_lifetime

#define AFW_VALUE_INF_ONLY 1
#define AFW_IMPLEMENTATION_ID "counted_compiled_regexp"
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_counted_compiled_regexp_inf
#include "afw_value_impl_declares.h"



/*
 * Implementation of method get_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_data_type(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)self;
    (void)xctx;
    return afw_data_type_regexp;
}


/*
 * Implementation of method produce_compiler_listing for interface
 * afw_value.
 */
void
impl_afw_value_produce_compiler_listing(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    afw_data_type_value_compiler_listing(afw_data_type_regexp,
        writer, &self->pub, xctx);
}


/*
 * Implementation of method decompile for interface afw_value.
 */
void
impl_afw_value_decompile(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    afw_data_type_write_as_expression(afw_data_type_regexp,
        writer, (const void *)&self->internal, xctx);
}


/*
 * Implementation of method get_info for interface afw_value.
 */
void
impl_afw_value_get_info(
    AFW_VALUE_SELF_T *self,
    afw_value_info_t *info,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    (void)p;
    (void)xctx;
    afw_memory_clear(info);
    info->value_inf_id = &self->pub.inf->rti.implementation_id;
    info->evaluated_data_type = afw_data_type_regexp;
    info->optimized_value = &self->pub;
}



/* Compile source into a compiled regexp value, pooled in p. */
const afw_value_t *
afw_value_compiled_regexp_create(
    const afw_utf8_t *source,
    const afw_utf8_t **error_message,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_compiled_regexp_t *self;
    const afw_utf8_regexp_t *regexp;

    regexp = afw_utf8_regexp_compile(source, 0, error_message, p, xctx);
    if (!regexp) {
        return NULL;
    }

    self = afw_pool_calloc_type(p, afw_value_compiled_regexp_t, xctx);
    self->inf = &afw_value_pooled_compiled_regexp_inf;
    afw_utf8_set(&self->internal, source->s, source->len, p, xctx);
    self->regexp = regexp;

    return &self->pub;
}
