// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Object Internal Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_OBJECT_INTERNAL_H__
#define __AFW_OBJECT_INTERNAL_H__

#include "afw_interface.h"
#include <apr_ring.h>
#include <apr_hash.h>

/**
 * @defgroup afw_object_internal Object
 * @ingroup afw_c_api_internal
 * 
 * Internal object API.
 * 
 * @{
 */

/**
 * @file afw_object_internal.h
 * @brief Internal header file for Adaptive Framework memory
 */

AFW_BEGIN_DECLARES
    
typedef struct afw_object_internal_memory_object_s
    afw_object_internal_memory_object_t;

/**
 * @brief Typedef for name/value list entry.
 */
typedef struct afw_object_internal_name_value_entry_s
afw_object_internal_name_value_entry_t;


/**
 * @brief Struct for name/value list entry.
 */
struct afw_object_internal_name_value_entry_s {
    afw_object_internal_name_value_entry_t *next;
    const afw_utf8_t *name;
    const afw_value_t *value;
};


struct afw_object_internal_memory_object_s {
    afw_object_t pub;
    afw_object_setter_t setter;

    /*
     * Implementation supports both property list and property hash table.
     * See boolean use_properties_ht to determine which is being used.
     */
    union {
        afw_object_internal_name_value_entry_t *first_property;
        apr_hash_t *properties_ht;
    };

    afw_boolean_t immutable;
    afw_boolean_t clone_on_set;
    afw_boolean_t unmanaged;
    afw_boolean_t managed_by_entity;

    /* Reference_count starting at 1 on create. */
    AFW_ATOMIC afw_integer_t reference_count;
};



/* Self for meta accessor object. */
typedef struct {
    afw_object_t pub;

    /* Mutable object or NULL if always immutable. */
    const afw_object_t *mutable;

    /* The objects to search. */
    const afw_object_t * *objects;

    /* Address of end of objects array. */
    const afw_object_t * *end;

    /* Object is immutable. */
    afw_boolean_t immutable;

} afw_object_internal_composite_self_t;



/* Self for meta accessor object. */
typedef struct {
    afw_object_t pub;
    afw_object_setter_t setter;

    /* Passed on create. */
    void *data;

    /* Array of callback entries. */
    const afw_object_properties_callback_entry_t *callbacks;

    /* Address of end of callback entries. */
    const afw_object_properties_callback_entry_t *end;

    /* Object is immutable. */
    afw_boolean_t immutable;

} afw_object_internal_properties_callback_self_t;



/* Self for meta accessor object. */
typedef struct {
    afw_object_t pub;

    /* Private implementation variables */
    const afw_object_t *instance;
    const afw_object_options_t *options;
    const afw_iterator_t *iterator;
    afw_boolean_t embedded;
    afw_boolean_t limited_meta;

} afw_object_internal_meta_accessor_self_t;


/* Self for meta properties object. */
typedef struct {
    afw_object_t pub;

} afw_object_internal_meta_properties_self_t;


/* Self for meta property object. */
typedef struct {
    afw_object_t pub;

} afw_object_internal_meta_property_self_t;


/* Self for meta propertyType object. */
typedef struct {
    afw_object_t pub;

} afw_object_internal_meta_propertyType_self_t;


/* Self for meta propertyTypes object. */
typedef struct {
    afw_object_t pub;

} afw_object_internal_meta_propertyTypes_self_t;


/* Self typedef "const for key value string" entry. */
typedef struct
afw_object_const_key_value_string_entry_s {
    afw_utf8_t key;
    afw_value_string_t value;
} afw_object_const_key_value_string_entry_t;


/* Self typedef "const for key value string" implementation of afw_object. */
typedef struct
afw_object_const_key_value_self_s {
    afw_object_t pub;

    afw_size_t entry_count;
    const afw_object_const_key_value_string_entry_t *entry;
    const afw_object_const_key_value_string_entry_t *entry_end;

} afw_object_const_key_value_self_t;


/* Self typedef for afw_object_aggregate_external object implementation. */
typedef struct
afw_object_aggregate_external_self_s {
    afw_object_t pub;
    const afw_object_t * const *object_list;
    struct {
        const afw_object_t * const *object;
        const afw_iterator_t *iterator;
    } iterator; 
} afw_object_aggregate_external_self_t;

AFW_END_DECLARES

/** @} */

#endif /* __AFW_OBJECT_INTERNAL_H__ */
