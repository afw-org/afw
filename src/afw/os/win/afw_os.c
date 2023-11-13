// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework OS Specific Functions for Windows
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_os.c
 * @brief Adaptive Framework OS Specific Functions for Windows
 */

/* Min windows 7. */
#define NTDDI_VERSION 0x06010000
#define _WIN32_WINNT 0x0601

#include "afw.h"
#include "Dbghelp.h"

static const afw_utf8_t
impl_s_no_memory_for_backtrace =
    AFW_UTF8_LITERAL("No memory for backtrace");

static const afw_utf8_t
impl_dso_suffix =
    AFW_UTF8_LITERAL(".dll");

static const afw_utf8_t
impl_s_os =
    AFW_UTF8_LITERAL("os");


#ifdef __EXAMPLE_FOR_CREATING_WITH_CB_WRAPPER_
static const afw_object_t *
impl_get_<objectId>_object_cb(
    void *data,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;

    /* Construct system specific object. */
    result = afw_object_create_unmanaged(p, xctx);
    afw_object_meta_set_ids(result,
        afw_s_afw,
        afw_s__AdaptiveSystemInfo_,
        afw_s_<objectId>,
        xctx);
    afw_object_meta_set_read_only(result, xctx);
    afw_object_meta_set_property_from_utf8_z(
        result, afw_s_description,
        "<objectId> description.",
        xctx);

    /* Add in system specific info properties. */
    /** @fixme Replace with appropriate code. */
    afw_object_set_property_from_utf8_z(result,
        &impl_s_os,
        "windows",
        xctx);

    afw_object_meta_set_property_type_property_from_utf8_z(
        result, &impl_s_os, afw_s_label,
        "OS",
        xctx);

    afw_object_meta_set_property_type_property_from_utf8_z(
        result, &impl_s_os, afw_s_description,
        "Operating system.",
        xctx);
 
    /* Return result. */
    return result;
}

#endif  /* __EXAMPLE_FOR_CREATING_WITH_CB_WRAPPER_ */

static const afw_object_t *
impl_create_general_object(afw_xctx_t *xctx)
{
    const afw_object_t *result;

    /* Construct system specific object. */
    result = afw_object_create_unmanaged(xctx->p, xctx);
    afw_object_meta_set_ids(result,
        afw_s_afw,
        afw_s__AdaptiveSystemInfo_,
        afw_s_general,
        xctx);
    afw_object_meta_set_read_only(result, xctx);
    afw_object_meta_set_property_from_utf8_z(
        result, afw_s_description,
        "General systems info.",
        xctx);

    /* Add in system specific info properties. */
    /** @fixme Replace with appropriate code. */
    afw_object_set_property_as_string_from_utf8_z(result,
        &impl_s_os,
        "windows",
        xctx);

    afw_object_meta_set_property_type_property_from_utf8_z(
        result, &impl_s_os, afw_s_label,
        "OS",
        xctx);

    afw_object_meta_set_property_type_property_from_utf8_z(
        result, &impl_s_os, afw_s_description,
        "Operating system.",
        xctx);

    /** @fixme Remove this section.  Was for testing time functions. */
    afw_object_set_property_as_date_from_parts(result,
        afw_s_S, 1923, 12, 31, -4, 0, xctx);
 
    afw_object_set_property_as_dateTime_from_parts(result,
        afw_s_N, 1923, 12, 31, 12, 30, 14,  999999, -4, 0, xctx);
 
    afw_object_set_property_as_time_from_parts(result,
        afw_s_M, 12, 30, 14, 999999, -4, 0, xctx);
 
    afw_object_set_property_as_dayTimeDuration_from_parts(result,
        afw_s_B, true, 12, 34, 56, 78, 90, xctx);
 
    afw_object_set_property_as_dayTimeDuration_from_parts(result,
        afw_s_L, false, 12, 34, 56, 78, 90, xctx);

    afw_object_set_property_as_yearMonthDuration_from_parts(result,
        afw_s_SS, true, 100, 200, xctx);
 
    afw_object_set_property_as_yearMonthDuration_from_parts(result,
        afw_s_NS, false, 100, 200, xctx);
 
    /* Return result. */
    return result;
}




