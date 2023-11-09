// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_adaptor_factory interface for afw_runtime
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_runtime_value_accessor.c
 * @brief Implementation of afw_adaptor_factory interface for afw_runtime.
 */

#include "afw_internal.h"
#include "afw_runtime_value_accessor.h"


/* Register core runtime value accessors. */
void afw_runtime_register_core_value_accessors(afw_xctx_t *xctx)
{
    afw_environment_register_runtime_value_accessor(
        afw_s_default,
        afw_runtime_value_accessor_default,
        xctx
    );

    afw_environment_register_runtime_value_accessor(
        afw_s_compile_type,
        afw_runtime_value_accessor_compile_type,
        xctx
    );

    afw_environment_register_runtime_value_accessor(
        afw_s_indirect,
        afw_runtime_value_accessor_indirect,
        xctx
    );

    afw_environment_register_runtime_value_accessor(
        afw_s_octet,
        afw_runtime_value_accessor_octet,
        xctx
    );

    afw_environment_register_runtime_value_accessor(
        afw_s_stopping_adaptor_instances,
        afw_runtime_value_accessor_stopping_adaptor_instances,
        xctx
    );

    afw_environment_register_runtime_value_accessor(
        afw_s_applicable_flags,
        afw_runtime_value_accessor_applicable_flags,
        xctx
    );

    afw_environment_register_runtime_value_accessor(
        afw_s_stopping_authorization_handler_instances,
        afw_runtime_value_accessor_stopping_authorization_handler_instances,
        xctx
    );

    afw_environment_register_runtime_value_accessor(
        afw_s_adaptor_metrics,
        afw_runtime_value_accessor_adaptor_metrics,
        xctx
    );

    afw_environment_register_runtime_value_accessor(
        afw_s_null_terminated_array_of_internal,
        afw_runtime_value_accessor_null_terminated_array_of_internal,
        xctx
    );

    afw_environment_register_runtime_value_accessor(
        afw_s_null_terminated_array_of_objects,
        afw_runtime_value_accessor_null_terminated_array_of_objects,
        xctx
    );
    
    afw_environment_register_runtime_value_accessor(
        afw_s_null_terminated_array_of_utf8_z_key_value_pair_objects,
        afw_runtime_value_accessor_null_terminated_array_of_utf8_z_key_value_pair_objects,
        xctx
    );
    
    afw_environment_register_runtime_value_accessor(
        afw_s_null_terminated_array_of_pointers,
        afw_runtime_value_accessor_null_terminated_array_of_pointers,
        xctx
    );

    afw_environment_register_runtime_value_accessor(
        afw_s_null_terminated_array_of_values,
        afw_runtime_value_accessor_null_terminated_array_of_values,
        xctx
    );

    afw_environment_register_runtime_value_accessor(
        afw_s_size,
        afw_runtime_value_accessor_size,
        xctx
    );

    afw_environment_register_runtime_value_accessor(
        afw_s_service_startup,
        afw_runtime_value_accessor_service_startup,
        xctx
    );

    afw_environment_register_runtime_value_accessor(
        afw_s_service_status,
        afw_runtime_value_accessor_service_status,
        xctx
    );

    afw_environment_register_runtime_value_accessor(
        afw_s_uint32,
        afw_runtime_value_accessor_uint32,
        xctx
    );

    afw_environment_register_runtime_value_accessor(
        afw_s_adaptor_additional_metrics,
        afw_runtime_value_accessor_adaptor_additional_metrics,
        xctx
    );

    afw_environment_register_runtime_value_accessor(
        afw_s_afw_components_extension_loaded,
        afw_runtime_value_accessor_ensure_afw_components_extension_loaded,
        xctx
    );

    afw_environment_register_runtime_value_accessor(
        afw_s_value,
        afw_runtime_value_accessor_value,
        xctx
    );
}


