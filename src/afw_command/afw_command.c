// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW - Adaptive Framework afw Command
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#include "afw.h"
#include "afw_command_internal.h"
#include "afw_command_local_server.h"
#include <apr_file_io.h>
#include <apr_getopt.h>
#include <stdio.h>

/**
 * @addtogroup afw_command_internal
 * @{
 */

/**
 * @file afw_command.c
 * @brief Adaptive Framework command line interface.
 *
 * This is internal documentation and thus is not API documentation. Anything
 * can change between releases and even between patches, so do not use outside
 * of the source for this command.
 */

static void
impl_print_result(afw_command_self_t *self, const char *format, ...);

static void
impl_print_result_value(afw_command_self_t *self, const afw_value_t *value);

static void
impl_print_error(afw_command_self_t *self, 
    const afw_error_t *error, afw_xctx_t *xctx);

static void
impl_print_end(afw_command_self_t *self);

static int
impl_octet_get_cb(afw_utf8_octet_t *octet, void *data, afw_xctx_t *xctx);


/* Command line options. */
static const apr_getopt_option_t opts[] = {
    /* long-option, short-option, has-arg flag, description. */
    { "allow", 'a', TRUE, "Content type used to output adaptive values." },
    { "check", 'k', FALSE, "Parse but don't evaluate." },
    { "conf", 'f', TRUE, "Configuration file." },
    { "expression", 'x', TRUE, "The first string to evaluate." },
    { "extension", 'e', TRUE, "Load extension." },
    { "help", 'h', FALSE, "Print this help and exit successfully." },
    { "local", 'l', TRUE, "Run in \"local\" mode with output to path or fd number." },
    { "syntax", 's', TRUE, "expression, hybrid, parenthesized, script, template, test_script" },
    { "type", 't', TRUE, "Content type of configuration file." },
    { "version", 'v', FALSE, "Print version and exit successfully." },
    { NULL, 0, 0, NULL }
};

/** @fixme Figure out local and interactive for -s s. */
static const char * impl_additional_help_text =
    "\n"
    "The syntax option (-s) determines how input will be parsed:\n"
    "\n"
    "  -s expression        - The input is an adaptive expression.\n"
    "  -s script            - The input is an adaptive script.\n"
    "  -s template          - The input is an adaptive template.\n"
    "  -s test_script       - The input is an adaptive test script.\n"
    "\n"
    "A line ending with \"\\\" will be combined with the line that follows\n"
    "to form one logical line.\n"
    "\n"
    "When option -s expression or -s template is specified, the input is\n"
    "evaluated one logical line at a time.\n"
    "\n"
    "When option -s parentheses is specified, logical lines are evaluated up\n"
    "up to the expression's closing parenthesis (')').  The parenthesized\n"
    "expression can be preceded by any amount of whitespace, including\n"
    "comments, and followed by whitespace up to the end of the line.\n"
    "\n"
    "When option -s hybrid is specified and the input does not begin with a\n"
    "square bracket ('['), the input must be on one logical line; otherwise,\n"
    "input will be evaluated through the logical line with the hybrid's\n"
    "closing square bracket (']').\n"
    "\n"
    "The conf option (-f) is used to specify the path to a configuration\n"
    "file.  By default, this file should be encoded with a relaxed json\n"
    "syntax, which accepts standard json, plus allows block comments\n"
    "(/* ... */), line comments (//), unquoted property names, and several\n"
    "other conveniences.\n"
    /** @fixme Point to somewhere that describes content type and config file.*/
    "\n"
    "The type option (-t) can be used to specify a content type other than the\n"
    "default.  If the content type is not part of AFW core, use the extension\n"
    "option (-e) to load the appropriate extension.\n"
    "\n"
    "If the -f option is not specified, only core AFW functionality is\n"
    "available.\n"
    "\n"
    "The optional adaptive expression specified by expression option (-x)\n"
    "is evaluated first.  If -x is specified and [IN] is not specified,\n"
    "afw will end after evaluating the expression.\n"
    "\n"
    "If neither -x or [IN] is specified, input is read from stdin.\n"
    "Each line read from stdin is evaluated as an adaptive expressions\n"
    "until either a line containing only \"exit\" or end of file is\n"
    "encountered.\n"
    "\n"
    "If [IN] is specified but -s is not, the file is evaluated as an\n"
    "adaptive script.\n"
    "\n"
    "If the -l option is specified, output is written to the path or fd\n"
    "number specified.  Input is always read from stdin so [IN] cannot be\n"
    "specified.\n"
    "\n"
    "When the -l option is specified, input must be, and output will be made\n"
    "up of one or more segments that are in turn made up of one or more\n"
    "chunks.  Each chunk is preceded with a decimal length of the chunk\n"
    "followed by a '\\n'.  A line containing only \"0\\n\" is used to\n"
    "indicate the end of a segment.  The last input segment must be:\n"
    "4\\nexit0\\n.\n"
    "\n"
    "See the afw command documentation for information about what can be\n"
    "contained in each segment.\n"
    "\n"
    "A simple example using default directives to add 2 plus 2:\n"
    "\n"
    "  8\\nadd(2,2)0\\n4\\nexit0\\n\n"
    "\n"
    "The output will be:\n"
    "\n"
    "  1\\n40\\n\n"
    "\n"
