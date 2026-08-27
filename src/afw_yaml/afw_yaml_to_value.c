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


typedef struct afw_yaml_parser_s {
    yaml_parser_t parser;
    apr_hash_t *anchors;
    afw_boolean_t docStarted;
    const afw_object_t *embedding_object;
    const afw_value_t *property_name;
    const afw_pool_t *p;
    const afw_utf8_t *path;
    afw_boolean_t cede_p;
    afw_size_t parse_nesting;
} afw_yaml_parser_t;


/*
 * Max nested YAML mappings/sequences. Stops C-stack overflow on
 * pathological input. Generous for real documents.
 */
#define AFW_YAML_PARSE_NESTING_MAX 256

#define impl_yaml_parse_nesting_enter(parser, xctx) \
do { \
    AFW_XCTX_THROW_IF_TERMINATING(xctx); \
    (parser)->parse_nesting++; \
    if ((parser)->parse_nesting > AFW_YAML_PARSE_NESTING_MAX) { \
        AFW_THROW_ERROR_Z(syntax, \
            "YAML nesting is too deep", xctx); \
    } \
} while (0)

#define impl_yaml_parse_nesting_leave(parser) \
do { \
    if ((parser)->parse_nesting > 0) { \
        (parser)->parse_nesting--; \
    } \
} while (0)

const afw_value_t * afw_yaml_parse_value(
    afw_yaml_parser_t *parser, afw_xctx_t *xctx);

const afw_object_t * afw_yaml_parse_object(
    afw_yaml_parser_t *parser, afw_xctx_t *xctx);

const afw_array_t * afw_yaml_parse_list(
    afw_yaml_parser_t *parser, afw_xctx_t *xctx);

const afw_value_t * afw_yaml_parse_scalar(
    afw_yaml_parser_t *parser, yaml_token_t *token, afw_xctx_t *xctx);

yaml_token_t * afw_yaml_parser_scan(
    afw_yaml_parser_t *parser, afw_xctx_t *xctx);


void afw_yaml_token_cleanup(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx)
{
    yaml_token_t *token = (yaml_token_t *)data;

    yaml_token_delete(token);
}

yaml_token_t * afw_yaml_parser_scan(
    afw_yaml_parser_t *parser, afw_xctx_t *xctx)
{
    int rc;
    yaml_token_t *token;

    token = afw_pool_calloc_type(xctx->p, yaml_token_t, xctx);

    rc = yaml_parser_scan(&parser->parser, token);
    if (rc == 0) {
        AFW_THROW_ERROR_RV_FZ(general, yaml_parser, rc, xctx,
            "Error:  %s, near line %d, column %d",
            parser->parser.problem, parser->parser.problem_mark.line,
            parser->parser.problem_mark.column);
    }

    afw_pool_register_cleanup_before(xctx->p, token, NULL,
        afw_yaml_token_cleanup, xctx);

    return token;
}

/*
 * Plain scalars: YAML 1.2 Core-ish — true/false/null/~ then full-string
 * integer/double via afw_number_parse; else string. Quoted / literal /
 * folded styles are always strings (never type-sniffed).
 */
const afw_value_t * afw_yaml_parse_scalar(
    afw_yaml_parser_t *parser, yaml_token_t *token, afw_xctx_t *xctx)
{
    const afw_value_t *value;
    const afw_utf8_t *str;
    const afw_utf8_octet_t *s;
    afw_size_t len;
    afw_size_t nlen;
    afw_integer_t i;
    afw_double_t d;
    afw_boolean_t is_double;

    s = (const afw_utf8_octet_t *)token->data.scalar.value;
    len = token->data.scalar.length;

    switch (token->data.scalar.style) {
        case YAML_PLAIN_SCALAR_STYLE:
            /* Boolean / null (lowercase Core Schema + common ~). */
            if (len == 4 && memcmp(s, "true", 4) == 0) {
                value = afw_boolean_v_true;
            }
            else if (len == 5 && memcmp(s, "false", 5) == 0) {
                value = afw_boolean_v_false;
            }
            else if ((len == 4 && memcmp(s, "null", 4) == 0) ||
                (len == 1 && s[0] == '~'))
            {
                value = afw_value_null;
            }
            else {
                /*
                 * Full-string number only (rejects "123foo"). Prefer integer
                 * when the token has no fractional/exponent form.
                 */
                nlen = afw_number_parse(s, len, &i, &d, &is_double,
                    parser->p, xctx);
                if (nlen == len) {
                    if (is_double) {
                        value = afw_value_create_unmanaged_double(
                            d, parser->p, xctx);
                    }
                    else {
                        value = afw_value_create_unmanaged_integer(
                            i, parser->p, xctx);
                    }
                }
                else {
                    str = afw_utf8_create(s, len, parser->p, xctx);
                    value = afw_value_create_unmanaged_string(
                        str, parser->p, xctx);
                }
            }
            break;

        /* Quoted, literal (|), folded (>), and unknown: always string. */
        case YAML_LITERAL_SCALAR_STYLE:
        case YAML_FOLDED_SCALAR_STYLE:
        case YAML_SINGLE_QUOTED_SCALAR_STYLE:
        case YAML_DOUBLE_QUOTED_SCALAR_STYLE:
        case YAML_ANY_SCALAR_STYLE:
        default:
            str = afw_utf8_create(s, len, parser->p, xctx);
            value = afw_value_create_unmanaged_string(str, parser->p, xctx);
            break;
    }

    return value;
}


