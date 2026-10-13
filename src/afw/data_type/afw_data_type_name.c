// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework rfc822Name and x500Name support
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_data_type_name.c
 * @brief Read checks, equality, and match for rfc822Name and x500Name.
 *
 * Both data types store the text as written. Their meaning is only used
 * when comparing:
 *
 * - rfc822Name is a mailbox, `local-part@domain`. The local part is
 *   compared exactly and the domain part ignoring case.
 *
 * - x500Name is a distinguished name (RFC 4514, accepting RFC 2253 forms:
 *   `;` between RDNs, quoted values, `OID.` before a numeric type, and
 *   spaces around separators). RDNs are compared one by one. Attribute
 *   types ignore case, and a known name and its OID are the same type.
 *   Values ignore case and insignificant spaces; `#` hex values are
 *   octets. The parts of a multi-valued RDN are sorted before comparing.
 */

#include "afw_internal.h"


/* ---- rfc822Name -----------------------------------------------------------*/

/* Split at the last '@'. Return false if either part would be empty. */
static afw_boolean_t
impl_rfc822Name_split(
    const afw_utf8_t *s,
    afw_utf8_t *local_part,
    afw_utf8_t *domain_part)
{
    afw_size_t i;

    for (i = s->len; i > 0; i--) {
        if (s->s[i - 1] == '@') {
            break;
        }
    }
    if (i <= 1 || i == s->len) {
        return false;
    }

    local_part->s = s->s;
    local_part->len = i - 1;
    domain_part->s = s->s + i;
    domain_part->len = s->len - i;
    return true;
}


/* Check text read as an rfc822Name. */
AFW_DEFINE(void)
afw_data_type_rfc822Name_check(
    const afw_utf8_t *s,
    afw_xctx_t *xctx)
{
    afw_utf8_t local_part;
    afw_utf8_t domain_part;

    if (!impl_rfc822Name_split(s, &local_part, &domain_part)) {
        AFW_THROW_ERROR_FZ(conversion_error, xctx,
            "Invalid rfc822Name '%ku': it needs a local part, '@', "
            "and a domain part",
            s);
    }
}


/* Compare two rfc822Name values. */
AFW_DEFINE(int)
afw_data_type_rfc822Name_compare(
    const afw_utf8_t *s1,
    const afw_utf8_t *s2,
    afw_xctx_t *xctx)
{
    afw_utf8_t local1, domain1;
    afw_utf8_t local2, domain2;
    afw_boolean_t ok1, ok2;
    int result;

    ok1 = impl_rfc822Name_split(s1, &local1, &domain1);
    ok2 = impl_rfc822Name_split(s2, &local2, &domain2);

    /* Text that was not checked compares as text, after checked text. */
    if (!ok1 || !ok2) {
        if (ok1 != ok2) {
            return ok1 ? -1 : 1;
        }
        return afw_utf8_compare(s1, s2);
    }

    result = afw_utf8_compare(&local1, &local2);
    if (result == 0) {
        result = afw_utf8_compare_ignore_case(&domain1, &domain2, xctx);
    }
    return result;
}


/* Return true if an rfc822Name matches a complete or partial pattern. */
AFW_DEFINE(afw_boolean_t)
afw_data_type_rfc822Name_match(
    const afw_utf8_t *pattern,
    const afw_utf8_t *value,
    afw_xctx_t *xctx)
{
    afw_utf8_t local_part;
    afw_utf8_t domain_part;
    afw_utf8_t suffix;
    afw_size_t i;

    if (!impl_rfc822Name_split(value, &local_part, &domain_part)) {
        return false;
    }

    /* A complete mailbox matches that mailbox. */
    for (i = 0; i < pattern->len && pattern->s[i] != '@'; i++);
    if (i < pattern->len) {
        return afw_data_type_rfc822Name_compare(pattern, value, xctx) == 0;
    }

    /* An empty pattern matches nothing. */
    if (pattern->len == 0) {
        return false;
    }

    /* A domain only matches mailboxes at exactly that domain. */
    if (pattern->s[0] != '.') {
        return afw_utf8_compare_ignore_case(pattern, &domain_part, xctx) == 0;
    }

    /*
     * A domain that starts with '.' matches mailboxes in a subdomain of it.
     * The suffix of the domain part starts with '.', so it is a whole
     * label, and the domain part is longer, so it is not the domain itself.
     */
    if (domain_part.len <= pattern->len) {
        return false;
    }
    suffix.s = domain_part.s + domain_part.len - pattern->len;
    if (*suffix.s != '.') {
        return false;
    }
    suffix.len = pattern->len;
    return afw_utf8_compare_ignore_case(pattern, &suffix, xctx) == 0;
}