;


int
impl_octet_get_cb(afw_utf8_octet_t *octet, void *data, afw_xctx_t *xctx)
{
    afw_command_self_t *self = data;
    int c;

    /* Get an octet. */
    c = fgetc(self->fd_input);

    /* If not an error, return octet with rv = 0. */
    if (c >= 0) {
        *octet = (afw_utf8_octet_t)c;
        return 0;
    }

    /* If there is an error, return 0 for octet and rv = -1. */
    *octet = 0;
    return -1;
}



/* Get input. */
static afw_utf8_t *
impl_get_input(afw_command_self_t *self, afw_xctx_t *xctx)
{
    int c;
    char prev_c;

    /* If eof, just return NULL. */
    if (self->eof) return NULL;

    /* Allocate array for input. */
    if (!self->input_buffer) {
        self->input_buffer = apr_array_make(
            afw_pool_get_apr_pool(self->xctx->p),
            2000, 1);
    }

    /* Read input up to a \n that is not preceded by a \ */
    apr_array_clear(self->input_buffer);
    for (prev_c = 0; ; prev_c = c) {
        c = fgetc(self->fd_input);
        if (c == EOF) {
            self->eof = true;
            break;
        }
        if (c == '\n' && prev_c == '\\') {
            apr_array_pop(self->input_buffer);
            continue;
        }
        APR_ARRAY_PUSH(self->input_buffer, unsigned char) = c;
        if (c == '\n') break;
    }

    /* Return resulting string making sure it is NFC utf8. */
    return (self->input_buffer->nelts == 0) 
        ? NULL 
        : (afw_utf8_t *)afw_utf8_create(self->input_buffer->elts,
            self->input_buffer->nelts, xctx->p, xctx);
}


/*
 * If exit_code is not NULL, the exit_code is returned instead of printing
 * output.
 */
