// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Array Template Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_ARRAY_TEMPLATE_H__
#define __AFW_ARRAY_TEMPLATE_H__

#include "afw_common.h"

/**
 * @defgroup afw_array_template_internal (deprecated) Array template
 * @ingroup afw_c_api_internal
 * 
 * (deprecated) Array Template
 * 
 * @{
 */

/**
 * @file afw_array_template.h
 * @brief Adaptive Framework Array Template Header.
*
 * mgg - I'm not real happy with this, so it may go away at some point.  Too
 *      much is obscured in macros.  But it does reduce the chance that errors
 *      will be made when using apr's array support.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Array template
*
 * This template creates a struct and functions to use allow use of
 * apr_array in a type-safe way.
*
 * Some examples where:
*
 *    type struct ... Something;
 *    Something *e;
 *    Something **ep;
 *    int i;
*
 * Instead of:
*
 *    apr_array_header_t *array;
*
 * Do this:
*
 *    APR_ARRAY_TEMPLATE(SomethingArray, Something *)
 *    SomethingArray_t *array;
*
 * Instead of:
*
 *    array = apr_array_make(pool, 10, sizeof(Something *));
*
 * Do this:
*
 *    array = make_SomethingArray(pool, 10);
*
 * Instead of:
*
 *    *apr_array_push(array, Something *) = e;
*
 * Do this:
*
 *    *push_SomethingArray(array) = e;
*
 * Instead of:
*
 *    e = (*((Something **)array->elts))[i]; or
 *    e = *(((Something **)array->elts)+i);
*
 * Do this:
*
 *    e = (*(array->elts))[i]; or
 *    e = *(array->elts+i);
*
 * Instead of:
*
 *    ep = (Something **)array->elts;
 *    for (i = 0; i < array->nelts; i++) {
 *        (*ep) to reference
 *        ep++;
 *    }
*
 * Do this:
*
 *    ep = array->elts;
 *    for (i = 0; i < array->nelts; i++) {
 *        (*ep) to reference
 *        ep++;
 *    }
*
 */
#define AFW_ARRAY_TEMPLATE(_NAME, _ELEMENT_TYPE)\
\
typedef struct {\
    apr_pool_t *pool;\
    int elt_size;\
    int nelts;\
    int nalloc;\
    _ELEMENT_TYPE *elts;\
    } _NAME##_t;\
\
typedef struct {\
    const _NAME##_t *list;\
    int next_index;\
    } _NAME##_iterator_t;\
\
    AFW_STATIC_INLINE _NAME##_t * make_##_NAME(apr_pool_t *p, int nelts)\
    {\
    return (_NAME##_t(*))apr_array_make(p, nelts, sizeof(_ELEMENT_TYPE)); \
    }\
\
    AFW_STATIC_INLINE _ELEMENT_TYPE * push_##_NAME(_NAME##_t *array)\
    {\
    return (_ELEMENT_TYPE *)apr_array_push((apr_array_header_t *)array);\
    }\
\
    AFW_STATIC_INLINE _ELEMENT_TYPE * pop_##_NAME(_NAME##_t *array)\
    {\
    return (_ELEMENT_TYPE *)apr_array_pop((apr_array_header_t *)array);\
    }\
\
    AFW_STATIC_INLINE void clear_##_NAME(_NAME##_t *array)\
    {\
    apr_array_clear((apr_array_header_t *)array);\
    }\
    \
    \
/** @fixme Uncomment this if it seems like a good idea later. mg (or remove it)*/\
    AFW_STATIC_INLINE _NAME##_iterator_t * make##_NAME##_iterator(apr_pool_t *p, const _NAME##_t *list)\
    {\
    _NAME##_iterator_t *iterator =  (_NAME##_iterator_t (*))apr_pcalloc(p, sizeof(_NAME##_iterator_t));\
    iterator->list = list;\
    iterator->next_index = 0;\
    return iterator;\
    }\
\
    AFW_STATIC_INLINE void initialize##_NAME##_iterator(_NAME##_iterator_t *iterator, const _NAME##_t *list)\
    {\
    iterator->list = list;\
    iterator->next_index = 0;\
    }\
\
AFW_STATIC_INLINE _ELEMENT_TYPE * first##_NAME##_iterator(_NAME##_iterator_t *iterator)\
    {\
    iterator->next_index = 1;\
    return (iterator->list->elts);\
    }\
\
AFW_STATIC_INLINE _ELEMENT_TYPE * next##_NAME##_iterator(_NAME##_iterator_t *iterator)\
    {\
    return (\
        (iterator->next_index < iterator->list->nelts)\
            ?iterator->list->elts+((iterator->next_index)++)\
            :NULL\
    );\
    }\
\
AFW_STATIC_INLINE int isDone##_NAME##_iterator(_NAME##_iterator_t *iterator)\
    {\
    return (iterator->next_index >= iterator->list->nelts);\
    }\
\
AFW_STATIC_INLINE _ELEMENT_TYPE * current##_NAME##_iterator(_NAME##_iterator_t *iterator)\
    {\
    return (\
        (iterator->next_index < iterator->list->nelts)\
            ?iterator->list->elts+(iterator->next_index)\
            :NULL\
    );\
}

AFW_ARRAY_TEMPLATE(afw_u8z_array,
    afw_utf8_z_t *)

AFW_ARRAY_TEMPLATE(const_afw_u8z_array,
    const afw_utf8_z_t *)

AFW_ARRAY_TEMPLATE(afw_char_ptr_array,
    char *)

AFW_ARRAY_TEMPLATE(const_afw_char_ptr_array,
    const char *)

AFW_END_DECLARES

/** @} */

#endif /* __AFW_ARRAY_TEMPLATE_H__ */
