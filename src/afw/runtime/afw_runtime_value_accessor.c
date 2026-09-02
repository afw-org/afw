// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Runtime Value Accessors
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_runtime_value_accessor.c
 * @brief Runtime object map value accessor implementations and registration.
 *
 * Each accessor has a co-located afw_runtime_value_accessor_info_t (key,
 * brief, description, lifetime flags) registered into the environment.
 */

#include "afw_internal.h"
#include "afw_runtime_value_accessor.h"


/* --------------------------------------------------------------------------
 * Environment register / get (registry value is the info struct)
 * ------------------------------------------------------------------------- */

AFW_DEFINE(void)
afw_environment_register_runtime_value_accessor(
    const afw_runtime_value_accessor_info_t *info,
    afw_xctx_t *xctx)
{
    if (!info || !info->key || !info->function) {
        AFW_THROW_ERROR_Z(general,
            "runtime value accessor info requires key and function",
            xctx);
    }

    afw_environment_registry_register(
        afw_environemnt_registry_type_runtime_value_accessor,
        info->key,
        info,
        xctx);
}


AFW_DEFINE(const afw_runtime_value_accessor_info_t *)
afw_environment_get_runtime_value_accessor_info(
    const afw_utf8_t *accessor_name,
    afw_xctx_t *xctx)
{
    return (const afw_runtime_value_accessor_info_t *)
        afw_environment_registry_get(
            afw_environemnt_registry_type_runtime_value_accessor,
            accessor_name,
            xctx);
}


AFW_DEFINE(afw_runtime_value_accessor_t)
afw_environment_get_runtime_value_accessor(
    const afw_utf8_t *accessor_name,
    afw_xctx_t *xctx)
{
    const afw_runtime_value_accessor_info_t *info;

    info = afw_environment_get_runtime_value_accessor_info(accessor_name, xctx);
    return info ? info->function : NULL;
}


/* --------------------------------------------------------------------------
 * Core accessors: info struct then function (contracts next to the code)
 * ------------------------------------------------------------------------- */

/* --- default ------------------------------------------------------------- */

static const afw_utf8_t
impl_brief_default =
    AFW_UTF8_LITERAL("Map a struct member as its Adaptive data type");

static const afw_utf8_t
impl_description_default =
    AFW_UTF8_LITERAL(
        "Default accessor. internal points at the member storage for "
        "prop->data_type. Builds an Adaptive value via afw_value_common_create. "
        "Returns NULL for NULL pointer cTypes and zero-length afw_utf8_t / "
        "afw_memory_t. The value may alias the live member storage "
        "(returnsLiveReference).");

static const afw_runtime_value_accessor_info_t
impl_info_default = {
    .key = afw_s_default,
    .function = afw_runtime_value_accessor_default,
    .brief = &impl_brief_default,
    .description = &impl_description_default,
    .copies_under_lock = false,
    .returns_live_reference = true
};

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


/* --- compile_type -------------------------------------------------------- */

static const afw_utf8_t
impl_brief_compile_type =
    AFW_UTF8_LITERAL("Map afw_compile_type_t to its name string");

static const afw_utf8_t
impl_description_compile_type =
    AFW_UTF8_LITERAL(
        "internal is a pointer to afw_compile_type_t (stored as afw_octet_t). "
        "Returns the permanent name string value for that compile type.");

static const afw_runtime_value_accessor_info_t
impl_info_compile_type = {
    .key = afw_s_compile_type,
    .function = afw_runtime_value_accessor_compile_type,
    .brief = &impl_brief_compile_type,
    .description = &impl_description_compile_type,
    .copies_under_lock = false,
    .returns_live_reference = false
};

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


/* --- data_type_id -------------------------------------------------------- */

static const afw_utf8_t
impl_brief_data_type_id =
    AFW_UTF8_LITERAL("Map const afw_data_type_t * to dataType id string");

static const afw_utf8_t
impl_description_data_type_id =
    AFW_UTF8_LITERAL(
        "internal is a pointer to const afw_data_type_t *. Returns a string "
        "value of data_type->data_type_id, or NULL if the pointer is NULL.");

static const afw_runtime_value_accessor_info_t
impl_info_data_type_id = {
    .key = afw_s_data_type_id,
    .function = afw_runtime_value_accessor_data_type_id,
    .brief = &impl_brief_data_type_id,
    .description = &impl_description_data_type_id,
    .copies_under_lock = false,
    .returns_live_reference = false
};

