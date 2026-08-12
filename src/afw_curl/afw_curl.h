// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW CURL Extension Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_CURL_H__
#define __AFW_CURL_H__


/**
 * @defgroup afw_curl CURL Extension
 * @ingroup afw_included_extensions
 *
 * HTTP / SMTP Adaptive functions and curl error RV decoding (`src/afw_curl/`).
 * This **srcdir is self-contained** relative to libafw core: it uses only
 * public core APIs and owns its generate/manifest/tests layout.
 *
 * @{
 */

/**
 * @file afw_curl.h
 * @brief Public header for the curl extension (HTTP / SMTP client).
 *
 * **Example loadable extension** for authors studying how to package
 * functions and an error RV decoder on top of libafw (not an adapter type).
 *
 * Layout to mirror when creating a new extension with afwdev:
 * - `afw_curl_extension.c` — `afw_extension` initialize / register
 * - `afw_curl_function_*.c` — Adaptive function execute implementations
 * - `generate/manifest/` — extension manifest + `error_rv_decoder/curl.c`
 * - `generated/` — bindings and register (do not hand-edit)
 *
 * After load, call Adaptive functions in the curl/http/smtp categories
 * (see generated function bindings). Use call macros from core interfaces
 * for any interface work; this extension mainly exports functions.
 *
 * See @ref afw_included_extensions, @ref afw_dev_extending, and
 * @ref afw_dev_implementing.
 */

#include "afw_minimal.h"

/* Include generated declare helpers. */
AFW_BEGIN_DECLARES

/** curl rv source id */
#define AFW_ERROR_RV_SOURCE_ID_Z_curl "curl"


AFW_END_DECLARES

/** @} */


#endif /* __AFW_CURL_H__ */
