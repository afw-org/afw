// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_array implementation helpers.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_ARRAY_IMPL_H__
#define __AFW_ARRAY_IMPL_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_array_impl
 * @{
 */

/**
 * @file afw_array_impl.h
 * @brief Helpers for implementing `afw_array` (and related) interfaces.
 *
 * See @ref afw_array_impl. Include from implementation `.c` only; callers
 * use array create helpers and `afw_array_*()` macros.
 */

AFW_BEGIN_DECLARES

#define AFW_LIST_IMPL_ASSERT_SELF_MUTABLE \
do { \
    if (self->immutable) { \
        AFW_OBJECT_ERROR_OBJECT_IMMUTABLE; \
    } \
} while (0)



/**
 * @brief A general impl of method get_entry_meta for interface afw_array.
 * @param instance of array.
 * @param index of entry to get meta for.
 * @param p for result.
 * @param xctx of caller.
 * @return meta array.
 *
 * To use in implementation, add the following lines after the #includes in
 * the implementation source:
 *
 * #define impl_afw_array_get_entry_meta \
 *    afw_array_impl_get_entry_meta
 */
extern const afw_value_t *
afw_array_impl_get_entry_meta(
    const afw_array_t *instance,
    afw_integer_t index,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief A general impl of method get_entry_meta for interface afw_array
 *    that can be accessed externally.
 * @param instance of array.
 * @param index of entry to get meta for.
 * @param p for result.
 * @param xctx of caller.
 * @return meta array.
 *
 * Function afw_array_impl_get_entry_meta() can not be used directly in
 * separate dlls or exes.
 *
 * Use the following implementation of impl_afw_array_get_entry_meta()
 * to access afw_array_impl_get_entry_meta():
 *
 * const afw_value_t *
 * impl_afw_array_get_entry_meta(
 *    const afw_array_t * instance,
 *    afw_integer_t index,
 *    afw_xctx_t *xctx)
 * {
 *    return afw_array_external_impl_get_entry_meta(
 *        instance, index, p, xctx);
 * }
 */
AFW_DECLARE(const afw_value_t *)
afw_array_external_impl_get_entry_meta(
    const afw_array_t *instance,
    afw_integer_t index,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief A general impl of method get_next_entry_meta for interface afw_array.
 * @param instance of array.
 * @param iterator pointer (set to NULL before first call)
 * @param p for result.
 * @param xctx of caller.
 * @return array value of array entry's meta.
 *
 * To use in implementation, add the following lines after the #includes in
 * the implementation source:
 *
 * #define impl_afw_array_get_next_entry_meta \
 *    afw_array_impl_get_next_entry_meta
 */
extern const afw_value_t *
afw_array_impl_get_next_entry_meta(
    const afw_array_t *instance,
    const afw_iterator_old_t **iterator,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Shared afw_array initialize_iterator (#153 defined afw_iterator).
 * @param instance array to walk.
 * @param iterator caller-defined afw_iterator_t storage to fill.
 * @param xctx of caller.
 *
 * Sets iterator->inf to the shared array-as-iterator vtable and resets
 * cursor fields. Use:
 *
 * #define impl_afw_array_initialize_iterator \\
 *    afw_array_impl_initialize_iterator
 *
 * before including afw_array_impl_declares.h.
 */
extern void
afw_array_impl_initialize_iterator(
    const afw_array_t *instance,
    const afw_iterator_t *iterator,
    afw_xctx_t *xctx);


/**
 * @brief A general impl of method get_next_entry_meta for interface
 *    afw_array that can be accessed externally.
 * @param instance of array.
 * @param iterator pointer (set to NULL before first call)
 * @param p for result.
 * @param xctx of caller.
 * @return array value of array entry's meta.
 *
 * Function afw_array_impl_get_next_entry_meta() can not be used directly in
 * separate dlls or exes.
 *
 * Use the following implementation of impl_afw_array_get_next_entry_meta()
 * to access afw_array_impl_get_next_entry_meta():
 *
 * const afw_value_t *
 * impl_afw_array_get_next_entry_meta(
 *    const afw_array_t *instance,
 *    const afw_iterator_old_t **iterator,
 *    afw_xctx_t *xctx)
 * {
 *    return afw_array_external_impl_get_next_entry_meta(
 *        instance, iterator, p, xctx);
 * }
 */
AFW_DECLARE(const afw_value_t *)
afw_array_external_impl_get_next_entry_meta(
    const afw_array_t *instance,
    const afw_iterator_old_t **iterator,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_ARRAY_IMPL_H__ */
