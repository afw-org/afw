// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Common Opaques Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_COMMON_OPAQUES_H__
#define __AFW_COMMON_OPAQUES_H__


/**
 * @addtogroup afw_common
 * @{
 */

/**
 * @file afw_common_opaques.h
 * @brief Adaptive Framework Common Opaques Header.
 *
 * This header is included by afw_common.h.  It includes most of the Adaptive
 * Framework core opaque typedefs that are not generated.
 */


/**
 * Opaque typedef for afw_adaptor_id_anchor_t.
 *
 * See afw_adaptor.h for struct and more information.
 */
typedef struct afw_adaptor_id_anchor_s
afw_adaptor_id_anchor_t;

/**
 * Opaque typedef for afw_adaptor_impl_t.
 *
 * See afw_adaptor_impl.h for struct and more information.
 */
typedef struct afw_adaptor_impl_s
afw_adaptor_impl_t;

/**
 * Opaque typedef for afw_adaptor_impl_request_t.
 *
 * See afw_adaptor_impl.h for struct and more information.
 */
typedef struct afw_adaptor_impl_request_s
afw_adaptor_impl_request_t;

/**
 * Opaque typedef for afw_adaptor_impl_session_t.
 *
 * See afw_adaptor_impl.h for struct and more information.
 */
typedef struct afw_adaptor_impl_session_s
afw_adaptor_impl_session_t;

/**
 * Opaque typedef for afw_adaptor_internal_cache_t.
 *
 * See afw_adaptor_internal.h for struct and more information.
 */
typedef struct afw_adaptor_internal_cache_s
afw_adaptor_internal_cache_t;

/**
 * Opaque typedef for afw_adaptor_internal_session_cache_t.
 *
 * See afw_adaptor_internal.h for struct and more information.
 */
typedef struct afw_adaptor_internal_session_cache_s
afw_adaptor_internal_session_cache_t;

/**
 * Opaque typedef for afw_adaptor_modify_entry_t.
 *
 * See afw_adaptor.h for struct and more information.
 */
typedef struct afw_adaptor_modify_entry_s
afw_adaptor_modify_entry_t;

/**
 * Opaque typedef for afw_adaptor_internal_t.
 *
 * See afw_adaptor_internal.h for struct and more information.
 */
typedef struct afw_adaptor_xctx_internal_s 
afw_adaptor_xctx_internal_t;

/**
 * @brief Application app (afw_app) object.
 *
 * See afw_components.h for more information.
 */
typedef struct afw_components_s afw_components_t;

/**
 * Opaque typedef for afw_authorization_advice_expression_t.
 *
 * See afw_authorization.h for struct and more information.
 */
typedef struct afw_authorization_advice_expression_s
afw_authorization_advice_expression_t;

/**
 * Opaque typedef for afw_authorization_combiner_parameter_t.
 *
 * See afw_authorization.h for struct and more information.
 */
typedef struct afw_authorization_combiner_parameter_s
afw_authorization_combiner_parameter_t;

/**
 * Opaque typedef for afw_authorization_control_t.
 *
 * See afw_authorization_internal.h for struct and more information.
 */
typedef struct afw_authorization_internal_control_s
afw_authorization_control_t;

/**
 * Opaque typedef for afw_authorization_decision_t.
 *
 * See afw_authorization.h for struct and more information.
 */
typedef struct afw_authorization_decision_s
afw_authorization_decision_t;

/**
 * Opaque typedef for afw_authorization_handler_id_t.
 *
 * See afw_authorization.h for struct and more information.
 */
typedef struct afw_authorization_handler_id_s
afw_authorization_handler_id_t;

/**
 * Opaque typedef for afw_authorization_handler_id_anchor_t.
 *
 * See afw_authorization_internal.h for struct and more information.
 */
typedef struct afw_authorization_internal_handler_id_anchor_s
afw_authorization_handler_id_anchor_t;

/**
 * Opaque typedef for afw_authorization_handler_impl_t.
 *
 * See afw_authorization_handler_impl.h for struct and more information.
 */
typedef struct afw_authorization_handler_impl_s
afw_authorization_handler_impl_t;

