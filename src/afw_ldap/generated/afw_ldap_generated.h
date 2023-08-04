// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Register Generated (afw_ldap) Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/*  ----------------------------- N O T E -------------------------------------
 *
 * This file is generated by "afwdev generate afw_ldap".
 *
 * Do not change this file directly or the changes will be lost the next time
 * this file is regenerated.
 *
 * -------------------------------------------------------------------------*/


#ifndef __AFW_LDAP_GENERATED_H__
#define __AFW_LDAP_GENERATED_H__


/**
 * @file afw_ldap_generated.h
 * @brief Adaptive Framework register generated (afw_ldap) header.
 */

#include "afw_minimal.h"
#include "afw_ldap_declare_helpers.h"
#include "afw_ldap_const_objects.h"
#include "afw_runtime.h"
#include "afw_ldap_runtime_object_maps.h"
#include "afw_ldap_strings.h"


/**
 * @brief Generated register for afw_ldap. 
 * @param xctx of caller.  Should be base xctx.
 */
AFW_LDAP_DECLARE_INTERNAL(void)
afw_ldap_generated_register(afw_xctx_t *xctx);


/**
 * @brief Register function for adaptor_type ldap
 * @param xctx of caller.
 */
AFW_LDAP_DECLARE_INTERNAL(void)
afw_ldap_adaptor_type_ldap_register(
    afw_xctx_t *xctx);


/**
 * @brief Register function for error_rv_decoder ldap
 * @param xctx of caller.
 */
AFW_LDAP_DECLARE_INTERNAL(void)
afw_ldap_error_rv_decoder_ldap_register(
    afw_xctx_t *xctx);


#endif /* __AFW_LDAP_GENERATED_H__ */
