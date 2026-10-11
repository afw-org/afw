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
#include <libxml/xmlregexp.h>

AFW_VECTOR_STRUCT(impl_index_cursor_p_vector_s,
    const afw_adapter_impl_index_cursor_t *);
typedef struct impl_index_cursor_p_vector_s
    impl_index_cursor_p_vector_t;


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
 *      "value"      : "return current::object.givenName + ' ' + current::object.surname;"
 *      "objectType" : ['Person']
 *      "filter"     : "return current::object.department == 'ENG';"
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
 *      With a value script, key is a computed name: a query on it means
 *      what the script gives, in every plan (issue #516), so it must be a
 *      name objects don't have.
 *
 *  The "objectType" property optionally determines a list of applicable objectType(s)
 *      this index definition. An empty array implies any/all objectType's.
 *
 *  The "filter" property optionally determines if a given object is applicable for
 *      this index definition (script must return boolean). It needs a value
 *      script: an object the filter leaves out has no value under key.
 *
 *  The "options" property is a list of options for how the index needs to
 *      be used.  Implemented options:
 *
 *          unique                  - reject/skip duplicate index values
 *          sort-reverse            - store index values in reverse order
 *          case-insensitive-string - fold string values for comparison
 *          integer / double        - the index's values are numbers: a
 *                                    string query value is the number's
 *                                    key when no object type says so
 *                                    (#544; one of the two at most)
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
    nextObjectType = afw_array_of_string_get_next_internal(
        objectTypes, &object_type_iterator, xctx);
    if (nextObjectType == NULL)
        return true;

    /* loop until we find a matching objectType */
    while (nextObjectType) {
        if (afw_utf8_equal(nextObjectType, object_type_id))
            return true;
 
        nextObjectType = afw_array_of_string_get_next_internal(
            objectTypes, &object_type_iterator, xctx);
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
        option = afw_array_get_next_value(options, &option_iterator, xctx);
        while (option) {
            if (afw_value_is_string(option)) {
                const afw_utf8_t *option_str =
                    afw_value_convert_to_utf8(option, xctx->p, xctx);

                if (afw_utf8_equal(option_str, afw_s_case_insensitive_string))
                    return true;
            }

            option = afw_array_get_next_value(
                options, &option_iterator, xctx);
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
        option = afw_array_get_next_value(options, &option_iterator, xctx);
        while (option) {
            if (afw_value_is_string(option)) {
                const afw_utf8_t *option_str = afw_value_convert_to_utf8(option,
                    xctx->p, xctx);

                if (afw_utf8_equal(option_str, afw_s_unique))
                    return true;
            }

            option = afw_array_get_next_value(
                options, &option_iterator, xctx);
        }
    }

    /* default to false, as case-sensitive is the default */
    return false;
}

/*
 * The data type the "integer" or "double" option gives an index, or
 * NULL (#544). A query string gives every value as a string; with no
 * object type to say a property is a number, this option makes "9" the
 * key of the number 9. Both options at once is an error.
 */
static const afw_data_type_t *
impl_index_option_data_type(
    const afw_object_t * indexDefinition,
    afw_xctx_t        * xctx)
{
    const afw_array_t  * options;
    const afw_value_t * option;
    const afw_iterator_old_t * option_iterator;
    const afw_utf8_t * option_str;
    const afw_data_type_t * data_type;

    data_type = NULL;
    options = afw_object_get_property_as_array_internal(
        indexDefinition, afw_v_options, xctx);
    for (option_iterator = NULL; options; ) {
        option = afw_array_get_next_value(options, &option_iterator, xctx);
        if (!option) {
            break;
        }
        if (!afw_value_is_string(option)) {
            continue;
        }
        option_str = (const afw_utf8_t *)AFW_VALUE_INTERNAL(option);
        if (afw_utf8_equal(option_str, afw_s_integer) ||
            afw_utf8_equal(option_str, afw_s_double))
        {
            if (data_type) {
                AFW_THROW_ERROR_Z(general,
                    "An index can have only one of the options integer "
                    "and double", xctx);
            }
            data_type = (afw_utf8_equal(option_str, afw_s_integer))
                ? afw_data_type_integer
                : afw_data_type_double;
        }
    }

    return data_type;
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
 * One "\0" byte: the empty string's index key, and the escape in front
 * of a text that starts with "\0".
 */
static const afw_utf8_t impl_index_key_nul = { "", 1 };

/*
 * The index key of a text. Index keys can't be empty (LMDB), and the
 * empty string was skipped, so an index query never found "" (eq "",
 * lt/le walking down, ge "") (#544). A key is the text, except that ""
 * is "\0" and a text that starts with "\0" gets another "\0" in front.
 * Every key stays distinct and in the texts' byte order ("" first), so
 * cursors and the dedup compare keys as before.
 */
static const afw_utf8_t *
impl_index_key_from_text(
    const afw_utf8_t *text, const afw_pool_t *p, afw_xctx_t *xctx)
{
    if (text->len > 0 && text->s[0] != '\0') {
        return text;
    }

    return afw_utf8_concat(p, xctx, &impl_index_key_nul, text, NULL);
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
            afw_value_as_integer_internal(value, p, xctx), p, xctx);
    }

    if (afw_value_is_double(value)) {
        return impl_index_double_as_sortable_utf8(
            afw_value_as_double_internal(value, p, xctx), p, xctx);
    }

    return impl_index_key_from_text(
        afw_value_convert_to_utf8(value, p, xctx), p, xctx);
}

/*
 * The index key of an object's value, lowercased for a case-insensitive
 * index. Writing an index and the dedup (afw_adapter_impl_index_applies)
 * make keys here. A value keeps its own data type's key: an index whose
 * values mix integers and doubles orders them apart, as a scan compares
 * them apart (it converts the filter value to each object's type).
 */
static const afw_utf8_t *
impl_index_object_value_key(
    const afw_object_t *indexDefinition,
    const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t *key;

    key = impl_index_value_as_key_utf8(value, p, xctx);
    if (afw_adapter_impl_index_option_case_insensitive(
        indexDefinition, xctx))
    {
        key = afw_utf8_to_lower(key, p, xctx);
    }

    return key;
}

/*
 * The key of a filter entry's value. A query string's values are all
 * strings: on an index whose integer / double option, or else whose
 * property's object type, says integer or double, a string value is
 * that number's key (n=gt=9 sought the string "9" among the integers'
 * sortable keys: it found nothing, and n=lt=10 found everything), and
 * on a double index an integer value is that double's key, as a scan
 * converts the filter value to each object's type (#544).
 *
 * A string that doesn't convert ("x", or "9" for a double: double("9")
 * is not a double) stays a string. A scan finds no number for it, but
 * its key's range can hold number keys, so *exact is set false: the
 * index query re-tests what that cursor returns. Otherwise true.
 */
static const afw_utf8_t *
impl_index_entry_value_as_key_utf8(
    const afw_object_t *indexDefinition,
    const afw_query_criteria_filter_entry_t *entry,
    afw_boolean_t *exact,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_value_t *value;
    const afw_data_type_t *data_type;

    *exact = true;
    value = entry->value;
    data_type = impl_index_option_data_type(indexDefinition, xctx);
    if (!data_type && entry->pt) {
        data_type = entry->pt->data_type;
    }
    if (!afw_data_type_is_integer(data_type) &&
        !afw_data_type_is_double(data_type))
    {
        data_type = NULL;
    }

    if (data_type && (afw_value_is_string(value) ||
        (afw_data_type_is_double(data_type) && afw_value_is_integer(value))))
    {
        AFW_TRY {
            value = afw_value_convert(value, data_type, false, p, xctx);
        }
        AFW_CATCH_UNHANDLED {
            value = entry->value;
            *exact = false;
        }
        AFW_ENDTRY;
    }

    return impl_index_value_as_key_utf8(value, p, xctx);
}

/*
 * Get the literal starts-with prefix of a match entry, if any.
 *
 * Patterns compiled for the match operator use XML Schema Datatype regex
 * (via libxml2), which is implicitly anchored to the whole string - there
 * is no '^' or '$' anchor syntax. A "starts with" test is therefore
 * written as `<literal>.*`, where `<literal>` contains no regex
 * metacharacters. Recognizes exactly that shape; anything else - a bare
 * ".*", a metacharacter in the literal part, or ".*" appearing anywhere
 * but at the very end - returns NULL and is left to the general
 * (non-sargable) full-scan regex evaluation in afw_query_criteria.c.
 *
 * This lets a match entry be sargable for adapter indexes without
 * implementing a general regex-to-index-range translation; it is scoped
 * to this adapter-index module rather than afw_query_criteria.c because
 * it is an index/b-tree concern, not part of RQL semantics itself.
 */
static const afw_utf8_t *
impl_index_match_literal_prefix(
    const afw_query_criteria_filter_entry_t *entry,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *pattern;
    afw_utf8_t *prefix;
    afw_size_t prefix_len;
    afw_size_t i;

    if (entry->op_id != afw_query_criteria_filter_op_id_match ||
        !entry->value || !afw_value_is_string(entry->value))
    {
        return NULL;
    }

    pattern = (const afw_utf8_t *)AFW_VALUE_INTERNAL(entry->value);

    if (pattern->len < 3 ||
        pattern->s[pattern->len - 2] != '.' ||
        pattern->s[pattern->len - 1] != '*')
    {
        return NULL;
    }

    prefix_len = pattern->len - 2;

    for (i = 0; i < prefix_len; i++) {
        switch (pattern->s[i]) {
            case '.': case '*': case '+': case '?':
            case '(': case ')': case '[': case ']':
            case '{': case '}': case '|':
            case '^': case '$': case '\\':
                return NULL;
            default:
                break;
        }
    }

    prefix = afw_pool_calloc_type(p, afw_utf8_t, xctx);
    prefix->s = pattern->s;
    prefix->len = prefix_len;

    return prefix;
}

/*
 * The key a cursor on entry seeks: the literal "starts with" prefix of a
 * match, or the entry value's key, lowercased for a case-insensitive
 * index. The dedup check (afw_adapter_impl_index_applies) compares
 * object keys with this same key.
 */
static const afw_utf8_t *
impl_index_entry_seek_key(
    const afw_object_t *indexDefinition,
    const afw_query_criteria_filter_entry_t *entry,
    const afw_utf8_t *literal_prefix,
    afw_boolean_t *exact,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *key;

    *exact = true;
    key = (literal_prefix)
        ? impl_index_key_from_text(literal_prefix, p, xctx)
        : impl_index_entry_value_as_key_utf8(indexDefinition, entry,
            exact, p, xctx);
    if (afw_adapter_impl_index_option_case_insensitive(
        indexDefinition, xctx))
    {
        key = afw_utf8_to_lower(key, p, xctx);
    }

    return key;
}


/* Whether object_type declares name (not just its otherProperties). */
static afw_boolean_t
impl_index_object_type_declares(
    const afw_object_type_t *object_type,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    const afw_object_type_property_type_t *pt;
    const afw_value_string_t name_value = AFW_VALUE_STRING_UNMANAGED(name);

    pt = afw_object_type_property_type_get(object_type, &name_value.pub,
        xctx);
    return pt && pt != object_type->other_properties;
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
    afw_boolean_t       unique;
    const afw_utf8_t  * value_string;

    unique = afw_adapter_impl_index_option_unique(indexDefinition, xctx);

    /* Generate the utf8 value of the index; this will be used as the
        index key in the underlying database. */
    value_string = impl_index_object_value_key(indexDefinition, value,
        object->p, xctx);

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
 * Compile a definition's value and filter scripts into p. Either is NULL
 * when the definition has none.
 */
static void
impl_index_compile_scripts(
    const afw_object_t * indexDefinition,
    const afw_value_t ** value_script,
    const afw_value_t ** filter_script,
    const afw_pool_t   * p,
    afw_xctx_t         * xctx)
{
    const afw_utf8_t *s;

    s = afw_object_get_property_as_string_internal(
        indexDefinition, afw_v_value, xctx);
    *value_script = (s)
        ? afw_compile_to_value(s, NULL, afw_compile_type_script, NULL, p, xctx)
        : NULL;

    s = afw_object_get_property_as_string_internal(
        indexDefinition, afw_v_filter, xctx);
    *filter_script = (s)
        ? afw_compile_to_value(s, NULL, afw_compile_type_script, NULL, p, xctx)
        : NULL;
}

/*
 * What an index definition gives an object: its value script's result,
 * or the property named key when there is no value script. NULL when
 * the filter script is false or the result is nullish. Writing an
 * index and every query test of a computed name get values here, so a
 * query means what the index holds (issue #516).
 *
 * The scripts are compiled (impl_index_compile_scripts). While they
 * evaluate, current:: holds the object, its id and type, and key
 * (issue #54).
 */
static const afw_value_t *
impl_index_evaluate(
    const afw_utf8_t   * key,
    const afw_object_t * object,
    const afw_utf8_t   * object_type_id,
    const afw_utf8_t   * object_id,
    const afw_value_t  * value_script,
    const afw_value_t  * filter_script,
    const afw_pool_t   * p,
    afw_xctx_t         * xctx)
{
    const afw_value_t *result;
    const afw_value_t *eval;
    impl_index_eval_ctx_t eval_ctx;
    int top;

    /* No scripts: the property named key, no current:: needed. */
    if (!value_script && !filter_script) {
        if (!key) {
            return NULL;
        }
        {
            const afw_value_string_t key_value =
                AFW_VALUE_STRING_UNMANAGED(key);
            result = afw_object_get_property(object, &key_value.pub, xctx);
        }
        return (afw_value_is_nullish(result)) ? NULL : result;
    }

    /*
     * Push current:: for filter and value scripts (issue #54). Always restore
     * the qualifier stack, including on error or early filter fail.
     */
    result = NULL;
    top = afw_xctx_qualifier_stack_top_get(xctx);
    AFW_TRY {

        eval_ctx.object = afw_value_create_unmanaged_object(
            object, p, xctx);
        eval_ctx.objectId = (object_id)
            ? afw_value_create_unmanaged_string(object_id, p, xctx)
            : NULL;
        eval_ctx.objectType = (object_type_id)
            ? afw_value_create_unmanaged_string(object_type_id, p, xctx)
            : NULL;
        eval_ctx.key = (key)
            ? afw_value_create_unmanaged_string(key, p, xctx)
            : NULL;

        afw_context_push_cb_variables(afw_s_current,
            impl_index_current_variables, &eval_ctx, p, xctx);

        /* A filter must be boolean; false means no value. */
        if (filter_script) {
            eval = afw_value_evaluate(filter_script, p, xctx);
            if (!afw_value_is_boolean(eval)) {
                AFW_THROW_ERROR_Z(general,
                    "Error: filter evaluation did not end with a boolean "
                    "result.", xctx);
            }
            if (!afw_value_as_boolean_internal(eval, p, xctx)) {
                break;
            }
        }

        if (value_script) {
            result = afw_value_evaluate(value_script, p, xctx);
        }
        else if (key) {
            const afw_value_string_t key_value =
                AFW_VALUE_STRING_UNMANAGED(key);
            result = afw_object_get_property(object, &key_value.pub, xctx);
        }
    }
    AFW_FINALLY {
        afw_xctx_qualifier_stack_top_set(top, xctx);
    }
    AFW_ENDTRY;

    return (afw_value_is_nullish(result)) ? NULL : result;
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
    const afw_value_t  * value_script;
    const afw_value_t  * filter_script;
    const afw_value_t  * eval;
    int i;

    /* first we make sure the objectType is applicable */
    if (!afw_adapter_impl_index_object_type_applicable(
        indexDefinition, object_type_id, xctx)) {
        /* the object_type_id does not apply for this definition */
        return false;
    }

    impl_index_compile_scripts(indexDefinition,
        &value_script, &filter_script, object->p, xctx);
    eval = impl_index_evaluate(key, object, object_type_id, object_id,
        value_script, filter_script, object->p, xctx);

    /* if eval is nullish, then we didn't generate a value and shouldn't index */
    if (!eval) {
        return false;
    }

    /* Check the type of afw_value_t we got back. */
    if (afw_value_is_object(eval))
    {
        /* we can't use an object as an index key */
        AFW_THROW_ERROR_Z(general,
            "Error: value expression generated an object and cannot be used as an index.",
            xctx);
    }

    /* if we have multiple values, then index each one */
    if (afw_value_is_array(eval))
    {
        index_values = afw_value_to_null_terminated_values(
            eval, object->p, xctx);
        for (i = 0; index_values[i]; i++) {
            afw_adapter_impl_index_apply(instance, indexDefinition,
                object_type_id, object_id, object, key, index_values[i],
                operation, xctx);
        }
        return true;
    }

    /* a single value can be converted to a single utf8 string */
    if (afw_value_is_defined_and_evaluated(eval))
    {
        afw_adapter_impl_index_apply(instance, indexDefinition,
            object_type_id, object_id, object, key, eval, operation, xctx);
        return true;
    }

    AFW_THROW_ERROR_Z(general,
        "Error: value expression generated an unknown and unhandled index value.",
        xctx);
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
        option = afw_array_get_next_value(options, &option_iterator, xctx);
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
                options, &option_iterator, xctx);
        }
    }

    objectType = afw_object_get_property_as_array_internal(
        indexDefinition, afw_v_objectType, xctx);

    /* An omitted or empty objectType list means all object types. */
    object_type_iterator = NULL;
    object_type_id = (objectType)
        ? afw_array_of_string_get_next_internal(
            objectType, &object_type_iterator, xctx)
        : NULL;
    if (object_type_id) {
        do {
            afw_adapter_impl_index_open(indexer, object_type_id,
                key, unique, reverse, pool, xctx);

            object_type_id = afw_array_of_string_get_next_internal(
                objectType, &object_type_iterator, xctx);
        } while (object_type_id);
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
            afw_object_string_property_name_internal(key, xctx), 
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
    const afw_object_t             * indexDefinitions;
    const afw_object_t             * indexDefinition;
    const afw_object_t             * result;

    session = afw_adapter_session_get_cached(adapterId, false, xctx);

    instance = afw_adapter_session_get_index_interface(session, xctx);
    if (instance == NULL) {
        AFW_THROW_ERROR_Z(general,
            "Error: Cannot find index interface for adapterId.", xctx);
    }

    /* fetch/refresh (issue #252 item 3) rather than trust a possibly
       stale cached instance->indexDefinitions directly */
    indexDefinitions = afw_adapter_impl_index_get_index_definitions(
        instance, xctx);

    result = indexDefinitions;

    if (object_type_id) {
        result = afw_object_create_unmanaged_new_p(pool, xctx);

        index_iterator = NULL;
        const afw_value_t *key;

        indexDefinition = afw_object_get_next_property_as_object_internal(
            indexDefinitions, &index_iterator, &key, xctx);
        while (indexDefinition) {
            if (afw_adapter_impl_index_object_type_applicable(
                indexDefinition, object_type_id, xctx)) {
                afw_object_set_property_as_object_internal(result,
                    key, indexDefinition, xctx);
            }

            indexDefinition = afw_object_get_next_property_as_object_internal(
                indexDefinitions, &index_iterator, &key, xctx);
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
    const afw_object_t                 * indexDefinitions;
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

    /* fetch/refresh (issue #252 item 3) so a remove on a long-lived
       session sees index definitions added elsewhere in the meantime */
    indexDefinitions = afw_adapter_impl_index_get_index_definitions(
        indexer, xctx);

    if (indexDefinitions == NULL) {
        AFW_THROW_ERROR_Z(general,
            "Error: there are no index definitions to remove.", xctx);
    }

    indexDefinition = afw_object_get_property_as_object_internal(
        indexDefinitions,
        afw_value_create_unmanaged_string(key, pool, xctx), xctx);
    if (indexDefinition == NULL) {
        AFW_THROW_ERROR_Z(general,
            "Error there is no index definition by this key.", xctx);
    }

    /* first, we remove the index from the configuration,
        so it's no longer in use */
    afw_object_remove_property(indexDefinitions,
        afw_value_create_unmanaged_string(key, pool, xctx), xctx);

    afw_adapter_impl_index_update_index_definitions(
        indexer, indexDefinitions, xctx);

    /*
     * get all applicable objectTypes. An omitted or empty objectType list
     * means all object types: drop once with no object type.
     */
    objectTypes = afw_object_get_property_as_array_internal(
        indexDefinition, afw_v_objectType, xctx);
    object_type_iterator = NULL;
    object_type_id = (objectTypes)
        ? afw_array_of_string_get_next_internal(
            objectTypes, &object_type_iterator, xctx)
        : NULL;
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
            object_type_id = afw_array_of_string_get_next_internal(
                objectTypes, &object_type_iterator, xctx);

    } while (object_type_id);

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
    const afw_object_type_t            * object_type;
    const afw_object_type_property_type_t * property_type;
    const afw_data_type_t              * option_data_type;

    session = afw_adapter_session_get_cached(adapterId, false, xctx);

    indexer = afw_adapter_session_get_index_interface(session, xctx);
    if (indexer == NULL) {
        AFW_THROW_ERROR_Z(general,
            "Error: unable to get index interface.", xctx);
    }

    /*
     * A value or filter script makes key a computed name: a query on it
     * means what the scripts give (issue #516). A filter on an index of a
     * real property would leave objects that have the property out of
     * every query the index answers, so a filter needs a value script,
     * and a computed name must not be a property an object type
     * declares (a query checks again, for object types changed later).
     */
    if (filter && !value) {
        AFW_THROW_ERROR_Z(general,
            "An index with a filter needs a value script and a name "
            "objects don't have: a filter on an index of a property would "
            "leave objects that have it out of queries", xctx);
    }
    if ((value || filter) && objectType) {
        for (object_type_iterator = NULL;;) {
            object_type_id = afw_array_of_string_get_next_internal(
                objectType, &object_type_iterator, xctx);
            if (!object_type_id) {
                break;
            }
            object_type = afw_adapter_get_object_type(adapterId,
                object_type_id, afw_object_create_unmanaged(pool, xctx),
                xctx);
            if (object_type &&
                impl_index_object_type_declares(object_type, key, xctx))
            {
                AFW_THROW_ERROR_FZ(general, xctx,
                    "Object type '%ku' declares property '%ku'; an index "
                    "with a value or filter script needs a name objects "
                    "don't have",
                    object_type_id, key);
            }
        }
    }

    /* create our result object to be returned */
    result = afw_object_create_unmanaged_new_p(pool, xctx);

    /* fetch/refresh (issue #252 item 3) so a create on a long-lived
       session doesn't clobber index definitions added elsewhere in the
       meantime */
    indexDefinitions = afw_adapter_impl_index_get_index_definitions(
        indexer, xctx);

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

    /*
     * The integer / double option says what the index's values are when
     * no object type does (#544). It throws for both options; an object
     * type that declares key as another data type contradicts it.
     */
    option_data_type = impl_index_option_data_type(indexDefinition, xctx);
    if (option_data_type && objectType) {
        const afw_value_string_t key_value = AFW_VALUE_STRING_UNMANAGED(key);

        for (object_type_iterator = NULL;;) {
            object_type_id = afw_array_of_string_get_next_internal(
                objectType, &object_type_iterator, xctx);
            if (!object_type_id) {
                break;
            }
            object_type = afw_adapter_get_object_type(adapterId,
                object_type_id, afw_object_create_unmanaged(pool, xctx),
                xctx);
            property_type = (object_type)
                ? afw_object_type_property_type_get(object_type,
                    &key_value.pub, xctx)
                : NULL;
            if (property_type &&
                property_type != object_type->other_properties &&
                property_type->data_type &&
                property_type->data_type != option_data_type)
            {
                AFW_THROW_ERROR_FZ(general, xctx,
                    "Object type '%ku' declares property '%ku' as %ku, "
                    "but the index option says %ku",
                    object_type_id, key,
                    &property_type->data_type->data_type_id,
                    &option_data_type->data_type_id);
            }
        }
    }

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

    /* A throw (for example a value that does not compile) still ends it. */
    AFW_TRY {

        /* the callback routine does all of our work for us */
        if (retroactive) {
            /*
             * If this definition is scoped to specific objectTypes, scan
             * only those types instead of paying for a full-table walk
             * (mirrors the per-objectType loop
             * afw_adapter_impl_index_remove() already uses). An omitted or
             * empty objectType list means all types apply, so fall back to
             * a single unscoped scan for that case.
             */
            object_type_iterator = NULL;
            object_type_id = (objectType)
                ? afw_array_of_string_get_next_internal(
                    objectType, &object_type_iterator, xctx)
                : NULL;

            if (object_type_id) {
                do {
                    /** @fixme should this be session->p, pool, or xctx->p? */
                    afw_adapter_session_retrieve_objects(session, NULL,
                        object_type_id, NULL, &ctx,
                        afw_adapter_impl_index_cb, NULL, pool, xctx);

                    object_type_id = afw_array_of_string_get_next_internal(
                        objectType, &object_type_iterator, xctx);
                } while (object_type_id);
            } else {
                /** @fixme should this be session->p, pool, or xctx->p? */
                afw_adapter_session_retrieve_objects(session, NULL, NULL,
                    NULL, &ctx, afw_adapter_impl_index_cb, NULL, pool, xctx);
            }
        }

        /*
         * Now, tell the adapter to add the new indexDefinition for
         * configuration. indexDefinitions is the indexer's cached copy and
         * outlives pool, so it gets a copy of the definition in its own pool.
         */
        afw_object_set_property_as_object_internal(
            indexDefinitions,
            afw_value_create_unmanaged_string(key, pool, xctx),
            afw_object_create_pooled_copy(indexDefinition,
                indexDefinitions->p, xctx),
            xctx);

        afw_adapter_impl_index_update_index_definitions(
            indexer, indexDefinitions, xctx);

        if (transaction) {
            afw_adapter_transaction_commit(transaction, xctx);
        }
    }
    AFW_FINALLY {
        if (transaction) {
            afw_adapter_transaction_release(transaction, xctx);
        }
    }
    AFW_ENDTRY;

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
AFW_DEFINE(afw_boolean_t) afw_adapter_impl_index_is_property_indexed(
    const afw_adapter_impl_index_t * instance,
    const afw_utf8_t               * object_type_id,
    const afw_utf8_t               * property_name,
    afw_xctx_t                    * xctx)
{
    const afw_object_t * indexDefinitions;
    const afw_object_t * indexDefinition;

    /* fetch/refresh (issue #252 item 3) */
    indexDefinitions = afw_adapter_impl_index_get_index_definitions(
        instance, xctx);

    if (indexDefinitions) {
        const afw_value_string_t property_name_value =
            AFW_VALUE_STRING_UNMANAGED(property_name);
        indexDefinition = afw_object_get_property_as_object_internal(
            indexDefinitions, &property_name_value.pub, xctx);
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
    const afw_object_t * indexDefinitions;
    const afw_object_t * indexDefinition = NULL;

    /* fetch/refresh (issue #252 item 3) -- this is also what query
       planning (sargable/cursor_list) relies on to see indexes created
       or removed since this session's indexer was created */
    indexDefinitions = afw_adapter_impl_index_get_index_definitions(
        instance, xctx);

    if (indexDefinitions) {
        const afw_value_string_t property_name_value =
            AFW_VALUE_STRING_UNMANAGED(property_name);
        indexDefinition = afw_object_get_property_as_object_internal(
            indexDefinitions, &property_name_value.pub, xctx);
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
    const afw_object_t * indexDefinitions;
    const afw_object_t * indexDefinition;
    const afw_iterator_old_t * index_iterator;

    /*
     * Fetch/refresh rather than trust this instance's own possibly-stale
     * cached copy (issue #252 item 3): a long-lived session must not
     * silently skip indexing under a definition created by another
     * session after this indexer was constructed.
     */
    indexDefinitions = afw_adapter_impl_index_get_index_definitions(
        instance, xctx);

    if (indexDefinitions) {
        /* iterate through each indexDefinition to see if it applies */
        index_iterator = NULL;
        indexDefinition = afw_object_get_next_property_as_object_internal(
            indexDefinitions, &index_iterator, &index_name, xctx);
        while (indexDefinition) {
            afw_adapter_impl_index_try(instance,
                afw_object_string_property_name_internal(index_name, xctx),
                object, 
                object_type_id, object_id, indexDefinition, 
                afw_adapter_impl_index_mode_add, xctx);

            indexDefinition = afw_object_get_next_property_as_object_internal(
                indexDefinitions, &index_iterator, &index_name, xctx);

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
    const afw_object_t * indexDefinitions;
    const afw_object_t * indexDefinition;
    const afw_iterator_old_t * index_iterator;

    /* fetch/refresh (issue #252 item 3) */
    indexDefinitions = afw_adapter_impl_index_get_index_definitions(
        instance, xctx);

    if (indexDefinitions) {
        /* iterate through each indexDefinition to see if it applies */
        index_iterator = NULL;
        indexDefinition = afw_object_get_next_property_as_object_internal(
            indexDefinitions, &index_iterator, &index_name, xctx);
        while (indexDefinition) {
            afw_adapter_impl_index_try(instance,
                afw_object_string_property_name_internal(index_name, xctx),
                object, 
                object_type_id, object_id, indexDefinition,
                afw_adapter_impl_index_mode_delete, xctx);

            indexDefinition = afw_object_get_next_property_as_object_internal(
                indexDefinitions, &index_iterator, &index_name, xctx);

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
    const afw_object_t * indexDefinitions;
    const afw_object_t * indexDefinition;

    /* fetch/refresh (issue #252 item 3) */
    indexDefinitions = afw_adapter_impl_index_get_index_definitions(
        instance, xctx);

    if (indexDefinitions) {
        index_iterator = NULL;
        indexDefinition = afw_object_get_next_property_as_object_internal(
            indexDefinitions, &index_iterator, &index_name, xctx);
        while (indexDefinition) {
            /* remove indexes from the old object */
            afw_adapter_impl_index_try(instance,
                afw_object_string_property_name_internal(index_name, xctx),
                old_object,
                object_type_id, object_id, indexDefinition,
                afw_adapter_impl_index_mode_delete, xctx);

            /* now add back the new ones */
            afw_adapter_impl_index_try(instance,
                afw_object_string_property_name_internal(index_name, xctx),
                new_object,
                object_type_id, object_id, indexDefinition,
                afw_adapter_impl_index_mode_add, xctx);

            indexDefinition = afw_object_get_next_property_as_object_internal(
                indexDefinitions, &index_iterator, &index_name, xctx);
        }
    }
}

/* useful macro for determining if node, x, is a non-leaf node */
#define AFW_QUERY_CRITERIA_CONTINUE(_x) \
    (_x != AFW_QUERY_CRITERIA_FALSE && _x != AFW_QUERY_CRITERIA_TRUE)

/*
 * Whether an index cursor can answer this entry's operator: eq, lt, le,
 * gt, ge, or a match that is a literal "starts with".
 */
static afw_boolean_t
impl_index_op_is_sargable(
    const afw_query_criteria_filter_entry_t *entry,
    afw_boolean_t is_starts_with)
{
    return
        entry->op_id == afw_query_criteria_filter_op_id_eq ||
        entry->op_id == afw_query_criteria_filter_op_id_lt ||
        entry->op_id == afw_query_criteria_filter_op_id_le ||
        entry->op_id == afw_query_criteria_filter_op_id_gt ||
        entry->op_id == afw_query_criteria_filter_op_id_ge ||
        is_starts_with;
}


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
    afw_boolean_t is_starts_with;

    /*
     * A match entry is only sargable when it's the literal-prefix
     * ("starts with") shape - see impl_index_match_literal_prefix().
     * Any other match pattern (general regex) is not translatable to an
     * index range scan and falls through to a full scan.
     */
    is_starts_with = entry->op_id == afw_query_criteria_filter_op_id_match &&
        impl_index_match_literal_prefix(entry, xctx->p, xctx) != NULL;

    /* For now, we will only evaluate certain operations for sargability */
    if (!impl_index_op_is_sargable(entry, is_starts_with))
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
    impl_index_cursor_p_vector_t   * cursor_list,
    size_t                         * cardinality,
    afw_xctx_t                    * xctx)
{
    const afw_adapter_impl_index_cursor_t *cursor;
    afw_boolean_t rc;
    size_t c;
    int i;

    /* no cursors in the list indicates they were not sargable */
    if (!cursor_list || cursor_list->count == 0)
        return false;
  
    *cardinality = 0;
    for (i = 0; i < (int)cursor_list->count; i++) {
        cursor = cursor_list->entries[i];

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
impl_index_cursor_p_vector_t * afw_adapter_impl_index_cursor_list_join(
    const afw_adapter_impl_index_t * instance,
    impl_index_cursor_p_vector_t   * this_list,
    impl_index_cursor_p_vector_t   * that_list,
    afw_xctx_t                    * xctx)
{
    size_t this_cardinality, that_cardinality;
    afw_adapter_impl_index_cursor_t *cursor;
    afw_boolean_t rc;
    int i;

    /* unless we can do a real inner-join, we must indicate that
        the "joined" result list is not the result of an inner-join, but 
        a left or right outer join */
    for (i = 0; i < (int)this_list->count; i++) {
        cursor = (afw_adapter_impl_index_cursor_t *)this_list->entries[i];
        cursor->inner_join = false;
    }

    for (i = 0; i < (int)that_list->count; i++) {
        cursor = (afw_adapter_impl_index_cursor_t *)that_list->entries[i];
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
 * During a disjunction (OR), we merge two cursor lists together so
 * every cursor from both sides ends up in the result (the later dedup
 * pass in afw_adapter_impl_index_query()/afw_adapter_impl_index_applies()
 * handles objects that satisfy more than one cursor). We sort them
 * lowest cardinality first to make that dedup pass cheaper, but that
 * ordering is only ever a hint: whether a cursor's cardinality is
 * known, or how it compares, must never affect *whether* it ends up in
 * the merged list, only *where*.
 *
 * Issue #296: this used to get that wrong two ways --
 *  - this_cursor was only inserted when it won a cardinality comparison
 *    against some that_list entry; if it never won (a tie, or simply
 *    the smaller side), it was silently dropped from the result -
 *    reproduced even with two plain eq cursors on a cardinality tie.
 *  - a cursor whose cardinality couldn't be determined (anything but eq
 *    - see afw_adapter_impl_index_cursor_get_count()) made the whole
 *    merge fail (return NULL or throw) instead of just skipping the
 *    ordering comparison for that cursor.
 *
 * Issue #303: it also sorted in the wrong direction. In
 * afw_adapter_impl_index_query()'s dedup loop, a cursor at position i
 * pays one cheap afw_adapter_impl_index_applies() check per object it
 * yields for each of the (count - i - 1) cursors after it - the last
 * position pays nothing. Total dedup-check cost is therefore
 * sum(objects_at(i) * (count - i - 1)), which the rearrangement
 * inequality minimizes by putting the *smallest* cursor first (many
 * cheap per-object checks, but few objects) and the *largest* last
 * (many objects, but zero checks each) - the opposite of "highest
 * cardinality first."
 */
impl_index_cursor_p_vector_t * afw_adapter_impl_index_cursor_list_merge(
    const afw_adapter_impl_index_t * instance,
    impl_index_cursor_p_vector_t   * this_list,
    impl_index_cursor_p_vector_t   * that_list,
    afw_xctx_t                    * xctx)
{
    impl_index_cursor_p_vector_t *merged_list;
    impl_index_cursor_p_vector_t *temp;
    const afw_adapter_impl_index_cursor_t *this_cursor;
    const afw_adapter_impl_index_cursor_t *that_cursor;
    size_t this_cardinality, that_cardinality;
    afw_boolean_t have_this_cardinality, have_that_cardinality;
    afw_boolean_t inserted;
    afw_size_t merged_size;
    int i, j;

    /*
     * An "or" with a side that has no cursors (not indexable) can't be
     * answered from cursors at all: return no cursors, so an enclosing
     * "and" takes its other side (afw_adapter_impl_index_cursor_list_join).
     * This returned NULL when this_list was empty, and the join
     * dereferenced it; returning the other side would lose objects.
     */
    if (!this_list || this_list->count == 0 ||
        !that_list || that_list->count == 0)
    {
        return afw_vector_create(impl_index_cursor_p_vector_t,
            8, xctx->p, xctx);
    }

    merged_size = this_list->count + that_list->count;
    merged_list = NULL;
    temp = that_list;

    /* walk through each item in this_list and merge it into a new one */
    for (i = 0; i < (int)this_list->count; i++) {
        merged_list = afw_vector_create(impl_index_cursor_p_vector_t,
            merged_size, xctx->p, xctx);

        this_cursor = this_list->entries[i];

        have_this_cardinality = afw_adapter_impl_index_cursor_get_count(
            this_cursor, &this_cardinality, xctx);
        inserted = false;

        for (j = 0; j < (int)temp->count; j++) {
            that_cursor = temp->entries[j];

            have_that_cardinality = afw_adapter_impl_index_cursor_get_count(
                that_cursor, &that_cardinality, xctx);

            if (!inserted && have_this_cardinality && have_that_cardinality &&
                this_cardinality < that_cardinality)
            {
                afw_vector_push(merged_list, xctx) = this_cursor;
                inserted = true;
            }

            afw_vector_push(merged_list, xctx) = that_cursor;
        }

        /*
         * this_cursor was never smaller than anything in temp -- either
         * its cardinality (or a that_cursor's) couldn't be determined,
         * or it genuinely is the largest, which belongs at the end
         * anyway (issue #303). It must still appear in the result
         * exactly once regardless (issue #296 defect 1).
         */
        if (!inserted) {
            afw_vector_push(merged_list, xctx) = this_cursor;
        }

        temp = afw_vector_copy(impl_index_cursor_p_vector_t,
            merged_list, xctx->p, xctx);
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
impl_index_cursor_p_vector_t * afw_adapter_impl_index_cursor_list(
    const afw_adapter_impl_index_t          * instance,
    const afw_utf8_t                        * object_type_id,
    const afw_query_criteria_filter_entry_t * entry,
    afw_xctx_t                             * xctx)
{  
    afw_adapter_impl_index_cursor_t *cursor = NULL;
    impl_index_cursor_p_vector_t *cursor_list, *next_list;
    const afw_object_t *indexDefinition;
    const afw_utf8_t *value_string;
    const afw_utf8_t *literal_prefix = NULL;
    int cursor_operator;
    afw_boolean_t unique;
    afw_boolean_t exact = true;

    /* allocate our cursor_list that will contain a conjunction
        of cursors, representing this particular decision branch. */
    cursor_list = afw_vector_create(impl_index_cursor_p_vector_t,
        8, xctx->p, xctx);

    if (entry == NULL) {
        /* No entry means empty cursor list */
        return cursor_list;
    }

    if (entry->op_id == afw_query_criteria_filter_op_id_match) {
        literal_prefix = impl_index_match_literal_prefix(
            entry, xctx->p, xctx);
    }

    cursor_operator = (literal_prefix)
        ? AFW_ADAPTER_IMPL_INDEX_OPERATOR_STARTS_WITH
        : (int)entry->op_id;

    /*
     * Determine if this property is indexed. Only an operator an index
     * can answer gets a cursor (out, ne, in, ... on an indexed property
     * threw "Unable to create cursor for this operator"); the rest is
     * tested on the objects the other cursors find.
     */
    indexDefinition = (impl_index_op_is_sargable(entry, literal_prefix != NULL))
        ? afw_adapter_impl_index_get_index_definition(
            instance, object_type_id, entry->property_name, xctx)
        : NULL;
    if (indexDefinition)
    {
        /* use the internal utf-8 string representation; integer/double
           get the sortable encoding so range ops walk keys in numeric
           order (issue #251). A match entry that reduces to a literal
           "starts with" prefix (see impl_index_match_literal_prefix())
           seeks on that prefix instead of the raw regex pattern text.
           A case-insensitive index seeks the lowercased key. */
        value_string = impl_index_entry_seek_key(indexDefinition, entry,
            literal_prefix, &exact, xctx->p, xctx);
        unique = afw_adapter_impl_index_option_unique(
            indexDefinition, xctx);

        /* get this cursor and add it to our current cursor list */
        /** @fixme we need to register open cursors to be released, because we
            may not know exactly when to discard them */
        cursor = afw_adapter_impl_index_open_cursor(instance, object_type_id,
            entry->property_name,
            cursor_operator, value_string, unique, xctx->p, xctx);
    }

    if (cursor) {
        /* A value that didn't convert to the index's number type: re-test. */
        cursor->inner_join = exact;
         
        /* remember the afw_query_criteria_filter_entry for later */ 
        cursor->filter_entry = entry;

        afw_vector_push(cursor_list, xctx) = cursor;
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

/* -------------------------------------------------------------------------
 * Query tests through index definitions (issue #516)
 *
 * An index can give a name another meaning than the object's property:
 * a value script (a computed name, maybe with a filter), or the
 * case-insensitive-string option. Cursors find what the index holds, so
 * the re-test of an index query and an adapter's scan test those names
 * the same way. A query that names neither kind uses
 * afw_query_criteria_test_object() as before.
 * ------------------------------------------------------------------------- */

typedef struct impl_index_query_name_s impl_index_query_name_t;

/* A relation of the filter, and how a query test gets its value. */
struct impl_index_query_name_s {
    impl_index_query_name_t *next;
    const afw_query_criteria_filter_entry_t *entry;

    /* entry, or for case-insensitive a copy with lowercased values. */
    const afw_query_criteria_filter_entry_t *test_entry;

    /* False: the object's property, as afw_query_criteria_test_object(). */
    afw_boolean_t through_definition;

    afw_boolean_t case_insensitive;

    /* Compiled once per query; NULL when the definition has none. */
    const afw_value_t *value_script;
    const afw_value_t *filter_script;
};

struct afw_adapter_impl_index_query_test_s {
    const afw_adapter_impl_index_t *instance;
    const afw_utf8_t *object_type_id;
    const afw_query_criteria_t *criteria;
    impl_index_query_name_t *first;
};


static void
impl_index_regexp_cleanup(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx)
{
    (void)data2;
    (void)p;
    (void)xctx;
    xmlRegFreeRegexp((xmlRegexpPtr)data);
}


/* A string, or each string of an array, lowercased. */
static const afw_value_t *
impl_index_value_to_lower(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t * const *values;
    const afw_value_t **lowered;
    afw_size_t count;
    afw_size_t i;

    if (afw_value_is_string(value)) {
        return afw_value_create_unmanaged_string(
            afw_utf8_to_lower(
                (const afw_utf8_t *)AFW_VALUE_INTERNAL(value), p, xctx),
            p, xctx);
    }

    if (afw_value_is_array(value)) {
        values = afw_value_to_null_terminated_values(value, p, xctx);
        for (count = 0; values[count]; count++);
        lowered = afw_pool_calloc(p,
            sizeof(afw_value_t *) * (count + 1), xctx);
        for (i = 0; i < count; i++) {
            lowered[i] = (afw_value_is_string(values[i]))
                ? impl_index_value_to_lower(values[i], p, xctx)
                : values[i];
        }
        return afw_value_create_unmanaged_array(
            afw_array_create_unmanaged_from_values(
                NULL, lowered, count, p, xctx),
            p, xctx);
    }

    return value;
}


/*
 * A match pattern for a case-insensitive name: the pattern lowercased
 * like the values, except its escapes. \s and \S, \d and \D, ... differ
 * by case, and \p{...} / \P{...} name Unicode categories, so they are
 * kept as written. A category that names a case (\p{Lu}) never matches
 * a lowercased value.
 */
static const afw_utf8_t *
impl_index_pattern_to_lower(
    const afw_utf8_t *pattern,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *result;
    afw_utf8_t run;
    afw_utf8_t escape;
    afw_size_t start;
    afw_size_t i;
    afw_size_t j;

    result = afw_s_a_empty_string;
    for (i = 0, start = 0; i < pattern->len; ) {
        if (pattern->s[i] != '\\') {
            i++;
            continue;
        }

        escape.s = pattern->s + i;
        escape.len = (i + 1 < pattern->len) ? 2 : 1;
        if (escape.len == 2 &&
            (pattern->s[i + 1] == 'p' || pattern->s[i + 1] == 'P') &&
            i + 2 < pattern->len && pattern->s[i + 2] == '{')
        {
            for (j = i + 3; j < pattern->len && pattern->s[j] != '}'; j++);
            escape.len = ((j < pattern->len) ? j + 1 : pattern->len) - i;
        }

        run.s = pattern->s + start;
        run.len = i - start;
        result = afw_utf8_concat(p, xctx, result,
            afw_utf8_to_lower(&run, p, xctx), &escape, NULL);

        i += escape.len;
        start = i;
    }

    run.s = pattern->s + start;
    run.len = pattern->len - start;
    return afw_utf8_concat(p, xctx, result,
        afw_utf8_to_lower(&run, p, xctx), NULL);
}


/*
 * A copy of entry that compares with lowercased values: its value
 * lowercased, and for match and differ the pattern recompiled from
 * impl_index_pattern_to_lower().
 */
static const afw_query_criteria_filter_entry_t *
impl_index_entry_to_lower(
    const afw_query_criteria_filter_entry_t *entry,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_query_criteria_filter_entry_t *copy;
    const afw_utf8_t *pattern;
    xmlRegexpPtr regexp;

    copy = afw_pool_calloc_type(p, afw_query_criteria_filter_entry_t, xctx);
    afw_memory_copy(copy, entry);

    if (entry->alt_op_id == afw_query_criteria_filter_op_id_match &&
        entry->value && afw_value_is_string(entry->value))
    {
        pattern = impl_index_pattern_to_lower(
            (const afw_utf8_t *)AFW_VALUE_INTERNAL(entry->value), p, xctx);
        regexp = xmlRegexpCompile(
            BAD_CAST afw_utf8_to_utf8_z(pattern, p, xctx));
        if (!regexp) {
            AFW_THROW_ERROR_Z(general, "regexp syntax error", xctx);
        }
        afw_pool_register_cleanup(p, regexp, NULL,
            impl_index_regexp_cleanup, xctx);
        copy->op_specific = regexp;
        copy->value = afw_value_create_unmanaged_string(pattern, p, xctx);
    }
    else if (entry->value) {
        copy->value = impl_index_value_to_lower(entry->value, p, xctx);
    }

    return copy;
}


static impl_index_query_name_t *
impl_index_query_test_name(
    const afw_adapter_impl_index_query_test_t *test,
    const afw_query_criteria_filter_entry_t *entry)
{
    impl_index_query_name_t *name;

    for (name = (test) ? test->first : NULL;
        name && name->entry != entry;
        name = name->next);

    return name;
}


/*
 * Add every relation reachable from entry, the ones the filter walk can
 * visit, once each.
 */
static void
impl_index_query_test_add(
    afw_adapter_impl_index_query_test_t *test,
    const afw_query_criteria_filter_entry_t *entry,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    impl_index_query_name_t *name;
    const afw_object_t *indexDefinition;
    const afw_object_type_t *object_type;

    if (entry == AFW_QUERY_CRITERIA_TRUE || entry == AFW_QUERY_CRITERIA_FALSE ||
        impl_index_query_test_name(test, entry))
    {
        return;
    }

    name = afw_pool_calloc_type(p, impl_index_query_name_t, xctx);
    name->entry = entry;
    name->test_entry = entry;
    name->next = test->first;
    test->first = name;

    indexDefinition = (entry->property_name)
        ? afw_adapter_impl_index_get_index_definition(test->instance,
            test->object_type_id, entry->property_name, xctx)
        : NULL;
    if (indexDefinition) {
        name->case_insensitive = afw_adapter_impl_index_option_case_insensitive(
            indexDefinition, xctx);
        impl_index_compile_scripts(indexDefinition,
            &name->value_script, &name->filter_script, p, xctx);
        name->through_definition = name->case_insensitive ||
            name->value_script || name->filter_script;
        if (name->case_insensitive) {
            name->test_entry = impl_index_entry_to_lower(entry, p, xctx);
        }

        /*
         * index_create refuses a computed name an object type declares;
         * an object type changed since then is caught here.
         */
        object_type = test->criteria->object_type;
        if ((name->value_script || name->filter_script) &&
            object_type && entry->pt &&
            entry->pt != object_type->other_properties)
        {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Index '%ku' has a value or filter script, but object type "
                "'%ku' declares property '%ku'; rename or remove the index",
                entry->property_name, object_type->object_type_id,
                entry->property_name);
        }
    }

    impl_index_query_test_add(test, entry->on_true, p, xctx);
    impl_index_query_test_add(test, entry->on_false, p, xctx);
}


/* afw_query_criteria_get_value_cb_t for a query test. */
static const afw_value_t *
impl_index_query_test_get_value(
    const afw_object_t *obj,
    const afw_query_criteria_filter_entry_t *entry,
    const afw_query_criteria_filter_entry_t **test_entry,
    void *data,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_adapter_impl_index_query_test_t *test = data;
    const impl_index_query_name_t *name;
    const afw_value_t *value;

    name = impl_index_query_test_name(test, entry);
    if (!name || !name->through_definition) {
        return afw_object_get_property_extended(obj,
            entry->property_name, xctx);
    }

    *test_entry = name->test_entry;
    value = impl_index_evaluate(entry->property_name, obj,
        test->object_type_id, afw_object_meta_get_object_id(obj, xctx),
        name->value_script, name->filter_script, p, xctx);
    if (value && name->case_insensitive) {
        value = impl_index_value_to_lower(value, p, xctx);
    }

    return value;
}


/* Create a test of criteria's filter through object_type_id's indexes. */
AFW_DEFINE(const afw_adapter_impl_index_query_test_t *)
afw_adapter_impl_index_query_test_create(
    const afw_adapter_impl_index_t * instance,
    const afw_utf8_t               * object_type_id,
    const afw_query_criteria_t     * criteria,
    const afw_pool_t               * p,
    afw_xctx_t                    * xctx)
{
    afw_adapter_impl_index_query_test_t *test;
    const impl_index_query_name_t *name;
    const afw_adapter_session_t *session;
    afw_boolean_t needed;

    if (!instance || !object_type_id || !criteria || !criteria->filter) {
        return NULL;
    }

    test = afw_pool_calloc_type(p, afw_adapter_impl_index_query_test_t,
        xctx);
    test->instance = instance;
    test->object_type_id = object_type_id;
    test->criteria = criteria;
    impl_index_query_test_add(test, criteria->filter, p, xctx);

    for (needed = false, name = test->first; name; name = name->next) {
        if (name->through_definition) {
            needed = true;
            session = afw_adapter_impl_index_get_session(instance, xctx);
            afw_trace_fz(1, session->adapter->trace_flag_index, NULL, xctx,
                "index query test: %ku through its index definition (%s)",
                name->entry->property_name,
                (name->value_script || name->filter_script)
                    ? "computed" : "case-insensitive");
        }
    }

    return (needed) ? test : NULL;
}


/* Test object against a query test's criteria. */
AFW_DEFINE(afw_boolean_t)
afw_adapter_impl_index_query_test_object(
    const afw_adapter_impl_index_query_test_t * test,
    const afw_object_t                        * object,
    const afw_pool_t                          * p,
    afw_xctx_t                               * xctx)
{
    return afw_query_criteria_test_object_cb(object, test->criteria,
        impl_index_query_test_get_value, (void *)test, p, xctx);
}


/*
 * afw_boolean_t afw_adapter_impl_index_applies()
 *
 * Whether cursor returns object: the object's values through the
 * cursor's index definition, as index keys, compared with the key the
 * cursor seeks. The dedup in afw_adapter_impl_index_query() skips an
 * object a later cursor returns, so this must be exactly the cursor's
 * set, or an object comes back twice or not at all. It read the filter's
 * property from the object, which a computed name doesn't have and a
 * case-insensitive index holds lowercased (issue #516).
 */
static afw_boolean_t afw_adapter_impl_index_applies(
    const afw_adapter_impl_index_t            * instance,
    const afw_adapter_impl_index_query_test_t * test,
    const afw_utf8_t                          * object_type_id,
    const afw_adapter_impl_index_cursor_t     * cursor,
    const afw_object_t                        * object,
    const afw_pool_t                          * p,
    afw_xctx_t                               * xctx)
{
    const afw_query_criteria_filter_entry_t *entry = cursor->filter_entry;
    const impl_index_query_name_t *name;
    const afw_object_t *indexDefinition;
    const afw_utf8_t *literal_prefix;
    const afw_utf8_t *seek_key;
    const afw_utf8_t *key;
    const afw_value_t *value_script;
    const afw_value_t *filter_script;
    const afw_value_t *value;
    const afw_value_t * const *values;
    const afw_value_t *single[2];
    afw_boolean_t exact;
    int compare;
    int i;

    indexDefinition = afw_adapter_impl_index_get_index_definition(
        instance, object_type_id, entry->property_name, xctx);
    if (!indexDefinition) {
        return false;
    }

    literal_prefix = (entry->op_id == afw_query_criteria_filter_op_id_match)
        ? impl_index_match_literal_prefix(entry, p, xctx)
        : NULL;
    seek_key = impl_index_entry_seek_key(indexDefinition, entry,
        literal_prefix, &exact, p, xctx);

    /* What the index holds for object, as afw_adapter_impl_index_try(). */
    name = impl_index_query_test_name(test, entry);
    if (name) {
        value_script = name->value_script;
        filter_script = name->filter_script;
    }
    else {
        impl_index_compile_scripts(indexDefinition,
            &value_script, &filter_script, p, xctx);
    }
    value = impl_index_evaluate(entry->property_name, object, object_type_id,
        afw_object_meta_get_object_id(object, xctx),
        value_script, filter_script, p, xctx);
    if (!value) {
        return false;
    }
    if (afw_value_is_array(value)) {
        values = afw_value_to_null_terminated_values(value, p, xctx);
    }
    else {
        single[0] = value;
        single[1] = NULL;
        values = single;
    }

    for (i = 0; values[i]; i++) {
        key = impl_index_object_value_key(indexDefinition, values[i],
            p, xctx);

        if (literal_prefix) {
            if (key->len >= seek_key->len &&
                memcmp(key->s, seek_key->s, seek_key->len) == 0)
            {
                return true;
            }
            continue;
        }

        compare = afw_utf8_compare(key, seek_key);
        switch (entry->op_id) {
            case afw_query_criteria_filter_op_id_eq:
                if (compare == 0) return true;
                break;
            case afw_query_criteria_filter_op_id_lt:
                if (compare < 0) return true;
                break;
            case afw_query_criteria_filter_op_id_le:
                if (compare <= 0) return true;
                break;
            case afw_query_criteria_filter_op_id_gt:
                if (compare > 0) return true;
                break;
            case afw_query_criteria_filter_op_id_ge:
                if (compare >= 0) return true;
                break;
            default:
                /* Only these operators get a cursor (cursor_list). */
                AFW_THROW_ERROR_Z(general, "Filter op invalid", xctx);
        }
    }

    return false;
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
    impl_index_cursor_p_vector_t *cursors;
    const afw_adapter_impl_index_query_test_t *test;
    const afw_adapter_impl_index_cursor_t *current_cursor;
    const afw_adapter_impl_index_cursor_t *next_cursor;
    const afw_adapter_session_t *session;
    const afw_object_t *object = NULL;
    const afw_pool_t *p;
    /*
     * Total afw_adapter_impl_index_applies() calls this query made -
     * issue #303's dedup-cost model, made externally observable (there is
     * no other way to see it) via a trace line when the query finishes.
     */
    size_t applies_calls = 0;
    int cursor_index = 0;
    int i;

    /*
     * Names a computed or case-insensitive index gives another meaning
     * are tested through it (NULL: none in this filter).
     */
    test = afw_adapter_impl_index_query_test_create(instance,
        object_type_id, criteria, pool, xctx);

    /* Recursively compute our cursors */
    cursors = afw_adapter_impl_index_cursor_list(instance,
        object_type_id, (criteria ? criteria->filter : NULL), xctx);

    if (!cursors || cursors->count == 0) {
        AFW_THROW_ERROR_Z(general,
            "Error: unable to parse filter into indexable cursors.", xctx);
    }

    current_cursor = cursors->entries[0];

    while (1)
    {
        p = afw_pool_tracker_create(pool, xctx);

        /* get the next value from this cursor */
        object = afw_adapter_impl_index_cursor_get_next_object(
            current_cursor, p, xctx);

        if (object == NULL) {
            /* No more objects left on this index cursor */
            afw_adapter_impl_index_cursor_release(current_cursor, xctx);

            cursor_index++;
            if ((int)cursors->count == cursor_index) {
                /* we're at the end of our cursors */
                session = afw_adapter_impl_index_get_session(instance, xctx);
                afw_trace_fz(1, session->adapter->trace_flag_index, NULL, xctx,
                    "index query: %d applies() checks for dedup",
                    (int)applies_calls);

                callback(NULL, context, xctx);

                afw_pool_release(p, xctx);
                return;
            }

            current_cursor = cursors->entries[cursor_index];

            afw_pool_release(p, xctx);
            continue;
        }

        /*
         * The reason we may need to test the object is because some of
         *  of the predicates may have specified non-indexed properties
         *  that we couldn't exclude automatically through joins.
         */
        if (!current_cursor->inner_join &&
            !((test)
                ? afw_adapter_impl_index_query_test_object(test, object,
                    p, xctx)
                : afw_query_criteria_test_object(object, criteria, p, xctx)))
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
        for (i = cursor_index+1; i < (int)cursors->count; i++) {
            next_cursor = cursors->entries[i];
            /* 
                The "applies" routine will check the filter entry for this
                cursor, along with the object's matching property value, 
                and determine whether this cursor contains the object. 
             */
            applies_calls++;
            if (afw_adapter_impl_index_applies(instance, test,
                object_type_id, next_cursor, object, p, xctx)) {
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