const afw_value_t *
afw_runtime_value_accessor_data_type_id(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_data_type_t *data_type;

    (void)prop;

    if (!internal) {
        return NULL;
    }
    data_type = *(const afw_data_type_t * const *)internal;
    if (!data_type) {
        return NULL;
    }
    return afw_value_create_unmanaged_string(&data_type->data_type_id, p, xctx);
}


/* --- indirect ------------------------------------------------------------ */

static const afw_utf8_t
impl_brief_indirect =
    AFW_UTF8_LITERAL("Like default, but member is a pointer to the value");

static const afw_utf8_t
impl_description_indirect =
    AFW_UTF8_LITERAL(
        "Like default, but internal points to a pointer to the storage used "
        "as the Adaptive value internal (e.g. afw_utf8_t ** instead of "
        "afw_utf8_t *). Result may alias that live storage.");

static const afw_runtime_value_accessor_info_t
impl_info_indirect = {
    .key = afw_s_indirect,
    .function = afw_runtime_value_accessor_indirect,
    .brief = &impl_brief_indirect,
    .description = &impl_description_indirect,
    .copies_under_lock = false,
    .returns_live_reference = true
};

const afw_value_t *
afw_runtime_value_accessor_indirect(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    if (!internal) return NULL;

    return afw_runtime_value_accessor_default(prop, *(void **)internal,
        p, xctx);
}


/* --- octet --------------------------------------------------------------- */

static const afw_utf8_t
impl_brief_octet =
    AFW_UTF8_LITERAL("Map afw_octet_t member as integer");

static const afw_utf8_t
impl_description_octet =
    AFW_UTF8_LITERAL(
        "internal points to an afw_octet_t. Returns an integer value in the "
        "caller pool (scalar copy).");

static const afw_runtime_value_accessor_info_t
impl_info_octet = {
    .key = afw_s_octet,
    .function = afw_runtime_value_accessor_octet,
    .brief = &impl_brief_octet,
    .description = &impl_description_octet,
    .copies_under_lock = false,
    .returns_live_reference = false
};

const afw_value_t *
afw_runtime_value_accessor_octet(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_integer_t integer;

    integer = (afw_integer_t)(*((afw_octet_t *)internal));
    return afw_value_create_unmanaged_integer(integer, p, xctx);
}


/* --- stopping_adapter_instances ------------------------------------------ */

static const afw_utf8_t
impl_brief_stopping_adapter_instances =
    AFW_UTF8_LITERAL("Snapshot stopping adapter instance reference counts");

static const afw_utf8_t
impl_description_stopping_adapter_instances =
    AFW_UTF8_LITERAL(
        "internal is adapter_id (afw_utf8_t **). Under adapter_id_anchor_lock, "
        "walks the stopping chain and copies each stopping instance's "
        "reference_count into an integer array allocated in p. Safe snapshot; "
        "does not return pointers into anchors.");

static const afw_runtime_value_accessor_info_t
impl_info_stopping_adapter_instances = {
    .key = afw_s_stopping_adapter_instances,
    .function = afw_runtime_value_accessor_stopping_adapter_instances,
    .brief = &impl_brief_stopping_adapter_instances,
    .description = &impl_description_stopping_adapter_instances,
    .copies_under_lock = true,
    .returns_live_reference = false
};

