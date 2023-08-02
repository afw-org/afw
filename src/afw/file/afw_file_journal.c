// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive framework file journal support
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_file_journal.c
 * @brief Adaptive framework file journal support
 */

#include "afw_internal.h"



/* Declares and rti/inf defines for interface afw_adaptor_factory */
#define AFW_IMPLEMENTATION_ID "file"
#include "afw_adaptor_journal_impl_declares.h"


/* afw_adaptor_journal lock holds UTC time of last use. */
typedef struct afw_adaptor_journal_lock_s {
    afw_endian_big_uint32_t usec;  /* (0-999999) microseconds past min */
    unsigned char sec;             /* (0-61) seconds past min          */
    unsigned char min;             /* (0-59) minutes past hour         */
    unsigned char hour;            /* (0-23) hours past midnight       */
    unsigned char day;             /* (1-31) day of the month          */
    unsigned char month;           /* (1-12) month of the year         */
    unsigned char year;            /* (0-99) year past century         */
    unsigned char century;         /* (19-*) century                   */
    unsigned char filler;          /* filler - ignore                  */
} afw_adaptor_journal_lock_t;


const afw_adaptor_journal_inf_t * afw_file_internal_get_journal_inf()
{
    return &impl_afw_adaptor_journal_inf;
}

/*
 * Convert entry_object_id to relative_entry_path_z
 *
 * param entry_object_id
 * param relative_entry_path_wa_z to be updated
 * param offset to be set
 * param xctx
 * return pointer to updated relative_entry_path_wa_z or NULL if error.
 *
 * relative_entry_path_wa_z must be size IMPL_RELATIVE_ENTRY_PATH_WA_Z_SIZE
 *
 * Convert  entry_object_id to relative journal file path and offset.
 *
 * Token format: ccyymmddhh_offset.
 *
 * Relative path format: 'y'ccyy/'m'mm/'d'dd/'h'hh
 *
 * For example if cursor is: 2016070423_3183
 *
 * Relative path to journal file will be: y2016/m07/d04/h23
 *
 * And journal entry will be at offset 3183.
 *
 * Note: this doesn't strictly check cursor syntax because open will
 * take care of not found.  Should probably do more checking.
 */
#define IMPL_RELATIVE_ENTRY_PATH_WA_Z_SIZE sizeof("y2016/m07/d04/h23")
static afw_utf8_z_t *
impl_object_id_to_relative_entry_path(
    const afw_utf8_t *entry_object_id,
    afw_utf8_z_t relative_entry_path_wa_z[],
    afw_off_t *offset,
    afw_xctx_t *xctx)
{
    const afw_utf8_z_t *i;
    afw_utf8_z_t *o;
    afw_size_t count;

    if (entry_object_id->len < strlen("ccyymmddhh_0")) return NULL;
    o = &relative_entry_path_wa_z[0];
    i = entry_object_id->s;
    *o++ = 'y';
    *o++ = *i++;
    *o++ = *i++;
    *o++ = *i++;
    *o++ = *i++;
    *o++ = '/';
    *o++ = 'm';
    *o++ = *i++;
    *o++ = *i++;
    *o++ = '/';
    *o++ = 'd';
    *o++ = *i++;
    *o++ = *i++;
    *o++ = '/';
    *o++ = 'h';
    *o++ = *i++;
    *o++ = *i++;
    *o = 0;
    if (*i++ != '_') return NULL;
    for (*offset = 0, count = entry_object_id->len - strlen("ccyymmddhh_");
        count > 0; count--)
    {
        *offset = *offset * 10 + (*i++ - '0');
    }

    return &relative_entry_path_wa_z[0];
}


/*
 * Convert relative_entry_path_z to entry_object_id.
 *
 * param z relative_entry_path_z
 * param offset
 * param xctx
 * return entry_object_id
 *
 * Example:                         1
 *                       01234567890123456
 *  relative_entry_path_z y2016/m07/d04/h23 offset 1234
 *
 *  yields entry_object_id 2016070423_1234
 */
AFW_DEFINE_STATIC_INLINE(const afw_utf8_t *)
impl_relative_entry_path_to_object_id(
    const afw_utf8_z_t *z, afw_off_t offset, afw_xctx_t *xctx)
{
    return afw_utf8_printf(xctx->p, xctx,
        "%.4s%.2s%.2s%.2s_" AFW_INTEGER_FMT,
        z + 1, z + 7, z + 11, z + 15, (afw_integer_t)offset);
}

