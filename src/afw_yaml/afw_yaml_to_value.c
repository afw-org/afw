// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW yaml conversion functions.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_yaml_to_value.c
 * @brief YAML decode into adaptive values.
 */

#include "afw.h"
#include "afw_yaml.h"
#include <yaml.h>


/*
 * The reader works on libyaml events (yaml_parser_parse): libyaml resolves
 * the structure (block sequences with no indentation under a key, keys vs
 * values, explicit or implicit documents, anchors and aliases), and each
 * node is one call of impl_parse_node (#553).
 */

/* An anchored node: its value, its expanded node count, a scalar's text. */
typedef struct {
    const afw_value_t *value;
    afw_size_t nodes;
    const afw_utf8_t *text;
} impl_anchor_t;

typedef struct {
    yaml_parser_t parser;

    /* The current event. Deleted before the next one is parsed. */
    yaml_event_t event;
    afw_boolean_t has_event;

    afw_void_hash_table_t *anchors;
    const afw_object_t *embedding_object;
    const afw_value_t *property_name;
    const afw_pool_t *p;
    afw_boolean_t cede_p;
    afw_size_t parse_nesting;

    /* Nodes the result stands for, each alias counted as what it names. */
    afw_size_t nodes;
} impl_parser_t;


/*
 * Max nested YAML mappings/sequences. Stops C-stack overflow on
 * pathological input. Generous for real documents.
 */
#define AFW_YAML_PARSE_NESTING_MAX 256

/*
 * Max nodes the result stands for when every alias is counted as the node
 * it names. An alias shares its value, so memory stays small, but anything
 * that walks the result (stringify, encode) visits each alias in full: a
 * few lines of nested aliases would stand for billions of nodes.
 */
#define AFW_YAML_EXPANDED_NODES_MAX 1000000

#define IMPL_YAML_TAG_STR "tag:yaml.org,2002:str"

#define IMPL_LINE(parser) ((int)(parser)->event.start_mark.line + 1)
#define IMPL_COLUMN(parser) ((int)(parser)->event.start_mark.column + 1)


static const afw_value_t *
impl_parse_node(impl_parser_t *parser, afw_xctx_t *xctx);


/* Parse the next event into parser->event. */
static void
impl_next(impl_parser_t *parser, afw_xctx_t *xctx)
{
    if (parser->has_event) {
        yaml_event_delete(&parser->event);
        parser->has_event = false;
    }
    if (!yaml_parser_parse(&parser->parser, &parser->event)) {
        AFW_THROW_ERROR_FZ(syntax, xctx,
            "YAML %s, near line %d, column %d",
            (parser->parser.problem)
                ? parser->parser.problem : "parse error",
            (int)parser->parser.problem_mark.line + 1,
            (int)parser->parser.problem_mark.column + 1);
    }
    parser->has_event = true;
}


static void
impl_nodes_add(impl_parser_t *parser, afw_size_t nodes, afw_xctx_t *xctx)
{
    parser->nodes += nodes;
    if (parser->nodes > AFW_YAML_EXPANDED_NODES_MAX) {
        AFW_THROW_ERROR_FZ(syntax, xctx,
            "YAML aliases expand to more than %d nodes, near line %d, "
            "column %d",
            AFW_YAML_EXPANDED_NODES_MAX,
            IMPL_LINE(parser), IMPL_COLUMN(parser));
    }
}


/* Copy the current event's anchor, or NULL. */
static const afw_utf8_t *
impl_anchor_name(impl_parser_t *parser, afw_xctx_t *xctx)
{
    const yaml_char_t *anchor;

    switch (parser->event.type) {
    case YAML_SCALAR_EVENT:
        anchor = parser->event.data.scalar.anchor;
        break;
    case YAML_SEQUENCE_START_EVENT:
        anchor = parser->event.data.sequence_start.anchor;
        break;
    case YAML_MAPPING_START_EVENT:
        anchor = parser->event.data.mapping_start.anchor;
        break;
    default:
        anchor = NULL;
    }

    return (anchor)
        ? afw_utf8_create((const afw_utf8_octet_t *)anchor,
            AFW_UTF8_Z_LEN, xctx->p, xctx)
        : NULL;
}


/* The anchor an alias event names. */
static const impl_anchor_t *
impl_alias(impl_parser_t *parser, afw_xctx_t *xctx)
{
    const char *name;
    const impl_anchor_t *anchor;

    name = (const char *)parser->event.data.alias.anchor;
    anchor = afw_hash_table_get(parser->anchors, name, strlen(name));
    if (!anchor) {
        AFW_THROW_ERROR_FZ(syntax, xctx,
            "YAML alias *%s names no anchor before it, near line %d, "
            "column %d",
            name, IMPL_LINE(parser), IMPL_COLUMN(parser));
    }

    return anchor;
}


