// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_adaptor_factory interface for afw_runtime
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_runtime.c
 * @brief Implementation of afw_adaptor_factory interface for afw_runtime.
 */

#include "afw_internal.h"



/* Declares and rti/inf defines for interface afw_adaptor_factory */
#define AFW_IMPLEMENTATION_ID "afw_runtime"
#include "afw_adaptor_factory_impl_declares.h"
#include "afw_adaptor_impl_declares.h"
#include "afw_adaptor_session_impl_declares.h"

typedef struct {
    void *always_NULL;
    afw_runtime_object_wrapper_p_cb_t cb;
    void *data;
} impl_ht_object_cb_entry;


typedef struct {
    union {
        const afw_object_t object;
        impl_ht_object_cb_entry cb_entry;
    };
} impl_ht_object_entry;


static const afw_object_t *
impl_entry_to_object(
    const impl_ht_object_entry *entry,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;

    if (!entry) {
        result = NULL;
    }

    else if ((entry)->cb_entry.always_NULL == NULL) {
        result = entry->cb_entry.cb(entry->cb_entry.data, xctx->p, xctx);
    }

    else {
        result = (const afw_object_t *)entry;
    }

    return result;
}


static const afw_object_t *
impl_get_object(
    apr_hash_t *ht, const void *key, apr_ssize_t klen,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;
    const impl_ht_object_entry *entry;

    entry =  apr_hash_get(ht, key, klen);

    result = impl_entry_to_object(entry, xctx);

    return result;
}


static const afw_runtime_object_type_meta_t
impl_runtime_meta_const_embedded_untyped_object = {
    NULL,
    NULL,
    offsetof(afw_runtime_const_object_instance_t, properties),
    false
};

AFW_RUNTIME_OBJECT_INF(
    afw_runtime_inf_const_embedded_untyped_object,
    impl_runtime_meta_const_embedded_untyped_object);


struct afw_runtime_objects_s {
    apr_hash_t *types_ht;
};

typedef struct impl_afw_adaptor_self_s {
    afw_adaptor_t pub;

    /* Private implementation variables */

} impl_afw_adaptor_self_t;

typedef struct impl_afw_adaptor_session_self_s {
    afw_adaptor_session_t pub;

    /* Private implementation variables */

    afw_adaptor_object_type_cache_t object_type_cache;

} impl_afw_adaptor_session_self_t;

