// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Model Adaptor
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_model_adaptor.c
 * @brief Adaptive Framework model adaptor.
 */

#include "afw_internal.h"



/* Declares and rti/inf defines for interface afw_adaptor */
#define AFW_IMPLEMENTATION_ID "model"
#include "afw_adaptor_factory_impl_declares.h"
#include "afw_adaptor_impl_declares.h"
#include "afw_adaptor_session_impl_declares.h"
#include "afw_adaptor_object_type_cache_impl_declares.h"


static const afw_utf8_t *
impl_on_object_type_ids[] = {
#define XX(id) \
    &afw_s_ ## id,
    AFW_MODEL_ON_MAP(XX)
#undef XX
    NULL
};

static afw_utf8_t
impl_on_paths[] = {
#define XX(id) \
    AFW_UTF8_LITERAL( "/afw/" #id "/current"),
    AFW_MODEL_ON_MAP(XX)
#undef XX
    { NULL }
};

typedef struct impl_filter_entry_s impl_filter_entry_t;

struct impl_filter_entry_s {
    const afw_query_criteria_filter_entry_t *old_entry;
    impl_filter_entry_t *next_entry;
    afw_query_criteria_filter_entry_t converted;
};


typedef struct {
    impl_filter_entry_t *anchor;
    const afw_model_object_type_t *model_object_type;
    const afw_pool_t *p;
    afw_xctx_t *xctx;
} impl_convert_entry_wa_t;

static const afw_utf8_t impl_factory_description =
AFW_UTF8_LITERAL("Adaptor type for using models to access objects.");


/* Model adaptor factory instance. */
AFW_DEFINE_CONST_DATA(afw_adaptor_factory_t)
afw_adaptor_factory_model =
{
    &impl_afw_adaptor_factory_inf,
    AFW_UTF8_LITERAL("model"),
    &impl_factory_description
};


static const afw_object_t *
impl_adapt_object_from_adaptor(
    afw_model_internal_adaptor_session_self_t *self,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_model_object_type_t *object_type,
    const afw_utf8_t *object_id,
    const afw_object_t *mapped_object,
    const afw_query_criteria_t *criteria,
    const afw_pool_t *p, afw_xctx_t *xctx);


static void
impl_get_property_type_by_mapped_property_name(
    const afw_model_property_type_t * *pt,
    const afw_utf8_t * *property_name,
    const afw_model_object_type_t *object_type,
    const afw_utf8_t *mapped_property_name,
    afw_xctx_t *xctx);


static void
impl_add_adapted_properties_from_adaptor(
    const afw_object_t *result,
    const afw_model_object_type_t *model_object_type,
    const afw_object_t *mapped_object,
    afw_model_internal_context_t *ctx,
    afw_xctx_t *xctx);


static void
impl_get_property_type_by_property_name(
    const afw_model_property_type_t * *pt,
    const afw_utf8_t * *mapped_property_name,
    const afw_model_object_type_t *object_type,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);


static afw_boolean_t
impl_model_object_cb(
    const afw_object_t *mapped_object,
    void *context,
    afw_xctx_t *xctx);


void
impl_get_property_type_by_mapped_property_name(
    const afw_model_property_type_t * *property_type,
    const afw_utf8_t * *property_name,
    const afw_model_object_type_t *object_type,
    const afw_utf8_t *mapped_property_name,
    afw_xctx_t *xctx)
{
    const afw_model_property_type_t * *pt;

    /* Check for matching property name. */
    for (pt = &object_type->property_type[0]; *pt; pt++)
    {
        if (afw_utf8_equal((*pt)->mapped_property_name,
            mapped_property_name))
        {
            *property_type = *pt;
            *property_name = (*pt)->property_name;
            return;
        }
    }

    /* Return otherProperties property type or NULL. */
    *property_type = object_type->property_type_other;
    *property_name = (object_type->property_type_other)
        ? mapped_property_name
        : NULL;
}



void
impl_get_property_type_by_property_name(
    const afw_model_property_type_t * *property_type,
    const afw_utf8_t * *mapped_property_name,
    const afw_model_object_type_t *object_type,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    const afw_model_property_type_t * *pt;

    /* Check for matching property name. */
    for (pt = &object_type->property_type[0]; *pt; pt++)
    {
        if (afw_utf8_equal((*pt)->property_name, property_name)) {
            *property_type = *pt;
            *mapped_property_name = (*pt)->mapped_property_name;
            return;
        }
    }

    /* Return otherProperties property type or NULL. */
    *property_type = object_type->property_type_other;
    *mapped_property_name = (object_type->property_type_other)
        ? property_name
        : NULL;
}



void
impl_add_adapted_properties_from_adaptor(
    const afw_object_t *result,
    const afw_model_object_type_t *model_object_type,
    const afw_object_t *mapped_object,
    afw_model_internal_context_t *ctx,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;
    const afw_model_property_type_t * *pt;
    afw_boolean_t use_default_processing;

    /* Process each property in object_type that is not in object. */
    for (pt = &model_object_type->property_type[0]; *pt; pt++)
    {
        AFW_TRY {

            /* If onGetProperty for this property, set it to value. */
            afw_memory_clear(&ctx->property_level);
            ctx->property_level.model_property_type = *pt;
            ctx->property_level.mapped_value = afw_object_get_property(
                mapped_object, (*pt)->mapped_property_name, xctx);

            use_default_processing = true;
            if ((*pt)->onGetProperty) {
                value = afw_value_evaluate((*pt)->onGetProperty,
                    ctx->p, xctx);
                if (value != ctx->useDefaultProcessing_value) {
                    value = afw_value_convert(value, (*pt)->data_type,
                        true, ctx->p, xctx);
                    use_default_processing = false;
                }
            }

            if (use_default_processing) {
                value = ctx->property_level.mapped_value;
                if (!value && (*pt)->default_value) {
                    value = afw_value_evaluate((*pt)->default_value,
                        result->p, xctx);
                }
            }

            if (!afw_value_is_nullish(value)) {
                afw_object_set_property(result,
                    (*pt)->property_name,
                    value, xctx);
            }
            else if ((*pt)->required) {
                afw_object_meta_add_error_fz(result, xctx,
                    "Missing property " AFW_UTF8_FMT_Q,
                    AFW_UTF8_FMT_ARG((*pt)->property_name));
            }
        }

        AFW_CATCH_UNHANDLED{
            afw_object_meta_add_thrown_property_error(result,
                (*pt)->property_name, -1, AFW_ERROR_THROWN,
                xctx);
        }

        AFW_ENDTRY;
    }

    afw_memory_clear(&ctx->property_level);
}



const afw_object_t *
impl_adapt_object_from_adaptor(
    afw_model_internal_adaptor_session_self_t *self,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_model_object_type_t *model_object_type,
    const afw_utf8_t *object_id,
    const afw_object_t *mapped_object,
    const afw_query_criteria_t *criteria,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_value_array_t *parent_paths;
    afw_model_internal_context_t *ctx;
    const afw_object_t *object;
    int top;

    /* Add a variable qualifier and remove it when finished. */
    top = afw_xctx_qualifier_stack_top_get(xctx);
    object = afw_object_create_and_cede_p(
        p, xctx);
    AFW_TRY {

        /* Prime context. */
        ctx = afw_model_internal_create_skeleton_context(
            &self->adaptor->
            instance_skeleton__AdaptiveModelCurrentOnGetProperty_,
            &afw_model_internal_context_current_property_from_mapped[0],
            self, impl_request, model_object_type, p, xctx);
        ctx->criteria = criteria;
        ctx->mapped_object = mapped_object;
        ctx->from_adaptor = true;

        /* Set ids in result object. */
        ctx->object_id = object_id;
        if (!object_id) {
            ctx->object_id = afw_object_meta_get_object_id(mapped_object, xctx);
        }
        afw_object_meta_set_ids(object,
            &self->adaptor->pub.adaptor_id,
            model_object_type->object_type_id,
            ctx->object_id, xctx);


        /** @fixme 
         * Need to think about mapping parent paths.  May not even be allowed or
         * must already be composite??
         *
         * There are other related "meta" mapping issues.
         */
        /* Set parent path in result object. */
        parent_paths = afw_object_meta_get_parent_paths_value(mapped_object,
            xctx);
        if (parent_paths) {
            afw_object_meta_set_parent_paths(object, parent_paths, xctx);
        }

        /* Add adapted properties to result object. */
        impl_add_adapted_properties_from_adaptor(object,
            model_object_type, mapped_object, ctx, xctx);
    }

    AFW_FINALLY{
        afw_xctx_qualifier_stack_top_set(top, xctx);
    }

    AFW_ENDTRY;

    return object;
}



/* Convert a property based on object_type and adapt_type. */
AFW_DEFINE_INTERNAL(void)
afw_model_internal_convert_property(
    const afw_model_object_type_t *object_type,
    afw_model_adapt_t adapt_type,
    const afw_utf8_t * *to_property_name,
    const afw_value_t * *to_value,
    const afw_utf8_t * from_property_name,
    const afw_value_t * from_value,
    const afw_object_t * const *variable_objects,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_model_property_type_t *property_type;

    /* Default mapped to unmapped and return if object_type NULL. */
    *to_property_name = from_property_name;
    *to_value = from_value;
    if (!object_type) return;

    /* Process based on adapt_type. */
    switch (adapt_type) {

    case afw_model_adapt_to_adaptor:
    case afw_model_adapt_to_requestor:
        impl_get_property_type_by_property_name(
            &property_type, to_property_name,
            object_type, from_property_name, xctx);
        if (!property_type) {
            goto error;
        }
        *to_value = afw_value_convert(from_value,
            property_type->data_type, false, p, xctx);
        break;

    case afw_model_adapt_from_adaptor:
    case afw_model_adapt_from_requestor:
        impl_get_property_type_by_mapped_property_name(
            &property_type, to_property_name,
            object_type, from_property_name, xctx);
        if (!property_type) {
            goto error;
        }
        *to_value = afw_value_convert(from_value,
            property_type->data_type, false, p, xctx);
        break;

    default:
        AFW_THROW_ERROR_Z(general, "Invalid adapt_type", xctx);
    }

    return;

error:
    AFW_THROW_ERROR_FZ(general, xctx,
        "Invalid property " AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(from_property_name));
}



/* Convert a property name based on object_type and adapt_type. */
AFW_DEFINE_INTERNAL(void)
afw_model_internal_convert_property_name(
    const afw_model_object_type_t *object_type,
    afw_model_adapt_t adapt_type,
    const afw_utf8_t * *to_property_name,
    const afw_utf8_t * from_property_name,
    const afw_object_t * const *variable_objects,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_model_property_type_t *property_type;

    /* Default mapped to unmapped and return if object_type NULL. */
    *to_property_name = from_property_name;
    if (!object_type) return;

    /* Process based on adapt_type. */
    switch (adapt_type) {

    case afw_model_adapt_to_adaptor:
    case afw_model_adapt_to_requestor:
        impl_get_property_type_by_property_name(
            &property_type, to_property_name,
            object_type, from_property_name, xctx);
        if (!property_type) goto error;
        break;

    case afw_model_adapt_from_adaptor:
    case afw_model_adapt_from_requestor:
        impl_get_property_type_by_mapped_property_name(
            &property_type, to_property_name,
            object_type, from_property_name, xctx);
        if (!property_type) goto error;
        break;

    default:
        AFW_THROW_ERROR_Z(general, "Invalid adapt_type", xctx);
    }

    return;

error:
    AFW_THROW_ERROR_FZ(general, xctx,
        "Invalid property " AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(from_property_name));

}


static const afw_utf8_t * const *
impl_convert_select(
    const afw_model_object_type_t *model_object_type,
    const afw_utf8_t * const *select,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t * *result;
    afw_size_t i;

    /* Return NULL if no select. */
    if (!select || !*select) {
        return NULL;
    }

    /* Return converted select. */
    for (i = 0; select[i]; i++);
    result = afw_pool_malloc(p, sizeof(const afw_utf8_t *) * i, xctx);
    result[i] = NULL;
    for (i = 0; select[i]; i++) {
        afw_model_internal_convert_property_name(
            model_object_type,
            afw_model_adapt_to_adaptor,
            &result[i],
            select[i],
            NULL /** @fixme include conf? */,
            p, xctx);
    }
    return result;
}


const afw_query_criteria_filter_entry_t *
impl_get_converted_entry(
    impl_convert_entry_wa_t *wa,
    const afw_query_criteria_filter_entry_t *old_entry)
{
    impl_filter_entry_t *entry;

    if (!old_entry || old_entry == (void *)1) {
        return old_entry;
    }

    for (entry = wa->anchor;
        entry && old_entry != entry->old_entry;
        entry = entry->next_entry);

    if (!entry) {
        entry = afw_pool_calloc_type(wa->p, impl_filter_entry_t, wa->xctx);
        entry->old_entry = old_entry;
        entry->next_entry = wa->anchor;
        wa->anchor = entry;
        entry->converted.next = impl_get_converted_entry(wa,
            old_entry->next);
        entry->converted.op_name = old_entry->op_name;
        entry->converted.op_id = old_entry->op_id;
        entry->converted.alt_not = old_entry->alt_not;
        entry->converted.alt_op_id = old_entry->alt_op_id;
        entry->converted.property_name = old_entry->property_name;
        entry->converted.value = old_entry->value;
        entry->converted.op_specific = old_entry->op_specific;

        entry->converted.first_conjunctive_child = impl_get_converted_entry(wa,
            old_entry->first_conjunctive_child);
        entry->converted.next_conjunctive_sibling = impl_get_converted_entry(wa,
            old_entry->next_conjunctive_sibling);
        entry->converted.on_true = impl_get_converted_entry(wa,
            old_entry->on_true);
        entry->converted.on_false = impl_get_converted_entry(wa,
            old_entry->on_false);

        /* If contains, just convert property name. */
        if (!entry->converted.value)
        {
            if (old_entry->property_name) {
                afw_model_internal_convert_property_name(
                    wa->model_object_type,
                    afw_model_adapt_to_adaptor,
                    &entry->converted.property_name,
                    old_entry->property_name,
                    NULL /** @fixme include conf? */,
                    wa->p, wa->xctx);
            }
        }

        /* Else, convert property name and value based on object type. */
        else {
            afw_model_internal_convert_property(
                wa->model_object_type,
                afw_model_adapt_to_adaptor,
                &entry->converted.property_name, &entry->converted.value,
                old_entry->property_name, old_entry->value,
                NULL /** @fixme include conf? */, wa->p, wa->xctx);
            if (afw_value_is_array(entry->converted.value)) {
                entry->converted.value = afw_value_one_and_only(
                    entry->converted.value,
                    wa->p, wa->xctx);
            }
        }
    }

    return &entry->converted;
}


static const afw_query_criteria_sort_entry_t *
impl_convert_first_sort(
    const afw_model_object_type_t *model_object_type,
    const afw_query_criteria_sort_entry_t *first_sort,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_query_criteria_sort_entry_t *from_entry;
    const afw_query_criteria_sort_entry_t *result = NULL;
    afw_query_criteria_sort_entry_t *new_entry;
    afw_query_criteria_sort_entry_t *prev_entry;

    /* Return NULL if no first_sort. */
    if (!first_sort) {
        return NULL;
    }

    /* Convert. */
    for (
        from_entry = first_sort,
        prev_entry = NULL;
        from_entry;
        prev_entry = new_entry,
        from_entry = from_entry->next)
    {
        new_entry = afw_pool_calloc_type(p,
            afw_query_criteria_sort_entry_t, xctx);
        if (prev_entry) {
            prev_entry->next = new_entry;
        }
        else {
            result = new_entry;
        }
        new_entry->descending = from_entry->descending;
        afw_model_internal_convert_property_name(
            model_object_type,
            afw_model_adapt_to_adaptor,
            &new_entry->property_name,
            from_entry->property_name,
            NULL /** @fixme include conf? */,
            p, xctx);
    }
    return result;
}



/*
 * Execute function for function thunk custom::mapBackObject
 * 
 * Like adaptive function: model_mapBackObject_signature
 *
 * Parameters:
 *     mappedObject - (object) This is the object to map back.
 *
 * Returns:
 *     (object) This is the mapped back object.
 */
const afw_value_t *
impl_execute_mapBackObject_thunk(
    const afw_value_function_thunk_t *thunk,
    afw_size_t argc, const afw_value_t *const *argv,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_model_internal_object_cb_context_t *cb_ctx = thunk->ctx;
    const afw_value_t *mapped_object_value;
    const afw_object_t *mapped_object;
    const afw_object_t *object;

    /** @fixme Temporary, remove */
    afw_function_execute_t modified_x;
    afw_function_execute_t *x;
    afw_memory_clear(&modified_x);
    modified_x.p = p;
    modified_x.xctx = xctx;
    modified_x.function = NULL;
    modified_x.argc = argc;
    modified_x.argv = argv;
    x = &modified_x;
    /*---------------------------------- */


    mapped_object = NULL;
    if (x->argc == 1) {
        mapped_object_value = afw_value_evaluate(argv[1], p, xctx);
        if (afw_value_is_object(mapped_object_value)) {
            mapped_object =
                ((const afw_value_object_t *)mapped_object_value)->internal;
        }
    }
    if (!mapped_object) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_FMT_Q " expects 1 object parameter",
            AFW_UTF8_FMT_ARG(thunk->name));
    }

    object = impl_adapt_object_from_adaptor(
        cb_ctx->session, cb_ctx->impl_request,
        cb_ctx->model_object_type, cb_ctx->object_id,
        mapped_object,
        cb_ctx->criteria,
        cb_ctx->p, xctx);

    return afw_value_create_object(object, p, xctx);
}



