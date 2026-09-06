// See the 'COPYING' file in the project root for licensing information.
/*
 * Helpers for afw_adapter implementation index development
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_adapter_impl_index.c
 * @brief Helpers for afw_adapter implementation index development
 */

#include "afw_internal.h"
#include "afw_adapter_impl_index.h"


/* -------------------------------------------------------------------------
 * current:: variables for index filter/value evaluation (issue #54)
 * ------------------------------------------------------------------------- */

typedef struct {
    const afw_value_t *object;
    const afw_value_t *objectId;
    const afw_value_t *objectType;
    const afw_value_t *key;
} impl_index_eval_ctx_t;


static const afw_value_t *
impl_index_current_object_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    impl_index_eval_ctx_t *ctx = entry->data;

    (void)name;
    (void)xctx;
    return ctx->object;
}

static const afw_value_t *
impl_index_current_objectId_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    impl_index_eval_ctx_t *ctx = entry->data;

    (void)name;
    (void)xctx;
    return ctx->objectId;
}

static const afw_value_t *
impl_index_current_objectType_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    impl_index_eval_ctx_t *ctx = entry->data;

    (void)name;
    (void)xctx;
    return ctx->objectType;
}

static const afw_value_t *
impl_index_current_key_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    impl_index_eval_ctx_t *ctx = entry->data;

    (void)name;
    (void)xctx;
    return ctx->key;
}


static const afw_context_cb_variable_meta_t
impl_index_current_meta_object =
{
    &afw_self_s_object,
    &afw_value_unmanaged_object_inf,
    &afw_data_type_object_direct,
    "Object"
};

static const afw_context_cb_variable_t
impl_index_current_variable_object = {
    &impl_index_current_meta_object,
    "The object being indexed.",
    impl_index_current_object_cb,
    1
};

static const afw_context_cb_variable_meta_t
impl_index_current_meta_objectId =
{
    &afw_self_s_objectId,
    &afw_value_unmanaged_string_inf,
    &afw_data_type_string_direct,
    "Object Id"
};

static const afw_context_cb_variable_t
impl_index_current_variable_objectId = {
    &impl_index_current_meta_objectId,
    "The object id of the object being indexed.",
    impl_index_current_objectId_cb,
    1
};

static const afw_context_cb_variable_meta_t
impl_index_current_meta_objectType =
{
    &afw_self_s_objectType,
    &afw_value_unmanaged_string_inf,
    &afw_data_type_string_direct,
    "Object Type"
};

static const afw_context_cb_variable_t
impl_index_current_variable_objectType = {
    &impl_index_current_meta_objectType,
    "The object type id of the object being indexed.",
    impl_index_current_objectType_cb,
    1
};

static const afw_context_cb_variable_meta_t
impl_index_current_meta_key =
{
    &afw_self_s_key,
    &afw_value_unmanaged_string_inf,
    &afw_data_type_string_direct,
    "Index Key"
};

static const afw_context_cb_variable_t
impl_index_current_variable_key = {
    &impl_index_current_meta_key,
    "The index definition key (query name / default property name).",
    impl_index_current_key_cb,
    1
};

static const afw_context_cb_variable_t * const
impl_index_current_variables[] = {
    &impl_index_current_variable_object,
    &impl_index_current_variable_objectId,
    &impl_index_current_variable_objectType,
    &impl_index_current_variable_key,
    NULL
};


/*
 *
 * An index definition looks like:
 *
 *  "FullName" : {
 *      "value"      : "string_concat(current::object.get('givenName'), current::object.get('surname'))"
 *      "objectType" : ['Person']
 *      "filter"     : "eq(current::object.get('department'), 'ENG')"
 *      "options"    : [ "case-insensitive-string" ]
 *  }
 *
 *  Filter and value are Adaptive scripts (expression-like: they must return a
 *  value). While they evaluate, issue #54 exposes current:: variables:
 *
 *      current::object      — the object under index
 *      current::objectId    — its object id
 *      current::objectType  — its object type id
 *      current::key         — this definition's key (query / default property name)
 *
 *  The "key" property is the named identifier for the property, which is
 *      used as a reference in a retrieve_objects() query.
 *
 *  The "value" property is the computed value(s) for the index, which
 *      is described by an Adaptive script (or omitted to index property `key`).
 *
 *  The "objectType" property optionally determines a list of applicable objectType(s)
 *      this index definition. An empty array implies any/all objectType's.
 *
 *  The "filter" property optionally determines if a given object is applicable for
 *      this index definition (script must return boolean).
 *
 *  The "options" property is a list of options for how the index needs to
 *      be used.  Implemented options:
 *
 *          unique                  - reject/skip duplicate index values
 *          sort-reverse            - store index values in reverse order
 *          case-insensitive-string - fold string values for comparison
 *
 *      Aspirational (parsed nowhere / not yet implemented):
 *
 *          presence
 *          case-sensitive-string
 *          starts-with
 *          ends-with
 *          contains
 *          geospatial
 *          soundex
 *
 */


/*
 * Convenience routine to check if an indexDefinition contains
 * the necessary objectType specifiers to allow the provided
 * object_type_id to pass.  The objectType property allows 
 * for a list of values:
 *
 * [] - an empty (or no list) implies all objectType's
 *
 * [ 'Person', 'eduPerson' ] - specifies two applicable
 *   objectTypes.
 *
 */
afw_boolean_t afw_adapter_impl_index_object_type_applicable(
    const afw_object_t * indexDefinition,
    const afw_utf8_t   * object_type_id,
    afw_xctx_t        * xctx)
{
    const afw_utf8_t   * nextObjectType;
    const afw_array_t   * objectTypes;
    const afw_iterator_old_t * object_type_iterator;

    objectTypes = afw_object_get_property_as_array_internal(
        indexDefinition, afw_v_objectType, xctx);

    /* no objectTypes means all/any are applicable */
    if (objectTypes == NULL)
        return true;

    object_type_iterator = NULL;
    nextObjectType = afw_array_of_string_get_next(
        objectTypes, &object_type_iterator, xctx);
    if (nextObjectType == NULL)
        return true;

    /* loop until we find a matching objectType */
    while (nextObjectType) {
        if (afw_utf8_equal(nextObjectType, object_type_id))
            return true;
 
        nextObjectType = afw_array_of_string_get_next(
            objectTypes, &object_type_iterator, xctx);
    }

    return false;
}

/*
 * This routine uses the configured filter on the indexDefinition to determine
 * if the object in question passes the filter test.  Caller must push current::
 * (see afw_adapter_impl_index_try) before calling when a filter script is used.
 * A filter is an Adaptive script that must evaluate to a single boolean value.
 * If the filter is omitted, then the default is 'true'.
 */
afw_boolean_t afw_adapter_impl_index_filter_applicable(
    const afw_object_t * object,
    const afw_object_t * indexDefinition,
    afw_xctx_t         * xctx)
{
    const afw_utf8_t  * filter;
    const afw_value_t * filterValue;
    const afw_value_t * eval;

    /* if we have a filter, evaluate it to decide if we should apply this indexDefinition */
    filter = afw_object_get_property_as_string_internal(
        indexDefinition, afw_v_filter, xctx);
    if (filter) {
        filterValue = afw_compile_to_value(filter,  NULL,
            afw_compile_type_script, NULL, NULL, object->p, xctx);
    } else {
        /* No filter means it always passes */
        return true;
    }

    eval = afw_value_evaluate(filterValue, object->p, xctx);
    if (afw_value_is_boolean(eval)) {
        if (afw_value_as_boolean_internal(eval, xctx)) {
            /* the filter expression evaluated to true */
            return true;
        }
    } else {
        /* the filter expression did not evaluate to a boolean expression */
        AFW_THROW_ERROR_Z(general,
            "Error: filter evaluation did not end with a boolean result.", 
            xctx);
    }

    return false;
}

/*
 * A handy routine to check of an indexDefinition is case-insensitive.
 * This will run through the "options" property, looking for the
 * 'case-insensitive' string.
 *
 */
