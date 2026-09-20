<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_memory_region for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_memory_region */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_MEMORY_REGION_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_memory_region_impl_declares.h"

/*
 * Implementation of method get for interface afw_memory_region.
 */
void
impl_afw_memory_region_get(
    AFW_MEMORY_REGION_SELF_T *self,
    void ** region,
    afw_size_t * size,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method free for interface afw_memory_region.
 */
void
impl_afw_memory_region_free(
    AFW_MEMORY_REGION_SELF_T *self,
    void * region,
    afw_size_t size,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method cleanup for interface afw_memory_region.
 */
void
impl_afw_memory_region_cleanup(
    AFW_MEMORY_REGION_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method lock for interface afw_memory_region.
 */
void
impl_afw_memory_region_lock(
    AFW_MEMORY_REGION_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method unlock for interface afw_memory_region.
 */
void
impl_afw_memory_region_unlock(
    AFW_MEMORY_REGION_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method release for interface afw_memory_region.
 */
void
impl_afw_memory_region_release(
    AFW_MEMORY_REGION_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