/*
 * Plain scalars: YAML 1.2 Core-ish — true/false/null/~ then full-string
 * integer/double via afw_number_parse; else string. Quoted / literal /
 * folded styles, and a scalar tagged !!str or ! (non-specific), are always
 * strings (never type-sniffed). Other tags are not applied.
 */
static const afw_value_t *
impl_parse_scalar(impl_parser_t *parser, afw_xctx_t *xctx)
{
    const afw_value_t *value;
    const afw_utf8_t *str;
    const afw_utf8_octet_t *s;
    const char *tag;
    afw_size_t len;
    afw_size_t nlen;
    afw_integer_t i;
    afw_double_t d;
    afw_boolean_t is_double;

    s = (const afw_utf8_octet_t *)parser->event.data.scalar.value;
    len = parser->event.data.scalar.length;
    tag = (const char *)parser->event.data.scalar.tag;

    if (parser->event.data.scalar.style != YAML_PLAIN_SCALAR_STYLE ||
        (tag && (strcmp(tag, IMPL_YAML_TAG_STR) == 0 ||
            strcmp(tag, "!") == 0)))
    {
        str = afw_utf8_create(s, len, parser->p, xctx);
        return afw_value_create_unmanaged_string(str, parser->p, xctx);
    }

    /* Boolean / null (lowercase Core Schema + common ~ and empty). */
    if (len == 4 && memcmp(s, "true", 4) == 0) {
        value = afw_boolean_v_true;
    }
    else if (len == 5 && memcmp(s, "false", 5) == 0) {
        value = afw_boolean_v_false;
    }
    else if (len == 0 ||
        (len == 4 && memcmp(s, "null", 4) == 0) ||
        (len == 1 && s[0] == '~'))
    {
        value = afw_value_null;
    }
    else {
        /*
         * Full-string number only (rejects "123foo"). Prefer integer
         * when the scalar has no fractional/exponent form.
         */
        nlen = afw_number_parse(s, len, &i, &d, &is_double,
            parser->p, xctx);
        if (nlen == len) {
            value = (is_double)
                ? afw_value_create_unmanaged_double(d, parser->p, xctx)
                : afw_value_create_unmanaged_integer(i, parser->p, xctx);
        }
        else {
            str = afw_utf8_create(s, len, parser->p, xctx);
            value = afw_value_create_unmanaged_string(
                str, parser->p, xctx);
        }
    }

    return value;
}


/* Sequence: the current event is its start. Ends on its end event. */
static const afw_value_t *
impl_parse_sequence(impl_parser_t *parser, afw_xctx_t *xctx)
{
    const afw_array_t *list;
    const afw_value_t *value;

    list = afw_array_create_unmanaged(parser->p, xctx);
    for (;;) {
        impl_next(parser, xctx);
        if (parser->event.type == YAML_SEQUENCE_END_EVENT) {
            break;
        }
        value = impl_parse_node(parser, xctx);
        afw_array_push_value(list, value, xctx);
    }

    return afw_value_create_unmanaged_array(list, parser->p, xctx);
}


/*
 * A mapping key: the current event is its node. A key is its text,
 * whatever its plain form looks like ("1: a", "true: b", "null: c" have
 * keys "1", "true", "null"). An alias names a scalar key.
 */
static const afw_value_t *
impl_parse_key(impl_parser_t *parser, afw_xctx_t *xctx)
{
    const afw_utf8_t *text;
    const afw_utf8_t *anchor_name;
    const impl_anchor_t *alias;
    impl_anchor_t *anchor;

    text = NULL;
    if (parser->event.type == YAML_SCALAR_EVENT) {
        anchor_name = impl_anchor_name(parser, xctx);
        text = afw_utf8_create(
            (const afw_utf8_octet_t *)parser->event.data.scalar.value,
            parser->event.data.scalar.length, parser->p, xctx);
        impl_nodes_add(parser, 1, xctx);
        if (anchor_name) {
            anchor = afw_pool_calloc_type(xctx->p, impl_anchor_t, xctx);
            anchor->value = afw_value_create_unmanaged_string(
                text, parser->p, xctx);
            anchor->nodes = 1;
            anchor->text = text;
            afw_hash_table_set(parser->anchors, anchor_name->s,
                anchor_name->len, anchor, xctx);
        }
    }
    else if (parser->event.type == YAML_ALIAS_EVENT) {
        alias = impl_alias(parser, xctx);
        text = alias->text;
        impl_nodes_add(parser, 1, xctx);
    }

    if (!text) {
        AFW_THROW_ERROR_FZ(syntax, xctx,
            "YAML mapping key must be a scalar, near line %d, column %d",
            IMPL_LINE(parser), IMPL_COLUMN(parser));
    }

    return afw_object_require_string_property_name(
        afw_value_create_unmanaged_string(text, parser->p, xctx), xctx);
}