/* ---- x500Name -------------------------------------------------------------*/

/* One attribute type and value of an RDN. */
typedef struct impl_ava_s {
    /* Lower case name, or the OID if the name is known or numeric. */
    afw_utf8_t type;
    /* True if value is octets (a '#' value or escapes that are not UTF-8). */
    afw_boolean_t octets;
    /* Lower case with insignificant spaces removed, or the octets. */
    afw_utf8_t value;
} impl_ava_t;


/* One RDN: its attribute types and values, sorted. */
typedef struct impl_rdn_s {
    afw_size_t count;
    impl_ava_t *avas;
} impl_rdn_t;


/* A parsed distinguished name. */
typedef struct impl_dn_s {
    afw_size_t count;
    impl_rdn_t *rdns;
} impl_dn_t;


/* Parser state. With dn NULL it only checks and counts. */
typedef struct impl_parser_s {
    const afw_utf8_octet_t *start;
    const afw_utf8_octet_t *c;
    const afw_utf8_octet_t *end;
    impl_dn_t *dn;
    impl_ava_t *next_ava;
    const afw_pool_t *p;
    afw_xctx_t *xctx;
    afw_size_t rdn_count;
    afw_size_t ava_count;
} impl_parser_t;


/* Attribute type names (lower case) and their OIDs. */
static const struct {
    afw_utf8_t name;
    afw_utf8_t oid;
} impl_type_aliases[] = {
    { AFW_UTF8_LITERAL("cn"), AFW_UTF8_LITERAL("2.5.4.3") },
    { AFW_UTF8_LITERAL("sn"), AFW_UTF8_LITERAL("2.5.4.4") },
    { AFW_UTF8_LITERAL("c"), AFW_UTF8_LITERAL("2.5.4.6") },
    { AFW_UTF8_LITERAL("l"), AFW_UTF8_LITERAL("2.5.4.7") },
    { AFW_UTF8_LITERAL("st"), AFW_UTF8_LITERAL("2.5.4.8") },
    { AFW_UTF8_LITERAL("street"), AFW_UTF8_LITERAL("2.5.4.9") },
    { AFW_UTF8_LITERAL("o"), AFW_UTF8_LITERAL("2.5.4.10") },
    { AFW_UTF8_LITERAL("ou"), AFW_UTF8_LITERAL("2.5.4.11") },
    { AFW_UTF8_LITERAL("title"), AFW_UTF8_LITERAL("2.5.4.12") },
    { AFW_UTF8_LITERAL("uid"),
        AFW_UTF8_LITERAL("0.9.2342.19200300.100.1.1") },
    { AFW_UTF8_LITERAL("dc"),
        AFW_UTF8_LITERAL("0.9.2342.19200300.100.1.25") },
};

#define IMPL_TYPE_ALIAS_COUNT \
    (sizeof(impl_type_aliases) / sizeof(impl_type_aliases[0]))


#define IMPL_IS_ALPHA(c) \
    (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))

#define IMPL_IS_DIGIT(c) ((c) >= '0' && (c) <= '9')

#define IMPL_IS_SPACE(c) ((c) == ' ')

#define IMPL_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? (c) + ('a' - 'A') : (c))


static int
impl_hex_digit(afw_utf8_octet_t c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}


static void
impl_skip_spaces(impl_parser_t *parser)
{
    while (parser->c < parser->end && IMPL_IS_SPACE(*parser->c)) {
        parser->c++;
    }
}


