#include "afw.h"
#include "afw_curl_generated.h"

#include <curl/curl.h>

/* CURL RV decoder */
static const afw_utf8_z_t *
impl_rv_decoder_z_curl(int rv,
    afw_utf8_z_t *wa, afw_size_t wa_size)
{
    return curl_easy_strerror(rv);
}


/* Register curl RV decoder. */
void
afw_curl_error_rv_decoder_curl_register(afw_xctx_t *xctx)
{
    afw_environment_register_error_rv_decoder(
        afw_curl_s_curl, impl_rv_decoder_z_curl, xctx);
}
