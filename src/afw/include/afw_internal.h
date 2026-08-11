// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Core Internal
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_INTERNAL_H__
#define __AFW_INTERNAL_H__


/**
 * @file afw_internal.h
 * @brief Umbrella internal header for libafw implementation files.
 * @ingroup afw_c_api_internal
 *
 * **libafw source under src/afw only** (implementation .c files). Includes
 * `afw.h` plus core `*_internal.h` and register/generated glue. May change
 * every build.
 * Not installed with the default public header set; use the source tree.
 * Extensions and commands must use `afw.h` only.
 *
 * See @ref afw_c_api_internal and `designs/libafw-headers-and-api-surface.md`.
 */

#include "afw.h"
#include "afw_generated.h"
#include "afw_adapter_internal.h"
#include "afw_application_internal.h"
#include "afw_authorization_internal.h"
#include "afw_compile_internal.h"
#include "afw_const_objects.h"
#include "afw_environment_internal.h"
#include "afw_file_internal.h"
#include "afw_flag_internal.h"
#include "afw_function_bindings.h"
#include "afw_json_internal.h"
#include "afw_array_internal.h"
#include "afw_lock_internal.h"
#include "afw_log_internal.h"
#include "afw_model_internal.h"
#include "afw_model_location.h"
#include "afw_object_internal.h"
#include "afw_object_view_internal.h"
#include "afw_pool_internal.h"
#include "afw_request_handler_internal.h"
#include "afw_runtime_object_maps.h"
#include "afw_stack_internal.h"
#include "afw_stream_internal.h"
#include "afw_xctx_internal.h"
#include "afw_service_internal.h"
#include "afw_value_internal.h"

#endif /* __AFW_INTERNAL_H__ */
