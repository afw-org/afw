// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework command-line options
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

/**
 * @file afw_getopt.c
 * @brief Table-driven argv walker; no pool.
 */

#include "afw_internal.h"


static const afw_getopt_option_t *
impl_find_long(
    const afw_getopt_option_t *opts,
    const char *name,
    afw_size_t len)
{
    const afw_getopt_option_t *opt;

    for (opt = opts; opt->name; opt++) {
        if (strlen(opt->name) == len &&
            memcmp(opt->name, name, len) == 0)
        {
            return opt;
        }
    }
    return NULL;
}


static const afw_getopt_option_t *
impl_find_short(const afw_getopt_option_t *opts, int ch)
{
    const afw_getopt_option_t *opt;

    for (opt = opts; opt->name; opt++) {
        if (opt->optch == ch) {
            return opt;
        }
    }
    return NULL;
}


AFW_DEFINE(void)
afw_getopt_init(
    afw_getopt_t *os,
    int argc,
    const char * const *argv)
{
    os->argc = argc;
    os->argv = argv;
    os->ind = 1;
    os->in_cluster = false;
    os->cluster_pos = 0;
}


AFW_DEFINE(int)
afw_getopt_long(
    afw_getopt_t *os,
    const afw_getopt_option_t *opts,
    int *option_ch,
    const char **option_arg)
{
    const char *a;
    const char *eq;
    const afw_getopt_option_t *opt;
    int ch;
    afw_size_t n;

    *option_arg = NULL;

    for (;;) {
        if (os->ind >= os->argc) {
            return AFW_GETOPT_EOF;
        }
        a = os->argv[os->ind];
        if (!a) {
            return AFW_GETOPT_EOF;
        }

        if (os->in_cluster) {
            ch = (unsigned char)a[os->cluster_pos];
            if (ch == 0) {
                os->in_cluster = false;
                os->cluster_pos = 0;
                os->ind++;
                continue;
            }
            opt = impl_find_short(opts, ch);
            if (!opt) {
                return AFW_GETOPT_ERROR;
            }
            *option_ch = opt->optch;
            if (opt->has_arg) {
                if (a[os->cluster_pos + 1] != 0) {
                    *option_arg = a + os->cluster_pos + 1;
                    os->in_cluster = false;
                    os->cluster_pos = 0;
                    os->ind++;
                } else {
                    os->in_cluster = false;
                    os->cluster_pos = 0;
                    os->ind++;
                    if (os->ind >= os->argc) {
                        return AFW_GETOPT_ERROR;
                    }
                    *option_arg = os->argv[os->ind];
                    os->ind++;
                }
                return AFW_GETOPT_OK;
            }
            os->cluster_pos++;
            return AFW_GETOPT_OK;
        }

        if (a[0] != '-' || a[1] == 0) {
            return AFW_GETOPT_EOF;
        }

        if (a[1] == '-') {
            if (a[2] == 0) {
                os->ind++;
                return AFW_GETOPT_EOF;
            }
            eq = strchr(a + 2, '=');
            if (eq) {
                n = (afw_size_t)(eq - (a + 2));
                opt = impl_find_long(opts, a + 2, n);
                if (!opt || !opt->has_arg) {
                    return AFW_GETOPT_ERROR;
                }
                *option_ch = opt->optch;
                *option_arg = eq + 1;
                os->ind++;
                return AFW_GETOPT_OK;
            }
            opt = impl_find_long(opts, a + 2, strlen(a + 2));
            if (!opt) {
                return AFW_GETOPT_ERROR;
            }
            *option_ch = opt->optch;
            os->ind++;
            if (opt->has_arg) {
                if (os->ind >= os->argc) {
                    return AFW_GETOPT_ERROR;
                }
                *option_arg = os->argv[os->ind];
                os->ind++;
            }
            return AFW_GETOPT_OK;
        }

        os->in_cluster = true;
        os->cluster_pos = 1;
    }
}


AFW_DEFINE(int)
afw_getopt_print_options(
    FILE *fd,
    const afw_getopt_option_t *opts)
{
    const afw_getopt_option_t *opt;
    int rv;

    for (opt = opts; opt->name; opt++) {
        rv = fprintf(fd, " -%c, --%-10s %s %s\n",
            opt->optch,
            opt->name,
            opt->has_arg ? " ARG " : "     ",
            opt->description);
        if (rv < 0) {
            return rv;
        }
    }
    return 0;
}