/**
 * Opaque typedef for afw_authorization_obligation_expression_t.
 *
 * See afw_authorization.h for struct and more information.
 */
typedef struct afw_authorization_obligation_expression_s
afw_authorization_obligation_expression_t;

/**
 * Opaque typedef for afw_authorization_policy_t.
 *
 * See afw_authorization.h for struct and more information.
 */
typedef struct afw_authorization_policy_s
afw_authorization_policy_t;

/**
 * Opaque typedef for afw_authorization_policy_issuer_t.
 *
 * See afw_authorization.h for struct and more information.
 */
typedef struct afw_authorization_policy_issuer_s
afw_authorization_policy_issuer_t;

/**
 * Opaque typedef for afw_authorization_policy_location_t.
 *
 * See afw_authorization.h for struct and more information.
 */
typedef struct afw_authorization_policy_location_s
afw_authorization_policy_location_t;

/**
 * Opaque typedef for afw_authorization_policy_set_t.
 *
 * See afw_authorization.h for struct and more information.
 */
typedef struct afw_authorization_policy_set_s
afw_authorization_policy_set_t;

/**
 * Opaque typedef for afw_authorization_rule_t.
 *
 * See afw_authorization.h for struct and more information.
 */
typedef struct afw_authorization_rule_s
afw_authorization_rule_t;

/**
 * Opaque typedef for afw_compile_args_t.
 *
 * See afw_compile_internal.h for struct and more information.
 */
typedef struct afw_compile_internal_args_s
afw_compile_args_t;

/**
 * Opaque typedef for afw_compile_assignment_property_t.
 *
 * See afw_compile_internal.h for struct and more information.
 */
typedef struct afw_compile_internal_assignment_property_s
afw_compile_assignment_property_t;

/**
 * Opaque typedef for afw_compile_assignment_target_t.
 *
 * See afw_compile_internal.h for struct and more information.
 */
typedef struct afw_compile_internal_assignment_target_s
afw_compile_assignment_target_t;

/**
 * Opaque typedef for afw_compile_list_destructure_t.
 *
 * See afw_compile_internal.h for struct and more information.
 */
typedef struct afw_compile_internal_list_destructure_s
afw_compile_list_destructure_t;

/**
 * Opaque typedef for afw_compile_assignment_element_t.
 *
 * See afw_compile_internal.h for struct and more information.
 */
typedef struct afw_compile_assignment_element_s
afw_compile_assignment_element_t;

/**
 * Opaque typedef for afw_compile_object_destructure_t.
 *
 * See afw_compile_internal.h for struct and more information.
 */
typedef struct afw_compile_internal_object_destructure_s
afw_compile_object_destructure_t;

/**
 * Opaque typedef for afw_compile_parser_t.
 *
 * See afw_compile_internal.h for struct and more information.
 */
typedef struct afw_compile_internal_parser_s
afw_compile_parser_t;

/**
 * Opaque typedef for afw_compile_token_t.
 *
 * See afw_compile_internal.h for struct and more information.
 */
typedef struct afw_compile_internal_token_s
afw_compile_token_t;

/**
 * Opaque typedef for afw_compile_shared_t.
 *
 * See afw_compile_internal.h for struct and more information.
 */
typedef struct afw_compile_internal_shared_s
afw_compile_shared_t;

/**
 * Opaque typedef for afw_compile_type_info_t.
 *
 * See afw_compile.h for struct and more information.
 */
typedef struct afw_compile_type_info_s
afw_compile_type_info_t;

/**
 * Opaque typedef for afw_const_utf8_stack_t.
 *
 * See afw_common.h for AFW_STACK_STRUCT() and more information.
 */
typedef struct afw_utf8_stack_s
afw_utf8_stack_t;

/**
 * Opaque typedef for afw_const_utf8_a_stack_t.
 *
 * See afw_common.h for AFW_STACK_STRUCT() and more information.
 */
typedef struct afw_const_utf8_a_stack_s
afw_const_utf8_a_stack_t;

/**
 * Opaque typedef for afw_context_t.
 *
 * See afw_context.h for struct and more information.
 */