static const afw_object_t *
impl_resolve_untyped_object(
    const afw_runtime_unresolved_const_embedded_untyped_object_t *unresolved,
    const afw_object_t *embedding_object,
    const afw_utf8_t *property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

static const afw_array_t *
impl_resolve_list(
    const afw_runtime_unresolved_const_array_t *unresolved,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

static const afw_runtime_property_t *
impl_resolve_property(
    const afw_runtime_unresolved_property_t *unresolved,
    const afw_object_t *embedding_object,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

static const afw_runtime_property_t * const *
impl_resolve_properties(
    const afw_runtime_unresolved_property_t * const *properties,
    const afw_object_t *embedding_object,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


static const afw_utf8_t impl_factory_description =
AFW_UTF8_LITERAL("Adaptor type for accessing runtime objects.");

static const afw_adaptor_factory_t
impl_factory =
{
    &impl_afw_adaptor_factory_inf,
    AFW_UTF8_LITERAL("afw_runtime"),
    &impl_factory_description
};


static void
impl_set_entry(
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const impl_ht_object_entry *entry,
    afw_boolean_t overwrite, 
    afw_xctx_t *xctx)
{
    afw_runtime_objects_t *runtime_objects;
    apr_hash_t *ht;
    apr_hash_t *test_ht;
    const afw_pool_t *p;
    afw_xctx_t *env_xctx;

    p = xctx->env->p;

    for (env_xctx = xctx; env_xctx->parent; env_xctx = env_xctx->parent);
    runtime_objects = (afw_runtime_objects_t *)env_xctx->runtime_objects;

    ht = NULL;   

    if (!runtime_objects) {
        runtime_objects = afw_pool_calloc_type(p, afw_runtime_objects_t, xctx);
        env_xctx->runtime_objects = runtime_objects;
    }

    if (!runtime_objects->types_ht) {
        runtime_objects->types_ht = apr_hash_make(afw_pool_get_apr_pool(p));
    }

    else {
        ht = apr_hash_get(runtime_objects->types_ht,
            object_type_id->s, object_type_id->len);
    }

    if (!ht) {
        ht = apr_hash_make(afw_pool_get_apr_pool(p));
        apr_hash_set(runtime_objects->types_ht,
            object_type_id->s, object_type_id->len, ht);
    }

    if (!overwrite) {
        test_ht = apr_hash_get(ht, object_id->s, object_id->len);
        if (test_ht) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Runtime object /afw/"
                AFW_UTF8_FMT "/" AFW_UTF8_FMT " already set",
                AFW_UTF8_FMT_ARG(object_type_id),
                AFW_UTF8_FMT_ARG(object_id));
        }
    }
    apr_hash_set(ht, object_id->s, object_id->len, entry);
}



/* Set an object pointer in the environment's runtime objects. */
AFW_DEFINE(void)
afw_runtime_env_set_object(
    const afw_object_t *object, afw_boolean_t overwrite, afw_xctx_t *xctx)
{
    const afw_utf8_t *object_id;
    const afw_utf8_t *object_type_id;

    object_id = afw_object_meta_get_object_id(object, xctx);
    object_type_id = afw_object_meta_get_object_type_id(object, xctx);

    impl_set_entry(object_type_id, object_id,
        (const impl_ht_object_entry *)object, overwrite, xctx);

    afw_object_get_reference(object, xctx);
}


/* Set a list of object pointers in the environment's runtime objects. */
AFW_DEFINE(void)
afw_runtime_env_set_objects(
    const afw_object_t * const * objects, afw_boolean_t overwrite,
    afw_xctx_t *xctx)
{
    for (; *objects; objects++) {
        afw_runtime_env_set_object(*objects, overwrite, xctx);
    }
}


/* Set environment object accessed via callback. */
AFW_DEFINE(void)
afw_runtime_env_set_object_cb_wrapper(
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    afw_runtime_object_wrapper_p_cb_t callback,
    void *data,
    afw_boolean_t overwrite,
    afw_xctx_t *xctx)
{
    impl_ht_object_cb_entry *entry;

    entry = afw_pool_calloc_type(xctx->env->p,
        impl_ht_object_cb_entry, xctx);
    entry->cb = callback;
    entry->data = data;

    impl_set_entry(object_type_id, object_id,
        (const impl_ht_object_entry *)entry, overwrite, xctx);
}



AFW_DEFINE(void)
afw_runtime_remove_object(
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    afw_xctx_t *xctx)
{
    const afw_object_t *object;
    const afw_xctx_t *c;
    apr_hash_t *ht;

    object = NULL;
    for (c = xctx; c; c = c->parent) {
        if (c->runtime_objects && c->runtime_objects->types_ht) {
            ht = apr_hash_get(c->runtime_objects->types_ht,
                object_type_id->s, object_type_id->len);
            if (ht) {
                object = impl_get_object(ht, object_id->s, object_id->len,
                    xctx);
                if (object) {
                    apr_hash_set(ht, object_id->s, object_id->len, NULL);
                    afw_object_release(object, xctx);
                    break;
                }
            }
        }
    }
}


/* Set an object pointer in the xctx's runtime objects. */
AFW_DEFINE(void)
afw_runtime_xctx_set_object(
    const afw_object_t *object, afw_boolean_t overwrite,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *id;
    const afw_utf8_t *type;
    afw_runtime_objects_t *runtime_objects;
    apr_hash_t *ht;
    apr_hash_t *test_ht;
    apr_pool_t *p;

    p = afw_pool_get_apr_pool(xctx->p);
    id = afw_object_meta_get_object_id(object, xctx);
    type = afw_object_meta_get_object_type_id(object, xctx);

    ht = NULL;
    runtime_objects = (afw_runtime_objects_t *)xctx->runtime_objects;

    if (!runtime_objects) {
        runtime_objects = afw_xctx_calloc_type(afw_runtime_objects_t, xctx);
        xctx->runtime_objects = runtime_objects;
    }

    if (!runtime_objects->types_ht) {
        runtime_objects->types_ht = apr_hash_make(p);
    }

    else {
        ht = apr_hash_get(runtime_objects->types_ht, type->s, type->len);
    }

    if (!ht) {
        ht = apr_hash_make(afw_pool_get_apr_pool(xctx->p));
        apr_hash_set(runtime_objects->types_ht, type->s, type->len, ht);
    }

    if (!overwrite) {
        test_ht = apr_hash_get(ht, id->s, id->len);
        if (test_ht) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Runtime object /afw/"
                AFW_UTF8_FMT "/" AFW_UTF8_FMT " already set",
                AFW_UTF8_FMT_ARG(type), AFW_UTF8_FMT_ARG(id));
        }
    }
    afw_object_get_reference(object, xctx);
    apr_hash_set(ht, id->s, id->len, object);
}


/* Set a list of object pointers in the xctx's runtime objects. */
AFW_DEFINE(void)
afw_runtime_xctx_set_objects(
    const afw_object_t * const * objects, afw_boolean_t overwrite,
    afw_xctx_t *xctx)
{
    for (; *objects; objects++) {
        afw_runtime_xctx_set_object(*objects, overwrite, xctx);
    }
}


/* Register runtime object map interfaces. */
AFW_DEFINE(void)
afw_runtime_register_object_map_infs(
    const afw_object_inf_t * const * inf,
    afw_xctx_t *xctx)
{
    const afw_runtime_object_type_meta_t *meta;

    /* Register all maps interfaces. */
    for (; *inf; inf++) {
        meta = (*inf)->rti.implementation_specific;
        afw_environment_register_runtime_object_map_inf(
            meta->object_type_id, *inf, xctx);
    }
}