/* Runtime value accessor for default internal. */
const afw_value_t *
afw_runtime_value_accessor_default(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_value_t *result;

    /* If internal is NULL, just return NULL. */
    if (!internal) {
        return NULL;
    }

    /* If pointer cType and pointer is NULL, return NULL. */
    if (prop->data_type->cType.s[prop->data_type->cType.len - 1] == '*'
        && !*(void **)internal)
    {
        result = NULL;
    }

    /* If cType afw_utf8_t len is 0, return NULL. */
    else if (afw_utf8_equal_utf8_z(&prop->data_type->cType, "afw_utf8_t")
        && ((afw_utf8_t *)internal)->len == 0)
    {
        result = NULL;
    }

    /* If cType afw_memory_t size is 0, return NULL. */
    else if (afw_utf8_equal_utf8_z(&prop->data_type->cType, "afw_memory_t")
        && ((afw_memory_t *)internal)->size == 0)
    {
        result = NULL;
    }

    /* If not a NULL, create an appropriate single value. */
    else {
        result = afw_value_common_create(internal, prop->data_type,
            p, xctx);
    }

    /* Return result. */
    return result;
}



/* Runtime value accessor 'compile_type'. */
const afw_value_t *
afw_runtime_value_accessor_compile_type(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_compile_type_t compile_type;
    const afw_value_t *result;

    compile_type = (afw_compile_type_t)(*((afw_octet_t *)internal));
    result = afw_compile_type_get_info(compile_type, xctx)->name_value;
    return result;   
}

 
 
/* Runtime value accessor for indirect internal. */
const afw_value_t *
afw_runtime_value_accessor_indirect(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    if (!internal) return NULL;

    return afw_runtime_value_accessor_default(prop, *(void **)internal,
        p, xctx);
}



/* Runtime value accessor for afw_octet_t as afw_integer_t. */
const afw_value_t *
afw_runtime_value_accessor_octet(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_integer_t integer;

    integer = (afw_integer_t)(*((afw_octet_t *)internal));
    return afw_value_create_integer(integer, p, xctx);
}


/* Runtime value accessor for stopping adaptor instances. */
const afw_value_t *
afw_runtime_value_accessor_stopping_adaptor_instances(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_adaptor_id_anchor_t *anchor;
    const afw_adaptor_id_anchor_t *stopping;
    afw_size_t count;
    const afw_value_t *result;
    afw_integer_t *entry;
    const afw_utf8_t *adaptor_id;
    const afw_array_t *list;

    /* A copy is required since it may change by a different thread. */
    adaptor_id = *(const afw_utf8_t **)internal;
    result = NULL;
    AFW_LOCK_BEGIN(xctx->env->adaptor_id_anchor_lock) {

        anchor = afw_environment_get_adaptor_id(adaptor_id, xctx);
        if (!anchor) {
            break;
        }

        for (
            stopping = anchor->stopping,
            count = 0;
            stopping;
            stopping = stopping->stopping,
            count++);

        if (count == 0) {
            break;
        }

        entry = afw_pool_malloc(p, count * sizeof(afw_integer_t), xctx);
        list = afw_array_create_wrapper_for_array(entry, false,
            afw_data_type_integer, count, p, xctx);
        result = afw_value_create_array(list, p, xctx);
        for (
            stopping = anchor->stopping,
            count = 0;
            stopping;
            stopping = stopping->stopping,
            entry++,
            count++)
        {
            *entry = stopping->reference_count;
        }
    }

    AFW_LOCK_END;

    return result;
}



