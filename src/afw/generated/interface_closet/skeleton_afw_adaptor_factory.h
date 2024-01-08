// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_adaptor_factory implementation for <afwdev {implementation_id}>
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __<afwdev {prefixed_interface_name.upper().replace('.', '_')}>_H__
#define __<afwdev {prefixed_interface_name.upper().replace('.', '_')}>_H__

/**
 * @file <afwdev {prefixed_interface_name}>.h
 * @brief <afwdev {brief}>
 *
 * This is the implementation of afw_adaptor_factory for <afwdev {implementation_id}>.
 */

#include "afw_interface.h"
#include "generated/<afwdev {srcdir_prefix}>declare_helpers.h"

AFW_BEGIN_DECLARES

/**
 * @brief Self typedef for <afwdev {implementation_id}> implementation of afw_adaptor_factory.
 */
typedef struct
<afwdev {prefixed_interface_name}>_self_s {
    afw_adaptor_factory_t pub;

    /** @todo Add private implementation variables */

} <afwdev {prefixed_interface_name}>_self_t;

/**
 * @brief Singleton instance of afw_adaptor_factory for <afwdev {implementation_id}>.
 */
<afwdev {srcdir_prefix.upper()}>DECLARE_CONST_DATA(afw_adaptor_factory_t)
<afwdev {prefixed_interface_name}>;

/**
 * @brief Get singleton instance of afw_adaptor_factory for <afwdev {implementation_id}>.
 */
<afwdev {srcdir_prefix.upper()}>DECLARE(const afw_adaptor_factory_t *)
<afwdev {prefixed_interface_name}>_get(
    const afw_object_t *parameters,
    afw_xctx_t *xctx);

AFW_END_DECLARES

#endif /* __<afwdev {prefixed_interface_name.upper().replace('.', '_')}>_H__ */
