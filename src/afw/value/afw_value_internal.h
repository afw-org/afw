// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Internal Header for afw_value*
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_VALUE_INTERNAL_H__
#define __AFW_VALUE_INTERNAL_H__

#include "afw_interface.h"
#include "afw_object_internal.h"

/**
 * @defgroup afw_value_internal Value
 * @ingroup afw_c_api_internal
 *
 * Adaptive Framework Value internal API.
 * @{
 */

/**
 * @file afw_value_internal.h
 * @brief Internal header for afw_value*.
 */

AFW_BEGIN_DECLARES

/* AFW_VALUE_SCRIPT_SUPPORT_NUMBER_NONE must be 0. */
#define AFW_VALUE_SCRIPT_SUPPORT_NUMBER_NONE      0
#define AFW_VALUE_SCRIPT_SUPPORT_NUMBER_ASSIGN    1
#define AFW_VALUE_SCRIPT_SUPPORT_NUMBER_BREAK     2
#define AFW_VALUE_SCRIPT_SUPPORT_NUMBER_CONST     3
#define AFW_VALUE_SCRIPT_SUPPORT_NUMBER_CONTINUE  4
#define AFW_VALUE_SCRIPT_SUPPORT_NUMBER_DO_WHILE  5
#define AFW_VALUE_SCRIPT_SUPPORT_NUMBER_FOR       6
#define AFW_VALUE_SCRIPT_SUPPORT_NUMBER_FOR_OF    7
#define AFW_VALUE_SCRIPT_SUPPORT_NUMBER_IF        8
#define AFW_VALUE_SCRIPT_SUPPORT_NUMBER_LET       9
#define AFW_VALUE_SCRIPT_SUPPORT_NUMBER_RETHROW   10
#define AFW_VALUE_SCRIPT_SUPPORT_NUMBER_RETURN    11
#define AFW_VALUE_SCRIPT_SUPPORT_NUMBER_SWITCH    12
#define AFW_VALUE_SCRIPT_SUPPORT_NUMBER_THROW     13
#define AFW_VALUE_SCRIPT_SUPPORT_NUMBER_TRY       14
#define AFW_VALUE_SCRIPT_SUPPORT_NUMBER_WHILE     15


#define AFW_VALUE_COMPILER_LISTING_IF_NOT_LIMIT_EXCEEDED \
    if (writer->indent > xctx->env->evaluation_stack_maximum_count) { \
        afw_writer_write_z(writer, ">>> Limit exceeded", xctx); \
        afw_writer_write_eol(writer, xctx); \
    } \
    else

typedef enum {
    afw_value_block_statement_type_sequential,
    afw_value_block_statement_type_break,
    afw_value_block_statement_type_continue,
    afw_value_block_statement_type_rethrow,
    afw_value_block_statement_type_return
} afw_value_block_statement_type_t;



/** @brief Struct for assignment target value. */
struct afw_value_assignment_target_s {
    const afw_value_inf_t *inf;
    const afw_compile_value_contextual_t *contextual;
    const afw_compile_assignment_target_t *assignment_target;
};



/** @brief Struct for value assignment. */
struct afw_value_assignment_s {
    const afw_utf8_t * name;
    const afw_value_t * value;
};



/**
 * @brief struct for afw_value_block_t
 *
 * This is a symbol table block value.
 */
struct afw_value_block_s {
    const afw_value_inf_t *inf;
    const afw_compile_value_contextual_t *contextual;

    afw_size_t statement_count;
    const afw_value_t * const * statements;

    afw_value_block_t *parent_block;
    afw_value_block_t *first_child_block;
    afw_value_block_t *final_child_block;
    afw_value_block_t *next_sibling_block;
    afw_value_block_symbol_t *first_entry;
    afw_value_block_symbol_t *final_entry;
    afw_size_t number;
    afw_size_t depth;
    afw_size_t symbol_count;
};



/**
 * @brief struct for afw_value_block_frame_t
 *
 * This is the frame for an invocation of a block
 */
struct afw_value_block_frame_s {
    const afw_pool_t *p;                     // Pool for this block frame
    const afw_value_block_t *block;          // Associated block value.
    const afw_value_block_frame_t *previous; // Previous block frame
    const afw_object_t *variables;           // Runtime variables for this block
    afw_value_block_statement_type_t type;
    afw_boolean_t allow_return;
    afw_boolean_t is_loop;    
};



/** @brief Type meta (data type, data type parameters, and value meta object. */
struct afw_value_type_s {

