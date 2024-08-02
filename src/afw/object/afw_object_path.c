// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adaptive Object Path
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_object_path.c
 * @brief Adaptive Framework Adaptive Object Path .
 */

#include "afw_internal.h"

typedef enum {
    impl_state_end,
    impl_state_slash_before_adapter_id,
    impl_state_adapter_id,
    impl_state_after_adapter_id,
    impl_state_object_type_id,
    impl_state_after_object_type_id,
    impl_state_option_name,
    impl_state_after_option_name,
    impl_state_option_value,
    impl_state_after_option_value,
    impl_state_entity_object_id,
    impl_state_entity_object_id_possible_second_asterisk,
    impl_state_property_name,
    impl_state_property_name_slash_or_end,
    impl_state_expect_end
} impl_state;


/* Set normalized and entity path, if applicable. */
static void
impl_set_result_paths(
    afw_object_path_parsed_t *parsed,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_size_t path_len, len;
    const afw_object_path_property_name_entry_t *name;
    afw_utf8_octet_t *s;

    /*
     * If adapter id,  object type id, or object id are missing, just return.
     */
    if (parsed->adapter_id.len == 0 ||
        parsed->object_type_id.len == 0 ||
        parsed->entity_object_id.len == 0)
    {
        return;
    }

    /*
     * Calculate len needed for normalized path.  Note, entity path is a subset
     * and will use the same memory.
     */
    path_len = 3 /* Slashes. */ +
        afw_uri_encode_len(&parsed->adapter_id,
            AFW_URI_OCTET_UNRESERVED, xctx) +
        afw_uri_encode_len(&parsed->object_type_id,
            AFW_URI_OCTET_UNRESERVED, xctx) +
        afw_uri_encode_len(&parsed->entity_object_id,
            AFW_URI_OCTET_UNRESERVED, xctx);
    parsed->entity_path.len = path_len;
    for (name = parsed->first_property_name; name; name = name->next) {
        path_len += name->property_name.len + 1 /* slash */;
    }
    parsed->normalized_path.len = path_len;

    /* Create path. */
    s = afw_pool_malloc(p, path_len, xctx);
    parsed->normalized_path.s = s;
    parsed->entity_path.s = s;

    *s++ = '/';
    path_len--;
    len = afw_uri_encode_to_preallocated(s, path_len, &parsed->adapter_id,
        AFW_URI_OCTET_UNRESERVED, p, xctx);
    path_len -= len;
    s += len;

    *s++ = '/';
    path_len--;
    len = afw_uri_encode_to_preallocated(s, path_len, &parsed->object_type_id,
        AFW_URI_OCTET_UNRESERVED, p, xctx);
    path_len -= len;
    s += len;

    *s++ = '/';
    path_len--;
    parsed->undecoded_object_id.s = s;
    parsed->undecoded_object_id.len = path_len;
    len = afw_uri_encode_to_preallocated(s, path_len, &parsed->entity_object_id,
        AFW_URI_OCTET_UNRESERVED, p, xctx);
    path_len -= len;
    s += len;

    for (name = parsed->first_property_name; name; name = name->next) {
        *s++ = '/';
        path_len--;
        len = afw_uri_encode_to_preallocated(s, path_len, &name->property_name,
            AFW_URI_OCTET_UNRESERVED, p, xctx);
        path_len -= len;
        s += len;
    }
}



