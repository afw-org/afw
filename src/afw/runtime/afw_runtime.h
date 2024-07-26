// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Runtime Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_RUNTIME_H__
#define __AFW_RUNTIME_H__

#include "afw_interface.h"

AFW_BEGIN_DECLARES

/**
 * @defgroup afw_runtime Runtime objects
 * @ingroup afw_c_api_public
 *
 * This is an Adaptive Framework read-only objects adapter.
 *
 * @{
 */

/**
 * @file afw_runtime.h
 * @brief Runtime objects adapter.
 */


AFW_DECLARE_CONST_DATA(afw_object_inf_t)
afw_runtime_inf_const_embedded_untyped_object;

AFW_DECLARE_CONST_DATA(afw_object_inf_t)
afw_runtime_inf_const_meta_object_inf;

/*
 * @brief Custom handling information for a runtime object type.
 * 
 * This is registered using afw_environment_register_runtime_custom() and
 * retrieved using afw_environment_get_runtime_custom().
 */
struct afw_runtime_custom_s {

    /** @brief The session retrieve_objects method. */
    afw_adapter_session_retrieve_objects_t retrieve_objects;

    /** @brief The session get_object method. */
    afw_adapter_session_get_object_t get_object;

};


struct afw_runtime_object_type_meta_s {

    /**
     * @brief Object Type Id.
     *
     * This is the object type id of all runtime object that use this meta.
     */
    const afw_utf8_t *object_type_id;

    /**
     * @brief Map of property offsets in internal or NULL.
     *
     * Map of property offset in internal or NULL.
     */
    const afw_runtime_object_map_t *property_map;

    /**
     * @brief Runtime properties offset in internal.
     * 
     * This is the offset in internal of a NULL terminated array of pointers
     * to (const afw_runtime_property_t *) runtime properties or -1 if
     * missing.
     */
    size_t properties_offset;

    /**
     * @brief Internal is indirect.
     *
     * If true, afw_runtime_object_indirect_t is used to map the object
     * instance and member internal points to the memory mapped by the
     * offsets in the meta point to by
     * (afw_runtime_object_type_meta_t *)inf->implementation_specific.
     *
     * If false, offsets are relative to the start of the object instance.
     */
    afw_boolean_t indirect;
};


/**
 * @brief Struct for runtime object map property.
 */
struct afw_runtime_object_map_property_s {

    /** @brief Property name. */
    const afw_utf8_t *name;

    /** @brief Offset of internal property value in internal object struct.
     *
     * If onGetValueCFunctionName is specified, this will be -1. 
     */
    afw_size_t offset;

    /**
     * @brief Offset of bag value count afw_size_t for bag values or -1.
     *
     * Can be -1 for bag values as well if values are pointers and array of
     * pointers in NULL terminated.
     */
    afw_size_t count_offset;

    /**
     * Data type.
     */
    const afw_data_type_t *data_type;

    /**
     * This is the dataTypeParameter.  If dataType is array, 'object ' is stripped off
     * since that can be determined by data_type_parameter_data_type.
     */
    afw_utf8_t data_type_parameter;

    /**
     * If data_type_parameter is a single data type, this is its pointer.
     */
    const afw_data_type_t *data_type_parameter_data_type;

    /**
     * @brief Name of registered runtime value accessor function.
     *
     * If this is zero length, the default accessor is used.  If offset is -1
     * (onGetValueCFunctionName was specified), this will always be NULL. 
     */
    const afw_utf8_t *accessor_name;

    /**
     * @brief Accessor function or NULL.
     *
     * If accessor_name is not zero length and this is NULL, it will
     * be resolved by afw_environment_get_runtime_value_accessor()
     * at register time.
     */
    afw_runtime_value_accessor_t accessor;

};



struct afw_runtime_object_map_s  {
    const afw_utf8_t *object_type_id;
    afw_size_t property_count;
    const afw_runtime_object_map_property_t *properties;
};


/** @brief Struct for runtime objects. */
struct afw_runtime_object_indirect_s {
    /** pub->inf MUST point to afw_runtime_object_inf. */
    afw_object_t pub;

    /** Pointer to internal representation mapped by property_map. */
    void * internal;
};


struct afw_runtime_const_object_instance_s {

    /** pub->inf MUST point to afw_runtime_object_inf. */
    afw_object_t pub;

    /** NULL terminated array of read-only properties. */ 
    const afw_runtime_property_t *const * properties;

};


/** @brief meta instance with single parent path. */
struct afw_runtime_const_object_meta_object_s {

    /** pub->inf MUST point to afw_runtime_object_inf. */
    afw_object_t pub;

    /** Object this is meta for. */
    const afw_runtime_const_object_instance_t *object;

    /** List of parent paths or NULL. */
    const afw_value_array_t *parent_paths;
};



struct afw_runtime_property_s {
    const afw_utf8_t *name;
    const afw_value_t *value;
};



