// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Minimal Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_MINIMAL_H__
#define __AFW_MINIMAL_H__

/* afw_doxygen.h must be first since the other headers need its groups. */
#include "afw_doxygen.h"

/**
 * @file afw_minimal.h
 * @brief Adaptive Framework Minimal Header.
 * @ingroup afw_c_api
 *
 * The main intent of this header is to be included in other afw_*.h headers.
 * 
 * DONT PUT ANYTHING HERE THAT CAN NOT BE INCLUDED IN ALL AFW PROJECTS.
 * 
 * The headers included here only #include afw_interface.h from AFW to avoid
 * dependency issues.
 *
 * Other afw_*.h headers should #include afw_interface.h or afw_minimal.h.
 */
#include "afw_version.h"
#include "afw_interface.h"
#include "afw_os.h"
#include "afw_data_type_typedefs.h"
#include "afw_error.h"
#include "afw_list.h"
#include "afw_object.h"
#include "afw_pool.h"
#include "afw_xctx.h"
#include "afw_log.h"

#endif /* __AFW_MINIMAL_H__ */
