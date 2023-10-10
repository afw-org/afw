// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW yaml conversion functions.
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_yaml.c
 * @brief AFW yaml conversion functions.
 *
 */

#include "afw.h"
#include "afw_yaml.h"
#include <yaml.h>


typedef struct afw_yaml_parser_s {
    yaml_parser_t parser;
    apr_hash_t *anchors;
    afw_boolean_t docStarted;
    const afw_object_t *embedding_object;
    const afw_utf8_t *property_name;
    const afw_pool_t *p;
    const afw_utf8_t *path;
    afw_boolean_t cede_p;
} afw_yaml_parser_t;

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

/** @fixme
    2.  Debugging/Tracing?
 */
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

const afw_value_t * afw_yaml_parse_scalar(
    afw_yaml_parser_t *parser, yaml_token_t *token, afw_xctx_t *xctx)
{
    const afw_value_t *value;
    const afw_utf8_t *str;
    double number;

    /* examine the "style" property to help decide how to interpret */
    switch (token->data.scalar.style) {
        case YAML_PLAIN_SCALAR_STYLE:
        case YAML_LITERAL_SCALAR_STYLE:
            /** @fixme figure out if it should be a number.  
                Maybe use the JSON code? For now, just try sscanf()*/
            if (sscanf((const char *)token->data.scalar.value, "%lf", &number) > 0) {
                value = afw_value_create_double_unmanaged(number, parser->p, xctx);
            } else if (strcmp((const char *)token->data.scalar.value, "true") == 0) {
                value = afw_value_true;
            } else if (strcmp((const char *)token->data.scalar.value, "false") == 0) {
                value = afw_value_false;
            } else if (strcmp((const char *)token->data.scalar.value, "null") == 0) {
                value = afw_value_null;
            } else {
                /* else must be a string value */
                str = afw_utf8_create_copy((const afw_utf8_octet_t *)token->data.scalar.value,
                    strlen((const afw_utf8_octet_t *)token->data.scalar.value), parser->p,
                    xctx);

                value = afw_value_create_string_unmanaged(str, parser->p, xctx);
            }
            break;

        /* treat all of these as string data types */
        case YAML_ANY_SCALAR_STYLE:
        case YAML_FOLDED_SCALAR_STYLE:
        case YAML_SINGLE_QUOTED_SCALAR_STYLE:
        case YAML_DOUBLE_QUOTED_SCALAR_STYLE:
        default:
            str = afw_utf8_create_copy(
                (const afw_utf8_octet_t *)token->data.scalar.value,
                strlen((const afw_utf8_octet_t *)token->data.scalar.value),
                parser->p, xctx);

            value = afw_value_create_string_unmanaged(str, parser->p, xctx);
            break;
    }

    return value;
}


const afw_array_t * afw_yaml_parse_list(
    afw_yaml_parser_t *parser, afw_xctx_t *xctx)
{
    const afw_array_t *list;
    const afw_value_t *value;

    list = afw_array_create_generic(parser->p, xctx);

    do {
        value = afw_yaml_parse_value(parser, xctx);
        if (value) {
            afw_array_add_value(list, value, xctx);
        }
    } while (value);

    /* Return. */
    return list;
}

const afw_object_t * afw_yaml_parse_object(
    afw_yaml_parser_t *parser, afw_xctx_t *xctx)
{
    yaml_token_t *token;
    const afw_object_t *object;
    const afw_utf8_t *key = NULL;
    const afw_value_t *v;
    int done = 0;
    const afw_object_t *saved_embedding_object;
    const afw_utf8_t *saved_property_name;
    const afw_object_t *_meta_;

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
                key = afw_value_as_utf8(v, xctx->p, xctx);
                parser->property_name = key;
            }
        } else if (token->type == YAML_VALUE_TOKEN) {
            v = afw_yaml_parse_value(parser, xctx);
            if (v) {
                /** @fixme, check that key is not NULL, or throw an exception? */

                /* check if it's a meta object */
                if (afw_utf8_equal(key, afw_s__meta_)) {
                    if (!afw_value_is_object(v)) {
                        AFW_THROW_ERROR_Z(general, "_meta_ property must be an object", xctx);
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
            
                value = afw_value_create_object_unmanaged(obj, parser->p, xctx);
                break;

            case YAML_FLOW_ENTRY_TOKEN:
            case YAML_BLOCK_ENTRY_TOKEN:
                continue;

            case YAML_FLOW_SEQUENCE_START_TOKEN:
            case YAML_BLOCK_SEQUENCE_START_TOKEN:
                /* FLOW sequence denotes [] list , while BLOCK sequence denotes - list */
                list = afw_yaml_parse_list(parser, xctx);

                value = afw_value_create_array_unmanaged(list, parser->p, xctx);
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
                /* A tag tells the application about the data, which could be useful in helping
                    us determine the data type */
                break;

            case YAML_VERSION_DIRECTIVE_TOKEN:
                /* This tells us about the YAML version.  FIXME: we may want to restrict features */
                break;

            case YAML_TAG_DIRECTIVE_TOKEN:
                /* This establishes shorthand notation for specifying node tags.  Allows for readability */
                /** @fixme we may decide to store tags in a global hash, so they can be referenced by tag tokens */
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
 * Create an adaptive object from yaml.
 */
const afw_value_t * afw_yaml_to_value(
    const afw_memory_t *yaml,
    const afw_utf8_t *path,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    yaml_token_t *token;
    const afw_value_t *value = NULL;
    afw_yaml_parser_t parser;

    memset(&parser, 0, sizeof(afw_yaml_parser_t));
    parser.embedding_object = NULL;
    parser.property_name = NULL;
    parser.p = p;
    parser.path = path;
    parser.cede_p = false;

    /* Initialize parser */
    if (!yaml_parser_initialize(&parser.parser)) {
        AFW_THROW_ERROR_Z(general, 
            "Unable to initialize libyaml parser", xctx);
    }

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

    return value;
}

/*
 * Implementation of method raw_to_object of interface afw_content_type.
 */
const afw_object_t * afw_yaml_to_object(
    const afw_memory_t  * yaml,
    const afw_utf8_t * source_location,
    const afw_utf8_t * adaptor_id,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    afw_boolean_t      cede_p,
    const afw_pool_t * p,
    afw_xctx_t      * xctx)
{
    afw_yaml_parser_t parser;

    memset(&parser, 0, sizeof(afw_yaml_parser_t));
    parser.embedding_object = NULL;
    parser.property_name = NULL;
    parser.p = (cede_p) ? p : afw_pool_create(p, xctx);
    parser.cede_p = afw_object_path_make(
        adaptor_id, object_type_id, object_id, parser.p, xctx); 
   
    /* Parse and return object. */
    return afw_yaml_parse_object(&parser, xctx); 
}
