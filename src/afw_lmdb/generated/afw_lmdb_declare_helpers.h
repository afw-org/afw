// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Declare Helpers for afw_lmdb_
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/*  ----------------------------- N O T E -------------------------------------
 *
 * This file is generated by "afwdev generate afw_lmdb".
 *
 * Do not change this file directly or the changes will be lost the next time
 * this file is regenerated.
 *
 * -------------------------------------------------------------------------*/


#ifndef __AFW_LMDB_DECLARE_HELPERS_H__
#define __AFW_LMDB_DECLARE_HELPERS_H__


/**
 * @file afw_lmdb_declare_helpers.h
 * @brief Adaptive Framework declare helpers for afw_lmdb_
 */


#include <apr_general.h>


/**
 * AFW_LMDB_BEGIN_DECLARES goes before declares and 
 * AFW_LMDB_END_DECLARES at end in afw_lmdb*.h files.
 */
#ifdef __cplusplus
#define AFW_LMDB_BEGIN_DECLARES extern "C" {
#define AFW_LMDB_END_DECLARES }
#else
#define AFW_LMDB_BEGIN_DECLARES
#define AFW_LMDB_END_DECLARES
#endif


/* #defines for declspec and calling convention. */
#if defined(DOXYGEN) || !defined(WIN32)
#define AFW_LMDB_DECLSPEC_DECLARE extern
#define AFW_LMDB_DECLSPEC_DEFINE
#define AFW_LMDB_CALLING_CONVENTION
#define AFW_LMDB_CALLING_CONVENTION_ELLIPSIS
#elif defined(AFW_LMDB_DECLARE_STATIC)
#define AFW_LMDB_DECLSPEC_DECLARE extern
#define AFW_LMDB_DECLSPEC_DEFINE
#define AFW_LMDB_CALLING_CONVENTION __stdcall
#define AFW_LMDB_CALLING_CONVENTION_ELLIPSIS __cdecl
#elif defined(AFW_LMDB_DECLARE_EXPORT)
#define AFW_LMDB_DECLSPEC_DECLARE extern __declspec(dllexport)
#define AFW_LMDB_DECLSPEC_DEFINE __declspec(dllexport)
#define AFW_LMDB_CALLING_CONVENTION __stdcall
#define AFW_LMDB_CALLING_CONVENTION_ELLIPSIS __cdecl
#else
#define AFW_LMDB_DECLSPEC_DECLARE extern __declspec(dllimport)
#define AFW_LMDB_DECLSPEC_DEFINE __declspec(dllimport)
#define AFW_LMDB_CALLING_CONVENTION __stdcall
#define AFW_LMDB_CALLING_CONVENTION_ELLIPSIS __cdecl
#endif


/**
 * @brief Declare an internal variable for /src/afw_lmdb/ source*.h files.
 * @param type of variable
 *
 * Use this macro to supply the storage specifier of a variable in a .h file
 * that is only intended to be accessed internally to a linked object.  There
 * must be a corresponding AFW_LMDB_DEFINE_INTERNAL_CONST_DATA()
 * in a /src/afw_lmdb/ source*.h files.
 * 
 * Any data declared this way is not considered part of the external API.
 */
#define AFW_LMDB_DECLARE_INTERNAL_CONST_DATA(type) extern const type


/**
 * @brief Define an internal variable for /src/afw_lmdb/ source*.c files.
 * @param type of variable
 *
 * Use this macro to supply the storage specifier of a variable in a .c files
 * that is only intended to be accessed internally to a linked object.  There
 * must be a corresponding AFW_LMDB_DECLARE_INTERNAL_CONST_DATA()
 * in a /src/afw_lmdb/ source*.h files.
 * 
 * Any data defined this way is not considered part of the external API. 
 */
#define AFW_LMDB_DEFINE_INTERNAL_CONST_DATA(type) const type


/**
 * @brief Declare an internal function for /src/afw_lmdb/ source*.h files.
 * @param type of return value.
 *
 * Use this macro to supply the storage specifier of a function in a .h file
 * that is only intended to be accessed internally to a linked object.  There
 * must be a corresponding AFW_LMDB_DEFINE_INTERNAL()
 * in a /src/afw_lmdb/ source*.c files.
 * 
 * Any function declared this way is not considered part of the external API.
 */
#define AFW_LMDB_DECLARE_INTERNAL(type) extern type


/**
 * @brief Define an internal function for /src/afw_lmdb/ source*.c files.
 * @param type of return value.
 *
 * Use this macro to supply the storage specifier of a function in a .c files
 * that is only intended to be accessed internally to a linked object.  There
 * must be a corresponding AFW_LMDB_DECLARE_INTERNAL()
 * in a /src/afw_lmdb/ source*.h files.
 * 
 * Any function defined this way is not considered part of the external API. 
 */
#define AFW_LMDB_DEFINE_INTERNAL(type) type


/**
 * @brief Declare a public afw_lmdb function.
 * @param type of return value.
 *
 * There must be a corresponding AFW_LMDB_DEFINE()
 * in a /src/afw_lmdb/ source*.c files.
 *
 * Example:
 *
 * const afw_utf8_t * afw_lmdb_some_function(int a);
 *
 * is decorated this way:
 *
 * AFW_LMDB_DECLARE(const afw_utf8_t *)
 * afw_lmdb_some_function(int a);
 */
