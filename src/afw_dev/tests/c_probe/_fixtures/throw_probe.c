/* Fixture for _afwdev.test.c_probe: expected throw under valgrind. */

#include "afw.h"

#include <stdio.h>
#include <string.h>

int
main(int argc, char **argv)
{
    const afw_error_t *create_error;
    afw_xctx_t *xctx;
    const char *name;
    int threw;

    xctx = afw_environment_create(afw_version(), argc,
        (const char * const *)argv, &create_error);
    if (!xctx) {
        fprintf(stderr, "environment create failed\n");
        return 2;
    }

    name = (argc > 1) ? argv[1] : "";
    if (strcmp(name, "ok") == 0) {
        afw_environment_release(xctx);
        return 0;
    }
    if (strcmp(name, "throw") != 0) {
        fprintf(stderr, "usage: throw_probe ok|throw\n");
        afw_environment_release(xctx);
        return 2;
    }

    threw = 0;
    AFW_TRY {
        AFW_THROW_ERROR_Z(general, "expected throw", xctx);
    }
    AFW_CATCH_UNHANDLED {
        if (AFW_ERROR_THROWN->message_z &&
            strstr(AFW_ERROR_THROWN->message_z, "expected throw"))
        {
            threw = 1;
        }
        else {
            fprintf(stderr, "throw: unexpected %s\n",
                AFW_ERROR_THROWN->message_z
                ? AFW_ERROR_THROWN->message_z : "?");
        }
    }
    AFW_ENDTRY;

    afw_environment_release(xctx);
    if (!threw) {
        fprintf(stderr, "throw: did not throw\n");
        return 1;
    }
    return 0;
}
