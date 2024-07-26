// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Object View Internal Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __NEW_AFW_OBJECT_VIEW_INTERNAL_H__
#define __NEW_AFW_OBJECT_VIEW_INTERNAL_H__

#include "afw_interface.h"
#include "afw_utf8.h"

/**
 * @defgroup afw_object_view_internal Object View
 * @ingroup afw_c_api_internal
 * 
 * Internal Object View API.
 * 
 * @{
 */

/**
 * @file afw_object_view_internal.h
 * @brief Internal header file for Adaptive Framework view adapter
 */

AFW_BEGIN_DECLARES

typedef struct afw_object_view_internal_view_s
    afw_object_view_internal_view_t;

typedef struct afw_object_view_internal_object_self_s
    afw_object_view_internal_object_self_t;

typedef struct afw_object_view_property_s
    afw_object_view_property_t;

typedef struct afw_object_view_string_list_s
    afw_object_view_string_list_t;

typedef struct afw_object_view_parent_s
afw_object_view_parent_t;

/* Property */
struct afw_object_view_property_s {

    /* Next property. */
    afw_object_view_property_t *next_property;

    /* Property name. */
    const afw_utf8_t *name;

    /* Property value. */
    const afw_value_t *value;

    /* Origin of property. */
    const afw_object_t *origin;

};


/* String list */
struct afw_object_view_string_list_s {
    afw_object_view_string_list_t *next;
    afw_utf8_t string;
};


/* Parent. */
struct afw_object_view_parent_s {

    /* Path as specified in origin object. */
    const afw_utf8_t *path;

    /* Resolved path name where ../ is replaced. */
    const afw_utf8_t *resolved_path;

    /* Internal parent object or NULL if not composite option. */
    afw_object_view_internal_object_self_t *parent;

};


/* Object self. */
struct afw_object_view_internal_object_self_s {

    /* Public part of object. */
    afw_object_t pub;
    afw_value_object_t value;
    
    /* View common info. */
    afw_object_view_internal_view_t *view;

    /* afw_uri_parsed_t */
    const afw_uri_parsed_t *uri_parsed;

    /* Next loaded entity if this is an entity object.  See view->first_entity. */
    afw_object_view_internal_object_self_t *next_entity;

    /* List of pointers to object's origin properties. */
    afw_object_view_property_t *first_property;

    /*
     * Object passed as origin parameter on impl_object_create().
     *
     * Warning: May not be valid after return from afw_object_view_create().
     *          May not be cloned and caller may release it.
     */
    const afw_object_t *origin;

    /* NULL terminated list of parents or NULL if not determined yet. */
    afw_object_view_internal_object_self_t * *parents;

    /* Indicates that inherited properties are being added.  */
    afw_boolean_t inherited_properties_being_added;

    /* Indicates that inherited properties have been added. */
    afw_boolean_t inherited_properties_added;

};


struct afw_object_view_internal_view_s {

    /**
     * @brief Pool used for all resources in view.
     *
     * This pool goes away when the reference count goes to 0.  The reference
     * count is incremented and decremented the contained objects
     * get_reference() and release() methods.
     */
    const afw_pool_t *p;

    /* Reference_count starting at 1 on create. */
    AFW_ATOMIC afw_integer_t reference_count;

    /**
     * @brief First loaded entity.
     *
     * This is a pointer to the object that will be returned from create view.
     * See its next_entity for a list of loaded objects used for parent
     * resolution.
     */
    afw_object_view_internal_object_self_t *main_entity;

    /** @brief View options. */
    const afw_object_options_t *options;

    /** @brief Parsed URI for view entity. */
    const afw_uri_parsed_t *uri_parsed;

    /** @brief Parsed URI for view entity. */
    const afw_utf8_t *adapter_id;

    /** @brief Journal entry used during path resolution. */
    const afw_object_t *journal_entry;

    /** @brief Shared string values. */
    apr_hash_t *string_values;

    /** @brief Shared path values. */
    apr_hash_t *path_values;
};

AFW_END_DECLARES

/** @} */

#endif /* __NEW_AFW_OBJECT_VIEW_INTERNAL_H__ */