/*
 * Execute function for function thunk custom::returnObject
 *
 * Like adaptive function: model_returnObject_signature
 *
 * See afw_function_bindings.h for more information.
 *
 * Parameters:
 *     object - (object) This is the object to return.
 *     userData? - (binary) If this is present and true, the object will be
 *         mapped its mapped adaptor's object type to the model adaptor's
 *         object type.
 *
 * Returns:
 *     (boolean) This will return true if no more objects can be returned for
 *         any reason including a limit exceeded, connection closed, or server
 *         stopping.
 */
const afw_value_t *
impl_execute_returnObject_thunk(
    const afw_value_function_thunk_t *thunk,
    afw_size_t argc, const afw_value_t *const *argv,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_model_internal_object_cb_context_t *cb_ctx = thunk->ctx;
    const afw_value_t *object_value;
    const afw_object_t *object;
    const afw_value_t *mapback_value;
    afw_boolean_t abort;

    /** @fixme Temporary, remove */
    afw_function_execute_t modified_x;
    afw_function_execute_t *x;
    afw_memory_clear(&modified_x);
    modified_x.p = p;
    modified_x.xctx = xctx;
    modified_x.function = NULL;
    modified_x.argc = argc;
    modified_x.argv = argv;
    x = &modified_x;
    /*---------------------------------- */


    /* Can have at most 2 parameters. */
    object = NULL;
    if (x->argc > 2) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_FMT_Q " expects at most 2 parameters",
            AFW_UTF8_FMT_OPTIONAL_ARG(thunk->name));
    }

    if (x->argc >= 1) {
        object_value = afw_value_evaluate(argv[1], p, xctx);
        if (afw_value_is_object(object_value)) {
            object = ((const afw_value_object_t *)object_value)->internal;
        }
    }
    if (!object) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_FMT_Q " expects 1 object parameter",
            AFW_UTF8_FMT_OPTIONAL_ARG(thunk->name));
    }

    /* If second parameter is specified and true, mapback object. */
    if (x->argc == 2) {
        mapback_value = afw_value_evaluate(argv[2], p, xctx);
        if (mapback_value) {
            if (!afw_value_is_boolean(mapback_value)) {
                AFW_THROW_ERROR_FZ(general, xctx,
                    AFW_UTF8_FMT_Q " expects parameter 2 to be boolean",
                    AFW_UTF8_FMT_ARG(thunk->name));
            }
            if (((afw_value_boolean_t *)mapback_value)->internal) {
                object = impl_adapt_object_from_adaptor(
                    cb_ctx->session, cb_ctx->impl_request,
                    cb_ctx->model_object_type, cb_ctx->object_id,
                    object,
                    cb_ctx->criteria,
                    cb_ctx->p, xctx);
            }
        }
    }

    /* Call original callback. */
    abort = cb_ctx->original_callback(object, cb_ctx->original_context, xctx);

    return abort ? afw_value_true : afw_value_false;
}



