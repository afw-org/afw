// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Compiler Lexical Analyzer
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_compile_lexical.c
 * @brief Adaptive Framework Compiler Lexical Analyzer.
 */

#include "afw_internal.h"

typedef void (*impl_lexical_cb_t) (afw_compile_parser_t *parser);

/* Static declarations. */

static afw_octet_t
impl_get_octet(afw_compile_parser_t *parser);

static afw_boolean_t
impl_consume_matching_octets_z(afw_compile_parser_t *parser,
    const afw_utf8_z_t *s);

static int
impl_get_required_HexDigit(afw_compile_parser_t *parser);

static int
impl_get_BinaryDigit(afw_compile_parser_t *parser);

static int
impl_get_HexDigit(afw_compile_parser_t *parser);

static int
impl_get_OctalDigit(afw_compile_parser_t *parser);

static const afw_utf8_t *
impl_get_identifier(afw_compile_parser_t *parser);

static void
impl_parse_String(afw_compile_parser_t *parser);

static afw_boolean_t
impl_parse_number(afw_compile_parser_t *parser);

static void
impl_parse_identifier(afw_compile_parser_t *parser);

static void
impl_parse_u(afw_compile_parser_t *parser);



/* Static functions. */

/* Get a utf-8 octet.  Set cursor_eof to cursor + 1 if at end. */
static afw_octet_t
impl_get_octet(afw_compile_parser_t *parser)
{
    afw_utf8_octet_t result;
    const afw_utf8_octet_t *s;
    afw_size_t len;
    int rv;

    /* If eof already, this is an error. */
    if (parser->last_octet_eof) {
        AFW_COMPILE_THROW_ERROR_Z("impl_get_octet() called after eof");
    }

    if (parser->all_eof && (parser->cursor == parser->cursor_eof)) {
        parser->last_octet_eof = true;
        return 0;
    }

    /* If first call, set up source. */
    if (!parser->full_source) {
        parser->full_source = afw_pool_calloc_type(
            parser->p, afw_utf8_t, parser->xctx);

        /* If passed source, set source point to it. */
        if (parser->passed_source) {
            parser->full_source->s = parser->passed_source->s;
            parser->full_source->len = parser->passed_source->len;
        }

        /*
         * If callback specified, make an array to hold source.  If passed
         * source is also specified, add it to array.
         *
         * The s and len in parser->source will always be updated to contain
         * source_buffer->elts and parser->source_buffer->nelts respectively.
         */
        if (parser->callback) {
            /** @fixme
             * For now this can be no larger than can fit in int because of
             * apr_array*.
             */
            if (parser->estimated_size != (int)parser->estimated_size) {
                AFW_THROW_ERROR_Z(general, "Limitation", parser->xctx);
            }
            parser->source_buffer = apr_array_make(parser->apr_p,
                (int)parser->estimated_size, sizeof(afw_utf8_octet_t));
            if (parser->passed_source) {
                for (s = parser->passed_source->s,
                    len = parser->passed_source->len;
                    len > 0;
                    s++, len--)
                {
                    APR_ARRAY_PUSH(parser->source_buffer, afw_utf8_octet_t) =
                        *s;
                }
            }
            parser->full_source->s =
                (const afw_utf8_octet_t *)parser->source_buffer->elts;
            parser->full_source->len = parser->source_buffer->nelts;
        }
    }

    /* If cursor is not past source length, return octet and update cursor. */
    if (parser->cursor < parser->full_source->len) {
        result = parser->full_source->s[parser->cursor];
        (parser->cursor)++;
    }

    /* If source exhausted, eof. */
    else if (!parser->callback || parser->callback_eof) {
        parser->all_eof = true;
        parser->last_octet_eof = true;
        parser->cursor_eof = parser->cursor;
        result = 0;
    }

    /* If callback has never been called and scanning for residual, eof. */
    else if (!parser->has_called_callback && parser->scanning_for_residual) {
        parser->callback_eof = true;
        parser->all_eof = true;
        parser->last_octet_eof = true;
        parser->cursor_eof = parser->cursor;
        result = 0;
    }

    /* If there is a callback, call it to get next octet.  If eof, indicate. */
    else {
        parser->has_called_callback = true;

        rv = parser->callback(&result, parser->callback_data,
            parser->xctx);

        if (rv < 0)
        {
            parser->callback_eof = true;
            parser->all_eof = true;
            parser->last_octet_eof = true;
            parser->cursor_eof = parser->cursor;
            result = 0;
        }

        else {
            APR_ARRAY_PUSH(parser->source_buffer, afw_utf8_octet_t) = result;
            parser->full_source->s =
                (const afw_utf8_octet_t *)parser->source_buffer->elts;
            parser->full_source->len = parser->source_buffer->nelts;
            (parser->cursor)++;
        }
    }

    /* Return result. */
    return result;
}


/*
 * Consume matching octets.  If not a match, cursor will be reset to entry
 * value and false will be returned.
 */
static afw_boolean_t
impl_consume_matching_octets_z(afw_compile_parser_t *parser,
    const afw_utf8_z_t *s)
{
    afw_boolean_t result;
    afw_size_t save;

    result = true;
    afw_compile_save_cursor(save);

    for (; *s; s++) {
        if (impl_get_octet(parser) != *s) {
            afw_compile_restore_cursor(save);
            result = false;
            break;
        }
    }

    return result;
}

/*ebnf>>>
 *
 * HexDigit ::= [0-9a-fA-F]
 *
 *<<<ebnf*/
static int
impl_get_required_HexDigit(afw_compile_parser_t *parser)
{
    afw_utf8_octet_t o;

    o = impl_get_octet(parser);
    if (afw_compile_is_at_eof()) {
        goto error;
    }

    if (o >= '0' && o <= '9') {
        return o - '0';
    }

    if (o >= 'a' && o <= 'f') {
        return (o - 'a') + 10;
    }

    if (o >= 'A' && o <= 'F') {
        return (o - 'A') + 10;
    }

error:
    AFW_COMPILE_THROW_ERROR_Z("Invalid hex digit");
}



static int
impl_get_HexDigit(afw_compile_parser_t *parser)
{
    afw_utf8_octet_t o;

    o = impl_get_octet(parser);
    if (afw_compile_is_at_eof()) {
        return -1;
    }

    if (o >= '0' && o <= '9') {
        return o - '0';
    }

    if (o >= 'a' && o <= 'f') {
        return (o - 'a') + 10;
    }

    if (o >= 'A' && o <= 'F') {
        return (o - 'A') + 10;
    }

    parser->cursor--;
    return -1;
}



/*ebnf>>>
 *
 * BinaryDigit ::= [0-1]
 *
 *<<<ebnf*/
static int
impl_get_BinaryDigit(afw_compile_parser_t *parser)
{
    afw_utf8_octet_t o;

    o = impl_get_octet(parser);

    if (afw_compile_is_at_eof()) {
        return -1;
    }

    if (o >= '0' && o <= '1') {
        return o - '0';
    }

    parser->cursor--;
    return -1;
}



/*ebnf>>>
 *
 * OctalDigit ::= [0-7]
 *
 *<<<ebnf*/
static int
impl_get_OctalDigit(afw_compile_parser_t *parser)
{
    afw_utf8_octet_t o;

    o = impl_get_octet(parser);

    if (afw_compile_is_at_eof()) {
        return -1;
    }

    if (o >= '0' && o <= '7') {
        return o - '0';
    }

    parser->cursor--;
    return -1;
}


/* Push cp on parser->s */
AFW_DEFINE_INTERNAL(void)
afw_compile_internal_s_push_code_point(
    afw_compile_parser_t *parser,
    afw_code_point_t cp)
{
    afw_utf8_octet_t utf8_z[5];
    const afw_utf8_octet_t *c;

    if (cp < 127) {
        APR_ARRAY_PUSH(parser->s, afw_utf8_octet_t) = (afw_octet_t)cp;
        return;
    }

    /* Convert code point to utf-8 and push on utf-8 bytes on parser->s */
    if (!afw_utf8_from_code_point(utf8_z, cp, parser->xctx)) {
        AFW_COMPILE_THROW_ERROR_Z("Invalid codepoint");
    }
    c = &utf8_z[0];
    do {
        APR_ARRAY_PUSH(parser->s, afw_utf8_octet_t) = *c;
    } while (*++c);
}