/* Parse an attribute type. Return false if there is not one. */
static afw_boolean_t
impl_parse_type(impl_parser_t *parser, impl_ava_t *ava)
{
    const afw_utf8_octet_t *s;
    afw_utf8_octet_t *lower;
    afw_size_t len;
    afw_size_t i;

    /* "OID." before a numeric OID is the same OID (RFC 2253). */
    if (parser->end - parser->c > 4 &&
        IMPL_LOWER(parser->c[0]) == 'o' &&
        IMPL_LOWER(parser->c[1]) == 'i' &&
        IMPL_LOWER(parser->c[2]) == 'd' &&
        parser->c[3] == '.' &&
        IMPL_IS_DIGIT(parser->c[4]))
    {
        parser->c += 4;
    }

    s = parser->c;

    /* descr = ALPHA *( ALPHA / DIGIT / HYPHEN ) */
    if (parser->c < parser->end && IMPL_IS_ALPHA(*parser->c)) {
        for (parser->c++;
            parser->c < parser->end &&
            (IMPL_IS_ALPHA(*parser->c) || IMPL_IS_DIGIT(*parser->c) ||
                *parser->c == '-');
            parser->c++);
        if (ava) {
            len = parser->c - s;
            lower = afw_pool_malloc(parser->p, len, parser->xctx);
            for (i = 0; i < len; i++) {
                lower[i] = IMPL_LOWER(s[i]);
            }
            ava->type.s = lower;
            ava->type.len = len;
            for (i = 0; i < IMPL_TYPE_ALIAS_COUNT; i++) {
                if (afw_utf8_equal(&ava->type, &impl_type_aliases[i].name)) {
                    ava->type = impl_type_aliases[i].oid;
                    break;
                }
            }
        }
        return true;
    }

    /* numericoid = number 1*( DOT number ) */
    if (parser->c < parser->end && IMPL_IS_DIGIT(*parser->c)) {
        for (;;) {
            if (parser->c >= parser->end || !IMPL_IS_DIGIT(*parser->c)) {
                return false;
            }
            for (; parser->c < parser->end && IMPL_IS_DIGIT(*parser->c);
                parser->c++);
            if (parser->c >= parser->end || *parser->c != '.') {
                break;
            }
            parser->c++;
        }
        if (ava) {
            ava->type.s = s;
            ava->type.len = parser->c - s;
        }
        return true;
    }

    return false;
}


/*
 * Parse an attribute value, up to the next unescaped ',', ';', '+', or the
 * end. Return false if it is not a valid value.
 */
static afw_boolean_t
impl_parse_value(impl_parser_t *parser, impl_ava_t *ava)
{
    afw_utf8_octet_t *out;
    afw_size_t out_len;
    afw_boolean_t quoted;
    afw_utf8_octet_t c;
    int hi, lo;
    afw_utf8_t decoded;

    out = NULL;
    out_len = 0;
    if (ava) {
        out = afw_pool_malloc(parser->p,
            (parser->end - parser->c) + 1, parser->xctx);
    }

    /* hexstring = SHARP 1*hexpair */
    if (parser->c < parser->end && *parser->c == '#') {
        parser->c++;
        do {
            if (parser->end - parser->c < 2) {
                return false;
            }
            hi = impl_hex_digit(parser->c[0]);
            lo = impl_hex_digit(parser->c[1]);
            if (hi < 0 || lo < 0) {
                return false;
            }
            if (out) {
                out[out_len++] = (afw_utf8_octet_t)((hi << 4) + lo);
            }
            parser->c += 2;
        } while (parser->c < parser->end && impl_hex_digit(*parser->c) >= 0);
        if (ava) {
            ava->octets = true;
            ava->value.s = out;
            ava->value.len = out_len;
        }
        return true;
    }

    /* A quoted value is RFC 2253's form for special characters. */
    quoted = false;
    if (parser->c < parser->end && *parser->c == '"') {
        quoted = true;
        parser->c++;
    }

    for (;;) {
        if (parser->c >= parser->end) {
            if (quoted) {
                return false;
            }
            break;
        }

        c = *parser->c;
        if (quoted) {
            if (c == '"') {
                parser->c++;
                break;
            }
        }
        else if (c == ',' || c == ';' || c == '+') {
            break;
        }

        if (c == '\\') {
            parser->c++;
            if (parser->c >= parser->end) {
                return false;
            }
            hi = impl_hex_digit(parser->c[0]);
            if (hi >= 0) {
                if (parser->end - parser->c < 2) {
                    return false;
                }
                lo = impl_hex_digit(parser->c[1]);
                if (lo < 0) {
                    return false;
                }
                c = (afw_utf8_octet_t)((hi << 4) + lo);
                parser->c += 2;
            }
            else {
                c = *parser->c;
                if (c == 0 || !strchr("\"+,;<>\\ #=", c)) {
                    return false;
                }
                parser->c++;
            }
        }
        else {
            parser->c++;
        }

        if (out) {
            out[out_len++] = c;
        }
    }

    if (ava) {
        decoded.s = out;
        decoded.len = out_len;
        if (afw_utf8_is_valid(out, out_len, parser->xctx)) {
            ava->octets = false;
            ava->value = *afw_utf8_normalize_space(
                afw_utf8_to_lower(&decoded, parser->p, parser->xctx),
                parser->p, parser->xctx);
        }
        else {
            ava->octets = true;
            ava->value = decoded;
        }
    }

    return true;
}