/* Convert query criteria based on model. */
AFW_DEFINE_INTERNAL(const afw_query_criteria_t *)
afw_model_internal_convert_query_criteria(
    const afw_model_object_type_t *model_object_type,
    const afw_query_criteria_t *criteria,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_query_criteria_t *result;
    impl_convert_entry_wa_t wa;

    result = afw_pool_calloc_type(p, afw_query_criteria_t, xctx);

    wa.anchor = NULL;
    wa.model_object_type = model_object_type;
    wa.p = p;
    wa.xctx = xctx;
    result->filter = impl_get_converted_entry(&wa, criteria->filter);
    result->tree = impl_get_converted_entry(&wa, criteria->tree);

    result->select =
        impl_convert_select(model_object_type, criteria->select,
            p, xctx);

    result->first_sort =
        impl_convert_first_sort(model_object_type, criteria->first_sort,
            p, xctx);

    return result;
}


/* Create an instance of the adaptor. */
AFW_DEFINE(const afw_adaptor_t *)
afw_model_adaptor_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_model_internal_adaptor_self_t *self;
    afw_adaptor_t *adaptor;
    const afw_adaptor_t *model_location_adaptor;
    const afw_utf8_t *source_location;
    afw_runtime_object_indirect_t *obj;
    const afw_runtime_object_type_meta_t *meta;
    int i;
    
    /* Create adaptor and process common properties.  */
    adaptor = afw_adaptor_impl_create_cede_p(
            &impl_afw_adaptor_inf,
            sizeof(afw_model_internal_adaptor_self_t),
            properties, p, xctx);
    self = (afw_model_internal_adaptor_self_t *)adaptor;

    source_location = adaptor->impl->source_location;

    /* Get signature functions for current:: thunks. */
    self->mapBackObject_signature = (const afw_value_t *)
        afw_environment_get_function(
        &afw_s_model_mapBackObject_signature, xctx);
    self->mapObject_signature = (const afw_value_t *)
        afw_environment_get_function(
        &afw_s_model_mapObject_signature, xctx);
    self->returnObject_signature = (const afw_value_t *)
        afw_environment_get_function(
        &afw_s_model_returnObject_signature, xctx);

    /* Don't allow model to be updated view a model adaptor. */
    //afw_adaptor_impl_set_supported_core_object_type(adaptor,
    //    &afw_s__AdaptiveModelObjectType_, true, false, xctx);
    //afw_adaptor_impl_set_supported_core_object_type(adaptor,
    //    &afw_s__AdaptiveModelPropertyType_, false, false, xctx);

    /* Get modelLocationAdaptorId.  It can not be the same as adaptorId. */
    self->model_location_adaptor_id = afw_object_old_get_property_as_utf8(properties,
        &afw_s_modelLocationAdaptorId, p, xctx);
    if (afw_utf8_equal(self->model_location_adaptor_id, &adaptor->adaptor_id)) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_CONTEXTUAL_LABEL_FMT
            "modelLocationAdaptorId can not be the same as adaptorId",
            AFW_UTF8_FMT_ARG(source_location));
    }

    /* Prime on* instance skeletons */
    for (obj = &self->instance_skeleton[0], i = 0;
        i < afw_model_on_map_count;
        obj++, i++)
    {
        obj->pub.inf = afw_environment_get_runtime_object_map_inf(
            impl_on_object_type_ids[i], xctx);
        if (!obj->pub.inf) {
            AFW_THROW_ERROR_Z(general,
                "Internal error afw_model_adaptor_create_cede_p()",
                xctx);
        }
        meta = obj->pub.inf->rti.implementation_specific;
        obj->pub.meta.id = &afw_s_current;
        obj->pub.meta.object_type_uri = meta->object_type_id;
        obj->pub.meta.object_uri = &impl_on_paths[i];
    }

    /* Make sure model location adaptor is started and modelId is loaded. */
    model_location_adaptor = NULL;
    AFW_TRY {
        AFW_LOG_FZ(debug, xctx,
            "Adaptor " AFW_UTF8_FMT_Q
            " specified modelLocationAdaptorId " AFW_UTF8_FMT_Q
            ".",
            AFW_UTF8_FMT_ARG(&adaptor->adaptor_id),
            AFW_UTF8_FMT_ARG(self->model_location_adaptor_id));
        model_location_adaptor = afw_adaptor_get_reference(
            self->model_location_adaptor_id, xctx);
        if (!model_location_adaptor->impl || !model_location_adaptor->impl->model_location) {
            AFW_THROW_ERROR_FZ(general, xctx,
                AFW_UTF8_CONTEXTUAL_LABEL_FMT
                "the specified modelLocationAdaptorId " AFW_UTF8_FMT_Q
                " does not hold _AdaptiveModel_ objects",
                AFW_UTF8_FMT_ARG(source_location),
                AFW_UTF8_FMT_ARG(self->model_location_adaptor_id));
        }

        /** @fixme Load modelId */
        self->model_id = afw_object_old_get_property_as_utf8(properties,
            &afw_s_modelId, p, xctx);
    }

    AFW_FINALLY{
        if (model_location_adaptor) {
            afw_adaptor_release(model_location_adaptor, xctx);
        }
    }

    AFW_ENDTRY;

    /* mappedAdaptorId */
    self->mappedAdaptorId_value = afw_object_get_property(properties,
        &afw_s_mappedAdaptorId, xctx);
    if (!self->mappedAdaptorId_value) {
        afw_adaptor_impl_throw_property_required(adaptor,
        &afw_s_mappedAdaptorId, xctx);
    }
    if (!afw_value_is_string(self->mappedAdaptorId_value)) {
        afw_adaptor_impl_throw_property_invalid(adaptor,
            &afw_s_mappedAdaptorId, xctx);
    }
    self->mapped_adaptor_id =
        &((const afw_value_string_t *)self->mappedAdaptorId_value)->internal;
    if (afw_utf8_equal(self->mapped_adaptor_id, &self->pub.adaptor_id)) {
        afw_adaptor_impl_throw_property_invalid(adaptor,
            &afw_s_mappedAdaptorId, xctx);
    }

    /* Return adaptor. */
    return adaptor;
}


