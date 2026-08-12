// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Minimal Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_MINIMAL_H__
#define __AFW_MINIMAL_H__

/* afw_doxygen.h must be first since the other headers need its groups. */
#include "afw_doxygen.h"

/**
 * @file afw_minimal.h
 * @brief Header bootstrap set (not a thinner product API for `.c` files).
 * @ingroup afw_c_api_public
 *
 * **Main intent:** include this from **other headers** that need more than
 * `afw_interface.h` alone, without pulling full `afw.h` or creating cycles.
 * `afw.h` includes this first so the include-order dance is done once.
 * **Do not** use this as a substitute for `afw.h` in `.c` files.
 *
 * DONT PUT ANYTHING HERE THAT CAN NOT BE INCLUDED IN ALL AFW PROJECTS.
 * Headers in this set only `#include` `afw_interface.h` from AFW (no peer
 * module spaghetti). Membership is stable; do not thin casually.
 *
 * Other `afw_*.h` headers should `#include` `afw_interface.h` or
 * `afw_minimal.h`. See `designs/libafw-headers-and-api-surface.md`.
 */
#include "afw_version.h"
#include "afw_interface.h"
#include "afw_os.h"
#include "afw_data_type_typedefs.h"
#include "afw_error.h"
#include "afw_array.h"
#include "afw_object.h"
#include "afw_pool.h"
#include "afw_xctx.h"
#include "afw_log.h"

#endif /* __AFW_MINIMAL_H__ */