/*ebnf>>>
 *

 *#
 *
 *# Tab character
 * TAB ::= #x0009
 *
 *# Vertical tab
 * VT ::= #x000B
 *
 *# Forms feed
 * FF ::= #x000C
 *
 *# Line feed
 * LF ::= #x000A
 *
 *# Carriage return
 * CR ::= #x000D
 *
 *# No break space
 * NBSP ::= #x00A0
 *
 *# Line Separator
 * LS ::= #x2028
 *
 *# Paragraph Separator
 * PS ::= #x2029
 *
 *# Space Separator code point
 * USP ::= "Any Unicode Space_Separator code point"
 *
 *# Zero Width No-Break Space (Unicode word join)
 * ZWNBSP ::= #xFEFF
 *
 *# Whitespace
 * Whitespace ::= ( TAB | VT | FF | ZWNBSP | USP )+
 *
 *# Line Terminators
 * EOL ::= LF | CR | LS | PS
 *
 *# Whitespace in source (between any production tokens without ws: explicit)
 * WhitespaceBetweenTokens ::= ( Whitespace | EOL | Comment )+
 *
 *# Comment is not allowed in strict mode.
 * Comment ::= InlineComment | BlockComment
 */
//* InlineComment ::= '/*' ( [^*] | '*'+ [^*/] )* '*'* '*/'
/*
 * BlockComment ::= '//' [^\n]* EOL
 *
 *<<<ebnf*/


/* Skip whitespace. */
AFW_DEFINE_INTERNAL(void)
afw_compile_skip_ws(afw_compile_parser_t *parser)
{
    afw_size_t start_offset;
    afw_size_t start_offset_first_slash;
    afw_code_point_t cp;

    enum {
        state_ws,
        state_finished,
        state_first_slash,
        state_block_comment,
        state_block_comment_asterisk,
        state_inline_comment,
    } state;

    state = state_ws;
    parser->get_token_found_eol = false;
    if (!afw_compile_is_at_eof()) do {

        afw_compile_save_cursor(start_offset);
        cp = afw_compile_get_code_point();

        switch (state) {

        case state_ws:

            if (afw_compile_is_at_eof()) {
                parser->last_octet_eof = false;
                state = state_finished;
            }

            else if (cp == '/') {
                if (!parser->strict) {
                    start_offset_first_slash = start_offset;
                    state = state_first_slash;
                }
                else {
                    afw_compile_restore_cursor(start_offset);
                    state = state_finished;
                }
            }

            else if (!afw_compile_code_point_is_WhitespaceOrEOL(cp))
            {
                afw_compile_restore_cursor(start_offset);
                state = state_finished;
            }

            break;

        case state_first_slash:

            if (afw_compile_is_at_eof()) {
                parser->last_octet_eof = false;
                state = state_finished;
            }

            else if (cp == '/') {
                state = state_inline_comment;
            }

            else if (cp == '*') {
                state = state_block_comment;
            }

            else {
                afw_compile_restore_cursor(start_offset_first_slash);
                state = state_finished;
            }

            break;

        case state_block_comment:

            if (afw_compile_is_at_eof()) {
                AFW_COMPILE_THROW_ERROR_Z("Block comment not terminated");
            }

            if (cp == '*') {
                state = state_block_comment_asterisk;
            }

            break;

        case state_block_comment_asterisk:

            if (afw_compile_is_at_eof()) {
                AFW_COMPILE_THROW_ERROR_Z("Block comment not terminated");
            }

            if (cp == '/') {
                state = state_ws;
            }

            else if (cp == '*') {
                state = state_block_comment_asterisk;
            }

            else {
                state = state_block_comment;
            }

            break;

        case state_inline_comment:

            if (afw_compile_is_at_eof()) {
                parser->last_octet_eof = false;
                state = state_finished;
            }
            else if (afw_compile_code_point_is_EOL(cp)) {
                state = state_ws;
                if (parser->get_token_before_eol) {
                    parser->get_token_found_eol = true;
                    state = state_finished;
                }
            }
            break;

        case state_finished:
            break;
        }

    } while (state != state_finished);
}



/*ebnf>>>*/

//* U ::= '\u' ( ( HexDigit HexDigit HexDigit HexDigit ) |
//*            ( '{' HexDigit+ '}' ) ) /* ws: explicit */

/*<<<ebnf*/
/*
 * Cursor starts after '\u'.
 *
 * Parse a \uxxxx.  If surrogate pair, parse \uxxxx\uxxxx.
 */
static void
impl_parse_u(afw_compile_parser_t *parser)
{
    afw_code_point_t cp, cp2;
    afw_utf8_octet_t utf8_z[5];
    const afw_utf8_octet_t *c;
    afw_utf8_octet_t o;
    int digit;

    /* Code point from /u{x...}. */
    o = impl_get_octet(parser);
    if (afw_compile_is_at_eof()) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting hex digit or '{'");
    }
    if (o == '{') {
        cp = 0;
        digit = impl_get_HexDigit(parser);
        if (digit < 0) {
            AFW_COMPILE_THROW_ERROR_Z("Expecting hex digit");
        }
        for (;;) {
            cp <<= 4;
            cp += digit;
            if (cp > 0x10FFFF) {
                AFW_COMPILE_THROW_ERROR_Z("Codepoint is out of range");               
            }
            digit = impl_get_HexDigit(parser);
            if (afw_compile_is_at_eof()) {
                AFW_COMPILE_THROW_ERROR_Z("Expecting hex digit or '}'");
            }
            if (digit < 0) {
                o = impl_get_octet(parser);
                if (afw_compile_is_at_eof() || o != '}') {
                    AFW_COMPILE_THROW_ERROR_Z("Expecting hex digit or '}'");
                }
                break;
            }
        }
    }

    /* Code point from /uxxxx with optional surrogate /uxxxx. */
    else {
        parser->cursor--;

        cp =
            impl_get_required_HexDigit(parser) * 0x1000 +
            impl_get_required_HexDigit(parser) * 0x100 +
            impl_get_required_HexDigit(parser) * 0x10 +
            impl_get_required_HexDigit(parser);

        /* If code point is a utf-16 surrogate, it must be paired. */
        if (cp >= 0xD800 && cp <= 0xDFFF)
        {
            if (cp >= 0xDC00)
            {
                goto error;
            }
            o = impl_get_octet(parser);
            if (o != '\\')
            {
                goto error;
            }
            o = impl_get_octet(parser);
            if (o != 'u')
            {
                goto error;
            }
            cp2 =
                impl_get_required_HexDigit(parser) * 0x1000 +
                impl_get_required_HexDigit(parser) * 0x100 +
                impl_get_required_HexDigit(parser) * 0x10 +
                impl_get_required_HexDigit(parser);
            if (cp2 < 0xDC00 || cp2 > 0xDFFF)
            {
                goto error;
            }
            cp = ((cp - 0xD800) << 10) + (cp2 - 0xDC00) + 0x10000;
        }
    }

    /* Convert code point to utf-8 and push on utf-8 bytes on parser->s */
    if (!afw_utf8_from_code_point(utf8_z, cp, parser->xctx)) {
        AFW_COMPILE_THROW_ERROR_Z("Invalid codepoint");
    }
    c = &utf8_z[0];
    do {
        APR_ARRAY_PUSH(parser->s, afw_utf8_octet_t) = *c;
    } while (*++c);

    /* Return. */
    return;

error:
    AFW_COMPILE_THROW_ERROR_Z("Invalid surrogate pair");
}



/*ebnf>>>
 *
 * String ::= '"' Char* '"'
 *
 * Char ::= [^\#x0-#x20] | '\"' | "\'" | '\`' | "\\" | '\/' |
 *         '\b' | '\f' | '\n' | '\r' | '\t' | U
 *
 * UnicodeCodePoint ::= #x0-#x10FFFF
 *
 * UnicodeNonControl ::= (UnicodeCodePoint - [^\#x0-#x1f] )
 *
 * UnicodeNonSpaceNonControl ::= (UnicodeCodePoint - [^\#x0-#x20] )
 *
 *<<<ebnf*/