/*
 * Implementation of method create_adaptor_cede_p of interface afw_adaptor_factory.
 */
const afw_adaptor_t *
impl_afw_adaptor_factory_create_adaptor_cede_p (
    const afw_adaptor_factory_t * instance,
    const afw_object_t * properties,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    return afw_model_adaptor_create_cede_p(properties, p, xctx);
}


/*
 * Implementation of method destroy of interface afw_adaptor.
 */
void
impl_afw_adaptor_destroy (
    const afw_adaptor_t * instance,
    afw_xctx_t *xctx)
{
    /* Release pool. */
    afw_pool_release(instance->p, xctx);
}



/*
 * Implementation of method create_adaptor_session of interface afw_adaptor.
 */
const afw_adaptor_session_t *
impl_afw_adaptor_create_adaptor_session (
    const afw_adaptor_t * instance,
    afw_xctx_t *xctx)
{
    afw_model_internal_adaptor_self_t * self =
        (afw_model_internal_adaptor_self_t *)instance;
    afw_model_internal_adaptor_session_self_t *session;

    session = afw_xctx_calloc_type(afw_model_internal_adaptor_session_self_t,
        xctx);
    session->pub.inf = &impl_afw_adaptor_session_inf;
    session->pub.adaptor = instance;
    session->pub.p = xctx->p;
    session->adaptor = self;

    session->model_location_adaptor = afw_adaptor_get_reference(
            self->model_location_adaptor_id, xctx);
    if (!session->model_location_adaptor) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Model adaptor " AFW_UTF8_FMT_Q " is not available",
            AFW_UTF8_FMT_ARG(self->model_location_adaptor_id));
    }
    session->model = afw_model_location_get_model(
        session->model_location_adaptor, self->model_id, xctx);
    if (!session->model) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Model adaptor " AFW_UTF8_FMT_Q
            " model " AFW_UTF8_FMT_Q
            " not found",
            AFW_UTF8_FMT_ARG(self->model_location_adaptor_id),
            AFW_UTF8_FMT_ARG(self->model_id));
    }

    return (const afw_adaptor_session_t *)session;
}



/*
 * Implementation of method get_additional_metrics of interface afw_adaptor.
 */
const afw_object_t *
impl_afw_adaptor_get_additional_metrics (
    const afw_adaptor_t * instance,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    return NULL;
}