afw_boolean_t afw_adapter_impl_index_option_case_insensitive(
    const afw_object_t * indexDefinition,
    afw_xctx_t        * xctx)
{
    const afw_array_t  * options;
    const afw_value_t * option;
    const afw_iterator_old_t * option_iterator;

    options = afw_object_get_property_as_array_internal(
        indexDefinition, afw_v_options, xctx);
   
    if (options) { 
        option_iterator = NULL;
        option = afw_array_get_next_value(options, &option_iterator,
            xctx->p, xctx);
        while (option) {
            if (afw_value_is_string(option)) {
                const afw_utf8_t *option_str =
                    afw_value_convert_to_utf8(option, xctx->p, xctx);

                if (afw_utf8_equal(option_str, afw_s_case_insensitive_string))
                    return true;
            }

            option = afw_array_get_next_value(
                options, &option_iterator, xctx->p, xctx);
        }
    }

    /* default to false, as case-sensitive is the default */
    return false;
}

/*
 * A handy routine to check of an indexDefinition is unique.
 * This will run through the "options" property, looking for the
 * 'unique' string.
 *
 */
afw_boolean_t afw_adapter_impl_index_option_unique(
    const afw_object_t * indexDefinition,
    afw_xctx_t        * xctx)
{
    const afw_array_t  * options;
    const afw_value_t * option;
    const afw_iterator_old_t * option_iterator;

    options = afw_object_get_property_as_array_internal(
        indexDefinition, afw_v_options, xctx);
  
    if (options) { 
        option_iterator = NULL;
        option = afw_array_get_next_value(options, &option_iterator,
            xctx->p, xctx);
        while (option) {
            if (afw_value_is_string(option)) {
                const afw_utf8_t *option_str = afw_value_convert_to_utf8(option,
                    xctx->p, xctx);

                if (afw_utf8_equal(option_str, afw_s_unique))
                    return true;
            }

            option = afw_array_get_next_value(
                options, &option_iterator, xctx->p, xctx);
        }
    }

    /* default to false, as case-sensitive is the default */
    return false;
}

/*
 * Encodes a signed 64-bit integer as a fixed-width (20-digit),
 * zero-padded decimal string whose byte-lexicographic order matches
 * numeric order.
 *
 * Index keys are stored/compared as plain text (see
 * impl_index_value_as_key_utf8), so a naive decimal rendering sorts
 * wrong ("100" < "9" as text). Flipping the sign bit maps the full
 * afw_integer_t range onto 0..UINT64_MAX while preserving order (the
 * standard two's-complement-to-offset-binary trick); zero-padding
 * that to a fixed width then makes text comparison match numeric
 * comparison (issue #251).
 */
static const afw_utf8_t *
impl_index_integer_as_sortable_utf8(
    afw_integer_t value, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_uint64_t offset;
    char buf[20];
    int i;

    offset = (afw_uint64_t)value ^ ((afw_uint64_t)1 << 63);

    for (i = 19; i >= 0; i--) {
        buf[i] = (char)('0' + (offset % 10));
        offset /= 10;
    }

    return afw_utf8_create(buf, sizeof(buf), p, xctx);
}

/*
 * Encodes an IEEE 754 double as a fixed-width, zero-padded decimal
 * string whose byte-lexicographic order matches numeric order.
 *
 * Same sortable-text goal as impl_index_integer_as_sortable_utf8, but
 * doubles need the standard IEEE-754-bit-pattern trick instead of a
 * sign-bit flip: for a non-negative double (sign bit 0), setting the
 * sign bit pushes it above all negative doubles; for a negative
 * double (sign bit 1), flipping every bit reverses its (otherwise
 * backwards) magnitude order and pushes it below all non-negative
 * doubles. The result is a uint64 whose unsigned order matches the
 * double's numeric order, which is then zero-padded as text the same
 * way (issue #251).
 */
static const afw_utf8_t *
impl_index_double_as_sortable_utf8(
    double value, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_uint64_t bits;
    char buf[20];
    int i;

    memcpy(&bits, &value, sizeof(bits));
    bits = (bits & ((afw_uint64_t)1 << 63))
        ? ~bits
        : (bits | ((afw_uint64_t)1 << 63));

    for (i = 19; i >= 0; i--) {
        buf[i] = (char)('0' + (bits % 10));
        bits /= 10;
    }

    return afw_utf8_create(buf, sizeof(buf), p, xctx);
}

/*
 * Returns the utf8 text used as an index key/comparison value for
 * `value`. Integer and double values get the fixed-width sortable
 * encoding above so lt/le/gt/ge walk keys in numeric order; LMDB (and
 * afw_utf8_compare) order index text byte-lexicographically, which
 * does not match numeric order for plain decimal text (issue #251).
 * Other types keep the existing plain-text representation, which is
 * already naturally sortable (e.g. zero-padded ISO 8601 dateTime).
 */
static const afw_utf8_t *
impl_index_value_as_key_utf8(
    const afw_value_t *value, const afw_pool_t *p, afw_xctx_t *xctx)
{
    if (afw_value_is_integer(value)) {
        return impl_index_integer_as_sortable_utf8(
            afw_value_as_integer_internal(value, xctx), p, xctx);
    }

    if (afw_value_is_double(value)) {
        return impl_index_double_as_sortable_utf8(
            afw_value_as_double_internal(value, xctx), p, xctx);
    }

    return afw_value_convert_to_utf8(value, p, xctx);
}

/*
 * When we need to add or remove an index value, this routine
 * will determine the appropriate way to do so, depending on
 * the indexDefinition options.
 */
void afw_adapter_impl_index_apply(
    const afw_adapter_impl_index_t * instance,
    const afw_object_t * indexDefinition,
    const afw_utf8_t   * object_type_id,
    const afw_utf8_t   * object_id,
    const afw_object_t * object,
    const afw_utf8_t   * key,
    const afw_value_t  * value,
    const int            operation,
    afw_xctx_t        * xctx)
{
    afw_boolean_t       case_sensitive;
    afw_boolean_t       unique;
    const afw_utf8_t  * value_string;

    case_sensitive = !afw_adapter_impl_index_option_case_insensitive(
        indexDefinition, xctx);
    unique = afw_adapter_impl_index_option_unique(indexDefinition, xctx);

    /* Generate the utf8 value of the index; this will be used as the
        index key in the underlying database.
     */
    value_string = impl_index_value_as_key_utf8(value, object->p, xctx);
    if (!case_sensitive) {
        /* For case-insensitive indexes, lower-case it so we will always
            do case-insensitive comparisons */
        value_string = afw_utf8_to_lower(value_string, object->p, xctx);
    }

    /* figure out which operation we are doing */
    if (operation == afw_adapter_impl_index_mode_add) {
        afw_adapter_impl_index_add(instance, object_type_id,
            object_id, key, value_string, unique, object->p, xctx);
    }

    else if (operation == afw_adapter_impl_index_mode_delete) {
        afw_adapter_impl_index_delete(instance, object_type_id,
            object_id, key, value_string, object->p, xctx);
    }
}

/*
 * This routine takes an object and a possible indexDefinition
 * and tries to add/remove the index, if it's applicable.  It will
 * return true if it was successful in and false otherwise.
 *
 * This routine was broken out, because it's used by both the 
 * adaptive function index routines, and by adapter session
 * routines.
 */
