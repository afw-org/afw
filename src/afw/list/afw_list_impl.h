// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_list implementation helpers.
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_LIST_IMPL_H__
#define __AFW_LIST_IMPL_H__


/**
 * @defgroup afw_list_impl afw_list interfaces
 * @ingroup afw_c_api_impl
 * @{
 */

/**
 * @file afw_list_impl.h
 * @brief Helpers for list implementation development.
 */

#include "afw_interface.h"

AFW_BEGIN_DECLARES

#define AFW_LIST_IMPL_ASSERT_SELF_MUTABLE \
do { \
    if (self->immutable) { \
        AFW_OBJECT_ERROR_OBJECT_IMMUTABLE; \
    } \
} while (0)



/**
 * @brief A general impl of method get_entry_meta for interface afw_list.
 * @param instance of list.
 * @param index of entry to get meta for.
 * @param p for result.
 * @param xctx of caller.
 * @return meta list.
 *
 * To use in implementation, add the following lines after the #includes in
 * the implementation source:
 *
 * #define impl_afw_list_get_entry_meta \
 *    afw_list_impl_get_entry_meta
 */
AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_list_impl_get_entry_meta(
    const afw_list_t *instance,
    afw_integer_t index,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief A general impl of method get_entry_meta for interface afw_list
 *    that can be accessed externally.
 * @param instance of list.
 * @param index of entry to get meta for.
 * @param p for result.
 * @param xctx of caller.
 * @return meta list.
 *
 * Function afw_list_impl_get_entry_meta() can not be used directly in
 * separate dlls or exes.
 *
 * Use the following implementation of impl_afw_list_get_entry_meta()
 * to access afw_list_impl_get_entry_meta():
 *
 * const afw_value_t *
 * impl_afw_list_get_entry_meta(
 *    const afw_list_t * instance,
 *    afw_integer_t index,
 *    afw_xctx_t *xctx)
 * {
 *    return afw_list_external_impl_get_entry_meta(
 *        instance, index, p, xctx);
 * }
 */
AFW_DECLARE(const afw_value_t *)
afw_list_external_impl_get_entry_meta(
    const afw_list_t *instance,
    afw_integer_t index,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief A general impl of method get_next_entry_meta for interface afw_list.
 * @param instance of list.
 * @param iterator pointer (set to NULL before first call)
 * @param p for result.
 * @param xctx of caller.
 * @return list value of list entry's meta.
 *
 * To use in implementation, add the following lines after the #includes in
 * the implementation source:
 *
 * #define impl_afw_list_get_next_entry_meta \
 *    afw_list_impl_get_next_entry_meta
 */
AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_list_impl_get_next_entry_meta(
    const afw_list_t *instance,
    const afw_iterator_t **iterator,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief A general impl of method get_next_entry_meta for interface
 *    afw_list that can be accessed externally.
 * @param instance of list.
 * @param iterator pointer (set to NULL before first call)
 * @param p for result.
 * @param xctx of caller.
 * @return list value of list entry's meta.
 *
 * Function afw_list_impl_get_next_entry_meta() can not be used directly in
 * separate dlls or exes.
 *
 * Use the following implementation of impl_afw_list_get_next_entry_meta()
 * to access afw_list_impl_get_next_entry_meta():
 *
 * const afw_value_t *
 * impl_afw_list_get_next_entry_meta(
 *    const afw_list_t *instance,
 *    const afw_iterator_t **iterator,
 *    afw_xctx_t *xctx)
 * {
 *    return afw_list_external_impl_get_next_entry_meta(
 *        instance, iterator, p, xctx);
 * }
 */
AFW_DECLARE(const afw_value_t *)
afw_list_external_impl_get_next_entry_meta(
    const afw_list_t *instance,
    const afw_iterator_t **iterator,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_LIST_IMPL_H__ */
