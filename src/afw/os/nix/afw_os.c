// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework OS Specific Functions for *nix systems
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_os.c
 * @brief Adaptive Framework OS Specific Functions for *nix systems
 */

#include "afw.h"
#include <sys/types.h> 
#include <sys/utsname.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <execinfo.h>

#ifndef __MACH__
#define UNW_LOCAL_ONLY
#include <libunwind.h>
#include <elfutils/libdwfl.h>
#endif

static const afw_utf8_t impl_s_no_memory_for_backtrace =
    AFW_UTF8_LITERAL("No memory for backtrace");

static const afw_utf8_t impl_dso_suffix =
    AFW_UTF8_LITERAL(".so");

static const afw_utf8_t impl_s_general_description =
    AFW_UTF8_LITERAL("General systems info.");

/* Property names as static string values (&pub / &internal). */
static const afw_value_string_t impl_v_sysname =
    AFW_VALUE_STRING_LITERAL("sysname");

static const afw_value_string_t impl_v_nodename =
    AFW_VALUE_STRING_LITERAL("nodename");

static const afw_value_string_t impl_v_release =
    AFW_VALUE_STRING_LITERAL("release");

static const afw_value_string_t impl_v_machine =
    AFW_VALUE_STRING_LITERAL("machine");

static const afw_value_string_t impl_v_domainname =
    AFW_VALUE_STRING_LITERAL("domainname");


/* string literals for resource usage object */
static const afw_utf8_t impl_s_resourceUsage =
AFW_UTF8_LITERAL("resourceUsage");

static const afw_value_string_t impl_v_ru_utime =
    AFW_VALUE_STRING_LITERAL("utime");

static const afw_value_string_t impl_v_ru_stime =
    AFW_VALUE_STRING_LITERAL("stime");

static const afw_value_string_t impl_v_ru_maxrss =
    AFW_VALUE_STRING_LITERAL("maxrss");

static const afw_value_string_t impl_v_ru_ixrss =
    AFW_VALUE_STRING_LITERAL("ixrss");

static const afw_value_string_t impl_v_ru_idrss =
    AFW_VALUE_STRING_LITERAL("idrss");

static const afw_value_string_t impl_v_ru_isrss =
    AFW_VALUE_STRING_LITERAL("isrss");

static const afw_value_string_t impl_v_ru_minflt =
    AFW_VALUE_STRING_LITERAL("minflt");

static const afw_value_string_t impl_v_ru_majflt =
    AFW_VALUE_STRING_LITERAL("majflt");

static const afw_value_string_t impl_v_ru_nswap =
    AFW_VALUE_STRING_LITERAL("nswap");

static const afw_value_string_t impl_v_ru_inblock =
    AFW_VALUE_STRING_LITERAL("inblock");

static const afw_value_string_t impl_v_ru_oublock =
    AFW_VALUE_STRING_LITERAL("oublock");

static const afw_value_string_t impl_v_ru_msgsnd =
    AFW_VALUE_STRING_LITERAL("msgsnd");

static const afw_value_string_t impl_v_ru_msgrcv =
    AFW_VALUE_STRING_LITERAL("msgrcv");

static const afw_value_string_t impl_v_ru_nsignals =
    AFW_VALUE_STRING_LITERAL("nsignals");

static const afw_value_string_t impl_v_ru_nvcsw =
    AFW_VALUE_STRING_LITERAL("nvcsw");

static const afw_value_string_t impl_v_ru_nivcsw =
    AFW_VALUE_STRING_LITERAL("nivcsw");

/* string literals for resource usage object */
static const afw_utf8_t impl_s_resourceLimits =
AFW_UTF8_LITERAL("resourceLimits");

static const afw_value_string_t impl_v_rl_cpu =
    AFW_VALUE_STRING_LITERAL("cpu");

static const afw_value_string_t impl_v_rl_fsize =
    AFW_VALUE_STRING_LITERAL("fsize");

