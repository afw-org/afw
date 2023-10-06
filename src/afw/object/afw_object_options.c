// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Object Processing Options
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_object_options.c
 * @brief Adaptive object processing options.
 */

#include "afw_internal.h"


/* Content type options - no options set. */
AFW_DEFINE_CONST_DATA(afw_object_options_t)
afw_object_options_none = {0};

/* Content type options - composite only. */
AFW_DEFINE_CONST_DATA(afw_object_options_t)
afw_object_options_composite = {
    AFW_OBJECT_OPTION_metaLimited |
    AFW_OBJECT_OPTION_composite
};

/* Content type options - checkRequired + composite + normalize + defaults. */
AFW_DEFINE_CONST_DATA(afw_object_options_t)
afw_object_options_composite_normalize_defaults_required = {
    AFW_OBJECT_OPTION_checkRequired |
    AFW_OBJECT_OPTION_composite |
    AFW_OBJECT_OPTION_includeDefaultValues |
    AFW_OBJECT_OPTION_normalize
};

/* Content type options - composite only. */
AFW_DEFINE_CONST_DATA(afw_object_options_t)
afw_object_options_composite_and_defaults = {
    AFW_OBJECT_OPTION_metaLimited |
    AFW_OBJECT_OPTION_composite |
    AFW_OBJECT_OPTION_includeDefaultValues
};

/* Content type options - include only essential (parentPaths) meta. */
AFW_DEFINE_CONST_DATA(afw_object_options_t)
afw_object_options_essential = {
    AFW_OBJECT_OPTION_metaLimited
};

/*
 * Content type options - include only essential (parentPaths) meta plus
 * whitespace if available for content type.
 */
AFW_DEFINE_CONST_DATA(afw_object_options_t)
afw_object_options_essential_with_whitespace = {
    AFW_OBJECT_OPTION_metaLimited |
    AFW_OBJECT_OPTION_whitespace
};

/* Object processing options needed for reconcilable. */
AFW_DEFINE_CONST_DATA(afw_object_options_t)
afw_object_options_reconcilable_meta_property = {
    AFW_OBJECT_OPTION_metaLimited |
    AFW_OBJECT_OPTION_path
};

/* Content type options - minimal + objectType. */
AFW_DEFINE_CONST_DATA(afw_object_options_t)
afw_object_options_objectType = {
    AFW_OBJECT_OPTION_metaLimited |
    AFW_OBJECT_OPTION_objectType
};

/* Content type options - composite + parentPaths. */
AFW_DEFINE_CONST_DATA(afw_object_options_t)
afw_object_options_composite_inheritedFrom_resolvedParentPaths = {
    AFW_OBJECT_OPTION_resolvedParentPaths |
    AFW_OBJECT_OPTION_composite |
    AFW_OBJECT_OPTION_inheritedFrom
};

/* Whitespace only */
AFW_DEFINE_CONST_DATA(afw_object_options_t)
afw_object_options_whitespace = {
    AFW_OBJECT_OPTION_whitespace
};

/* Use non-standard tokens */
AFW_DEFINE_CONST_DATA(afw_object_options_t)
afw_object_options_useNonStandardTokens = {
    AFW_OBJECT_OPTION_useNonStandardTokens
};