const afw_value_t *
afw_runtime_value_accessor_stopping_adapter_instances(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_adapter_id_anchor_t *anchor;
    const afw_adapter_id_anchor_t *stopping;
    afw_size_t count;
    const afw_value_t *result;
    afw_integer_t *entry;
    const afw_utf8_t *adapter_id;
    const afw_array_t *list;

    /* A copy is required since it may change by a different thread. */
    adapter_id = *(const afw_utf8_t **)internal;
    result = NULL;
    AFW_LOCK_BEGIN(xctx->env->adapter_id_anchor_lock) {

        anchor = afw_environment_get_adapter_id(adapter_id, xctx);
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
        list = afw_array_create_view_of_c_array(entry, false,
            afw_data_type_integer, count, p, xctx);
        result = afw_value_create_unmanaged_array(list, p, xctx);
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


/* --- stopping_authorization_handler_instances ---------------------------- */

static const afw_utf8_t
impl_brief_stopping_authorization_handler_instances =
    AFW_UTF8_LITERAL(
        "Snapshot stopping authorization handler instance reference counts");

static const afw_utf8_t
impl_description_stopping_authorization_handler_instances =
    AFW_UTF8_LITERAL(
        "internal is authorization_handler_id (afw_utf8_t **). Under the "
        "authorization handler id rw lock (write), walks the stopping chain "
        "and copies reference counts into an integer array in p.");

static const afw_runtime_value_accessor_info_t
impl_info_stopping_authorization_handler_instances = {
    .key = afw_s_stopping_authorization_handler_instances,
    .function =
        afw_runtime_value_accessor_stopping_authorization_handler_instances,
    .brief = &impl_brief_stopping_authorization_handler_instances,
    .description = &impl_description_stopping_authorization_handler_instances,
    .copies_under_lock = true,
    .returns_live_reference = false
};

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
        list = afw_array_create_view_of_c_array(entry, false,
            afw_data_type_integer, count, p, xctx);
        result = afw_value_create_unmanaged_array(list, p, xctx);
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


/* --- adapter_reference_count --------------------------------------------- */

static const afw_utf8_t
impl_brief_adapter_reference_count =
    AFW_UTF8_LITERAL("Snapshot adapter anchor reference_count under lock");

static const afw_utf8_t
impl_description_adapter_reference_count =
    AFW_UTF8_LITERAL(
        "internal points to afw_integer_t reference_count on an "
        "afw_adapter_id_anchor_t. Under adapter_id_anchor_lock, copies the "
        "integer into a value in p. Mutators of the anchor use the same lock.");

static const afw_runtime_value_accessor_info_t
impl_info_adapter_reference_count = {
    .key = afw_s_adapter_reference_count,
    .function = afw_runtime_value_accessor_adapter_reference_count,
    .brief = &impl_brief_adapter_reference_count,
    .description = &impl_description_adapter_reference_count,
    .copies_under_lock = true,
    .returns_live_reference = false
};

const afw_value_t *
afw_runtime_value_accessor_adapter_reference_count(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_integer_t integer;

    (void)prop;

    if (!internal) {
        return NULL;
    }

    AFW_LOCK_BEGIN(xctx->env->adapter_id_anchor_lock) {
        integer = *(const afw_integer_t *)internal;
    }
    AFW_LOCK_END;

    return afw_value_create_unmanaged_integer(integer, p, xctx);
}


/* --- authorization_handler_reference_count ------------------------------- */

static const afw_utf8_t
impl_brief_authorization_handler_reference_count =
    AFW_UTF8_LITERAL(
        "Snapshot authorization handler anchor reference_count under lock");

static const afw_utf8_t
impl_description_authorization_handler_reference_count =
    AFW_UTF8_LITERAL(
        "internal points to afw_integer_t reference_count on an "
        "afw_authorization_handler_id_anchor_t. Under "
        "authorization_handler_id_anchor_rw_lock (read), copies the integer "
        "into a value in p.");

static const afw_runtime_value_accessor_info_t
impl_info_authorization_handler_reference_count = {
    .key = afw_s_authorization_handler_reference_count,
    .function =
        afw_runtime_value_accessor_authorization_handler_reference_count,
    .brief = &impl_brief_authorization_handler_reference_count,
    .description = &impl_description_authorization_handler_reference_count,
    .copies_under_lock = true,
    .returns_live_reference = false
};

const afw_value_t *
afw_runtime_value_accessor_authorization_handler_reference_count(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_integer_t integer;

    (void)prop;

    if (!internal) {
        return NULL;
    }

    AFW_LOCK_READ_BEGIN(xctx->env->authorization_handler_id_anchor_rw_lock) {
        integer = *(const afw_integer_t *)internal;
    }
    AFW_LOCK_READ_END;

    return afw_value_create_unmanaged_integer(integer, p, xctx);
}


/* --- service_startup ----------------------------------------------------- */

static const afw_utf8_t
impl_brief_service_startup =
    AFW_UTF8_LITERAL("Map afw_service_startup_t to status string");

static const afw_utf8_t
impl_description_service_startup =
    AFW_UTF8_LITERAL(
        "internal points to afw_service_startup_t. Copies the enum and returns "
        "the corresponding permanent/utf8 name as a string value in p.");

static const afw_runtime_value_accessor_info_t
impl_info_service_startup = {
    .key = afw_s_service_startup,
    .function = afw_runtime_value_accessor_service_startup,
    .brief = &impl_brief_service_startup,
    .description = &impl_description_service_startup,
    .copies_under_lock = false,
    .returns_live_reference = false
};

const afw_value_t *
afw_runtime_value_accessor_service_startup(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_service_startup_t startup;
    const afw_utf8_t *s;

    memcpy(&startup, internal, sizeof(startup));
    s = afw_service_startup_as_utf8(startup);
    return afw_value_create_unmanaged_string(s, p, xctx);
}


/* --- service_status ------------------------------------------------------ */

static const afw_utf8_t
impl_brief_service_status =
    AFW_UTF8_LITERAL("Map afw_service_status_t to status string");

static const afw_utf8_t
impl_description_service_status =
    AFW_UTF8_LITERAL(
        "internal points to afw_service_status_t. Copies the enum and returns "
        "the corresponding name as a string value in p.");

static const afw_runtime_value_accessor_info_t
impl_info_service_status = {
    .key = afw_s_service_status,
    .function = afw_runtime_value_accessor_service_status,
    .brief = &impl_brief_service_status,
    .description = &impl_description_service_status,
    .copies_under_lock = false,
    .returns_live_reference = false
};

const afw_value_t *
afw_runtime_value_accessor_service_status(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_service_status_t status;
    const afw_utf8_t *s;

    memcpy(&status, internal, sizeof(status));
    s = afw_service_status_as_utf8(status);
    return afw_value_create_unmanaged_string(s, p, xctx);
}


/* --- size ---------------------------------------------------------------- */

static const afw_utf8_t
impl_brief_size =
    AFW_UTF8_LITERAL("Map afw_size_t member as integer");

static const afw_utf8_t
impl_description_size =
    AFW_UTF8_LITERAL(
        "internal points to an afw_size_t. Returns an integer value in the "
        "caller pool (scalar copy).");

static const afw_runtime_value_accessor_info_t
impl_info_size = {
    .key = afw_s_size,
    .function = afw_runtime_value_accessor_size,
    .brief = &impl_brief_size,
    .description = &impl_description_size,
    .copies_under_lock = false,
    .returns_live_reference = false
};

const afw_value_t *
afw_runtime_value_accessor_size(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_integer_t integer;

    integer = (afw_integer_t)(*((afw_size_t *)internal));
    return afw_value_create_unmanaged_integer(integer, p, xctx);
}


/* --- uint32 -------------------------------------------------------------- */

static const afw_utf8_t
impl_brief_uint32 =
    AFW_UTF8_LITERAL("Map afw_uint32_t member as integer");

static const afw_utf8_t
impl_description_uint32 =
    AFW_UTF8_LITERAL(
        "internal points to an afw_uint32_t. Returns an integer value in the "
        "caller pool (scalar copy).");

static const afw_runtime_value_accessor_info_t
impl_info_uint32 = {
    .key = afw_s_uint32,
    .function = afw_runtime_value_accessor_uint32,
    .brief = &impl_brief_uint32,
    .description = &impl_description_uint32,
    .copies_under_lock = false,
    .returns_live_reference = false
};

const afw_value_t *
afw_runtime_value_accessor_uint32(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_integer_t integer;

    integer = (afw_integer_t)(*((afw_uint32_t *)internal));
    return afw_value_create_unmanaged_integer(integer, p, xctx);
}


/* --- adapter live-object pin (metrics / properties) ---------------------- */

static void
impl_release_adapter_cleanup(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx)
{
    (void)data2;
    (void)p;

    if (data) {
        afw_adapter_release((const afw_adapter_t *)data, xctx);
    }
}

/*
 * Caller holds adapter_id_anchor_lock. Increment the instance's anchor
 * count so stop/replace drains instead of destroying while p still holds
 * a live metrics/properties object. Skip when p is the instance pool
 * (same lifetime — extra pin would release during destroy).
 */
static const afw_adapter_t *
impl_pin_adapter_for_pool_lock_held(
    const afw_adapter_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_adapter_id_anchor_t *anchor;

    if (!instance || p == instance->p) {
        return NULL;
    }

    for (
        anchor = (afw_adapter_id_anchor_t *)
            afw_environment_get_adapter_id(&instance->adapter_id, xctx);
        anchor;
        anchor = anchor->stopping)
    {
        if (anchor->adapter == instance) {
            anchor->reference_count++;
            return instance;
        }
    }

    return NULL;
}

static void
impl_register_adapter_pin_cleanup(
    const afw_adapter_t *held,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    if (!held) {
        return;
    }

    AFW_TRY {
        afw_pool_register_cleanup_before(
            p, (void *)held, NULL, impl_release_adapter_cleanup, xctx);
    }
    AFW_CATCH_UNHANDLED {
        afw_adapter_release(held, xctx);
        AFW_ERROR_RETHROW;
    }
    AFW_ENDTRY;
}


/* --- adapter_metrics ----------------------------------------------------- */

static const afw_utf8_t
impl_brief_adapter_metrics =
    AFW_UTF8_LITERAL(
        "Live adapter metrics object (pointer loaded under anchor lock)");

static const afw_utf8_t
impl_description_adapter_metrics =
    AFW_UTF8_LITERAL(
        "internal is a pointer to const afw_adapter_t * on an "
        "afw_adapter_id_anchor_t. Under adapter_id_anchor_lock, loads the "
        "active adapter and returns an object value wrapping "
        "adapter->impl->metrics_object without deep-copying metrics. NULL when "
        "no active adapter. The metrics object is live environment state "
        "(returnsLiveReference): counters may change while held. The accessor "
        "increments the instance reference count and releases it when p is "
        "cleaned up, so a concurrent stop drains instead of destroying the "
        "pool behind the object. Do not cache beyond the pool that produced "
        "the value.");

static const afw_runtime_value_accessor_info_t
impl_info_adapter_metrics = {
    .key = afw_s_adapter_metrics,
    .function = afw_runtime_value_accessor_adapter_metrics,
    .brief = &impl_brief_adapter_metrics,
    .description = &impl_description_adapter_metrics,
    .copies_under_lock = false,
    .returns_live_reference = true
};

const afw_value_t *
afw_runtime_value_accessor_adapter_metrics(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_adapter_t *adapter;
    const afw_adapter_t *held;
    const afw_object_t *metrics_object;

    (void)prop;

    if (!internal) {
        return NULL;
    }

    metrics_object = NULL;
    held = NULL;
    AFW_LOCK_BEGIN(xctx->env->adapter_id_anchor_lock) {
        adapter = *(const afw_adapter_t * const *)internal;
        if (adapter && adapter->impl) {
            metrics_object = adapter->impl->metrics_object;
            if (metrics_object) {
                held = impl_pin_adapter_for_pool_lock_held(
                    adapter, p, xctx);
            }
        }
    }
    AFW_LOCK_END;

    impl_register_adapter_pin_cleanup(held, p, xctx);

    return (metrics_object)
        ? afw_value_create_unmanaged_object(metrics_object, p, xctx)
        : NULL;
}


/* --- adapter_properties -------------------------------------------------- */

static const afw_utf8_t
impl_brief_adapter_properties =
    AFW_UTF8_LITERAL(
        "Live adapter anchor properties object (pointer under lock)");

static const afw_utf8_t
impl_description_adapter_properties =
    AFW_UTF8_LITERAL(
        "internal is a pointer to const afw_object_t * properties on an "
        "afw_adapter_id_anchor_t. Under adapter_id_anchor_lock, loads the "
        "properties pointer and returns an object value wrapping it without "
        "deep copy. NULL when no properties. Live environment state "
        "(returnsLiveReference). Same pin as adapter_metrics: the accessor "
        "increments the instance reference count and releases it when p is "
        "cleaned up, so a concurrent stop drains instead of destroying the "
        "pool behind the object. Typically absent on the active anchor after "
        "full stop.");

static const afw_runtime_value_accessor_info_t
impl_info_adapter_properties = {
    .key = afw_s_adapter_properties,
    .function = afw_runtime_value_accessor_adapter_properties,
    .brief = &impl_brief_adapter_properties,
    .description = &impl_description_adapter_properties,
    .copies_under_lock = false,
    .returns_live_reference = true
};

const afw_value_t *
afw_runtime_value_accessor_adapter_properties(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_adapter_id_anchor_t *anchor;
    const afw_adapter_t *adapter;
    const afw_adapter_t *held;
    const afw_object_t *properties;

    if (!internal) {
        return NULL;
    }

    properties = NULL;
    adapter = NULL;
    held = NULL;
    AFW_LOCK_BEGIN(xctx->env->adapter_id_anchor_lock) {
        properties = *(const afw_object_t * const *)internal;
        if (properties && prop &&
            prop->offset != (afw_size_t)-1)
        {
            anchor = (const afw_adapter_id_anchor_t *)(
                (const char *)internal - prop->offset);
            adapter = anchor->adapter;
            if (adapter) {
                held = impl_pin_adapter_for_pool_lock_held(
                    adapter, p, xctx);
            }
        }
    }
    AFW_LOCK_END;

    impl_register_adapter_pin_cleanup(held, p, xctx);

    return (properties)
        ? afw_value_create_unmanaged_object(properties, p, xctx)
        : NULL;
}


/* --- applicable_flags ---------------------------------------------------- */

static const afw_utf8_t
impl_brief_applicable_flags =
    AFW_UTF8_LITERAL("Build array of applicable flag ids for a flag");

static const afw_utf8_t
impl_description_applicable_flags =
    AFW_UTF8_LITERAL(
        "internal points at an afw_flag_t (typically offset of flag_id at "
        "start of struct). Builds a new array in p of flag id values for "
        "each applicable flag bit. Array contents are permanent flag id "
        "strings; the array itself is allocated in p.");

static const afw_runtime_value_accessor_info_t
impl_info_applicable_flags = {
    .key = afw_s_applicable_flags,
    .function = afw_runtime_value_accessor_applicable_flags,
    .brief = &impl_brief_applicable_flags,
    .description = &impl_description_applicable_flags,
    .copies_under_lock = false,
    .returns_live_reference = false
};

const afw_value_t *
afw_runtime_value_accessor_applicable_flags(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_flag_t *self = internal;
    const afw_array_t *list;
    const afw_flag_t *flag;
    afw_size_t i;

    list = afw_array_create_unmanaged(p, xctx);
    for (i = 0; i < self->applicable_flags_count_allocated; i++) {
        if (self->applicable_flags[i]) {
            flag = afw_flag_get_by_index(i, xctx);
            afw_array_push_value(list, flag->flag_id_value, xctx);
        }
    }

    return afw_value_create_unmanaged_array(list, p, xctx);
}


/* --- null_terminated_array_of_internal ----------------------------------- */

static const afw_utf8_t
impl_brief_null_terminated_array_of_internal =
    AFW_UTF8_LITERAL("Map NULL-terminated array of value internals");

static const afw_utf8_t
impl_description_null_terminated_array_of_internal =
    AFW_UTF8_LITERAL(
        "internal points to a pointer to a NULL-terminated C array of "
        "internals of the array dataTypeParameter type. Builds a view then "
        "clones into p. dataType must be array.");

static const afw_runtime_value_accessor_info_t
impl_info_null_terminated_array_of_internal = {
    .key = afw_s_null_terminated_array_of_internal,
    .function = afw_runtime_value_accessor_null_terminated_array_of_internal,
    .brief = &impl_brief_null_terminated_array_of_internal,
    .description = &impl_description_null_terminated_array_of_internal,
    .copies_under_lock = false,
    .returns_live_reference = false
};

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

    /* Must be data type array. */
    if (prop->data_type != afw_data_type_array) {
        AFW_THROW_ERROR_Z(general,
            "data type must be array for value accessor array_of_pointers.",
            xctx);
    }

    /* The dataTypeParameter is needed for runtime list. */
    if (!prop->data_type_parameter_data_type) {
        AFW_THROW_ERROR_Z(general, "array data type required", xctx);
    }

    /* Support for pointer to array of internals. */
    list = afw_array_create_view_of_c_array(*((const void * const *)internal),
        false, prop->data_type_parameter_data_type, -1, p, xctx);
    result = afw_value_create_unmanaged_array(list, p, xctx);
    result = afw_value_clone(result, p, xctx); /* Clone while locked. */

    return result;
}


/* --- null_terminated_array_of_objects ------------------------------------ */

static const afw_utf8_t
impl_brief_null_terminated_array_of_objects =
    AFW_UTF8_LITERAL("Map NULL-terminated array of object pointers");

static const afw_utf8_t
impl_description_null_terminated_array_of_objects =
    AFW_UTF8_LITERAL(
        "internal points to const afw_object_t * const * (NULL-terminated). "
        "Returns an array value of those objects. Object identities are live "
        "references.");

static const afw_runtime_value_accessor_info_t
impl_info_null_terminated_array_of_objects = {
    .key = afw_s_null_terminated_array_of_objects,
    .function = afw_runtime_value_accessor_null_terminated_array_of_objects,
    .brief = &impl_brief_null_terminated_array_of_objects,
    .description = &impl_description_null_terminated_array_of_objects,
    .copies_under_lock = false,
    .returns_live_reference = true
};

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

    return afw_value_create_unmanaged_array(list, p, xctx);
}


