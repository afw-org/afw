// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Core Adaptive Function Bindings 
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



/**
 * @file afw_lmdb_function_bindings.c
 * @brief Adaptive Framework core adaptive function bindings.
 */

#include "afw.h"
#include "afw_lmdb_function_bindings.h"
#include "afw_lmdb_runtime_object_maps.h"
#include "afw_lmdb_strings.h"


/* ---------- reader_check ---------- */

static const afw_value_function_definition_t
impl_reader_check;

static const afw_utf8_t
impl_object_path__reader_check =
    AFW_UTF8_LITERAL("/afw/_AdaptiveFunction_/reader_check");

static const afw_runtime_object_indirect_t
impl_object__reader_check = {
    {
        NULL,
        NULL,
        {
            NULL,
            NULL,
            &afw_lmdb_s_reader_check,
            &afw_lmdb_s__AdaptiveFunction_,
            &impl_object_path__reader_check
        }
    },
    (void *)&impl_reader_check
};

static const afw_value_function_parameter_t
impl_reader_check_returns = {
    {
        NULL,
        NULL,
        {
            NULL,
            NULL,
            NULL,
            &afw_lmdb_s__AdaptiveFunctionParameter_,
            NULL,
        }
    },
    NULL,
    &afw_lmdb_v_integer,
    NULL,
    NULL,
    NULL,
    &afw_lmdb_v__g__Number_of_stale_slots_that_were_cleared_,
    &afw_lmdb_integer_v__g___1,
    &afw_lmdb_boolean_v_false,
    &afw_lmdb_boolean_v_false,
    &afw_lmdb_boolean_v_false,
    &afw_lmdb_boolean_v_false,
};

static const afw_value_function_parameter_t
impl_reader_check_parameter_1 = {
    {
        NULL,
        NULL,
        {
            NULL,
            NULL,
            NULL,
            &afw_lmdb_s__AdaptiveFunctionParameter_,
            NULL,
        }
    },
    NULL,
    &afw_lmdb_v_string,
    NULL,
    &afw_lmdb_v_adaptorId,
    NULL,
    &afw_lmdb_v__g__Adaptor_Id_,
    &afw_lmdb_integer_v__g___1,
    &afw_lmdb_boolean_v_false,
    &afw_lmdb_boolean_v_false,
    &afw_lmdb_boolean_v_false,
    &afw_lmdb_boolean_v_false,
};

static const afw_value_function_parameter_t *
impl_reader_check_parameters[] = {
    &impl_reader_check_parameter_1,
    NULL
};

static const afw_value_function_definition_t
impl_reader_check = {
    NULL,
    (const afw_object_t *)&impl_object__reader_check,
    &afw_lmdb_v_lmdb,
    &afw_lmdb_v_reader_check,
    &afw_lmdb_v_reader_check,
    &afw_lmdb_v_reader_check,
    &afw_lmdb_v_readerCheck,
    &afw_lmdb_v_afwReaderCheck,
    NULL,
    &afw_lmdb_v__g__Check_for_stale_entries_in_the_reader_lock_table_,
    &afw_lmdb_v__g___adaptorId__string___integer,
    &afw_lmdb_v__g__function_reader_check_______adaptorId__string____integer__,
    AFW_UTF8_LITERAL(""),
    afw_lmdb_function_execute_reader_check,
    NULL,
    1,
    1,
    &impl_reader_check_parameters[0],
    1,
    &impl_reader_check_returns,
    NULL,
    NULL,
    AFW_UTF8_LITERAL(""),
    NULL,
    NULL,
    0, /* Not a data type method. */
    0,
    false,
    false,
    false,
    false,
    false
};

/* ---------- reader_list ---------- */

static const afw_value_function_definition_t
impl_reader_list;

static const afw_utf8_t
impl_object_path__reader_list =
    AFW_UTF8_LITERAL("/afw/_AdaptiveFunction_/reader_list");

static const afw_runtime_object_indirect_t
impl_object__reader_list = {
    {
        NULL,
        NULL,
        {
            NULL,
            NULL,
            &afw_lmdb_s_reader_list,
            &afw_lmdb_s__AdaptiveFunction_,
            &impl_object_path__reader_list
        }
    },
    (void *)&impl_reader_list
};

static const afw_value_function_parameter_t
impl_reader_list_returns = {
    {
        NULL,
        NULL,
        {
            NULL,
            NULL,
            NULL,
            &afw_lmdb_s__AdaptiveFunctionParameter_,
            NULL,
        }
    },
    NULL,
    &afw_lmdb_v_string,
    NULL,
    NULL,
    NULL,
    &afw_lmdb_v__g__Reader_entries_,
    &afw_lmdb_integer_v__g___1,
    &afw_lmdb_boolean_v_false,
    &afw_lmdb_boolean_v_false,
    &afw_lmdb_boolean_v_false,
    &afw_lmdb_boolean_v_false,
};

static const afw_value_function_parameter_t
impl_reader_list_parameter_1 = {
    {
        NULL,
        NULL,
        {
            NULL,
            NULL,
            NULL,
            &afw_lmdb_s__AdaptiveFunctionParameter_,
            NULL,
        }
    },
    NULL,
    &afw_lmdb_v_string,
    NULL,
    &afw_lmdb_v_adaptorId,
    NULL,
    &afw_lmdb_v__g__Adaptor_Id_,
    &afw_lmdb_integer_v__g___1,
    &afw_lmdb_boolean_v_false,
    &afw_lmdb_boolean_v_false,
    &afw_lmdb_boolean_v_false,
    &afw_lmdb_boolean_v_false,
};

static const afw_value_function_parameter_t *
impl_reader_list_parameters[] = {
    &impl_reader_list_parameter_1,
    NULL
};

static const afw_value_function_definition_t
impl_reader_list = {
    NULL,
    (const afw_object_t *)&impl_object__reader_list,
    &afw_lmdb_v_lmdb,
    &afw_lmdb_v_reader_list,
    &afw_lmdb_v_reader_list,
    &afw_lmdb_v_reader_list,
    &afw_lmdb_v_readerList,
    &afw_lmdb_v_afwReaderList,
    NULL,
    &afw_lmdb_v__g__Dump_the_entries_in_the_reader_lock_table_,
    &afw_lmdb_v__g___adaptorId__string___string,
    &afw_lmdb_v__g__function_reader_list_______adaptorId__string____string__,
    AFW_UTF8_LITERAL(""),
    afw_lmdb_function_execute_reader_list,
    NULL,
    1,
    1,
    &impl_reader_list_parameters[0],
    1,
    &impl_reader_list_returns,
    NULL,
    NULL,
    AFW_UTF8_LITERAL(""),
    NULL,
    NULL,
    0, /* Not a data type method. */
    0,
    false,
    false,
    false,
    false,
    false
};

static const afw_value_function_definition_t * 
impl_function_bindings[] = {
    &impl_reader_check,
    &impl_reader_list,
    NULL
};

/* Get array of pointers to afw_lmdb_function bindings. */
AFW_LMDB_DEFINE(const afw_value_function_definition_t **)
afw_lmdb_function_bindings_get()
{
    return &impl_function_bindings[0];
}