afw_boolean_t afw_adapter_impl_index_try(
    const afw_adapter_impl_index_t * instance,
    const afw_utf8_t   * key,
    const afw_object_t * object,
    const afw_utf8_t   * object_type_id,
    const afw_utf8_t   * object_id,
    const afw_object_t * indexDefinition,
    afw_adapter_impl_index_mode_t operation,
    afw_xctx_t        * xctx)
{
    const afw_value_t  * const *index_values;
    const afw_value_t  * index_value;
    afw_boolean_t        indexed = false;
    const afw_utf8_t   * value_expression;
    const afw_value_t  * value;
    const afw_value_t  * eval;
    impl_index_eval_ctx_t eval_ctx;
    int top;
    int i;

    /* first we make sure the objectType is applicable */
    if (!afw_adapter_impl_index_object_type_applicable(
        indexDefinition, object_type_id, xctx)) {
        /* the object_type_id does not apply for this definition */
        return false;
    }

    /*
     * Push current:: for filter and value scripts (issue #54). Always restore
     * the qualifier stack, including on error or early filter fail.
     */
    top = afw_xctx_qualifier_stack_top_get(xctx);
    AFW_TRY {

        eval_ctx.object = afw_value_create_unmanaged_object(
            object, object->p, xctx);
        eval_ctx.objectId = (object_id)
            ? afw_value_create_unmanaged_string(object_id, object->p, xctx)
            : NULL;
        eval_ctx.objectType = (object_type_id)
            ? afw_value_create_unmanaged_string(object_type_id, object->p, xctx)
            : NULL;
        eval_ctx.key = (key)
            ? afw_value_create_unmanaged_string(key, object->p, xctx)
            : NULL;

        afw_context_push_cb_variables(afw_s_current,
            impl_index_current_variables, &eval_ctx, object->p, xctx);

        if (!afw_adapter_impl_index_filter_applicable(
            object, indexDefinition, xctx))
        {
            /* the filter expression did not pass */
            break;
        }

        /* generate an index value based on the provided expression */
        value_expression = afw_object_get_property_as_string_internal(
            indexDefinition, afw_v_value, xctx);
        if (value_expression)
        {
            value = afw_compile_to_value(value_expression, NULL,
                afw_compile_type_script, NULL, NULL, object->p, xctx);

            /*
             * Evaluate the 'value' definition for the index.  This may generate
             * one or more afw_value_t types.  For each one, generate/remove an
             * index.
             */
            eval = afw_value_evaluate(value, object->p, xctx);
        }
        else
        {
            /* no value script: index property with the same name as the key */
            if (key) {
                const afw_value_string_t key_value =
                    AFW_VALUE_STRING_UNMANAGED(key);
                eval = afw_object_get_property(object, &key_value.pub, xctx);
            }
            else {
                eval = NULL;
            }
        }

        /* if eval is nullish, then we didn't generate a value and shouldn't index */
        if (afw_value_is_nullish(eval))
        {
            /* nothing to do */
        }

        /* Check the type of afw_value_t we got back. */
        else if (afw_value_is_object(eval))
        {
            /* we can't use an object as an index key */
            AFW_THROW_ERROR_Z(general,
                "Error: value expression generated an object and cannot be used as an index.",
                xctx);
        }

        /* if we have multiple values, then index each one */
        else if (afw_value_is_array(eval))
        {
            index_values = afw_value_to_null_terminated_values(
                eval, object->p, xctx);
            for (i = 0; index_values[i]; i++) {
                index_value = index_values[i];

                afw_adapter_impl_index_apply(instance, indexDefinition,
                    object_type_id, object_id, object, key, index_value,
                    operation, xctx);
            }
            indexed = true;
        }

        /* a single value can be converted to a single utf8 string */
        else if (afw_value_is_defined_and_evaluated(eval))
        {
            /* scalar value */
            afw_adapter_impl_index_apply(instance, indexDefinition,
                object_type_id, object_id, object, key, eval, operation, xctx);
            indexed = true;
        }

        else {
            AFW_THROW_ERROR_Z(general,
                "Error: value expression generated an unknown and unhandled index value.",
                xctx);
        }
    }
    AFW_FINALLY {
        afw_xctx_qualifier_stack_top_set(top, xctx);
    }
    AFW_ENDTRY;

    return indexed;
}

void afw_adapter_impl_index_open_definition(
    const afw_adapter_impl_index_t * indexer,
    const afw_utf8_t               * key,
    const afw_object_t             * indexDefinition,
    const afw_pool_t               * pool,
    afw_xctx_t                    * xctx)
{
    const afw_array_t *objectType;
    const afw_array_t *options;
    const afw_value_t *option;
    const afw_iterator_old_t *option_iterator;
    const afw_iterator_old_t *object_type_iterator;
    const afw_utf8_t *object_type_id;
    afw_boolean_t unique  = false;
    afw_boolean_t reverse = false;

    options = afw_object_get_property_as_array_internal(
        indexDefinition, afw_v_options, xctx);
    if (options) {
        option_iterator = NULL;
        option = afw_array_get_next_value(options, &option_iterator,
            xctx->p, xctx);
        while (option) {
            if (afw_value_is_string(option)) {
                const afw_utf8_t *option_str = afw_value_convert_to_utf8(option,
                    xctx->p, xctx);

                if (afw_utf8_equal(option_str, afw_s_sort_reverse))
                    reverse = true;
                else if (afw_utf8_equal(option_str, afw_s_unique))
                    unique = true;
            }

            option = afw_array_get_next_value(
                options, &option_iterator, xctx->p, xctx);
        }
    }

    objectType = afw_object_get_property_as_array_internal(
        indexDefinition, afw_v_objectType, xctx);

    if (objectType) {
        object_type_iterator = NULL;
        object_type_id = afw_array_of_string_get_next(
            objectType, &object_type_iterator, xctx);
        while (object_type_id) {
            afw_adapter_impl_index_open(indexer, object_type_id,
                key, unique, reverse, pool, xctx);

            object_type_id = afw_array_of_string_get_next(
                objectType, &object_type_iterator, xctx);
        }
    } else {
        afw_adapter_impl_index_open(indexer, NULL, key,
            unique, reverse, pool, xctx);
    }
}

AFW_DEFINE(void) afw_adapter_impl_index_open_definitions(
    const afw_adapter_impl_index_t * indexer,
    const afw_object_t             * indexDefinitions,
    const afw_pool_t               * pool,
    afw_xctx_t                    * xctx)
{
    const afw_object_t * indexDefinition;
    const afw_iterator_old_t * index_iterator;
    const afw_value_t  * key;

    index_iterator = NULL;
    indexDefinition = afw_object_get_next_property_as_object_internal(
        indexDefinitions, &index_iterator, &key, xctx);

    while (indexDefinition) {
        afw_adapter_impl_index_open_definition(indexer,
            afw_object_string_property_name_as_utf8(key, xctx), 
            indexDefinition, pool, xctx);

        indexDefinition = afw_object_get_next_property_as_object_internal(
            indexDefinitions, &index_iterator, &key, xctx);
    }
}

/*
 * Callback routine for maintaining an index.  This is invoked 
 * from the "create" and "delete" routines when we need to apply
 * an index retroactively against active records in the database.
 */
afw_boolean_t afw_adapter_impl_index_cb(
    const afw_object_t * object,
    void               * context,
    afw_xctx_t        * xctx)
{
    impl_retrieve_objects_cb_context_t * ctx =
        (impl_retrieve_objects_cb_context_t *) context;
    const afw_utf8_t                   * object_id;
    const afw_utf8_t                   * object_type_id;
    const afw_utf8_t                   * key;    

    if (object == NULL) {
        /* no more objects, so we are finished */
        return false;
    }

    /* increment our number of processed for metrics */
    ctx->num_processed++;

    /* fetch the objectId from the object, so we can use as the index value */
    object_id = afw_object_meta_get_object_id(object, xctx);
    if (!object_id)
        AFW_THROW_ERROR_Z(general,
            "Error: unable to determine object_id for object.", xctx);

    object_type_id = afw_object_meta_get_object_type_id(object, xctx);
    if (!object_type_id) {
        AFW_THROW_ERROR_Z(general,
            "Error: unable to determine object_type_id for object.", xctx);
    }

    /* current:: for filter/value is pushed inside afw_adapter_impl_index_try. */

    /* The index "key" is the name that will match the query */
    key = ctx->key;

    /* Try the index operation, which may fail and return false */
    if (afw_adapter_impl_index_try(ctx->instance, key, object, 
        object_type_id, object_id, ctx->indexDefinition, ctx->mode, xctx)) {
        ctx->num_indexed++;
    }

    /* release object to free memory */
    afw_object_release(object, xctx);

    /* Return indicating not to short circuit */
    return false;
}

