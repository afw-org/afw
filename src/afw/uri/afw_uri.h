// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Uniform Resource Identifier (URI) helpers
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_URI_H__
#define __AFW_URI_H__

#include "afw_interface.h"

/**
 * @defgroup afw_uri Uniform Resource Identifier (URI)
 * @ingroup afw_c_api_public
 * 
 * Uniform Resource Identifier (URI) support.
 * 
 * @{
 */

/**
 * @file afw_uri.h
 * @brief Header for Uniform Resource Identifier (URI) helpers.
 */

AFW_BEGIN_DECLARES

/** @brief Token type typedef. */
typedef afw_uint32_t afw_uri_octet_type_t;



#define AFW_URI_OCTET_INVALID                   (afw_uri_octet_type_t)0x00000000

#define AFW_URI_OCTET_GEN_DELIM                 (afw_uri_octet_type_t)0x00000001
#define AFW_URI_OCTET_SUB_DELIM                 (afw_uri_octet_type_t)0x00000002
#define AFW_URI_OCTET_UNRESERVED                (afw_uri_octet_type_t)0x00000004
#define AFW_URI_OCTET_PCT_ENCODED               (afw_uri_octet_type_t)0x00000008

#define AFW_URI_OCTET_ALPHA_UPPER               (afw_uri_octet_type_t)0x00000010
#define AFW_URI_OCTET_ALPHA_LOWER               (afw_uri_octet_type_t)0x00000020
#define AFW_URI_OCTET_DIGIT                     (afw_uri_octet_type_t)0x00000040
#define AFW_URI_OCTET_HEXDIGIT                  (afw_uri_octet_type_t)0x00000080

/** @brief %encode except A-Z a-z 0-9 ; , / ? : @ & = + $ - _ . ! ~ * ' ( ) # */
#define AFW_URI_OCTET_ENCODE_URI                (afw_uri_octet_type_t)0x00000100

/** @brief %encode except A-Z a-z 0-9 - _ . ! ~ * ' ( ) */
#define AFW_URI_OCTET_ENCODE_COMPONENT          (afw_uri_octet_type_t)0x00000200

/** @brief %encode except A-Z a-z 0-9 - _ . ! ~ * ' */
#define AFW_URI_OCTET_ENCODE_COMPONENT_VALUE    (afw_uri_octet_type_t)0x00000400

#define AFW_URI_OCTET_AT                        (afw_uri_octet_type_t)0x00010000
#define AFW_URI_OCTET_COLON                     (afw_uri_octet_type_t)0x00020000
#define AFW_URI_OCTET_MINUS                     (afw_uri_octet_type_t)0x00040000
#define AFW_URI_OCTET_PERCENT                   (afw_uri_octet_type_t)0x00080000
#define AFW_URI_OCTET_PERIOD                    (afw_uri_octet_type_t)0x00100000
#define AFW_URI_OCTET_PLUS                      (afw_uri_octet_type_t)0x00200000
#define AFW_URI_OCTET_QUESTION                  (afw_uri_octet_type_t)0x00400000
#define AFW_URI_OCTET_SLASH                     (afw_uri_octet_type_t)0x00800000

#define AFW_URI_OCTET_MASK_IS_BITWISE_NOT       (afw_uri_octet_type_t)0x80000000

/** @brief Upper or lower ascii alpha uri octet type mask. */
#define AFW_URI_OCTET_ALPHA \
    ( AFW_URI_OCTET_ALPHA_UPPER | \
    AFW_URI_OCTET_ALPHA_LOWER )

/** @brief pchar uri octet type mask. */
#define AFW_URI_OCTET_PCHAR \
    ( AFW_URI_OCTET_UNRESERVED | \
    AFW_URI_OCTET_PCT_ENCODED | \
    AFW_URI_OCTET_SUB_DELIM | \
    AFW_URI_OCTET_COLON | \
    AFW_URI_OCTET_AT )

/** @brief Start char of scheme uri octet type mask. */
#define AFW_URI_OCTET_SCHEME_START \
    ( AFW_URI_OCTET_ALPHA )

/** @brief Continue char of scheme uri octet type mask. */
#define AFW_URI_OCTET_SCHEME_CONTINUE \
    ( AFW_URI_OCTET_ALPHA | \
    AFW_URI_OCTET_DIGIT | \
    AFW_URI_OCTET_PLUS | \
    AFW_URI_OCTET_MINUS | \
    AFW_URI_OCTET_PERIOD )

/** @brief Userinfo uri octet type mask. */
#define AFW_URI_OCTET_USERINFO \
    ( AFW_URI_OCTET_UNRESERVED | \
    AFW_URI_OCTET_PCT_ENCODED | \
    AFW_URI_OCTET_SUB_DELIM | \
    AFW_URI_OCTET_COLON )