static const afw_object_t *
impl_open_and_retrieve_peer_object(
    const afw_adaptor_journal_t *journal,
    const afw_utf8_t *consumer_id,
    apr_file_t * *peer_f,
    const afw_utf8_z_t * *full_peer_path_z,
    afw_xctx_t *xctx)
{
    afw_file_internal_adaptor_session_t * session =
        (afw_file_internal_adaptor_session_t *)journal->session;
    afw_file_internal_adaptor_t *adaptor = session->adaptor;
    const afw_pool_t *p = xctx->p;
    apr_pool_t * apr_p = afw_pool_get_apr_pool(p);
    void *memory;
    afw_memory_t buffer;
    apr_finfo_t finfo;
    const afw_value_t *value;
    afw_error_footprint_t footprint;
    apr_status_t rv;
    afw_size_t len;

    *full_peer_path_z = afw_utf8_z_printf(p, xctx,
        AFW_UTF8_FMT AFW_OBJECT_Q_OBJECT_TYPE_ID_PROVISIONING_PEER
        "/" AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(adaptor->root),
        AFW_UTF8_FMT_ARG(consumer_id));

    AFW_ERROR_FOOTPRINT("apr_stat()");
    rv = apr_stat(&finfo, *full_peer_path_z, APR_FINFO_SIZE,
        afw_pool_get_apr_pool(xctx->p));
    if (rv != APR_SUCCESS) goto error_peer_apr;

    AFW_ERROR_FOOTPRINT("apr_file_open()");
    rv = apr_file_open(peer_f, *full_peer_path_z,
        APR_FOPEN_READ + APR_FOPEN_WRITE + APR_FOPEN_BINARY,
        APR_FPROT_OS_DEFAULT, apr_p);
    if (rv != APR_SUCCESS) goto error_peer_apr;

    AFW_ERROR_FOOTPRINT("apr_file_read()");
    buffer.size = afw_safe_cast_off_to_size(finfo.size, xctx);
    len = buffer.size;
    memory = afw_xctx_calloc(len, xctx);
    buffer.ptr = memory;
    rv = apr_file_read(*peer_f, memory, &len);
    if (rv != APR_SUCCESS) goto error_peer;
    if (len != finfo.size) goto error_peer;

    AFW_ERROR_FOOTPRINT("afw_content_type_raw_to_value()");
    value = afw_content_type_raw_to_value(adaptor->content_type, &buffer,
        consumer_id, xctx->p, xctx);
    if (!value || !afw_value_is_object(value)) goto error_peer;

    /* Return peer object. */
    return ((const afw_value_object_t *)value)->internal;

error_peer:
    AFW_THROW_ERROR_FOOTPRINT_FZ(general, xctx,
        "Error detected processing adaptor " AFW_UTF8_FMT_Q " "
        AFW_OBJECT_Q_OBJECT_TYPE_ID_PROVISIONING_PEER
        " file %s - %s",
        AFW_UTF8_FMT_ARG(&adaptor->pub.adaptor_id),
        *full_peer_path_z, footprint.z);

error_peer_apr:
    AFW_THROW_ERROR_FOOTPRINT_RV_FZ(general, apr, rv, xctx,
        "Error detected processing adaptor " AFW_UTF8_FMT_Q " "
        AFW_OBJECT_Q_OBJECT_TYPE_ID_PROVISIONING_PEER
        " file %s - %s",
        AFW_UTF8_FMT_ARG(&adaptor->pub.adaptor_id),
        *full_peer_path_z, footprint.z);
}


static void
impl_write_and_close_peer_object(
    const afw_adaptor_journal_t *journal,
    const afw_object_t * peer,
    apr_file_t *peer_f,
    const afw_utf8_z_t *full_peer_path_z,
    afw_xctx_t *xctx)
{
    afw_file_internal_adaptor_session_t * session =
        (afw_file_internal_adaptor_session_t *)journal->session;
    afw_file_internal_adaptor_t *adaptor = session->adaptor;
    afw_error_footprint_t footprint;
    apr_status_t rv;
    const afw_memory_t *encoded;
    afw_size_t len;
    apr_off_t offset;

    /* Encode peer object. */
    encoded = afw_content_type_object_to_raw(
        adaptor->content_type, peer, NULL, xctx->p, xctx);

    /* Seek to start of peer file. */
    AFW_ERROR_FOOTPRINT("apr_file_seek()");
    offset = 0;
    rv = apr_file_seek(peer_f, APR_SET, &offset);
    if (rv != APR_SUCCESS) goto error_peer_apr;

    /* Write to store. */
    AFW_ERROR_FOOTPRINT("apr_file_write()");
    len = encoded->size;
    rv = apr_file_write(peer_f, encoded->ptr, &len);
    if (rv != APR_SUCCESS) goto error_peer_apr;
    if (len != encoded->size) {
        AFW_ERROR_FOOTPRINT("apr_file_write() wrong len");
        goto error_peer_apr;
    }

    /* Truncate file to length written. */
    AFW_ERROR_FOOTPRINT("apr_file_trunc()");
    rv = apr_file_trunc(peer_f, encoded->size);
    if (rv != APR_SUCCESS) goto error_peer_apr;

    /* Close peer object. */
    AFW_ERROR_FOOTPRINT("apr_file_close()");
    rv = apr_file_close(peer_f);
    if (rv != APR_SUCCESS) goto error_peer_apr;

    /* Return. */
    return;

error_peer_apr:
    AFW_THROW_ERROR_FOOTPRINT_RV_FZ(general, apr, rv, xctx,
        "Error detected processing adaptor " AFW_UTF8_FMT_Q " "
        AFW_OBJECT_Q_OBJECT_TYPE_ID_PROVISIONING_PEER
        " file %s - %s",
        AFW_UTF8_FMT_ARG(&adaptor->pub.adaptor_id),
        full_peer_path_z, footprint.z);
}