/* --- null_terminated_array_of_utf8_z_key_value_pair_objects -------------- */

static const afw_utf8_t
impl_brief_null_terminated_array_of_utf8_z_key_value_pair_objects =
    AFW_UTF8_LITERAL(
        "Map NULL-terminated utf8_z key/value pair tables to objects");

static const afw_utf8_t
impl_description_null_terminated_array_of_utf8_z_key_value_pair_objects =
    AFW_UTF8_LITERAL(
        "internal points to const afw_utf8_z_t *[] groups of key/value pairs "
        "(each object ends with NULL; list ends with extra NULL). Builds new "
        "objects and an array in p.");

static const afw_runtime_value_accessor_info_t
impl_info_null_terminated_array_of_utf8_z_key_value_pair_objects = {
    .key = afw_s_null_terminated_array_of_utf8_z_key_value_pair_objects,
    .function =
        afw_runtime_value_accessor_null_terminated_array_of_utf8_z_key_value_pair_objects,
    .brief = &impl_brief_null_terminated_array_of_utf8_z_key_value_pair_objects,
    .description =
        &impl_description_null_terminated_array_of_utf8_z_key_value_pair_objects,
    .copies_under_lock = false,
    .returns_live_reference = false
};

const afw_value_t *
afw_runtime_value_accessor_null_terminated_array_of_utf8_z_key_value_pair_objects(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_z_t * const *s_z;
    const afw_array_t *list;
    const afw_object_t *object;
    const afw_value_t *property_name;
    const afw_value_t *value;
    const afw_value_t *result;

    s_z = *(const afw_utf8_z_t * const **)internal;
    if (!s_z) {
        return NULL;
    }

    list = afw_array_create_unmanaged(p, xctx);
    result = afw_value_create_unmanaged_array(list, p, xctx);

    for (; *s_z; s_z++)
    {
        object = afw_object_create_unmanaged(p, xctx);
        for (; *s_z; s_z++) {
            property_name = afw_value_create_unmanaged_string(
                afw_utf8_create(*s_z, AFW_UTF8_Z_LEN, p, xctx), p, xctx);
            s_z++;
            if (!s_z) {
                AFW_THROW_ERROR_Z(general, "Unpaired key/value pair", xctx);
            }
            value = afw_value_create_string_from_u8z(*s_z, p, xctx);
            afw_object_set_property(object, property_name, value, xctx);
        }
        afw_array_push_value(list,
            afw_value_create_unmanaged_object(object, p, xctx), xctx);
    }

    return result;
}


