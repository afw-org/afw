// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Unicode code point support
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

/**
 * @file afw_code_point.c
 * @brief Unicode code-point property tests (ICU lives here and in utf8).
 */

#include "afw_internal.h"
#include <unicode/uchar.h>
#include <unicode/utypes.h>


AFW_DEFINE(afw_boolean_t)
afw_code_point_is_identifier_start(afw_code_point_t cp)
{
    return u_hasBinaryProperty(cp, UCHAR_ID_START)
        || cp == '$'
        || cp == '_';
}


AFW_DEFINE(afw_boolean_t)
afw_code_point_is_identifier_continue(afw_code_point_t cp)
{
    return u_hasBinaryProperty(cp, UCHAR_ID_CONTINUE)
        || cp == '$'
        || cp == 0x200c  /* ZWNJ */
        || cp == 0x200d; /* ZWJ */
}


AFW_DEFINE(afw_boolean_t)
afw_code_point_is_eol(afw_code_point_t cp)
{
    return (   cp == 0x000A /* LF  */
            || cp == 0x000D /* CR  */
            || cp == 0x2028 /* LS  */
            || cp == 0x2029 /* PS  */
            );
}


AFW_DEFINE(afw_boolean_t)
afw_code_point_is_whitespace(afw_code_point_t cp)
{
    if (   cp == 0x0009 /* Tab */
        || cp == 0x000B /* VT */
        || cp == 0x000C /* FF */
        || cp == 0xFEFF /* ZWNBSP  */
        )
    {
        return true;
    }

    /* TAB + Zs */
    return u_charType(cp) == U_SPACE_SEPARATOR;
}


AFW_DEFINE(afw_boolean_t)
afw_code_point_is_whitespace_or_eol(afw_code_point_t cp)
{
    if (   cp == 0x0009 /* Tab */
        || cp == 0x000B /* VT */
        || cp == 0x000C /* FF */
        || cp == 0xFEFF /* ZWNBSP  */
        || cp == 0x000A /* LF  */
        || cp == 0x000D /* CR  */
        || cp == 0x2028 /* LS  */
        || cp == 0x2029 /* PS  */
        )
    {
        return true;
    }

    return u_charType(cp) == U_SPACE_SEPARATOR;
}


AFW_DEFINE(afw_boolean_t)
afw_code_point_is_control(afw_code_point_t cp)
{
    return u_charType(cp) == U_CONTROL_CHAR;
}