/* Create and register runtime object map interfaces. */
AFW_DEFINE(void)
afw_runtime_resolve_and_register_object_map_infs(
    const afw_interface_implementation_rti_t * const * rti,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p = xctx->env->p;
    afw_object_inf_t *inf;
    const afw_runtime_object_type_meta_t *unresolved_meta;
    afw_runtime_object_type_meta_t *meta;
    afw_runtime_object_map_t *map;
    const afw_runtime_object_map_property_t *unresolved_properties;
    afw_runtime_object_map_property_t *properties;
    afw_size_t count;

    /* Register all map interfaces. */
    for (; *rti; rti++) {

        /* Model inf after afw_runtime_inf_const_embedded_untyped_object */
        inf = afw_pool_calloc_type(p, afw_object_inf_t, xctx);
        afw_memory_copy(inf, &afw_runtime_inf_const_embedded_untyped_object);
        afw_memory_copy(&inf->rti, *rti);

        /* Resolve meta. */
        unresolved_meta = (*rti)->implementation_specific;
        meta = afw_pool_malloc_type(p, afw_runtime_object_type_meta_t,
            xctx);
        inf->rti.implementation_specific = meta;
        afw_memory_copy(meta, unresolved_meta);

        /* Resolve object map. */
        if (meta->property_map && meta->property_map->property_count > 0) {
            map = afw_pool_calloc_type(p, afw_runtime_object_map_t, xctx);
            memcpy(map, meta->property_map, sizeof(afw_runtime_object_map_t));
            meta->property_map = map;

            unresolved_properties = map->properties;
            properties = afw_pool_calloc(p,
                sizeof(afw_runtime_object_map_property_t) *
                map->property_count, xctx);
            map->properties = properties;
            memcpy(properties, unresolved_properties,
                sizeof(afw_runtime_object_map_property_t) *
                map->property_count);

            for (count = map->property_count; count > 0; count--, properties++)
            {
                properties->data_type =
                    afw_environment_get_data_type(
                        properties->unresolved_data_type_id, xctx);
                if (properties->unresolved_data_type_parameter_data_type_id) {
                    properties->data_type_parameter_data_type =
                        afw_environment_get_data_type(
                            properties->unresolved_data_type_parameter_data_type_id, xctx);
                }
                if (!properties->accessor) {
                    properties->accessor =
                        afw_environment_get_runtime_value_accessor(
                            properties->accessor_name, xctx);
                }
                if (!properties->data_type || !properties->accessor)
                {
                    AFW_THROW_ERROR_FZ(general, xctx,
                        "Error resolving runtime object map " AFW_UTF8_FMT_Q,
                        AFW_UTF8_FMT_ARG(meta->object_type_id));
                }
            }
        }

        /* Register inf. */
        afw_environment_register_runtime_object_map_inf(
            meta->object_type_id, inf, xctx);
    }
}


/* Create an indirect runtime object using a know inf. */
AFW_DEFINE(const afw_object_t *)
afw_runtime_object_create_indirect_using_inf(
    const afw_object_inf_t *inf,
    const afw_utf8_t *object_id,
    void * internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_runtime_object_type_meta_t *meta;
    afw_runtime_object_indirect_t *obj;
        
    /* Get object id. */
    meta = inf->rti.implementation_specific;

    /* Make an indirect object. */
    obj = afw_pool_calloc_type(p, afw_runtime_object_indirect_t,
        xctx);
    obj->pub.inf = inf;
    obj->pub.p = p;
    obj->pub.meta.id = object_id;
    obj->pub.meta.object_type_uri = meta->object_type_id;
    obj->pub.meta.object_uri = afw_object_path_make(
        afw_s_afw, meta->object_type_id, object_id,
        p, xctx);
    obj->internal = internal;

    /* Return object. */
    return (const afw_object_t *)obj;
}


/* Create and set an indirect runtime object. */
AFW_DEFINE(const afw_object_t *)
afw_runtime_object_create_indirect(
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    void * internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_inf_t *inf;
    
    /* Get inf associated with object type. */
    inf = afw_environment_get_runtime_object_map_inf(
        object_type_id, xctx);
    if (!inf) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Runtime object map " AFW_UTF8_FMT_Q " is not registered",
            AFW_UTF8_FMT_ARG(object_type_id));
    }

    return afw_runtime_object_create_indirect_using_inf(inf,
        object_id, internal, p, xctx);    
}