/* --- null_terminated_array_of_pointers ----------------------------------- */

static const afw_utf8_t
impl_brief_null_terminated_array_of_pointers =
    AFW_UTF8_LITERAL("Map NULL-terminated array of pointers to values");

static const afw_utf8_t
impl_description_null_terminated_array_of_pointers =
    AFW_UTF8_LITERAL(
        "internal points to a pointer to a NULL-terminated array of pointers "
        "to internals of the array dataTypeParameter type. Builds a view then "
        "clones into p.");

static const afw_runtime_value_accessor_info_t
impl_info_null_terminated_array_of_pointers = {
    .key = afw_s_null_terminated_array_of_pointers,
    .function = afw_runtime_value_accessor_null_terminated_array_of_pointers,
    .brief = &impl_brief_null_terminated_array_of_pointers,
    .description = &impl_description_null_terminated_array_of_pointers,
    .copies_under_lock = false,
    .returns_live_reference = false
};

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

    /* Must be data type array. */
    if (prop->data_type != afw_data_type_array) {
        AFW_THROW_ERROR_Z(general,
            "data type must be array for value accessor array_of_pointers.",
            xctx);
    }

    /* The dataTypeParameter is needed for runtime list. */
    if (!prop->data_type_parameter_data_type) {
        AFW_THROW_ERROR_Z(general, "array data type parameter required", xctx);
    }

    /* Support for pointer to array of pointers. */
    list = afw_array_create_view_of_c_array(*((const void * const *)internal),
        true, prop->data_type_parameter_data_type, -1, p, xctx);
    result = afw_value_create_unmanaged_array(list, p, xctx);
    result = afw_value_clone(result, p, xctx); /* Clone while locked. */

    return result;
}


