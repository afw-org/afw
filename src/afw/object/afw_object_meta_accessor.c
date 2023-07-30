// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_object Implementation for meta accessor
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_object_meta_accessor.c
 * @brief Implementation of afw_object interface for meta accessor
 */

#include "afw_internal.h"


#define impl_afw_object_get_meta \
    afw_object_impl_internal_get_meta

#define impl_afw_object_get_property_meta \
    afw_object_impl_internal_get_property_meta

#define impl_afw_object_get_next_property_meta \
    afw_object_impl_internal_get_next_property_meta


/* Declares and rti/inf defines for interface afw_object */
#define AFW_IMPLEMENTATION_ID "afw_object_meta_accessor"
#include "afw_object_impl_declares.h"



/* Create an object instance to access an object meta with options. */
AFW_DEFINE(const afw_object_t *)
afw_object_meta_create_accessor_with_options(
    const afw_object_t *instance,
    const afw_object_options_t *options,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_object_internal_meta_accessor_self_t *self;
    const afw_iterator_t *iterator;
    const afw_utf8_t *property_name;

    /* If instance of this implementation with no meta, return NULL. */
    if (!instance->meta.meta_object && instance->inf == &impl_afw_object_inf)
    {
        return NULL;
    }

    /* Create self. */
    self = afw_pool_calloc_type(p, afw_object_internal_meta_accessor_self_t, xctx);
    self->pub.inf = &impl_afw_object_inf;
    self->pub.p = p;
    self->options = options;
    self->instance = instance;
    self->embedded = (instance->meta.embedding_object != NULL);
    self->limited_meta = !afw_object_meta_object(self->instance) ||
        AFW_OBJECT_OPTION_IS(self->options, metaLimited);

    /* If there are no properties to return, return NULL. */
    iterator = NULL;
    if (!impl_afw_object_get_next_property((afw_object_t *)self,
        &iterator, &property_name, xctx))
    {
        return NULL;
    }
    self->iterator = NULL;

    /* Return new instance. */
    return (afw_object_t *)self;
}


/*
 * Implementation of method release of interface afw_object.
 */
void
impl_afw_object_release(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    /* Nothing to do. */
}



/*
 * Implementation of method add_reference of interface afw_object.
 */
void
impl_afw_object_add_reference(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    /* There is no intent to implement this method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_count for interface afw_object.
 */
afw_size_t
impl_afw_object_get_count(
    const afw_object_t * instance,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}


/*
 * Implementation of method get_property of interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_property(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    /* There is no intent to implement this method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}



/*
 * Implementation of method get_next_property of interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_next_property(
    const afw_object_t *instance,
    const afw_iterator_t **iterator,
    const afw_utf8_t **property_name,
    afw_xctx_t *xctx)
{
    afw_object_internal_meta_accessor_self_t *self =
        (afw_object_internal_meta_accessor_self_t *)instance;
    const afw_pool_t *p = self->pub.p;
    const afw_value_t *result = NULL;
    const afw_utf8_t *s;
    int state;

    /*
     * Note: The options are fairly simple atm.  If there is an option added
     *       to merge in associated _AdaptiveObjectType_ properties, multiple
     *       internal iterators will be needed.
     */

    /* Initial state is 0 and after that iterator holds state. */
    if (!*iterator) {
        *iterator = afw_pool_calloc(self->pub.p, sizeof(int), xctx);
    }
    state = *(int *)* iterator;

    /* Process state. */
    for (;;) {
        switch (state) {

        /* Initial state. */
        case 0:

            /* Next state is 1 or 2 depending on limited_meta. */
            state = (!self->limited_meta) ? 1 : 2;

            /* If limited_data, always provide parentPaths. */
            if (self->limited_meta && afw_object_meta_object(self->instance)) {
                result = afw_object_get_property(
                    afw_object_meta_object(self->instance), &afw_s_parentPaths,
                    xctx);
                if (result) {
                    if (property_name) {
                        *property_name = &afw_s_parentPaths;
                    }
                    goto return_result;
                }
            }

            /* Next state. */
            break;

        /* Not limited meta. Just return all properties in meta and end. */
        case 1:

            result = afw_object_get_next_property(
                afw_object_meta_object(self->instance),
                &self->iterator, property_name, xctx);
            if (result) {
                goto return_result;
            }
            state = 5;
            break;

        /* options = objectId. */
        case 2:

            /* Next state is 3. */
            state = 3;

            /* If not already returned by fullMetaObject, return value. */
            if (!self->embedded &&
                AFW_OBJECT_OPTION_IS(self->options, objectId))
            {
                if (self->limited_meta ||
                    !afw_object_has_property(
                        afw_object_meta_object(self->instance),
                        &afw_s_objectId, xctx))
                {
                    s = afw_object_meta_get_object_id(self->instance, xctx);
                    if (s) {
                        if (property_name) {
                            *property_name = &afw_s_objectId;
                        }
                        result = afw_value_create_string(s, p, xctx);
                        goto return_result;
                    }
                }
            }


        /* options = objectType. */
        case 3:

            /* Next state is 4. */
            state = 4;

            /*
             * If option objectType or metaLimited and this is an embedded
             * object, set meta objectType, return value.
             */
            if (AFW_OBJECT_OPTION_IS(self->options, objectType) ||
                (self->limited_meta && self->instance->meta.embedding_object))
            {
                if (self->limited_meta ||
                    !afw_object_has_property(
                        afw_object_meta_object(self->instance),
                        &afw_s_objectType, xctx))
                {
                    s = afw_object_meta_get_object_type_id(self->instance,
                        xctx);
                    if (s) {
                        if (property_name) {
                            *property_name = &afw_s_objectType;
                        }
                        result = afw_value_create_string(s, p, xctx);
                        goto return_result;
                    }
                }
            }


        /* options = path and pathEmbedded. */
        case 4:

            /* Next state is 5. */
            state = 5;

            /* If not already returned by fullMetaObject, return value. */
            if ((!self->embedded &&
                AFW_OBJECT_OPTION_IS(self->options, path))
                ||
                (self->embedded &&
                    AFW_OBJECT_OPTION_IS(self->options, pathEmbedded))
                )
            {
                if (self->limited_meta ||
                    !afw_object_has_property(
                        afw_object_meta_object(self->instance), &afw_s_path, xctx))
                {
                    s = afw_object_meta_get_path(self->instance, xctx);
                    if (s) {
                        if (property_name) {
                            *property_name = &afw_s_path;
                        }
                        result = afw_value_create_string(s, p, xctx);
                        goto return_result;
                    }
                }
            }


        /* Nothing left. */
        default:
            result = NULL;
            goto return_result;
        }
    }


    /* Return result. */
return_result:
    if (!result) {
        *iterator = NULL;
    }
    else {
        *(int *)*iterator = state;
    }
    return result;
}



/*
 * Implementation of method has_property of interface afw_object.
 */
afw_boolean_t
impl_afw_object_has_property(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    /* There is no intent to implement this method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}



/*
 * Implementation of method get_setter of interface afw_object.
 */
const afw_object_setter_t *
impl_afw_object_get_setter(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    /* There is no intent to implement this method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