typedef struct afw_context_s
afw_context_t;

/**
 * Opaque typedef for afw_context_cb_variable_meta_t.
 *
 * See afw_context.h for struct and more information.
 */
typedef struct afw_context_cb_variable_meta_s
afw_context_cb_variable_meta_t;

/**
 * Opaque typedef for afw_context_cb_variable_t.
 *
 * See afw_context.h for struct and more information.
 */
typedef struct afw_context_cb_variable_s
afw_context_cb_variable_t;

/**
 * Opaque typedef for afw_environment_t.
 *
 * See afw_interface.h for struct and more information.
 */
typedef struct afw_environment_s
afw_environment_t;

/**
 * Opaque typedef for afw_environment_conf_type_t.
 *
 * See afw_environment.h for struct and more information.
 */
typedef struct afw_environment_conf_type_s
afw_environment_conf_type_t;

/**
 * Opaque typedef for afw_environment_registry_type_t.
 *
 * See afw_common.h for struct and more information.
 */
typedef struct afw_environment_registry_type_s
afw_environment_registry_type_t;

/**
 * Opaque typedef for afw_error_t.
 *
 * See afw_error.h for struct and more information.
 */
typedef struct afw_error_s
afw_error_t;

/**
 * Opaque typedef for afw_error_context_t.
 *
 * See afw_error.h for struct and more information.
 */
typedef struct afw_error_context_s
afw_error_context_t;

/**
 * Opaque typedef for afw_expression_t.
 *
 * See afw_expression.h for struct and more information.
 */
typedef struct afw_expression_s
afw_expression_t;

/**
 * Opaque typedef for afw_flag_t.
 *
 * See afw_flag.h for struct and more information.
 */
typedef struct  afw_flag_s
afw_flag_t;

/**
 * Opaque typedef for afw_function_environment_t.
 *
 * See afw_function.h for struct and more information.
 */
typedef struct  afw_function_environment_s
afw_function_environment_t;

/**
 * Opaque typedef for afw_function_execute_t.
 *
 * See afw_function.h for struct and more information.
 */
typedef struct  afw_function_execute_s
afw_function_execute_t;

/**
 * Opaque typedef iterator.
 *
 * Each implementation with a afw_iterator_t ** parameter has its own
 * private afw_iterator_s struct definition or use.
 */
typedef struct afw_iterator_s
afw_iterator_t;

/**
 * Opaque typedef for afw_lock_t.
 *
 * See afw_lock.h for struct and more information.
 */
typedef struct afw_lock_s
afw_lock_t;

/**
 * Opaque typedef for afw_lock_rw_t.
 *
 * See afw_lock.h for struct and more information.
 */
typedef struct afw_lock_rw_s afw_lock_rw_t;

/**
 * Opaque typedef for afw_log_impl_s.
 *
 * See afw_log_impl.h for struct and more information.
 */
typedef struct afw_log_impl_s afw_log_impl_t;

/**
 * Opaque typedef for afw_object_meta_object_t.
 *
 * See afw_object_meta.h for struct and more information.
 */
typedef struct afw_object_meta_object_s
afw_object_meta_object_t;

/**
 * Opaque typedef for afw_model_associative_array_t.
 *
 * See afw_associative_array.c for struct and more information. This is used
 * by AFW_ASSOCIATIVE_ARRAY_TEMPLATE() in afw_model.h.
 */
typedef struct afw_model_associative_array_s
afw_model_associative_array_t;

/**
 * Opaque typedef for afw_model_t.
 *
 * See afw_model_internal.h for struct and more information.
 */
typedef struct afw_model_internal_s
afw_model_t;

/**
 * Opaque typedef for afw_model_internal_context_t.
 *
 * See afw_model_internal.h for struct and more information.
 */
typedef struct afw_model_internal_context_s
afw_model_internal_context_t;

/**
 * Opaque typedef for type=model adaptor self.
 *
 * See afw_model.h for struct definition.
 */
typedef struct afw_model_internal_adaptor_self_s
afw_model_internal_adaptor_self_t;