/* Create and set an indirect runtime object using a know inf. */
AFW_DEFINE(void)
afw_runtime_env_create_and_set_indirect_object_using_inf(
    const afw_object_inf_t *inf,
    const afw_utf8_t *object_id,
    void * internal,
    afw_boolean_t overwrite,
    afw_xctx_t *xctx)
{
    const afw_object_t *obj;

    /* Create runtime object. */
    obj = afw_runtime_object_create_indirect_using_inf(inf, object_id,
        internal, xctx->env->p, xctx);
        
    /* Set it as a runtime object. */
    afw_runtime_env_set_object(obj, overwrite, xctx);
}



/* Create and set an indirect runtime object in environment. */
AFW_DEFINE(void)
afw_runtime_env_create_and_set_indirect_object(
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    void * internal,
    afw_boolean_t overwrite,
    afw_xctx_t *xctx)
{
    const afw_object_t *obj;
    
    /* Create runtime object. */
    obj  = afw_runtime_object_create_indirect(object_type_id, object_id,
        internal, xctx->env->p, xctx);

    /* Set it as a runtime object. */
    afw_runtime_env_set_object(obj, overwrite, xctx);
}



static const afw_object_t *
impl_resolve_untyped_object(
    const afw_runtime_unresolved_const_embedded_untyped_object_t *unresolved,
    const afw_object_t *embedding_object,
    const afw_utf8_t *property_name,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
     
    afw_runtime_const_object_instance_t *resolved;
    const afw_array_t *parent_paths;
    const afw_object_t *delta;

    resolved = afw_pool_calloc_type(p,
        afw_runtime_const_object_instance_t, xctx);
    resolved->pub.inf = &afw_runtime_inf_const_embedded_untyped_object;
    resolved->pub.p = p;
    resolved->pub.meta.embedding_object = embedding_object;
    resolved->pub.meta.id = property_name;

    resolved->properties =
        impl_resolve_properties(unresolved->properties, embedding_object,
            p, xctx);

    if (unresolved->parent_paths) {
        delta = afw_object_meta_get_nonempty_delta(
            (const afw_object_t *)resolved, xctx);
        parent_paths = afw_array_create_wrapper_for_array(
            (const void *)unresolved->parent_paths, false,
            afw_data_type_anyURI, unresolved->parent_paths_count,
            p, xctx);
        afw_object_set_property_as_array(delta,
            afw_s_parentPaths, parent_paths, xctx);
    }

    return (const afw_object_t *)resolved;
}


static const afw_array_t *
impl_resolve_list(
    const afw_runtime_unresolved_const_array_t *unresolved,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_data_type_t *data_type;
    const afw_array_t *result;

    data_type = afw_environment_get_data_type(&unresolved->data_type_id, xctx);
    if (!data_type) {
        AFW_THROW_ERROR_Z(general, "Unsupported data type", xctx);
    }

    result = afw_array_create_wrapper_for_array(unresolved->array,
        false, data_type, unresolved->count, p, xctx);
    return result;
}


static const afw_runtime_property_t *
impl_resolve_property(
    const afw_runtime_unresolved_property_t *unresolved,
    const afw_object_t *embedding_object,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_runtime_property_t *resolved;
    const afw_object_t *object;
    const afw_array_t *list;
    const afw_utf8_t *string;
    const afw_utf8_t *name;

    resolved = afw_pool_calloc_type(p, afw_runtime_property_t, xctx);
    resolved->name = afw_utf8_from_utf8_z(unresolved->name, p, xctx);

    switch (unresolved->value.type) {

    case afw_runtime_unresolved_primitive_type_boolean:
        resolved->value = (unresolved->value.boolean)
            ? afw_boolean_v_true
            : afw_boolean_v_false;
        break;

    case afw_runtime_unresolved_primitive_type_number:
        resolved->value = afw_value_create_unmanaged_double(
            unresolved->value.number, p, xctx);
        break;

    case afw_runtime_unresolved_primitive_type_integer:
        resolved->value = afw_value_create_unmanaged_integer(
            unresolved->value.integer, p, xctx);
        break;

    case afw_runtime_unresolved_primitive_type_array:
        list = impl_resolve_list(unresolved->value.array, p, xctx);
        resolved->value = afw_value_create_unmanaged_array(list, p, xctx);
        break;

    case afw_runtime_unresolved_primitive_type_object:
        name = afw_utf8_create(unresolved->name, -1, p, xctx);
        object = impl_resolve_untyped_object(unresolved->value.object,
            embedding_object, name, p, xctx);
        resolved->value = afw_value_create_unmanaged_object(object, p, xctx);
        break;
        
    case afw_runtime_unresolved_primitive_type_string:
        string = afw_utf8_from_utf8_z(unresolved->value.string, p, xctx);
        resolved->value = afw_value_create_unmanaged_string(string, p, xctx);
        break;

    default:
        AFW_THROW_ERROR_Z(general, "Unexpected type", xctx);
    }

    return resolved;
}