const afw_array_t * afw_yaml_parse_list(
    afw_yaml_parser_t *parser, afw_xctx_t *xctx)
{
    const afw_array_t *list;
    const afw_value_t *value;

    impl_yaml_parse_nesting_enter(parser, xctx);

    list = afw_array_create_in_pool(parser->p, xctx);

    do {
        value = afw_yaml_parse_value(parser, xctx);
        if (value) {
            afw_array_push_value(list, value, xctx);
        }
    } while (value);

    impl_yaml_parse_nesting_leave(parser);

    /* Return. */
    return list;
}

const afw_object_t * afw_yaml_parse_object(
    afw_yaml_parser_t *parser, afw_xctx_t *xctx)
{
    yaml_token_t *token;
    const afw_object_t *object;
    const afw_value_t *key = NULL;
    const afw_value_t *v;
    int done = 0;
    const afw_object_t *saved_embedding_object;
    const afw_value_t *saved_property_name;
    const afw_object_t *_meta_;

    impl_yaml_parse_nesting_enter(parser, xctx);

    /* Create new memory object.*/
    AFW_OBJECT_CREATE_ENTITY_OR_EMBEDDED(object,
        parser->embedding_object,
        parser->property_name,
        false, parser->cede_p, parser->p, xctx);
    _meta_ = NULL;

    /*
     * Save parser->embedding_object and set to new object.
     *
     * Save parser->property_name and set to NULL.
     *
     * If entity, default path is supplied by caller, otherwise it's NULL.
     */
    saved_embedding_object = parser->embedding_object;
    parser->embedding_object = object;
    saved_property_name = parser->property_name;
    parser->property_name = NULL;

    while (!done) {
        token = afw_yaml_parser_scan(parser, xctx);

        if (token->type == YAML_FLOW_ENTRY_TOKEN)
            continue;

        if (token->type == YAML_KEY_TOKEN) {
            v = afw_yaml_parse_value(parser, xctx);
            if (v && afw_value_is_string(v)) {
                key = afw_object_require_string_property_name(v, xctx);
                parser->property_name = key;
            }
        } else if (token->type == YAML_VALUE_TOKEN) {
            v = afw_yaml_parse_value(parser, xctx);
            if (v) {
                if (!key) {
                    AFW_THROW_ERROR_RV_FZ(general, yaml_token_type,
                        token->type, xctx,
                        "YAML mapping value without a key, near line %d, "
                        "column %d",
                        parser->parser.mark.line,
                        parser->parser.mark.column);
                }

                /* check if it's a meta object */
                if (afw_value_equal(key, afw_v__meta_, xctx)) {
                    if (!afw_value_is_object(v)) {
                        AFW_THROW_ERROR_Z(general,
                            "_meta_ property must be an object", xctx);
                    }
                    _meta_ = ((const afw_value_object_t*)v)->internal;
                }

                /*
                 * If not _meta_ property, set property in new object to this
                 * value if it's not an object.  Object will already have been
                 * added by afw_object_create_embedded().
                 */
                else if (!afw_value_is_object(v)) {
                    afw_object_set_property(object, key, v, xctx);
                }

                /* Next pair needs its own key. */
                key = NULL;
                parser->property_name = NULL;
            }
        } else if (token->type != YAML_BLOCK_END_TOKEN &&
                    token->type != YAML_FLOW_MAPPING_END_TOKEN) {
            AFW_THROW_ERROR_RV_FZ(general, yaml_token_type, token->type, xctx,
                "Unexpected token inside map, starting at line %d, column %d", 
                parser->parser.mark.line, parser->parser.mark.column);
        }

        done = (token->type == YAML_BLOCK_END_TOKEN || 
                token->type == YAML_FLOW_MAPPING_END_TOKEN);
    }

    /* Set meta.  Note: must be called after properties are set. */
    afw_object_meta_set_meta_object(object, _meta_, xctx);

    /* Set parser->embedding_object to previous value and return object. */
    parser->embedding_object = saved_embedding_object;
    parser->property_name = saved_property_name;
    impl_yaml_parse_nesting_leave(parser);
    return object;
}