static const afw_value_string_t impl_v_rl_stack =
    AFW_VALUE_STRING_LITERAL("stack");

static const afw_value_string_t impl_v_rl_rss =
    AFW_VALUE_STRING_LITERAL("rss");

static const afw_value_string_t impl_v_rl_memlock =
    AFW_VALUE_STRING_LITERAL("memlock");

static const afw_value_string_t impl_v_rl_nproc =
    AFW_VALUE_STRING_LITERAL("nproc");

static const afw_value_string_t impl_v_rl_ofile =
    AFW_VALUE_STRING_LITERAL("ofile");


#ifdef __EXAMPLE_FOR_CREATING_WITH_CB_WRAPPER_

static const afw_utf8_t
impl_s_<objectId>_description =
    AFW_UTF8_LITERAL("FIXME");

static const afw_utf8_t
impl_s_os =
AFW_UTF8_LITERAL("os");

static const afw_utf8_t
impl_s_os_label =
AFW_UTF8_LITERAL("OS");

static const afw_utf8_t
impl_s_os_description =
AFW_UTF8_LITERAL("Operating system.");

static const afw_utf8_t
impl_s_linux =
AFW_UTF8_LITERAL("linux");

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
    afw_object_meta_set_property_as(result, afw_v_description,
        string, &impl_s_<objectId>_description, xctx);
    afw_object_meta_set_read_only(result, xctx);

    /* Add in system specific info properties. */
    /** @fixme Replace with appropriate code. */
    afw_object_set_property_as_string(result,
        afw_value_create_unmanaged_string(&impl_s_os, xctx->p, xctx), &impl_s_linux, xctx);
    afw_object_meta_set_property_type_property_as(result,
        &impl_s_os, afw_s_label,
        single, string, &impl_s_os_label, xctx);
    afw_object_meta_set_property_type_property_as(result,
        &impl_s_os, afw_s_description,
        single, string, &impl_s_os_description, xctx);

    /* Return result. */
    return result;
}

#endif /* __EXAMPLE_FOR_CREATING_WITH_CB_WRAPPER_ */

