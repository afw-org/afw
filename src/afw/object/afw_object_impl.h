// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_object implementation helpers.
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_OBJECT_IMPL_H__
#define __AFW_OBJECT_IMPL_H__


/**
 * @defgroup afw_object_impl afw_object
 * @ingroup afw_c_api_impl
 * 
 * @{
 */

/**
 * @file afw_object_impl.h
 * @brief Helpers for object implementation development.
 */

#include "afw_interface.h"

AFW_BEGIN_DECLARES


typedef struct afw_object_impl_property_meta_object_self_s {
    afw_object_t pub;
    afw_object_setter_t setter;
    const afw_object_t *owning_object;
    afw_value_object_t meta_object_value;
    const afw_value_t *property_value;
    const afw_utf8_t *property_name;
    const afw_object_t *property_type_object;
    afw_boolean_t is_immutable;
} afw_object_impl_property_meta_object_self_t;


typedef const afw_value_t *(*afw_object_impl_special_get_t) (
    afw_object_impl_property_meta_object_self_t *self,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);

typedef void (*afw_object_impl_special_set_t) (
    afw_object_impl_property_meta_object_self_t *self,
    const afw_utf8_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx);

typedef struct {
    const afw_utf8_t *property_name;
    afw_object_impl_special_get_t get;
    afw_object_impl_special_set_t set;
} afw_object_impl_name_handler_t;



#define AFW_OBJECT_IMPL_ASSERT_SELF_MUTABLE \
do { \
    if (self->immutable) { \
        AFW_OBJECT_ERROR_OBJECT_IMMUTABLE; \
    } \
} while (0)

/**
 * @brief Get list of parent paths from list of parents.
 * @param parents is NULL terminated list of parent object pointers.
 * @param xctx of caller.
 * @return NULL terminated list of parent paths.
 *
 * This can be used in implementations of afw_object_get_parent_paths()
 * when parent_paths is not set but parents is.
 */
AFW_DECLARE(const afw_utf8_t * const *)
afw_object_impl_get_parent_paths_from_parents(
    const afw_object_t * const *parents, afw_xctx_t *xctx);



/**
 * @brief A general impl of method get_meta for interface afw_object.
 * @param instance of object.
 * @param p for result.
 * @param xctx of caller.
 * @return meta object.
 *
 * To use in implementation, add the following lines after the #includes in
 * the implementation source:
 *
 * #define impl_afw_object_get_property_meta \
 *    afw_object_impl_internal_get_property_meta
 */
AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_object_impl_internal_get_meta(
    const afw_object_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx);




/**
 * @brief A general impl of method get_meta for interface afw_object
 *    that can be accessed externally.
 * @param instance of object.
 * @param p for result.
 * @param xctx of caller.
 * @return meta object.
 *
 * Function afw_object_impl_internal_get_meta() can not be used directly in
 * separate dlls or exes.
 *
 * Use the following implementation of impl_afw_object_get_meta()
 * to access afw_object_impl_internal_get_meta():
 *
 * const afw_value_t *
 * impl_afw_object_get_meta(
 *    const afw_object_t * instance,
 *    afw_xctx_t *xctx)
 * {
 *    return afw_object_impl_get_meta(
 *        instance, property_name, p, xctx);
 * }
 */
AFW_DECLARE(const afw_value_t *)
afw_object_impl_get_meta(
    const afw_object_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief A general impl of method get_property_meta for interface afw_object.
 * @param instance of object.
 * @param property_name to get meta for.
 * @param p for result.
 * @param xctx of caller.
 * @return meta object.
 *
 * To use in implementation, add the following lines after the #includes in
 * the implementation source:
 *
 * #define impl_afw_object_get_property_meta \
 *    afw_object_impl_internal_get_property_meta
 */
AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_object_impl_internal_get_property_meta(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief A general impl of method get_property_meta for interface afw_object
 *    that can be accessed externally.
 * @param instance of object.
 * @param property_name to get meta for.
 * @param p for result.
 * @param xctx of caller.
 * @return meta object.
 *
 * Function afw_object_impl_internal_get_property_meta() can not be used
 * directly in separate dlls or exes.
 *
 * Use the following implementation of impl_afw_object_get_property_meta()
 * to access afw_object_impl_internal_get_property_meta():
 *
 * const afw_value_t *
 * impl_afw_object_get_property_meta(
 *    const afw_object_t * instance,
 *    const afw_utf8_t * property_name,
 *    afw_xctx_t *xctx)
 * {
 *    return afw_object_impl_get_property_meta(
 *        instance, property_name, p, xctx);
 * }
 */
AFW_DECLARE(const afw_value_t *)
afw_object_impl_get_property_meta(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief A general impl of method get_next_property_meta for interface
 *    afw_object.
 * @param instance of object.
 * @param iterator pointer (set to NULL before first call)
 * @param property_name pointer to return property name.
 * @param p for result.
 * @param xctx of caller.
 * @return list value of object's property meta.
 *
 * To use in implementation, add the following lines after the #includes in
 * the implementation source:
 *
 * #define impl_afw_object_get_next_property_meta \
 *    afw_object_impl_internal_get_next_property_meta
 */
AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_object_impl_internal_get_next_property_meta(
    const afw_object_t *instance,
    const afw_iterator_t **iterator,
    const afw_utf8_t **property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief A general impl of method get_next_property_meta for interface
 *    afw_object that can be accessed externally.
 * @param instance of object.
 * @param iterator pointer (set to NULL before first call)
 * @param property_name pointer to return property name.
 * @param p for result.
 * @param xctx of caller.
 * @return list value of object's property meta.
 *
 * Function afw_object_impl_internal_get_next_property_meta() can not be used
 * directly in separate dlls or exes.
 *
 * Use the following implementation of impl_afw_object_get_next_property_meta()
 * to access afw_object_impl_internal_get_next_property_meta():
 *
 * const afw_value_t *
 * impl_afw_object_get_next_property_meta(
 *    const afw_object_t *instance,
 *    const afw_iterator_t **iterator,
 *    const afw_utf8_t **property_name,
 *    afw_xctx_t *xctx)
 * {
 *    return afw_object_impl_get_next_property_meta(
 *        instance, iterator, property_name, p, xctx);
 * }
 */
AFW_DECLARE(const afw_value_t *)
afw_object_impl_get_next_property_meta(
    const afw_object_t *instance,
    const afw_iterator_t **iterator,
    const afw_utf8_t **property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_OBJECT_IMPL_H__ */
