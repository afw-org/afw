// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Runtime Object Mapping Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/*  ----------------------------- N O T E -------------------------------------
 *
 * This file is generated by "afwdev generate afw_curl".
 *
 * Do not change this file directly or the changes will be lost the next time
 * this file is regenerated.
 *
 * -------------------------------------------------------------------------*/


#ifndef __AFW_CURL_RUNTIME_OBJECT_MAPS_H__
#define __AFW_CURL_RUNTIME_OBJECT_MAPS_H__


/**
 * @file afw_curl_runtime_object_maps.h
 * @brief Adaptive Framework runtime object mapping header.
 */


/**
 * @brief Runtime object inf for object type _AdaptiveCurlHttpResponse_
 */
AFW_DECLARE_CONST_DATA(afw_object_inf_t)
afw_runtime_inf__AdaptiveCurlHttpResponse_;

/**
 * @brief Runtime object inf for object type _AdaptiveCurlOptions_
 */
AFW_DECLARE_CONST_DATA(afw_object_inf_t)
afw_runtime_inf__AdaptiveCurlOptions_;

/**
 * @brief Runtime object inf for object type _AdaptiveCurlVersionInfo_
 */
AFW_DECLARE_CONST_DATA(afw_object_inf_t)
afw_runtime_inf__AdaptiveCurlVersionInfo_;

/**
 * @brief Register (afw_curl_) runtime object maps.
 * @param xctx of caller.  This should be base xctx.
 */
void afw_curl_register_runtime_object_maps(
    afw_xctx_t *xctx);


#endif /* __AFW_CURL_RUNTIME_OBJECT_MAPS_H__ */