/*
 * Implementation of method destroy of interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_destroy (
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    afw_model_internal_adaptor_session_self_t * self = 
        (afw_model_internal_adaptor_session_self_t *)instance;

    if (self->model_location_adaptor) {
        afw_adaptor_release(self->model_location_adaptor, xctx);
    }
}


/* _AdaptiveObjectType_ callback */
static afw_boolean_t
impl_AdaptiveObjectType_cb(
    const void *value,
    void *context,
    afw_xctx_t *xctx)
{
    afw_model_internal_object_cb_context_t *ctx = context;
    const afw_model_object_type_t *object_type = value;
    const afw_object_t *object;

    object = (object_type) ? object_type->object_type_object : NULL;

    if (object && ctx->criteria) {
        if (!afw_query_criteria_test_object(object, ctx->criteria,
            ctx->p, xctx))
        {
            return false;
        }
    }

    return ctx->original_callback(object, ctx->original_context, xctx);
}



/* Map object callback. */
static afw_boolean_t
impl_model_object_cb(
    const afw_object_t *mapped_object,
    void *context,
    afw_xctx_t *xctx)
{
    afw_model_internal_object_cb_context_t *ctx = context;
    const afw_object_t *object;

    /* Process object from adaptor. */
    object = mapped_object;
    if (object) {
        object = impl_adapt_object_from_adaptor(
            ctx->session, ctx->impl_request,
            ctx->model_object_type, ctx->object_id,
            mapped_object,
            ctx->criteria, 
            ctx->p, xctx);
    }

    /* Callback with view or NULL.  Callback will release view.  */
    return ctx->original_callback(object, ctx->original_context, xctx);
}


/*
 * Implementation of method retrieve_objects for interface
 * afw_adaptor_session.
 */
void
impl_afw_adaptor_session_retrieve_objects(
    const afw_adaptor_session_t *instance,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_query_criteria_t *criteria,
    void *context,
    afw_object_cb_t callback,
    const afw_object_t *adaptor_type_specific,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_adaptor_session_self_t * self =
        (afw_model_internal_adaptor_session_self_t *)instance;
    afw_model_internal_adaptor_self_t * adaptor =
        (afw_model_internal_adaptor_self_t *)instance->adaptor;
    afw_model_internal_context_t *ctx;
    afw_model_internal_object_cb_context_t cb_ctx;
    apr_hash_index_t *hi;
    afw_model_object_type_t *model_object_type;
    afw_utf8_t id;
    afw_boolean_t use_default_processing;

    afw_memory_clear(&cb_ctx);
    cb_ctx.p = p;
    cb_ctx.session = self;
    cb_ctx.original_context = context;
    cb_ctx.original_callback = callback;
    cb_ctx.criteria = criteria;

    if (afw_utf8_equal(object_type_id, &afw_s__AdaptiveObjectType_)) {

        /* Process other object types and return. */
        for (hi = apr_hash_first(afw_pool_get_apr_pool(p),
            self->model->model_object_types);
            hi;
            hi = apr_hash_next(hi))
        {
            /** @todo make sure (apr_ssize_t *) doesn't cause loss of bits. */
            apr_hash_this(hi, (const void **)&id.s, (apr_ssize_t *)&id.len,
                (void **)&model_object_type);
            impl_AdaptiveObjectType_cb(model_object_type, &cb_ctx, xctx);
        }
        callback(NULL, context, xctx);
        return;
    }

    cb_ctx.model_object_type = afw_model_get_object_type(self->model,
        object_type_id, xctx);
    if (!cb_ctx.model_object_type) {
        callback(NULL, context, xctx);
        return;
    }

    use_default_processing = true;
    if (cb_ctx.model_object_type->onRetrieveObjects) {
        const afw_value_t *result;
        int top;

        use_default_processing = false;
        top = afw_xctx_qualifier_stack_top_get(xctx);
        AFW_TRY{

            /* Prime context for "onRetrieveObjects". */
            ctx = afw_model_internal_create_skeleton_context(
                &self->adaptor->
                instance_skeleton__AdaptiveModelCurrentOnRetrieveObjects_,
                &afw_model_internal_context_current_retrieve_objects[0],
                self, impl_request, cb_ctx.model_object_type, p, xctx);
            ctx->mapBackObject_value = afw_value_function_thunk_create(
                &afw_s_a_current_colon_colon_mapBackObject,
                adaptor->mapBackObject_signature,
                impl_execute_mapBackObject_thunk, &cb_ctx, p, xctx);
            ctx->returnObject_value = afw_value_function_thunk_create(
                &afw_s_a_current_colon_colon_returnObject,
                adaptor->returnObject_signature,
                impl_execute_returnObject_thunk, &cb_ctx, p, xctx);
            ctx->mapped_object_type_id_value =
                ctx->model_object_type->mapped_object_type_id_value;
            ctx->criteria = criteria;

            result = afw_value_evaluate(
                cb_ctx.model_object_type->onRetrieveObjects,
                p, xctx);
            if (result == ctx->useDefaultProcessing_value) {
                use_default_processing = true;
                break;
            }
            callback(NULL, context, xctx);
        }

        /* Always restore top for qualifier stack to entry value. */
            AFW_FINALLY{
                afw_xctx_qualifier_stack_top_set(top, xctx);
        }

        AFW_ENDTRY;
    }

    if (use_default_processing) {
        if (criteria) {
            cb_ctx.criteria = afw_model_internal_convert_query_criteria(
                cb_ctx.model_object_type, criteria, p, xctx);
        }

        afw_adaptor_retrieve_objects(self->adaptor->mapped_adaptor_id,
            cb_ctx.model_object_type->mapped_object_type_id,
            impl_request->options, cb_ctx.criteria,
            afw_object_create_managed(p, xctx),
            &cb_ctx, impl_model_object_cb,
            adaptor_type_specific,
            p, xctx);
    }
}



