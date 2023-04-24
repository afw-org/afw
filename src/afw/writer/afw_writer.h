// See the 'COPYING' file in the project root for licensing information.
/*
 * Helpers for interfaces afw_writer*
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_WRITER_H__
#define __AFW_WRITER_H__

#include "afw_interface.h"

/**
 * @defgroup afw_writer Writer
 * @ingroup afw_c_api_public
 *
 * Provide afw_writer interface common support.
 * @{
 */

/**
 * @file afw_adaptor.h
 * @brief Header for interface afw_writer* helpers.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Call afw_writer_write() with zero terminated string.
 * @param writer instance.
 * @param s_z zero terminated string to write.
 * @param xctx of caller.
 */
#define afw_writer_write_z(writer, s_z, xctx) \
afw_writer_write(writer, s_z, strlen(s_z), xctx)


/**
 * @brief Call afw_writer_write() with a afw_utf8_t string.
 * @param writer instance.
 * @param s utf8 string.
 * @param xctx of caller.
 */
#define afw_writer_write_utf8(writer, S, xctx) \
afw_writer_write(writer, (S)->s, (S)->len, xctx)



/**
 * @brief Call afw_writer_write() with an integer.
 * @param writer instance.
 * @param integer to write as as string.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_writer_write_integer(
    const afw_writer_t *writer,
    afw_integer_t integer,
    afw_xctx_t *xctx);



/**
 * @brief Call afw_writer_write() with an size.
 * @param writer instance.
 * @param size to write as string.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_writer_write_size(
    const afw_writer_t *writer,
    afw_size_t size,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_WRITER_H__ */
