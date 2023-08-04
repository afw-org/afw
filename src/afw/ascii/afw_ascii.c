// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Uniform Resource Identifier (URI) helpers
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_ascii.c
 * @brief Adaptive Framework ASCII support.
 */

#include "afw_internal.h"

static const afw_octet_t
impl_hex_digits[16] =
{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

static const afw_octet_t
impl_hex_lookup[] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,    /* 16 */
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,    /* 32 */
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,    /* 48 */
    0,     1,   2,   3,   4,   5,   6,   7,   8,   9, 255, 255, 255, 255, 255, 255,    /* 64 */
    255,  10,  11,  12,  13,  14,  15, 255, 255, 255, 255, 255, 255, 255, 255, 255,    /* 80 */
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,    /* 96 */
    255, 10, 11, 12, 13, 14, 15 };

AFW_DEFINE(afw_octet_t)
afw_ascii_decode_hex_digit(afw_octet_t octet)
{
    return (octet >= sizeof(impl_hex_lookup))
        ? 255
        : impl_hex_lookup[octet];
}

AFW_DEFINE(afw_octet_t)
afw_ascii_encode_hex_digit(afw_octet_t octet)
{
    return (octet >= 16)
        ? 255
        : impl_hex_digits[octet];
}
