<afwdev {license_c}>

/**
 * @file <afwdev {srcdir}>.c
 * @brief <afwdev {brief}>
 */

/** @todo
 * This is a skeleton command that uses the AFW environment. Much of it
 * will need to change to meet your needs. Some of these possible
 * changes are highlighted with @todo comments.
 */

#include "afw.h"
#include "generated/<afwdev {prefix}>generated.h"
#include "generated/<afwdev {prefix}>version_info.h"
#include <apr_getopt.h>

/* Self typedef for <afwdev {srcdir}> command. */
typedef struct impl_<afwdev {srcdir}>_self_s {
    afw_xctx_t *xctx;
    int index_first_non_option;

    /* Positional parameter IN */
    union {
        afw_utf8_t in;
        const afw_utf8_z_t * in_z;
    };

    /* Conf file path */
    union {
        afw_utf8_t conf;
        const afw_utf8_z_t * conf_z;
    };

    /* Extension to load */
    union {
        afw_utf8_t extension;
        const afw_utf8_z_t * extension_z;
    };

    /* Type <Just an example> */
    union {
        afw_utf8_t type_in;
        const afw_utf8_z_t *type_in_z;
    };

    const afw_content_type_t *content_type;

    afw_boolean_t help_option;

    /** @todo Add/remove variables to meet your needs.*/

} impl_<afwdev {srcdir}>_self_t;


/* Command line options. */
static const apr_getopt_option_t
impl_options[] = {
    /** @todo Add/remove arguments to meet your needs. */
    /* long-option, short-option, has-arg flag, description. */
    { "conf", 'f', TRUE, "Configuration file." },
    { "extension", 'e', TRUE, "Extension to load." },
    { "help", 'h', FALSE, "Print this help and exit successfully." },
    { "type", 't', TRUE, "Type <Just an example>." },
    { "version", 'v', FALSE, "Print version and exit successfully." },
    { NULL, 0, 0, NULL }
};