/* Parse an object path in specific pool. */
static afw_object_path_parsed_t *
impl_object_path_parse(
    const afw_utf8_t *path,
    const afw_utf8_t *current_path,
    const afw_object_options_t *default_options,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_object_path_parsed_t *parsed;
    afw_object_path_parsed_t *current_parsed;
    afw_uri_parser_t parser;
    impl_state state;
    const afw_utf8_t *token;
    afw_boolean_t is_reserved;
    afw_boolean_t at_end;
    const afw_utf8_t *name;
    const afw_octet_t *c;
    const afw_octet_t *prev_c;
    afw_object_path_property_name_entry_t *prev_name;
    afw_object_path_property_name_entry_t *curr_name;
    afw_object_path_property_name_entry_t *relative_name;
    afw_boolean_t has_asterisk;
    afw_size_t len;

    if (!path || path->len == 0) return NULL;

    parsed = afw_pool_calloc_type(p, afw_object_path_parsed_t, xctx);
    afw_uri_parser_initialize(&parser, path, p, xctx);
    parsed->original_path.s = afw_memory_dup(path->s, path->len, p, xctx);
    parsed->original_path.len = path->len;
    prev_name = NULL;
    relative_name = NULL;
    name = NULL;

    /* 
     * If there is an asterisk in path, current_path is needed for
     * substitution, so parse it if present.
     */
    current_parsed = NULL;
    for (has_asterisk = false,
        c = (const afw_octet_t *) path->s,
        len = path->len;
        len > 0;
        c++, len--)
    {
        if (*c == '*') {
            has_asterisk = true;
            break;
        }
    }
    if (has_asterisk) {
        if (!current_path) {
            parsed->contains_unresolved_substitutions = true;
        }
        else {
            current_parsed = impl_object_path_parse(current_path, NULL, NULL,
                p, xctx);
        }
    }

    /* If starts with '/', set state to slash before adapter id. */
    if (*(parser.c) == '/') {
        state = impl_state_slash_before_adapter_id;
    }

    /*
     * If not '/', use current path to get relative parts and start with
     * entity object id.
     */
    else {
        if (current_parsed) {
            afw_memory_copy(&parsed->adapter_id, &current_parsed->adapter_id);
            afw_memory_copy(&parsed->object_type_id, &current_parsed->object_type_id);
            if (afw_utf8_equal(&parsed->adapter_id, afw_s_a_asterisk) ||
                afw_utf8_equal(&parsed->object_type_id, afw_s_a_asterisk))
            {
                parsed->contains_unresolved_substitutions = true;
            }
        }
        else {
            parsed->contains_unresolved_substitutions = true;
            afw_memory_copy(&parsed->adapter_id, afw_s_a_asterisk);
            afw_memory_copy(&parsed->object_type_id, afw_s_a_asterisk);
        }
        state = impl_state_entity_object_id;
    }

    /* Process tokens until end or error. */
    while (state != impl_state_end) {

        /* Parse next token and set some variables for convenience. */
        prev_c = parser.c;
        afw_uri_parse_next_token(&parser, xctx);
        token = &parser.token;
        is_reserved = parser.is_reserved;
        at_end = !token->s;
    
        /* Process based on state. */
        switch (state) {


        /* Initial state where token is a slash before adapter id. */
        case impl_state_slash_before_adapter_id:
            state = impl_state_adapter_id;
            break;


        /* Token should be an adapter id, '*', or end. */
        case impl_state_adapter_id:

            if (at_end) {
                state = impl_state_end;
                break;
            }

            if (is_reserved) {
                if (!afw_utf8_equal(token, afw_s_a_asterisk)) {
                    goto error;
                }

                if (!current_parsed ||
                    afw_utf8_equal(&current_parsed->adapter_id,
                        afw_s_a_asterisk))
                {
                    parsed->contains_unresolved_substitutions = true;
                    parsed->adapter_id.s = afw_self_s_a_asterisk.s;
                    parsed->adapter_id.len = afw_self_s_a_asterisk.len;
                }
                else {
                    parsed->substituted_adapter_id = true;
                    parsed->substitution_occurred = true;
                    afw_memory_copy(&parsed->adapter_id,
                        &current_parsed->adapter_id);
                }
            }

            else {
                parsed->adapter_id.s = token->s;
                parsed->adapter_id.len = token->len;
            }

            state = impl_state_after_adapter_id;
            break;


        /* After adapter id is either '/' or end. */
        case impl_state_after_adapter_id:

            if (at_end) {
                state = impl_state_end;
                break;
            }

            if (!is_reserved || !afw_utf8_equal(token, afw_s_a_slash)) {
                goto error;
            }

            state = impl_state_object_type_id;
            break;


        /* Object type id, '*', or end. */
        case impl_state_object_type_id:

            if (at_end) {
                state = impl_state_end;
                break;
            }

            if (is_reserved) {
                if (!afw_utf8_equal(token, afw_s_a_asterisk)) {
                    goto error;
                }
                if (!current_parsed ||
                    afw_utf8_equal(&current_parsed->object_type_id,
                        afw_s_a_asterisk))
                {
                    parsed->contains_unresolved_substitutions = true;
                    parsed->object_type_id.s = afw_self_s_a_asterisk.s;
                    parsed->object_type_id.len = afw_self_s_a_asterisk.len;
                }
                else {
                    parsed->substituted_object_type_id = true;
                    parsed->substitution_occurred = true;
                    afw_memory_copy(&parsed->object_type_id,
                        &current_parsed->object_type_id);
                }
            }

            else {
                parsed->object_type_id.s = token->s;
                parsed->object_type_id.len = token->len;
            }

            state = impl_state_after_object_type_id;
            break;


        /* After object type id can be '/', ';' followed by parms, or end. */
        case impl_state_after_object_type_id:
          
            if (at_end) {
                state = impl_state_end;
                break;
            }

            if (!is_reserved) {
                goto error;
            }

            if (afw_utf8_equal(token, afw_s_a_slash)) {
                state = impl_state_entity_object_id;
                break;
            }

            if (afw_utf8_equal(token, afw_s_a_semicolon)) {
                state = impl_state_option_name;
                parsed->options_object =
                    afw_object_create_unmanaged(p, xctx);
                break;
            }

            goto error;


        /* Option name. */
        case impl_state_option_name:

            if (at_end || is_reserved) {
                goto error;
            }

            name = afw_utf8_create(token->s, token->len, p, xctx);
            state = impl_state_after_option_name;
            break;


        /* After option name can be '=', '&', '/', or end. */
        case impl_state_after_option_name:

            if (at_end ||
                (is_reserved && !afw_utf8_equal(token, afw_s_a_equal)))
            {
                afw_object_set_property(parsed->options_object,
                    name, afw_boolean_v_true, xctx);
            }

            if (at_end) {
                state = impl_state_end;
                break;
            }

            if (is_reserved) {

                if (afw_utf8_equal(token, afw_s_a_equal)) {
                    state = impl_state_option_value;
                    break;
                }

                if (afw_utf8_equal(token, afw_s_a_ampersand)) {
                    state = impl_state_option_name;
                    break;
                }

                if (afw_utf8_equal(token, afw_s_a_slash)) {
                    state = impl_state_entity_object_id;
                    break;
                }
            }

            goto error;


        /* Value for name '=' pair. */
        case impl_state_option_value:

            if (at_end || is_reserved) {
                goto error;
            }

            afw_object_set_property_as_string(parsed->options_object,
                name, token, xctx);

            state = impl_state_after_option_value;

            break;


        /* After value in name '=' value pair can be '&', '/', or end. */
        case impl_state_after_option_value:

            if (at_end) {
                state = impl_state_end;
                break;
            }

            if (afw_utf8_equal(token, afw_s_a_ampersand)) {
                state = impl_state_option_name;
                break;
            }

            if (afw_utf8_equal(token, afw_s_a_slash)) {
                state = impl_state_entity_object_id;
                break;
            }

            goto error;


        /* Entity object id can be '*' or entity object id. */
        case impl_state_entity_object_id:

            if (at_end) {
                state = impl_state_end;
                break;
            }

            if (is_reserved) {
                if (!afw_utf8_equal(token, afw_s_a_asterisk)) {
                    goto error;
                }
                if (!current_parsed ||
                    afw_utf8_equal(&current_parsed->entity_object_id,
                        afw_s_a_asterisk))
                {
                    parsed->contains_unresolved_substitutions = true;
                    parsed->entity_object_id.s = afw_self_s_a_asterisk.s;
                    parsed->entity_object_id.len = afw_self_s_a_asterisk.len;
                }
                else {
                    parsed->substituted_entity_object_id = true;
                    parsed->substitution_occurred = true;
                    afw_memory_copy(&parsed->entity_object_id,
                        &current_parsed->entity_object_id);
                }
                state =
                    impl_state_entity_object_id_possible_second_asterisk;
                break;
            }

            parsed->entity_object_id.s = token->s;
            parsed->entity_object_id.len = token->len;
            state = impl_state_property_name_slash_or_end;
            break;


        /*
         * If entity object id starts with '*', another '*' means use all
         * property names from object path.
         */
        case impl_state_entity_object_id_possible_second_asterisk:

            if (at_end) {
                state = impl_state_end;
                break;
            }

            if (is_reserved && !afw_utf8_equal(token, afw_s_a_slash))
            {
                if (!afw_utf8_equal(token, afw_s_a_asterisk))
                {
                    goto error;
                }
                parsed->substituted_entire_object_id = true;
                if (!current_parsed ||
                    afw_utf8_equal(&current_parsed->entity_object_id,
                            afw_s_a_asterisk))
                {
                    parsed->contains_unresolved_substitutions = true;
                    parsed->entity_object_id.s = afw_self_s_a_asterisk.s;
                    parsed->entity_object_id.len = afw_self_s_a_asterisk.len;
                }
                else {
                    parsed->first_property_name = current_parsed->first_property_name;
                    for (curr_name = (afw_object_path_property_name_entry_t *)
                            parsed->first_property_name;
                        !parsed->contains_unresolved_substitutions && curr_name;
                        curr_name = (afw_object_path_property_name_entry_t *)
                            curr_name->next)
                    {
                        if (afw_utf8_starts_with(&curr_name->property_name,
                            afw_s_a_asterisk))
                        {
                            parsed->contains_unresolved_substitutions = true;
                        }
                    }
                }
                state = impl_state_expect_end;
                break;
            }

            if (!afw_utf8_equal(token, afw_s_a_slash)) {
                goto error;
            }

            state = impl_state_property_name;
            break;


        /* Property name. */
        case impl_state_property_name:

            if (at_end) {
                goto error;
            }

            curr_name = afw_pool_calloc_type(p,
                afw_object_path_property_name_entry_t, xctx);
            if (!prev_name) {
                parsed->first_property_name = curr_name;
                relative_name = (current_parsed)
                    ? (afw_object_path_property_name_entry_t *)
                        current_parsed->first_property_name
                    : NULL;
            }
            else {
                prev_name->next = curr_name;
                if (relative_name) {
                    relative_name = (afw_object_path_property_name_entry_t *)
                        relative_name->next;
                }
            }

            if (is_reserved && !afw_utf8_equal(token, afw_s_a_slash))
            {

                if (!afw_utf8_equal(token, afw_s_a_asterisk))
                {
                    goto error;
                }

                if (!current_parsed ||
                    !current_parsed->first_property_name ||
                    !relative_name ||
                    afw_utf8_equal(&relative_name->property_name,
                        afw_s_a_asterisk)
                    )
                {
                    parsed->contains_unresolved_substitutions = true;
                    afw_memory_copy(&curr_name->property_name,
                        afw_s_a_asterisk);
                }

                else {
                    parsed->substituted_property_name = true;
                    parsed->substitution_occurred = true;
                    afw_memory_copy(&curr_name->property_name,
                        &relative_name->property_name);
                }
            }

            else {
                afw_memory_copy(&curr_name->property_name, token);
            }

            prev_name = curr_name;
            state = impl_state_property_name_slash_or_end;
            break;


        /* After property name can be slash or end. */
        case impl_state_property_name_slash_or_end:

            if (at_end) {
                state = impl_state_end;
                break;
            }

            if (!afw_utf8_equal(token, afw_s_a_slash))
            {
                goto error;
            }

            state = impl_state_property_name;
            break;


        /* Expecting end. */
        case impl_state_expect_end:

            if (!at_end) {
                goto error;
            }

            state = impl_state_end;
            break;


        /* State is not implemented. */
        default:
            AFW_THROW_ERROR_Z(general, "Internal error", xctx);
        }
    }

    /* Set normalized and entity path, if applicable. */
    impl_set_result_paths(parsed, p, xctx);

    /* If options object, set options. */
    if (parsed->options_object) {
        parsed->options = afw_object_options_set_from_object(
            default_options, parsed->options_object, p, xctx);
    }
    else if (default_options) {
        parsed->options = default_options;
    }

    /* Return parsed result. */
    return parsed;

error:
    AFW_THROW_ERROR_FZ(general, xctx,
        "Error parsing object path " AFW_UTF8_FMT_Q " at offset %d",
        AFW_UTF8_FMT_ARG(parser.uri),
        (int)(prev_c - (const afw_octet_t *)parser.uri->s));
}