static const afw_runtime_property_t * const *
impl_resolve_properties(
    const afw_runtime_unresolved_property_t * const * properties,
    const afw_object_t *embedding_object,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_size_t count;
    const afw_runtime_property_t * * resolved;
    const afw_runtime_property_t * const * result;
    const afw_runtime_unresolved_property_t * const *unresolved;

    /* Count properties. */
    for (count = 0, unresolved = properties;
        *unresolved; count++, unresolved++);

    /* Create list of resolved properties. */
    resolved = afw_pool_malloc(p,
        (count + 1) * sizeof(afw_runtime_property_t *), xctx);
    result = resolved;
    for (unresolved = properties; *unresolved; unresolved++)
    {
        *resolved++ = impl_resolve_property(*unresolved, embedding_object,
            p, xctx);
    }
    *resolved++ = NULL;

    /* Return resolved properties. */
    return result;
}


/*
 * Resolve a runtime const object.
 */
AFW_DEFINE(const afw_object_t *)
afw_runtime_resolve_const_object(
    const afw_runtime_unresolved_const_object_t *unresolved,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_runtime_const_object_instance_t *o;
    afw_value_array_t *parent_paths;

    /* Allocate memory for const object. */
    o = afw_pool_calloc_type(p,
        afw_runtime_const_object_instance_t, xctx);
    o->pub.p = p;

    /* Get interface for object. */
    o->pub.inf = afw_environment_get_runtime_object_map_inf(
        unresolved->object_type_id, xctx);
    if (!o->pub.inf) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "No map available for object type " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_ARG(unresolved->object_type_id));
    }

    /* Copy id and paths. */
    o->pub.meta.id = unresolved->object_id;
    o->pub.meta.object_type_uri = unresolved->object_type_id;
    o->pub.meta.object_uri = unresolved->path;

    if (unresolved->parent_path.len > 0) {
        parent_paths = afw_value_allocate_unmanaged_array(p, xctx);
        parent_paths->internal = afw_array_create_wrapper_for_array(
            (const void *)&unresolved->parent_path, false,
            afw_data_type_anyURI, 1, p, xctx);
        afw_object_meta_set_parent_paths((const afw_object_t *)o,
            parent_paths, xctx);
    }

    o->properties = impl_resolve_properties(unresolved->properties,
        (const afw_object_t *)o, p, xctx);

    /* Return object. */
    return (afw_object_t *)o;
}


/*
 * Resolve and set a NULL terminated list of object pointers in the
 * xctx's runtime objects.
 */
AFW_DEFINE(void)
afw_runtime_env_resolve_and_set_const_objects(
    const afw_runtime_unresolved_const_object_t * const * unresolved,
    afw_boolean_t overwrite, afw_xctx_t *xctx)
{
    const afw_object_t *obj;
    const afw_pool_t *p = xctx->env->p;

    for (; *unresolved; unresolved++) {
        obj = afw_runtime_resolve_const_object(*unresolved,
            p, xctx);
        afw_runtime_env_set_object(obj, false, xctx);
    }
}



typedef struct {
    const afw_utf8_t *object_type_id;
    const afw_utf8_t *object_id;
} impl_check_manifest_cb_context_t;



static afw_boolean_t
impl_check_manifest_cb(
    const afw_object_t *object,
    void *context,
    afw_xctx_t *xctx)
{
    const afw_value_t *providesObjects_value;
    const afw_utf8_t *extension_id;
    const afw_utf8_t *module_path;
    const afw_utf8_t *entry;
    const afw_iterator_t *iterator;
    const afw_array_t *list;
    impl_check_manifest_cb_context_t *ctx = context;
    afw_utf8_t s;

    if (!object) {
        return false;
    }

    providesObjects_value = afw_object_get_property(object, afw_s_providesObjects, xctx);
    if (!providesObjects_value) {
        return false;
    }

    if (!afw_value_is_array_of_string(providesObjects_value) &&
        !afw_value_is_array_of_anyURI(providesObjects_value))
    {
        return false;
    }

    /* Loop through entries. */
    list = ((const afw_value_array_t *)providesObjects_value)->internal;
    for (iterator = NULL;;)
    {
        afw_array_get_next_internal(list, &iterator, NULL,
            (const void **)&entry, xctx);
        if (!entry) {
            break;
        }

        /* Check for entry match. */
        if (!afw_utf8_starts_with_z(entry, "/afw/")) {
            continue;
        }
        s.s = entry->s + 5;
        s.len = entry->len - 5;
        if (!afw_utf8_starts_with(&s, ctx->object_type_id)) {
            continue;
        }
        s.s = s.s + ctx->object_type_id->len;
        s.len = s.len - ctx->object_type_id->len;
        if (s.len == 0 || *(s.s) != '/') {
            continue;
        }
        s.s++;
        s.len--;
        if (!afw_utf8_equal(&s, ctx->object_id)) {
            continue;
        }

        /* If extension provides object, load extension and break. */
        extension_id = afw_object_old_get_property_as_string(object,
            afw_s_extensionId, xctx);
        module_path = afw_object_old_get_property_as_string(object,
            afw_s_modulePath, xctx);
        if (extension_id && module_path) {
            afw_environment_load_extension(extension_id, module_path,
                NULL, xctx);
            /*Return indicating complete. */
            return true;
        }
    }

    /* Return indicating not to short circuit */
    return false;
}