/**
 * Opaque typedef for type=model adaptor session self.
 *
 * See afw_model.h for struct definition.
 */
typedef struct afw_model_internal_adaptor_session_self_s
afw_model_internal_adaptor_session_self_t;

/**
 * Opaque typedef for model object type.
 *
 * See afw_model.h for struct definition.
 */
typedef struct afw_model_internal_object_type_s
afw_model_object_type_t;

/**
 * Opaque typedef for object type property type.
 *
 * See afw_model.h for struct definition.
 */
typedef struct afw_model_internal_property_type_s
afw_model_property_type_t;

/**
 * Opaque typedef for object type property type constraint.
 *
 * See afw_model.h for struct definition.
 */
typedef struct afw_model_property_constraint_s
afw_model_property_constraint_t;

/**
 * Opaque typedef for afw_model_location_t.
 *
 * See afw_model_location.h for struct and more information.
 */
typedef struct afw_model_location_s
afw_model_location_t;

/**
 * @brief Name/value stack.
 *
 * See afw_common.h for more information.
 */
typedef struct afw_name_value_stack_s afw_name_value_stack_t;

/**
 * @brief Object view options.
 *
 * See afw_object_view.h for more information.
 */
typedef struct afw_object_options_s afw_object_options_t;

/**
 * Opaque typedef for afw_object_path_parsed_t.
 *
 * See afw_object_path.h for struct and more information.
 */
typedef struct afw_object_path_parsed_s
afw_object_path_parsed_t;

/**
 * Opaque typedef for afw_object_path_property_name_entry_t.
 *
 * See afw_object_path.h for struct and more information.
 */
typedef struct afw_object_path_property_name_entry_s
afw_object_path_property_name_entry_t;

/**
 * Opaque typedef for afw_object_properties_callback_entry_t.
 *
 * See afw_object.h for struct and more information.
 */
typedef struct afw_object_properties_callback_entry_s
afw_object_properties_callback_entry_t;

/**
 * Opaque typedef for afw_object_type_t.
 *
 * See afw_object_type.h for struct and more information.
 */
typedef struct afw_object_type_s
afw_object_type_t;

/**
 * Opaque typedef for afw_object_type_property_type_t.
 *
 * See afw_object_type.h for struct and more information.
 */
typedef struct afw_object_type_property_type_s
afw_object_type_property_type_t;

/**
 * Opaque typedef for query criteria.
 *
 * See afw_query_criteria.h for struct definition.
 */
typedef struct afw_query_criteria_s
afw_query_criteria_t;

/**
 * Opaque typedef for query criteria filter entry.
 *
 * See afw_query_criteria.h for struct definition.
 */
typedef struct afw_query_criteria_filter_entry_s
afw_query_criteria_filter_entry_t;

/**
 * Opaque typedef for afw_runtime_objects_t.
 *
 * See afw_runtime.t for struct and more information.
 */
typedef struct afw_runtime_objects_s
afw_runtime_objects_t;

/**
 * Opaque typedef for afw_runtime_object_type_meta_t.
 *
 * See afw_runtime.h for struct and more information.
 */
typedef struct afw_runtime_object_type_meta_s
afw_runtime_object_type_meta_t;

/**
 * Opaque typedef for afw_runtime_object_map_t.
 *
 * See afw_runtime.h for struct and more information.
 */
typedef struct afw_runtime_object_map_s
afw_runtime_object_map_t;

/**
 * Opaque typedef for afw_runtime_object_map_t.
 *
 * See afw_runtime.h for struct and more information.
 */
typedef struct afw_runtime_object_map_property_s
afw_runtime_object_map_property_t;

/**
 * Opaque typedef for afw_runtime_object_indirect_t.
 *
 * See afw_runtime.h for struct and more information.
 */
typedef struct afw_runtime_object_indirect_s
afw_runtime_object_indirect_t;

/**
 * Opaque typedef for afw_runtime_const_object_instance_t.
 *
 * See afw_runtime.h for struct and more information.
 */
typedef struct afw_runtime_const_object_instance_s
afw_runtime_const_object_instance_t;

