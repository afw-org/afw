// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Unicode Code Point Support
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_compile_code_point.c
 * @brief Adaptive framework unicode code point support.
 */

#include "afw_internal.h"
#include <unicode/uchar.h>
#include <unicode/utypes.h>


/* Determine if codepoint is one that can start an afw identifier. */
AFW_DEFINE(afw_boolean_t)
afw_compile_code_point_is_IdentifierStart(afw_code_point_t cp)
{
    return u_hasBinaryProperty(cp, UCHAR_ID_START)
        || cp == '$'
        || cp == '_';
}


/* Determine if codepoint is one that can continue an afw identifier. */
AFW_DEFINE(afw_boolean_t)
afw_compile_code_point_is_IdentifierContinue(afw_code_point_t cp)
{
    return u_hasBinaryProperty(cp, UCHAR_ID_CONTINUE)
        || cp == '$'
        || cp == 0x200c  /* ZWNJ */
        || cp == 0x200d; /* ZWJ */
}


/* Determine if codepoint matches AFW EOL production. */
AFW_DEFINE(afw_boolean_t)
afw_compile_code_point_is_EOL(afw_code_point_t cp)
{   
    return (   cp == 0x000A /* LF  */
            || cp == 0x000D /* CR  */
            || cp == 0x2028 /* LS  */
            || cp == 0x2029 /* PS  */
            );
}


/* Determine if codepoint matches AFW Whitespace production. */
AFW_DEFINE(afw_boolean_t)
afw_compile_code_point_is_Whitespace(afw_code_point_t cp)
{
    if (   cp == 0x0009 /* Tab */
        || cp == 0x000B /* VT */
        || cp == 0x000C /* FF */
        || cp == 0xFEFF /* ZWNBSP  */
        )
    {
        return true;
    }

    /* This function checks for TAB + Zs */
    return u_charType(cp) == U_SPACE_SEPARATOR;
}

/* Determine if codepoint matches AFW WhitespaceOrEOL production. */
AFW_DEFINE(afw_boolean_t)
afw_compile_code_point_is_WhitespaceOrEOL(afw_code_point_t cp)
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

    /* USP = SPACE_SEPARATOR = Zs */
    return u_charType(cp) == U_SPACE_SEPARATOR;
}