/* Get a runtime object. */
AFW_DEFINE(const afw_object_t *)
afw_runtime_get_object(
    const afw_utf8_t *object_type_id, const afw_utf8_t *object_id,
    AFW_COMPILER_ANNOTATION_NONNULL afw_xctx_t *xctx)
{
    const afw_object_t *result;
    const afw_xctx_t *c;
    apr_hash_t *ht;
    impl_check_manifest_cb_context_t ctx;

    result = NULL;

    for (c = xctx; c; c = c->parent) {
        if (c->runtime_objects && c->runtime_objects->types_ht) {
            ht = apr_hash_get(c->runtime_objects->types_ht,
                object_type_id->s, object_type_id->len);
            if (ht) {
                result = impl_get_object(ht, object_id->s, object_id->len,
                    xctx);
                if (result) break;
            }
        }
    }

    if (!result) {
        ctx.object_type_id = object_type_id;
        ctx.object_id = object_id;
        afw_runtime_foreach(afw_s__AdaptiveManifest_,
            &ctx, impl_check_manifest_cb, xctx);
        for (c = xctx; c; c = c->parent) {
            if (c->runtime_objects && c->runtime_objects->types_ht) {
                ht = apr_hash_get(c->runtime_objects->types_ht,
                    object_type_id->s, object_type_id->len);
                if (ht) {
                    result = impl_get_object(ht, object_id->s, object_id->len,
                        xctx);
                    if (result) break;
                }
            }
        }
    }

    if (result) {
        afw_object_get_reference(result, xctx);
    }
    return result;
}


/* Call a callback for each runtime object. */
AFW_DEFINE(void)
afw_runtime_foreach(
    const afw_utf8_t *object_type_id,
    void *context, afw_object_cb_t callback,
    afw_xctx_t *xctx)
{
    /*
     * Function impl_afw_adaptor_session_retrieve_objects() can accept NULL
     * session instance for a session-less retrieve.
     */
    impl_afw_adaptor_session_retrieve_objects(NULL, NULL,
        object_type_id, NULL, context, callback, NULL, xctx->p, xctx);
}


AFW_DEFINE(const afw_adaptor_factory_t *)
afw_runtime_get_adaptor_factory()
{
    return &impl_factory;
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
    afw_adaptor_t *adaptor;

    adaptor = afw_adaptor_impl_create_cede_p(
        &impl_afw_adaptor_inf,
        sizeof(impl_afw_adaptor_self_t), properties, p, xctx);

    /* If this is afw adaptor, remember it in environment. */
    if (afw_utf8_equal(&adaptor->adaptor_id, afw_s_afw)) {
        ((afw_environment_t*)xctx->env)->afw_adaptor = adaptor;
    }

    return adaptor;
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


/* Get an internal session for runtime objects. */
AFW_DEFINE(const afw_adaptor_session_t *)
afw_runtime_get_internal_session(afw_xctx_t *xctx)
{
    return impl_afw_adaptor_create_adaptor_session(NULL, xctx);
}


/*
 * Implementation of method create_adaptor_session of interface afw_adaptor.
 */
const afw_adaptor_session_t *
impl_afw_adaptor_create_adaptor_session (
    const afw_adaptor_t * instance,
    afw_xctx_t *xctx)
{
    impl_afw_adaptor_session_self_t *self;
    const afw_pool_t *session_p;

    session_p = afw_pool_create(xctx->p, xctx);;
    self = afw_pool_calloc_type(session_p, impl_afw_adaptor_session_self_t, xctx);
    self->pub.inf = &impl_afw_adaptor_session_inf;
    self->pub.adaptor = instance;
    self->pub.p = session_p;

    return (const afw_adaptor_session_t *)self;
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
    /* There are no adaptor specific metrics. */
    return NULL;
}


/* ----------------- Runtime adaptor session implementation ---------------- */

/*
 * Implementation of method destroy of interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_destroy (
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    impl_afw_adaptor_session_self_t * self = 
        (impl_afw_adaptor_session_self_t *)instance;

    /* Release pool. */
    afw_pool_release(self->pub.p, xctx);
}



