// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Unicode code point support header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_CODE_POINT_H__
#define __AFW_CODE_POINT_H__

#include "afw_interface.h"

/**
 * @defgroup afw_code_point Unicode code point support
 * @ingroup afw_c_api_public
 *
 * Unicode code point support.
 *
 * @{
 */

/**
 * @file afw_compile_code_point.h
 * @brief Adaptive Framework unicode code point support header.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Determine if codepoint matches AFW IdentifierStart production.
 * @param cp is a Unicode codepoint to test.
 * @return result of test.
 */
AFW_DECLARE(afw_boolean_t)
afw_compile_code_point_is_IdentifierStart(afw_code_point_t cp);

/**
 * @brief Determine if codepoint matches AFW IdentifierContinue production.
 * @param cp is a Unicode codepoint to test.
 * @return result of test.
 */
AFW_DECLARE(afw_boolean_t)
afw_compile_code_point_is_IdentifierContinue(afw_code_point_t cp);


/**
 * @brief Determine if codepoint matches AFW EOL production.
 * @param cp code point
 * @returns true if match
 */
AFW_DECLARE(afw_boolean_t)
afw_compile_code_point_is_EOL(afw_code_point_t cp);



/**
 * @brief Determine if codepoint matches Whitespace production.
 * @param cp code point
 * @returns true if match
 */
AFW_DECLARE(afw_boolean_t)
afw_compile_code_point_is_Whitespace(afw_code_point_t cp);


/**
 * @brief Determine if codepoint matches AFW Whitespace or EOL productions.
 * @param cp code point
 * @returns true if match
 */
AFW_DECLARE(afw_boolean_t)
afw_compile_code_point_is_WhitespaceOrEOL(afw_code_point_t cp);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_CODE_POINT_H__ */