/* Runtime value accessor for stopping authorization handler instances. */
const afw_value_t *
afw_runtime_value_accessor_stopping_authorization_handler_instances(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_authorization_handler_id_anchor_t *anchor;
    const afw_authorization_handler_id_anchor_t *stopping;
    afw_size_t count;
    const afw_value_t *result;
    afw_integer_t *entry;
    const afw_utf8_t *authorization_handler_id;
    const afw_array_t *list;

    /* A copy is required since it may change by a different thread. */
    authorization_handler_id = *(const afw_utf8_t **)internal;
    result = NULL;
    AFW_LOCK_WRITE_BEGIN(xctx->env->authorization_handler_id_anchor_rw_lock) {

        anchor = afw_environment_get_authorization_handler_id(
            authorization_handler_id,
            xctx);
        if (!anchor) {
            break;
        }

        for (
            stopping = anchor->stopping,
            count = 0;
            stopping;
            stopping = stopping->stopping,
            count++);

        if (count == 0) {
            break;
        }

        entry = afw_pool_malloc(p, count * sizeof(afw_integer_t), xctx);
        list = afw_array_create_wrapper_for_array(entry, false,
            afw_data_type_integer, count, p, xctx);
        result = afw_value_create_array(list, p, xctx);
        for (
            stopping = anchor->stopping,
            count = 0;
            stopping;
            stopping = stopping->stopping,
            entry++,
            count++)
        {
            *entry = stopping->reference_count;
        }
    }

    AFW_LOCK_WRITE_END;

    return result;
}



/* Runtime value accessor for afw_service_startup_t. */
const afw_value_t *
afw_runtime_value_accessor_service_startup(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_service_startup_t startup;
    const afw_utf8_t *s;

    memcpy(&startup, internal, sizeof(startup));
    s = afw_service_startup_as_utf8(startup);
    return afw_value_create_string(s, p, xctx);
}


/* Runtime value accessor for afw_service_status_t. */
const afw_value_t *
afw_runtime_value_accessor_service_status(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_service_status_t status;
    const afw_utf8_t *s;

    memcpy(&status, internal, sizeof(status));
    s = afw_service_status_as_utf8(status);
    return afw_value_create_string(s, p, xctx);
}


/* Runtime value accessor for afw_size_t as afw_integer_t. */
const afw_value_t *
afw_runtime_value_accessor_size(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_integer_t integer;

    integer = (afw_integer_t)(*((afw_size_t *)internal));
    return afw_value_create_integer(integer, p, xctx);
}


/* Runtime value accessor for afw_uint32_t as afw_integer_t. */
const afw_value_t *
afw_runtime_value_accessor_uint32(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_integer_t integer;

    integer = (afw_integer_t)(*((afw_uint32_t *)internal));
    return afw_value_create_integer(integer, p, xctx);
}


/* Runtime value accessor for /afw/_AdaptiveAdaptorMetrics_/<adaptorId>. */
const afw_value_t *
afw_runtime_value_accessor_adaptor_metrics(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_adaptor_t *adaptor = *(const afw_adaptor_t * const *)internal;
   
    return (adaptor)
        ? afw_value_create_object(adaptor->impl->metrics_object,
            p, xctx)
        : NULL;
}



/* Runtime value accessor to produce triggeredBy for a flag. */
const afw_value_t *
afw_runtime_value_accessor_applicable_flags(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_flag_t *self = internal;
    const afw_array_t *list;
    const afw_flag_t *flag;
    afw_size_t i;

    list = afw_array_create_generic(p, xctx);
    for (i = 0; i < self->applicable_flags_count_allocated; i++) {
        if (self->applicable_flags[i]) {
            flag = afw_flag_get_by_index(i, xctx);
            afw_array_add_value(list, flag->flag_id_value, xctx);
        }
    }

    return afw_value_create_array(list, p, xctx);
}



/* Runtime value accessor for NULL terminated array of pointers. */
const afw_value_t *
afw_runtime_value_accessor_null_terminated_array_of_internal(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_value_t *result;
    const afw_array_t *list;

    /* If internal is NULL, return NULL. */
    if (!internal || !*(void **)internal)
    {
        return NULL;
    }

    /* Must be data type list. */
    if (prop->data_type != afw_data_type_array) {
        AFW_THROW_ERROR_Z(general,
            "data type must be list for value accessor array_of_pointers.",
            xctx);
    }

    /* The dataTypeParameter is needed for runtime list. */
    if (!prop->data_type_parameter_data_type) {
        AFW_THROW_ERROR_Z(general, "list data type required", xctx);
    }

    /* Support for pointer to array of internals. */
    list = afw_array_create_wrapper_for_array(*((const void * const *)internal),
        false, prop->data_type_parameter_data_type, -1, p, xctx);
    result = afw_value_create_array(list, p, xctx);
    result = afw_value_clone(result, p, xctx); /* Clone while locked. */

    return result;
}