/*
 * Implementation of method retrieve_objects of interface afw_adaptor_session.
 *
 * This function is also called by afw_runtime_foreach() with a NULL session
 * instance.  In this case, custom handled object types are not supported.
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
    const afw_object_t *obj;
    const afw_xctx_t *c;
    apr_hash_t *ht;
    apr_pool_t *apr_p;
    apr_hash_index_t *hi;
    const afw_runtime_custom_t *custom;
    const impl_ht_object_entry *entry;

    /* If this is a custom handled object type, call its function and return. */
    if (instance) {
        custom = afw_environment_get_runtime_custom(object_type_id, xctx);
        if (custom) {
            custom->retrieve_objects(instance, impl_request,
                object_type_id, criteria,
                context, callback, NULL, p, xctx);
            return;
        }
    }

    /* Call callback with all applicable set runtime objects. */
    apr_p = afw_pool_get_apr_pool(p);
    for (c = xctx; c; c = c->parent) {
        if (c->runtime_objects && c->runtime_objects->types_ht) {
            ht = apr_hash_get(c->runtime_objects->types_ht,
                object_type_id->s, object_type_id->len);
            if (ht) {
                for (hi = apr_hash_first(apr_p, ht); hi;
                    hi = apr_hash_next(hi))
                {
                    apr_hash_this(hi, NULL, NULL, (void **)&entry);
                    obj = impl_entry_to_object(entry, xctx);
                    if (afw_query_criteria_test_object(obj,
                        criteria, p, xctx))
                    {
                        /* If complete, short circuit retrieve and return. */
                        if (callback(obj, context, xctx)) {
                            return;
                        }
                    }
                }
            }
        }
    }

    /* Call callback one last time with NULL object pointer. */
    callback(NULL, context, xctx);
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
    const afw_object_t *object;
    const afw_runtime_custom_t *custom;

    /* If this is a custom handled object type, call its function and return. */
    custom = afw_environment_get_runtime_custom(object_type_id, xctx);
    if (custom) {
        custom->get_object(instance, impl_request,
            object_type_id, object_id,
            context, callback, adaptor_type_specific, p, xctx);
        return;
    }

    /* Call callback with object. */
    object = afw_runtime_get_object(object_type_id, object_id, xctx);
    if (object) afw_object_get_reference(object, xctx);

    callback(object, context, xctx);
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
    AFW_ADAPTOR_IMPL_ERROR_ADAPTOR_IMMUTABLE;
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
    AFW_ADAPTOR_IMPL_ERROR_ADAPTOR_IMMUTABLE;
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
    AFW_ADAPTOR_IMPL_ERROR_ADAPTOR_IMMUTABLE;
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

    AFW_ADAPTOR_IMPL_ERROR_ADAPTOR_IMMUTABLE;
}



/*
 * Implementation of method begin_transaction of interface afw_adaptor_session.
 */
const afw_adaptor_transaction_t *
impl_afw_adaptor_session_begin_transaction (
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    /* No transaction support. */
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
    /* No journal support. */
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
    /* No key support. */
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
    /* No index support. */
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
    impl_afw_adaptor_session_self_t *self =
        (impl_afw_adaptor_session_self_t *)instance;

    afw_adaptor_impl_object_type_cache_initialize(
        &self->object_type_cache,
        &afw_adaptor_impl_object_type_cache_inf,
        instance, true, xctx);

    return &self->object_type_cache;
}


/* ----------------- Object implementation ----------------------- */

typedef struct impl_runtime_iterator_s {
    const afw_runtime_object_map_property_t *map_entry;
    const afw_runtime_property_t ** extra_prop;
} impl_runtime_iterator_t;

/* Find map entry for property. */
static const afw_value_t *
impl_make_value_from_map_entry(
    const afw_object_t *instance,
    const afw_runtime_object_map_property_t *prop,
    afw_xctx_t *xctx)
{
    const afw_runtime_object_type_meta_t *meta;
    const char * internal;

    meta = instance->inf->rti.implementation_specific;

    if (prop->offset == -1) {
        return prop->accessor(prop,
            (const void *)
            ((const afw_runtime_object_indirect_t *)instance)->internal,
            xctx->p, xctx);
    }
    else {
        internal = (const char *)
            ((meta->indirect)
                ? ((const afw_runtime_object_indirect_t *)instance)->internal
                : instance);
        return prop->accessor(prop, (const void *)(internal + prop->offset),
            xctx->p, xctx);
    }
}



/*
 * Implementation of method release of interface afw_object.
 */
void
afw_runtime_object_release(
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    /** @fixme Think about this. */
}



/*
 * Implementation of method get_reference of interface afw_object.
 */
void
afw_runtime_object_get_reference (
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    /** @fixme Think about this. */
}



/*
 * Implementation of method get_count for interface afw_object.
 */
afw_size_t
afw_runtime_object_get_count(
    const afw_object_t * instance,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}



/*
 * Implementation of method get_object_type_id of interface afw_object.
 */
const afw_utf8_t *
afw_runtime_object_get_object_type_id(
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    const afw_runtime_object_type_meta_t *meta; 
     
    meta = instance->inf->rti.implementation_specific;

    return (meta) ? meta->object_type_id : NULL;
}



/*
 * Implementation of method get_property of interface afw_object.
 */