/* Print usage. */
static void
impl_print_usage(void)
{
    const apr_getopt_option_t *opt;

    /** @todo Fill in the correct usage and description HERE to match your command. */

    fprintf(stderr, "Usage: <afwdev {srcdir}> HERE Example [OPTION]... IN\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "HERE Add description.\n");
    fprintf(stderr, "\n");

    fprintf(stderr, "OPTION:\n");
    opt = &impl_options[0];
    while (opt->name) {
        fprintf(stderr, " -%c, --%-10s %s %s\n",
            opt->optch,
            opt->name,
            (opt->has_arg) ? " ARG " : "     ",
            opt->description);
        opt++;
    }
    fprintf(stderr, "\n");

    fprintf(stderr, "HERE Add more documentation.\n");
}


/* Process args. */
static int
impl_process_options(
    impl_<afwdev {srcdir}>_self_t *self,
    int argc, const char * const *argv,
    afw_xctx_t *xctx)
{
    apr_getopt_t *os;
    int option_ch;
    const char * option_arg;
    int rv;

    /* Parse parameters. */
    if ((apr_getopt_init(&os, afw_pool_get_apr_pool(xctx->p), argc, argv))
        != APR_SUCCESS)
    {
        fprintf(xctx->env->stderr_fd, "apr_getopt_init() error.\n");
        return EXIT_FAILURE;
    }

    self->index_first_non_option = 1;
    while ((rv = apr_getopt_long(os, impl_options, &option_ch, &option_arg))
        == APR_SUCCESS)
    {
        self->index_first_non_option = os->ind;
        switch (option_ch) {

        /** @todo Add/remove cases for each new option. */

        /* Extension file */
        case 'e':
            self->extension_z = option_arg;
            self->extension.len = strlen(self->extension_z);
            break;

        /* Conf file */
        case 'f':
            self->conf_z = option_arg;
            self->conf.len = strlen(self->conf_z);
            break;

        /* Type in */
        case 't':
            self->type_in_z = option_arg;
            self->type_in.len = strlen(self->type_in_z);
            break;

        /* Version */
        case 'v':
            rv = fprintf(stderr, "%s\n", <afwdev {prefix.upper()}>VERSION_COMMAND_STRING);
            if (rv < 0) exit(EXIT_FAILURE);
            return EXIT_SUCCESS;

        /* Help */
        case 'h':
            self->help_option = true;
            impl_print_usage();
            return EXIT_SUCCESS;

        default:
            rv = fprintf(stderr, "Error: Invalid command line parameter.\n");
            if (rv < 0) exit(EXIT_FAILURE);
            impl_print_usage();
            return EXIT_FAILURE;
        }
    }

    /* Error if apr_getopt_long() returns other than success. */
    if (rv != APR_EOF) {
        fprintf(xctx->env->stderr_fd, "Try --help.\n");
        return EXIT_FAILURE;
    }

    /* Default type to a relaxed json syntax. */
    if (!self->type_in_z) {
        self->type_in.s = "json";
    }
    self->type_in.len = strlen(self->type_in.s);

    /** @todo Adjust for number of positional parameters. */
    /* Can have no more than 1 positional. */
    if (argc > self->index_first_non_option + 1) {
        rv = fprintf(stderr, "Error: Can not specify more than one positional parameter.\n");
        if (rv < 0) exit(EXIT_FAILURE);
        impl_print_usage();
        return EXIT_FAILURE;
    }

    /* Set input if [IN] specified. */
    if (argc == self->index_first_non_option + 1) {
        self->in_z = argv[self->index_first_non_option];
        self->in.len = strlen(self->in_z);
    }

    /* Normal return. */
    return EXIT_SUCCESS;
}


void
impl_print_error(impl_<afwdev {srcdir}>_self_t *self, 
    const afw_error_t *error, afw_xctx_t *xctx)
{
    int rv;

    rv = fprintf(xctx->env->stderr_fd, "\n--- Error ---\n");
    if (rv < 0) exit(EXIT_FAILURE);
    rv = afw_error_print_with_xctx(xctx->env->stderr_fd, error,
        xctx->p, xctx);
    if (rv < 0) exit(EXIT_FAILURE);
    rv = fprintf(xctx->env->stderr_fd, "\n");
    if (rv < 0) exit(EXIT_FAILURE);
    rv = fflush(xctx->env->stderr_fd);
    if (rv < 0) exit(EXIT_FAILURE);
}


int main(int argc, const char * const *argv)
{
    impl_<afwdev {srcdir}>_self_t *self;
    const afw_error_t *create_error;
    afw_xctx_t *xctx;
    const afw_memory_t *conf_file;
    const afw_value_t *conf;
    int rv;

    /* Create Adaptive Framework environment for command. */
    AFW_ENVIRONMENT_CREATE(xctx, argc, argv, &create_error);
    if (!xctx) {
        afw_error_print(stderr, create_error);
        return EXIT_FAILURE;
    }

    /** @todo Remove if you don't want afw environment stdout to go to stderr */
    /* stdout except for result goes to stderr.*/
    afw_environment_set_stdout_fd(stderr, xctx);

    /* Generated register. */
    <afwdev {prefix}>generated_register(xctx);

    /* Make sure environment is cleaned up. */
    /* IMPORTANT Inside AFW_TRY use break or AFW_THROW_* instead of return. */
    self = NULL;
    AFW_TRY {

        /* Allocate and initialize self. */
        self = afw_xctx_calloc_type(impl_<afwdev {srcdir}>_self_t, xctx);
        self->xctx = xctx;

        /* Process arguments. */
        rv = impl_process_options(self, argc, argv, xctx);
        if (self->help_option || rv != EXIT_SUCCESS) {
            break;
        }

        /* If extension specified, load it. */
        if (self->extension.len > 0) {
            afw_environment_load_extension(&self->extension,
                NULL, NULL, xctx);
        }

        /* If conf specified, process it. */
        if (self->conf.len > 0) {

            /* Get content type for conf. */
            self->content_type = afw_environment_get_content_type(
                &self->type_in, xctx);
            if (!self->content_type) {
                AFW_THROW_ERROR_Z(general, "Invalid content-type.", xctx);
            }

            /* Load conf as a list of conf objects. */
            conf_file = afw_file_to_memory(&self->conf, 0, xctx->p, xctx);
            conf = afw_content_type_raw_to_value(self->content_type,
                conf_file, &self->conf, xctx->p, xctx);
            if (!afw_value_is_array(conf)) {
                AFW_THROW_ERROR_Z(general, "Invalid configuration.",
                    xctx);
            }
            afw_environment_configure_with_object_list(
                ((const afw_value_array_t *)conf)->internal,
                &self->conf, xctx);
        }

        /** @todo Add your command's code here. */
        fprintf(stdout, "Processing input %s\n", self->in_z);
    }


    /* Print any unhandled errors. */
    AFW_CATCH_UNHANDLED{
        impl_print_error(self, AFW_ERROR_THROWN, xctx);
        rv = EXIT_FAILURE;
    }

    /* Make sure things are cleaned up. */
    AFW_FINALLY{

        /** @todo Do cleanup.  For example close files */
        /* If [IN] specified and file open, close it.
        if (self && self->fd_input && self->in_z) {
            fclose(self->fd_input);
        }
        */

        /* Release enviornment. */
        afw_environment_release(xctx);

        /* Special case: xctx is gone, so return before AFW_ENDTRY. */
        return rv;
    }

    AFW_ENDTRY;
}
