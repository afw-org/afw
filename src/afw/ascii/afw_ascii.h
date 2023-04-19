// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework ASCII
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_ASCII_H__
#define __AFW_ASCII_H__


/** @defgroup afw_ascii ASCII
 * @ingroup afw_c_api_public
 * @{
 */

/**
 * @file afw_ascii.h
 * @brief Adaptive Framework ASCII
 *
 * These also work for UNICODE codepoints, since ASCII is a subset.
 */

#include "afw_minimal.h"

AFW_BEGIN_DECLARES

/** @brief NULL character */
#define AFW_ASCII_NUL         0x00

/** @brief Start of Heading */
#define AFW_ASCII_SOH         0x01

/** @brief Start of Text */
#define AFW_ASCII_STX         0x02

/** @brief End of Text */
#define AFW_ASCII_ETX         0x03

/** @brief End of Transmission */
#define AFW_ASCII_EOT         0x04

/** @brief ENQuiry */
#define AFW_ASCII_ENQ         0x05

/** @brief ACKnowledge */
#define AFW_ASCII_ACK         0x06

/** @brief Bell */
#define AFW_ASCII_BEL         0x07

/** @brief BackSpace (non-destructive) */
#define AFW_ASCII_BS          0x08

/** @brief Horizontal Tab */
#define AFW_ASCII_HT          0x09

/** @brief Line Feed */
#define AFW_ASCII_LF          0x0A

/** @brief Vertical Tab */
#define AFW_ASCII_VT          0x0B

/** @brief Form Feed */
#define AFW_ASCII_FF          0x0C

/** @brief Carriage Return */
#define AFW_ASCII_CR          0x0D

/** @brief Shift Out */
#define AFW_ASCII_SO          0x0E

/** @brief Shift In */
#define AFW_ASCII_SI          0x0F

/** @brief Data Link Escape */
#define AFW_ASCII_DLE         0x10

/** @brief Device Control 1 (XON) */
#define AFW_ASCII_DC1         0x11

/** @brief Device Control 2 */
#define AFW_ASCII_DC2         0x12

/** @brief Device Control 3 (XOFF) */
#define AFW_ASCII_DC3         0x13

/** @brief Device Control 4 */
#define AFW_ASCII_DC4         0x14

/** @brief Negative Acknowledge */
#define AFW_ASCII_NAK         0x15

/** @brief Synchronize idle */
#define AFW_ASCII_SYN         0x16

/** @brief End Transmission Block */
#define AFW_ASCII_ETB         0x17

/** @brief Cancel Line */
#define AFW_ASCII_CAN         0x18

/** @brief End of Media */
#define AFW_ASCII_EM          0x19

/** @brief Substitute */
#define AFW_ASCII_SUB         0x1A

/** @brief Escape */
#define AFW_ASCII_ESC         0x1B

/** @brief File Separator */
#define AFW_ASCII_FS          0x1C

/** @brief Group Separator */
#define AFW_ASCII_GS          0x1D

/** @brief Record Separator */
#define AFW_ASCII_RS          0x1E

/** @brief Unit Separator */
#define AFW_ASCII_US          0x1F

/** @brief Delete */
#define AFW_ASCII_DELETE      0x7F

/** @brief Path separator */
#if defined(WIN32)
#define AFW_ASCII_Z_PATH_SEPARATOR  "\\"
#define AFW_ASCII_PATH_SEPARATOR    '\\'
#else
#define AFW_ASCII_Z_PATH_SEPARATOR  "/"
#define AFW_ASCII_PATH_SEPARATOR    '/'
#endif

/**
 * @brief Determine if octet is ascii printable character.
 * @param v is octet to test.
 * @return result of test.
 */
#define afw_ascii_is_printable(v) \
    ( ((v)>=AFW_ASCII_SPACE) && ((v)<=AFW_ASCII_TILDE) )


/**
 * @brief Determine if octet is ascii lowercase character.
 * @param v is octet to test.
 * @return result of test.
 */
#define afw_ascii_is_lowercase(v) \
    ( ((v) >= 'a') && ((v) <= 'z') )


/**
 * @brief Determine if octet is ascii uppercase character.
 * @param v is octet to test.
 * @return result of test.
 */
#define afw_ascii_is_uppercase(v) \
    ( ((v) >= 'A') && ((v) <= 'Z') )


/**
 * @brief Determine if octet is ascii alpha character.
 * @param v is octet to test.
 * @return result of test.
 */
#define afw_ascii_is_alpha(v) \
    ( afw_ascii_is_ascii_lowercase(v) || afw_ascii_is_ascii_uppercase(v) )


/**
 * @brief Determine if octet is ascii digit character.
 * @param v is octet to test.
 * @return result of test.
 */
#define afw_ascii_is_alphanumeric(v) \
    ( afw_ascii_is_ascii_alpha(v) || ( ((v)>='0') && ((v)<='9') ) )


/**
 * @brief Determine if octet is ascii control character.
 * @param v is octet to test.
 * @return result of test.
 */
#define afw_ascii_is_control_character(v) \
    ( ((v) <= 31) || ((v) == AFW_ASCII_DELETE) )


/**
 * @brief Determine if octet is digit character.
 * @param v is octet to test.
 * @return result of test.
 */
#define afw_ascii_is_digit(v) \
    ( ((v)>='0') && ((v)<='9') )



/**
 * @brief Decode ascii hex digit.
 * @param octet contain ascii hex digit.
 * @return octet containing binary value of hex digit or -1 if error.
 */
AFW_DEFINE(afw_octet_t)
afw_ascii_decode_hex_digit(afw_octet_t octet);



/**
 * @brief Determine if octet is ascii hex digit.
 * @param v is octet to test.
 * @return result of test.
 */
#define afw_ascii_is_hex_digit(v) \
    ( afw_ascii_decode_hex_digit(v) != -1 )


/**
 * @brief Encode ascii hex digit.
 * @param octet containing binary value 0 through 15.
 * @return octet containing ascii encoded hex digit or -1 if error.
 */
AFW_DEFINE(afw_octet_t)
afw_ascii_encode_hex_digit(afw_octet_t octet);



/**
 * @brief Determine if octet is white space.
 * @param v is octet to test.
 * @return result of test.
 */
#define afw_ascii_is_whitespace(v) \
    ((v) == 32 || ((v) >= 9 && (v) <= 13))


AFW_END_DECLARES

/** @} */

#endif /* __AFW_ASCII_H__ */