#define AFW_LMDB_DECLARE(type) \
AFW_LMDB_DECLSPEC_DECLARE \
type \
AFW_LMDB_CALLING_CONVENTION


/**
 * @brief Declare a public afw_lmdb function with variable arguments.
 * @param type of return value.
 *
 * There must be a corresponding AFW_LMDB_DEFINE_ELLIPSIS()
 * in a /src/afw_lmdb/ source*.c files.
 *
 * Example:
 *
 * const afw_utf8_t *
 * afw_lmdb_some_function(int a, ...);
 *
 * is decorated this way:
 *
 * AFW_LMDB_DECLARE_ELLIPSIS(const afw_utf8_t *)
 * afw_lmdb_some_function(int a, ...);
 */
#define AFW_LMDB_DECLARE_ELLIPSIS(type) \
AFW_LMDB_DECLSPEC_DECLARE \
type \
AFW_LMDB_CALLING_CONVENTION_ELLIPSIS


/**
 * @brief Declare a public afw_lmdb variable.
 * @param type of variable
 *
 * There must be a corresponding AFW_LMDB_DEFINE_CONST_DATA
 * in a /src/afw_lmdb/ source*.c files.
 *
 * Example:
 *
 * extern const afw_utf8_t *
 * afw_lmdb_a_string;
 *
 * is decorated this way:
 *
 * AFW_LMDB_DECLARE_CONST_DATA(afw_utf8_t *)
 * afw_lmdb_a_string;
 */
#define AFW_LMDB_DECLARE_CONST_DATA(type) \
AFW_LMDB_DECLSPEC_DECLARE \
const type


/**
 * @brief Define a public afw_lmdb function.
 * @param type of return value.
 *
 * There must be a corresponding AFW_LMDB_DECLARE()
 * in a /src/afw_lmdb/ source*.h files.
 *
 * Example:
 *
 * const afw_utf8_t *
 * afw_lmdb_some_function(int a) {...}
 *
 * is decorated this way:
 *
 * AFW_LMDB_DEFINE(const afw_utf8_t *)
 * afw_lmdb_some_function(int a) {...}
 */
#define AFW_LMDB_DEFINE(type) \
AFW_LMDB_DECLSPEC_DEFINE \
type \
AFW_LMDB_CALLING_CONVENTION


/**
 * @brief Define a public afw_lmdb function with variable arguments.
 * @param type of return value.
 *
 * There must be a corresponding AFW_LMDB_DECLARE_ELLIPSIS()
 * in a /src/afw_lmdb/ source*.h files.
 *
 * Example:
 *
 * const afw_utf8_t * 
 * afw_lmdb_some_function(int a, ...) {...}
 *
 * is decorated this way:
 *
 * AFW_LMDB_DEFINE_ELLIPSIS(const afw_utf8_t *) 
 * afw_lmdb_some_function(int a, ...) {...}
 */
#define AFW_LMDB_DEFINE_ELLIPSIS(type) \
AFW_LMDB_DECLSPEC_DEFINE \
type \
AFW_LMDB_CALLING_CONVENTION_ELLIPSIS


/**
 * @brief Define a public afw_lmdb variable.
 * @param type of variable
 *
 * There must be a corresponding AFW_LMDB_DECLARE_CONST_DATA
 * in a /src/afw_lmdb/ source*.h files.
 *
 * Example:
 *
 * const afw_utf8_t *
 * afw_lmdb_a_string;
 *
 * is decorated this way:
 *
 * AFW_LMDB_DEFINE_CONST_DATA(afw_utf8_t *)
 * afw_lmdb_a_string;
 */
#define AFW_LMDB_DEFINE_CONST_DATA(type) \
AFW_LMDB_DECLSPEC_DEFINE \
const type


#define AFW_LMDB_CALLBACK \
AFW_LMDB_CALLING_CONVENTION


#define AFW_LMDB_CALLBACK_ELLIPSIS \
AFW_LMDB_CALLING_CONVENTION_ELLIPSIS


#ifdef WIN32
#define AFW_LMDB_DEFINE_DSO(type) __declspec(dllexport) type
#else
#define AFW_LMDB_DEFINE_DSO(type) type
#endif


#ifdef AFW_LMDB_DISABLE_INLINE
#define AFW_LMDB_HAS_INLINE 0
#define AFW_LMDB_INLINE
#else
#define AFW_LMDB_HAS_INLINE APR_HAS_INLINE
#define AFW_LMDB_INLINE APR_INLINE
#endif


/** AFW Inline. */
#define AFW_LMDB_STATIC_INLINE static AFW_LMDB_INLINE

#define AFW_LMDB_DECLARE_STATIC(type) static type

#if defined(DOXYGEN)
#define AFW_LMDB_DEFINE_STATIC_INLINE(type) type
#else
#define AFW_LMDB_DEFINE_STATIC_INLINE(type) AFW_LMDB_STATIC_INLINE type
#endif



#endif /* __AFW_LMDB_DECLARE_HELPERS_H__ */