    /** @brief data type or NULL. */
    const afw_data_type_t *data_type;

    /** @brief contextual for data type parameter or NULL. */
    const afw_compile_value_contextual_t *data_type_parameter_contextual;

    /** @brief Data type specific data type parameter or NULL. */
    union {

        /** @brief string, base64Binary, hexBinary. */
        const afw_utf8_t *media_type;

        /** @brief expression, hybrid, script and template. */
        const afw_value_type_t *return_type;

        /** @brief function. */
        const afw_value_script_function_signature_t *function_signature;

        /** @brief list type (If NULL, 1 dimension with untyped cells). */
        const afw_value_type_list_t *list_type;

        /** @brief object and objectId. */
        const afw_utf8_t *object_type_id;

        /** @brief unevaluated. */
        const afw_value_type_t *type;
    };

    /** @brief _AdaptiveValueMeta_ object or NULL. */
    const afw_object_t *value_meta_object;
};



/** @brief Type for list. */
struct afw_value_type_list_s {

    /** @brief Number of subscripts needed to access cell. */
    afw_size_t dimension;

    /** @brief Cell type. If NULL, cell is untyped. */
    const afw_value_type_t *cell_type;
};

/**
 * @brief Block symbol type map
 * @param XX macro
 *
 * There must be and afw_s_ with each name in map.
 */
#define AFW_VALUE_BLOCK_SYMBOL_TYPE(XX)                                     \
                                                                            \
    XX(undeclared, "Undeclared symbol")                                     \
                                                                            \
    XX(const, "A const variable")                                           \
                                                                            \
    XX(function, "A function")                                              \
                                                                            \
    XX(let, "A let variable")                                               \
                                                                            \
    XX(parameter, "A function parameter")                                   \


/**
 * @brief Block symbol type enum
 */
typedef enum afw_value_block_symbol_type_e {
#define XX(name, description) \
    afw_value_block_symbol_type_ ## name,
    AFW_VALUE_BLOCK_SYMBOL_TYPE(XX)
#undef XX
    afw_value_block_symbol_type_count
} afw_value_block_symbol_type_t;


/**
 * @brief struct for afw_value_block_symbol_t
 *
 * This is a symbol table entry for block.
 */
struct afw_value_block_symbol_s {
    afw_value_block_t *parent_block;
    afw_value_block_symbol_t *next_entry;
    const afw_utf8_t *name;
    afw_value_type_t type;
    afw_size_t index; /* Index in block entries. */
    afw_value_block_symbol_type_t symbol_type;
    const afw_value_t *initial_value;
};



/** @brief Struct for contextual and args for call values. */
struct afw_value_call_args_s {

    /** Contextual info for function call. */
    const afw_compile_value_contextual_t *contextual;

    /** The number of function parameters (does not include argv[0]). */
    afw_size_t argc;

    /**
     * The function to call argv[0] followed by function parameters.
     * 
     * argv[0] might need to be evaluated to determine the function to call.
     * 
     */
    const afw_value_t * const * argv;
};



/**
 * @brief Struct for call value.
 *
 * This is a call to the function defined in argv[0] with argc parameters
 * beginning with argv[1]. The function defined can be unevaluated, a
 * function definition, a script function definition, or a thunk definition.
 */
struct afw_value_call_s {
    const afw_value_inf_t *inf;
    const afw_value_t *function_value;
    afw_value_call_args_t args;
    
    /*
     * This is the optimized value or self. If self can be evaluated at create
     * time, this will the evaluated result. If this value references other
     * values, their optimized value will be used. If no optimization can occur,
     * this will be self.
     */
    const afw_value_t *optimized_value;

    /* This is the data type of the evaluated result or NULL if unknown. */
    const afw_data_type_t *evaluated_data_type;
};



/**
 * @brief Struct for call value.
 *
 * This is a call to the function defined in argv[0] with argc parameters
 * beginning with argv[1]. The function defined can be unevaluated, a
 * function definition, a script function definition, or a thunk definition.
 */
struct afw_value_call_built_in_function_s {
    const afw_value_inf_t *inf;
    const afw_value_function_definition_t *function;
    afw_value_call_args_t args;
  
    /*
     * This is the optimized value or self. If self can be evaluated at create
     * time, this will the evaluated result. If this value references other
     * values, their optimized value will be used. If no optimization can occur,
     * this will be self.
     */
    const afw_value_t *optimized_value;

    /* This is the data type of the evaluated result or NULL if unknown. */
    const afw_data_type_t *evaluated_data_type;
};