/* Runtime value accessor for NULL terminated list of objects. */
const afw_value_t *
afw_runtime_value_accessor_null_terminated_array_of_objects(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_object_t * const *objects =
        *(const afw_object_t * const * const *)internal;
    const afw_array_t *list;

    list = afw_array_const_create_null_terminated_array_of_objects(objects,
        p, xctx);

    return afw_value_create_array(list, p, xctx);
}


/*
 * Runtime value accessor for NULL terminated list of utf8_z key/value pair
 * objects.
 */
const afw_value_t *
afw_runtime_value_accessor_null_terminated_array_of_utf8_z_key_value_pair_objects(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_z_t * const *s_z;
    const afw_array_t *list;
    const afw_object_t *object;
    const afw_utf8_t *property_name;
    const afw_value_t *value;
    const afw_value_t *result;

    s_z = *(const afw_utf8_z_t * const **)internal;
    if (!s_z) {
        return NULL;
    }

    list = afw_array_create_generic(p, xctx);
    result = afw_value_create_array(list, p, xctx);

    for (; *s_z; s_z++)
    {
        object = afw_object_create(p, xctx);
        for (; *s_z; s_z++) {
            property_name = afw_utf8_create(*s_z, AFW_UTF8_Z_LEN, p, xctx);
            s_z++;
            if (!s_z) {
                AFW_THROW_ERROR_Z(general, "Unpaired key/value pair", xctx);
            }
            value = afw_value_create_string_from_u8z(*s_z, p, xctx);
            afw_object_set_property(object, property_name, value, xctx);
        }
        afw_array_add_value(list,
            afw_value_create_object(object, p, xctx), xctx);
    }

    return result;
}


/* Runtime value accessor for NULL terminated array of pointers. */
const afw_value_t *
afw_runtime_value_accessor_null_terminated_array_of_pointers(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_value_t *result;
    const afw_array_t *list;

    /* If internal is NULL, return NULL. */
    if (!internal || !*(void **)internal)
    {
        return NULL;
    }

    /* Must be data type list. */
    if (prop->data_type != afw_data_type_array) {
        AFW_THROW_ERROR_Z(general,
            "data type must be list for value accessor array_of_pointers.",
            xctx);
    }

    /* The dataTypeParameter is needed for runtime list. */
    if (!prop->data_type_parameter_data_type) {
        AFW_THROW_ERROR_Z(general, "list data type required", xctx);
    }

    /* Support for pointer to array of pointers. */
    list = afw_array_create_wrapper_for_array(*((const void * const *)internal),
        true, prop->data_type_parameter_data_type, -1, p, xctx);
    result = afw_value_create_array(list, p, xctx);
    result = afw_value_clone(result, p, xctx); /* Clone while locked. */

    return result;
}



/* Runtime value accessor for NULL terminated array of values. */
const afw_value_t *
afw_runtime_value_accessor_null_terminated_array_of_values(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_value_t * const *values =  (const afw_value_t * const *)internal;
    const afw_array_t *list;

    list = afw_array_const_create_null_terminated_array_of_values(values, p, xctx);

    return afw_value_create_array(list, p, xctx);
}


/* Runtime value accessor to call afw_adaptor_get_additional_metrics(). */
const afw_value_t *
afw_runtime_value_accessor_adaptor_additional_metrics(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_adaptor_impl_t *impl = internal;
    const afw_object_t *obj;

    obj = afw_adaptor_get_additional_metrics(impl->adaptor, p, xctx);
   
    return (obj)
        ? afw_value_create_object(obj, p, xctx)
        : NULL;
}

const afw_value_t *
afw_runtime_value_accessor_ensure_afw_components_extension_loaded(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_environment_load_extension(afw_s_afw_components, NULL, NULL, xctx);
    
    return afw_value_true;
}



/* Runtime value accessor for an afw_value_t. */
const afw_value_t *
afw_runtime_value_accessor_value(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    return *(const afw_value_t **)internal;
}
