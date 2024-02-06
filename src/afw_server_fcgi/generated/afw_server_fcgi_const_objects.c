// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Const
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/*  ----------------------------- N O T E -------------------------------------
 *
 * This file is generated by "afwdev generate afw_server_fcgi".
 *
 * Do not change this file directly or the changes will be lost the next time
 * this file is regenerated.
 *
 * -------------------------------------------------------------------------*/



/**
 * @file afw_server_fcgi_const_objects.c
 * @brief Adaptive Framework builtin objects.
 */

#include "afw.h"
#include "afw_server_fcgi_generated.h"


static const afw_runtime_const_object_instance_t
impl_1;

/*
 * /afw/_AdaptiveCollection_/afw_server_fcgi
 */

static const afw_runtime_property_t
impl_1_property_description = {
    &afw_server_fcgi_self_s_description,
    &afw_server_fcgi_self_v_zz__Adaptive_Framework_afw_server_fcgi_collection_.pub
};

static const afw_runtime_property_t
impl_1_property_objectId = {
    &afw_server_fcgi_self_s_objectId,
    &afw_server_fcgi_self_v_afw_server_fcgi.pub
};

static const afw_runtime_property_t *
impl_1_properties[] = {
    &impl_1_property_description,
    &impl_1_property_objectId,
    NULL
};

static const afw_runtime_const_object_meta_object_t
impl_1_meta_object = {
    {
        &afw_runtime_inf_const_meta_object_inf
    },
    &impl_1,
    NULL
};

static const afw_utf8_t
impl_1_meta_path =
    AFW_UTF8_LITERAL("/afw/_AdaptiveCollection_/afw_server_fcgi");

static const afw_value_object_t
impl_1__value;

static const afw_runtime_const_object_instance_t
impl_1 = {
    {
        &afw_runtime_inf__AdaptiveCollection_,
        NULL,
        (const afw_value_t *)&impl_1__value,
        {
            (const afw_object_t *)&impl_1_meta_object,
            NULL,
            &afw_server_fcgi_self_s_afw_server_fcgi,
            &afw_server_fcgi_self_s__AdaptiveCollection_,
            &impl_1_meta_path
        }
    },
    &impl_1_properties[0]
};

static const afw_value_object_t
impl_1__value = {
    {&afw_value_permanent_object_inf},
    (const afw_object_t *)&impl_1
};

/* NULL terminated list of objects. */
static const afw_object_t *
impl_const_objects[] = {
    (const afw_object_t *)&impl_1,
    NULL
};

/* Set const objects */
void
afw_server_fcgi_const_objects_register(afw_xctx_t *xctx)
{
    afw_runtime_env_set_objects(
        &impl_const_objects[0], false, xctx);
}