/**
 * @brief Struct for call script function value.
 *
 * This is a call to the function defined in argv[0] with argc parameters
 * beginning with argv[1]. The function defined can be unevaluated, a
 * function definition, a script function definition, or a thunk definition.
 */
struct afw_value_call_script_function_s {
    const afw_value_inf_t *inf;
    const afw_value_script_function_definition_t *script_function_definition;
    const afw_xctx_scope_t *enclosing_scope;
    afw_value_call_args_t args;
    
    /*
     * This is the optimized value or self. If self can be evaluated at create
     * time, this will the evaluated result. If this value references other
     * values, their optimized value will be used. If no optimization can occur,
     * this will be self.
     */
    const afw_value_t *optimized_value;

    /* This is the data type of the evaluated result or NULL if unknown. */
    const afw_data_type_t *evaluated_data_type;
};



/**
 * @brief Struct for closure binding value.
 *
 * This is a closure binding.
 */
struct afw_value_closure_binding_s {
    const afw_value_inf_t *inf;
    const afw_value_script_function_definition_t *script_function_definition;
    const afw_xctx_scope_t *enclosing_scope;
    afw_size_t reference_count;
};



/**
 * @brief Struct for compiled value value.
 *
 * This is the top level value return from the adaptive compiler when the
 * result is more than a literal.  It contains the full source that is
 * used by all children values, the symbol table, the literal value table
 * shared strings.
 *
 * The compiled value can also have a parent where literal values, etc
 * are shared.  This parent also contributes to the source location.
 *
 * Methods of a compiled value value calls the root value's corresponding
 * methods for optional_evaluate() and get_data_type().
 */
struct afw_value_internal_compiled_value_s {
    const afw_value_inf_t *inf;

    /** @brief Pool containing value. */
    const afw_pool_t *p;

    /** @brief The parent compiled value or NULL. */
    const afw_value_compiled_value_t *parent;

    /** @brief The root value */
    const afw_value_t *root_value;

    /** @brief Contextual Info */
    const afw_compile_value_contextual_t *contextual;

    /** @brief Source location provided to compiler. */
    const afw_utf8_t *source_location;

    /** @brief A uuid for this compiled value. */
    const afw_utf8_t *reference_id;

    /** @brief The full source that was compiled. */
    const afw_utf8_t *full_source;

    /** @brief The type of the full source. */
    const afw_utf8_t *full_source_type;

    /**
     * @brief The total number of lines in source.
     *
     * This will be 0 if the source does not have any newlines.
     */
    afw_size_t line_count;

    /**
     * @brief The longest line excluding \n.
     *
     * This will be 0 if the source does not have any newlines.
     */
    afw_size_t longest_line;

    /* @brief Top symbol block. */
    afw_value_block_t *top_block;

    /* @brief Current symbol block. */
    afw_value_block_t *current_block;

    /**
     * @brief Literal values
     *
     * The property name is formed by concatenating the data type, a ':'
     * and the string_value of the literal.
     *
     * The value is an evaluated value of the data type.
     */
    const afw_object_t *literals;

    /**
     * @brief Shared strings.
     *
     * The property name is the string and value is an evaluated string.
     */
    const afw_object_t *strings;
    
    /*
     * This is the optimized value or self. If self can be evaluated at create
     * time, this will the evaluated result. If this value references other
     * values, their optimized value will be used. If no optimization can occur,
     * this will be self.
     */
    const afw_value_t *optimized_value;

    /* This is the data type of the evaluated result or NULL if unknown. */
    const afw_data_type_t *evaluated_data_type;
};



/** @brief struct for list expression value. */
struct afw_value_list_expression_s {
    const afw_value_inf_t *inf;
    const afw_value_t *internal;
    const afw_compile_value_contextual_t *contextual;
};



/** @brief Struct for function thunk value. */
struct afw_value_function_thunk_s {
    const afw_value_inf_t *inf;

    /** @brief name. */
    const afw_utf8_t *name;

    /** @brief includes name and source location. */
    const afw_utf8_t *detail;

    /** @brief The thunk signature is like this function. */
    const afw_value_t *like_function_value;

    /** @brief Function called to execute this thunk. */
    afw_function_thunk_execute_t execute;

    /** @brief ctx passed on create. */
    void *ctx;
};



/** @brief struct for object expression value. */
struct afw_value_object_expression_s {
    const afw_value_inf_t *inf;
    const afw_object_t *internal;
    const afw_compile_value_contextual_t *contextual;
};