/**
 * @brief Create an indirect runtime object.
 * @param inf of map.
 * @param object_id of object.
 * @param internal is pointer to object's internal.
 * @param p for result.
 * @param xctx
 *
 * The intent of this function is to create a runtime
 * object to represent a runtime struct mapped by a
 * registered runtime object map.
 */
AFW_DECLARE(const afw_object_t *)
afw_runtime_object_create_indirect_using_inf(
    const afw_object_inf_t *inf,
    const afw_utf8_t *object_id,
    void * internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Set an object pointer in the environment's runtime objects.
 * @param object to set in the xctx's runtime objects.
 * @param overwrite whether to throw error is object with path exists.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_runtime_env_set_object(
    const afw_object_t *object, afw_boolean_t overwrite,
    afw_xctx_t *xctx);



/**
 * @brief Set an array of object pointers in the environment's runtime objects.
 * @param objects NULL terminated array of object pointers.
 * @param overwrite whether to throw error is object with path exists.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_runtime_env_set_objects(
    const afw_object_t * const * objects, afw_boolean_t overwrite,
    afw_xctx_t *xctx);


/**
 * @brief Typedef used for afw_runtime_env_set_object_cb_wrapper() cb.
 * @param data set on afw_runtime_env_set_object_cb_wrapper().
 * @param p to use.
 * @param xctx of caller.
 * @return object.
 */
typedef const afw_object_t *
(*afw_runtime_object_wrapper_p_cb_t)(
    void *data,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Set environment object accessed via callback.
 * @param object_type_id of a registered map.
 * @param object_id of object.
 * @param callback to call;
 * @param data passed to callback.
 * @param overwrite whether to throw error is object with path exists.
 * @param xctx
 */
AFW_DECLARE(void)
afw_runtime_env_set_object_cb_wrapper(
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    afw_runtime_object_wrapper_p_cb_t callback,
    void *data,
    afw_boolean_t overwrite,
    afw_xctx_t *xctx);



/**
 * @brief Create and set an indirect runtime object.
 * @param object_type_id of a registered map.
 * @param object_id of object.
 * @param internal is pointer to object's internal.
 * @param overwrite whether to throw error is object with path exists.
 * @param xctx
 *
 * The intent of this function is to create a runtime
 * object to represent a runtime struct mapped by a
 * registered runtime object map.
 */
AFW_DECLARE(void)
afw_runtime_env_create_and_set_indirect_object(
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    void *internal,
    afw_boolean_t overwrite,
    afw_xctx_t *xctx);



/**
 * @brief Create and set an indirect runtime object.
 * @param inf of map.
 * @param object_id of object.
 * @param internal is pointer to object's internal.
 * @param overwrite whether to throw error is object with path exists.
 * @param xctx
 *
 * The intent of this function is to create a runtime
 * object to represent a runtime struct mapped by a
 * registered runtime object map.
 */
AFW_DECLARE(void)
afw_runtime_env_create_and_set_indirect_object_using_inf(
    const afw_object_inf_t *inf,
    const afw_utf8_t *object_id,
    void * internal,
    afw_boolean_t overwrite,
    afw_xctx_t *xctx);



/**
 * @brief Call a callback for each runtime object.
 * @param object_type_id of objects to pass to callback.
 * @param context to be passed to callback.
 * @param callback function.
 * @param xctx of caller.
 *
 * Custom handled object types are not supported and will call callback
 * once with NULL object.
 *
 * The callback function is the same type as the normal adapter get/retrieve
 * callback, but should not release the object passed.
 */
AFW_DECLARE(void)
afw_runtime_foreach(
    const afw_utf8_t *object_type_id,
    void *context, afw_object_cb_t callback,
    afw_xctx_t *xctx);



/**
 * @brief Get singleton factory for runtime adapter.
 * @return pointer to adapter factory.
 */
AFW_DECLARE(const afw_adapter_factory_t *)
afw_runtime_get_adapter_factory();



/**
 * @brief Get an internal session for runtime objects.
 * @param p to use for session.
 * @param xctx of caller.
 * @return pointer to adapter factory.
 */
AFW_DECLARE(const afw_adapter_session_t *)
afw_runtime_get_internal_session(
    afw_xctx_t *xctx);



/**
 * @brief Get a runtime object.
 * @param object_type_id of object.
 * @param object_id of object.
 * @param xctx of caller.
 */
AFW_DECLARE(const afw_object_t *)
afw_runtime_get_object(
    const afw_utf8_t *object_type_id, const afw_utf8_t *object_id,
    afw_xctx_t *xctx);


/**
 * @brief Create an indirect runtime object.
 * @param object_type_id of a registered map.
 * @param object_id of object.
 * @param internal is pointer to object's internal.
 * @param p for result.
 * @param xctx
 *
 * The intent of this function is to create a runtime
 * object to represent a runtime struct mapped by a
 * registered runtime object map.
 */
AFW_DECLARE(const afw_object_t *)
afw_runtime_object_create_indirect(
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    void * internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Remove object by object type id and object id.
 * @param object_type_id  of object to remove.
 * @param object_id of object to remove.
 * @param xctx of caller.
 *
 * No error is thrown if object is not found.
 */
AFW_DECLARE(void)
afw_runtime_remove_object(
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    afw_xctx_t *xctx);


/**
 * @brief Set an object pointer in the xctx's runtime objects.
 * @param object to set in the xctx's runtime objects.
 * @param overwrite whether to throw error is object with path exists.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_runtime_xctx_set_object(
    const afw_object_t *object, afw_boolean_t overwrite,
    afw_xctx_t *xctx);



/**
 * @brief Set an array of object pointers in the xctx's runtime objects.
 * @param objects NULL terminated array of object pointers.
 * @param overwrite whether to throw error is object with path exists.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_runtime_xctx_set_objects(
    const afw_object_t * const * objects, afw_boolean_t overwrite,
    afw_xctx_t *xctx);



/**
 * @brief Register runtime object map interfaces.
 * @param inf is NULL terminate array of inf pointers.
 * @param xctx
 *
 * inf->rti->implementation_specific MUST pointed to
 * afw_runtime_object_type_meta_t of map.
 */
AFW_DECLARE(void)
afw_runtime_register_object_map_infs(
    const afw_object_inf_t * const * inf,
    afw_xctx_t *xctx
);



#define AFW_RUNTIME_OBJECT_RTI(label_, meta_)                                   \
                                                                                \
static const afw_interface_implementation_rti_t                                 \
label_ = {                                                                      \
    AFW_UTF8_LITERAL("afw_object"),                                             \
    AFW_UTF8_LITERAL(__FILE__),                                                 \
    AFW_UTF8_LITERAL("afw_runtime"),                                            \
    &meta_                                                                      \
};                                                                              \
           

#define AFW_RUNTIME_OBJECT_INF(inf_, meta_)                                     \
                                                                                \
AFW_DEFINE_CONST_DATA(afw_object_inf_t)                                         \
inf_ = {                                                                        \
    {                                                                           \
        AFW_UTF8_LITERAL("afw_object"),                                         \
        AFW_UTF8_LITERAL(__FILE__),                                             \
        AFW_UTF8_LITERAL("afw_runtime"),                                        \
        &meta_                                                                  \
    },                                                                          \
    afw_runtime_object_release,                                                 \
    afw_runtime_object_get_reference,                                           \
    afw_runtime_object_get_count,                                           \
    afw_runtime_object_get_meta,                                                \
    afw_runtime_object_get_property,                                            \
    afw_runtime_object_get_property_meta,                                       \
    afw_runtime_object_get_next_own_property,                                   \
    afw_runtime_object_get_next_property_meta,                                  \
    afw_runtime_object_has_property,                                            \
    afw_runtime_object_get_setter                                               \
}                                                                               \


/**
 * @brief Method release for runtime object.
 */
void
afw_runtime_object_release(
    const afw_object_t * instance,
    afw_xctx_t *xctx);



/**
 * @brief Method get_reference for runtime object.
 */
void
afw_runtime_object_get_reference(
    const afw_object_t * instance,
    afw_xctx_t *xctx);



/**
 * @brief Method get_count for runtime object.
 */
afw_size_t
afw_runtime_object_get_count(
    const afw_object_t * instance,
    afw_xctx_t * xctx);



/**
 * @brief Method get_object_type_id for runtime object.
 */
const afw_utf8_t *
afw_runtime_object_get_object_type_id(
    const afw_object_t * instance,
    afw_xctx_t *xctx);



/**
 * @brief Method get_meta for runtime object.
 */
const afw_value_t *
afw_runtime_object_get_meta(
    const afw_object_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Method get_property for runtime object.
 */
const afw_value_t *
afw_runtime_object_get_property(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);



/**
 * @brief Method get_property_meta for runtime object.
 */
const afw_value_t *
afw_runtime_object_get_property_meta(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Method get_next_property for runtime object.
 */
const afw_value_t *
afw_runtime_object_get_next_own_property(
    const afw_object_t * instance,
    const afw_iterator_t * * iterator,
    const afw_utf8_t * * property_name,
    afw_xctx_t *xctx);



/**
 * @brief Method get_next_property_meta for runtime object.
 */
const afw_value_t *
afw_runtime_object_get_next_property_meta(
    const afw_object_t *instance,
    const afw_iterator_t **iterator,
    const afw_utf8_t **property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Method has_property for runtime object.
 */
afw_boolean_t
afw_runtime_object_has_property(
    const afw_object_t * instance,
    const afw_utf8_t * property_name,
    afw_xctx_t *xctx);



/**
 * @brief Method get_setter for runtime object.
 */
const afw_object_setter_t *
afw_runtime_object_get_setter(
    const afw_object_t * instance,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_RUNTIME_H__ */