/* Mapping: the current event is its start. Ends on its end event. */
static const afw_value_t *
impl_parse_mapping(impl_parser_t *parser, afw_xctx_t *xctx)
{
    const afw_object_t *object;
    const afw_value_t *key;
    const afw_value_t *v;
    const afw_object_t *saved_embedding_object;
    const afw_value_t *saved_property_name;
    const afw_object_t *_meta_;

    /* Create new memory object.*/
    AFW_OBJECT_CREATE_ENTITY_OR_EMBEDDED(object,
        parser->embedding_object,
        parser->property_name,
        false, parser->cede_p, parser->p, xctx);
    _meta_ = NULL;

    /* Nested objects embed in this one, under their key. */
    saved_embedding_object = parser->embedding_object;
    parser->embedding_object = object;
    saved_property_name = parser->property_name;
    parser->property_name = NULL;

    for (;;) {
        impl_next(parser, xctx);
        if (parser->event.type == YAML_MAPPING_END_EVENT) {
            break;
        }
        key = impl_parse_key(parser, xctx);

        impl_next(parser, xctx);
        parser->property_name = key;
        v = impl_parse_node(parser, xctx);
        parser->property_name = NULL;

        if (afw_value_equal(key, afw_v__meta_, xctx)) {
            if (!afw_value_is_object(v)) {
                AFW_THROW_ERROR_FZ(syntax, xctx,
                    "YAML _meta_ property must be a mapping, near line %d, "
                    "column %d",
                    IMPL_LINE(parser), IMPL_COLUMN(parser));
            }
            _meta_ = ((const afw_value_object_t *)v)->internal;
        }

        /*
         * A new nested object is already set by afw_object_create_embedded.
         * Set it anyway: the value of a later duplicate key, or an alias,
         * is a different object.
         */
        else {
            afw_object_set_property(object, key, v, xctx);
        }
    }

    /* Set meta.  Note: must be called after properties are set. */
    afw_object_meta_set_meta_object(object, _meta_, xctx);

    parser->embedding_object = saved_embedding_object;
    parser->property_name = saved_property_name;

    return afw_value_create_unmanaged_object(object, parser->p, xctx);
}


/*
 * A node: the current event is its first (scalar, alias, sequence or
 * mapping start). Ends on its last event.
 */
static const afw_value_t *
impl_parse_node(impl_parser_t *parser, afw_xctx_t *xctx)
{
    const afw_value_t *value;
    const afw_utf8_t *anchor_name;
    const impl_anchor_t *alias;
    impl_anchor_t *anchor;
    afw_size_t nodes_before;
    const afw_utf8_t *text;

    AFW_XCTX_THROW_IF_TERMINATING(xctx);
    nodes_before = parser->nodes;
    anchor_name = impl_anchor_name(parser, xctx);
    text = NULL;

    switch (parser->event.type) {

    case YAML_SCALAR_EVENT:
        impl_nodes_add(parser, 1, xctx);
        value = impl_parse_scalar(parser, xctx);
        if (anchor_name) {
            text = afw_utf8_create(
                (const afw_utf8_octet_t *)parser->event.data.scalar.value,
                parser->event.data.scalar.length, parser->p, xctx);
        }
        break;

    case YAML_ALIAS_EVENT:
        alias = impl_alias(parser, xctx);
        impl_nodes_add(parser, alias->nodes, xctx);
        return alias->value;

    case YAML_SEQUENCE_START_EVENT:
    case YAML_MAPPING_START_EVENT:
        impl_nodes_add(parser, 1, xctx);
        parser->parse_nesting++;
        if (parser->parse_nesting > AFW_YAML_PARSE_NESTING_MAX) {
            AFW_THROW_ERROR_FZ(syntax, xctx,
                "YAML nesting is too deep, near line %d, column %d",
                IMPL_LINE(parser), IMPL_COLUMN(parser));
        }
        value = (parser->event.type == YAML_SEQUENCE_START_EVENT)
            ? impl_parse_sequence(parser, xctx)
            : impl_parse_mapping(parser, xctx);
        parser->parse_nesting--;
        break;

    default:
        AFW_THROW_ERROR_FZ(syntax, xctx,
            "YAML unexpected event %d, near line %d, column %d",
            (int)parser->event.type,
            IMPL_LINE(parser), IMPL_COLUMN(parser));
    }

    if (anchor_name) {
        anchor = afw_pool_calloc_type(xctx->p, impl_anchor_t, xctx);
        anchor->value = value;
        anchor->nodes = parser->nodes - nodes_before;
        anchor->text = text;
        afw_hash_table_set(parser->anchors, anchor_name->s, anchor_name->len,
            anchor, xctx);
    }

    return value;
}