/*
 * Translate a UTF-8 JSON string. Remove containing quotes and convert escaped
 * characters. Input string must begin with either a single or double quote.
 * Whichever one is used, it must be escaped if contained within the string.
 * The binary value of control characters is allowed in the string as well
 * as the escaped form.  \uxxxx and utf16be surrogates are supported.
 */
static void
impl_parse_String(afw_compile_parser_t *parser)
{
    afw_utf8_octet_t quot, o;

    /* Clear array used for building string. */
    apr_array_clear(parser->s);

    /* First octet is quote to use. */
    quot = impl_get_octet(parser);
    parser->token->string_quote_character = quot;

    /* Set token type to utf8_string. */
    parser->token->type = (quot == '`')
        ? afw_compile_token_type_template_string
        : afw_compile_token_type_utf8_string;

    /* Keep going until last quote located or input exhausted. */
    for (;;) {
        o = impl_get_octet(parser);
        if (afw_compile_is_at_eof()) break;
        if (o == quot) break;

        /* If '\', convert escaped code. */
        if (o == '\\') {

            /* If escape, get next octet and break if eof.*/
            o = impl_get_octet(parser);
            if (afw_compile_is_at_eof()) break;

            /* Process based on octet after \. */
            switch (o) {

            case '"':
            case '\'':
            case '`':
            case '\\':
            case '/':
                APR_ARRAY_PUSH(parser->s, afw_utf8_octet_t) = o;
                break;

            case 'b':
                APR_ARRAY_PUSH(parser->s, afw_utf8_octet_t) = AFW_ASCII_BS;
                break;

            case 'f':
                APR_ARRAY_PUSH(parser->s, afw_utf8_octet_t) = AFW_ASCII_FF;
                break;

            case 'n':
                APR_ARRAY_PUSH(parser->s, afw_utf8_octet_t) = AFW_ASCII_LF;
                break;

            case 'r':
                APR_ARRAY_PUSH(parser->s, afw_utf8_octet_t) = AFW_ASCII_CR;
                break;

            case 't':
                APR_ARRAY_PUSH(parser->s, afw_utf8_octet_t) = AFW_ASCII_HT;
                break;

            case 'v':
                APR_ARRAY_PUSH(parser->s, afw_utf8_octet_t) = AFW_ASCII_VT;
                break;

            case 'u':
                impl_parse_u(parser);
                break;

            default:
                AFW_COMPILE_THROW_ERROR_Z("Invalid escape code");
            }
        }

        /* If not '\', just copy character from input to output. */
        else {
            APR_ARRAY_PUSH(parser->s, afw_utf8_octet_t) = o;
        }

    }

    /* If string didn't end with a quote, it's an error. */
    if (afw_compile_is_at_eof() || o != quot) {
        AFW_COMPILE_THROW_ERROR_Z("String missing ending quote");
    }

    /* Create token.string making sure it is NFC utf-8 normalized. */
    parser->token->string = afw_compile_get_string_literal(
        parser,
        (const afw_utf8_octet_t *)parser->s->elts,
        (afw_size_t)parser->s->nelts);
}


/*ebnf>>>
 *
 *# Only DecimalIntegerLiteral is allowed in strict mode.
 * Integer ::= '-'? (
 *     DecimalIntegerLiteral |
 *     BinaryIntegerLiteral |
 *     HexIntegerLiteral |
 *     OctetIntegerLiteral )
 *
 * DecimalIntegerLiteral ::= '0' | ( [1-9] [0-9]* )
 *
 * BinaryIntegerLiteral ::= '0' ('b' | 'B' ) BinaryDigit+
 *
 * HexIntegerLiteral ::= '0' ('x' | 'X' ) HexDigit+
 *
 * OctalIntegerLiteral ::= '0' ('o' | 'O' ) OctalDigit+
 *
 *# Infinity, INF and NaN are not allowed in strict mode.
 *
 * Double ::= '-'? ( ( DecimalIntegerLiteral '.' [0-9]+ ([eE] [+-]? [0-9]+)? ) |
 *    'Infinity' | 'INF' | 'NaN' )
 *
 *<<<ebnf*/
/*
 * Parse a number.
 *
 * Note: Infinity, INF, and NaN are handled by impl_parse_identifier(), but
 *      if they are preceded by a '-', they are handled here.
 */

static afw_integer_t
impl_parse_BinaryIntegerLiteral(afw_compile_parser_t *parser)
{
    int digit;
    afw_integer_t result;

    digit = impl_get_BinaryDigit(parser);
    if (digit < 0) {
        AFW_COMPILE_THROW_ERROR_Z("Invalid binary digit");
    }
    result = digit;

    for (;;) {
        digit = impl_get_BinaryDigit(parser);
        if (digit < 0) {
            return result;
        }
        if (result > AFW_INTEGER_MAX / 2)
        {
            AFW_COMPILE_THROW_ERROR_Z("Integer is out of range");
        }
        result <<= 1;
        if (result > AFW_INTEGER_MAX - digit) {
            AFW_COMPILE_THROW_ERROR_Z("Integer is out of range");
        }
        result += digit;
    }
}



static afw_integer_t
impl_parse_HexIntegerLiteral(afw_compile_parser_t *parser)
{
    int digit;
    afw_integer_t result;

    digit = impl_get_HexDigit(parser);
    if (digit < 0) {
        AFW_COMPILE_THROW_ERROR_Z("Invalid hex digit");
    }
    result = digit;

    for (;;) {
        digit = impl_get_HexDigit(parser);
        if (digit < 0) {
            return result;
        }
        if (result > AFW_INTEGER_MAX / 16)
        {
            AFW_COMPILE_THROW_ERROR_Z("Integer is out of range");
        }
        result <<= 4;
        if (result > AFW_INTEGER_MAX - digit) {
            AFW_COMPILE_THROW_ERROR_Z("Integer is out of range");
        }
        result += digit;
    }
}



static afw_integer_t
impl_parse_OctalIntegerLiteral(afw_compile_parser_t *parser)
{
    int digit;
    afw_integer_t result;

    digit = impl_get_OctalDigit(parser);
    if (digit < 0) {
        AFW_COMPILE_THROW_ERROR_Z("Invalid octal digit");
    }
    result = digit;

    for (;;) {
        digit = impl_get_OctalDigit(parser);
        if (digit < 0) {
            return result;
        }
        if (result > AFW_INTEGER_MAX / 8)
        {
            AFW_COMPILE_THROW_ERROR_Z("Integer is out of range");
        }
        result <<= 3;
        if (result > AFW_INTEGER_MAX - digit) {
            AFW_COMPILE_THROW_ERROR_Z("Integer is out of range");
        }
        result += digit;
    }
}



