// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework application
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_COMPONENTS_H__
#define __AFW_COMPONENTS_H__

#include "afw_interface.h"

/**
 * @defgroup afw_components Components
 * @ingroup afw_c_api_public
 *
 * Adaptive application components
 *
 * @{
 */

/**
 *  \file afw_components.h
 *  \brief UI components.
 */

AFW_BEGIN_DECLARES

struct afw_components_s {
    const afw_utf8_t          * object_id;
};

AFW_END_DECLARES

/** @} */

#endif /* __AFW_COMPONENTS_H__ */
