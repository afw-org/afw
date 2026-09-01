/* Fixture: extra -I must win over a leftover header in AFW_INCLUDE_DIR. */

#include "override_marker.h"

int
main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    return MARKER == 1 ? 0 : 1;
}