/*
 * void afw_adapter_impl_index_list()
 *
 * This routine will use adapter interfaces to go through
 * and list indexes for a particular adapterId.
 *
 */
AFW_DEFINE(const afw_object_t *) afw_adapter_impl_index_list(
    const afw_utf8_t * adapterId,
    const afw_utf8_t * object_type_id,
    const afw_pool_t * pool,
    afw_xctx_t      * xctx)
{
    const afw_adapter_impl_index_t * instance;
    const afw_adapter_session_t    * session;
    const afw_iterator_old_t           * index_iterator;
    const afw_object_t             * indexDefinition;
    const afw_object_t             * result;

    session = afw_adapter_session_get_cached(adapterId, false, xctx);

    instance = afw_adapter_session_get_index_interface(session, xctx);
    if (instance == NULL) {
        AFW_THROW_ERROR_Z(general,
            "Error: Cannot find index interface for adapterId.", xctx);
    }

    result = instance->indexDefinitions;

    if (object_type_id) {
        result = afw_object_create_unmanaged_new_p(pool, xctx);

        index_iterator = NULL;
        const afw_value_t *key;

        indexDefinition = afw_object_get_next_property_as_object_internal(
            instance->indexDefinitions, &index_iterator, &key, xctx);
        while (indexDefinition) {
            if (afw_adapter_impl_index_object_type_applicable(
                indexDefinition, object_type_id, xctx)) {
                afw_object_set_property_as_object_internal(result,
                    key, indexDefinition, xctx);
            }

            indexDefinition = afw_object_get_next_property_as_object_internal(
                instance->indexDefinitions, &index_iterator, &key, xctx);
        }
    }

    return result;
}

AFW_DEFINE(const afw_object_t *) afw_adapter_impl_index_remove(
    const afw_utf8_t  * adapterId,
    const afw_utf8_t  * key,
    const afw_pool_t  * pool,
    afw_xctx_t       * xctx)
{
    const afw_adapter_session_t        * session;
    const afw_adapter_impl_index_t     * indexer;
    impl_retrieve_objects_cb_context_t   ctx;
    const afw_object_t                 * indexDefinition;
    const afw_array_t                   * objectTypes;
    const afw_object_t                 * result;
    const afw_iterator_old_t               * object_type_iterator;
    const afw_utf8_t                   * object_type_id;
    afw_rc_t                             rc;

    session = afw_adapter_session_get_cached(adapterId, true, xctx);

    indexer = afw_adapter_session_get_index_interface(session, xctx);
    if (indexer == NULL) {
        AFW_THROW_ERROR_Z(general, 
            "Error: unable to get index interface.", xctx);
    }

    /* create our result object to be returned */
    result = afw_object_create_unmanaged_new_p(pool, xctx);

    if (indexer->indexDefinitions == NULL) {
        AFW_THROW_ERROR_Z(general,
            "Error: there are no index definitions to remove.", xctx);
    }

    indexDefinition = afw_object_get_property_as_object_internal(
        indexer->indexDefinitions,
        afw_value_create_unmanaged_string(key, pool, xctx), xctx);
    if (indexDefinition == NULL) {
        AFW_THROW_ERROR_Z(general,
            "Error there is no index definition by this key.", xctx);
    }

    /* first, we remove the index from the configuration, 
        so it's no longer in use */
    afw_object_remove_property(indexer->indexDefinitions,
        afw_value_create_unmanaged_string(key, pool, xctx), xctx);

    afw_adapter_impl_index_update_index_definitions(
        indexer, indexer->indexDefinitions, xctx);

    /* get all applicable objectTypes */
    objectTypes = afw_object_get_property_as_array_internal(
        indexDefinition, afw_v_objectType, xctx);
    if (objectTypes) {
        object_type_iterator = NULL;

        object_type_id = afw_array_of_string_get_next(
            objectTypes, &object_type_iterator, xctx);
        do
        {
            /* try to drop it first, if possible */
            rc = afw_adapter_impl_index_drop(indexer,
                object_type_id, key, pool, xctx);
            if (rc) {
                ctx.instance = indexer;
                ctx.key = key;
                ctx.indexDefinition = indexDefinition;
                ctx.num_indexed = 0;
                ctx.num_processed = 0;
                ctx.mode = afw_adapter_impl_index_mode_delete;

                /** @fixme xctx->p, session->p, or pool?  */
                afw_adapter_session_retrieve_objects(session, NULL,
                    object_type_id,
                    NULL, &ctx, afw_adapter_impl_index_cb, NULL,
                    /** @fixme is pool correct? */ pool, xctx);
            }

            if (object_type_id)
                object_type_id = afw_array_of_string_get_next(
                    objectTypes, &object_type_iterator, xctx);

        } while (object_type_id);
    }

    return result;
}

/*
 * void afw_adapter_impl_index_create()
 *
 * This routine will create an index definition.
 *
 */
AFW_DEFINE(const afw_object_t *) afw_adapter_impl_index_create(
    const afw_utf8_t  * adapterId,
    const afw_utf8_t  * key,
    const afw_utf8_t  * value,
    const afw_array_t  * objectType,
    const afw_utf8_t  * filter,
    const afw_array_t  * options,
    afw_boolean_t       retroactive,
    afw_boolean_t       test,
    const afw_pool_t  * pool,
    afw_xctx_t       * xctx)
{
    const afw_adapter_session_t        * session;
    const afw_adapter_impl_index_t     * indexer;
    impl_retrieve_objects_cb_context_t   ctx;
    const afw_object_t                 * indexDefinitions;
    const afw_object_t                 * indexDefinition;
    const afw_object_t                 * result;
    const afw_adapter_transaction_t    * transaction;
    const afw_iterator_old_t           * object_type_iterator;
    const afw_utf8_t                   * object_type_id;

    session = afw_adapter_session_get_cached(adapterId, false, xctx);

    indexer = afw_adapter_session_get_index_interface(session, xctx);
    if (indexer == NULL) {
        AFW_THROW_ERROR_Z(general, 
            "Error: unable to get index interface.", xctx);
    }

    /* create our result object to be returned */
    result = afw_object_create_unmanaged_new_p(pool, xctx);

    indexDefinitions = indexer->indexDefinitions;

    /* if we don't have any index definitions, create a new object */
    if (indexDefinitions == NULL) {
        indexDefinitions = afw_object_create_unmanaged_new_p(pool, xctx);
    }

    indexDefinition = afw_object_get_property_as_object_internal(
        indexDefinitions,
        afw_value_create_unmanaged_string(key, pool, xctx), xctx);
    if (indexDefinition) {
        /** @fixme already indexed, rebuild? */
        afw_object_set_property_as_string_from_utf8_z(result, afw_v_message, 
            "An index definition by this key already exists.", xctx);
        return result;
    }

    /* create a new indexDefinition */
    indexDefinition = afw_object_create_unmanaged_new_p(pool, xctx);

    if (value)
        afw_object_set_property_as_string_internal(indexDefinition,
            afw_v_value, value, xctx);

    if (objectType)
        afw_object_set_property_as_array_internal(indexDefinition,
            afw_v_objectType, objectType, xctx);
   
    if (filter)
        afw_object_set_property_as_string_internal(indexDefinition,
            afw_v_filter, filter, xctx);
   
    if (options)
        afw_object_set_property_as_array_internal(indexDefinition,
            afw_v_options, options, xctx);

    ctx.instance = indexer;
    ctx.key = key;
    ctx.indexDefinition = indexDefinition;
    ctx.num_indexed = 0;
    ctx.num_processed = 0;
    ctx.mode = afw_adapter_impl_index_mode_add;
    ctx.test = test;

    /* open the index definition ahead of time */
    afw_adapter_impl_index_open_definition(indexer, key, 
        indexDefinition, pool, xctx);

    transaction = afw_adapter_session_begin_transaction(session, xctx);

    /* the callback routine does all of our work for us */
    if (retroactive) {
        /*
         * If this definition is scoped to specific objectTypes, scan only
         * those types instead of paying for a full-table walk (mirrors the
         * per-objectType loop afw_adapter_impl_index_remove() already uses).
         * An omitted or empty objectType list means all types apply, so
         * fall back to a single unscoped scan for that case.
         */
        object_type_iterator = NULL;
        object_type_id = (objectType)
            ? afw_array_of_string_get_next(
                objectType, &object_type_iterator, xctx)
            : NULL;

        if (object_type_id) {
            do {
                /** @fixme should this be session->p, pool, or xctx->p? */
                afw_adapter_session_retrieve_objects(session, NULL,
                    object_type_id,
                    NULL, &ctx, afw_adapter_impl_index_cb, NULL, pool, xctx);

                object_type_id = afw_array_of_string_get_next(
                    objectType, &object_type_iterator, xctx);
            } while (object_type_id);
        } else {
            /** @fixme should this be session->p, pool, or xctx->p? */
            afw_adapter_session_retrieve_objects(session, NULL, NULL,
                NULL, &ctx, afw_adapter_impl_index_cb, NULL, pool, xctx);
        }
    }

    /* now, tell the adapter to add the new indexDefinition for configuration */
    afw_object_set_property_as_object_internal(
        indexDefinitions,
        afw_value_create_unmanaged_string(key, pool, xctx),
        indexDefinition, xctx);

    afw_adapter_impl_index_update_index_definitions(
        indexer, indexDefinitions, xctx);

    afw_adapter_transaction_commit(transaction, xctx);
    afw_adapter_transaction_release(transaction, xctx);

    /* return metrics */
    afw_object_set_property_as_integer_internal(
        result, afw_v_num_indexed, ctx.num_indexed, xctx);
    afw_object_set_property_as_integer_internal(
        result, afw_v_num_processed, ctx.num_processed, xctx);

    return result;
}