static afw_boolean_t
impl_evaluate(
    afw_command_self_t *self,
    const afw_utf8_t *input,
    int *exit_code)
{
    const afw_value_t *value;
    const afw_value_t *evaluated_value;
    afw_xctx_t *xctx;
    afw_boolean_t error_occurred;
    afw_boolean_t keep_going;
    afw_utf8_t *line;


    xctx = afw_xctx_create(&afw_command_s_afw_command, 0, self->xctx);
    error_occurred = false;
    keep_going = true;
    if (exit_code) {
        *exit_code = 0;
    }
    AFW_TRY {
       
        /* Set environment object and qualifier in new xctx. */
        afw_runtime_xctx_set_object(self->environment_variables_object,
            true, xctx);
        afw_xctx_qualifier_stack_qualifier_object_push(&afw_s_environment,
            self->environment_variables_object, true, xctx->p, xctx);        

        /* If input is not NULL, insure it is NFC normalized utf-8. */
        if (input) {
            input = afw_utf8_create(input->s, input->len, xctx->p, xctx);
        }

        /* If input is NULL, get a non-empty input or break if eof. */
        else if (!self->read_full) {
            for (;;) {
                line = impl_get_input(self, xctx);
                if (!line) {
                    keep_going = false;
                    break;
                }
                if (line->len != 0 && !afw_utf8_equal_utf8_z(line, "\n")) {
                    /* If this is a template, remove trailing \n. */
                    if (self->compile_option == afw_compile_type_template)
                    {
                        if (line->s[line->len - 1] == '\n') {
                            line->len--;
                        }
                    }
                    input = line;
                    break;
                }
                impl_print_end(self);
            }
        }
        if (!keep_going) break;

        /* If input is "exit", just set keep_going to false and break. */
        if (input) {
            if (afw_utf8_equal_utf8_z(input, "exit\n") ||
                afw_utf8_equal_utf8_z(input, "exit"))
            {
                keep_going = false;
                break;
            }
        }

        /* Compile eval and evaluate if not check mode. */
        value = afw_compile_to_value_with_callback(input,
            self->callback, self->callback_data,
            self->source_location, self->compile_option,
            self->residual_check,
            NULL, NULL, xctx->p, xctx);

        /*
         * If option is parenthesized_expression, NULL can be returned if
         * everything remaining is whitespace/comments.  In this case, set
         * keep_going to false and break.
         */
        if (!value) {
            keep_going = false;
            break;
        }

        /* If not check mode, evaluate expression and print or return result. */
        if (!self->check_mode) {
            evaluated_value = afw_value_evaluate(value, xctx->p, xctx);

            /* If requested, determine exit code. */
            if (exit_code) {
                if (!afw_value_is_null(evaluated_value)) {
                    if (!afw_value_is_integer(evaluated_value) ||
                        ((const afw_value_integer_t *)evaluated_value)
                        ->internal < 0 ||
                        ((const afw_value_integer_t *)evaluated_value)
                        ->internal > 255)
                    {
                        *exit_code = EXIT_FAILURE;
                        fprintf(xctx->env->stderr_fd,
                            "Adaptive script run from shell must return "
                            "null or an integer between 0 and 255 but returned "
                            "this instead:\n");
                        impl_print_result_value(self, evaluated_value);
                    }
                    else {
                        *exit_code = (int)
                            ((const afw_value_integer_t *)evaluated_value)
                            ->internal;
                    }
                }
            }

            /* If not requesting exit code, print result. */
            else {               
                impl_print_result_value(self, evaluated_value);
            }
        }
    }

    AFW_CATCH_UNHANDLED{
        impl_print_error(self, AFW_ERROR_THROWN, xctx);
        error_occurred = true;
    }

    AFW_FINALLY{
        afw_adaptor_session_commit_and_release_cache(error_occurred, xctx);
        afw_xctx_release(xctx, xctx);
        /* Special case: xctx is gone, so return before AFW_ENDTRY. */
        if (keep_going) impl_print_end(self);
        return keep_going && !self->terminal_error;
    }

    AFW_ENDTRY;

    /* Fix warning. Should not get to here. See Special case comment above. */
    return false;
}



/* Print usage. */
static void
print_usage(void)
{
    const apr_getopt_option_t *opt;
    int rv;

    rv = fprintf(stderr, "Usage: afw [OPTION]... [IN]\n\n");
    if (rv < 0) exit(EXIT_FAILURE);

    rv = fprintf(stderr, "\n");
    if (rv < 0) exit(EXIT_FAILURE);

    rv = fprintf(stderr, "OPTION:\n");
    if (rv < 0) exit(EXIT_FAILURE);

    opt = &opts[0];
    while (opt->name) {
        rv = fprintf(stderr, " -%c, --%-10s %s %s\n",
            opt->optch,
            opt->name,
            (opt->has_arg) ? " ARG " : "     ",
            opt->description);
        if (rv < 0) exit(EXIT_FAILURE);
        opt++;
    }

    rv = fprintf(stderr, "%s", impl_additional_help_text);
    if (rv < 0) exit(EXIT_FAILURE);

}