AFW_DEFINE(const afw_value_t *)
afw_object_path_property_name_list_get_property(
    const afw_object_t *object,
    const afw_object_path_property_name_entry_t *first_property_name,
    afw_xctx_t *xctx)
{
    const afw_object_path_property_name_entry_t *name;
    const afw_value_t *result;

    result = afw_object_get_property(object,
        &first_property_name->property_name, xctx);

    for (name = first_property_name->next; name; name = name->next) {
        result = afw_object_get_property(object,
            &name->property_name, xctx);
        if (!result) {
            break;
        }
    }

    return result;
}


/* Parse an object path in specific pool. */
AFW_DEFINE(const afw_object_path_parsed_t *)
afw_object_path_parse(
    const afw_utf8_t *path,
    const afw_utf8_t *current_path,
    const afw_object_options_t *default_options,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return impl_object_path_parse(path, current_path, default_options,
        p, xctx);
}


/* Parse simple path into ids. */
AFW_DEFINE(void)
afw_object_path_parse_simple(
    const afw_utf8_t *path,
    const afw_utf8_t * *adapter_id,
    const afw_utf8_t * *object_type_id,
    const afw_utf8_t * *object_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_octet_t *c;
    const afw_utf8_octet_t *s;
    afw_size_t len;

    *adapter_id = NULL;
    *object_type_id = NULL;
    *object_id = NULL;

    len = path->len;
    c = path->s;

    if (len-- <= 0 || *c++ != '/') {
        goto error;
    }

    for (s = c; len > 0 && *c != '/'; len--, c++);
    if (len <= 0 || s == c) {
        goto error;
    }
    *adapter_id = afw_uri_decode_create(s, c - s, p, xctx);
    len--;
    c++;

    for (s = c; len > 0 && *c != '/'; len--, c++);
    if (len <= 0 || s == c) {
        goto error;
    }
    *object_type_id = afw_uri_decode_create(s, c - s, p, xctx);
    c++;
    len--;
    if (len <= 0) {
        goto error;
    }

    *object_id = afw_uri_decode_create(c, len, p, xctx);

    return;

error:
    AFW_THROW_ERROR_FZ(general, xctx,
        "Invalid path " AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(path));
}