/** @brief Query uri octet type mask. */
#define AFW_URI_OCTET_QUERY \
    ( AFW_URI_OCTET_PCHAR | \
    AFW_URI_OCTET_SLASH | \
    AFW_URI_OCTET_QUESTION )

/** @brief Fragment uri octet type mask. */
#define AFW_URI_OCTET_FRAGMENT \
    ( AFW_URI_OCTET_PCHAR | \
    AFW_URI_OCTET_SLASH | \
    AFW_URI_OCTET_QUESTION )

/** @brief Reserved uri octet type mask. */
#define AFW_URI_OCTET_RESERVED \
    ( AFW_URI_OCTET_GEN_DELIM | \
    AFW_URI_OCTET_SUB_DELIM )


/**
 * @brief Test uri octet type using mask.
 * @param type to test.
 * @param mask for true types.
 * @return boolean result of test.
 */
#define AFW_URI_OCTET_IS(type, mask) \
((type) & (AFW_URI_OCTET_ ## mask))


/** @brief Token type table indexed by afw_octet_t ( 0 - 255 ). */
AFW_DECLARE_CONST_DATA(afw_uri_octet_type_t)
afw_uri_octet_type[256];


/** @brief Typedef for parsed URI returned from afw_uri_parse(). */
typedef struct afw_uri_parsed_s {

    /* @brief Unparsed uri from call to afw_uri_parse(). */
    const afw_utf8_t *original_uri;

    /* @brief Current path or NULL passed to afw_uri_parse(). */
    const afw_utf8_t *current_path;

    /*
     * @brief Parsed value path
     *
     * If path part of URI is an adaptive value path, this is the parsed
     * path; otherwise, NULL.
     */
    const afw_object_path_parsed_t *path_parsed;

    /*
     * @brief  The form of the URI that AFW considers normalized.
     *
     * This will begin with a single slash if the URI is a local path;
     * otherwise, this is a full URI.
     */
    afw_utf8_t normalized_uri;

    /* @brief  Scheme. Can not be percent encoded. */
    afw_utf8_t scheme;

    /* @brief  Original hier-part */
    afw_utf8_t original_hier_part;

    /* @brief  Original authority */
    afw_utf8_t original_authority;

    /* @brief  Decoded authority */
    const afw_utf8_t *authority;

    /* @brief  Original userinfo */
    afw_utf8_t original_userinfo;

    /* @brief  Decoded userinfo */
    const afw_utf8_t *userinfo;

    /* @brief  Original host */
    afw_utf8_t original_host;

    /* @brief  Decoded host */
    const afw_utf8_t *host;

    /* @brief  Port. Can not be percent encoded. */
    afw_utf8_t port;

    /* @brief  Original path */
    afw_utf8_t original_path;

    /* @brief  Original query */
    afw_utf8_t original_query;

    /* @brief  Original fragment */
    afw_utf8_t original_fragment;

} afw_uri_parsed_t;


/**  @brief URI parser typedef. */
typedef struct afw_uri_parser_s {

    /* @brief Decoded string representing token. */
    afw_utf8_t token;

    /* @brief Indicates that token is AFW_URI_OCTET_RESERVED. */
    afw_boolean_t is_reserved;

    /* Private members. */
    const afw_pool_t *p;
    const afw_utf8_t *uri;
    const afw_octet_t *c;
    const afw_octet_t *end;
    afw_uri_octet_type_t type;
    afw_boolean_t reuse;
    afw_boolean_t consider_period_a_token;
} afw_uri_parser_t;


/**
 * @brief Initialize a parser in specific pool.
 * @param parser is preallocated afw_uri_parser_t struct to initialize.
 * @param uri to parse.
 * @param p to use for anything allocated while parsing.
 * @param xctx of caller.
 *
 * The URI parser can be used to parse a full URI or a URI component or
 * subcomponent.  In the case of a full URI you may want to consider using
 * afw_uri_parse() instead.
 */
AFW_DECLARE(void)
afw_uri_parser_initialize(afw_uri_parser_t *parser,
    const afw_utf8_t *uri,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Create and initialize a parser in specific pool.
 * @param uri to parse.
 * @param p to use for anything allocated while parsing.
 * @param xctx of caller.
 * @return initialized parser.
 *
 * The URI parser can be used to parse a full URI or a URI component or
 * subcomponent.  In the case of a full URI you may want to consider using
 * afw_uri_parse() instead.
 */
AFW_DECLARE(const afw_uri_parser_t *)
afw_uri_parser_create(const afw_utf8_t *uri,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Parse next token.
 * @param parser.
 * @param xctx of caller.
 *
 * See parser->token and parser->token_is_delimiter for results.  If there are
 * no more tokens, parser->token.s will be NULL.
 */
AFW_DECLARE(void)
afw_uri_parse_next_token(const afw_uri_parser_t *parser,
    afw_xctx_t *xctx);



/**
 * @brief Cause next call afw_uri_parse_get_next_token() to return current
 *    token.
 * @param parser.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_uri_parse_reuse_token(const afw_uri_parser_t *parser,
    afw_xctx_t *xctx);


/**
 * @brief Set consider_period_a_token flag.
 * @param parser.
 * @param consider_period_a_token flag.
 * @param xctx of caller.
 *
 * Use this function to tell parser to consider '.' as a token, even
 * though it is normally not.  This is useful when parsing dotted property
 * names.
 */
AFW_DECLARE(void)
afw_uri_parse_set_consider_period_a_token(
    const afw_uri_parser_t *parser,
    afw_boolean_t consider_period_a_token,
    afw_xctx_t *xctx);


/**
 * @brief The length needed to URI encode a string.
 * @param string to encode.
 * @param mask identifying octets to remain asis.  See afw_uri_octet_test().
 * @param xctx of caller.
 * @return len needed to encode string.
 */
AFW_DECLARE(afw_size_t)
afw_uri_encode_len(
    const afw_utf8_t *string,
    afw_uri_octet_type_t mask,
    afw_xctx_t *xctx);


/**
 * @brief The length needed to URI encode raw.
 * @param raw to encode. 
 * @param mask identifying octets to remain asis.  See afw_uri_octet_test().
 * @param xctx of caller.
 * @return len needed to encode raw.
 *
 * The string should be a component or subcomponent since all reserved
 * and invalid characters will be percent encoded.
 */
AFW_DECLARE(afw_size_t)
afw_uri_encode_raw_len(
    const afw_memory_t *raw,
    afw_uri_octet_type_t mask,
    afw_xctx_t *xctx);


/**
 * @brief URI encode a string.
 * @param string to encode.
 * @param mask identifying octets to remain asis.  See afw_uri_octet_test().
 * @param p to use for result.
 * @param xctx of caller.
 * @return encoded string.
 *
 * If string does not contain octets that need to be encoded, the original
 * string parameter will be returned as result.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_uri_encode(
    const afw_utf8_t *string,
    afw_uri_octet_type_t mask,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Create a URI encode a string.
 * @param s points to first octet.
 * @param len is length in octets of string.
 * @param mask identifying octets to remain asis.  See afw_uri_octet_test().
 * @param p to use for result.
 * @param xctx of caller.
 * @return encoded string.
 *
 * If string does not contain octets that need to be encoded, the original
 * string parameter will be returned as result.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_uri_encode_create(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    afw_uri_octet_type_t mask,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief URI encode raw.
 * @param raw to encode.
 * @param mask identifying octets to remain asis.  See afw_uri_octet_test().
 * @param p to use for result.
 * @param xctx of caller.
 * @return encoded string.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_uri_encode_raw(
    const afw_memory_t *raw,
    afw_uri_octet_type_t mask,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief URI encode a string using supplied afw_utf8_t.
 * @param s is preallocated place for result.
 * @param s_len is size of preallocated place.
 * @param string to encode.
 * @param mask identifying octets to remain asis.  See afw_uri_octet_test().
 * @param p to use for result.
 * @param xctx of caller.
 * @return length of encoded string or -1 if error.
 *
 * This form is useful if you do not want a afw_utf8_t stuct to be allocated.
 */
AFW_DECLARE(afw_size_t)
afw_uri_encode_to_preallocated(
    afw_utf8_octet_t *s,
    afw_size_t s_len,
    const afw_utf8_t *string,
    afw_uri_octet_type_t mask,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief URI encode from raw using supplied afw_utf8_t.
 * @param s is preallocated place for result.
 * @param s_len is size of preallocated place.
 * @param raw to encode.
 * @param mask identifying octets to remain asis.  See afw_uri_octet_test().
 * @param p to use for result.
 * @param xctx of caller.
 * @return length of encoded string or -1 if error.
 *
 * This form is useful if you do not want a afw_memory_t stuct to be allocated.
 */
AFW_DECLARE(afw_size_t)
afw_uri_encode_raw_to_preallocated(
    afw_utf8_octet_t *s,
    afw_size_t s_len,
    const afw_memory_t *raw,
    afw_uri_octet_type_t mask,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Decode a URI encoded string.
 * @param encoded string.
 * @param p to use for result.  
 * @param xctx of caller.
 * @return original encoded if no % is string otherwise decoded string.
 *
 * The decoded value must be an Unicode NFC string or an error is thrown.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_uri_decode(
    const afw_utf8_t *encoded,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Create a URI decoded string.
 * @param s points to first octet.
 * @param len is length in octets to decode.
 * @param p to use for result.  
 * @param xctx of caller.
 * @return decoded string.
 *
 * The decoded value must be an Unicode NFC string or an error is thrown.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_uri_decode_create(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Decode URI encoded string to raw.
 * @param s points to first octet.
 * @param len is length in octets to decode.
 * @param p to use for result.  
 * @param xctx of caller.
 * @return original encoded if no % is string otherwise decoded raw.
 *
 * Use this instead of afw_uri_decode() is decoded value might not be valid
 * Unicode NFC.
 */
AFW_DECLARE(const afw_memory_t *)
afw_uri_decode_to_raw_create(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Decode a URI encoded string to raw.
 * @param encoded string.
 * @param p to use for result.  
 * @param xctx of caller.
 * @return original encoded if no % is string otherwise decoded raw.
 *
 * Use this instead of afw_uri_decode() is decoded value might not be valid
 * Unicode NFC.
 */
AFW_DECLARE(const afw_memory_t *)
afw_uri_decode_to_raw(
    const afw_utf8_t *encoded,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Parse a URI in specific pool.
 * @param uri to parse.
 * @param is_value_path indicates path is an adaptive value path.
 * @param current_path to use to resolve relative value paths.
 * @param p to use for result.
 * @param xctx of caller
 * @return Parsed URI or NULL if invalid.
 *
 * If is_value_path is true, afw_object_path_parse() will be called to further
 * parse uri if it begins with a single slash.
 */
AFW_DECLARE(const afw_uri_parsed_t *)
afw_uri_parse(
    const afw_utf8_t *uri,
    afw_boolean_t is_value_path,
    const afw_utf8_t *current_path,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Turn a parsed URI into an object representation.
 * @param parsed URI.
 * @param p to use for result.
 * @param xctx of caller
 * @return _AdaptiveParsedURI_ representation of parsed URI.
 */
AFW_DECLARE(const afw_object_t *)
afw_uri_parsed_to_object(
    const afw_uri_parsed_t * parsed,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Turn a URI into an object representation.
 * @param uri to parse.
 * @param is_value_path indicates path is an adaptive value path.
 * @param current_path to use to resolve relative value paths.
 * @param p to use for result.
 * @param xctx of caller
 * @return _AdaptiveParsedURI_ representation of parsed URI.
 *
 * If is_value_path is true, afw_object_path_parse() will be called to further
 * parse uri if it begins with a single slash.
 */
AFW_DECLARE(const afw_object_t *)
afw_uri_to_object(
    const afw_utf8_t *uri,
    afw_boolean_t is_value_path,
    const afw_utf8_t *current_path,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Test uri octet type mask.
 * @param octet to test
 * @param mask (see details in description)
 * @return boolean result.
 * 
 * See #define AFW_URI_OCTET_* for values.
 * 
 * If (afw_uri_octet_type_t)0x80000000) & mask is not 0, ie high bit on, the
 * mask parameter is the ~ of the bits to test.
 * 
 * afw_uri_octet_test(octet, mask) returns true if any bit in mask are 1.
 *
 * afw_uri_octet_test(octet, ~mask) returns true if all bits in ~mask are 0.
 * 
 * Examples:
 *
 * // Test to see if octet is to be left asis when encoding URI components
 * afw_uri_octet_test(octet, AFW_URI_OCTET_ENCODE_COMPONENT)
 *
 * // Test to see if octet should be % encoded when encoding URI components
 * afw_uri_octet_test(octet, ~AFW_URI_OCTET_ENCODE_COMPONENT)
 *
 */
AFW_DECLARE(afw_boolean_t)
afw_uri_octet_test(
    afw_octet_t octet,
    afw_uri_octet_type_t mask);


/**
 * @brief Determine if two parsed URIs are equivalent.
 * @param parsed1 parsed uri.
 * @param parsed2 parsed uri.
 * @param xctx of caller
 * @return boolean result of equivalent test.
 */
AFW_DECLARE(afw_boolean_t)
afw_uri_parsed_are_equivalent(
    const afw_uri_parsed_t *parsed1,
    const afw_uri_parsed_t *parsed2,
    afw_xctx_t *xctx);


/**
 * @brief Determine if two URIs are equivalent.
 * @param uri1 parsed uri.
 * @param uri2 parsed uri.
 * @param is_value_path indicates URIs are of adaptive values.
 * @param current_path2 used for resolving asterisks in uri2 path or NULL.
 * @param p to use while determining equivalence.
 * @param xctx of caller
 * @return boolean result of equivalent test.
 */
AFW_DECLARE(afw_boolean_t)
afw_uri_are_equivalent(
    const afw_utf8_t *uri1,
    const afw_utf8_t *uri2,
    afw_boolean_t is_value_path,
    const afw_utf8_t *current_path2,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_URI_H__ */
