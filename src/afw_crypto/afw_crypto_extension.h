// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_extension implementation for afw_crypto
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_CRYPTO_EXTENSION_H__
#define __AFW_CRYPTO_EXTENSION_H__

/**
 * @file afw_crypto_extension.h
 * @brief Cryptographic functions for Adaptive Script
 *
 * This is the implementation of afw_extension for afw_crypto.
 */

#include "afw_interface.h"
#include "generated/afw_crypto_declare_helpers.h"

AFW_BEGIN_DECLARES

/**
 * @brief Self typedef for afw_crypto implementation of afw_extension.
 */
typedef struct
afw_crypto_extension_self_s {
    afw_extension_t pub;

} afw_crypto_extension_self_t;

AFW_END_DECLARES

#endif /* __AFW_CRYPTO_EXTENSION_H__ */
