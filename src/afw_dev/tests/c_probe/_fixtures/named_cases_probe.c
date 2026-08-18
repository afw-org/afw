/* Fixture for _afwdev.test.c_probe: named cases, no libafw calls. */

#include <stdio.h>
#include <string.h>

int
main(int argc, char **argv)
{
    const char *name;

    name = (argc > 1) ? argv[1] : "";
    if (strcmp(name, "ok") == 0) {
        return 0;
    }
    if (strcmp(name, "fail") == 0) {
        fprintf(stderr, "intentional fail\n");
        return 1;
    }
    fprintf(stderr, "usage: named_cases_probe ok|fail\n");
    return 2;
}
