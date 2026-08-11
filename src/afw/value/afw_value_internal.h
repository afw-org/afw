// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Internal Header for afw_value*
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_VALUE_INTERNAL_H__
#define __AFW_VALUE_INTERNAL_H__

#include "afw_interface.h"
#include "afw_object_internal.h"

/**
 * @addtogroup afw_value_internal
 * @{
 */

/**
 * @file afw_value_internal.h
 * @brief Concrete value-kind structs (internal to libafw).
 *
 * These are **implementation layouts** for the value graph (blocks, calls,
 * closures, symbol references, compiled values, …). The public API type
 * remains `afw_value_t` (@ref afw_value). Each of these structs begins with
 * the usual inf/`afw_value_t` pub union so instances can be passed as
 * `const afw_value_t *`.
 *
 * Extension authors should rarely need this header; prefer `afw_value.h`
 * and data-type create/evaluate APIs. Core maintainers use this file when
 * adding or debugging a value kind.
 */

AFW_BEGIN_DECLARES

/**
 * Whether compile-time checks for adaptive (built-in) function formals run.
 * Default: same as compile type checking. A future compile:* flag can narrow
 * this without hunting call sites.
 *
 * @see designs/adaptive-function-compile-typecheck.md
 */
/** Compile-time adaptive formals: same gate as unit/flag compile typeCheck. */
#define AFW_VALUE_TYPE_CHECK_ADAPTIVE_FUNCTION_FORMALS(contextual, xctx) \
    (AFW_VALUE_TYPE_CHECK_COMPILE_ENABLED((contextual), (xctx)))


#define AFW_VALUE_COMPILER_LISTING_IF_NOT_LIMIT_EXCEEDED \
    if (writer->indent > xctx->env->evaluation_stack_maximum_count) { \
        afw_writer_write_z(writer, ">>> Limit exceeded", xctx); \
        afw_writer_write_eol(writer, xctx); \
    } \
    else


/** @brief Struct for assignment target value. */
struct afw_value_assignment_target_s {
    /* Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

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
    /* Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

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
 * @brief Kind of script/value type expression (TS-like; issue #28).
 *
 * Leaves are Adaptive data types (permanent pointers). Composites build
 * script-local structure. Not adaptive object types.
 */
typedef enum afw_value_type_kind_e {
    /** Adaptive data type leaf (integer, string, any, void, array, …). */
    afw_value_type_kind_data_type,

    /** Named type/interface reference (type Foo = … / interface Bar). */
    afw_value_type_kind_reference,

    /** Object type literal or interface body: { a?: T, … }. */
    afw_value_type_kind_object,

    /** Array type: T[] (element type). */
    afw_value_type_kind_array,

    /** Tuple type: [T, U, …]. */
    afw_value_type_kind_tuple,

    /** Function type: (a: T, b?: U) => R. */
    afw_value_type_kind_function,

    /** Union: A | B | … */
    afw_value_type_kind_union,

    /** Intersection: A & B & … */
    afw_value_type_kind_intersection
} afw_value_type_kind_t;



/** @brief Property in an object/interface type. */
struct afw_value_type_property_s {
    const afw_value_type_property_t *next;
    const afw_utf8_t *name;
    const afw_value_type_t *type;
    afw_boolean_t optional;
};



/** @brief Parameter in a function type expression. */
struct afw_value_type_function_param_s {
    const afw_value_type_function_param_t *next;
    const afw_utf8_t *name; /* optional; may be NULL for bare type params */
    const afw_value_type_t *type;
    afw_boolean_t optional;
    afw_boolean_t is_rest;
};



/**
 * @brief Type information for adaptive values / script bindings.
 *
 * TS-like type expression graph. Embeddable root (fixed size); children are
 * pool pointers. Leaf identity for data types is permanent
 * `afw_data_type_*` pointer comparison.
 */
struct afw_value_type_s {

    /** @brief Discriminator for union below. */
    afw_value_type_kind_t kind;

    union {

        /** kind_data_type: permanent adaptive data type (never NULL). */
        const afw_data_type_t *data_type;

        /** kind_reference: name of type/interface; resolved may be NULL. */
        struct {
            const afw_utf8_t *name;
            const afw_value_type_t *resolved;
        } reference;

        /**
         * kind_object: structural object or interface body.
         * extends/extends_count used for interface extends list.
         */
        struct {
            const afw_value_type_property_t *properties;
            const afw_value_type_t *const *extends;
            afw_size_t extends_count;
            const afw_utf8_t *interface_name; /* NULL if type literal */
        } object;