/*
 * Implementation of method get_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_get_object(
    const afw_adaptor_session_t *instance,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    void *context,
    afw_object_cb_t callback,
    const afw_object_t *adaptor_type_specific,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_adaptor_session_self_t * self =
        (afw_model_internal_adaptor_session_self_t *)instance;
    afw_model_internal_adaptor_self_t *adaptor =
        (afw_model_internal_adaptor_self_t *)instance->adaptor;
    afw_model_internal_context_t *ctx;
    afw_model_internal_object_cb_context_t cb_ctx;
    const afw_object_t *object;
    const afw_object_t *mapped_object;
    afw_boolean_t use_default_processing;

    afw_memory_clear(&cb_ctx);
    cb_ctx.p = p;
    cb_ctx.session = self;
    cb_ctx.original_context = context;
    cb_ctx.original_callback = callback;
 
    if (afw_utf8_equal(object_type_id, &afw_s__AdaptiveObjectType_)) {
        cb_ctx.model_object_type = afw_model_get_object_type(self->model,
            object_id, xctx);
        object = (cb_ctx.model_object_type)
            ? cb_ctx.model_object_type->object_type_object
            : NULL;
        callback(object, context, xctx);
        return;
    }

    cb_ctx.model_object_type = afw_model_get_object_type(self->model,
        object_type_id, xctx);
    cb_ctx.object_id = object_id;
    if (!cb_ctx.model_object_type) {
        callback(NULL, context, xctx);
        return;
    }

    /* Get object via onGet. */
    use_default_processing = true;
    if (cb_ctx.model_object_type->onGetObject) {
        const afw_value_t *object_value;
        const afw_object_t *object;
        int top;

        use_default_processing = false;
        top = afw_xctx_qualifier_stack_top_get(xctx);
        AFW_TRY{

            /* Prime context for onGetObject. */
            ctx = afw_model_internal_create_skeleton_context(
                &self->adaptor->
                instance_skeleton__AdaptiveModelCurrentOnGetObject_,
                &afw_model_internal_context_current_get_object[0],
                self, impl_request, cb_ctx.model_object_type, p, xctx);
            ctx->mapBackObject_value = afw_value_function_thunk_create(
                &afw_s_a_current_colon_colon_mapBackObject,
                adaptor->mapBackObject_signature,
                impl_execute_mapBackObject_thunk, &cb_ctx, p, xctx);
            ctx->mapped_object_type_id_value =
                ctx->model_object_type->mapped_object_type_id_value;
            ctx->object_id = object_id;

            object_value = afw_value_evaluate(
                cb_ctx.model_object_type->onGetObject,
                p, xctx);
            if (object_value == ctx->useDefaultProcessing_value) {
                use_default_processing = true;
                break;
            }
            if (afw_value_is_nullish(object_value)) {
                object = NULL;
            }
            else if (afw_value_is_object(object_value)) {
                object = ((const afw_value_object_t *)object_value)->internal;
            }
            else {
                AFW_THROW_ERROR_Z(general,
                    "onGetObject returned invalid value", xctx);
            }

            callback(object, context, xctx);
        }

        /* Always restore top for qualifier stack to entry value. */
        AFW_FINALLY{
            afw_xctx_qualifier_stack_top_set(top, xctx);
        }

        AFW_ENDTRY;
    }

    /* Get object and map.  */
    if (use_default_processing) {
        mapped_object = afw_adaptor_get_object(
            self->adaptor->mapped_adaptor_id,
            cb_ctx.model_object_type->mapped_object_type_id,
            object_id, NULL, NULL,
            afw_object_create_managed(p, xctx),
            adaptor_type_specific, p, xctx);
        cb_ctx.p = p;
        cb_ctx.session = self;
        cb_ctx.original_context = context;
        cb_ctx.original_callback = callback;
        impl_model_object_cb(mapped_object, &cb_ctx, xctx);
    }
}


/*
 * This is used by add and replace to create basic mapped object.  Each
 * does further appropriated processing on returned object.  ctx must be
 * initialized before call.
 */
AFW_DEFINE_INTERNAL(const afw_object_t *)
afw_model_internal_create_basic_to_adaptor_mapped_object(
    afw_model_internal_context_t *ctx,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p = ctx->p;
    const afw_object_t *result;
    const afw_utf8_t *property_name;
    const afw_utf8_t *mapped_property_name;
    const afw_value_t *value;
    const afw_value_t *mapped_value;
    const afw_model_property_type_t *pt;
    const afw_model_property_type_t * const *pts;
    const afw_iterator_t *iterator;

    /* */
    result = afw_object_create(p, xctx);

    /* Loop processing object. */
    iterator = NULL;
    while ((value = afw_object_get_next_property(ctx->object,
        &iterator, &property_name, xctx)))
    {
        impl_get_property_type_by_property_name(
            &pt, &mapped_property_name,
            ctx->model_object_type, property_name, xctx);

        if (!pt) {
            afw_object_meta_add_property_error_fz(
                ctx->impl_request->request,
                property_name, xctx,
                "Invalid property " AFW_UTF8_FMT_Q,
                AFW_UTF8_FMT_ARG(property_name));
        }
        else if (pt->allow_write)
        {
            mapped_value = value;
            if (pt->onSetProperty) {
                ctx->property_level.model_property_type = pt;
                ctx->property_level.value = value;
                mapped_value = afw_value_evaluate(
                    pt->onSetProperty, p, xctx);
                if (mapped_value == ctx->useDefaultProcessing_value) {
                    mapped_value = value;
                }
                ctx->property_level.model_property_type = NULL;
                ctx->property_level.value = NULL;
            }
            if (!afw_value_is_nullish(mapped_value)) {
                afw_object_set_property(result,
                    mapped_property_name, mapped_value, xctx);
            }
        }
    }

    /** @fixme chance loop to check required etc. */

    /* If processing initial values or general onset??? */
    if (ctx->process_initial_values) {
        pts = ctx->model_object_type->property_type;
        if (pts) for (; *pts; pts++) {
            pt = *pts;
            if (pt->onGetInitialValue &&
                !afw_object_has_property(result, pt->mapped_property_name,
                    xctx))
            {
                mapped_value = afw_value_evaluate(
                    pt->onGetInitialValue, p, xctx);
                afw_object_set_property(result,
                    pt->mapped_property_name, mapped_value,
                    xctx);
            }
        }
    }

    /** @fixme add code */

    return result;
}



AFW_DEFINE_INTERNAL(void)
afw_model_internal_complete_ctx_default_add_object(
    afw_model_internal_context_t *ctx,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;
    
    /* Create basic mapped object. */
    ctx->mapped_object =
        afw_model_internal_create_basic_to_adaptor_mapped_object(ctx, xctx);

    /* If onGetInitialObjectId, evaluate it. */
    if (ctx->model_object_type->onGetInitialObjectId) {
        ctx->current_variables =
            &afw_model_internal_context_current_for_initial_object_id[0];
        value = afw_value_evaluate(
            ctx->model_object_type->onGetInitialObjectId,
            xctx->p, xctx);
        ctx->mapped_object_id = afw_value_as_utf8(value, ctx->p, xctx);
    }
}



/*
 * Implementation of method add_object for interface afw_adaptor_session.
 */
const afw_utf8_t *
impl_afw_adaptor_session_add_object(
    const afw_adaptor_session_t *instance,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *suggested_object_id,
    const afw_object_t *object,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    afw_model_internal_adaptor_session_self_t * self =
        (afw_model_internal_adaptor_session_self_t *)instance;
    afw_model_internal_context_t *ctx;
    const afw_utf8_t *mapped_object_id;
    const afw_object_t *journal_entry;
    const afw_value_t *value;
    int top;
    afw_boolean_t use_default_processing;

    /* _AdaptiveObjectType_ objects are immutable in model adaptor. */
    if (afw_utf8_equal(object_type_id, &afw_s__AdaptiveObjectType_)) {
        AFW_OBJECT_ERROR_OBJECT_IMMUTABLE;
    }

    /* Save top for qualifier stack. */
    top = afw_xctx_qualifier_stack_top_get(xctx);
    mapped_object_id = NULL;
    AFW_TRY {

        /* Prime context for "to adaptor" (current property to mapped is superset). */
        ctx = afw_model_internal_create_to_adaptor_skeleton_context(
            self,
            &self->adaptor->instance_skeleton__AdaptiveModelCurrentOnAddObject_,
            self->model,
            impl_request,
            object_type_id,
            suggested_object_id,
            xctx);
        ctx->object = object;
        ctx->process_initial_values = true;

        /* If onAddObject, evaluate it. */
        use_default_processing = true;
        if (ctx->model_object_type->onAddObject) {
            use_default_processing = false;
            value = afw_value_evaluate(ctx->model_object_type->onAddObject,
                ctx->p, xctx);
            if (value == ctx->useDefaultProcessing_value) {
                use_default_processing = true;
            }
            else {
                if (!afw_value_is_string(value)) {
                    AFW_THROW_ERROR_Z(general, "onAddObject must return string", xctx);
                }
                mapped_object_id = &((const afw_value_string_t *)value)->internal;
            }
        }

        /* If no onAddObject or it returned undefined, do default processing. */
        if (use_default_processing) {
            afw_model_internal_complete_ctx_default_add_object(ctx, xctx);
            journal_entry = afw_object_create_managed(ctx->p, xctx);
            mapped_object_id = afw_adaptor_add_object(
                self->adaptor->mapped_adaptor_id,
                ctx->mapped_object_type_id,
                ctx->mapped_object_id,
                ctx->mapped_object,
                journal_entry,
                adaptor_type_specific,
                xctx);
        }

    }

    /* Always restore top for qualifier stack to entry value. */
    AFW_FINALLY{
        afw_xctx_qualifier_stack_top_set(top, xctx);
    }

    AFW_ENDTRY;

    /* Return object id. */
    return mapped_object_id;
}



