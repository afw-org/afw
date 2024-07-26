// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Model Location
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_model_location.c
 * @brief Adaptive Framework model location
 */

#include "afw_internal.h"



/* Get object type and model objects callback. */
static afw_boolean_t
impl_get_model_object_types_cb(
    const afw_object_t *object,
    void *context,
    afw_xctx_t *xctx)
{
    const afw_adapter_t *model_location_adapter;
    afw_model_location_t *model_location;
    const afw_pool_t *p;
    afw_model_t *model;

    if (!object) {
        AFW_THROW_ERROR_Z(general, "Map not found", xctx);
    }

    model_location_adapter = context;
    p = model_location_adapter->p;

    /* Compile model and add to object types associative array. */
    model = afw_model_compile(&model_location_adapter->adapter_id,
        object, p, xctx);

    /* Add model to loaded models. */
    model_location = (afw_model_location_t *)
        model_location_adapter->impl->model_location;
    model->next_model = model_location->first_model;
    model_location->first_model = model;

    /* Return indicating not to short circuit */
    return false;
}



static const afw_model_t *
impl_load_model(
    const afw_adapter_t *model_location_adapter,
    const afw_utf8_t *model_id,
    afw_xctx_t *xctx)
{
    const afw_adapter_session_t *session;

    session = afw_adapter_create_adapter_session(
        model_location_adapter, xctx);
    AFW_TRY {

        afw_adapter_session_get_object(session, NULL,
            afw_s__AdaptiveModel_, model_id,
            (void *)model_location_adapter,
            impl_get_model_object_types_cb, NULL,
            model_location_adapter->p, xctx);

    }

    AFW_FINALLY{
        afw_adapter_session_destroy(session, xctx);
    }

    AFW_ENDTRY;

    return model_location_adapter->impl->model_location->first_model;
}


/* Create model location. */
AFW_DEFINE(const afw_model_location_t *)
afw_model_location_create(
    const afw_adapter_t *adapter,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_model_location_t *model_location;
    apr_status_t rv;

    model_location = afw_pool_calloc_type(p, afw_model_location_t, xctx);
    model_location->model_location_adapter = adapter;
    rv = apr_thread_mutex_create(&model_location->mutex,
        APR_THREAD_MUTEX_UNNESTED, afw_pool_get_apr_pool(p));
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, apr, rv,
            "apr_thread_mutex_create() failed",
            xctx);
    }

    return model_location;
}


AFW_DEFINE(const afw_model_t *)
afw_model_location_get_model(
    const afw_adapter_t *model_location_adapter,
    const afw_utf8_t *model_id,
    afw_xctx_t *xctx)
{
    const afw_model_t *model;
    afw_model_location_t *model_location;

    model = NULL;
    model_location = NULL;

    if (model_location_adapter->impl) {
        model_location = (afw_model_location_t *)
            model_location_adapter->impl->model_location;
    }

    if (model_location) {

        AFW_AUTHORIZATION_INTERMEDIATE_ACCESS_BEGIN {
            AFW_THREAD_MUTEX_LOCK(model_location->mutex, xctx) {
                for (model = (afw_model_t *)model_location->first_model;
                    model && !afw_utf8_equal(model_id, model->model_id);
                    model = (afw_model_t *)model->next_model);

                if (!model) {
                    model = impl_load_model(model_location_adapter, model_id,
                        xctx);
                }
            }
            AFW_THREAD_MUTEX_UNLOCK();
        }
        AFW_AUTHORIZATION_INTERMEDIATE_ACCESS_END;
    }

    return model;
}