/** @brief Struct for reference_by_key value. */
struct afw_value_reference_by_key_s {
    const afw_value_inf_t *inf;
    const afw_compile_value_contextual_t *contextual;
    const afw_value_t *aggregate_value; /* Object or list */
    const afw_value_t *key;
    const afw_utf8_t *backtrace_detail;
    
    /*
     * This is the optimized value or self. If self can be evaluated at create
     * time, this will the evaluated result. If this value references other
     * values, their optimized value will be used. If no optimization can occur,
     * this will be self.
     */
    const afw_value_t *optimized_value;

    /* This is the data type of the referenced value or NULL if unknown. */
    const afw_data_type_t *evaluated_data_type;
};



/** @brief Struct for lambda parameter. */
struct afw_value_script_function_signature_s {
    const afw_compile_value_contextual_t *contextual;
    const afw_value_block_t *block;
    const afw_value_type_t *returns;
    const afw_value_block_symbol_t *function_name_symbol;
    const afw_value_string_t *function_name_value;
    afw_size_t count;
    const afw_value_script_function_parameter_t **parameters;
};



/** @brief Struct for script function parameter. */
struct afw_value_script_function_parameter_s {
    const afw_value_block_symbol_t *symbol;
    const afw_utf8_t *name;
    const afw_value_type_t *type;
    const afw_value_t *default_value;
    afw_boolean_t is_optional;
    afw_boolean_t is_rest;
};



/** @brief Struct for lambda value. */
struct afw_value_script_function_definition_s {
    const afw_value_inf_t *inf;
    const afw_compile_value_contextual_t *contextual;
    const afw_value_script_function_signature_t *signature;
    const afw_value_type_t *returns;
    afw_size_t depth;
    afw_size_t count;
    const afw_value_script_function_parameter_t **parameters;
    const afw_value_t *body;
    
    /*
     * This is the optimized value or self. If self can be evaluated at create
     * time, this will the evaluated result. If this value references other
     * values, their optimized value will be used. If no optimization can occur,
     * this will be self.
     */
    const afw_value_t *optimized_value;

    /* This is the data type of the evaluated result or NULL if unknown. */
    const afw_data_type_t *evaluated_data_type;
};



/** @brief Struct for qualified variable reference value. */
struct afw_value_qualified_variable_reference_s {
    const afw_value_inf_t *inf;
    const afw_compile_value_contextual_t *contextual;
    afw_utf8_t qualifier;
    afw_utf8_t name;
    const afw_utf8_t *optionally_qualified_name;
    const afw_utf8_t *backtrace_detail;
    
    /*
     * This is the optimized value or self. If self can be evaluated at create
     * time, this will the evaluated result. If this value references other
     * values, their optimized value will be used. If no optimization can occur,
     * this will be self.
     */
    const afw_value_t *optimized_value;

    /* This is the data type of the evaluated result or NULL if unknown. */
    const afw_data_type_t *evaluated_data_type;
};



/** @brief Struct for template value. */
struct afw_value_template_definition_s {
    const afw_value_inf_t *inf;
    const afw_compile_value_contextual_t *contextual;
    afw_size_t count;
    const afw_value_t * const * values;
};



/* @brief struct for afw_value_symbol_reference_t */
struct afw_value_symbol_reference_s {
    const afw_value_inf_t *inf;
    const afw_compile_value_contextual_t *contextual;
    const afw_value_block_symbol_t *symbol;
    
    /*
     * This is the optimized value or self. If self can be evaluated at create
     * time, this will the evaluated result. If this value references other
     * values, their optimized value will be used. If no optimization can occur,
     * this will be self.
     */
    const afw_value_t *optimized_value;

    /* This is the data type of the evaluated result or NULL if unknown. */
    const afw_data_type_t *evaluated_data_type;
};



typedef struct afw_value_meta_object_self_s {
    afw_object_t pub;
    afw_object_setter_t setter;
    afw_value_object_t meta_object_value;
    const afw_value_t *value;
    const afw_value_t *evaluated_value;
    const afw_utf8_t *key;
    const afw_object_t *additional;
    afw_boolean_t immutable;
} afw_value_meta_object_self_t;



typedef struct afw_value_meta_values_object_list_self_s {
    afw_array_t pub;
    const afw_pool_t *p;
    const afw_value_object_t *value;
} afw_value_meta_values_object_list_self_t;



typedef struct afw_value_meta_values_list_list_self_s {
    afw_array_t pub;
    const afw_pool_t *p;
    const afw_value_array_t *value;
} afw_value_meta_values_list_list_self_t;