/**
 * Opaque typedef for afw_runtime_const_object_meta_object_t.
 *
 * See afw_runtime.h for struct and more information.
 */
typedef struct afw_runtime_const_object_meta_object_s
afw_runtime_const_object_meta_object_t;

/**
 * Opaque typedef for afw_runtime_custom_t.
 *
 * See afw_runtime.h for struct and more information.
 */
typedef struct afw_runtime_custom_s
afw_runtime_custom_t;

/**
 * Opaque typedef for afw_runtime_property_t.
 *
 * See afw_runtime.h for struct and more information.
 */
typedef struct afw_runtime_property_s
afw_runtime_property_t;

/**
 * Opaque typedef for afw_runtime_unresolved_const_array_t.
 *
 * See afw_runtime.h for struct and more information.
 */
typedef struct afw_runtime_unresolved_const_array_s
afw_runtime_unresolved_const_array_t;

/**
 * Opaque typedef for afw_runtime_unresolved_const_object_t.
 *
 * See afw_runtime.h for struct and more information.
 */
typedef struct afw_runtime_unresolved_const_object_s
afw_runtime_unresolved_const_object_t;

/**
 * Opaque typedef for afw_runtime_unresolved_property_t.
 *
 * See afw_runtime.h for struct and more information.
 */
typedef struct afw_runtime_unresolved_property_s
afw_runtime_unresolved_property_t;

/**
 * Opaque typedef for afw_runtime_unresolved_const_embedded_untyped_object_t.
 *
 * See afw_runtime.h for struct and more information.
 */
typedef struct afw_runtime_unresolved_const_embedded_untyped_object_s
afw_runtime_unresolved_const_embedded_untyped_object_t;

/**
 * Opaque typedef for afw_runtime_unresolved_value_t.
 *
 * See afw_runtime.h for struct and more information.
 */
typedef struct afw_runtime_unresolved_value_s
afw_runtime_unresolved_value_t;

/**
 * Opaque typedef for afw_xctx_evaluation_stack_t.
 *
 * See afw_common.h for struct and more information.
 */
typedef struct afw_xctx_evaluation_stack_s
afw_xctx_evaluation_stack_t;

/**
 * Opaque typedef for afw_xctx_evaluation_stack_entry_t.
 *
 * See afw_common.h for struct and more information.
 */
typedef struct afw_xctx_evaluation_stack_entry_s
afw_xctx_evaluation_stack_entry_t;

/**
 * @brief Typedef for xctx qualifier stack.
 *
 * Struct defined in afw_xctx_internal.h.
 */
typedef struct afw_xctx_internal_qualifier_stack_s
afw_xctx_qualifier_stack_t;

/**
 * @brief Typedef for xctx qualifier stack entry.
 * 
 * Struct defined in afw_xctx.h.
 */
typedef struct afw_xctx_qualifier_stack_entry_s
    afw_xctx_qualifier_stack_entry_t;

/**
 * @brief Typedef for service.
 * 
 * Struct defined in afw_service.h.
 */
typedef struct afw_service_s
afw_service_t;

/**
 * Opaque typedef for afw_stack_t.
 *
 * See afw_stack.h for struct and more information.
 */
typedef struct afw_stack_s afw_stack_t;

/**
 * @brief Typedef for stream anchor.
 *
 * Struct defined in afw_stream.h.
 */
typedef struct afw_stream_anchor_s afw_stream_anchor_t;

/**
 * @brief Typedef for stream.
 *
 * Struct defined in afw_stream.h.
 */
typedef struct afw_stream_s afw_stream_t;

/**
 * Opaque typedef for afw_thread_t.
 *
 * See afw_thread.h for struct and more information.
 */
typedef struct afw_thread_s
afw_thread_t;

/**
 * Opaque typedef for afw_thread_mutex_t.
 *
 * See afw_thread.h for struct and more information.
 */
typedef struct afw_thread_mutex_s
afw_thread_mutex_t;

/**
 * Opaque typedef for afw_try_t.
 *
 * See afw_error.h for struct and more information.
 */
typedef struct afw_try_s
afw_try_t;

