// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW - String Functions
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_AFW_QUERY_CRITERIA_H__
#define __AFW_AFW_QUERY_CRITERIA_H__

#include "afw_minimal.h"

/**
 * @defgroup afw_query_criteria Query Criteria
 * @ingroup afw_c_api_public
 * 
 * All property_name parameters can use dotted notation to refer to properties
 * of embedded objects.
 *
 * @{
 */

/**
 * @file afw_query_criteria.h
 * @brief AFW query criteria header.
 *
 * This is the header file for AFW query criteria.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Query string style
 * 
 * The style of the query string produced.  All of the styles are appropriately
 * url encoded.
 *
 * 0 - "name=op=value" where "=op=" will be "==", "!=", "=ge=", etc. with
 *     conjunctions "&" (and) and "|" (or).  Parentheses are placed around
 *     each group of like conjunctions except for outer "&". (default)
 * 
 * 1 - the same as 1 with "=gte=" instead of "=ge=", "=lte=" instead of
 *     "=le=", "=eq=" instead of "=", and "=ne=" instead of '!='.
 * 
 * 2 - similar to 1 with ';' for '&' and ',' for '|'.
 *
 * 3 - comparisons "op(name,value)" where "op" will be "eq", "ne", "ge", ...,
 *     plus conjunctions "and(list)" and "or(list)" where "array" is a comma
 *     separated list of any comparison or conjunction.
 */
typedef enum {
    afw_query_criteria_style_operator = 0,
    afw_query_criteria_style_operator_long = 1,
    afw_query_criteria_style_semicolon_comma = 2,
    afw_query_criteria_style_function = 3,
    afw_query_criteria_style_max = 3
} afw_query_criteria_style_t;


/* Note: Opaque typedef for afw_query_criteria_t is in afw_interface.h. */

/** Typedef for a query criteria sort entry. */
typedef struct afw_query_criteria_sort_entry_s
    afw_query_criteria_sort_entry_t;



/** Enum for query criteria operators. */
typedef enum {
    /* This can be in entry->alt_op if not applicable. */
    afw_query_criteria_filter_op_id_na,

    /*
     * These can be in entry->op and entry->alt_op.
     *
     * If only supporting these, implementation must use entry->alt_not and
     * entry->alt_op.
     */
    afw_query_criteria_filter_op_id_eq,
    afw_query_criteria_filter_op_id_lt,
    afw_query_criteria_filter_op_id_le,
    afw_query_criteria_filter_op_id_match,
    afw_query_criteria_filter_op_id_contains,
    afw_query_criteria_filter_op_id_in,

    /*
     * These can be in entry->op. These will not be in entry->alt_op.
     */
    afw_query_criteria_filter_op_id_ne,        /* not eq */
    afw_query_criteria_filter_op_id_ge,        /* not lt */
    afw_query_criteria_filter_op_id_gt,        /* not lte */
    afw_query_criteria_filter_op_id_differ,    /* not match */
    afw_query_criteria_filter_op_id_excludes,  /* not contains */
    afw_query_criteria_filter_op_id_out,       /* not in */

    /*
     * These can be in entry->op, but will not be in an on_true, on_false, or
     * entry->alt_op.
     */
    afw_query_criteria_filter_op_id_and,
    afw_query_criteria_filter_op_id_or

} afw_query_criteria_filter_op_id_t;


/** Parsed sort entry from query string. */
struct afw_query_criteria_sort_entry_s {

    /* Next sort entry. */
    const afw_query_criteria_sort_entry_t *next;

    /* Property name of value to use for sort. */
    const afw_utf8_t *property_name;

    /* Sort descending if true. */
    afw_boolean_t descending;

    /*
     * @brief Property type or NULL (Internal use)
     */
    const afw_object_type_property_type_t *pt;
};


/** afw_query_criteria_filter_entry_t * of 1 indicates success. */
#define AFW_QUERY_CRITERIA_TRUE ((void *) 1)

/** afw_query_criteria_filter_entry_t * of 1 indicates fail. */
#define AFW_QUERY_CRITERIA_FALSE ((void *) 0)


/**
 * @brief Parsed filter entry from query string.
 *
 * See afw_query_criteria_t for more information.
 */
struct afw_query_criteria_filter_entry_s {

    /* @brief Next entry in the order coded in query string/object traversal. */
    const afw_query_criteria_filter_entry_t *next;

    /* @brief Operation string. */
    const afw_utf8_t *op_name;

    /* @brief Enum of operation to perform. */
    afw_query_criteria_filter_op_id_t op_id;

    /* @brief Indicates alt_op is the "not" of op. */
    afw_boolean_t alt_not;

    /*
     * @brief  Enum of operation to perform in conjunction with alt_not.
     *
     * Enum of alt operator that is used in conjunction with alt_not as an
     * alternative to op.  Using the alt_not/alt_op combo requires half of
     * operators to be implemented, but requires not to be supported.
     */
    afw_query_criteria_filter_op_id_t alt_op_id;

    /* Property name parameter for operation. */
    const afw_utf8_t *property_name;

    /*
     * Parameter for other operators converted to adaptive value based on
     * associated _AdaptiveObjectType_.
     */
    const afw_value_t *value;

    /* For op match, this is an internal regexp expression. */
    const void *op_specific;

    /* First child entry for and/or operator. */
    const afw_query_criteria_filter_entry_t *first_conjunctive_child;

    /* Next sibling entry for and/or traversal. */
    const afw_query_criteria_filter_entry_t *next_conjunctive_sibling;

    /*
     * Next entry to process if this entry evaluates to true.  Can also be
     * AFW_QUERY_CRITERIA_TRUE or AFW_QUERY_CRITERIA_FALSE.
     */
    const afw_query_criteria_filter_entry_t *on_true;