AFW_DEFINE_INTERNAL(void)
afw_model_internal_complete_ctx_default_modify_object(
    afw_model_internal_context_t *ctx,
    afw_xctx_t *xctx)
{
    const afw_array_t *mapped_entry;
    const afw_adaptor_modify_entry_t * const * entry;
    const afw_object_path_property_name_entry_t *first_property_name_entry;
    const afw_utf8_t *mapped_property_name;
    const afw_model_property_type_t *model_property_type;
    const afw_value_t *value;

    /* Make a mapped entries list. */
    ctx->mapped_entries = afw_array_create_with_options(
        0, afw_data_type_array, ctx->p, xctx);

    /* Process all entries. */
    for (entry = ctx->modify_entries; *entry; entry++, mapped_entry++)
    {
        first_property_name_entry = ((*entry)->first_property_name_entry);
        if (!first_property_name_entry ||
            first_property_name_entry->next)
        {
            AFW_THROW_ERROR_Z(general,
                "Model adaptor only supports modify at the entity level",
                xctx);
        }

        /* Make a skeleton modify entry list and add it. */
        mapped_entry = afw_array_create_generic(ctx->p, xctx);
        value = afw_value_create_array(mapped_entry, ctx->p, xctx);
        afw_array_add_value(ctx->mapped_entries, value, xctx);

        /* Add type to entry. */
        if ((*entry)->type < 0 ||
            (*entry)->type >= afw_adaptor_modify_entry_type_invalid)
        {
            AFW_THROW_ERROR_FZ(general, xctx, "Invalid modify type %d",
                (*entry)->type);
        }
        afw_array_add_value(mapped_entry,
            afw_adaptor_modify_entry_type_value((*entry)->type), xctx);

        /* Add mapped name to entry. */
        impl_get_property_type_by_property_name(
            &model_property_type, &mapped_property_name, ctx->model_object_type,
            &(*entry)->first_property_name_entry->property_name, xctx);
        if (!model_property_type) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Property name " AFW_UTF8_FMT_Q " invalid",
                AFW_UTF8_FMT_ARG(&(*entry)->first_property_name_entry->property_name));
        }
        afw_array_add_value(mapped_entry,
            model_property_type->mapped_property_name_value, xctx);

        /* Add value if there is one. */
        /** @fixme Might should call to_mapped_value with single values from bag or list. */
        if ((*entry)->value) {
            value = (*entry)->value;
            if (model_property_type->onSetProperty) {
                ctx->property_level.model_property_type =
                    model_property_type;
                ctx->property_level.value = value;
                value = afw_value_evaluate(
                    model_property_type->onSetProperty,
                    ctx->p, xctx);
                afw_memory_clear(&ctx->property_level);
            }
            afw_array_add_value(mapped_entry, value, xctx);
        }
    }
}



/*
 * Implementation of method modify_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_modify_object(
    const afw_adaptor_session_t *instance,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_adaptor_modify_entry_t *const *entry,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    afw_model_internal_adaptor_session_self_t * self =
        (afw_model_internal_adaptor_session_self_t *)instance;
    afw_model_internal_context_t *ctx;
    const afw_value_t *value;
    const afw_object_t *journal_entry;
    const afw_array_t *entries_list;
    int top;
    afw_boolean_t use_default_processing;

    /* _AdaptiveObjectType_ objects are immutable in model adaptor. */
    if (afw_utf8_equal(object_type_id, &afw_s__AdaptiveObjectType_)) {
        AFW_OBJECT_ERROR_OBJECT_IMMUTABLE;
    }

    /* Save top for qualifier stack. */
    top = afw_xctx_qualifier_stack_top_get(xctx);
    AFW_TRY {

        /* Prime context for "to adaptor". */
        ctx = afw_model_internal_create_to_adaptor_skeleton_context(
            self,
            &self->adaptor->instance_skeleton__AdaptiveModelCurrentOnModifyObject_,
            self->model,
            impl_request,
            object_type_id,
            object_id,
            xctx);
        ctx->modify_entries = entry;

        /* If onModifyObject, evaluate it to modify object. */
        use_default_processing = true;
        if (ctx->model_object_type->onModifyObject) {
            use_default_processing = false;
            entries_list = afw_adaptor_modify_entries_to_list(entry,
                ctx->p, xctx);
            ctx->modify_entries_value = afw_value_create_array(entries_list,
                ctx->p, xctx);
            value = afw_value_evaluate(ctx->model_object_type->onModifyObject,
                ctx->p, xctx);
            if (value == ctx->useDefaultProcessing_value) {
                use_default_processing = true;
            }
        }

        /* If no onModifyObject or it returned undefined, do default processing. */
        if(use_default_processing) {
            afw_model_internal_complete_ctx_default_modify_object(ctx, xctx);
            journal_entry = afw_object_create_managed(ctx->p, xctx);
            afw_adaptor_modify_object(
                self->adaptor->mapped_adaptor_id,
                ctx->mapped_object_type_id,
                ctx->mapped_object_id,
                ctx->mapped_entries,
                journal_entry, adaptor_type_specific,
                xctx);
        }

        /** @fixme Deal with results in journal entry. */
    }

    /* Always restore top for qualifier stack to entry value. */
    AFW_FINALLY{
        afw_xctx_qualifier_stack_top_set(top, xctx);
    }

    AFW_ENDTRY;
}



AFW_DEFINE_INTERNAL(void)
afw_model_internal_complete_ctx_default_replace_object(
    afw_model_internal_context_t *ctx,
    afw_xctx_t *xctx)
{  
    /* Create basic mapped object. */
    ctx->mapped_object = afw_model_internal_create_basic_to_adaptor_mapped_object(ctx, xctx);
}