/**
 * Opaque typedef for afw_value_assignment_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_assignment_s
afw_value_assignment_t;

/**
 * Opaque typedef for afw_value_assignment_target_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_assignment_target_s
afw_value_assignment_target_t;

/**
 * Opaque typedef for afw_value_block_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_block_s
afw_value_block_t;

/**
 * Opaque typedef for afw_value_block_frame_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_block_frame_s
afw_value_block_frame_t;

/**
 * Opaque typedef for afw_value_block_symbol_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_block_symbol_s
afw_value_block_symbol_t;

/**
 * Opaque typedef for afw_value_call_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_call_s
afw_value_call_t;

/**
 * Opaque typedef for afw_value_call_built_in_function_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_call_built_in_function_s
afw_value_call_built_in_function_t;

/**
 * Opaque typedef for afw_value_call_script_function_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_call_script_function_s
afw_value_call_script_function_t;

/**
 * Opaque typedef for afw_value_call_args_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_call_args_s
afw_value_call_args_t;

/**
 * Opaque typedef for afw_value_closure_binding_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_closure_binding_s
afw_value_closure_binding_t;

/**
 * Opaque typedef for afw_value_compiled_value_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_internal_compiled_value_s
afw_value_compiled_value_t;

/**
 * Opaque typedef for afw_value_compiler_listing_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_compiler_listing_s
afw_value_compiler_listing_t;

/**
 * Opaque typedef for afw_compile_value_contextual_t.
 *
 * See afw_compile_internal.h for struct and more information.
 */
typedef struct afw_compile_internal_value_contextual_s
afw_compile_value_contextual_t;

/**
 * Opaque typedef for afw_value_evaluated_t.
 *
 * See afw_value.h for struct and more information.
 */
typedef struct afw_value_evaluated_s
afw_value_evaluated_t;

/**
 * Opaque typedef for afw_value_function_definition_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_function_definition_s
afw_value_function_definition_t;

/**
 * Opaque typedef for afw_value_function_parameter_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_function_parameter_s
afw_value_function_parameter_t;

/**
 * Opaque typedef for afw_value_function_thunk_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_function_thunk_s
afw_value_function_thunk_t;

/**
 * Opaque typedef for afw_value_info_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_info_s
afw_value_info_t;

/**
 * Opaque typedef for afw_value_script_function_definition_t.
 *
 * See afw_value.h for struct and more information.
 */
typedef struct afw_value_script_function_definition_s
afw_value_script_function_definition_t;

/**
 * Opaque typedef for afw_value_script_function_parameter_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_script_function_parameter_s
afw_value_script_function_parameter_t;

/**
 * Opaque typedef for afw_value_script_function_signature_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_script_function_signature_s
afw_value_script_function_signature_t;

/**
 * Opaque typedef for afw_value_list_expression_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_list_expression_s
afw_value_list_expression_t;

/**
 * Opaque typedef for afw_value_object_expression_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_object_expression_s
afw_value_object_expression_t;

/**
 * Opaque typedef for afw_value_qualified_variable_reference_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_qualified_variable_reference_s
afw_value_qualified_variable_reference_t;

/**
 * Opaque typedef for afw_value_reference_by_key_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_reference_by_key_s
afw_value_reference_by_key_t;

/**
 * Opaque typedef for afw_value_template_definition_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_template_definition_s
afw_value_template_definition_t;

/**
 * Opaque typedef for afw_value_type_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_type_s
afw_value_type_t;

/**
 * Opaque typedef for afw_value_type_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_type_list_s
afw_value_type_list_t;

/**
 * Opaque typedef for afw_value_symbol_reference_t.
 *
 * See afw_value_internal.h for struct and more information.
 */
typedef struct afw_value_symbol_reference_s
afw_value_symbol_reference_t;

/**
 * Opaque typedef for afw_xctx_scope_t.
 *
 * See afw_xctx.h for struct and more information.
 */
typedef struct afw_xctx_scope_s
afw_xctx_scope_t;


/** @} */

#endif /* __AFW_COMMON_OPAQUES_H__ */