/*
 * Implementation of method add_entry of interface afw_adaptor_journal.
 */
const afw_utf8_t *
impl_afw_adaptor_journal_add_entry(
    const afw_adaptor_journal_t * instance,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_object_t * entry,
    afw_xctx_t *xctx)
{
    afw_file_internal_adaptor_session_t * session = 
        (afw_file_internal_adaptor_session_t *)instance->session;
    afw_file_internal_adaptor_t *adaptor = session->adaptor;
    const afw_memory_t *encoded;
    afw_memory_t temp_raw;
    apr_status_t rv;
    apr_time_exp_t now;
    const afw_utf8_z_t *relative_entry_path_z;
    const afw_utf8_z_t *full_entry_path_z;
    const afw_utf8_z_t *full_entry_dir_path_z;
    const afw_utf8_z_t *old_full_entry_path_z;
    const afw_utf8_t *first_entry_save_path;
    const afw_utf8_t *cursor;
    apr_file_t *lock_f;
    apr_file_t *entry_f;
    afw_adaptor_journal_lock_t lock;
    apr_size_t lock_len;
    apr_off_t offset;
    afw_endian_big_uint64_t encoded_len_be;
    const afw_pool_t *p = xctx->p;
    apr_pool_t *apr_p = afw_pool_get_apr_pool(p);
    afw_boolean_t first_entry;
    apr_size_t len;
    afw_error_footprint_t footprint;

    /** @fixme add in registers for cleanup. */

    /* Encode object using content type of adaptor. */
    /** @fixme is this the right options to use?  Probably not. */
    encoded = afw_content_type_object_to_raw(
        adaptor->content_type, entry, NULL, xctx->p, xctx);
    encoded_len_be = afw_endian_native_to_big_uint64(encoded->size);

    /* Determine relative path from root for this entry. */
    rv = apr_time_exp_gmt(&now, apr_time_now());
    if (rv != APR_SUCCESS) {
        AFW_ERROR_FOOTPRINT("apr_time_exp_gmt()");
        AFW_THROW_ERROR_RV_Z(general, apr, rv, "apr_time_exp_gmt() failed",
            xctx);
    }
    relative_entry_path_z = afw_utf8_z_printf(p, xctx, 
        "y%04d/m%02d/d%02d/h%02d",
        now.tm_year + 1900, now.tm_mon + 1, now.tm_mday, now.tm_hour);

    /* Open lock file creating it if needed. */
    AFW_ERROR_FOOTPRINT("apr_file_open()");
    rv = apr_file_open(&lock_f, adaptor->journal_lock_file_path_z,
        APR_FOPEN_READ + APR_FOPEN_CREATE + APR_FOPEN_WRITE + 
        APR_FOPEN_BINARY,  APR_FPROT_OS_DEFAULT, apr_p);
    if (APR_STATUS_IS_ENOENT(rv)) {
        /* Create parent directories and try again. */
        AFW_ERROR_FOOTPRINT("apr_dir_make_recursive()");
        rv = apr_dir_make_recursive(adaptor->journal_dir_path_z,
            APR_FPROT_OS_DEFAULT, apr_p);
        if (rv == APR_SUCCESS) {
            /* Try again. */
            AFW_ERROR_FOOTPRINT("apr_file_open()");
            rv = apr_file_open(&lock_f, adaptor->journal_lock_file_path_z,
                APR_FOPEN_READ + APR_FOPEN_CREATE + APR_FOPEN_WRITE +
                APR_FOPEN_BINARY, APR_FPROT_OS_DEFAULT, apr_p);
        }
    }
    if (rv != APR_SUCCESS) goto error_lock_apr;

    /*
     * Read lock record and determine if new lock file.  If it is new,
     * save the first relative_entry_path in path_to_first_entry.
     */
    AFW_ERROR_FOOTPRINT("apr_file_read()");
    lock_len = sizeof(afw_adaptor_journal_lock_t);
    rv = apr_file_read(lock_f, &lock, &lock_len);
    first_entry = rv == APR_EOF;
    if (!first_entry && rv != APR_SUCCESS) goto error_lock_apr;
    if (first_entry) {
        first_entry_save_path = afw_utf8_printf(xctx->p, xctx,
            AFW_UTF8_FMT AFW_OBJECT_Q_OBJECT_TYPE_ID_JOURNAL_ENTRY
            "/path_to_first_journal_file",
            AFW_UTF8_FMT_ARG(adaptor->root));
        temp_raw.ptr = (const afw_byte_t *)relative_entry_path_z;
        temp_raw.size = strlen(relative_entry_path_z);
        afw_file_from_memory(first_entry_save_path, &temp_raw,
            afw_file_mode_write, xctx);
    }
    
    /* Make sure lock record was completely read. */
    AFW_ERROR_FOOTPRINT("check lock record");
    if (rv == APR_SUCCESS && lock_len != sizeof(afw_adaptor_journal_lock_t))
        goto error_lock;

    /*
     * If year, mon, day, and hour are not the same as last time, need to
     * point current journal file last entry to new journal.
     */
    old_full_entry_path_z = NULL;
    if (!first_entry &&
        (  lock.century != now.tm_year / 100 + 19
        || lock.year    != now.tm_year % 100
        || lock.month   != now.tm_mon + 1
        || lock.day     != now.tm_mday
        || lock.hour    != now.tm_hour)
        )
    {
        old_full_entry_path_z = afw_utf8_z_printf(p, xctx,
            AFW_UTF8_FMT AFW_OBJECT_Q_OBJECT_TYPE_ID_JOURNAL_ENTRY
            "/y%02d%02d/m%02d/d%02d/h%02d",
            AFW_UTF8_FMT_ARG(adaptor->root),
            lock.century, lock.year, lock.month, lock.day, lock.hour);
    }

    /* Update lock struct. */
    lock.filler  = 0;
    lock.century = (now.tm_year / 100) + 19;
    lock.year    = now.tm_year % 100;
    lock.month   = now.tm_mon + 1;
    lock.day     = now.tm_mday;
    lock.hour    = now.tm_hour;
    lock.min     = now.tm_min;
    lock.sec     = now.tm_sec;
    lock.usec    = afw_endian_native_to_big_uint32(now.tm_usec);

    /* Path to file that will hold event. */
    full_entry_path_z = afw_utf8_z_printf(p, xctx,
        AFW_UTF8_FMT AFW_OBJECT_Q_OBJECT_TYPE_ID_JOURNAL_ENTRY "/%s",
        AFW_UTF8_FMT_ARG(adaptor->root), relative_entry_path_z);

    /*
     * If switching journal file or first entry, make sure all directories
     * exist.
     */
    if (old_full_entry_path_z || first_entry) {
        full_entry_dir_path_z = afw_utf8_z_printf(p, xctx,
            AFW_UTF8_FMT AFW_OBJECT_Q_OBJECT_TYPE_ID_JOURNAL_ENTRY
            "/y%02d%02d/m%02d/d%02d/",
            AFW_UTF8_FMT_ARG(adaptor->root),
            lock.century, lock.year, lock.month, lock.day);
        AFW_ERROR_FOOTPRINT("apr_dir_make_recursive()");
        rv = apr_dir_make_recursive(full_entry_dir_path_z,
            APR_FPROT_OS_DEFAULT, apr_p);
        if (rv != APR_SUCCESS) goto error_journal_apr;
    }

    /* Open journal entry file. */
    AFW_ERROR_FOOTPRINT("apr_file_open()");
    rv = apr_file_open(&entry_f, full_entry_path_z,
        APR_FOPEN_CREATE +
        APR_FOPEN_WRITE +
        APR_FOPEN_BINARY +
        APR_FOPEN_APPEND,
        APR_FPROT_OS_DEFAULT, apr_p);
    if (rv != APR_SUCCESS) goto error_journal_apr;

    /* Determine cursor of entry. */
    AFW_ERROR_FOOTPRINT("apr_file_seek()");
    offset = 0;
    rv = apr_file_seek(entry_f, APR_CUR, &offset);
    if (rv != APR_SUCCESS) goto error_journal_apr;
    cursor = afw_utf8_printf(xctx->p, xctx,
        "%02d%02d%02d%02d%02d_" AFW_INTEGER_FMT,
        lock.century, lock.year, lock.month, lock.day, lock.hour,
        (afw_integer_t)offset);

    /* Write entry prefix (length). */
    AFW_ERROR_FOOTPRINT("apr_file_write()");
    len = sizeof(encoded_len_be);
    rv = apr_file_write(entry_f, &encoded_len_be, &len);
    if (rv != APR_SUCCESS) goto error_journal_apr;

    /* Write entry . */
    AFW_ERROR_FOOTPRINT("apr_file_write()");
    len = encoded->size;
    rv = apr_file_write(entry_f, encoded->ptr, &len);
    if (rv != APR_SUCCESS) goto error_journal_apr;

    /* Close journal. */
    AFW_ERROR_FOOTPRINT("apr_file_close()");
    rv = apr_file_close(entry_f);
    if (rv != APR_SUCCESS) goto error_journal_apr;

    /*
     * If a switch occurred, write last record of old file with
     * an entry length of 0 followed by relative_entry_path_z
     * of the now new current file.
     */
    if (old_full_entry_path_z) {
        /* Open old journal. */
        AFW_ERROR_FOOTPRINT("apr_file_open()");
        rv = apr_file_open(&entry_f, old_full_entry_path_z,
            APR_FOPEN_WRITE +
            APR_FOPEN_BINARY +
            APR_FOPEN_APPEND,
            APR_FPROT_OS_DEFAULT, apr_p);
        if (rv != APR_SUCCESS) goto error_old_journal_apr;

        /* Write 0 to old journal prefix (length). */
        AFW_ERROR_FOOTPRINT("apr_file_write()");
        len = sizeof(encoded_len_be);
        encoded_len_be.i = 0;
        rv = apr_file_write(entry_f, &encoded_len_be, &len);
        if (rv != APR_SUCCESS) goto error_old_journal_apr;

        /* Write relative path to new journal. */
        AFW_ERROR_FOOTPRINT("apr_file_write()");
        len = strlen(relative_entry_path_z);
        rv = apr_file_write(entry_f, relative_entry_path_z, &len);
        if (rv != APR_SUCCESS) goto error_old_journal_apr;

        /* Close old journal. */
        AFW_ERROR_FOOTPRINT("apr_file_close()");
        rv = apr_file_close(entry_f);
        if (rv != APR_SUCCESS) goto error_old_journal_apr;
    }

    /* Write the new lock struct and close so lock is released. */

    /* Seek to start of lock file. */
    AFW_ERROR_FOOTPRINT("apr_file_seek()");
    offset = 0;
    rv = apr_file_seek(lock_f, APR_SET, &offset);
    if (rv != APR_SUCCESS) goto error_lock_apr;

    /* Replace lock. */
    AFW_ERROR_FOOTPRINT("apr_file_write()");
    len = sizeof(lock);
    rv = apr_file_write(lock_f, &lock, &len);
    if (rv != APR_SUCCESS) goto error_lock_apr;

    /* Close lock file which unblocks new journal entry adds. */
    AFW_ERROR_FOOTPRINT("apr_file_close()");
    rv = apr_file_close(lock_f);
    if (rv != APR_SUCCESS) goto error_lock_apr;

    /* Return cursor. */
    return cursor;

error_lock_apr:
    AFW_THROW_ERROR_FOOTPRINT_RV_FZ(general, apr, rv, xctx,
        "Error detected processing adaptor " AFW_UTF8_FMT_Q
        " journal lock file '%s' - %s",
        AFW_UTF8_FMT_ARG(&adaptor->pub.adaptor_id),
        adaptor->journal_lock_file_path_z, footprint.z);

error_lock:
    AFW_THROW_ERROR_FOOTPRINT_FZ(general, xctx,
        "Error detected while processing  "
        "adaptor " AFW_UTF8_FMT_Q " journal lock file '%s' - %s",
        AFW_UTF8_FMT_ARG(&adaptor->pub.adaptor_id),
        adaptor->journal_lock_file_path_z, footprint.z);

error_journal_apr:
    AFW_THROW_ERROR_FOOTPRINT_RV_FZ(general, apr, rv, xctx,
        "Error detected processing adaptor " AFW_UTF8_FMT_Q
        " journal file '%s' - %s",
        AFW_UTF8_FMT_ARG(&adaptor->pub.adaptor_id),
        full_entry_path_z, footprint.z);

error_old_journal_apr:
    AFW_THROW_ERROR_FOOTPRINT_RV_FZ(general, apr, rv, xctx,
        "Error detected processing adaptor " AFW_UTF8_FMT_Q
        " journal file '%s' - %s",
        AFW_UTF8_FMT_ARG(&adaptor->pub.adaptor_id),
        old_full_entry_path_z, footprint.z);
}