static int
process_args_getopt(afw_command_self_t *self, int argc, const char * const *argv,
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

    self->compile_option = afw_compile_type_error;
    self->residual_check = afw_compile_residual_check_to_full;
    self->index_first_non_option = 1;
    while ((rv = apr_getopt_long(os, opts, &option_ch, &option_arg))
        == APR_SUCCESS)
    {
        self->index_first_non_option = os->ind;
        switch (option_ch) {

        case 'a':
            self->type_out_z = option_arg;
            self->type_out.len = strlen(self->type_out_z);
            break;

        case 'f':
            self->conf_z = option_arg;
            self->conf.len = strlen(self->conf_z);
            break;

        case 'e':
            self->extension_z = option_arg;
            self->extension.len = strlen(self->extension_z);
            break;

        case 'k':
            self->check_mode = true;
            break;

        case 'l':
            self->local_mode_z = option_arg;
            self->local_mode_out.len = strlen(self->local_mode_z);
            break;

        case 's':

            if (afw_utf8_z_equal(option_arg, "expression"))
            {
                self->compile_option = afw_compile_type_script;
                self->can_span_lines = false;
            }

            else if (afw_utf8_z_equal(option_arg, "script"))
            {
                self->compile_option = afw_compile_type_script;
                self->can_span_lines = false;
            }

            else if (afw_utf8_z_equal(option_arg, "template"))
            {
                self->compile_option = afw_compile_type_template;
                self->can_span_lines = false;
            }

            else if (afw_utf8_z_equal(option_arg, "test_script"))
            {
                self->compile_option = afw_compile_type_test_script;
                self->can_span_lines = false;
            }
            else {
                rv = fprintf(stderr, "Invalid option --syntax %s\n", option_arg);
                if (rv < 0) exit(EXIT_FAILURE);
                print_usage();
                return EXIT_FAILURE;
            }

            break;

        case 't':
            self->type_in_z = option_arg;
            self->type_in.len = strlen(self->type_in_z);
            break;

        case 'v':
            rv = fprintf(stderr, "%s\n", AFW_COMMAND_VERSION_COMMAND_STRING);
            if (rv < 0) exit(EXIT_FAILURE);
            self->help_option = true;
            return EXIT_SUCCESS;

        case 'x':
            self->expression_z = option_arg;
            self->expression.len = strlen(self->expression_z);
            break;

        case 'h':
            self->help_option = true;
            print_usage();
            return EXIT_SUCCESS;

        default:
            rv = fprintf(stderr, "Error: Invalid command line parameter.\n");
            if (rv < 0) exit(EXIT_FAILURE);
            print_usage();
            return EXIT_FAILURE;
        }
    }

    /* Error if apr_getopt_long() returns other than success. */
    if (rv != APR_EOF) {
        fprintf(xctx->env->stderr_fd, "Try --help.\n");
        return EXIT_FAILURE;
    }

    /* Normal return. */
    return EXIT_SUCCESS;
}


static int
process_args(afw_command_self_t *self, int argc, const char * const *argv,
    afw_xctx_t *xctx)
{
    int rv;

    /* Process args with the ones from command line. */
    rv = process_args_getopt(self, argc, argv, xctx);
    if (rv != EXIT_SUCCESS) {
        return rv;
    }

    /* Default input type to a relaxed json syntax. */
    if (!self->type_in_z) {
        self->type_in.s = "json";
    }
    self->type_in.len = strlen(self->type_in.s);

    /* Default output type to json syntax. */
    if (!self->type_out_z) {
        self->type_out.s = "json";
    }
    self->type_out.len = strlen(self->type_out.s);

    /* Can have no more than 1 positional. */
    if (argc > self->index_first_non_option + 1) {
        rv = fprintf(stderr, "Error: Can not specify more than one positional parameter.\n");
        if (rv < 0) exit(EXIT_FAILURE);
        print_usage();
        goto error;
    }

    /* Set input if [IN] specified. */
    if (argc == self->index_first_non_option + 1) {
        self->in_z = argv[self->index_first_non_option];
        self->in.len = strlen(self->in_z);
    }

    /* Can not specify both -l and [IN] */
    if (self->in_z && self->local_mode_z) {
        rv = fprintf(stderr, "Error: Can not specify [IN] if -l specified.\n");
        if (rv < 0) exit(EXIT_FAILURE);
        print_usage();
        goto error;
    }

    /* Default -s is script. If no [IN], set can_span_lines to false. */
    if (self->compile_option == afw_compile_type_error)
    {
        self->compile_option = afw_compile_type_script;
        if (!self->in_z) {
            self->can_span_lines = false;
        }
    }

    /* Interactive mode if not -x or [IN]. */
    if (!self->local_mode_z && !self->expression_z && !self->in_z) {
        self->interactive_mode = true;
    }

    /* Normal return. */
    return EXIT_SUCCESS;

    /* Error return. */
error:
    return EXIT_FAILURE;
}