/* Set content type options from options object. */
AFW_DEFINE(const afw_object_options_t *)
afw_object_options_set_from_object(
    const afw_object_options_t *initial_options,
    const afw_object_t *options_object,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_iterator_t *iterator;
    const afw_value_t *v;
    const afw_utf8_t *name;
    afw_object_options_t *options;

    /* Allocate options and initialize to initial options. */
    options = afw_pool_calloc_type(p, afw_object_options_t, xctx);
    if (initial_options) {
        memcpy(options, initial_options, sizeof(afw_object_options_t));
    }

    /** @fixme May put in at some point ... Process options property first.
    v = afw_object_get_property(options_object, &afw_self_s_options, xctx);
    if (v) {

    }
     */

    /* Handle other properties in any order. */
    for (iterator = NULL; ;) {
        v = afw_object_get_next_property(options_object, &iterator,
            &name, xctx);
        if (!v) break;

        /** @fixme May put in at some point ...  options property already handled.
        if (afw_utf8_equal(name, &afw_self_s_options)) continue;
         */

        /* checkRequired */
        if (afw_utf8_equal(name, &afw_self_s_checkRequired)) {
            AFW_OBJECT_OPTION_SET(options, checkRequired,
                afw_value_as_boolean(v, xctx));
            AFW_OBJECT_OPTION_SET(options, normalize,
                afw_value_as_boolean(v, xctx));
        }

        /* composite */
        else if (afw_utf8_equal(name, &afw_self_s_composite)) {
            AFW_OBJECT_OPTION_SET(options, composite,
                afw_value_as_boolean(v, xctx));
        }

        /* includeDefaultValues */
        else if (afw_utf8_equal(name, &afw_self_s_includeDefaultValues)) {
            AFW_OBJECT_OPTION_SET(options, includeDefaultValues,
                afw_value_as_boolean(v, xctx));
            AFW_OBJECT_OPTION_SET(options, normalize,
                afw_value_as_boolean(v, xctx));
        }

        /* includeDescendentObjectTypes */
        else if (afw_utf8_equal(name, &afw_self_s_includeDescendentObjectTypes)) {
            AFW_OBJECT_OPTION_SET(options, includeDescendentObjectTypes,
                afw_value_as_boolean(v, xctx));
        }

        /* inheritedFrom */
        else if (afw_utf8_equal(name, &afw_self_s_inheritedFrom)) {
            AFW_OBJECT_OPTION_SET(options, inheritedFrom,
                afw_value_as_boolean(v, xctx));
        }

        /* integersAsString */
        else if (afw_utf8_equal(name, &afw_self_s_integersAsString)) {
            AFW_OBJECT_OPTION_SET(options, integersAsString,
                afw_value_as_boolean(v, xctx));
        }

        /* largeIntegersAsString */
        else if (afw_utf8_equal(name, &afw_self_s_largeIntegersAsString)) {
            AFW_OBJECT_OPTION_SET(options, largeIntegersAsString,
                afw_value_as_boolean(v, xctx));
        }

        /* metaFull */
        else if (afw_utf8_equal(name, &afw_self_s_metaFull)) {
            AFW_OBJECT_OPTION_SET(options, metaFull,
                afw_value_as_boolean(v, xctx));
            if (AFW_OBJECT_OPTION_IS(options, metaFull)) {
                AFW_OBJECT_OPTION_SET_OFF(options, metaLimited);
            }
        }

        /* metaLimited */
        else if (afw_utf8_equal(name, &afw_self_s_metaLimited)) {
            if (!AFW_OBJECT_OPTION_IS(options, metaFull)) {
                AFW_OBJECT_OPTION_SET(options, metaLimited,
                    afw_value_as_boolean(v, xctx));
            }
        }

        /* normalize */
        else if (afw_utf8_equal(name, &afw_self_s_normalize)) {
            AFW_OBJECT_OPTION_SET(options, normalize,
                afw_value_as_boolean(v, xctx));
        }

        /* objectId */
        else if (afw_utf8_equal(name, &afw_self_s_objectId)) {
            AFW_OBJECT_OPTION_SET(options, objectId,
                afw_value_as_boolean(v, xctx));
        }

        /* objectType */
        else if (afw_utf8_equal(name, &afw_self_s_objectType)) {
            AFW_OBJECT_OPTION_SET(options, objectType,
                afw_value_as_boolean(v, xctx));
        }

        /* objectTypes */
        else if (afw_utf8_equal(name, &afw_self_s_objectTypes)) {
            AFW_OBJECT_OPTION_SET(options, objectTypes,
                afw_value_as_boolean(v, xctx));
            AFW_OBJECT_OPTION_SET(options, normalize,
                afw_value_as_boolean(v, xctx));
        }

        /* path */
        else if (afw_utf8_equal(name, &afw_self_s_path)) {
            AFW_OBJECT_OPTION_SET(options, path,
                afw_value_as_boolean(v, xctx));
        }

        /* pathEmbedded */
        else if (afw_utf8_equal(name, &afw_self_s_pathEmbedded)) {
            AFW_OBJECT_OPTION_SET(options, pathEmbedded,
                afw_value_as_boolean(v, xctx));
        }

        /* reconcilable */
        else if (afw_utf8_equal(name, &afw_self_s_reconcilable)) {
            AFW_OBJECT_OPTION_SET(options, reconcilable,
                afw_value_as_boolean(v, xctx));
            AFW_OBJECT_OPTION_SET(options, path,
                afw_value_as_boolean(v, xctx));
            AFW_OBJECT_OPTION_SET(options, inheritedFrom,
                afw_value_as_boolean(v, xctx));
        }

        /* resolvedParentPaths */
        else if (afw_utf8_equal(name, &afw_self_s_resolvedParentPaths)) {
            AFW_OBJECT_OPTION_SET(options, resolvedParentPaths,
                afw_value_as_boolean(v, xctx));
        }

        /* typedValues */
        else if (afw_utf8_equal(name, &afw_self_s_typedValues)) {
            AFW_OBJECT_OPTION_SET(options, typedValues,
                afw_value_as_boolean(v, xctx));
        }

        /* whitespace */
        else if (afw_utf8_equal(name, &afw_self_s_whitespace)) {
            AFW_OBJECT_OPTION_SET(options, whitespace,
                afw_value_as_boolean(v, xctx));
        }

        /* If not of above, it's an error. */
        else {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Invalid view option " AFW_UTF8_FMT_Q,
                AFW_UTF8_FMT_ARG(name));
        }
    }

    /* Return result. */
    return options;
}



/* Create a options. */
AFW_DEFINE(const afw_object_options_t *)
afw_object_options_create(
    const afw_object_options_t *initial_options,
    const afw_object_options_t *options,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_object_options_t *result;

    result = afw_memory_dup(options, sizeof(afw_object_options_t),
        p, xctx);
    if (initial_options) {
        result->mask |= initial_options->mask;
    }

    if (AFW_OBJECT_OPTION_IS(result, includeDefaultValues)) {
        AFW_OBJECT_OPTION_SET_ON(result, normalize);
    }

    return result;
}