static afw_boolean_t
impl_parse_number(afw_compile_parser_t *parser)
{
    afw_size_t start_offset;
    const afw_utf8_octet_t *s;
    afw_integer_t negative;
    afw_boolean_t is_negative;
    afw_boolean_t is_integer;
    afw_boolean_t is_zero;
    afw_utf8_octet_t o;
    afw_code_point_t cp;

    afw_compile_save_cursor(start_offset);
    negative = 0;
    is_negative = false;
    is_integer = true;
    is_zero = true;

    /* Determine if negative and handle reserved identifiers. */
    o = impl_get_octet(parser);

    /* Error if '+' and strict, otherwise just ignore it. */
    if (o == '+') {
        if (parser->strict) {
            goto error;
        }
    }

    /* Error if '-' indicate minus and handle special negative strings. */
    else if (o == '-') {
        is_negative = true;

        if (impl_consume_matching_octets_z(parser, "Infinity") ||
            impl_consume_matching_octets_z(parser, "INF"))
        {
            parser->token->type = afw_compile_token_type_number;
            parser->token->number = parser->xctx->env->minus_infinity;
            goto reserved_identifier;
        }

        if (impl_consume_matching_octets_z(parser, "NaN")) {
            parser->token->type = afw_compile_token_type_number;
            parser->token->number = parser->xctx->env->NaN;
            goto reserved_identifier;
        }
    }

    /* If not + or -, restore cursor. */
    else {
        afw_compile_restore_cursor(start_offset);
    }

    /* Parse integer part and prescan floating point part. */
    do {
        o = impl_get_octet(parser);

        /* Number can't end here. */
        if (afw_compile_is_at_eof()) {
            goto error;
        }

        /* Can be '0' or digits'1'-'9' followed by additional digits '0'-'9'. */
        if (o >= '1' && o <= '9') {
            is_zero = false;
            /* Sum digits as a negative number.  Negative is one larger. */
            negative = -(o - '0');
            for (;;) {
                o = impl_get_octet(parser);
                if (afw_compile_is_at_eof()) {
                    break;
                }
                if (o < '0' || o > '9') {
                    parser->cursor--;
                    break;
                }
                if (negative < AFW_INTEGER_MIN / 10)
                {
                    AFW_COMPILE_THROW_ERROR_Z("Integer is out of range");
                }
                negative = (negative * 10);
                if (negative < AFW_INTEGER_MIN + (o - '0'))
                {
                    AFW_COMPILE_THROW_ERROR_Z("Integer is out of range");
                }
                negative -= (o - '0');
            }
        }
        else if (o == '0') {
            o = impl_get_octet(parser);
            if (afw_compile_is_at_eof()) {
                break;
            }

            /* Decimal literal can not start with a '0'. */
            if (o >= '0' && o <= '9') {
                goto error;
            }

            /* If not strict mode, can be binary, hex, or octal literal. */
            if (!parser->strict) {
                if (o == 'b' || o == 'B') {
                    parser->token->type = afw_compile_token_type_integer;
                    parser->token->integer =
                        impl_parse_BinaryIntegerLiteral(parser);
                    if (is_negative) {
                        parser->token->integer = -parser->token->integer;
                    }
                    return true;
                }
                if (o == 'x' || o == 'X') {
                    parser->token->type = afw_compile_token_type_integer;
                    parser->token->integer =
                        impl_parse_HexIntegerLiteral(parser);
                    if (is_negative) {
                        parser->token->integer = -parser->token->integer;
                    }
                    return true;
                }
                if (o == 'o' || o == 'O') {
                    parser->token->type = afw_compile_token_type_integer;
                    parser->token->integer =
                        impl_parse_OctalIntegerLiteral(parser);
                    if (is_negative) {
                        parser->token->integer = -parser->token->integer;
                    }
                    return true;
                }
            }

            /* Parse as double or decimal integer. */
            parser->cursor--;
        }
        else {
            goto error;
        }

        /* Number is an integer if at end of eof. */
        if (afw_compile_is_at_eof()) {
            break;
        }

        /* Next can be an optional period followed by digits '0'-'9'. */
        o = impl_get_octet(parser);
        if (afw_compile_is_at_eof()) {
            break;
        }
        if (o != '.') {
            parser->cursor--;
        }
        else {
            is_integer = false;
            o = impl_get_octet(parser);
            /* Number can't end here. */
            if (afw_compile_is_at_eof() || o < '0' || o > '9') {
                goto error;
            }
            if (o != '0') {
                is_zero = false;
            }
            for (;;) {
                o = impl_get_octet(parser);
                if (afw_compile_is_at_eof()) {
                    break;
                }
                if (o < '0' || o > '9') {
                    parser->cursor--;
                    break;
                }
                if (o != '0') {
                    is_zero = false;
                }
            }
        }

        /* Number can end here. */
        if (afw_compile_is_at_eof()) {
            break;
        }

        /*
         * Next can be an 'e' or 'E' followed by optional '-' or '+' followed
         * by digits '0' - '9'.
         */
        o = impl_get_octet(parser);
        if (o != 'e' && o != 'E') {
            parser->cursor--;
        }
        else {
            is_integer = false;
            o = impl_get_octet(parser);

            /* Number can't end here. */
            if (afw_compile_is_at_eof()) {
                goto error;
            }

            /* Skip '+' or '-'. */
            if (o == '+' || o == '-') {
                o = impl_get_octet(parser);
            }

            /* Number can't end here. */
            if (afw_compile_is_at_eof() || o < '0' || o > '9') goto error;

            /* Skip digits. */
            for (;;) {
                o = impl_get_octet(parser);
                if (afw_compile_is_at_eof()) {
                    break;
                }
                if (o < '0' || o > '9') {
                    parser->cursor--;
                    break;
                }
            }
        }
    } while (0);

    /*
     * If is_integer (no '.', 'e', or 'E'), return integer.
     *
     * Note: javascript treats all numbers as double, but adaptive
     *       frameworks treats integers as afw_integer_t.
     */
    if (is_integer) {
        parser->token->type = afw_compile_token_type_integer;
        if (is_negative) {
            parser->token->integer = negative;
        }
        else if (negative != AFW_INTEGER_MIN) {
            parser->token->integer = -negative;
        }
        else {
            goto error;
        }
        return true;
    }


    /*
     * Not integer, create a double value. Use atof to convert number if it
     * is not zero.
     */
    if (is_negative)
        parser->token->number = -0.0;
    else
        parser->token->number = 0;
    parser->token->type = afw_compile_token_type_number;
    if (!is_zero) {
        s = apr_pstrndup(parser->apr_p,
            parser->full_source->s + start_offset,
            parser->cursor - start_offset);
        errno = 0;
        parser->token->number = strtod(s, NULL);
        if (errno != 0) goto error;
    }

    return true;

reserved_identifier:
    /* Make sure reserved identifier is not followed by id_continue cp. */
    afw_compile_save_cursor(start_offset);
    cp = afw_compile_get_code_point();
    if (afw_compile_code_point_is_IdentifierContinue(cp)) goto error;
    afw_compile_restore_cursor(start_offset);
    return true;

error:
    return false;
}

static const afw_utf8_t *
impl_get_identifier(afw_compile_parser_t *parser)
{
    afw_code_point_t cp;
    afw_size_t start_offset;
    afw_size_t save;
    const afw_utf8_t *result;

    /* Scan for end of identifier. */
    afw_compile_save_cursor(start_offset);
    cp = afw_compile_get_code_point();
    if (!afw_compile_code_point_is_IdentifierStart(cp)) {
        AFW_COMPILE_THROW_ERROR_Z("Invalid start code point for identifier");
    }
    for (;;) {
        afw_compile_save_cursor(save);
        cp = afw_compile_get_code_point();
        if (cp < 0) break;
        if (!afw_compile_code_point_is_IdentifierContinue(cp)) {
            afw_compile_restore_cursor(save);
            break;
        }
    }

    /* Create string for identifier. */
    result = afw_compile_get_string_literal(
        parser,
        parser->full_source->s + start_offset,
        parser->cursor - start_offset);


    /* Return result. */
    return result;
}


/*ebnf>>>
 *
 *# ZWNJ - Unicode Zero-width non-joiner (0x200c)
 *# ZWJ - Unicode Zero-width joiner (0x200d)
 *# ID_START is any codepoint with ID_START flag
 *# ID_CONTINUE is any codepoint with ID_CONTINUE flag
 *
 * IdentifierStart ::= ID_START | '$' | '_'
 *
 * IdentifierContinue ::= ID_CONTINUE | '$' | ZWNJ | ZWJ
 *
 * Identifier ::= IdentifierStart IdentifierContinue*
 *
 *<<<ebnf*/
/*
 * This function returns the literal evaluated value of special identifiers
 * like true and NaN if parser->next_identifier_is_not_special_literal is not
 * true.
 */