typedef const afw_value_t *(*afw_value_meta_special_get_t) (
    afw_value_meta_object_self_t *self,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);

typedef void (*afw_value_meta_special_set_t) (
    afw_value_meta_object_self_t *self,
    const afw_utf8_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx);

typedef struct {
    const afw_utf8_t *property_name;
    afw_value_meta_special_get_t get;
    afw_value_meta_special_set_t set;
} afw_value_meta_name_handler_t;

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_call_built_in_function(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_function_definition_t *function,
    afw_size_t argc,
    const afw_value_t * const *argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_call_script_function(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_script_function_definition_t *script_function_definition,
    const afw_xctx_scope_t *enclosing_scope, /* NULL if not enclosed. */
    afw_size_t argc,
    const afw_value_t * const * argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(afw_value_meta_object_self_t *)
afw_value_internal_create_meta_object_self(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_internal_get_evaluated_meta_default(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_internal_get_evaluated_metas_default(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_internal_get_evaluated_meta_for_array(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_internal_get_evaluated_metas_for_array(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_internal_get_evaluated_meta_for_object(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_internal_get_evaluated_metas_for_object(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_meta_values_list_for_object_create(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_meta_values_list_for_list_create(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(afw_value_compiler_listing_t *)
afw_value_compiler_listing_to_string_instance(
    const afw_value_t *value,
    afw_value_compiler_listing_t *parent,
    const afw_utf8_t *tab,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



struct afw_value_compiler_listing_s {
    afw_writer_t writer;
    const afw_value_compiler_listing_t *parent;
    afw_value_compiler_listing_t *first_child;
    afw_value_compiler_listing_t *final_child;
    afw_value_compiler_listing_t *next_sibling;
    const afw_pool_t *p;
    const afw_value_compiled_value_t *compiled_value;
    const afw_utf8_t *reference_id;
    apr_array_header_t *ary;
    apr_array_header_t *child_compiler_listings;
    const afw_utf8_t *last_full_source;
    afw_size_t prefix_size;
    afw_size_t max_value_cursor;
    afw_size_t cursor_written;
    afw_size_t last_line_written;
    const char *prefix_format;
    afw_utf8_t empty_prefix;
    const afw_utf8_t *current_prefix;
    afw_value_info_t first_info;
    afw_boolean_t offset_only;
    afw_boolean_t is_new_line;
    afw_boolean_t include_source;
    afw_boolean_t empty_line_between_switch;
    afw_boolean_t source_eof;
    int tab_size;
};

AFW_DECLARE_INTERNAL(const afw_utf8_t *)
afw_value_compiler_listing_symbol_type_name(
    afw_value_block_symbol_type_t type);

AFW_DECLARE_INTERNAL(const afw_utf8_t *)
afw_value_compiler_listing_for_child(
    const afw_value_t *instance,
    const afw_writer_t *writer,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(void)
afw_value_compiler_listing_begin_value(
    const afw_writer_t *writer,
    const afw_value_t *value,
    const afw_compile_value_contextual_t *contextual,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(void)
afw_value_compiler_listing_end_value(
    const afw_writer_t *writer,
    const afw_value_t *value,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(void)
afw_value_compiler_listing_call_args(
    const afw_writer_t *writer,
    const afw_value_call_args_t *args,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(void)
afw_value_compiler_listing_value(
    const afw_value_t *instance,
    const afw_writer_t *writer,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(void)
afw_value_compiler_listing_name_and_type(
    const afw_writer_t *writer,
    const afw_utf8_t *name,
    const afw_value_type_t *type,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_statement(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    afw_boolean_t allow_return,
    afw_boolean_t is_loop,
    const afw_value_t *block,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_block(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    const afw_value_block_t *self,
    afw_boolean_t is_loop,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_for(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    afw_size_t argc,
    const afw_value_t * const * argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_for_of(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    afw_size_t argc,
    const afw_value_t *const *argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_do_while(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    afw_size_t argc,
    const afw_value_t *const *argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_if(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    afw_size_t argc,
    const afw_value_t * const * argv,
    afw_boolean_t is_loop,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_switch(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    afw_size_t argc,
    const afw_value_t * const * argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_throw(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    afw_size_t argc,
    const afw_value_t * const * argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_try(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    afw_size_t argc,
    const afw_value_t * const * argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_while(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    afw_size_t argc,
    const afw_value_t * const * argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



AFW_END_DECLARES

/** @} */

#endif /* __AFW_VALUE_INTERNAL_H__ */
