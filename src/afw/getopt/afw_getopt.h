// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework command-line options
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#ifndef __AFW_GETOPT_H__
#define __AFW_GETOPT_H__

#include "afw_interface.h"
#include <stdio.h>

/**
 * @addtogroup afw_getopt
 * @{
 */

/**
 * @file afw_getopt.h
 * @brief Table-driven argv options for commands (help from the same table).
 *
 * See @ref afw_getopt.
 *
 * One row per flag: long name, short character, has-arg, description.
 * Parse with afw_getopt_long(); print help with afw_getopt_print_options().
 * Callers still switch on the short character. No pool. Stops at the first
 * non-option (usage is `[OPTION]... [IN]`). `--` ends the option list.
 */

AFW_BEGIN_DECLARES

/** afw_getopt_long() found an option. */
#define AFW_GETOPT_OK     0

/** No more options (first non-option or `--` or end of argv). */
#define AFW_GETOPT_EOF    1

/** Unknown option or missing argument. */
#define AFW_GETOPT_ERROR  (-1)

/**
 * @brief One command-line option.
 *
 * `name` is the long name without leading dashes. `optch` is the short
 * character. `has_arg` is true if a value follows. `description` is help.
 */
typedef struct afw_getopt_option_s {
    const char *name;
    int optch;
    afw_boolean_t has_arg;
    const char *description;
} afw_getopt_option_t;

/**
 * @brief One option table row.
 * @param name Long name without leading dashes
 * @param ch Short option character
 * @param has_arg true if the option takes an argument
 * @param description Help text
 */
#define AFW_GETOPT_OPTION(name, ch, has_arg, description) \
    { name, (ch), (has_arg), (description) }

/** Sentinel that ends an option table. */
#define AFW_GETOPT_TABLE_END { NULL, 0, false, NULL }

/**
 * @brief Parse state for afw_getopt_long().
 *
 * After AFW_GETOPT_EOF, `ind` is the index of the first non-option
 * (or argc). No allocation; keep this on the stack.
 */
typedef struct afw_getopt_s {
    int argc;
    const char * const *argv;
    int ind;
    afw_boolean_t in_cluster;
    int cluster_pos;
} afw_getopt_t;

/**
 * @brief Initialize parse state. `ind` starts at 1.
 * @param os State to initialize
 * @param argc From main
 * @param argv From main
 */
AFW_DECLARE(void)
afw_getopt_init(
    afw_getopt_t *os,
    int argc,
    const char * const *argv);

/**
 * @brief Next option from argv.
 * @param os Parse state
 * @param opts Table ended by AFW_GETOPT_TABLE_END
 * @param option_ch Set to the matching short character
 * @param option_arg Set to the argument, or NULL
 * @return AFW_GETOPT_OK, AFW_GETOPT_EOF, or AFW_GETOPT_ERROR
 *
 * Supports `--long`, `--long=arg`, `--long arg`, `-x`, `-xarg`, `-xyz`
 * clustered shorts, and `--` to end options. Does not permute argv.
 */
AFW_DECLARE(int)
afw_getopt_long(
    afw_getopt_t *os,
    const afw_getopt_option_t *opts,
    int *option_ch,
    const char **option_arg);

/**
 * @brief Print each option as ` -c, --long      ARG  description`.
 * @param fd Usually stderr
 * @param opts Table ended by AFW_GETOPT_TABLE_END
 * @return 0, or negative if fprintf failed
 */
AFW_DECLARE(int)
afw_getopt_print_options(
    FILE *fd,
    const afw_getopt_option_t *opts);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_GETOPT_H__ */