/* --- null_terminated_array_of_values ------------------------------------- */

static const afw_utf8_t
impl_brief_null_terminated_array_of_values =
    AFW_UTF8_LITERAL("Map NULL-terminated array of afw_value_t *");

static const afw_utf8_t
impl_description_null_terminated_array_of_values =
    AFW_UTF8_LITERAL(
        "internal points to a NULL-terminated array of const afw_value_t *. "
        "Returns an array value of those values (live value pointers).");

static const afw_runtime_value_accessor_info_t
impl_info_null_terminated_array_of_values = {
    .key = afw_s_null_terminated_array_of_values,
    .function = afw_runtime_value_accessor_null_terminated_array_of_values,
    .brief = &impl_brief_null_terminated_array_of_values,
    .description = &impl_description_null_terminated_array_of_values,
    .copies_under_lock = false,
    .returns_live_reference = true
};

const afw_value_t *
afw_runtime_value_accessor_null_terminated_array_of_values(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_value_t * const *values =  (const afw_value_t * const *)internal;
    const afw_array_t *list;

    list = afw_array_const_create_null_terminated_array_of_values(values, p, xctx);

    return afw_value_create_unmanaged_array(list, p, xctx);
}


/* --- adapter_additional_metrics ------------------------------------------ */

static const afw_utf8_t
impl_brief_adapter_additional_metrics =
    AFW_UTF8_LITERAL("Call adapter get_additional_metrics()");

