<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_pool for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_pool */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_POOL_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_pool_impl_declares.h"

/*
 * Implementation of method release for interface afw_pool.
 */
void
impl_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method add_reference for interface afw_pool.
 */
void
impl_afw_pool_add_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method destroy for interface afw_pool.
 */
void
impl_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_apr_pool for interface afw_pool.
 */
apr_pool_t *
impl_afw_pool_get_apr_pool(
    AFW_POOL_SELF_T *self)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method calloc for interface afw_pool.
 */
void *
impl_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method malloc for interface afw_pool.
 */
void *
impl_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method free for interface afw_pool.
 */
void
impl_afw_pool_free(
    AFW_POOL_SELF_T *self,
    void * address,
    afw_size_t size,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_symbol_value for interface afw_pool.
 */
const afw_value_t *
impl_afw_pool_get_symbol_value(
    AFW_POOL_SELF_T *self,
    const afw_value_block_symbol_t * symbol,
    const afw_boolean_t * found,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_named_value for interface afw_pool.
 */
const afw_value_t *
impl_afw_pool_get_named_value(
    AFW_POOL_SELF_T *self,
    const afw_utf8_t * name,
    const afw_boolean_t * found,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method set_symbol_value for interface afw_pool.
 */
void
impl_afw_pool_set_symbol_value(
    AFW_POOL_SELF_T *self,
    const afw_value_block_symbol_t * symbol,
    const afw_value_t * value,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method set_local_variable for interface afw_pool.
 */
afw_boolean_t
impl_afw_pool_set_local_variable(
    AFW_POOL_SELF_T *self,
    const afw_value_block_symbol_t * name,
    const afw_value_t * value,
    afw_boolean_t replace,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method register_cleanup_before for interface afw_pool.
 */
void
impl_afw_pool_register_cleanup_before(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method deregister_cleanup for interface afw_pool.
 */
void
impl_afw_pool_deregister_cleanup(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method release_debug for interface afw_pool.
 */
void
impl_afw_pool_release_debug(
    AFW_POOL_SELF_T *self,
    afw_xctx_t * xctx,
    const afw_utf8_z_t * source_z)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method add_reference_debug for interface afw_pool.
 */
void
impl_afw_pool_add_reference_debug(
    AFW_POOL_SELF_T *self,
    afw_xctx_t * xctx,
    const afw_utf8_z_t * source_z)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method destroy_debug for interface afw_pool.
 */
void
impl_afw_pool_destroy_debug(
    AFW_POOL_SELF_T *self,
    afw_xctx_t * xctx,
    const afw_utf8_z_t * source_z)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method calloc_debug for interface afw_pool.
 */
void *
impl_afw_pool_calloc_debug(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t * xctx,
    const afw_utf8_z_t * source_z)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method malloc_debug for interface afw_pool.
 */
void *
impl_afw_pool_malloc_debug(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t * xctx,
    const afw_utf8_z_t * source_z)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method free_debug for interface afw_pool.
 */
void
impl_afw_pool_free_debug(
    AFW_POOL_SELF_T *self,
    void * address,
    afw_size_t size,
    afw_xctx_t * xctx,
    const afw_utf8_z_t * source_z)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method register_cleanup_before_debug for interface
 * afw_pool.
 */
void
impl_afw_pool_register_cleanup_before_debug(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t * xctx,
    const afw_utf8_z_t * source_z)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method deregister_cleanup_debug for interface afw_pool.
 */
void
impl_afw_pool_deregister_cleanup_debug(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t * xctx,
    const afw_utf8_z_t * source_z)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