/* Compare two attribute types and values. */
static int
impl_ava_compare(const impl_ava_t *a1, const impl_ava_t *a2)
{
    int result;

    result = afw_utf8_compare(&a1->type, &a2->type);
    if (result == 0 && a1->octets != a2->octets) {
        result = a1->octets ? 1 : -1;
    }
    if (result == 0) {
        result = afw_utf8_compare(&a1->value, &a2->value);
    }
    return result;
}


static int
impl_ava_qsort_compare(const void *a1, const void *a2)
{
    return impl_ava_compare(a1, a2);
}


/*
 * Parse a distinguished name. Without parser->dn, only check it and count
 * its RDNs and their parts. Return false if it does not parse.
 */
static afw_boolean_t
impl_parse_dn(impl_parser_t *parser)
{
    impl_rdn_t *rdn;
    impl_ava_t *ava;

    rdn = NULL;
    ava = NULL;
    parser->rdn_count = 0;
    parser->ava_count = 0;

    /* The empty DN has no RDNs. */
    impl_skip_spaces(parser);
    if (parser->c >= parser->end) {
        return true;
    }

    for (;;) {

        /* Each RDN's parts follow the parts of the RDNs before it. */
        if (parser->dn) {
            rdn = &parser->dn->rdns[parser->rdn_count];
            if (rdn->count == 0) {
                rdn->avas = parser->next_ava;
            }
            ava = parser->next_ava++;
        }

        impl_skip_spaces(parser);
        if (!impl_parse_type(parser, ava)) {
            return false;
        }
        impl_skip_spaces(parser);
        if (parser->c >= parser->end || *parser->c != '=') {
            return false;
        }
        parser->c++;
        impl_skip_spaces(parser);
        if (!impl_parse_value(parser, ava)) {
            return false;
        }
        impl_skip_spaces(parser);

        parser->ava_count++;
        if (rdn) {
            rdn->count++;
        }

        if (parser->c >= parser->end) {
            parser->rdn_count++;
            break;
        }

        if (*parser->c == '+') {
            parser->c++;
            continue;
        }

        if (*parser->c == ',' || *parser->c == ';') {
            parser->c++;
            parser->rdn_count++;
            continue;
        }

        return false;
    }

    return true;
}


/* Check and count a DN. Return the offset of the error, or -1 if valid. */
static afw_integer_t
impl_check_dn(
    const afw_utf8_t *s,
    afw_size_t *rdn_count,
    afw_size_t *ava_count,
    afw_xctx_t *xctx)
{
    impl_parser_t parser;

    afw_memory_clear(&parser);
    parser.start = s->s;
    parser.c = s->s;
    parser.end = s->s + s->len;
    parser.xctx = xctx;

    if (!impl_parse_dn(&parser)) {
        return parser.c - parser.start;
    }
    if (rdn_count) {
        *rdn_count = parser.rdn_count;
    }
    if (ava_count) {
        *ava_count = parser.ava_count;
    }
    return -1;
}


/* Parse a checked DN into p, or return NULL if it does not parse. */
static const impl_dn_t *
impl_dn_create(
    const afw_utf8_t *s,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    impl_parser_t parser;
    impl_dn_t *dn;
    impl_ava_t *avas;
    afw_size_t rdn_count;
    afw_size_t ava_count;
    afw_size_t i;

    /* First pass checks and counts, so the second can fill exact arrays. */
    rdn_count = 0;
    ava_count = 0;
    if (impl_check_dn(s, &rdn_count, &ava_count, xctx) >= 0) {
        return NULL;
    }

    dn = afw_pool_calloc_type(p, impl_dn_t, xctx);
    if (rdn_count == 0) {
        return dn;
    }
    dn->rdns = afw_pool_calloc(p, sizeof(impl_rdn_t) * rdn_count, xctx);
    avas = afw_pool_calloc(p, sizeof(impl_ava_t) * ava_count, xctx);

    afw_memory_clear(&parser);
    parser.start = s->s;
    parser.c = s->s;
    parser.end = s->s + s->len;
    parser.dn = dn;
    parser.next_ava = avas;
    parser.p = p;
    parser.xctx = xctx;
    impl_parse_dn(&parser);
    dn->count = parser.rdn_count;

    for (i = 0; i < dn->count; i++) {
        if (dn->rdns[i].count > 1) {
            qsort(dn->rdns[i].avas, dn->rdns[i].count, sizeof(impl_ava_t),
                impl_ava_qsort_compare);
        }
    }

    return dn;
}