const afw_value_t *
afw_runtime_object_get_property(
    const afw_object_t * instance,
    const afw_utf8_t * property_name,
    afw_xctx_t *xctx)
{
    const afw_runtime_object_map_property_t  *prop;
    const afw_runtime_property_t **more;
    afw_size_t count;
    const afw_runtime_object_type_meta_t *meta;
    const char * internal;
     
    meta = instance->inf->rti.implementation_specific;

    /* Search for property and return it if found. */
    if (meta->property_map) {
        for (prop = meta->property_map->properties,
            count = meta->property_map->property_count;
            count > 0;
            prop++, count--)
        {
            if (afw_utf8_equal(property_name, prop->name)) {
                return impl_make_value_from_map_entry(instance, prop, xctx);
            }
        }
    }
 
    /* Search unmapped properties. */
    internal = (const char *)
        ((meta->indirect)
        ? ((const afw_runtime_object_indirect_t *)instance)->internal
        : instance);
    more = (const afw_runtime_property_t **)
        ((meta->properties_offset != -1)
        ? *((const afw_runtime_property_t **)
            (internal + meta->properties_offset))
        : NULL);

    if (more) {
        for (; *more; more++) {
            if (afw_utf8_equal(property_name, (*more)->name)) {
                return (*more)->value;
            }
        }
    }

    /* Return NULL if not found. */
    return NULL;
}



/*
 * Implementation of method get_meta for interface afw_object.
 */
const afw_value_t *
afw_runtime_object_get_meta(
    const afw_object_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_object_impl_internal_get_meta(
        instance, p, xctx);
}



/*
 * Implementation of method get_property_meta for interface afw_object.
 */
const afw_value_t *
afw_runtime_object_get_property_meta(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_object_impl_internal_get_property_meta(
        instance, property_name, p, xctx);
}



/*
 * Implementation of method get_next_property of interface afw_object.
 */
const afw_value_t *
afw_runtime_object_get_next_own_property(
    const afw_object_t * instance,
    const afw_iterator_t * * iterator,
    const afw_utf8_t * * property_name,
    afw_xctx_t *xctx)
{
    const afw_runtime_object_map_property_t  *map_entry;
    const afw_value_t *result;
    impl_runtime_iterator_t *i;
    const afw_runtime_object_type_meta_t *meta;
    const char * internal;
     
    meta = instance->inf->rti.implementation_specific;

    /* If iterator is not NULL, it point to last property pointer. */
    if (*iterator) {
        i = (impl_runtime_iterator_t *)*iterator;
    }
    else {
        i = afw_xctx_calloc_type(impl_runtime_iterator_t, xctx);
        *iterator = (afw_iterator_t *)i;
        i->map_entry = (meta->property_map) 
            ? meta->property_map->properties
            : NULL;
    }
   
    /*
        prop = *(const afw_runtime_object_map_property_t  **)iterator;
    }
    / * If iterator is NULL, start from beginning. * /
    else {
        prop = self->property_map->properties;
    }*/

    do {
        if (!i->extra_prop) {

            if (i->map_entry &&
                i->map_entry < meta->property_map->properties +
                meta->property_map->property_count)
            {
                map_entry = i->map_entry;
                (i->map_entry)++;
                *property_name = map_entry->name;
                result = impl_make_value_from_map_entry(instance, map_entry,
                    xctx);
                if (result) break;
                continue;
            }

            internal = (const char *)
                ((meta->indirect)
                ? ((const afw_runtime_object_indirect_t *)instance)->internal
                : instance);
            i->extra_prop = (const afw_runtime_property_t **)
                ((meta->properties_offset != -1)
                ? *((const afw_runtime_property_t **)
                    (internal + meta->properties_offset))
                : NULL);
        }

        if (i->extra_prop && *(i->extra_prop)) {
            result = (*(i->extra_prop))->value;
            *property_name = (*(i->extra_prop))->name;
            (i->extra_prop)++;
            break;
        }

        *iterator = NULL;
        *property_name = NULL;
        result = NULL;
        break;

    } while (1);

    return result;
}



/*
 * Implementation of method get_next_property_meta for interface afw_object.
 */
const afw_value_t *
afw_runtime_object_get_next_property_meta(
    const afw_object_t *instance,
    const afw_iterator_t **iterator,
    const afw_utf8_t **property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_object_impl_internal_get_next_property_meta(
        instance, iterator, property_name, p, xctx);
}


/*
 * Implementation of method has_property of interface afw_object.
 */
afw_boolean_t
afw_runtime_object_has_property(
    const afw_object_t * instance,
    const afw_utf8_t * property_name,
    afw_xctx_t *xctx)
{
    /* Use impl_get_own_property() to determine if property exists. */
    return afw_runtime_object_get_property(instance, property_name, xctx)
        ? AFW_TRUE
        : AFW_FALSE;
}



/*
 * Implementation of method get_setter of interface afw_object.
 */
const afw_object_setter_t *
afw_runtime_object_get_setter (
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    return NULL;
}