/* Turn a parsed path into an object representation. */
AFW_DEFINE(const afw_object_t *)
afw_object_path_parsed_to_object(
    const afw_object_path_parsed_t *parsed,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;
    const afw_array_t *list;
    const afw_value_t *value;
    const afw_object_path_property_name_entry_t *name;

    result = afw_object_create(p, xctx);

    if (parsed->original_path.len > 0) {
        afw_object_set_property_as_string(result,
            afw_s_originalPath, &parsed->original_path, xctx);
    }

    if (parsed->normalized_path.len > 0) {
        afw_object_set_property_as_string(result,
            afw_s_normalizedPath, &parsed->normalized_path, xctx);
    }

    if (parsed->entity_path.len > 0) {
        afw_object_set_property_as_string(result,
            afw_s_entityPath, &parsed->entity_path, xctx);
    }

    if (parsed->adapter_id.len > 0) {
        afw_object_set_property_as_string(result,
            afw_s_adapterId, &parsed->adapter_id, xctx);
    }

    if (parsed->object_type_id.len > 0) {
        afw_object_set_property_as_string(result,
            afw_s_objectType, &parsed->object_type_id, xctx);
    }

    if (parsed->entity_object_id.len > 0) {
        afw_object_set_property_as_string(result,
            afw_s_entityObjectId, &parsed->entity_object_id, xctx);
    }

    if (parsed->undecoded_object_id.len > 0) {
        afw_object_set_property_as_string(result,
            afw_s_objectId, &parsed->undecoded_object_id, xctx);
    }

    if (parsed->first_property_name) {
        list = afw_array_of_create(
            afw_data_type_string, p, xctx);
        for (name = parsed->first_property_name; name; name = name->next) {
            value = afw_value_create_unmanaged_string(
                &name->property_name, p, xctx);
            afw_array_add_value(list, value, xctx);
        }
        afw_object_set_property_as_array(result,
            afw_s_propertyTypes, list, xctx);
    }

    if (parsed->options_object) {
        afw_object_set_property_as_object(result,
            afw_s_optionsObject, parsed->options_object, xctx);
    }

    if (parsed->substitution_occurred) {
        afw_object_set_property(result,
            afw_s_substitutionOccurred, afw_boolean_v_true, xctx);
    }

    if (parsed->substituted_adapter_id) {
        afw_object_set_property(result,
            afw_s_substitutedAdapterId, afw_boolean_v_true, xctx);
    }

    if (parsed->substituted_object_type_id) {
        afw_object_set_property(result,
            afw_s_substitutedObjectTypeId, afw_boolean_v_true, xctx);
    }

    if (parsed->substituted_entire_object_id) {
        afw_object_set_property(result,
            afw_s_substitutedEntireObjectId, afw_boolean_v_true, xctx);
    }

    if (parsed->substituted_entity_object_id) {
        afw_object_set_property(result,
            afw_s_substitutedEntityObjectId, afw_boolean_v_true, xctx);
    }

    if (parsed->substituted_property_name) {
        afw_object_set_property(result,
            afw_s_substitutedPropertyName, afw_boolean_v_true, xctx);
    }

    if (parsed->contains_unresolved_substitutions) {
        afw_object_set_property(result,
            afw_s_containsUnresolvedSubstitutions, afw_boolean_v_true, xctx);
    }

    return result;}