/* afw_os environment initialize */
AFW_DEFINE(void)
afw_os_environment_initialize(afw_xctx_t *xctx)
{
    const afw_log_factory_t *log_factory;
    const afw_object_t *object;

    /* Register factory for os log. */
    log_factory = afw_os_log_factory_get();
    afw_environment_register_log_type(
        &log_factory->log_type,
        log_factory, xctx);

    /* Create and set general object. */
    object = impl_create_general_object(xctx);
    afw_runtime_env_set_object(object, false, xctx);

#ifdef __EXAMPLE_FOR_CREATING_WITH_CB_WRAPPER_

    /* Set system info object wrapper. */
    afw_runtime_env_set_object_cb_wrapper(
        afw_s__AdaptiveSystemInfo_, afw_s_<objectId>,
        impl_get_<objectId>_object_cb, NULL, false, xctx);

#endif /* __EXAMPLE_FOR_CREATING_WITH_CB_WRAPPER_ */

}


/* Return a process id or similar number. */
AFW_DEFINE(afw_uint32_t)
afw_os_get_pid()
{
    return GetCurrentProcessId();
}


/* Return the suffix appended to dso file names for this system. */
AFW_DEFINE(const afw_utf8_t *)
afw_os_get_dso_suffix()
{
    return &impl_dso_suffix;
}


/*  Provide a backtrace if possible. */
AFW_DEFINE(const afw_utf8_t *)
afw_os_backtrace(
    afw_error_code_t code,
    int max_backtrace,
    afw_xctx_t *xctx)
{

    afw_utf8_t *result;
    afw_byte_t *s;
    afw_size_t len;
    int wlen;

    USHORT frames;
    ULONG max;
    void *stack[100];
    unsigned int i;
    HANDLE process;
    DWORD error;
    IMAGEHLP_LINE64 line;
    const char *filename, *c;
    char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
    DWORD dwDisplacement = 0;
    DWORD64 dwDisplacement64 = 0;
    PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;
    pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    pSymbol->MaxNameLen = MAX_SYM_NAME;

    process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);
    SymSetOptions(SYMOPT_LOAD_LINES);
    line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

    if (max_backtrace == 0) return NULL;
    if (max_backtrace < 0) max = 100;
    else if (max_backtrace > 99) max = 100;
    else max = max_backtrace + 1;

    /*
     * Allocate and initialize memory to hold result.
     *
     * Variable len is an approximation of the buffer needed to build the
     * result.  if it is not large enough, the the trace returned will be
     * truncated to what will fit.  The average line size is assumed to
     * be 100 plus 10 extra lines for other error info.
     */
    len = (max + 10) * 100;
    s = apr_palloc(afw_pool_get_apr_pool(xctx->p), len);
    if (!s) return &impl_s_no_memory_for_backtrace;
    result = apr_palloc(afw_pool_get_apr_pool(xctx->p), sizeof(afw_utf8_t));
    if (!result) return &impl_s_no_memory_for_backtrace;
    result->s = s;

    /* Capture backtrace stack. */
    frames = CaptureStackBackTrace(0, max, stack, NULL);

    /* Process each frame of stack. */
    for (i = 1; i < frames; i++) {
        if (SymGetLineFromAddr64(
            process,(DWORD64)(stack[i]), &dwDisplacement, &line))
        {
            filename = c = line.FileName;
            if (filename) {
                for (; *c; c++) if (*c == '\\') filename = c + 1;
            }
            else {
                filename = "";
            }

            SymFromAddr(process, (DWORD64)( stack[i] ),
                &dwDisplacement64, pSymbol);

            /* Add line for frame.  Stop if preallocated string is full. */
            wlen = snprintf(s, len, "%i: %s:%d %s\n", i - 1,
                filename, line.LineNumber,
                (pSymbol->Name) ? pSymbol->Name : "");
            if (wlen < 0 || (afw_size_t)wlen > len) break;
            s += wlen;
            len -= wlen;       
        }
        else {
            error = GetLastError();
            break;
        }
    }

    result->len = s - result->s;
    return result;
}
