/* Interface afw_adapter_factory Implementation for afw_vfs        */
// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_vfs_adapter_factory.c
 * @brief Implementation of afw_adapter_factory interface for afw_vfs
 */

#include "afw.h"
#include "afw_adapter_impl.h"
#include "afw_vfs_adapter_internal.h"


/* Declares and rti/inf defines for interface afw_adapter_factory */
#define AFW_IMPLEMENTATION_ID "vfs"
#include "afw_adapter_factory_impl_declares.h"

static const afw_utf8_t
impl_factory_description =
    AFW_UTF8_LITERAL("Adapter type vfs.");


const afw_adapter_factory_t
afw_vfs_adapter_factory_vfs =
{
    &impl_afw_adapter_factory_inf,
    AFW_UTF8_LITERAL("vfs"),
    &impl_factory_description
};


/*
 * Implementation of method create_adapter_cede_p of interface afw_adapter_factory.
 */
const afw_adapter_t *
impl_afw_adapter_factory_create_adapter_cede_p (
    const afw_adapter_factory_t * instance,
    const afw_object_t * properties,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    return afw_vfs_adapter_internal_create_cede_p(
        properties, p, xctx);
}