/*
 * Implementation of method get_entry of interface afw_adaptor_journal.
 */
void
impl_afw_adaptor_journal_get_entry(
    const afw_adaptor_journal_t * instance,
    const afw_adaptor_impl_request_t * impl_request,
    afw_adaptor_journal_option_t option,
    const afw_utf8_t * consumer_id,
    const afw_utf8_t * entry_cursor,
    afw_size_t limit,
    const afw_object_t * response,
    afw_xctx_t *xctx)
{
    afw_file_internal_adaptor_session_t * session =
        (afw_file_internal_adaptor_session_t *)instance->session;
    afw_file_internal_adaptor_t *adaptor = session->adaptor;
    const afw_pool_t *p = xctx->p;
    apr_pool_t *apr_p = afw_pool_get_apr_pool(p);

    afw_error_footprint_t footprint;
    apr_file_t *entry_f;
    apr_file_t *peer_f;
    const afw_utf8_t *entry_object_id;
    const afw_object_t *peer;
    const afw_object_t *entry;
    const afw_value_t *value;
    const afw_utf8_t *currentCursor;
    const afw_utf8_t *consumeCursor;
    const afw_utf8_t *advanceCursor;
    const afw_utf8_t *first_entry_save_path;
    afw_boolean_t get_first;
    afw_boolean_t skip_first_entry;
    afw_boolean_t use_consumer_cursors;
    afw_boolean_t advance_consumer_cursor;
    afw_boolean_t use_consumer;
    afw_boolean_t reissue;
    afw_boolean_t check_filter;
    afw_boolean_t open_journal;
    afw_boolean_t applicable;
    const afw_utf8_z_t *relative_entry_path_z;
    afw_utf8_z_t relative_entry_path_wa_z[IMPL_RELATIVE_ENTRY_PATH_WA_Z_SIZE];
    const afw_utf8_z_t *full_entry_path_z;
    const afw_utf8_z_t *full_peer_path_z;
    afw_off_t offset;
    afw_size_t len;
    apr_status_t rv;
    afw_endian_big_uint64_t encoded_len_be;
    afw_size_t encoded_len;
    void *memory;
    afw_memory_t buffer;
    const afw_value_t * filter;
    const afw_dateTime_t *now;

    /* Initialize variables. */
    get_first = false;
    skip_first_entry = false;
    reissue = false;
    use_consumer = false;
    use_consumer_cursors = false;
    advance_consumer_cursor = false;
    check_filter = false;
    applicable = false;
    entry_object_id = NULL;
    filter = NULL;
    peer_f = NULL;
    peer = NULL;
    full_peer_path_z = NULL;
    full_entry_path_z = NULL;
    entry_f = NULL;
    entry = NULL;

    /* Set variables based on options */
    switch (option) {

    case afw_adaptor_journal_option_get_first:
        get_first = true;
        break;

    case afw_adaptor_journal_option_get_next_after_cursor:
        skip_first_entry = true;
        entry_object_id = entry_cursor;
        break;

    case afw_adaptor_journal_option_get_next_for_consumer:
        use_consumer = true;
        use_consumer_cursors = true;
        check_filter = true;
        break;

    case afw_adaptor_journal_option_get_next_for_consumer_after_cursor:
        skip_first_entry = true;
        entry_object_id = entry_cursor;
        use_consumer = true;
        check_filter = true;
        break;

    case afw_adaptor_journal_option_advance_cursor_for_consumer:
        use_consumer = true;
        check_filter = true;
        advance_consumer_cursor = true;
        break;

    case afw_adaptor_journal_option_get_by_cursor:
        entry_object_id = entry_cursor;
        break;

    default:
        AFW_THROW_ERROR_FZ(general, xctx,
            "Invalid option %d passed to afw_adaptor_journal_get_entry()",
            option);
    };

    /* If using peer object, get it and set variables as appropriate. */
    if (use_consumer) {
        
        /* Get peer object. */
        peer = impl_open_and_retrieve_peer_object(instance, consumer_id,
            &peer_f, &full_peer_path_z, xctx);

        /* If using consumer cursors, set variables as appropriate. */
        if (use_consumer_cursors) {
            currentCursor = afw_object_old_get_property_as_string(peer,
                &afw_s_currentCursor, xctx);
            consumeCursor = afw_object_old_get_property_as_string(peer,
                &afw_s_consumeCursor, xctx);
            advanceCursor = afw_object_old_get_property_as_string(peer,
                &afw_s_advanceCursor, xctx);
            if (consumeCursor) {
                entry_object_id = consumeCursor;
                check_filter = false;
                reissue = true;
            } 
            else if (advanceCursor) {
                entry_object_id = advanceCursor;
                skip_first_entry = false;
            }
            else if (currentCursor) {
                entry_object_id = currentCursor;
                skip_first_entry = true;
            }
            else {
                get_first = true;
            }
        }
    }

    /* If get_first, get path to first journal file. */
    if (get_first) {
        first_entry_save_path = afw_utf8_printf(xctx->p, xctx,
            AFW_UTF8_FMT AFW_OBJECT_Q_OBJECT_TYPE_ID_JOURNAL_ENTRY
            "/path_to_first_journal_file",
            AFW_UTF8_FMT_ARG(adaptor->root));
        relative_entry_path_z = afw_utf8_to_utf8_z(
            afw_utf8_from_raw(
                afw_file_to_memory(first_entry_save_path, 0, p, xctx),
                p, xctx),
            p, xctx);
        offset = 0;
    }

    /* Convert entry_object_id and offset to relative journal file path. */
    else if (entry_object_id) {
        relative_entry_path_z = impl_object_id_to_relative_entry_path(
            entry_object_id, relative_entry_path_wa_z, &offset, xctx);
        if (!relative_entry_path_z) return; /** @fixme Is this an error?*/
    }

    else {
        AFW_THROW_ERROR_Z(general,
            "get_first or entry_object_id required",
            xctx);
    }

    /* Loop until end or an applicable entry found. */
    open_journal = true;
    for (;;) {

        /* Make entry_object_id for the entry. */
        entry_object_id = impl_relative_entry_path_to_object_id(
            relative_entry_path_z, offset, xctx);

        /* If needed, open journal file. */
        if (open_journal) {
            open_journal = false;
            full_entry_path_z = afw_utf8_z_concat(p, xctx,
                adaptor->journal_dir_path_z,
                relative_entry_path_z, NULL);
            AFW_ERROR_FOOTPRINT("apr_file_open()");
            rv = apr_file_open(&entry_f, full_entry_path_z,
                APR_FOPEN_READ + APR_FOPEN_BINARY, APR_FPROT_OS_DEFAULT,
                apr_p);
            if (rv != APR_SUCCESS) goto error_journal_apr;
        }

        /* Position to offset and read length. */
        AFW_ERROR_FOOTPRINT("apr_file_seek()");
        rv = apr_file_seek(entry_f, APR_SET, &offset);
        if (rv != APR_SUCCESS) goto error_journal_apr;
        len = sizeof(encoded_len_be);
        AFW_ERROR_FOOTPRINT("apr_file_read()");
        rv = apr_file_read(entry_f, &encoded_len_be, &len);
        if (APR_STATUS_IS_EOF(rv)) break;  /* Nothing applicable. */
        if (rv != APR_SUCCESS)  goto error_journal_apr;
        AFW_ERROR_FOOTPRINT("check if encoded_len_be truncated");
        if (len != sizeof(encoded_len_be)) goto error_journal;
        encoded_len = afw_endian_safe_big_uint64_to_native_size_t(
            encoded_len_be, xctx);
        len = encoded_len;

        /* If length is 0, switch to next journal file. */
        if (len == 0) {
            AFW_ERROR_FOOTPRINT("apr_file_read()");
            len = sizeof(relative_entry_path_wa_z);
            rv = apr_file_read(entry_f, &relative_entry_path_wa_z[0], &len);
            if (rv != APR_SUCCESS) goto error_journal_apr;
            relative_entry_path_z = &relative_entry_path_wa_z[0];
            AFW_ERROR_FOOTPRINT("apr_file_close()");
            rv = apr_file_close(entry_f);
            if (rv != APR_SUCCESS) goto error_journal_apr;
            offset = 0;
            open_journal = true;
            continue;
        }

        /* If not skipping first entry, read and process it. */
        if (!skip_first_entry) {
  
            /* Read journal entry. */
            memory = afw_xctx_calloc(len, xctx);
            buffer.size = len;
            buffer.ptr = memory;
            AFW_ERROR_FOOTPRINT("apr_file_read()");
            rv = apr_file_read(entry_f, memory, &len);
            if (rv != APR_SUCCESS) goto error_journal_apr;
            AFW_ERROR_FOOTPRINT("check that all of encoded entry read");
            if (len != buffer.size) goto error_journal;
            value = afw_content_type_raw_to_value(adaptor->content_type, &buffer,
                consumer_id, p, xctx);
            AFW_ERROR_FOOTPRINT("check that converted entry is an object");
            if (!afw_value_is_object(value)) goto error_journal;
            entry = ((const afw_value_object_t *)value)->internal;
              
            /* If reissue, set reissue property and mark applicable. */
            if (reissue) {
                applicable = true;
                afw_object_set_property_as_boolean(response, &afw_s_reissue,
                    true, xctx);
            }

            /* Else if check_filter, set applicable using filter. */
            else if (check_filter) {
                applicable = afw_adaptor_impl_is_journal_entry_applicable(
                    instance, entry, peer, &filter, xctx);
            }

            /* Else entry is applicable. */
            else {
                applicable = true;
            }

            /* If applicable entry, leave loop. */
            if (applicable) break;
        }

        /* Set offset for next loop. */
        skip_first_entry = false;
        offset = encoded_len + offset + sizeof(encoded_len_be);
    };

    AFW_ERROR_FOOTPRINT("apr_file_close()");
    rv = apr_file_close(entry_f);
    if (rv != APR_SUCCESS) goto error_journal_apr;


    /* If use_consumer, finishup. */
    if (use_consumer) {

        /* Set peer lastContactTime to now. */
        now = afw_dateTime_now_utc(p, xctx);
        afw_object_set_property_as_dateTime(peer,
            &afw_s_lastContactTime, now, xctx);

        /* If advance_consumer_cursor, set peer advanceCursor */
        if (advance_consumer_cursor) {
            afw_object_set_property_as_string(peer,
                &afw_s_advanceCursor, entry_object_id, xctx);
        }

        /* If use_consumer_cursors and not reissue, update peer cursors. */
        else if (use_consumer_cursors && !reissue) {

            /*
             * If applicable, set currentCursor and consumeCursor and remove
             * advanceCursor.
             */
            if (applicable) {
                afw_object_set_property_as_string(peer,
                    &afw_s_currentCursor, entry_object_id, xctx);
                afw_object_set_property_as_string(peer,
                    &afw_s_consumeCursor, entry_object_id, xctx);
                afw_object_set_property_as_dateTime(peer,
                    &afw_s_consumeStartTime, now, xctx);
                afw_object_remove_property(peer, &afw_s_advanceCursor,
                    xctx);
            }

            /* If not applicable, set advance cursor to to point to eof. */
            else {
                afw_object_set_property_as_string(peer,
                    &afw_s_advanceCursor, entry_object_id, xctx);
            }

        }

        /* Replace peer object and close. */
        impl_write_and_close_peer_object(instance, peer, peer_f,
            full_peer_path_z, xctx);
 
    }

    /* Set entryCursor property. */
    afw_object_set_property_as_string(response, &afw_s_entryCursor,
        entry_object_id, xctx);

    /*
     * Return entry if there is an applicable one and not
     * advance_consumer_cursor request.
     */
    if (applicable && !advance_consumer_cursor) {
        afw_object_set_property_as_object(response, &afw_s_entry, entry,
            xctx);
    }
    return;

error_journal_apr:
    AFW_THROW_ERROR_FOOTPRINT_RV_FZ(general, apr, rv, xctx,
        "Error detected processing adaptor " AFW_UTF8_FMT_Q
            " journal file '%s' - %s",
        adaptor->pub.adaptor_id.len, adaptor->pub.adaptor_id.s,
        full_entry_path_z, footprint.z);

error_journal:
    AFW_THROW_ERROR_FOOTPRINT_FZ(general, xctx,
        "Error detected while processing  "
        "adaptor " AFW_UTF8_FMT_Q " journal file '%s' - %s",
        AFW_UTF8_FMT_ARG(&adaptor->pub.adaptor_id),
        full_entry_path_z, footprint.z);

}