static const afw_object_t *
impl_get_resourceLimits_object_cb(
    void *data,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;
    struct rlimit *limits;
    int rc;

    /* Construct system specific object. */
    result = afw_object_create_unmanaged(p, xctx);
    afw_object_meta_set_ids(result,
        afw_s_afw,
        afw_s__AdaptiveSystemInfo_,
        &impl_s_resourceLimits,
        xctx);

    afw_object_meta_set_property_from_utf8_z(
        result, afw_v_description,
        "Resource Limits for the calling process.",
        xctx);
    afw_object_meta_set_read_only(result, xctx);

    limits = afw_pool_calloc_type(p, struct rlimit, xctx);

    rc = getrlimit(RLIMIT_CPU, limits);
    if (rc == 0) {
        afw_object_set_property_as_integer(result,
            &impl_v_rl_cpu.pub, limits->rlim_cur, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_rl_cpu.pub, afw_v_label, "CPU Time", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_rl_cpu.pub, afw_v_description,
            "The maximum amount of CPU time the process can use. If it runs for longer than this, it gets a signal: SIGXCPU. The value is measured in seconds.",
            xctx);
    }

    rc = getrlimit(RLIMIT_FSIZE, limits);
    if (rc == 0) {
        afw_object_set_property_as_integer(result,
            &impl_v_rl_fsize.pub, limits->rlim_cur, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_rl_fsize.pub, afw_v_label, "File Size", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_rl_fsize.pub, afw_v_description,
            "The maximum size of file the process can create. Trying to write a larger file causes a signal: SIGXFSZ.",
            xctx);
    }

    rc = getrlimit(RLIMIT_STACK, limits);
    if (rc == 0) {
        afw_object_set_property_as_integer(result,
            &impl_v_rl_stack.pub, limits->rlim_cur, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_rl_stack.pub, afw_v_label, "Stack Size", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_rl_stack.pub, afw_v_description,
            "The maximum stack size for the process. If the process tries to extend its stack past this size, it gets a SIGSEGV signal",
            xctx);
    }

    rc = getrlimit(RLIMIT_CORE, limits);
    if (rc == 0) {
        afw_object_set_property_as_integer(result,
            afw_v_core, limits->rlim_cur, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            afw_v_core, afw_v_label, "Core Size", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            afw_v_core, afw_v_description,
            "The maximum size core file that this process can create. If the process terminates and would dump a core file larger than this, then no core file is created.  So setting this limit to zero prevents core files from ever being created.",
            xctx);
    }

    rc = getrlimit(RLIMIT_RSS, limits);
    if (rc == 0) {
        afw_object_set_property_as_integer(result,
            &impl_v_rl_rss.pub, limits->rlim_cur, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_rl_rss.pub, afw_v_label, "Physical Memory", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_rl_rss.pub, afw_v_description,
            "The maximum amount of physical memory that this process should get. This parameter is a guide for the system’s scheduler and memory allocator; the system may give the process more memory when there is a surplus.",
            xctx);
    }

    rc = getrlimit(RLIMIT_MEMLOCK, limits);
    if (rc == 0) {
        afw_object_set_property_as_integer(result,
            &impl_v_rl_memlock.pub, limits->rlim_cur, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_rl_memlock.pub, afw_v_label, "Locked Physical Memory", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_rl_memlock.pub, afw_v_description,
            "The maximum amount of memory that can be locked into physical memory (so it will never be paged out).",
            xctx);
    }

    rc = getrlimit(RLIMIT_NPROC, limits);
    if (rc == 0) {
        afw_object_set_property_as_integer(result,
            &impl_v_rl_nproc.pub, limits->rlim_cur, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_rl_nproc.pub, afw_v_label, "Processes", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_rl_nproc.pub, afw_v_description,
            "The maximum number of processes that can be created with the same user ID.  If you have reached the limit for your user ID, fork will fail with EAGAIN.",
            xctx);
    }

#if defined(RLIMIT_NOFILE)
    rc = getrlimit(RLIMIT_NOFILE, limits);
#else
    rc = getrlimit(RLIMIT_OFILE, limits);
#endif
    if (rc == 0) {
        afw_object_set_property_as_integer(result,
            &impl_v_rl_ofile.pub, limits->rlim_cur, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_rl_ofile.pub, afw_v_label, "Open Files", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_rl_ofile.pub, afw_v_description,
            "The maximum number of files that the process can open. If it tries to open more files than this, its open attempt fails with errno EMFILE.",
            xctx);
    }

#ifdef __MACH__
    rc = getrlimit(RLIMIT_DATA, limits);
#else
    rc = getrlimit(RLIMIT_AS, limits);
#endif
    if (rc == 0) {
        afw_object_set_property_as_integer(result,
            afw_v_as, limits->rlim_cur, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            afw_v_as, afw_v_label, "Total Memory", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            afw_v_as, afw_v_description,
            "The maximum size of total memory that this process should get. If the process tries to allocate more memory beyond this amount with, for example, brk, malloc, mmap or sbrk, the allocation function fails.",
            xctx);
    }

    return result;
}