    /*
    * Next entry to process if this entry evaluates to false.  Can also be
    * AFW_QUERY_CRITERIA_TRUE or AFW_QUERY_CRITERIA_FALSE.
    */
    const afw_query_criteria_filter_entry_t *on_false;

    /*
     * @brief Property type or NULL (Internal use)
     */
    const afw_object_type_property_type_t *pt;
};


/**
 * @brief Parsed query criteria.
 *
 * This struct represents a parsed query.
 * 
 * Member filter and tree represent the same query in two different
 * ways.  An implementation of afw_adaptor_session should use the one that
 * is easiest for it to process.
 * 
 * To process filter, evaluate the entry and iteratively follow the
 * appropriate on_true or on_false link until the link is
 * AFW_QUERY_CRITERIA_TRUE ((void *) 1) or AFW_QUERY_CRITERIA_FALSE
 * ((void *) 0).  Using this traversal, there will be no "and" or "or"
 * operators.
 *
 * To process tree, recursively processing "and" and "or" operators
 * by following the first_sibling link then the next_sibling link of each
 * entry.
 */
struct afw_query_criteria_s {

    /*
     * This is the origin unparsed query string which may or may not
     * be NULL.  This should only be used for context during debugging
     * and should not be further interpreted.  A query string can
     * be mapped mutiple times, but the origin query string will
     * remain the same.
     */
    const afw_utf8_t *origin_query_string;

    /*
     * @brief _AdaptiveQueryCriteria_ object representing query criteria.
     */
    const afw_object_t *query_object;

    /*
     * @brief Property names to return in objects.
     *
     * This is a NULL terminated list of property name pointers or NULL
     * if there are none.
     */
    const afw_utf8_t * const *select;

    /*
     * @brief Parsed filter.
     * 
     * Use on_true/on_false to traverse to determine if filter is true or
     * false.  No recursion is required.  There are no and/or entries since
     * the compilation process sets on_true/on_false to true (1), false (0)
     * or the address of the next entry to test.  Note that on_true and
     * on_false can both contain false (0) as well as true (1).
     *
     * The value or on_true in an entry is applicable if the comparison is
     * true and on_false if the comparison is false.  An applicable value
     * of true (1) or false (0) is the final result of the filter test.
     */
    const afw_query_criteria_filter_entry_t *filter;

    /*
     * @brief Parse tree that needs to be traversed recursively.
     * 
     * Starting with this entry, use first_conjunctive_child and
     * next_conjunctive_sibling to traverse tree of ands, ors, and
     * comparisons.
     * 
     * Also, from this entry, ->next can be used to traverse the entries
     * in the order they appear in the query.
     */
    const afw_query_criteria_filter_entry_t *tree;

    /*
     * @brief Parsed sort
     */
    const afw_query_criteria_sort_entry_t *first_sort;

    /*
     * @brief Object type or NULL (Internal use)
     */
    const afw_object_type_t *object_type;
};



/**
 * @brief Parse URI encoded query string.
 * @param query_object is an _AdaptiveQueryCriteria_ object.
 * @param object_type to use for adapting values or NULL.
 * @param p to use for result.
 * @param xctx of caller.
 * @return parsed query criteria structure.
 *
 * FIXME these comments are incorrect.  Inspiration came from here.
 *
 * Query string is parsed based on:
 *  Resource Query Language (RQL)
 *
 * which is a superset of:
 *  FIQL: The Feed Item Query Language draft-nottingham-atompub-fiql-00
 *
 * to produce a afw_query_criteria_filter_entry_t.
 *
 */
AFW_DECLARE(const afw_query_criteria_t *)
afw_query_criteria_parse_AdaptiveQueryCriteria_object(
    const afw_object_t *query_object,
    const afw_object_type_t *object_type,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Parse URI encoded RQL query string.
 * @param url_encoded_rql_string URL encoded RQL string.
 * @param object_type to use for adapting values or NULL.
 * @param p to use for result.
 * @param xctx of caller.
 * @return parsed query criteria structure.
 *
 * FIXME these comments are incorrect.  Inspiration came from here.
 *
 * Query string is parsed based on:
 *  Resource Query Language (RQL)
 *
 * which is a superset of:
 *  FIQL: The Feed Item Query Language draft-nottingham-atompub-fiql-00
 *
 * to produce a afw_query_criteria_filter_entry_t.
 *
 */
AFW_DECLARE(const afw_query_criteria_t *)
afw_query_criteria_parse_url_encoded_rql_string(
    const afw_utf8_t *url_encoded_rql_string,
    const afw_object_type_t *object_type,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Test object against query criteria.
 * @param obj Object to be tested.
 * @param criteria Parsed query criteria.
 * @param p to use.
 * @param xctx of caller.
 * @return True if object passes test.
 */
AFW_DECLARE(afw_boolean_t)
afw_query_criteria_test_object(
    const afw_object_t *obj,
    const afw_query_criteria_t *criteria,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Convert query criteria to a _AdaptiveQueryCriteria_ object.
 * @param criteria produced by a afw_query_criteria_parse_*() functions.
 * @param p to use for result.
 * @param xctx of caller.
 * @return _AdaptiveQueryCriteria_ object.
 */
AFW_DECLARE(const afw_object_t *)
afw_query_criteria_to_AdaptiveQueryCriteria_object(
    const afw_query_criteria_t *criteria,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Convert query criteria to query string.
 * @param criteria produced by a afw_query_criteria_parse_*() functions.
 * @param style of query string to produce.
 * @param p to use for result.
 * @param xctx of caller.
 * @return url encoded query string.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_query_criteria_to_query_string(
    const afw_query_criteria_t *criteria,
    afw_query_criteria_style_t style,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_AFW_QUERY_CRITERIA_H__ */