/*
 * Implementation of method mark_entry_consumed of interface afw_adaptor_journal.
 */
void
impl_afw_adaptor_journal_mark_entry_consumed(
    const afw_adaptor_journal_t * instance,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * consumer_id,
    const afw_utf8_t * entry_cursor,
    afw_xctx_t *xctx)
{
    const afw_object_t *peer;
    apr_file_t *peer_f;
    const afw_utf8_z_t *full_peer_path_z;
    const afw_utf8_t *consume_cursor;
    const afw_dateTime_t *now;


    peer = impl_open_and_retrieve_peer_object(instance,
        consumer_id, &peer_f,
        &full_peer_path_z, xctx);

    consume_cursor = afw_object_old_get_property_as_string(peer,
        &afw_s_consumeCursor, xctx);
    if (!consume_cursor || !afw_utf8_equal(entry_cursor, consume_cursor)) {
        AFW_THROW_ERROR_Z(general,
            "Object id supplied is not currently being consumed", xctx);
    }

    /* Remove consume* properties. */
    afw_object_remove_property(peer, &afw_s_consumeCursor, xctx);
    afw_object_remove_property(peer, &afw_s_consumeStartTime, xctx);

    /* Update lastContactTime property. */
    now = afw_dateTime_now_utc(xctx->p, xctx);
    afw_object_set_property_as_dateTime(peer, &afw_s_lastContactTime,
        now, xctx);

    /* Write peer object and close. */
    impl_write_and_close_peer_object(instance, peer, peer_f, full_peer_path_z,
        xctx);
}
