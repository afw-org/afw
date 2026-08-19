// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Unicode code point support
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#ifndef __AFW_CODE_POINT_H__
#define __AFW_CODE_POINT_H__

#include "afw_minimal.h"

/**
 * @addtogroup afw_code_point
 * @{
 */

/**
 * @file afw_code_point.h
 * @brief Unicode code-point properties (encoding-neutral).
 *
 * These tests take a **code point**, not UTF-8 octets. UTF-8 encode/decode
 * stays in @ref afw_utf8. Identifier and whitespace productions are those
 * Adaptive Script uses; `is_control` is Unicode general category Cc.
 *
 * See @ref afw_code_point.
 */

AFW_BEGIN_DECLARES

/**
 * @brief True if cp can start an Adaptive identifier.
 */
AFW_DECLARE(afw_boolean_t)
afw_code_point_is_identifier_start(afw_code_point_t cp);

/**
 * @brief True if cp can continue an Adaptive identifier.
 */
AFW_DECLARE(afw_boolean_t)
afw_code_point_is_identifier_continue(afw_code_point_t cp);

/**
 * @brief True if cp is an Adaptive EOL (LF, CR, LS, PS).
 */
AFW_DECLARE(afw_boolean_t)
afw_code_point_is_eol(afw_code_point_t cp);

/**
 * @brief True if cp is Adaptive whitespace (tab/VT/FF/ZWNBSP or Zs).
 */
AFW_DECLARE(afw_boolean_t)
afw_code_point_is_whitespace(afw_code_point_t cp);

/**
 * @brief True if cp is Adaptive whitespace or EOL.
 */
AFW_DECLARE(afw_boolean_t)
afw_code_point_is_whitespace_or_eol(afw_code_point_t cp);

/**
 * @brief True if cp is Unicode general category Cc (NUL, C0, DEL, C1).
 *
 * Used by forced_safe encode. Does not include Cf or Zs.
 */
AFW_DECLARE(afw_boolean_t)
afw_code_point_is_control(afw_code_point_t cp);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_CODE_POINT_H__ */