static const afw_object_t *
impl_get_resourceUsage_object_cb(
    void *data,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;
    struct rusage *usage;
    int rc;

    /* Construct system specific object. */
    result = afw_object_create_unmanaged(p, xctx);
    afw_object_meta_set_ids(result,
        afw_s_afw,
        afw_s__AdaptiveSystemInfo_,
        &impl_s_resourceUsage,
        xctx);

    afw_object_meta_set_property_from_utf8_z(
        result, afw_v_description,
        "Resource usage statistics for the calling process, which is the sum of resources used by all threads in the process.",
        xctx);
    afw_object_meta_set_read_only(result, xctx);

    usage = afw_pool_calloc_type(p, struct rusage, xctx);
    rc = getrusage(RUSAGE_SELF, usage);
    if (rc == 0) {
        afw_object_set_property_as_dayTimeDuration_from_parts(result,
            &impl_v_ru_utime.pub, true,
            0, 0, 0, usage->ru_utime.tv_sec, usage->ru_utime.tv_usec,
            xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_utime.pub, afw_v_label, "User Time", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_utime.pub, afw_v_description, 
            "Time spent executing user instructions.", 
            xctx);

        afw_object_set_property_as_dayTimeDuration_from_parts(result,
            &impl_v_ru_stime.pub, true,
            0, 0, 0, usage->ru_stime.tv_sec, usage->ru_stime.tv_usec,
            xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_stime.pub, afw_v_label, "System Time", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_stime.pub, afw_v_description, 
            "Time spent in operating system code on behalf of this processes.",
            xctx);

        afw_object_set_property_as_integer(result,
            &impl_v_ru_maxrss.pub, usage->ru_maxrss, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_maxrss.pub, afw_v_label, "Max Resident Size", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_maxrss.pub, afw_v_description,
            "The maximum resident set size used, in kilobytes. That is, the maximum number of kilobytes of physical memory that this process used simultaneously.", 
            xctx);

        afw_object_set_property_as_integer(result,
            &impl_v_ru_ixrss.pub, usage->ru_ixrss, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_ixrss.pub, afw_v_label, "Shared Memory Size", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_ixrss.pub, afw_v_description,
            "An integral value expressed in kilobytes times ticks of execution, which indicates the amount of memory used by text that was shared with other processes.",
            xctx);

        afw_object_set_property_as_integer(result,
            &impl_v_ru_idrss.pub, usage->ru_idrss, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_idrss.pub, afw_v_label, "Unshared Data Size", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_idrss.pub, afw_v_description,
            "An integral value expressed the same way, which is the amount of unshared memory used for data.",
            xctx);

        afw_object_set_property_as_integer(result,
            &impl_v_ru_isrss.pub, usage->ru_isrss, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_isrss.pub, afw_v_label, "Unshared Stack Size", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_isrss.pub, afw_v_description,
            "An integral value expressed the same way, which is the amount of unshared memory used for stack space.",
            xctx);

        afw_object_set_property_as_integer(result,
            &impl_v_ru_minflt.pub, usage->ru_minflt, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_minflt.pub, afw_v_label, "Page Reclaims", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_minflt.pub, afw_v_description,
            "The number of page faults which were serviced without requiring any I/O.",
            xctx);

        afw_object_set_property_as_integer(result,
            &impl_v_ru_majflt.pub, usage->ru_majflt, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_majflt.pub, afw_v_label, "Page Faults", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_majflt.pub, afw_v_description,
            "The number of page faults which were serviced by doing I/O.",
            xctx);

        afw_object_set_property_as_integer(result,
            &impl_v_ru_nswap.pub, usage->ru_nswap, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_nswap.pub, afw_v_label, "Swaps", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_nswap.pub, afw_v_description,
            "The number of times this processes was swapped entirely out of main memory.",
            xctx);

        afw_object_set_property_as_integer(result,
            &impl_v_ru_inblock.pub, usage->ru_inblock, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_inblock.pub, afw_v_label, "Disk Reads", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_inblock.pub, afw_v_description,
            "The number of times the file system had to read from the disk on behalf of this processes.",
            xctx);

        afw_object_set_property_as_integer(result,
            &impl_v_ru_oublock.pub, usage->ru_oublock, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_oublock.pub, afw_v_label, "Disk Writes", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_oublock.pub, afw_v_description,
            "The number of times the file system had to write to the disk on behalf of this processes.",
            xctx);

        afw_object_set_property_as_integer(result,
            &impl_v_ru_msgsnd.pub, usage->ru_msgsnd, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_msgsnd.pub, afw_v_label, "Messages Sent", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_msgsnd.pub, afw_v_description,
            "Number of IPC messages sent.",
            xctx);

        afw_object_set_property_as_integer(result,
            &impl_v_ru_msgrcv.pub, usage->ru_msgrcv, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_msgrcv.pub, afw_v_label, "Messages Received", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_msgrcv.pub, afw_v_description,
            "Number of IPC messages received.",
            xctx);

        afw_object_set_property_as_integer(result,
            &impl_v_ru_nsignals.pub, usage->ru_nsignals, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_nsignals.pub, afw_v_label, "Signals Received", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_nsignals.pub, afw_v_description,
            "Number of signals received.",
            xctx);

        afw_object_set_property_as_integer(result,
            &impl_v_ru_nvcsw.pub, usage->ru_nvcsw, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_nvcsw.pub, afw_v_label, "Voluntary Context Switches", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_nvcsw.pub, afw_v_description,
            "The number of times this processes voluntarily invoked a context switch (usually to wait for some service).",
            xctx);

        afw_object_set_property_as_integer(result,
            &impl_v_ru_nivcsw.pub, usage->ru_nivcsw, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_nivcsw.pub, afw_v_label, "Involuntary Context Switches", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_ru_nivcsw.pub, afw_v_description,
            "The number of times an involuntary context switch took place (because a time slice expired, or another process of higher priority was scheduled).",
            xctx);
    }

    /* Return result. */
    return result;
}