static void
impl_parse_identifier(afw_compile_parser_t *parser)
{
    afw_code_point_t cp;
    afw_size_t start_cursor;
    afw_size_t save_cursor;

    afw_compile_save_cursor(start_cursor);

    /* Set token type. */
    parser->token->type = afw_compile_token_type_identifier;

    /* Get identifier_name. */
    parser->token->identifier_name = impl_get_identifier(parser);
    parser->token->identifier = parser->token->identifier_name;

    /* Qualified identifier, get qualifier. */
    if (!afw_compile_is_at_eof()) {
        afw_compile_save_cursor(save_cursor);
        cp = afw_compile_get_code_point();
        if (cp != ':') {
            afw_compile_restore_cursor(save_cursor);
        }
        else {
            cp = afw_compile_get_code_point();
            if (cp != ':') {
                afw_compile_restore_cursor(save_cursor);
            }
            else {
                parser->token->identifier_qualifier =
                    parser->token->identifier_name;
                parser->token->identifier_name = impl_get_identifier(parser);
                parser->token->identifier = afw_compile_get_string_literal(
                    parser,
                    parser->full_source->s + start_cursor,
                    parser->cursor - start_cursor);
            }
        }
    }

    if (parser->token->identifier_qualifier ||
        parser->next_identifier_is_not_special_literal)
    {
        return;
    }

    /* Handle reserved identifier true. */
    if (afw_utf8_equal_utf8_z(parser->token->identifier_name, "true")) {
        parser->token->type = afw_compile_token_type_boolean;
        parser->token->boolean = true;
    }

    /* Handle reserved identifier false. */
    else if (afw_utf8_equal_utf8_z(parser->token->identifier_name, "false")) {;
        parser->token->type = afw_compile_token_type_boolean;
        parser->token->boolean = false;
    }

    /* Handle reserved identifier null. */
    else if (afw_utf8_equal_utf8_z(parser->token->identifier_name, "null")) {
        parser->token->type = afw_compile_token_type_null;
        parser->token->null = NULL;
    }

    /* Handle reserved identifier undefined. */
    else if (afw_utf8_equal_utf8_z(parser->token->identifier_name, "undefined"))
    {
        parser->token->type = afw_compile_token_type_undefined;
    }

    /* Handle reserved identifier Infinity and INF.  */
    else if (
        afw_utf8_equal_utf8_z(parser->token->identifier_name, "Infinity") ||
        afw_utf8_equal_utf8_z(parser->token->identifier_name, "INF"))
    {
        parser->token->type = afw_compile_token_type_number;
        parser->token->number = parser->xctx->env->infinity;
    }

    /* Handle reserved identifier NaN.  */
    else if (afw_utf8_equal_utf8_z(parser->token->identifier_name, "NaN")) {
        parser->token->type = afw_compile_token_type_number;
        parser->token->number = parser->xctx->env->NaN;
    }
}



/*ebnf>>>
 *
 *# Reserved words can not be used as an identifier.
 * ReservedWords ::= (
 *      PredefinedConstReservedWords |
 *      StatementReservedWords |
 *      UnusedButReservedWords )
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(afw_boolean_t)
afw_compile_is_reserved_word(
    afw_compile_parser_t *parser,
    const afw_utf8_t *s)
{
    /*
     * The 'if' is probably a good way to do this since there are so few but
     * might should change to a binary search.
     */
    if (

/*ebnf>>>
 *
 * PredefinedConstReservedWords ::= ( 'false' | 'INF' | 'Infinity' | 'NaN'  |
 *      'null' | 'true' | 'undefined' )
 * 
 *<<<ebnf*/

        afw_utf8_equal(s, afw_s_false)        ||
        afw_utf8_equal(s, afw_s_INF)          ||
        afw_utf8_equal(s, afw_s_Infinity)     ||
        afw_utf8_equal(s, afw_s_NaN)          ||
        afw_utf8_equal(s, afw_s_null)         ||
        afw_utf8_equal(s, afw_s_true)         ||
        afw_utf8_equal(s, afw_s_undefined)    ||

/*ebnf>>>
 * 
 * StatementReservedWords ::= ( 'break' | 'case' | 'catch' | 'const' | 
 *      'continue' | 'default' | 'do' | 'else' | 'finally' | 'for' | 
 *      'function' | 'if' | 'let' | 'return' | 'switch' |
 *      'throw' | 'try' | 'void' | 'while' )
 * 
 *<<<ebnf*/

        afw_utf8_equal(s, afw_s_break)        ||
        afw_utf8_equal(s, afw_s_case)         ||
        afw_utf8_equal(s, afw_s_catch)        ||
        afw_utf8_equal(s, afw_s_const)        ||
        afw_utf8_equal(s, afw_s_continue)     ||
        afw_utf8_equal(s, afw_s_default)      ||
        afw_utf8_equal(s, afw_s_do)           ||
        afw_utf8_equal(s, afw_s_else)         ||
        afw_utf8_equal(s, afw_s_finally)      ||
        afw_utf8_equal(s, afw_s_for)          ||
        afw_utf8_equal(s, afw_s_function)     ||
        afw_utf8_equal(s, afw_s_if)           ||
        afw_utf8_equal(s, afw_s_let)          ||
        afw_utf8_equal(s, afw_s_return)       ||
        afw_utf8_equal(s, afw_s_switch)       ||
        afw_utf8_equal(s, afw_s_throw)        ||
        afw_utf8_equal(s, afw_s_try)          ||
        afw_utf8_equal(s, afw_s_void)         ||
        afw_utf8_equal(s, afw_s_while)        ||

/*ebnf>>>
 * 
 * UnusedButReservedWords ::= ( 'as' | 'async' | 'await' | 'class' | 'delete' |
 *      'export' | 'extends' | 'from' | 'import' | 'in' | 'interface' |
 *      'instanceof' | 'super' | 'this' | 'type' | 'typeof' | 'var' | 'with' )
 *
 *<<<ebnf*/

        afw_utf8_equal(s, afw_s_as)           ||
        afw_utf8_equal(s, afw_s_async)        ||
        afw_utf8_equal(s, afw_s_await)        ||
        afw_utf8_equal(s, afw_s_class)        ||
        afw_utf8_equal(s, afw_s_delete)       ||
        afw_utf8_equal(s, afw_s_export)       ||
        afw_utf8_equal(s, afw_s_extends)      ||
        afw_utf8_equal(s, afw_s_from)         ||
        afw_utf8_equal(s, afw_s_import)       ||
        afw_utf8_equal(s, afw_s_in)           ||
        afw_utf8_equal(s, afw_s_instanceof)   ||
        afw_utf8_equal(s, afw_s_interface)    ||
        afw_utf8_equal(s, afw_s_super)        ||
        afw_utf8_equal(s, afw_s_this)         ||
        afw_utf8_equal(s, afw_s_type)         ||
        afw_utf8_equal(s, afw_s_typeof)       ||
        afw_utf8_equal(s, afw_s_var)          ||
        afw_utf8_equal(s, afw_s_with)         )
    {
        return true;
    }

    return false;
}



/* Internal functions. */

AFW_DEFINE_INTERNAL(afw_code_point_t)
afw_compile_get_code_point_impl(afw_compile_parser_t *parser)
{
    afw_code_point_t result;
    afw_octet_t o;

    o = impl_get_octet(parser);
    if (afw_compile_is_at_eof()) return -1;

    if (o <= 127) {
        result = o;
    }

    else if (o >= 0xf0 /* 11110000 */) {
        o &= 0x0f;
        if (o > 0x07) goto error;
        result = o << 18;

        o = impl_get_octet(parser);
        if (afw_compile_is_at_eof()) goto error;
        if (o < 0x80) goto error;
        o &= 0x7f;
        if (o > 0x3f) goto error;
        result += o << 12;

        o = impl_get_octet(parser);
        if (afw_compile_is_at_eof()) goto error;
        if (o < 0x80) goto error;
        o &= 0x7f;
        if (o > 0x3f) goto error;
        result += o << 6;

        o = impl_get_octet(parser);
        if (afw_compile_is_at_eof()) goto error;
        if (o < 0x80) goto error;
        o &= 0x7f;
        if (o > 0x3f) goto error;
        result += o ;
    }

    else if (o >= 0xe0 /* 11100000 */) {
        o &= 0x1f;
        if (o > 0x0f) goto error;
        result = o << 12;

        o = impl_get_octet(parser);
        if (afw_compile_is_at_eof()) goto error;
        if (o < 0x80) goto error;
        o &= 0x7f;
        if (o > 0x3f) goto error;
        result += o << 6;

        o = impl_get_octet(parser);
        if (afw_compile_is_at_eof()) goto error;
        if (o < 0x80) goto error;
        o &= 0x7f;
        if (o > 0x3f) goto error;
        result += o ;
    }

    else {
        o &= 0x3f;
        if (o > 0x1f) goto error;
        result = o << 6;

        o = impl_get_octet(parser);
        if (afw_compile_is_at_eof()) goto error;
        if (o < 0x80) goto error;
        o &= 0x7f;
        if (o > 0x3f) goto error;
        result += o ;
    }

    return result;

error:
    AFW_COMPILE_THROW_ERROR_Z("Invalid utf-8");

}