        /** kind_array: element type (NULL = untyped elements). */
        struct {
            const afw_value_type_t *element;
        } array;

        /** kind_tuple: fixed element list. */
        struct {
            const afw_value_type_t *const *elements;
            afw_size_t count;
        } tuple;

        /** kind_function: parameter list + return type. */
        struct {
            const afw_value_type_function_param_t *parameters;
            const afw_value_type_t *returns;
        } function;

        /** kind_union / kind_intersection: member types. */
        struct {
            const afw_value_type_t *const *members;
            afw_size_t count;
        } compound;
    };
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



/**
 * @brief Call argv bundle (shared by call, call_built_in, call_script, …).
 *
 * Unified layout for built-in adaptive functions and script functions:
 *
 *   argv[0]       — callee (may still need evaluation to find the function)
 *   argv[1..argc] — user parameters (1-based parameter numbers 1..argc)
 *   argc          — count of user parameters only (not including argv[0])
 *
 * Example: call f(a, b) → argc=2, argv = { f, a, b }.
 *
 * Script formal lists (`script_function_parameter_t **parameters`) are a
 * separate C array indexed 0..count-1 for the same formals: parameters[0]
 * is parameter number 1 / argv[1]. Do not mix 0-based C indexes with
 * 1-based parameter_number without adjusting (usually ±1).
 */
struct afw_value_call_args_s {

    /** Contextual info for function call. */
    const afw_compile_value_contextual_t *contextual;

    /** Number of user parameters (does not include argv[0]). */
    afw_size_t argc;

    /**
     * argv[0] = callee, argv[1..argc] = parameters.
     * argv[0] might still need evaluation to determine the function.
     */
    const afw_value_t * const * argv;
};



/**
 * @brief Struct for call value.
 *
 * Call of the function in argv[0] with argc user parameters starting at
 * argv[1]. Callee may be unevaluated, a built-in function definition, a
 * script function definition, a closure binding, or similar.
 */
struct afw_value_call_s {
    /* Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

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
    /* Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

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
    /* Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

    const afw_value_script_function_definition_t *script_function_definition;
    const afw_xctx_scope_t *enclosing_lexical_scope;
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
 * @brief Struct for call test script value.
 */
struct afw_value_call_test_script_s {
    /* Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

    const afw_compile_value_contextual_t *contextual;
    const afw_value_object_t *test_script_object_value;
};


/**
 * @brief Struct for closure binding value.
 *
 * This is a closure binding.
 */
struct afw_value_closure_binding_s {
    /* Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

    const afw_value_script_function_definition_t *script_function_definition;
    const afw_xctx_scope_t *enclosing_lexical_scope;
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
    /** @brief Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

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

    /** @brief Top symbol block. */
    afw_value_block_t *top_block;

    /** @brief Current symbol block. */
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

    /**
     * Effective compile policy for this unit (flags snapshotted at compile
     * start; #compile may override fields without touching process flags).
     */
    afw_compile_policy_t compile_policy;
};



/** @brief struct for list expression value. */
struct afw_value_list_expression_s {
    /* Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

    const afw_value_t *internal;
    const afw_compile_value_contextual_t *contextual;
};



/** @brief Struct for function thunk value. */
struct afw_value_function_thunk_s {
    /* Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

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
    /* Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

    const afw_object_t *internal;
    const afw_compile_value_contextual_t *contextual;
};



/** @brief Kind of one entry in an object_construct value. */
typedef enum {
    /** Static property name + value expression. */
    afw_value_object_construct_entry_static,

    /** Expression property name + value expression. */
    afw_value_object_construct_entry_name_expr,

    /** Spread of an object expression (...obj). */
    afw_value_object_construct_entry_spread
} afw_value_object_construct_entry_type_t;



/** @brief One ordered entry for object_construct. */
struct afw_value_object_construct_entry_s {
    afw_value_object_construct_entry_type_t type;

    /** Static name when type is static; otherwise NULL. */
    const afw_utf8_t *static_name;

    /** Name expression when type is name_expr; otherwise NULL. */
    const afw_value_t *name_expr;

    /** Value expression for static / name_expr; NULL for spread. */
    const afw_value_t *value;

    /** Object expression when type is spread; otherwise NULL. */
    const afw_value_t *spread_expr;