/* Create /afw/_AdaptiveSystemInfo_/general object. */
static const afw_object_t *
impl_create_general_object(afw_xctx_t *xctx)
{
    const afw_object_t *result;
    struct utsname *uname_s;
    int rc;

    /* Construct system specific object. */
    result = afw_object_create_unmanaged(xctx->p, xctx);
    afw_object_meta_set_ids(result,
        afw_s_afw,
        afw_s__AdaptiveSystemInfo_,
        afw_s_general,
        xctx);
    afw_object_meta_set_property_as(result, afw_v_description,
        string, &impl_s_general_description, xctx);
    afw_object_meta_set_read_only(result, xctx);

    /* use uname() to gather system name information */
    uname_s = afw_xctx_calloc_type(struct utsname, xctx);
    rc = uname(uname_s);
    if (rc > -1) {
        afw_object_set_property_as_string_from_utf8_z(result,
            &impl_v_sysname.pub, uname_s->sysname, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_sysname.pub, afw_v_label, "System Name", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_sysname.pub, afw_v_description, "This is the name of the operating system in use.", xctx);

        afw_object_set_property_as_string_from_utf8_z(result,
            &impl_v_nodename.pub, uname_s->nodename, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_nodename.pub, afw_v_label, "Node Name", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_nodename.pub, afw_v_description, "This is the host name of this particular computer. In the GNU C Library, the value is the same as that returned by gethostname.", xctx);

        afw_object_set_property_as_string_from_utf8_z(result,
            &impl_v_release.pub, uname_s->release, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_release.pub, afw_v_label, "Release", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_release.pub, afw_v_description, "This is the current release level of the operating system implementation.", xctx);

        afw_object_set_property_as_string_from_utf8_z(result,
            afw_v_version, uname_s->version, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            afw_v_version, afw_v_label, "Version", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            afw_v_version, afw_v_description, "This is the current version level within the release of the operating system.", xctx);

        afw_object_set_property_as_string_from_utf8_z(result,
            &impl_v_machine.pub, uname_s->machine, xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_machine.pub, afw_v_label, "Machine", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_machine.pub, afw_v_description, "This is a description of the type of hardware that is in use.", xctx);

#ifndef __MACH__
        /** 
         * \fixme The only wait to access ->domainname is to define 
         * _GNU_SOURCE before include headers, and we need to understand 
         * the implications of that, including access to non-standard / 
         * non-portable code.
         * 
         * afw_object_set_property_as_string_from_utf8_z(result,
         *    &impl_s_domainname, uname_s->domainname, xctx);
         */
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_domainname.pub, afw_v_label, "Domain Name", xctx);
        afw_object_meta_set_property_type_property_from_utf8_z(result,
            &impl_v_domainname.pub, afw_v_description, "This is the NIS or YP domain name. It is the same value returned by getdomainname.", xctx);
#endif
    }

    /* Return result. */
    return result;
}



