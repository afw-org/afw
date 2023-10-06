#include "afw.h"
#include "afw_lmdb_generated.h"
#include "../afw_lmdb_internal.h"

/* LMDB RV decoder */
static const afw_utf8_z_t *
impl_rv_decoder_z_lmdb(int rv,
    afw_utf8_z_t *wa, afw_size_t wa_size)
{
    return mdb_strerror(rv);
}


/* Register lmdb RV decoder. */
void
afw_lmdb_error_rv_decoder_lmdb_register(afw_xctx_t *xctx)
{
    afw_environment_register_error_rv_decoder(
        afw_lmdb_s_lmdb, impl_rv_decoder_z_lmdb, xctx);
}