const afw_value_t * afw_yaml_parse_value(
    afw_yaml_parser_t *parser, afw_xctx_t *xctx)
{
    yaml_token_t *token;
    const afw_value_t *value = NULL;
    const afw_object_t *obj;
    const afw_array_t *list;

    while (value == NULL) {
        token = afw_yaml_parser_scan(parser, xctx);
    
        switch (token->type) {
            case YAML_SCALAR_TOKEN:
                /* return the appropriate scalar value */
                value = afw_yaml_parse_scalar(parser, token, xctx);
                break;
 
            case YAML_BLOCK_MAPPING_START_TOKEN:
            case YAML_FLOW_MAPPING_START_TOKEN:
                /* FLOW mapping denotes {} map, while BLOCK uses spacing */
                obj = afw_yaml_parse_object(parser, xctx);
            
                value = afw_value_create_unmanaged_object(obj, parser->p, xctx);
                break;

            case YAML_FLOW_ENTRY_TOKEN:
            case YAML_BLOCK_ENTRY_TOKEN:
                continue;

            case YAML_FLOW_SEQUENCE_START_TOKEN:
            case YAML_BLOCK_SEQUENCE_START_TOKEN:
                /* FLOW sequence denotes [] list , while BLOCK sequence denotes - list */
                list = afw_yaml_parse_list(parser, xctx);

                value = afw_value_create_unmanaged_array(list, parser->p, xctx);
                break;

            case YAML_BLOCK_END_TOKEN:
            case YAML_FLOW_SEQUENCE_END_TOKEN:
                /* The end of a sequence or block should return NULL to caller to quit */
                return NULL;
                break;

            case YAML_ANCHOR_TOKEN:
                /* an anchor defines a value that can be later referenced */
                value = afw_yaml_parse_value(parser, xctx);
                apr_hash_set(parser->anchors, 
                    apr_pstrdup(afw_pool_get_apr_pool(xctx->p),
                        (const char *)token->data.anchor.value), 
                    APR_HASH_KEY_STRING, value);

                break;

            case YAML_ALIAS_TOKEN:
                /* an alias references an anchor */
                value = apr_hash_get(parser->anchors, token->data.alias.value,
                    APR_HASH_KEY_STRING);
                if (value == NULL) {
                    AFW_THROW_ERROR_RV_FZ(general, yaml_token_type,
                        token->type, xctx,
                        "Alias references undefined anchor, near line %d, column %d", 
                        parser->parser.mark.line, parser->parser.mark.column);
                }
                break;

            case YAML_DOCUMENT_START_TOKEN:
                if (parser->docStarted) {
                    /* Not supported, can't have more than one document */
                    AFW_THROW_ERROR_RV_Z(general, yaml_token_type, token->type,
                        "Multiple documents are not supported.", xctx);
                } else 
                    parser->docStarted = AFW_TRUE;
                break;

            case YAML_TAG_TOKEN:
                /*
                 * Node tags ignored for now (no typed tags beyond plain
                 * Core Schema sniffing on plain scalars).
                 */
                break;

            case YAML_VERSION_DIRECTIVE_TOKEN:
                /* %YAML version directive — accepted, not enforced. */
                break;

            case YAML_TAG_DIRECTIVE_TOKEN:
                /* %TAG shorthand — accepted, not stored. */
                break;

            default:
                AFW_THROW_ERROR_RV_FZ(general, yaml_token_type, token->type, xctx, 
                    "Unexpected token type, starting at line %d, column %d", 
                    parser->parser.mark.line, parser->parser.mark.column);
                break;
        }
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
    yaml_token_t *token;
    const afw_value_t *value = NULL;
    afw_yaml_parser_t parser;
    afw_boolean_t parser_initialized;

    memset(&parser, 0, sizeof(afw_yaml_parser_t));
    parser.embedding_object = NULL;
    parser.property_name = NULL;
    parser.p = p;
    parser.path = path;
    parser.cede_p = cede_p;
    parser_initialized = false;

    AFW_TRY {

        if (!yaml_parser_initialize(&parser.parser)) {
            AFW_THROW_ERROR_Z(general,
                "Unable to initialize libyaml parser", xctx);
        }
        parser_initialized = true;

        parser.anchors = apr_hash_make(afw_pool_get_apr_pool(xctx->p));

        yaml_parser_set_input_string(&parser.parser, yaml->ptr, yaml->size);

        /* We should expect a STREAM_START */
        token = afw_yaml_parser_scan(&parser, xctx);
        if (token->type != YAML_STREAM_START_TOKEN) {
            AFW_THROW_ERROR_RV_Z(general, yaml_token_type, token->type,
                "Expected start of stream token", xctx);
        }

        /* Now parse and return the top-level afw_value_t */
        value = afw_yaml_parse_value(&parser, xctx);

        /* We should expect a STREAM_END */
        token = afw_yaml_parser_scan(&parser, xctx);
        if (token->type != YAML_STREAM_END_TOKEN) {
            AFW_THROW_ERROR_RV_Z(general, yaml_token_type, token->type,
                "Expected end of stream token", xctx);
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
 * afw_compile_to_object: subpool when !cede_p, require object root, set ids.
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
    use_p = (cede_p) ? p : afw_pool_create(p, xctx);

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

    /* Same id policy as afw_compile_to_object (source_location is not path). */
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