/* Print result followed by a \n */
void
impl_print_result(afw_command_self_t *self, const char *format, ...)
{
    va_list ap;
    int rv;

    va_start (ap, format);
    rv = vfprintf(self->fd_output, format, ap);
    va_end(ap);
    if (rv < 0) exit(EXIT_FAILURE);

    rv = fprintf(self->fd_output, "\n");
    if (rv < 0) exit(EXIT_FAILURE);
    rv = fflush(self->fd_output);
    if (rv < 0) exit(EXIT_FAILURE);
}



/* Print result value. */
void impl_print_result_value(afw_command_self_t *self, const afw_value_t *value)
{
    const afw_memory_t *raw;
    const afw_utf8_t *string;

    /* JSON does not support undefined values. */
    if (afw_value_is_undefined(value))
    {
        impl_print_result(self,
            AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(&afw_s_a_undefined_as_string));
    }

    /* If value is defined and evaluated, use output content type.*/
    else if (afw_value_is_defined_and_evaluated(value))
    {
        raw = afw_content_type_value_to_raw(
            self->content_type_out,
            value,
            &afw_object_options_whitespace,
            self->xctx->p, self->xctx);
        if (raw)
        {
            impl_print_result(self, "%.*s",
                (int)(raw)->size, (const char *)(raw)->ptr);
        }
    }

    /* If defined but not evaluated, return decompiled value. */
    else
    {
        string = afw_value_decompile_to_string(value, NULL,
                                                self->xctx->p, self->xctx);
        impl_print_result(self,
            "<unevaluated> " AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(string));
    }
}