/*
 * afw_boolean_t afw_adapter_impl_index_is_property_indexed()
 *
 * Looks through the list of internal indexes and returns true/false
 * if the property is indexed or not.
 *
 */
afw_boolean_t afw_adapter_impl_index_is_property_indexed(
    const afw_adapter_impl_index_t * instance,
    const afw_utf8_t               * object_type_id,
    const afw_utf8_t               * property_name,
    afw_xctx_t                    * xctx)
{
    const afw_object_t * indexDefinition;

    if (instance->indexDefinitions) {
        const afw_value_string_t property_name_value =
            AFW_VALUE_STRING_UNMANAGED(property_name);
        indexDefinition = afw_object_get_property_as_object_internal(
            instance->indexDefinitions, &property_name_value.pub, xctx);
        if (indexDefinition) {
            if (afw_adapter_impl_index_object_type_applicable(
                indexDefinition, object_type_id, xctx)) {
                return true;
            }
        }
    }

    return false;
}

/*
 * const afw_object_t * afw_adapter_impl_index_get_index_definition()
 *
 * Looks through the list of internal indexes and returns the
 * matching indexDefinition, if one is found.
 *
 */
const afw_object_t * afw_adapter_impl_index_get_index_definition(
    const afw_adapter_impl_index_t * instance,
    const afw_utf8_t               * object_type_id,
    const afw_utf8_t               * property_name,
    afw_xctx_t                    * xctx)
{
    const afw_object_t * indexDefinition = NULL;

    if (instance->indexDefinitions) {
        const afw_value_string_t property_name_value =
            AFW_VALUE_STRING_UNMANAGED(property_name);
        indexDefinition = afw_object_get_property_as_object_internal(
            instance->indexDefinitions, &property_name_value.pub, xctx);
        if (indexDefinition) {
            if (afw_adapter_impl_index_object_type_applicable(
                indexDefinition, object_type_id, xctx)) {
                return indexDefinition;
            } else {
                indexDefinition = NULL;
            }
        }
    }

    return indexDefinition;
}

/*
 * void afw_adapter_impl_index_object()
 *
 * When an object is created, this routine will iterate through
 * each index and apply them, if appropriate.
 *
 */
AFW_DEFINE(void) afw_adapter_impl_index_object(
    const afw_adapter_impl_index_t * instance,
    const afw_utf8_t               * object_type_id,
    const afw_object_t             * object,
    const afw_utf8_t               * object_id,
    afw_xctx_t                    * xctx)
{
    const afw_value_t  * index_name;
    const afw_object_t * indexDefinition;
    const afw_iterator_old_t * index_iterator;

    if (instance->indexDefinitions) {
        /* iterate through each indexDefinition to see if it applies */
        index_iterator = NULL;
        indexDefinition = afw_object_get_next_property_as_object_internal(
            instance->indexDefinitions, &index_iterator, &index_name, xctx);
        while (indexDefinition) {
            afw_adapter_impl_index_try(instance,
                afw_object_string_property_name_as_utf8(index_name, xctx),
                object, 
                object_type_id, object_id, indexDefinition, 
                afw_adapter_impl_index_mode_add, xctx);

            indexDefinition = afw_object_get_next_property_as_object_internal(
                instance->indexDefinitions, &index_iterator, &index_name, xctx);

        }
    }
}

/*
 * void afw_adapter_impl_index_unindex_object()
 *
 * When an object is removed from the data store,
 * this routine will remove any associated indexes.
 *
 */
AFW_DEFINE(void) afw_adapter_impl_index_unindex_object(
    const afw_adapter_impl_index_t * instance,
    const afw_utf8_t               * object_type_id,
    const afw_object_t             * object,
    const afw_utf8_t               * object_id,
    afw_xctx_t                    * xctx)
{
    const afw_value_t  * index_name;
    const afw_object_t * indexDefinition;
    const afw_iterator_old_t * index_iterator;

    if (instance->indexDefinitions) {
        /* iterate through each indexDefinition to see if it applies */
        index_iterator = NULL;
        indexDefinition = afw_object_get_next_property_as_object_internal(
            instance->indexDefinitions, &index_iterator, &index_name, xctx);
        while (indexDefinition) {
            afw_adapter_impl_index_try(instance,
                afw_object_string_property_name_as_utf8(index_name, xctx),
                object, 
                object_type_id, object_id, indexDefinition,
                afw_adapter_impl_index_mode_delete, xctx);

            indexDefinition = afw_object_get_next_property_as_object_internal(
                instance->indexDefinitions, &index_iterator, &index_name, xctx);

        }
    }
}


/*
 * void afw_adapter_impl_index_reindex_object()
 *
 * When an object is modified in the data store, this
 * routine removes any old indexes that existed, and
 * replaces them with new indexes.
 *
 */
AFW_DEFINE(void) afw_adapter_impl_index_reindex_object(
    const afw_adapter_impl_index_t * instance,
    const afw_utf8_t               * object_type_id,
    const afw_object_t             * old_object,
    const afw_object_t             * new_object,
    const afw_utf8_t               * object_id,
    afw_xctx_t                    * xctx)
{
    const afw_value_t  * index_name;
    const afw_iterator_old_t * index_iterator;
    const afw_object_t * indexDefinition;

    if (instance->indexDefinitions) {
        index_iterator = NULL;
        indexDefinition = afw_object_get_next_property_as_object_internal(
            instance->indexDefinitions, &index_iterator, &index_name, xctx);
        while (indexDefinition) {
            /* remove indexes from the old object */
            afw_adapter_impl_index_try(instance,
                afw_object_string_property_name_as_utf8(index_name, xctx),
                old_object,
                object_type_id, object_id, indexDefinition,
                afw_adapter_impl_index_mode_delete, xctx);

            /* now add back the new ones */
            afw_adapter_impl_index_try(instance,
                afw_object_string_property_name_as_utf8(index_name, xctx),
                new_object,
                object_type_id, object_id, indexDefinition, 
                afw_adapter_impl_index_mode_add, xctx);

            indexDefinition = afw_object_get_next_property_as_object_internal(
                instance->indexDefinitions, &index_iterator, &index_name, xctx);
        }
    }
}