/*
 * Implementation of method replace_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_replace_object(
    const afw_adaptor_session_t *instance,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *replacement_object,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    afw_model_internal_adaptor_session_self_t * self =
        (afw_model_internal_adaptor_session_self_t *)instance;
    afw_model_internal_context_t *ctx;
    const afw_object_t *journal_entry;
    const afw_value_t *value;
    int top;
    afw_boolean_t use_default_processing;

    /* _AdaptiveObjectType_ objects are immutable in model adaptor. */
    if (afw_utf8_equal(object_type_id, &afw_s__AdaptiveObjectType_)) {
        AFW_OBJECT_ERROR_OBJECT_IMMUTABLE;
    }

    /* Save top for qualifier stack. */
    top = afw_xctx_qualifier_stack_top_get(xctx);
    AFW_TRY {

        /* Prime context for "to adaptor". */
        ctx = afw_model_internal_create_to_adaptor_skeleton_context(
            self,
            &self->adaptor->instance_skeleton__AdaptiveModelCurrentOnReplaceObject_,
            self->model,
            impl_request,
            object_type_id,
            object_id,
            xctx);
        ctx->object = replacement_object;

        /* If onReplaceObject, evaluate it. */
        use_default_processing = true;
        if (ctx->model_object_type->onReplaceObject) {
            use_default_processing = false;
            value = afw_value_evaluate(ctx->model_object_type->onReplaceObject,
                ctx->p, xctx);
            if (value == ctx->useDefaultProcessing_value) {
                use_default_processing = true;
            }
        }

        /* If no onReplaceObject or it returned undefined, do default processing. */
        if (use_default_processing) {
            journal_entry = afw_object_create_managed(ctx->p, xctx);
            afw_model_internal_complete_ctx_default_replace_object(ctx, xctx);
            afw_adaptor_replace_object(
                self->adaptor->mapped_adaptor_id,
                ctx->mapped_object_type_id,
                ctx->mapped_object_id,
                ctx->mapped_object,
                journal_entry, adaptor_type_specific,
                xctx);
        }
 
        /** @fixme Deal with results in journal entry. */
    }

    /* Always restore top for qualifier stack to entry value. */
    AFW_FINALLY{
        afw_xctx_qualifier_stack_top_set(top, xctx);
    }

    AFW_ENDTRY;
}



AFW_DEFINE_INTERNAL(void)
afw_model_internal_complete_ctx_default_delete_object(
    afw_model_internal_context_t *ctx,
    afw_xctx_t *xctx)
{  
    /* Nothing currently to do. */
}



/*
 * Implementation of method delete_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_delete_object(
    const afw_adaptor_session_t *instance,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    afw_model_internal_adaptor_session_self_t * self =
        (afw_model_internal_adaptor_session_self_t *)instance;
    afw_model_internal_context_t *ctx;
    const afw_object_t *journal_entry;
    const afw_value_t *value;
    int top;
    afw_boolean_t use_default_processing;

    /* _AdaptiveObjectType_ objects are immutable in model adaptor. */
    if (afw_utf8_equal(object_type_id, &afw_s__AdaptiveObjectType_)) {
        AFW_OBJECT_ERROR_OBJECT_IMMUTABLE;
    }

    /* Save top for qualifier stack. */
    top = afw_xctx_qualifier_stack_top_get(xctx);
    AFW_TRY {

        /* Prime context for "to adaptor". */
        ctx = afw_model_internal_create_to_adaptor_skeleton_context(
            self,
            &self->adaptor->instance_skeleton__AdaptiveModelCurrentOnDeleteObject_,
            self->model,
            impl_request,
            object_type_id,
            object_id,
            xctx);

        /* If onDeleteObject, evaluate it. */
        use_default_processing = true;
        if (ctx->model_object_type->onDeleteObject) {
            use_default_processing = false;
            value = afw_value_evaluate(ctx->model_object_type->onDeleteObject,
                ctx->p, xctx);
            if (value == ctx->useDefaultProcessing_value) {
                use_default_processing = true;
            }
        }

        /* If no onDeleteObjector or it returned undefined, do default processing. */
        if (use_default_processing) {
            journal_entry = afw_object_create(ctx->p, xctx);
            afw_model_internal_complete_ctx_default_delete_object(ctx, xctx);
            afw_adaptor_delete_object(self->adaptor->mapped_adaptor_id,
                ctx->mapped_object_type_id, ctx->mapped_object_id,
                journal_entry,  /*FIXME */ NULL, xctx);
        }
    }

    /* Always restore top for qualifier stack to entry value. */
    AFW_FINALLY{
        afw_xctx_qualifier_stack_top_set(top, xctx);
    }

    AFW_ENDTRY;
}



/*
 * Implementation of method begin_transaction of interface afw_adaptor_session.
 */
const afw_adaptor_transaction_t *
impl_afw_adaptor_session_begin_transaction (
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    afw_model_internal_adaptor_self_t * adaptor =
        (afw_model_internal_adaptor_self_t *)instance->adaptor;

    /* Get cached session with begin_transaction. Ignore result. */
    afw_adaptor_session_get_cached(adaptor->mapped_adaptor_id,
        true, xctx);

    /* Return NULL since cached mapped adaptor session handle cleanup. */
    return NULL;
}



/*
 * Implementation of method get_journal_interface of interface afw_adaptor_session.
 */
const afw_adaptor_journal_t *
impl_afw_adaptor_session_get_journal_interface (
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    /* There is no journal interface for model adaptor. */
    return NULL;
}



/*
 * Implementation of method get_key_value_interface of interface afw_adaptor_session.
 */
const afw_adaptor_key_value_t *
impl_afw_adaptor_session_get_key_value_interface (
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    /* There is no key_value interface for model adaptor. */
    return NULL;
}



/*
 * Implementation of method get_index_interface of interface afw_adaptor_session.
 */
const afw_adaptor_impl_index_t *
impl_afw_adaptor_session_get_index_interface (
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    /* There is no index interface for model adaptor. */
    return NULL;
}



/*
 * Implementation of method get_object_type_cache_interface for interface
 * afw_adaptor_session.
 */
const afw_adaptor_object_type_cache_t *
impl_afw_adaptor_session_get_object_type_cache_interface(
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    afw_model_internal_adaptor_session_self_t * self =
        (afw_model_internal_adaptor_session_self_t *)instance;

    afw_adaptor_impl_object_type_cache_initialize(
        &self->object_type_cache,
        &impl_afw_adaptor_object_type_cache_inf,
        instance, true, xctx);

    return &self->object_type_cache;
}



/*
 * Implementation of method get for interface afw_adaptor_object_type_cache.
 */
const afw_object_type_t *
impl_afw_adaptor_object_type_cache_get(
    const afw_adaptor_object_type_cache_t * instance,
    const afw_utf8_t * object_type_id,
    afw_boolean_t * final_result,
    afw_xctx_t *xctx)
{
    afw_model_internal_adaptor_session_self_t * session =
        (afw_model_internal_adaptor_session_self_t *)instance->session;
    const afw_object_type_t *result;
    afw_model_t *model = (afw_model_t *)session->model;

    *final_result = false;

    if (!model->object_types_ht) {
        return NULL;
    }

    AFW_ADAPTOR_IMPL_LOCK_READ_BEGIN(session->model_location_adaptor)
    {
        result = apr_hash_get(model->object_types_ht,
            object_type_id->s, object_type_id->len);
    }
    AFW_ADAPTOR_IMPL_LOCK_READ_END;

    return result;
}



/*
 * Implementation of method set for interface afw_adaptor_object_type_cache.
 */
void
impl_afw_adaptor_object_type_cache_set(
    const afw_adaptor_object_type_cache_t * instance,
    const afw_object_type_t * object_type,
    afw_xctx_t *xctx)
{
    afw_model_internal_adaptor_session_self_t * session =
        (afw_model_internal_adaptor_session_self_t *)instance->session;
    afw_model_t *model = (afw_model_t *)session->model;

    AFW_ADAPTOR_IMPL_LOCK_WRITE_BEGIN(session->model_location_adaptor)
    {

        if (!model->object_types_ht) {
            model->object_types_ht = apr_hash_make(
                afw_pool_get_apr_pool(session->adaptor->pub.p));
        }

        apr_hash_set(model->object_types_ht,
            object_type->object_type_id->s,
            object_type->object_type_id->len,
            object_type);
    }
    AFW_ADAPTOR_IMPL_LOCK_WRITE_END;
}
