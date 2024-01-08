// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Register Generated (afw_lmdb) Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/*  ----------------------------- N O T E -------------------------------------
 *
 * This file is generated by "afwdev generate afw_lmdb".
 *
 * Do not change this file directly or the changes will be lost the next time
 * this file is regenerated.
 *
 * -------------------------------------------------------------------------*/


#ifndef __AFW_LMDB_GENERATED_H__
#define __AFW_LMDB_GENERATED_H__


/**
 * @file afw_lmdb_generated.h
 * @brief Adaptive Framework register generated (afw_lmdb) header.
 */

#include "afw_minimal.h"
#include "afw_lmdb_declare_helpers.h"
#include "afw_lmdb_const_objects.h"
#include "afw_lmdb_function_bindings.h"
#include "afw_runtime.h"
#include "afw_lmdb_runtime_object_maps.h"
#include "afw_lmdb_strings.h"


/**
 * @brief Generated register for afw_lmdb. 
 * @param xctx of caller.  Should be base xctx.
 */
AFW_LMDB_DECLARE_INTERNAL(void)
afw_lmdb_generated_register(afw_xctx_t *xctx);


/**
 * @brief Register function for adaptor_type lmdb
 * @param xctx of caller.
 */
AFW_LMDB_DECLARE_INTERNAL(void)
afw_lmdb_adaptor_type_lmdb_register(
    afw_xctx_t *xctx);


/**
 * @brief Register function for error_rv_decoder lmdb
 * @param xctx of caller.
 */
AFW_LMDB_DECLARE_INTERNAL(void)
afw_lmdb_error_rv_decoder_lmdb_register(
    afw_xctx_t *xctx);


#endif /* __AFW_LMDB_GENERATED_H__ */