/* useful macro for determining if node, x, is a non-leaf node */
#define AFW_QUERY_CRITERIA_CONTINUE(x) \
    (x != AFW_QUERY_CRITERIA_FALSE && x != AFW_QUERY_CRITERIA_TRUE)

/*
 * afw_boolean_t afw_adapter_impl_index_sargable_entry()
 *
 * This recursive function takes a filter entry and evaluates
 * the full decision tree to determine if it's sargable.  The
 * sargability looks opposite to the semantic of the expression
 * being evaluated on purpose.
 *
 * For example, when evaluating the sargability of an exclusive
 *   relationship (AND), we only need one clause to be sargable
 *   to gain a performance increase, using an index.
 *
 * However, when evaluating an inclusive relationship (OR), we need
 *   every clause to be sargable in order for indexes to be useful.
 *
 * FIXME:  This routine will only report that a clause is sargable 
 *   if the operation is eq, lt, lte, gt, or gte.
 *
 */
AFW_DEFINE(afw_boolean_t) afw_adapter_impl_index_sargable_entry(
    const afw_adapter_impl_index_t          * instance,
    const afw_utf8_t                        * object_type_id,
    const afw_query_criteria_filter_entry_t * entry,
    afw_xctx_t                             * xctx)
{
    afw_boolean_t sargable;
    afw_boolean_t on_true, on_false;

    /* For now, we will only evaluate certain operations for sargability */
    if (! (
            entry->op_id == afw_query_criteria_filter_op_id_eq  ||
            entry->op_id == afw_query_criteria_filter_op_id_lt  ||
            entry->op_id == afw_query_criteria_filter_op_id_le ||
            entry->op_id == afw_query_criteria_filter_op_id_gt  ||
            entry->op_id == afw_query_criteria_filter_op_id_ge
        )
        )
        return false;

    /* Determine if this property is indexed */
    sargable = afw_adapter_impl_index_is_property_indexed(instance,
        object_type_id,
        entry->property_name, xctx);
   
    /* A bottom leaf of our query decision tree */ 
    if (entry->on_true == AFW_QUERY_CRITERIA_TRUE &&
        entry->on_false == AFW_QUERY_CRITERIA_FALSE) {
        return sargable;
    }

    /* (entry AND on_true) OR (on_false) */
    else if (AFW_QUERY_CRITERIA_CONTINUE(entry->on_true) &&
            AFW_QUERY_CRITERIA_CONTINUE(entry->on_false)) {
        on_true = afw_adapter_impl_index_sargable_entry(
            instance, object_type_id, entry->on_true, xctx);

        on_false = afw_adapter_impl_index_sargable_entry(
            instance, object_type_id, entry->on_false, xctx);

        return ((sargable || on_true) && (on_false));
    }

    /* (entry AND on_true) */
    else if (AFW_QUERY_CRITERIA_CONTINUE(entry->on_true)) {
        on_true = afw_adapter_impl_index_sargable_entry(
            instance, object_type_id, entry->on_true, xctx);

        return (sargable || on_true);
    }

    /* (entry OR on_false) */
    else if (AFW_QUERY_CRITERIA_CONTINUE(entry->on_false)) {
        on_false = afw_adapter_impl_index_sargable_entry(
            instance, object_type_id, entry->on_false, xctx);

        return (sargable && on_false);
    }

    /* bug?? */
    else {
        AFW_THROW_ERROR_Z(general, 
            "Error: unexpected condition while parsing filter expression.", xctx);
    }

    return false;
}

/*
 * afw_boolean_t afw_adapter_impl_index_sargable()
 *
 * "Sargable", or Search ARGument ABLE
 *
 * Returns true if the query_criteria contains properties
 * that can leverage index(es) to locate.
 *
 */
AFW_DEFINE(afw_boolean_t) afw_adapter_impl_index_sargable(
    const afw_adapter_impl_index_t * instance,
    const afw_utf8_t               * object_type_id,
    const afw_query_criteria_t     * criteria,
    afw_xctx_t                    * xctx)
{
    const afw_query_criteria_filter_entry_t * entry;

    /* if we have no criteria, then there's no index to help */
    entry = (criteria) ? criteria->filter : NULL;
    if (entry == NULL)
        return false;

    return afw_adapter_impl_index_sargable_entry(
        instance, object_type_id, entry, xctx);
}

/*
 * afw_adapter_impl_index_cursor_list_cardinality()
 *
 * Takes a list of cursor(s) and computes the cardinality
 * for the entire set.  For individual cursors, this is
 * exact.  For a conjunction of cursors, this computes the
 * worst-case scenario (a total sum).
 */
afw_boolean_t afw_adapter_impl_index_cursor_list_cardinality(
    const afw_adapter_impl_index_t * instance,
    apr_array_header_t             * cursor_list,
    size_t                         * cardinality,
    afw_xctx_t                    * xctx)
{
    const afw_adapter_impl_index_cursor_t *cursor;
    afw_boolean_t rc;
    size_t c;
    int i;

    /* no cursors in the list indicates they were not sargable */
    if (apr_is_empty_array(cursor_list))
        return false;
  
    *cardinality = 0;
    for (i = 0; i < cursor_list->nelts; i++) {
        cursor = ((const afw_adapter_impl_index_cursor_t **)
            cursor_list->elts)[i];

        rc = afw_adapter_impl_index_cursor_get_count(
            cursor, &c, xctx);
        if (!rc) return rc;

        /* Each conjunction may, at worst-case, add to
            the total overall cardinality */
        *cardinality += c;
    } 

    return true;
}

/*
 * afw_adapter_impl_index_cursor_list_join()
 *
 * During a disjunction decision, we would ideally like to
 * choose the cursor_list with lower cardinality.  This 
 * routine computes those and returns the better choice.
 *
 * If cardinality cannot be computed, we simply choose one.
 */
apr_array_header_t * afw_adapter_impl_index_cursor_list_join(
    const afw_adapter_impl_index_t * instance,
    apr_array_header_t             * this_list,
    apr_array_header_t             * that_list,
    afw_xctx_t                    * xctx)
{
    size_t this_cardinality, that_cardinality;
    afw_adapter_impl_index_cursor_t *cursor;
    afw_boolean_t rc;
    int i;

    /* unless we can do a real inner-join, we must indicate that
        the "joined" result list is not the result of an inner-join, but 
        a left or right outer join */
    for (i = 0; i < this_list->nelts; i++) {
        cursor = ((afw_adapter_impl_index_cursor_t**)this_list->elts)[i];
        cursor->inner_join = false;
    }

    for (i = 0; i < that_list->nelts; i++) {
        cursor = ((afw_adapter_impl_index_cursor_t**)that_list->elts)[i];
        cursor->inner_join = false;
    }
        
    rc = afw_adapter_impl_index_cursor_list_cardinality(
        instance, this_list, &this_cardinality, xctx);
    if (!rc) {
        /* Unable to compute the cardinality, so we must simply choose one */
        return that_list;
    }

    rc = afw_adapter_impl_index_cursor_list_cardinality(
        instance, that_list, &that_cardinality, xctx);
    if (!rc) {
        /* Unable to compute the cardinality, so we must simply choose one */
        return this_list;
    }

    return (this_cardinality <= that_cardinality) ? this_list : that_list;
}

/*
 * afw_adapter_impl_index_cursor_list_merge()
 *
 * During a conjunction, we merge two lists together.
 * We like to have them sorted from highest 
 * cardinality to lowest to make the process of 
 * removing duplicates easier.
 *
 */
