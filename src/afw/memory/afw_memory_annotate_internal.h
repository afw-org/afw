// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework memory checker annotations.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_MEMORY_ANNOTATE_INTERNAL_H__
#define __AFW_MEMORY_ANNOTATE_INTERNAL_H__

/**
 * @file afw_memory_annotate_internal.h
 * @brief Tell a memory checker which pool bytes are live.
 *
 * Heaps carve mapped chunks themselves, so a checker sees a whole
 * chunk as valid memory. These macros mark bytes no-access when they
 * are not part of a live block and accessible when they are handed
 * out. The checker then reports a read or write of freed pool
 * memory, of the slack after a block, or of a destroyed heap's
 * chunks.
 *
 * Accessible: chunk headers, free-node headers, tracker node
 * headers, and live blocks up to the asked-for size. No-access: the
 * rest of a free block, the slack after a live block, unused bump,
 * and region free-list chunks past their node header.
 *
 * LeakSanitizer does not look for pointers in mapped pages. A heap
 * chunk is a root region while its heap holds it, so C memory that
 * only pool memory points to (the base thread, regions, library
 * handles) is not reported as leaked.
 *
 * Backends:
 * - AddressSanitizer, when the compiler builds with -fsanitize=address
 *   (gcc `__SANITIZE_ADDRESS__`, clang
 *   `__has_feature(address_sanitizer)`).
 * - Valgrind memcheck, when built with `--define AFW_VALGRIND_POOL`
 *   (and not ASan). Client requests are a few no-op instructions when
 *   the process is not running under valgrind. Valgrind also tracks
 *   whether bytes were written: ACCESS marks bytes addressable and
 *   defined (allocator bookkeeping), and UNDEFINED marks the bytes a
 *   malloc hands to its caller as not yet written, so a read of pool
 *   memory nobody wrote is reported. ROOT / UNROOT are no-ops: the
 *   valgrind leak checker already scans mapped memory.
 * Otherwise `AFW_MEMORY_ANNOTATE_ACTIVE` is 0 and every macro is a
 * no-op, so a normal build is unchanged. See `designs/asan-opt-in.md`.
 */

#if defined(__SANITIZE_ADDRESS__)
#define AFW_MEMORY_ANNOTATE_ACTIVE 1
#elif defined(__has_feature)
#if __has_feature(address_sanitizer)
#define AFW_MEMORY_ANNOTATE_ACTIVE 1
#endif
#endif

#if !defined(AFW_MEMORY_ANNOTATE_ACTIVE) && defined(AFW_VALGRIND_POOL)
#define AFW_MEMORY_ANNOTATE_ACTIVE 1
#define AFW_MEMORY_ANNOTATE_VALGRIND 1
#endif

#ifndef AFW_MEMORY_ANNOTATE_ACTIVE
#define AFW_MEMORY_ANNOTATE_ACTIVE 0
#endif

#if AFW_MEMORY_ANNOTATE_ACTIVE && defined(AFW_MEMORY_ANNOTATE_VALGRIND)

#include <valgrind/memcheck.h>

#define AFW_MEMORY_ANNOTATE_NOACCESS(_addr, _size) \
    ((void)VALGRIND_MAKE_MEM_NOACCESS((_addr), (_size)))

#define AFW_MEMORY_ANNOTATE_ACCESS(_addr, _size) \
    ((void)VALGRIND_MAKE_MEM_DEFINED((_addr), (_size)))

#define AFW_MEMORY_ANNOTATE_UNDEFINED(_addr, _size) \
    ((void)VALGRIND_MAKE_MEM_UNDEFINED((_addr), (_size)))

#define AFW_MEMORY_ANNOTATE_ROOT(_addr, _size) ((void)0)
#define AFW_MEMORY_ANNOTATE_UNROOT(_addr, _size) ((void)0)

#elif AFW_MEMORY_ANNOTATE_ACTIVE

#include <sanitizer/asan_interface.h>
#include <sanitizer/lsan_interface.h>

/** @brief Mark [_addr, _addr + _size) no-access. */
#define AFW_MEMORY_ANNOTATE_NOACCESS(_addr, _size) \
    ASAN_POISON_MEMORY_REGION((_addr), (_size))

/** @brief Mark [_addr, _addr + _size) accessible. */
#define AFW_MEMORY_ANNOTATE_ACCESS(_addr, _size) \
    ASAN_UNPOISON_MEMORY_REGION((_addr), (_size))

/** @brief Bytes handed to a malloc caller, not yet written (valgrind). */
#define AFW_MEMORY_ANNOTATE_UNDEFINED(_addr, _size) ((void)0)

/** @brief Leak checker scans [_addr, _addr + _size) for pointers. */
#define AFW_MEMORY_ANNOTATE_ROOT(_addr, _size) \
    __lsan_register_root_region((_addr), (_size))

/** @brief Undo AFW_MEMORY_ANNOTATE_ROOT with the same address and size. */
#define AFW_MEMORY_ANNOTATE_UNROOT(_addr, _size) \
    __lsan_unregister_root_region((_addr), (_size))

#else

#define AFW_MEMORY_ANNOTATE_NOACCESS(_addr, _size) ((void)0)
#define AFW_MEMORY_ANNOTATE_ACCESS(_addr, _size) ((void)0)
#define AFW_MEMORY_ANNOTATE_UNDEFINED(_addr, _size) ((void)0)
#define AFW_MEMORY_ANNOTATE_ROOT(_addr, _size) ((void)0)
#define AFW_MEMORY_ANNOTATE_UNROOT(_addr, _size) ((void)0)

#endif

#endif /* __AFW_MEMORY_ANNOTATE_INTERNAL_H__ */