/*  afw_os environment initialize */
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

    /* Set resourceUsage object wrapper. */
    afw_runtime_env_set_object_cb_wrapper(
        afw_s__AdaptiveSystemInfo_, &impl_s_resourceUsage,
        impl_get_resourceUsage_object_cb, NULL, false, xctx);

    /* Set resourceLimits object wrapper. */
    afw_runtime_env_set_object_cb_wrapper(
        afw_s__AdaptiveSystemInfo_, &impl_s_resourceLimits,
        impl_get_resourceLimits_object_cb, NULL, false, xctx);
}


/* Return a process id or similar number. */
AFW_DEFINE(afw_uint32_t) afw_os_get_pid()
{
    return getpid();
}


/* Current RSS in kilobytes. Linux /proc; no heap allocation. */
AFW_DEFINE(afw_size_t)
afw_os_get_rss()
{
#ifdef __linux__
    char buf[64];
    char *p;
    unsigned long resident_pages;
    ssize_t n;
    long page_kb;
    int fd;

    fd = open("/proc/self/statm", O_RDONLY);
    if (fd < 0) {
        return 0;
    }
    n = read(fd, buf, sizeof(buf) - 1);
    close(fd);
    if (n <= 0) {
        return 0;
    }
    buf[n] = 0;

    /* skip size; parse resident (pages). */
    p = buf;
    while (*p && *p != ' ') {
        p++;
    }
    if (*p != ' ') {
        return 0;
    }
    p++;
    resident_pages = 0;
    while (*p >= '0' && *p <= '9') {
        resident_pages = resident_pages * 10UL +
            (unsigned long)(*p - '0');
        p++;
    }
    page_kb = sysconf(_SC_PAGESIZE) / 1024;
    if (page_kb <= 0) {
        return 0;
    }
    return (afw_size_t)resident_pages * (afw_size_t)page_kb;
#else
    return 0;
#endif
}


/* Return the suffix appended to dso file names for this system. */
AFW_DEFINE(const afw_utf8_t *)
afw_os_get_dso_suffix()
{
    return &impl_dso_suffix;
}


/*  
 * Provide a backtrace if possible.  This routine
 * relies on libunwind to capture the stack frames
 * and libdw (from ELF utilities) to resolve debug
 * info such as file names and line numbers.
 *
 * There are a few caveats here:
 * 1.  Symbols are needed, so the binary needs to be
 *     compiled with the -g option.
 * 2.  Unpredictable results may happen when compiler 
 *     optimizations are turned on.
 */
#ifdef __MACH__
AFW_DEFINE(const afw_utf8_t *)
afw_os_backtrace(
    afw_error_code_t code,
    int max_backtrace,
    afw_xctx_t *xctx)
{
    void * callstack[128];
    int i, max;
    int frames;
    char ** strs;
    afw_byte_t *s; 
    afw_size_t len;
    afw_utf8_t *trace = NULL;
    int wlen;

    /*
     * If max_backtrace is -1 or greater than 99, cap at 99.  If 0, NULL will
     * be returned.
     *
     * Variable max is one more to avoid the trace of call to this function.
     */
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
    trace = apr_palloc(afw_pool_get_apr_pool(xctx->p), sizeof(afw_utf8_t));
    if (!trace) return &impl_s_no_memory_for_backtrace;
    trace->s = s;
    *s++ = '\n';
    len--;

    frames = backtrace(callstack, 128);
    if (!frames)
        return NULL; 

    strs = backtrace_symbols(callstack, frames);
    if (!strs)
        return NULL;

    for (i = 0; i < frames; i++) {
        wlen = snprintf(s, len, "%s\n", strs[i]);
        if (wlen < 0 || (afw_size_t)wlen > len) break;
        s += wlen;
        len -= wlen;
    }
    
    free(strs);

    trace->len = s - trace->s;

    return trace;
}