static const afw_utf8_t
impl_description_adapter_additional_metrics =
    AFW_UTF8_LITERAL(
        "internal points at afw_adapter_impl_t. Calls "
        "afw_adapter_get_additional_metrics() with the adapter. Returned "
        "object lifetime follows that API (typically allocated in p).");

static const afw_runtime_value_accessor_info_t
impl_info_adapter_additional_metrics = {
    .key = afw_s_adapter_additional_metrics,
    .function = afw_runtime_value_accessor_adapter_additional_metrics,
    .brief = &impl_brief_adapter_additional_metrics,
    .description = &impl_description_adapter_additional_metrics,
    .copies_under_lock = false,
    .returns_live_reference = false
};

const afw_value_t *
afw_runtime_value_accessor_adapter_additional_metrics(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_adapter_impl_t *impl = internal;
    const afw_object_t *obj;

    obj = afw_adapter_get_additional_metrics(impl->adapter, p, xctx);

    return (obj)
        ? afw_value_create_unmanaged_object(obj, p, xctx)
        : NULL;
}


/* --- afw_components_extension_loaded (ensure load) ----------------------- */

static const afw_utf8_t
impl_brief_afw_components_extension_loaded =
    AFW_UTF8_LITERAL("Ensure afw_components extension is loaded");