static afw_code_point_t
impl_unescaped(afw_compile_parser_t *parser)
{
    afw_utf8_octet_t o;
    afw_code_point_t cp, cp2;

    /* If escape, get next octet and break if eof.*/
    o = impl_get_octet(parser);
    if (afw_compile_is_at_eof()) {
        AFW_COMPILE_THROW_ERROR_Z("Invalid escape code");
    }

    /* Process based on octet after \. */
    switch (o) {

    case '"':
    case '\'':
    case '`':
    case '\\':
    case '/':
        cp = o;
        break;

    case 'b':
        cp = AFW_ASCII_BS;
        break;

    case 'f':
        cp = AFW_ASCII_FF;
        break;

    case 'n':
        cp = AFW_ASCII_LF;
        break;

    case 'r':
        cp = AFW_ASCII_CR;
        break;

    case 't':
        cp = AFW_ASCII_HT;
        break;

    case 'v':
        cp = AFW_ASCII_VT;
        break;

    case 'u':
        /* Code point for /uxxxx. */
        cp =
            impl_get_required_HexDigit(parser) * 0x1000 +
            impl_get_required_HexDigit(parser) * 0x100 +
            impl_get_required_HexDigit(parser) * 0x10 +
            impl_get_required_HexDigit(parser);

        /* If code point is a utf-16 surrogate, it must be paired. */
        if (cp >= 0xD800 && cp <= 0xDFFF) {
            if (cp >= 0xDC00) goto error;
            o = impl_get_octet(parser);
            if (o != '\\') goto error;
            o = impl_get_octet(parser);
            if (o != 'u') goto error;
            cp2 =
                impl_get_required_HexDigit(parser) * 0x1000 +
                impl_get_required_HexDigit(parser) * 0x100 +
                impl_get_required_HexDigit(parser) * 0x10 +
                impl_get_required_HexDigit(parser);
            if (cp2 < 0xDC00 || cp2 > 0xDFFF) goto error;
            cp = ((cp - 0xD800) << 10) + (cp2 - 0xDC00) + 0x10000;
        }

        break;

    default:
        goto error;
    }

    return cp;

error:
    AFW_COMPILE_THROW_ERROR_Z("Invalid escape code");
}



AFW_DEFINE_INTERNAL(afw_code_point_t)
afw_compile_get_unescaped_code_point_impl(afw_compile_parser_t *parser)
{
    afw_code_point_t cp;

    cp = afw_compile_get_code_point();

    if (cp == '\\') {
        cp = impl_unescaped(parser);
    }

    return cp;
}


AFW_DEFINE_INTERNAL(afw_boolean_t)
afw_compile_next_raw_starts_with_impl(
    afw_compile_parser_t *parser,
    const afw_utf8_t *s)
{
    afw_size_t cursor;
    afw_size_t i;
    afw_utf8_octet_t o;
    afw_boolean_t result;

    afw_compile_save_cursor(cursor);
    for (result = true, i = 0; i < s->len; i++) {
        o = impl_get_octet(parser);
        if (afw_compile_is_at_eof() || o != s->s[i]) {
            result = false;
            break;
        }
    }
    afw_compile_restore_cursor(cursor);

    return result;
}


AFW_DEFINE_INTERNAL(afw_boolean_t)
afw_compile_next_raw_starts_with_z_impl(
    afw_compile_parser_t *parser,
    const afw_utf8_z_t *s_z)
{
    afw_utf8_t s;

    s.s = s_z;
    s.len = strlen(s_z);
    return afw_compile_next_raw_starts_with_impl(parser, &s);
}



AFW_DEFINE_INTERNAL(void)
afw_compile_get_raw_line_impl(
    afw_compile_parser_t *parser,
    afw_utf8_t *line)
{
    afw_size_t cursor;
    afw_utf8_octet_t o;

    if (parser->last_octet_eof) {
        line->s = NULL;
        line->len = 0;
        return;
    }

    afw_compile_save_cursor(cursor);

    /* Get next token after whitespace. */
    parser->current_cursor_index++;
    if (parser->current_cursor_index >= AFW_COMPILE_MAX_TOKENS) {
        parser->current_cursor_index = 0;
    }
    parser->cursors[parser->current_cursor_index] = parser->cursor;
    parser->cursor_count++;
    if (parser->cursor_count > AFW_COMPILE_MAX_TOKENS) {
        parser->cursor_count = AFW_COMPILE_MAX_TOKENS;
    }
    parser->token->token_source_offset = parser->cursor;


    for (;;) {
        o = impl_get_octet(parser);
        if (afw_compile_is_at_eof() || o == '\n') {
            break;
        }
    }

    line->s = parser->full_source->s + cursor;
    line->len = parser->cursor - cursor;
    if (line->len > 0 && o != -1 && !afw_compile_is_at_eof())
    {
        (line->len)--;
    }

    parser->token->type = afw_compile_token_type_raw_line;
    parser->token->token_source_offset = cursor;
    parser->token->token_source_len = line->len;
    parser->token->raw_line.len = line->len;
    parser->token->raw_line.s = line->s;


    /* Set token source length. */
    parser->token->token_source_len =
        parser->cursor - parser->token->token_source_offset;

}


