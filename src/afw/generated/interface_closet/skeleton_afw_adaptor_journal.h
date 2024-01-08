// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_adaptor_journal implementation for <afwdev {implementation_id}>
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
 * This is the implementation of afw_adaptor_journal for <afwdev {implementation_id}>.
 */

#include "afw_interface.h"
#include "generated/<afwdev {srcdir_prefix}>declare_helpers.h"

AFW_BEGIN_DECLARES

/**
 * @brief Self typedef for <afwdev {implementation_id}> implementation of afw_adaptor_journal.
 */
typedef struct
<afwdev {prefixed_interface_name}>_self_s {
    afw_adaptor_journal_t pub;

    /** @todo Add private implementation variables */

} <afwdev {prefixed_interface_name}>_self_t;

AFW_END_DECLARES

#endif /* __<afwdev {prefixed_interface_name.upper().replace('.', '_')}>_H__ */