/* Determine if two parsed paths are equivalent. */
AFW_DEFINE(afw_boolean_t)
afw_object_path_parsed_are_equivalent(
    const afw_object_path_parsed_t *parsed1,
    const afw_object_path_parsed_t *parsed2,
    afw_xctx_t *xctx)
{
    const afw_object_path_property_name_entry_t *name1;
    const afw_object_path_property_name_entry_t *name2;

    if (parsed1->adapter_id.len != parsed2->adapter_id.len ||
        (parsed1->adapter_id.len != 0 &&
        !afw_utf8_equal(&parsed1->adapter_id, &parsed2->adapter_id)))
    {
        goto not_equal;
    }

    if (parsed1->object_type_id.len != parsed2->object_type_id.len ||
        (parsed1->object_type_id.len != 0 &&
        !afw_utf8_equal(&parsed1->object_type_id, &parsed2->object_type_id)))
    {
        goto not_equal;
    }

    if (parsed1->entity_object_id.len != parsed2->entity_object_id.len ||
        (parsed1->entity_object_id.len != 0 &&
        !afw_utf8_equal(
            &parsed1->entity_object_id, &parsed2->entity_object_id)))
    {
        goto not_equal;
    }

    for (
        name1 = parsed1->first_property_name,
        name2 = parsed1->first_property_name;
        ;
        name1 = name1->next,
        name2 = name2->next
        )
    {
        if (!name1) {
            if (name2) goto not_equal;
            break;
        }

        if (!name2) goto not_equal;

        if (!afw_utf8_equal(&name1->property_name, &name2->property_name)) {
            goto not_equal;
        }
    }

    return true;

not_equal:
    return false;
}