/*ebnf>>>
 *
 *# End of source
 * END ::=
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(void)
afw_compile_get_token_impl(afw_compile_parser_t *parser)
{
    afw_code_point_t cp, cp2;
    afw_size_t entry_cursor, temp_cursor;

    /* Clear token and skip whitespace. */
    memset(parser->token, 0, sizeof(afw_compile_token_t));
    afw_compile_skip_ws(parser);

    /*
     * If call by afw_compile_get_token_before_eol() and only whitespace
     * was found before end of line, just return eol token.
     */
    parser->get_token_before_eol = false;
    if (parser->get_token_found_eol) {
        parser->token->type = afw_compile_token_type_eol;
        return;
    }

    /* Get next token after whitespace. */
    parser->current_cursor_index++;
    if (parser->current_cursor_index >= AFW_COMPILE_MAX_TOKENS) {
        parser->current_cursor_index = 0;
    }
    parser->cursors[parser->current_cursor_index] = parser->cursor;
    parser->cursor_count++;
    if (parser->cursor_count > AFW_COMPILE_MAX_TOKENS) {
        parser->cursor_count = AFW_COMPILE_MAX_TOKENS;
    }
    parser->token->token_source_offset = entry_cursor = parser->cursor;
    cp = (afw_compile_is_at_eof()) ? -1 : afw_compile_get_code_point();
    switch (cp) {

    case -1:
        parser->token->type = afw_compile_token_type_end;
        break;

    case '"':
    case '\'':
        afw_compile_restore_cursor(entry_cursor);
        impl_parse_String(parser);
        break;

    case '`':
        if (parser->next_can_be_template_string) {
            afw_compile_restore_cursor(entry_cursor);
            impl_parse_String(parser);
        }
        else {
            parser->token->type = afw_compile_token_type_grave;
        }
        break;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        afw_compile_restore_cursor(entry_cursor);
        if (!impl_parse_number(parser)) {
            AFW_COMPILE_THROW_ERROR_Z("Invalid number");
        };
        break;

    /* $ and ${ */
    case '$':
        afw_compile_save_cursor(temp_cursor);
        cp2 = afw_compile_get_code_point();
        if (cp2 == '{') {
            parser->token->type =
                afw_compile_token_type_substitute_start;
        }
        else {
            afw_compile_restore_cursor(temp_cursor);
            parser->token->type = afw_compile_token_type_dollar_sign;
        }
        break;

    /* . and ... */
    case '.':
        afw_compile_save_cursor(temp_cursor);
        cp2 = afw_compile_get_code_point();
        if (cp2 == '.') {
            cp2 = afw_compile_get_code_point();
            if (cp2 == '.') {
                parser->token->type = afw_compile_token_type_ellipsis;
            }
            else {
                afw_compile_restore_cursor(temp_cursor);
                parser->token->type = afw_compile_token_type_period;
            }
        }
        else {
            afw_compile_restore_cursor(temp_cursor);
            parser->token->type = afw_compile_token_type_period;
        }
        break;

    /* +, positive number literal, += ( token type of + determined by context) */
    case '+':
        afw_compile_save_cursor(temp_cursor);
        cp2 = afw_compile_get_code_point();
        if (cp2 == '=') {
            parser->token->type = afw_compile_token_type_plus_equal;
        }
        else if (cp2 == '+') {
            parser->token->type = afw_compile_token_type_increment;
        }
        else {
            afw_compile_restore_cursor(temp_cursor);
            if (parser->next_can_be_nonunary_operator) {
                parser->token->type = afw_compile_token_type_add;
            }
            else {
                if (!impl_parse_number(parser)) {
                    afw_compile_restore_cursor(temp_cursor);
                    parser->token->type = afw_compile_token_type_unary_plus;
                }
            }
        }
        break;

    /* -, negative number literal, -= ( token type of - determined by context) */
    case '-':
        afw_compile_save_cursor(temp_cursor);
        cp2 = afw_compile_get_code_point();
        if (cp2 == '=') {
            parser->token->type = afw_compile_token_type_minus_equal;
        }
        else if (cp2 == '-') {
            parser->token->type = afw_compile_token_type_decrement;
        }
        else if (cp2 == '>') {
            parser->token->type = afw_compile_token_type_thin_arrow;
        }
        else {
            afw_compile_restore_cursor(temp_cursor);
            if (parser->next_can_be_nonunary_operator) {
                parser->token->type = afw_compile_token_type_subtract;
            }
            else {
                afw_compile_restore_cursor(entry_cursor);
                if (!impl_parse_number(parser)) {
                    afw_compile_restore_cursor(temp_cursor);
                    parser->token->type = afw_compile_token_type_unary_minus;
                }
            }
        }
        break;

    /* *, **, *=, **= */
    case '*':
        afw_compile_save_cursor(temp_cursor);
        cp2 = afw_compile_get_code_point();
        if (cp2 == '=') {
            parser->token->type = afw_compile_token_type_multiply_equal;
        }
        else if (cp2 == '*') {
            afw_compile_save_cursor(temp_cursor);
            cp2 = afw_compile_get_code_point();
            if (cp2 == '=') {
                parser->token->type = afw_compile_token_type_exponentiation_equal;
            }
            else {
                afw_compile_restore_cursor(temp_cursor);
                parser->token->type = afw_compile_token_type_exponentiation;
            }
        }
        else {
            afw_compile_restore_cursor(temp_cursor);
            parser->token->type = afw_compile_token_type_multiply;
        }
        break;

    /* / and /= */
    case '/':
        afw_compile_save_cursor(temp_cursor);
        cp2 = afw_compile_get_code_point();
        if (cp2 == '=') {
            parser->token->type = afw_compile_token_type_divide_equal;
        }
        else {
            afw_compile_restore_cursor(temp_cursor);
            parser->token->type = (parser->next_can_be_nonunary_operator)
                ? afw_compile_token_type_divide
                : afw_compile_token_type_slash;
            parser->token->type = afw_compile_token_type_divide;
        }
        break;

    /* % and %= ( token type of % determined by context) */
    case '%':
        afw_compile_save_cursor(temp_cursor);
        cp2 = afw_compile_get_code_point();
        if (cp2 == '=') {
            parser->token->type = afw_compile_token_type_modulus_equal;
        }
        else {
            afw_compile_restore_cursor(temp_cursor);
            parser->token->type = (parser->next_can_be_nonunary_operator)
                ? afw_compile_token_type_modulus
                : afw_compile_token_type_percent;
        }
        break;

    /* = == => === */
    case '=':
        afw_compile_save_cursor(temp_cursor);
        cp2 = afw_compile_get_code_point();
        if (cp2 == '=') {
            afw_compile_save_cursor(temp_cursor);
            cp2 = afw_compile_get_code_point();
            if (cp2 == '=') {
                parser->token->type =
                    afw_compile_token_type_equal_value_and_type;
            }
            else {
                afw_compile_restore_cursor(temp_cursor);
                parser->token->type = afw_compile_token_type_equal_to;
            }
        }
        else if (cp2 == '>') {
            parser->token->type = afw_compile_token_type_fat_arrow;
        }
        else {
            afw_compile_restore_cursor(temp_cursor);
            parser->token->type = afw_compile_token_type_equal;
        }
        break;

    /* ! != !== */
    case '!':
        afw_compile_save_cursor(temp_cursor);
        cp2 = afw_compile_get_code_point();
        if (cp2 == '=') {
            afw_compile_save_cursor(temp_cursor);
            cp2 = afw_compile_get_code_point();
            if (cp2 == '=') {
                parser->token->type =
                    afw_compile_token_type_not_equal_value_and_type;
            }
            else {
                afw_compile_restore_cursor(temp_cursor);
                parser->token->type = afw_compile_token_type_not_equal_to;
            }
        }
        else {
            afw_compile_restore_cursor(temp_cursor);
            parser->token->type = afw_compile_token_type_unary_not;
        }
        break;

    /* < and <= ( token type of < determined by context) */
    case '<':
        afw_compile_save_cursor(temp_cursor);
        cp2 = afw_compile_get_code_point();
        if (cp2 == '=') {
            parser->token->type = afw_compile_token_type_less_than_or_equal_to;
        }
        else {
            afw_compile_restore_cursor(temp_cursor);
            parser->token->type = (parser->next_can_be_nonunary_operator)
                ? afw_compile_token_type_less_than
                : afw_compile_token_type_open_angle_bracket;
        }
        break;

    /* > and >= ( token type of > determined by context) */
    case '>':
        afw_compile_save_cursor(temp_cursor);
        cp2 = afw_compile_get_code_point();
        if (cp2 == '=') {
            parser->token->type =
                afw_compile_token_type_greater_than_or_equal_to;
        }
        else {
            afw_compile_restore_cursor(temp_cursor);
            parser->token->type = (parser->next_can_be_nonunary_operator)
                ? afw_compile_token_type_greater_than
                : afw_compile_token_type_close_angle_bracket;
        }
        break;

    case '&':
        afw_compile_save_cursor(temp_cursor);
        cp2 = afw_compile_get_code_point();
        if (cp2 == '&') {
            afw_compile_save_cursor(temp_cursor);
            cp2 = afw_compile_get_code_point();
            if (cp2 == '=') {
                parser->token->type = afw_compile_token_type_and_equal;
            }
            else {
                afw_compile_restore_cursor(temp_cursor);
                parser->token->type = afw_compile_token_type_and;
            }
        }
        else {
            afw_compile_restore_cursor(temp_cursor);
            parser->token->type = afw_compile_token_type_ampersand;
        }
        break;

    case '|':
        afw_compile_save_cursor(temp_cursor);
        cp2 = afw_compile_get_code_point();
        if (cp2 == '|') {
            afw_compile_save_cursor(temp_cursor);
            cp2 = afw_compile_get_code_point();
            if (cp2 == '=') {
                parser->token->type = afw_compile_token_type_or_equal;
            }
            else {
                afw_compile_restore_cursor(temp_cursor);
                parser->token->type = afw_compile_token_type_or;
            }
        }
        else {
            afw_compile_restore_cursor(temp_cursor);
            parser->token->type = afw_compile_token_type_vertical_bar;
        }
        break;

    case '\\':
        parser->token->type = afw_compile_token_type_back_slash;
        break;

    case '^':
        parser->token->type = afw_compile_token_type_caret;
        break;

    case ':':
        parser->token->type = afw_compile_token_type_colon;
        break;

    case ',':
        parser->token->type = afw_compile_token_type_comma;
        break;

    case '?':
        afw_compile_save_cursor(temp_cursor);
        cp2 = afw_compile_get_code_point();
        if (cp2 == '?') {
            afw_compile_save_cursor(temp_cursor);
            cp2 = afw_compile_get_code_point();
            if (cp2 == '=') {
                parser->token->type = afw_compile_token_type_nullish_equal;
            }
            else {
                afw_compile_restore_cursor(temp_cursor);
                parser->token->type = afw_compile_token_type_nullish_coalescing;
            }
        }
        else if (cp2 == '.') {
            parser->token->type = afw_compile_token_type_optional_chaining;
        }
        else if (cp2 == '-') {
            cp2 = afw_compile_get_code_point();
            if (cp2 == '>') {
                parser->token->type =
                    afw_compile_token_type_optional_chaining_thin_arrow;
            }
            else {
                afw_compile_restore_cursor(temp_cursor);
                parser->token->type = afw_compile_token_type_question_mark;
            }
        }
        else {
            afw_compile_restore_cursor(temp_cursor);
            parser->token->type = afw_compile_token_type_question_mark;
        }
        break;

    case ';':
        parser->token->type = afw_compile_token_type_semicolon;
        break;

    case '~':
        parser->token->type = afw_compile_token_type_tilde;
        break;

    case '{':
        parser->token->type = afw_compile_token_type_open_brace;
        break;

    case '}':
        parser->token->type = afw_compile_token_type_close_brace;
        break;

    case '(':
        parser->token->type = afw_compile_token_type_open_parenthesis;
        break;

    case ')':
        parser->token->type = afw_compile_token_type_close_parenthesis;
        break;

    case '[':
        parser->token->type = afw_compile_token_type_open_bracket;
        break;

    case ']':
        parser->token->type = afw_compile_token_type_close_bracket;
        break;

    default:
        if (afw_compile_code_point_is_IdentifierStart(cp)) {
            afw_compile_restore_cursor(entry_cursor);
            impl_parse_identifier(parser);
        }
        else {
            parser->token->type = afw_compile_token_type_invalid;
        }
    }

    /* Reset next_can_be_* */
    parser->next_can_be_nonunary_operator = false;
    parser->next_can_be_template_string = false;
    parser->next_identifier_is_not_special_literal = false;

    /* Set token source length. */
    parser->token->token_source_len =
        parser->cursor - parser->token->token_source_offset;
}


