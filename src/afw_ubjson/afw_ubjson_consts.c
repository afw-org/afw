// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW ubjson type constants.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_ubjson_consts.c
 * @brief AFW ubjson constants.
 *
 */

#include "afw.h"
#include "afw_endian.h"
#include "afw_ubjson.h"

/* the size (in bytes) of each marker */
const int AFW_UBJSON_SIZE[128] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 1, 8, 0, 0, 0, 
    0, 2, 0, 0, 8, 0, 0, 0, 
    0, 0, 0, 0, 0, 1, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 4, 0, 0, 0, 
    0, 1, 0, 0, 4, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0
};