apr_array_header_t * afw_adapter_impl_index_cursor_list_merge(
    const afw_adapter_impl_index_t * instance,
    apr_array_header_t             * this_list,
    apr_array_header_t             * that_list,
    afw_xctx_t                    * xctx)
{
    apr_array_header_t *merged_list;
    apr_array_header_t *temp;
    const afw_adapter_impl_index_cursor_t *this_cursor;
    const afw_adapter_impl_index_cursor_t *that_cursor;
    size_t this_cardinality, that_cardinality;
    afw_boolean_t rc;
    int merged_size;
    int i, j;

    merged_size = this_list->nelts + that_list->nelts;
    merged_list = NULL;
    temp = that_list;

    /* walk through each item in this_list and merge it into a new one */
    for (i = 0; i < this_list->nelts; i++) {
        merged_list = apr_array_make(afw_pool_get_apr_pool(xctx->p),
            merged_size, sizeof(const afw_adapter_impl_index_cursor_t *));

        this_cursor = ((const afw_adapter_impl_index_cursor_t **)
            this_list->elts)[i];

        rc = afw_adapter_impl_index_cursor_get_count(
            this_cursor, &this_cardinality, xctx);
        if (!rc) return NULL;

        for (j = 0; j < temp->nelts; j++) {
            that_cursor = ((const afw_adapter_impl_index_cursor_t **)
                temp->elts)[j];

            rc = afw_adapter_impl_index_cursor_get_count(
                that_cursor, &that_cardinality, xctx);
            if (!rc) {
                AFW_THROW_ERROR_FZ(general, xctx,
                    "Error occurred obtaining cursor count, rc = %d", rc); 
            }

            if (this_cardinality > that_cardinality) {
                *(const afw_adapter_impl_index_cursor_t**)
                    apr_array_push(merged_list) = this_cursor;
                /* now allow the rest of that_list to be merged */
                this_cardinality = 0;
            }

            *(const afw_adapter_impl_index_cursor_t**)
                apr_array_push(merged_list) = that_cursor;
        }

        temp = apr_array_copy(afw_pool_get_apr_pool(xctx->p), merged_list);
    }

    return merged_list;
}

/*
 * void afw_adapter_impl_index_cursor_list()
 *
 * Recursive routine for building out an array of cursors,
 * The end result is the indexed result set, in conjunctive 
 * form.  To evaluate it, the union of the cursor lists 
 * must be evaluated.
 *
 * The query_criteria must be sargable before calling this
 * routine.
 *
 * Note:  The cursor arrays may contain "outer joins", which 
 *      need to be evaluated to exclude false results.  This
 *      is because we do not do actual conjunctive joins 
 *      at the time we make this decision.  Moreover, an
 *      individual clause may not have indexes we can hit. 
 *
 *      In order to do inner joins, we may rely on the adapter to
 *      efficiently perform this step.  For now, these joins
 *      are effectively performed by calculating cardinality.
 *
 *      This will return an array of cursors, representing the
 *      resulting conjunction.
 *
 */
apr_array_header_t * afw_adapter_impl_index_cursor_list(
    const afw_adapter_impl_index_t          * instance,
    const afw_utf8_t                        * object_type_id,
    const afw_query_criteria_filter_entry_t * entry,
    afw_xctx_t                             * xctx)
{  
    afw_adapter_impl_index_cursor_t *cursor = NULL;
    apr_array_header_t *cursor_list, *next_list;
    const afw_object_t *indexDefinition;
    const afw_utf8_t *value_string;
    afw_boolean_t unique;

    /* allocate our cursor_list that will contain a conjunction
        of cursors, representing this particular decision branch. */
    cursor_list = apr_array_make(afw_pool_get_apr_pool(xctx->p), 8, 
        sizeof(const afw_adapter_impl_index_cursor_t*));

    if (entry == NULL) {
        /* No entry means empty cursor list */
        return cursor_list;
    }

    /* use the internal utf-8 string representation; integer/double
       get the sortable encoding so range ops walk keys in numeric
       order (issue #251) */
    value_string = impl_index_value_as_key_utf8(entry->value, xctx->p, xctx);

    /* Determine if this property is indexed */
    indexDefinition = afw_adapter_impl_index_get_index_definition(
        instance, object_type_id,
        entry->property_name, xctx);
    if (indexDefinition)
    {
        /* if the indexDefinition is case-insensitive, then we need
            to lowercase our query value for comparison */
        if (afw_adapter_impl_index_option_case_insensitive(
            indexDefinition, xctx)) {
            value_string = afw_utf8_to_lower(value_string, xctx->p, xctx);
        }
        unique = afw_adapter_impl_index_option_unique(
            indexDefinition, xctx);

        /* get this cursor and add it to our current cursor list */
        /** @fixme we need to register open cursors to be released, because we
            may not know exactly when to discard them */
        cursor = afw_adapter_impl_index_open_cursor(instance, object_type_id, 
            entry->property_name,
            entry->op_id, value_string, unique, xctx->p, xctx);
    } 

    if (cursor) {
        cursor->inner_join = true;
         
        /* remember the afw_query_criteria_filter_entry for later */ 
        cursor->filter_entry = entry;

        *(const afw_adapter_impl_index_cursor_t**)
            apr_array_push(cursor_list) = cursor;
    }

    /* A bottom leaf of our query decision tree */
    if (entry->on_true == AFW_QUERY_CRITERIA_TRUE &&
        entry->on_false == AFW_QUERY_CRITERIA_FALSE) 
    {
        /* Here there is nothing to do.  We simply return the 
            current cursor for this entry */
    }

    /* (entry AND on_true) OR (on_false) */
    else if (AFW_QUERY_CRITERIA_CONTINUE(entry->on_true) &&
            AFW_QUERY_CRITERIA_CONTINUE(entry->on_false)) 
    {
        /* 
            The disjunction of the a cursor on the current criteria
            entry and the on_true entry results in a subset of either
            one.

            This results in a superset, which will shrink later on when
            we perform evaluations.  If the cursor implementation supports
            complete inner joins, then we won't need to evaluate objects
            later on.
         */

        next_list = afw_adapter_impl_index_cursor_list(
            instance, object_type_id, entry->on_true, xctx);
        
        /* compute the optimal disjunction choice */
        cursor_list = afw_adapter_impl_index_cursor_list_join(
            instance, cursor_list, next_list, xctx);

        /*
        However, the conjunction clause with on_false requires that
        we evaluate it's cursor(s) and add it to our list as well.
         */
        next_list = afw_adapter_impl_index_cursor_list(
            instance, object_type_id, entry->on_false, xctx);
    
        cursor_list = afw_adapter_impl_index_cursor_list_merge(instance,
            cursor_list, next_list, xctx);
    }

    /* (entry AND on_true) */
    else if (AFW_QUERY_CRITERIA_CONTINUE(entry->on_true)) 
    {
        /* disjunction of cursors, again, choose the best one */
        next_list = afw_adapter_impl_index_cursor_list(instance,
            object_type_id, entry->on_true, xctx);

        cursor_list = afw_adapter_impl_index_cursor_list_join(
            instance, cursor_list, next_list, xctx);
    }

    /* (entry OR on_false) */
    else if (AFW_QUERY_CRITERIA_CONTINUE(entry->on_false)) 
    {
        /* conjunction of cursors, so we must use both */
        next_list = afw_adapter_impl_index_cursor_list(instance,
            object_type_id, entry->on_false, xctx);

        cursor_list = afw_adapter_impl_index_cursor_list_merge(instance,
            cursor_list, next_list, xctx);
    }

    /* bug?? */
    else {
        AFW_THROW_ERROR_Z(general, 
            "Error: unexpected condition while parsing filter expression.", xctx);
    }

    return cursor_list;
}

/*
 * int afw_adapter_impl_index_compare()
 *
 * This routine compares a value to the filter criteria entry
 * and returns the comparison result as:
 *
 * <0 less than
 * =  equivalent
 * >= grater than
 *
 */