#else
AFW_DEFINE(const afw_utf8_t *)
afw_os_backtrace(
    afw_error_code_t code,
    int max_backtrace,
    afw_xctx_t *xctx)
{
    char *debuginfo_path = NULL;
    Dwfl_Callbacks callbacks = {
        .find_elf = dwfl_linux_proc_find_elf,
        .find_debuginfo = dwfl_standard_find_debuginfo,
        .debuginfo_path = &debuginfo_path,
    };
    Dwfl *dwfl;
    Dwarf_Addr addr;
    Dwfl_Module *module;
    Dwfl_Line *line;
    const afw_utf8_octet_t *function_name;
    unw_cursor_t cursor;
    unw_context_t uc;
    int i, max;
    int rc;

    afw_utf8_octet_t *s;
    afw_size_t len;
    afw_utf8_t *trace = NULL;
    int wlen;

    /*
     * If max_backtrace is -1 or greater than 99, cap at 99.  If 0, NULL will
     * be returned.
     *
     * Variable max is one more to avoid the trace of call to this function.
     */
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
    trace = apr_palloc(afw_pool_get_apr_pool(xctx->p), sizeof(afw_utf8_t));
    if (!trace) return &impl_s_no_memory_for_backtrace;
    trace->s = s;
    *s++ = '\n';
    len--;

    dwfl = dwfl_begin(&callbacks);
    if (!dwfl) {
        return &impl_s_no_memory_for_backtrace;
    }

    dwfl_linux_proc_report(dwfl, getpid());
    dwfl_report_end(dwfl, NULL, NULL);

    unw_getcontext(&uc);
    rc = unw_init_local(&cursor, &uc);
    if (rc) {
        /* error occurred, initializing cursor for local unwinding */
        dwfl_end(dwfl);
        return trace;
    }

    for (i = 0; unw_step(&cursor) > 0 && i < max; i++) {
        unw_word_t offset, ip, sp;
        char sym[256];

        unw_get_reg(&cursor, UNW_REG_IP, &ip);
        unw_get_reg(&cursor, UNW_REG_SP, &sp);
        if (ip == 0) {
            break;
        }

        wlen = snprintf(s, len, "0x%012lx in ", ip); 
        if (wlen < 0 || (afw_size_t)wlen > len) break;
        s += wlen;
        len -= wlen;       

        addr = (uintptr_t)(ip-4);
        module = dwfl_addrmodule(dwfl, addr);
        function_name = dwfl_module_addrname(module, addr);
        line = dwfl_getsrc(dwfl, addr);
        if (line != NULL) {
            int nline;
            Dwarf_Addr addr;
            const char *filename = dwfl_lineinfo(line, &addr, &nline, NULL, NULL, NULL);
       
            wlen = snprintf(s, len, "%s at %s:%d\n", 
                function_name, strrchr(filename, '/') + 1, nline);
            if (wlen < 0 || (afw_size_t)wlen > len) break;
            s += wlen;
            len -= wlen;       
        } else {
            if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0) {
                wlen = snprintf(s, len, "%s at +0x%lx\n", 
                    sym, offset); 
                if (wlen < 0 || (afw_size_t)wlen > len) break;
                s += wlen;
                len -= wlen;       
            } else {
                wlen = snprintf(s, len, "(symbol missing)\n"); 
                if (wlen < 0 || (afw_size_t)wlen > len) break;
                s += wlen;
                len -= wlen;       
            }
        }
    }
    
    trace->len = s - trace->s;

    /* free resources from dwfl_begin */
    dwfl_end(dwfl);

    return trace;
}
#endif