AFW_DEFINE_INTERNAL(void)
afw_compile_reuse_token_impl(afw_compile_parser_t *parser)
{
    /* Ignore reuse token is last one is end of line. */
    if (parser->get_token_found_eol) {
        parser->get_token_found_eol = false;
        return;
    }

    /* Reset to previous token. */
    if (parser->cursor_count > 0) {
        parser->cursor_count--;
        parser->cursor = parser->cursors[parser->current_cursor_index];
        parser->current_cursor_index--;
        if (parser->current_cursor_index < 0) {
            parser->current_cursor_index = AFW_COMPILE_MAX_TOKENS - 1;
        }
        parser->last_octet_eof = false;
        parser->token->type = afw_compile_token_type_invalid;
    }
    else {
        AFW_THROW_ERROR_Z(general, "No token to reuse", parser->xctx);
    }
}


AFW_DEFINE_INTERNAL(afw_compile_internal_token_type_t)
afw_compile_peek_next_token_impl(afw_compile_parser_t *parser)
{
    afw_compile_internal_token_type_t result;

    afw_compile_get_token_impl(parser);
    result = parser->token->type;
    afw_compile_reuse_token_impl(parser);

    return result;
}


/* Make sure there is no residual data. */
AFW_DEFINE_INTERNAL(void)
afw_compile_check_for_residual(afw_compile_parser_t *parser)
{
    afw_octet_t o;
    afw_size_t save;

    parser->scanning_for_residual = true;

    switch (parser->residual_check) {

    case afw_compile_residual_check_none:
        break;

    case afw_compile_residual_check_to_newline:
        if (!afw_compile_is_at_eof()) for (;;) {
            o = impl_get_octet(parser);
            if (afw_compile_is_at_eof() || o == '\n') break;
            if (!afw_ascii_is_whitespace(o)) {
                afw_compile_save_cursor(save);
                goto error;
            }
        }
        break;

    case afw_compile_residual_check_to_full:
        if (afw_compile_is_at_eof()) break;
        afw_compile_skip_ws(parser);
        afw_compile_save_cursor(save);
        if (!afw_compile_next_is_at_eof()) goto error;
        break;

    default:
        AFW_THROW_ERROR_Z(general, "Invalid residual_check value",
            parser->xctx);
    }

    return;

error:
    AFW_THROW_ERROR_FZ(syntax, parser->xctx,
        "Residual data beginning at offset " AFW_SIZE_T_FMT,
        save);

}



/* Created a struct for sharing resources by multiple compiles. */
AFW_DEFINE(const afw_compile_shared_t *)
afw_compile_shared_create(
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_compile_shared_t *shared;

    shared = afw_pool_calloc_type(p, afw_compile_shared_t, xctx);
    shared->p = p;
    shared->temp_p = afw_pool_create(p, xctx);
    shared->string_literals = apr_hash_make(
        afw_pool_get_apr_pool(shared->temp_p));

    return shared;
}



/* Create a parser. */
AFW_DEFINE_INTERNAL(afw_compile_parser_t *)
afw_compile_lexical_parser_create(
    const afw_utf8_t *source,
    afw_utf8_octet_get_cb_t callback,
    void *callback_data,
    const afw_utf8_t *source_location,
    afw_compile_type_t compile_type,
    afw_compile_residual_check_t residual_check,
    afw_boolean_t cede_p,
    const afw_value_compiled_value_t *parent,
    const afw_compile_shared_t *shared,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_compile_parser_t *parser;

    /* Initialize parser work area. */
    parser = afw_xctx_calloc_type(afw_compile_parser_t, xctx);
    if (cede_p && ( shared || parent)) {
        AFW_THROW_ERROR_Z(general,
            "afw_compile_lexical_parser_create() parameter cede_p true when "
            "parent or shared is not NULL",
            xctx);
    }
    if (!p && !shared && !parent) {
        AFW_THROW_ERROR_Z(general,
            "afw_compile_lexical_parser_create() either "
            "parent, shared or p must be non-NULL",
            xctx);
    }
    if (shared) {
        parser->p = shared->p;
        parser->shared = shared;
    }
    else {
        if (parent) {
            parser->p = parent->p;
        }
        else if (cede_p) {
            parser->p = p;
        }
        else {
            parser->p = afw_pool_create(p, xctx);
        }
        parser->shared = afw_compile_shared_create(parser->p, xctx);
    }
    parser->apr_p = afw_pool_get_apr_pool(parser->p);
    parser->xctx = xctx;
    parser->cede_p = cede_p;
    parser->error = xctx->error;
    parser->passed_source = source;
    parser->callback = callback,
    parser->callback_data = callback_data,
    parser->strict = compile_type == afw_compile_type_json;
    parser->compile_type = compile_type;
    parser->residual_check = residual_check;
    parser->contextual.source_location = source_location;
    parser->s = apr_array_make(parser->apr_p, 256,
        sizeof(afw_utf8_octet_t));
    parser->values = apr_array_make(parser->apr_p, 10,
        sizeof(afw_value_t *));

    parser->estimated_size = 4096;
    parser->token = &parser->token_storage;

    /* Initialize parser->compiled_value. */
    parser->compiled_value = afw_pool_calloc_type(parser->p,
        afw_value_compiled_value_t, xctx);
    parser->compiled_value->inf = &afw_value_compiled_value_inf;
    parser->compiled_value->p = parser->p;
    if (source_location) {
        parser->compiled_value->source_location =
            afw_utf8_clone(source_location, parser->p, xctx);
    }
    if (parent) {
        parser->compiled_value->parent = parent;
        parser->compiled_value->top_block = parent->current_block;
        parser->compiled_value->current_block = parent->current_block;
    }

    return parser;
}


AFW_DEFINE_INTERNAL(void)
afw_compile_lexical_parser_finish_and_release(
    afw_compile_parser_t *parser,
    afw_xctx_t *xctx)
{

}



AFW_DEFINE_INTERNAL(const afw_utf8_t *)
afw_compile_get_string_literal(
    afw_compile_parser_t *parser,
    const afw_utf8_octet_t *s,
    afw_size_t len)
{
    const afw_utf8_t *result;

    result = apr_hash_get(parser->shared->string_literals, s, len);
    if (!result) {
        result = afw_utf8_create_copy(s, len,
            parser->p, parser->xctx);
        apr_hash_set(parser->shared->string_literals,
            (const void *)result->s,
            (apr_ssize_t)result->len,
            (const void *)result);
    }

    return result;
}


AFW_DEFINE_INTERNAL(const afw_utf8_t*)
afw_compile_current_raw_token(
    afw_compile_parser_t* parser)
{
    return afw_compile_get_string_literal(parser,
        parser->full_source->s + parser->token->token_source_offset,
        parser->token->token_source_len);
}