static int afw_adapter_impl_index_compare(
    const afw_adapter_impl_index_t          * instance,
    const afw_query_criteria_filter_entry_t * entry,
    const afw_value_t                       * value,
    afw_xctx_t                             * xctx)
{
    const afw_utf8_t *string;
    const afw_value_t * const *values, *v;
    int i;
    const afw_utf8_t *property_value;

    /* use the internal utf-8 string representation; integer/double
       get the sortable encoding so lt/le/gt/ge below compare
       numerically, not lexicographically (issue #251) */
    property_value = impl_index_value_as_key_utf8(entry->value, xctx->p, xctx);

    /* can't compare Objects */
    if (afw_value_is_object(value)) {
        AFW_THROW_ERROR_Z(general,
            "Error: property value cannot be of type object.", xctx);
    }

    /* all we can do with Lists is check for equivalence */
    else if (afw_value_is_array(value)) {
        values = afw_value_to_null_terminated_values(value, xctx->p, xctx);
        for (i = 0; values[i]; i++) {
            v = values[i];
            string = impl_index_value_as_key_utf8(v, xctx->p, xctx);

            if (afw_utf8_equal(string, property_value))
                return 0;
        }
    }

    else {
        /* scalars are easy */
        string = impl_index_value_as_key_utf8(value, xctx->p, xctx);
        return afw_utf8_compare(string, property_value);
    }

    /* not sure what else to do here, but say they definitely aren't equal */
    return -1;
}

/*
 * afw_boolean_t afw_adapter_impl_index_applies()
 *
 * This routine determines whether a known object applies to
 * a specific cursor.  It's used to eliminate duplicates during
 * a cursor conjunction operation.
 *
 * We could ask the cursor if the object applies, but that is often
 * more expensive than testing the criteria ourselves, since there
 * are typically more objects in a cursor than there are properties
 * in an object.
 *
 */
static afw_boolean_t afw_adapter_impl_index_applies(
    const afw_adapter_impl_index_t        * instance,
    const afw_adapter_impl_index_cursor_t * cursor,
    const afw_object_t                    * object,
    afw_xctx_t                           * xctx)
{
    afw_boolean_t contains = false;
    const afw_query_criteria_filter_entry_t *entry = cursor->filter_entry;
    const afw_value_t *value;

    value = afw_object_get_property(object,
        afw_value_create_unmanaged_string(
            entry->property_name, xctx->p, xctx), xctx);
    if (value) {
        switch (entry->op_id) {
            case afw_query_criteria_filter_op_id_eq:
                if (afw_adapter_impl_index_compare(instance, 
                        entry, value, xctx) == 0)
                    return true;
                break;
            case afw_query_criteria_filter_op_id_ne:
                if (afw_adapter_impl_index_compare(instance, 
                        entry, value, xctx) != 0)
                    return true;
                break;
            case afw_query_criteria_filter_op_id_lt:
                if (afw_adapter_impl_index_compare(instance, 
                        entry, value, xctx) < 0)
                    return true;
                break;
            case afw_query_criteria_filter_op_id_le:
                if (afw_adapter_impl_index_compare(instance, 
                        entry, value, xctx) <= 0)
                    return true;
                break;
            case afw_query_criteria_filter_op_id_gt:
                if (afw_adapter_impl_index_compare(instance, 
                        entry, value, xctx) > 0)
                    return true;
                break;
            case afw_query_criteria_filter_op_id_ge:
                if (afw_adapter_impl_index_compare(instance, 
                        entry, value, xctx) >= 0)
                    return true;
                break;

            case afw_query_criteria_filter_op_id_na:
                break;

            case afw_query_criteria_filter_op_id_match:
            case afw_query_criteria_filter_op_id_contains:
            case afw_query_criteria_filter_op_id_in:
            case afw_query_criteria_filter_op_id_differ:
            case afw_query_criteria_filter_op_id_excludes:
            case afw_query_criteria_filter_op_id_out:
            case afw_query_criteria_filter_op_id_and:
            case afw_query_criteria_filter_op_id_or:
                AFW_THROW_ERROR_Z(query_too_complex,
                    "Filter op not implemented", xctx);

            default:
                AFW_THROW_ERROR_Z(general, "Filter op invalid", xctx);
        }
    } else {
        /* only "ne" operator is meaningful if the value doesn't exist */
        if (entry->op_id == afw_query_criteria_filter_op_id_ne)
            return true;
    }

    return contains;
}

/*
 * afw_adapter_impl_index_query()
 *
 * This routine will use the indexes to filter and create
 * a list, which the caller can iterate over.
 *
 * A query criteria filter converts our set of cursors into 
 * conjunctive normal form (CNF).  Each cursor in the set
 * is the result of a disjunctive join (AND) clauses.  The
 * final result must compute the union, eliminating duplicate
 * entries that may be contained within each cursor.
 */
AFW_DEFINE(void) afw_adapter_impl_index_query(
    const afw_adapter_impl_index_t * instance,
    const afw_utf8_t               * object_type_id,
    const afw_query_criteria_t     * criteria,
    afw_object_cb_t    callback,
    void                           * context, 
    const afw_pool_t               * pool,
    afw_xctx_t                    * xctx)
{
    apr_array_header_t *cursors;
    const afw_adapter_impl_index_cursor_t *current_cursor;
    const afw_adapter_impl_index_cursor_t *next_cursor;
    const afw_object_t *object = NULL;
    const afw_pool_t *p;
    int cursor_index = 0;
    int i;

    /* Recursively compute our cursors */
    cursors = afw_adapter_impl_index_cursor_list(instance,
        object_type_id, (criteria ? criteria->filter : NULL), xctx);

    if (apr_is_empty_array(cursors)) {
        AFW_THROW_ERROR_Z(general,
            "Error: unable to parse filter into indexable cursors.", xctx);
    }

    current_cursor = 
        ((const afw_adapter_impl_index_cursor_t**)cursors->elts)[0];

    while (1)
    {
        p = afw_pool_create(pool, xctx);

        /* get the next value from this cursor */
        object = afw_adapter_impl_index_cursor_get_next_object(
            current_cursor, p, xctx);

        if (object == NULL) {
            /* No more objects left on this index cursor */
            afw_adapter_impl_index_cursor_release(current_cursor, xctx);

            cursor_index++;
            if (cursors->nelts == cursor_index) {
                /* we're at the end of our cursors */
                callback(NULL, context, xctx);

                afw_pool_release(p, xctx);
                return;
            }

            current_cursor =
                ((const afw_adapter_impl_index_cursor_t**)
                cursors->elts)[cursor_index];

            afw_pool_release(p, xctx);
            continue;
        }

        /*
         * The reason we may need to test the object is because some of
         *  of the predicates may have specified non-indexed properties
         *  that we couldn't exclude automatically through joins.
         */
        if (!current_cursor->inner_join &&
            !afw_query_criteria_test_object(object, criteria, p, xctx))
        {
            /* this object can be discarded/ignored, so release its memory */
            afw_pool_release(p, xctx);
            continue;
        }

        /*
         * We may have duplicates that we need to eliminate.  We do this
         *  by simply looking ahead at the following cursors.  This
         *  does require some additional CPU, but cuts down on memory.
         */
        afw_boolean_t duplicate = false;
        for (i = cursor_index+1; i < cursors->nelts; i++) {
            next_cursor = ((const afw_adapter_impl_index_cursor_t**)
                cursors->elts)[i];
            /* 
                The "applies" routine will check the filter entry for this
                cursor, along with the object's matching property value, 
                and determine whether this cursor contains the object. 
             */
            if (afw_adapter_impl_index_applies(instance,
                next_cursor, object, xctx)) {
                /* we have a duplicate, which we skip for now and let the
                    future cursor provide instead. */
                duplicate = true;
                break;
            }
        }

        if (duplicate) {
            /* release this object to save memory, and move onto the next */
            afw_pool_release(p, xctx);
            continue;
        }

        /* finally, we have an object that satisfies all conditions, and is unique */
        callback(object, context, xctx);
    }

    /* should never get here */
    callback(NULL, context, xctx);
}