/* Determine if two paths are equivalent. */
AFW_DEFINE(afw_boolean_t)
afw_object_path_are_equivalent(
    const afw_utf8_t *path1,
    const afw_utf8_t *path2,
    const afw_utf8_t *current_path,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_path_parsed_t *parsed1;
    const afw_object_path_parsed_t *parsed2;

    parsed1 = afw_object_path_parse(path1, current_path, NULL, p, xctx);
    parsed2 = afw_object_path_parse(path2, current_path, NULL, p, xctx);
    return afw_object_path_parsed_are_equivalent(parsed1, parsed2, xctx);
}


/* Construct an object path. */
AFW_DEFINE(const afw_utf8_t *)
afw_object_path_make(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *path;

    /* URL encode each parameter if they are on asterisk. */
    if (!afw_utf8_equal(adapter_id, afw_s_a_asterisk)) {
        adapter_id = afw_uri_encode(adapter_id,
            AFW_URI_OCTET_UNRESERVED, p, xctx);
    }

    if (!afw_utf8_equal(object_type_id, afw_s_a_asterisk)) {
        object_type_id = afw_uri_encode(object_type_id,
            AFW_URI_OCTET_UNRESERVED, p, xctx);
    }

    if (object_id) {
        if (!afw_utf8_equal(object_id, afw_s_a_asterisk)) {
            object_id = afw_uri_encode(object_id,
                AFW_URI_OCTET_UNRESERVED, p, xctx);
        }
        path = afw_utf8_concat(p, xctx,
            afw_s_a_slash, adapter_id,
            afw_s_a_slash, object_type_id,
            afw_s_a_slash, object_id,
            NULL);
    }
    else {
        path = afw_utf8_concat(p, xctx,
            afw_s_a_slash, adapter_id,
            afw_s_a_slash, object_type_id,
            NULL);
    }

    return path;
    
}