    /** Next entry or NULL. */
    afw_value_object_construct_entry_t *next;
};



/** @brief struct for object construct value (expression property names). */
struct afw_value_object_construct_s {
    /* Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

    const afw_compile_value_contextual_t *contextual;

    /** Head of ordered entry list. */
    const afw_value_object_construct_entry_t *entries;

    /**
     * Optional meta object from a literal `_meta_` property in source.
     * Installed on the evaluated object after properties are set.
     */
    const afw_object_t *meta;
};



/** @brief Struct for reference_by_key value. */
struct afw_value_reference_by_key_s {
    /* Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

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



/**
 * @brief Script function signature (formals + optional return type).
 *
 * `count` / `parameters` describe formals only. At call time those line up
 * with call argv as: parameters[i] ↔ parameter number (i+1) ↔ argv[i+1].
 * See `afw_value_call_args_s` for the full argv convention.
 */
struct afw_value_script_function_signature_s {
    const afw_compile_value_contextual_t *contextual;
    const afw_value_block_t *block;
    const afw_value_type_t *returns;
    const afw_value_block_symbol_t *function_name_symbol;
    const afw_value_string_t *function_name_value;
    /** Number of formals; parameters has this many entries (0-based C array). */
    afw_size_t count;
    const afw_value_script_function_parameter_t **parameters;
};



/**
 * @brief One script-function formal (name or Pattern).
 *
 * Simple name: `symbol` + `name` set; `assignment_target` NULL.
 * Pattern (list/object destructure, issue #140): `assignment_target` holds
 * the Pattern; nested leaves are parameter symbols in the function's
 * parameter block. `name` / `symbol` may be NULL (no whole-arg binding).
 *
 * This struct is always the formal list entry (0-based index into
 * `parameters[]`). Call argv still uses 1-based parameter numbers.
 */
struct afw_value_script_function_parameter_s {
    const afw_value_block_symbol_t *symbol;
    const afw_utf8_t *name;
    const afw_value_type_t *type;
    const afw_value_t *default_value;
    /** Pattern bind target (list/object destructure), or NULL. */
    const afw_value_t *assignment_target;
    afw_boolean_t is_optional;
    afw_boolean_t is_rest;
};



/**
 * @brief Compiled script function (lambda or named function value).
 *
 * `count` / `parameters` mirror the signature formals. When this value is
 * called, bind using `afw_value_call_args_t`: argv[0] is this function,
 * argv[1..] are arguments for parameters[0..].
 */
struct afw_value_script_function_definition_s {
    /* Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

    const afw_compile_value_contextual_t *contextual;
    const afw_value_script_function_signature_t *signature;
    const afw_value_type_t *returns;
    afw_size_t depth;
    /** Formal count (same as signature->count when signature is present). */
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
    /* Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

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
    /* Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

    const afw_compile_value_contextual_t *contextual;
    afw_size_t count;
    const afw_value_t * const * values;
};



/** @brief struct for afw_value_symbol_reference_t */
struct afw_value_symbol_reference_s {
    /* Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

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
    afw_value_object_t value;
    afw_object_setter_t setter;
    afw_value_object_t meta_object_value;
    const afw_value_t *associated_value;
    const afw_value_t *evaluated_value;
    const afw_utf8_t *key;
    const afw_object_t *additional;
    afw_boolean_t immutable;
} afw_value_meta_object_self_t;



typedef struct afw_value_meta_values_object_list_self_s {
    afw_array_t pub;
    afw_value_array_t value;
    const afw_value_object_t *associated_value;
} afw_value_meta_values_object_list_self_t;



typedef struct afw_value_meta_values_list_list_self_s {
    afw_array_t pub;
    afw_value_array_t value;
    const afw_value_array_t *associated_value;
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
    const afw_xctx_scope_t *enclosing_lexical_scope, /* NULL if not enclosed. */
    afw_size_t argc,
    const afw_value_t * const * argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(afw_value_meta_object_self_t *)
afw_value_internal_create_meta_object_self(
    const afw_value_t *associated_value,
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
    const afw_value_t *block,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_block(
    afw_function_execute_t *x,
    const afw_value_block_t *self,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Register core value infs during environment bootstrap (libafw only).
 * @param xctx of caller.
 *
 * Called from afw_environment_register_core.c. Implemented in afw_value.c.
 */
AFW_DECLARE_INTERNAL(void)
afw_value_register_core_value_infs(afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_VALUE_INTERNAL_H__ */
