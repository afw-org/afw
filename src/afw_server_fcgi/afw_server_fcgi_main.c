// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Frame FCGI Server
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_server_fcgi_main.c
 * @brief Adaptive Frame FastCGI Server
 */

#include "afw.h"
#include "afw_server_fcgi_internal.h"
#include "generated/afw_server_fcgi_generated.h"
#include "generated/afw_server_fcgi_version_info.h"

typedef struct self_args_s {
    union {
        afw_utf8_t conf_file;
        const afw_utf8_z_t *conf_file_z;
    };
    union {
        afw_utf8_t path;
        const afw_utf8_z_t *path_z;
    };
    union {
        afw_utf8_t type_in;
        const afw_utf8_z_t *type_in_z;
    };
    const afw_content_type_t *content_type_in;

    const afw_utf8_t *in_content;

    afw_integer_t number_of_threads;
    afw_boolean_t help_option;

} self_args_t;


/* Print usage. */
static void print_usage(void)
{
    fprintf(stderr,
        "Usage: afw_server_fcgi_main [-f filename] [-p path] [-n threads] [-e extension] "
        "[-t content-type] [--help] [--version]\n");
}


/* Process args. */
static int process_args(int argc, const char * const * argv, self_args_t *args, afw_xctx_t *xctx)
{
    int i;
    long int n;
    
    /* Clear args. */
    memset(args, 0, sizeof(self_args_t));

    args->number_of_threads = 1;

    /* Loop though argv. */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            i++;
            if (args->conf_file_z || i >= argc) {
                print_usage();
                goto error;
            }
            args->conf_file_z = argv[i];
        }
        else if (strcmp(argv[i], "-p") == 0) {
            i++;
            if (args->path_z || i >= argc) {
                print_usage();
                goto error;
            }
            /** @fixme Remove quotes if there. */
            args->path_z = argv[i];
        }
        else if (strcmp(argv[i], "-n") == 0) {
            i++;
            n = strtol(argv[i], NULL, 10);
            if (n < 0 || n > 9999 || errno == ERANGE) {
                print_usage();
                goto error;
            }
            args->number_of_threads = n;
        }
        else if (strcmp(argv[i], "--help") == 0) {
            i++;
            print_usage();
            args->help_option = true;
            return EXIT_SUCCESS;
        }
        else if (strcmp(argv[i], "--version") == 0) {
            i++;
            fprintf(stderr,
                AFW_SERVER_FCGI_VERSION_COMMAND_STRING "\n");
            args->help_option = true;
            return EXIT_SUCCESS;
        }
        else if (strcmp(argv[i], "-t") == 0) {
            i++;
            if (i >= argc) {
                print_usage();
                goto error;
            }
            args->type_in_z = argv[i];
        }
        else if (strcmp(argv[i], "-e") == 0) {
            i++;
            if (i >= argc) {
                print_usage();
                goto error;
            }
            afw_environment_load_extension(
                afw_utf8_create(argv[i], AFW_UTF8_Z_LEN, xctx->p, xctx),
                NULL, NULL, xctx);
        }
        else {
            print_usage();
            goto error;
        }
    }

    /* If config file was not specified, set it to a default. */
    if (!args->conf_file_z) {
        args->conf_file_z = "afw.conf";
    }

    /* If content-type was not specified, set it to a default. */
    if (!args->type_in_z) {
        args->type_in_z = "json";
    }
    args->type_in.len = strlen(args->type_in_z);

    /* If path was not specified, set it to a default. */
    if (!args->path_z) {
        args->path_z = ":9345";
    }

    /* Set string lengths. */
    args->conf_file.len = strlen(args->conf_file_z);
    args->path.len = strlen(args->path_z);

    /* Normal return. */
    return EXIT_SUCCESS;

    /* Error return. */
error:
    return EXIT_FAILURE;

}

/* Main for afw_server_fcgi. */
int main(int argc, const char * const * argv) {
    const afw_server_t *server;
    const afw_error_t *create_error;
    const afw_request_handler_t *director;
    afw_xctx_t *xctx;
    self_args_t args;
    int rv;
    const afw_value_t *conf;
    const afw_memory_t *conf_file;

    /* Create Adaptive Framework environment for server. */
    AFW_ENVIRONMENT_CREATE(xctx, argc, argv, &create_error);
    if (!xctx) {
        afw_error_print(stderr, create_error);
        return EXIT_FAILURE;
    }

    /* Make sure environment is cleaned up. */
    AFW_TRY {

        /* Register generated. */
        afw_server_fcgi_generated_register(xctx);

        /* Process arguments. */
        rv = process_args(argc, argv, &args, xctx);
        if (args.help_option || rv != EXIT_SUCCESS) {
            break;
        }

        /* Configure environment. */
        conf_file = afw_file_to_memory(&args.conf_file, 0, xctx->p, xctx);

        args.content_type_in = afw_environment_get_content_type(
            &args.type_in, xctx);
        if (!args.content_type_in) {
            AFW_THROW_ERROR_Z(general, "Invalid content-type.", xctx);
        }

        conf = afw_content_type_raw_to_value(args.content_type_in,
            conf_file, &args.conf_file, xctx->p, xctx);
        if (!afw_value_is_array(conf)) {
            AFW_THROW_ERROR_Z(general, "Invalid configuration.", xctx);
        }
        afw_environment_configure_with_object_list(
            ((const afw_value_array_t *)conf)->internal,
            &args.conf_file, xctx);

        /* Create server. */
        server = afw_server_fcgi_internal_create(args.path_z,
            args.number_of_threads, xctx);

        /* Create director. */
        director = afw_request_handler_director_create(server,
            args.conf_file_z, xctx->p, xctx);

        /* Run server. */
        afw_server_run(server, director, xctx);
    }

    /* Print any unhandled errors. */
    AFW_CATCH_UNHANDLED{
        afw_error_print(xctx->env->stderr_fd, AFW_ERROR_THROWN);
        rv = EXIT_FAILURE;
    }
    
    /* Make sure things are cleaned up. */
    AFW_FINALLY{
        afw_environment_release(xctx);

        /* Special case: xctx is gone, so return before AFW_ENDTRY. */
        return rv;
    }

    AFW_ENDTRY;
}