void
impl_print_error(afw_command_self_t *self, 
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


void
impl_print_end(afw_command_self_t *self)
{
    /* Nothing to do. */
}

static const afw_utf8_t impl_response_error_flag =
    AFW_UTF8_LITERAL("response:error");

int
main(int argc, const char * const *argv) {
    afw_command_self_t *self;
    const afw_error_t *create_error ;
    afw_xctx_t *xctx;
    int rv;
    const afw_memory_t *conf_file;
    const afw_value_t *conf;
    const char *s;

    /* Create Adaptive Framework environment for command. */
    AFW_ENVIRONMENT_CREATE(xctx, argc, argv, &create_error);
    if (!xctx) {
        afw_error_print(xctx->env->stderr_fd, create_error);
        return EXIT_FAILURE;
    }

    /* stdout except for result goes to stderr.*/
    afw_environment_set_stdout_fd(stderr, xctx);

    /* Generated register. */
    afw_command_generated_register(xctx);

    /*
     * Since the caller of the afw command has to have access to all resources
     * anyways to run the afw command, default to receiving additional error
     * info that might be sensitive.
     */
    afw_flag_set_default(&impl_response_error_flag, true, xctx);

    /* Make sure environment is cleaned up. */
    self = NULL;
    AFW_TRY{

        /* Allocate and initialize self. */
        self = afw_xctx_calloc_type(afw_command_self_t, xctx);
        self->xctx = xctx;
        self->fd_input = stdin;
        self->fd_output = stdout;
        self->source_location = &afw_command_s_afw_command;

        /* Process arguments. */
        rv = process_args(self, argc, argv, xctx);
        if (self->help_option || rv != EXIT_SUCCESS) {
            break;
        }

        /* Create and set environment object and qualifier. */
        self->environment_variables_object =
            afw_environment_create_environment_variables_object(false, xctx);
        afw_runtime_xctx_set_object(self->environment_variables_object,
            true, xctx);
        afw_xctx_qualifier_stack_qualifier_object_push(&afw_s_environment,
            self->environment_variables_object, true, xctx->p, xctx);        

        /* If extension specified, load it. */
        if (self->extension.len > 0) {
            afw_environment_load_extension(&self->extension,
                NULL, NULL, xctx);
        }

        /* If conf specified, process it. */
        if (self->conf.len > 0) {

            /* Get content type for conf. */
            self->content_type_in = afw_environment_get_content_type(
                &self->type_in, xctx);
            if (!self->content_type_in) {
                AFW_THROW_ERROR_Z(general, "Invalid content-type.", xctx);
            }

            /* Load conf as a list of conf objects. */
            conf_file = afw_file_to_memory(&self->conf, 0, xctx->p, xctx);
            conf = afw_content_type_raw_to_value(self->content_type_in,
                conf_file, &self->conf, xctx->p, xctx);
            if (!afw_value_is_array(conf)) {
                AFW_THROW_ERROR_Z(general, "Invalid configuration.",
                    xctx);
            }
            afw_environment_configure_with_object_list(
                ((const afw_value_array_t *)conf)->internal,
                &self->conf, xctx);
        }

        /* Get output content type. */
        self->content_type_out = afw_environment_get_content_type(
            &self->type_out, xctx);
        if (!self->content_type_out) {
            AFW_THROW_ERROR_Z(general, "Invalid --allow content-type.", xctx);
        }

        /* If expression specified, evaluate it. */
        if (self->expression_z) {
            self->source_location = &afw_command_s_afw_command_dash_x;
            impl_evaluate(self, &self->expression, NULL);
            self->source_location = &afw_command_s_afw_command;
        }

        /* If [IN] specified, read from file instead of stdin. */
        if (self->in_z) {
            self->source_location = &self->in;
            self->fd_input = fopen(self->in_z, "r");
            if (!self->fd_input) {
                fprintf(xctx->env->stderr_fd,
                    "Error opening input %s errno %d - %s\n",
                    self->in_z, errno, strerror(errno));
                return -1;
            }
        }

        /* If local mode specified, open output. */
        if (self->local_mode_z) {
            self->source_location = &afw_command_s_afw_command_local_mode;
            for (s = self->local_mode_z; *s && *s >= '0' && *s <= '9'; s++);
            if (*s == 0)
            {
                rv = atoi(self->local_mode_z);
                if (rv > 0) {
                    self->fd_output = fdopen(rv, "w");
                }
            }
            else {
                self->fd_output = fopen(self->local_mode_z, "w");
            }
            if (!self->fd_output) {
                fprintf(xctx->env->stderr_fd,
                    "Error opening -l path %s errno %d - %s\n",
                    self->local_mode_z, errno, strerror(errno));
                return -1;
            }
        }

        if (self->interactive_mode) {
            self->source_location = &afw_command_s_afw_command_interactive;
        }

        /* If script, only evaluate once. */
        if (
                (
                    self->compile_option == afw_compile_type_script &&
                    self->can_span_lines
                ) ||
                self->compile_option == afw_compile_type_test_script
            )
        {
            self->read_full = true;
            self->callback = impl_octet_get_cb;
            self->callback_data = self;
            self->residual_check = afw_compile_residual_check_to_full;
            if (self->compile_option == afw_compile_type_script) {
                impl_evaluate(self, NULL, &rv);
            }
            else {
                impl_evaluate(self, NULL, NULL);
                rv = 0;
            }
        }

        /* If local mode, start local server and run. */
        else if (self->local_mode_z) {
            self->local_server = afw_command_local_server_create(self);
            afw_server_run(self->local_server, NULL, xctx);
        }

        /*
         * If local or interactive mode, loop evaluating expressions until
         * exit.
         */
        else if (self->in_z || self->interactive_mode) {
            if (self->can_span_lines) {
                self->callback = impl_octet_get_cb;
                self->callback_data = self;
                self->residual_check = afw_compile_residual_check_to_newline;
            }
            if (self->interactive_mode) {
                impl_print_result(self,
                    "afw " AFW_VERSION_STRING "\n\n"
                    "Input will be evaluated as entered.  "
                    "Type exit to end.\n");
            }
            impl_print_end(self);

            while (impl_evaluate(self, NULL, NULL));
        }
    }

    /* Print any unhandled errors. */
    AFW_CATCH_UNHANDLED{
        impl_print_error(self, AFW_ERROR_THROWN, xctx);
        impl_print_end(self);
        rv = EXIT_FAILURE;
    }

    /* Make sure things are cleaned up. */
    AFW_FINALLY{

        /* If [IN] specified and file open, close it. */
        if (self && self->fd_input && self->in_z) {
            fclose(self->fd_input);
        }

        /* Release enviornment. */
        afw_environment_release(xctx);

        /* Special case: xctx is gone, so return before AFW_ENDTRY. */
        return rv;
    }

    AFW_ENDTRY;
}

/** @} */