static const afw_utf8_t
impl_description_afw_components_extension_loaded =
    AFW_UTF8_LITERAL(
        "Side-effect accessor: loads the afw_components extension if needed "
        "and returns boolean true. Key is afw_components_extension_loaded.");

static const afw_runtime_value_accessor_info_t
impl_info_afw_components_extension_loaded = {
    .key = afw_s_afw_components_extension_loaded,
    .function =
        afw_runtime_value_accessor_ensure_afw_components_extension_loaded,
    .brief = &impl_brief_afw_components_extension_loaded,
    .description = &impl_description_afw_components_extension_loaded,
    .copies_under_lock = false,
    .returns_live_reference = false
};

const afw_value_t *
afw_runtime_value_accessor_ensure_afw_components_extension_loaded(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_environment_load_extension(afw_s_afw_components, NULL, NULL, xctx);

    return afw_boolean_v_true;
}


/* --- value --------------------------------------------------------------- */

static const afw_utf8_t
impl_brief_value =
    AFW_UTF8_LITERAL("Return afw_value_t * member as-is");

static const afw_utf8_t
impl_description_value =
    AFW_UTF8_LITERAL(
        "internal points to an afw_value_t *. Returns that value pointer "
        "without cloning (returnsLiveReference). Lifetime is that of the "
        "stored value.");

static const afw_runtime_value_accessor_info_t
impl_info_value = {
    .key = afw_s_value,
    .function = afw_runtime_value_accessor_value,
    .brief = &impl_brief_value,
    .description = &impl_description_value,
    .copies_under_lock = false,
    .returns_live_reference = true
};

const afw_value_t *
afw_runtime_value_accessor_value(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx)
{
    return *(const afw_value_t **)internal;
}


/* --------------------------------------------------------------------------
 * Register all core accessors
 * ------------------------------------------------------------------------- */

static const afw_runtime_value_accessor_info_t * const
impl_core_value_accessor_infos[] = {
    &impl_info_default,
    &impl_info_compile_type,
    &impl_info_data_type_id,
    &impl_info_indirect,
    &impl_info_octet,
    &impl_info_stopping_adapter_instances,
    &impl_info_applicable_flags,
    &impl_info_stopping_authorization_handler_instances,
    &impl_info_adapter_reference_count,
    &impl_info_authorization_handler_reference_count,
    &impl_info_adapter_metrics,
    &impl_info_adapter_properties,
    &impl_info_null_terminated_array_of_internal,
    &impl_info_null_terminated_array_of_objects,
    &impl_info_null_terminated_array_of_utf8_z_key_value_pair_objects,
    &impl_info_null_terminated_array_of_pointers,
    &impl_info_null_terminated_array_of_values,
    &impl_info_size,
    &impl_info_service_startup,
    &impl_info_service_status,
    &impl_info_uint32,
    &impl_info_adapter_additional_metrics,
    &impl_info_afw_components_extension_loaded,
    &impl_info_value,
    NULL
};


/* Register core runtime value accessors. */
void afw_runtime_register_core_value_accessors(afw_xctx_t *xctx)
{
    const afw_runtime_value_accessor_info_t * const *infop;

    for (infop = impl_core_value_accessor_infos; *infop; infop++) {
        afw_environment_register_runtime_value_accessor(*infop, xctx);
    }
}