/* Construct the path for an embedded object. */
AFW_DEFINE(const afw_utf8_t *)
afw_object_path_make_for_embedded(
    const afw_object_t *embedded_object,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *object;
    afw_size_t len;
    const afw_utf8_t *entity_path;
    const afw_utf8_octet_t *s;
    afw_utf8_octet_t *c;
    const afw_utf8_t *encoded;

    /* Determine length of embedded path. If not entity path, return NULL. */
    for (len = 0, object = embedded_object;
        object->meta.embedding_object;
        object = object->meta.embedding_object)
    {
        len += afw_uri_encode_len(object->meta.id,
            AFW_URI_OCTET_UNRESERVED, xctx) + 1;
    }
    entity_path = afw_object_meta_get_path(object, xctx);
    if (!entity_path) {
        return NULL;
    }
    len += entity_path->len;

    /* Construct embedded path. */
    s = c = afw_pool_malloc(p, len, xctx);
    memcpy(c, entity_path->s, entity_path->len);
    c += len;
    for (object = embedded_object;
        object->meta.embedding_object;
        object = object->meta.embedding_object)
    {
        encoded = afw_uri_encode(object->meta.id,
            AFW_URI_OCTET_UNRESERVED, p, xctx);
        c -= encoded->len;
        memcpy(c, encoded->s, encoded->len);
        c--;
        *c = '/';
    }

    /* Return embedded path. */
    return afw_utf8_create(s, len, p, xctx);
}



/* Construct a property name expression from property name list. */
AFW_DEFINE(const afw_utf8_t *)
afw_object_path_make_property_name_expression(
    const afw_object_path_property_name_entry_t *first,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_utf8_octet_t *s;
    afw_size_t len;
    const afw_object_path_property_name_entry_t *entry;
    afw_utf8_t *result;

    /* This shouldn't be able to be called with first NULL, but just in case: */
    if (!first) {
        AFW_THROW_ERROR_Z(general, "first can't be NULL", xctx);
    }

    /** @fixme Improve to generate ['name'] when needed. */
    for (len = 0, entry = first; entry; entry = entry->next) {
        len += entry->property_name.len + 1;
    }
    len--;

    result = afw_pool_malloc_type(p, afw_utf8_t, xctx);
    result->s = s = afw_pool_malloc(p, len, xctx);
    result->len = len;

    memcpy(s, first->property_name.s, first->property_name.len);
    s += first->property_name.len;

    for (entry = first->next; entry; entry = entry->next) {
        *s++ = '/';
        memcpy(s, entry->property_name.s, entry->property_name.len);
        s += entry->property_name.len;
    }

    return result;
}
