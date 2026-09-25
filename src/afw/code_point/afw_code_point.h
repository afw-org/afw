// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Unicode code point support
 *
 * Copyright (c) 2010-2024 Clemson University
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
 * @brief ASCII whitespace: tab, VT, FF, space.
 *
 * U+0020 is the only Zs below U+0080. LF and CR are EOL, not whitespace.
 * The code point is in U+0000–U+007F.
 */
#define AFW_CODE_POINT_ASCII_IS_WHITESPACE(_cp) \
    ((_cp) == 0x0009 || (_cp) == 0x000B || \
     (_cp) == 0x000C || (_cp) == 0x0020)

/**
 * @brief ASCII EOL: LF, CR. The code point is in U+0000–U+007F.
 */
#define AFW_CODE_POINT_ASCII_IS_EOL(_cp) \
    ((_cp) == 0x000A || (_cp) == 0x000D)

/**
 * @brief ASCII whitespace or EOL.
 */
#define AFW_CODE_POINT_ASCII_IS_WHITESPACE_OR_EOL(_cp) \
    (AFW_CODE_POINT_ASCII_IS_WHITESPACE(_cp) || \
     AFW_CODE_POINT_ASCII_IS_EOL(_cp))

/**
 * @brief C0 controls and DEL, including tab, LF, VT, FF, and CR.
 *
 * The code point is in U+0000–U+007F. C1 is not ASCII.
 */
#define AFW_CODE_POINT_ASCII_IS_CONTROL(_cp) \
    ((_cp) <= 0x1F || (_cp) == 0x7F)

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
 * Used by ks encode. Does not include Cf or Zs.
 */
AFW_DECLARE(afw_boolean_t)
afw_code_point_is_control(afw_code_point_t cp);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_CODE_POINT_H__ */