/*
 * Shared parse: bytes → adaptive value. cede_p controls entity object pool
 * ownership (true for adapter raw_to_object; false for conf / raw_to_value).
 * path is applied as meta ids when the root value is an object entity.
 */
static const afw_value_t *
impl_yaml_to_value(
    const afw_memory_t *yaml,
    const afw_utf8_t *path,
    afw_boolean_t cede_p,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t * volatile value = NULL;
    impl_parser_t parser;
    volatile afw_boolean_t parser_initialized;

    memset(&parser, 0, sizeof(parser));
    parser.p = p;
    parser.cede_p = cede_p;
    parser_initialized = false;

    AFW_TRY {

        if (!yaml_parser_initialize(&parser.parser)) {
            AFW_THROW_ERROR_Z(general,
                "Unable to initialize libyaml parser", xctx);
        }
        parser_initialized = true;

        parser.anchors = afw_hash_table_create(
            afw_void_hash_table_t, xctx->p, xctx);

        yaml_parser_set_input_string(&parser.parser, yaml->ptr, yaml->size);

        /* Stream start, then one document. */
        impl_next(&parser, xctx);
        impl_next(&parser, xctx);
        if (parser.event.type != YAML_DOCUMENT_START_EVENT) {
            AFW_THROW_ERROR_Z(syntax, "YAML has no document", xctx);
        }

        /* The document's node: the top-level afw_value_t. */
        impl_next(&parser, xctx);
        value = impl_parse_node(&parser, xctx);

        /* Document end, then stream end. */
        impl_next(&parser, xctx);
        impl_next(&parser, xctx);
        if (parser.event.type != YAML_STREAM_END_EVENT) {
            AFW_THROW_ERROR_FZ(syntax, xctx,
                "YAML multiple documents are not supported, near line %d, "
                "column %d",
                IMPL_LINE(&parser), IMPL_COLUMN(&parser));
        }

        /*
         * Entity path for object roots (documented on afw_yaml_to_value).
         * Nested / non-object roots leave path unused.
         */
        if (path && value && afw_value_is_object(value)) {
            afw_object_meta_set_ids_using_path(
                ((const afw_value_object_t *)value)->internal,
                path, xctx);
        }
    }
    AFW_FINALLY {
        if (parser.has_event) {
            yaml_event_delete(&parser.event);
        }
        if (parser_initialized) {
            yaml_parser_delete(&parser.parser);
        }
    }
    AFW_ENDTRY;

    return value;
}


/*
 * Create an adaptive value from yaml (conf, request body, journal, …).
 * Does not face-wrap objects/arrays: conf and adapter storage stay plain;
 * script-facing adapter/journal APIs apply issue #17 faces on return.
 */
const afw_value_t * afw_yaml_to_value(
    const afw_memory_t *yaml,
    const afw_utf8_t *path,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return impl_yaml_to_value(yaml, path, false, p, xctx);
}

/*
 * Implementation of method raw_to_object of interface afw_content_type.
 *
 * Used by file (and similar) adapters when contentType is yaml. Mirrors
 * afw_compile_json_to_object: subpool when !cede_p, require object root, set ids.
 */
const afw_object_t * afw_yaml_to_object(
    const afw_memory_t  * yaml,
    const afw_utf8_t * source_location,
    const afw_utf8_t * adapter_id,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    afw_boolean_t      cede_p,
    const afw_pool_t * p,
    afw_xctx_t      * xctx)
{
    const afw_pool_t *use_p;
    const afw_value_t *value;
    const afw_object_t *object;

    if ((adapter_id || object_type_id || object_id) &&
        (!adapter_id || !object_type_id || !object_id))
    {
        AFW_THROW_ERROR_Z(general,
            "If adapter_id, object_type_id, or object_id is not NULL, all must "
            "not be NULL",
            xctx);
    }

    /* If not cede_p, allocate a subpool owned by the created entity. */
    use_p = (cede_p) ? p : afw_pool_tracker_create(p, xctx);

    /*
     * Entity always cedes use_p (caller already ceded p, or we created a
     * subpool that the object must own). Meta ids applied after parse.
     * source_location is accepted for interface parity with JSON.
     */
    value = impl_yaml_to_value(yaml, NULL, true, use_p, xctx);

    if (!value || !afw_value_is_object(value)) {
        AFW_THROW_ERROR_Z(general,
            "YAML root must be a mapping (object) for raw_to_object",
            xctx);
    }

    object = ((const afw_value_object_t *)value)->internal;

    /* Same id policy as afw_compile_json_to_object (source_location is not path). */
    if (adapter_id) {
        afw_object_meta_set_ids(object,
            adapter_id, object_type_id, object_id, xctx);
    }
    else {
        /* Interface parity; reserved for future error context. */
        (void)source_location;
    }

    return object;
}
