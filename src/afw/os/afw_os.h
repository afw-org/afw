// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework OS Specific Header for unix-like systems
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_OS_H__
#define __AFW_OS_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_os
 * @{
 */

/**
 * @file afw_os.h
 * @brief Adaptive Framework OS Specific Header for unix-like systems 
 */

AFW_BEGIN_DECLARES

/**
 * @brief afw_os environment initialize
 * @param xctx of caller
 *
 * This is called when the Adaptive Framework is being initialized.  Pool
 * xctx->p will last for the life of the environment.  Any
 * afw_environment_register functions can be called from here.  One possible
 * use is to register an afw_log_factory so that the system's log can
 * be configured by environment configuration.
 */
AFW_DECLARE(void)
afw_os_environment_initialize(afw_xctx_t *xctx);


/**
 * @brief Return a process id or similar number.
 * @return process id or similar number specific to system.
 */
AFW_DECLARE(afw_uint32_t)
afw_os_get_pid();


/**
 * @brief Current resident set size for this process.
 * @return RSS in kilobytes, or 0 if unavailable.
 *
 * Linux: `/proc/self/statm` resident pages (open/read/close, no alloc).
 * Same KB units as Linux `ru_maxrss`, but this is **current** RSS, not
 * peak. Used as the `rss` field on `>debug pool` lines. Win stubs 0.
 *
 * Do not call this from gdb after SIGSTOP — read `/proc/<pid>/status`
 * VmRSS from the gdb process instead.
 */
AFW_DECLARE(afw_size_t)
afw_os_get_rss();


/**
 * @brief Return the suffix appended to dso file names for this system. 
 * @return suffix such as ".so" or ".dll".
 */
AFW_DECLARE(const afw_utf8_t *)
afw_os_get_dso_suffix();


/**
 * @brief Create an instance of the OS log.
 * @param properties
 * @param p to use for log's resources.
 * @param xctx of caller.
 * @return log instance
 */
AFW_DECLARE(const afw_log_t *)
afw_os_log_create(
    const afw_object_t * properties,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Get the factory for OS log.
 * @return factory singleton instance.
 */
AFW_DECLARE(const afw_log_factory_t *)
afw_os_log_factory_get();


/**
 * @brief Provide a backtrace if possible.
 * @param code Error code or 0.  Code afw_error_code_memory or others might
 *    influence whether producing backtrace is possible.
 * @param max_backtrace
 * @param xctx of caller.
 * @return backtrace or NULL if not possible.
 *
 * IMPORTANT Implementation of this function must not do anything that might
 * throw an error or change xctx->error or a recursive error producing a
 * stack overflow will occur.
 *
 * If max_backtrace is -1 or greater than the implementation's max, the max is
 * used.  If 0 is specifed, no trace occurs.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_os_backtrace(
    afw_error_code_t code,
    int max_backtrace,
    afw_xctx_t *xctx);


/**
 * @brief Current working directory, allocated in p.
 * @return directory or NULL if unavailable.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_os_getcwd(const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Absolute real path of path (follows symlinks).
 * @param path 0-terminated host path (absolute or relative to CWD).
 * @param p pool for the returned path.
 * @param xctx of caller.
 * @return pool-allocated path, or NULL if it cannot be resolved
 *     (does not exist, permission, etc.). Does not throw on ENOENT.
 *
 * On failure errno is the libc/OS error from the resolve. Callers that
 * want that code in an AFW throw must save errno before the next call.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_os_realpath(
    const afw_utf8_z_t *path,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Fill buf with cryptographically strong random bytes.
 * @param buf destination.
 * @param len number of bytes.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_os_random_bytes(void *buf, afw_size_t len, afw_xctx_t *xctx);


/**
 * @brief POSIX-style glob match (flags 0).
 * @return true if name matches pattern.
 */
AFW_DECLARE(afw_boolean_t)
afw_os_fnmatch(
    const afw_utf8_z_t *pattern,
    const afw_utf8_z_t *name);


/** @brief Signal handler passed to afw_os_signal(). */
typedef void (*afw_os_signal_handler_t)(int);

/**
 * @brief Install a process signal handler (sigaction on nix).
 */
AFW_DECLARE(void)
afw_os_signal(int signo, afw_os_signal_handler_t handler);


/**
 * @brief Microseconds since the Unix epoch.
 */
typedef afw_integer_t afw_os_time_t;

/**
 * @brief Broken-down time (civil calendar).
 *
 * year is the full year; month is 1-12; gmtoff is seconds east of UTC.
 */
typedef struct afw_os_time_exploded_s {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int microsecond;
    int gmtoff;
} afw_os_time_exploded_t;

/**
 * @brief Current time in microseconds since the Unix epoch.
 */
AFW_DECLARE(afw_os_time_t)
afw_os_time_now(void);

/**
 * @brief Explode microseconds since epoch as local time.
 */
AFW_DECLARE(void)
afw_os_time_explode_local(
    afw_os_time_exploded_t *out,
    afw_os_time_t t,
    afw_xctx_t *xctx);

/**
 * @brief Explode microseconds since epoch as UTC.
 */
AFW_DECLARE(void)
afw_os_time_explode_utc(
    afw_os_time_exploded_t *out,
    afw_os_time_t t,
    afw_xctx_t *xctx);


/** @brief Opaque loaded DSO handle. */
typedef struct afw_os_dso_s afw_os_dso_t;

/**
 * @brief Load a shared library. NULL on failure.
 *
 * The handle is closed when p is destroyed, or by afw_os_dso_unload().
 * After a failed load, afw_os_dso_error() is the last error text.
 */
AFW_DECLARE(afw_os_dso_t *)
afw_os_dso_load(
    const afw_utf8_z_t *path,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Last DSO load/sym error (thread-local; may be empty).
 */
AFW_DECLARE(const afw_utf8_z_t *)
afw_os_dso_error(void);

/**
 * @brief Resolve a symbol. NULL on failure.
 */
AFW_DECLARE(void *)
afw_os_dso_sym(
    afw_os_dso_t *dso,
    const afw_utf8_z_t *name,
    afw_xctx_t *xctx);

/**
 * @brief Unload. Safe if already unloaded or NULL.
 */
AFW_DECLARE(void)
afw_os_dso_unload(afw_os_dso_t *dso);


/**
 * Mutex, rwlock, and native thread. Create allocates in p; pool
 * cleanup destroys the OS object. Threads are joinable — join is
 * the caller's job, not pool destroy. Public AFW names are
 * afw_thread_mutex_* / afw_thread_rwlock_* (see afw_thread.h).
 */

/** @brief Mutex allocated from an afw_pool; destroyed with that pool. */
typedef struct afw_os_mutex_s afw_os_mutex_t;

/** @brief Read/write lock allocated from an afw_pool. */
typedef struct afw_os_rwlock_s afw_os_rwlock_t;

/** @brief Native thread handle allocated from an afw_pool. */
typedef struct afw_os_thread_s afw_os_thread_t;

/** @brief Platform default mutex (non-recursive on nix). */
#define AFW_OS_MUTEX_DEFAULT  0

/** @brief Recursive mutex (was APR_THREAD_MUTEX_NESTED). */
#define AFW_OS_MUTEX_NESTED   1

/** @brief Non-recursive mutex (was APR_THREAD_MUTEX_UNNESTED). */
#define AFW_OS_MUTEX_UNNESTED 2

/**
 * @brief Create a mutex in p. Destroyed when p is destroyed.
 * @param flags AFW_OS_MUTEX_DEFAULT, NESTED, or UNNESTED.
 *
 * Nested is recursive. Unnested is errorcheck (relock throws).
 * pthread functions return the errno value (not -1 + errno).
 */
AFW_DECLARE(afw_os_mutex_t *)
afw_os_mutex_create(
    unsigned int flags,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE(void)
afw_os_mutex_lock(afw_os_mutex_t *mutex, afw_xctx_t *xctx);

/** @brief true if the lock was taken. */
AFW_DECLARE(afw_boolean_t)
afw_os_mutex_trylock(afw_os_mutex_t *mutex, afw_xctx_t *xctx);

AFW_DECLARE(void)
afw_os_mutex_unlock(afw_os_mutex_t *mutex, afw_xctx_t *xctx);

/** @brief Idempotent. Optional; pool cleanup also calls this. */
AFW_DECLARE(void)
afw_os_mutex_destroy(afw_os_mutex_t *mutex);

/**
 * @brief Create a read/write lock in p. Destroyed when p is destroyed.
 */
AFW_DECLARE(afw_os_rwlock_t *)
afw_os_rwlock_create(
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE(void)
afw_os_rwlock_rdlock(afw_os_rwlock_t *rwlock, afw_xctx_t *xctx);

AFW_DECLARE(void)
afw_os_rwlock_wrlock(afw_os_rwlock_t *rwlock, afw_xctx_t *xctx);

AFW_DECLARE(void)
afw_os_rwlock_unlock(afw_os_rwlock_t *rwlock, afw_xctx_t *xctx);

/** @brief Idempotent. Optional; pool cleanup also calls this. */
AFW_DECLARE(void)
afw_os_rwlock_destroy(afw_os_rwlock_t *rwlock);

/** @brief Thread start function (pthread-style). */
typedef void *(*afw_os_thread_start_t)(void *arg);

/**
 * @brief Create a joinable thread. Does not join on pool destroy.
 */
AFW_DECLARE(afw_os_thread_t *)
afw_os_thread_create(
    afw_os_thread_start_t start,
    void *arg,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE(void)
afw_os_thread_join(afw_os_thread_t *thread, afw_xctx_t *xctx);

/**
 * @brief Send signo to the thread (pthread_kill on nix).
 *
 * Safe to call from a signal handler. No-op if thread is NULL or
 * already joined.
 */
AFW_DECLARE(void)
afw_os_thread_kill(const afw_os_thread_t *thread, int signo);


AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_OS_H__ */