/* Compare two RDNs. */
static int
impl_rdn_compare(const impl_rdn_t *r1, const impl_rdn_t *r2)
{
    afw_size_t i;
    int result;

    for (i = 0; i < r1->count && i < r2->count; i++) {
        result = impl_ava_compare(&r1->avas[i], &r2->avas[i]);
        if (result != 0) {
            return result;
        }
    }
    if (r1->count == r2->count) {
        return 0;
    }
    return (r1->count < r2->count) ? -1 : 1;
}


/* Check text read as an x500Name. */
AFW_DEFINE(void)
afw_data_type_x500Name_check(
    const afw_utf8_t *s,
    afw_xctx_t *xctx)
{
    afw_integer_t offset;

    offset = impl_check_dn(s, NULL, NULL, xctx);
    if (offset >= 0) {
        AFW_THROW_ERROR_FZ(conversion_error, xctx,
            "Invalid x500Name '%ku': not a distinguished name at offset %d",
            s, (int)offset);
    }
}


/*
 * Parse s1 and s2 in a heap of their own, then compare (match false) or
 * match s1 to the RDNs at the end of s2 (match true).
 */
static int
impl_x500Name_compare_or_match(
    const afw_utf8_t *s1,
    const afw_utf8_t *s2,
    afw_boolean_t match,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    const impl_dn_t *dn1;
    const impl_dn_t *dn2;
    afw_size_t i;
    afw_size_t skip;
    int result;

    p = afw_pool_heap_create(xctx->p, xctx->env->small_chunk_min, xctx);
    AFW_TRY {
        dn1 = impl_dn_create(s1, p, xctx);
        dn2 = impl_dn_create(s2, p, xctx);

        /* Text that was not checked compares as text, after checked text. */
        if (!dn1 || !dn2) {
            if (match) {
                result = 1;
            }
            else if (dn1 || dn2) {
                result = dn1 ? -1 : 1;
            }
            else {
                result = afw_utf8_compare(s1, s2);
            }
        }

        /* Match: s1's RDNs are the last RDNs of s2. */
        else if (match) {
            result = 1;
            if (dn1->count <= dn2->count) {
                skip = dn2->count - dn1->count;
                result = 0;
                for (i = 0; i < dn1->count && result == 0; i++) {
                    result = impl_rdn_compare(
                        &dn1->rdns[i], &dn2->rdns[skip + i]);
                }
            }
        }

        /* Compare RDN by RDN; a DN that runs out first is less. */
        else {
            result = 0;
            for (i = 0; i < dn1->count && i < dn2->count && result == 0; i++)
            {
                result = impl_rdn_compare(&dn1->rdns[i], &dn2->rdns[i]);
            }
            if (result == 0 && dn1->count != dn2->count) {
                result = (dn1->count < dn2->count) ? -1 : 1;
            }
        }
    }
    AFW_FINALLY {
        afw_pool_release(p, xctx);
    }
    AFW_ENDTRY;

    return result;
}


/* Compare two x500Name values. */
AFW_DEFINE(int)
afw_data_type_x500Name_compare(
    const afw_utf8_t *s1,
    const afw_utf8_t *s2,
    afw_xctx_t *xctx)
{
    return impl_x500Name_compare_or_match(s1, s2, false, xctx);
}


/* Return true if pattern's RDNs are the last RDNs of value. */
AFW_DEFINE(afw_boolean_t)
afw_data_type_x500Name_match(
    const afw_utf8_t *pattern,
    const afw_utf8_t *value,
    afw_xctx_t *xctx)
{
    return impl_x500Name_compare_or_match(pattern, value, true, xctx) == 0;
}
