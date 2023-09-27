# See the 'COPYING' file in the project root for licensing information.
#
# File list variables for subdir afw
#
#----------------------------- N O T E -------------------------------------
#
# This file is generated by "afwdev generate".
#
# Do not change this file directly or the changes will be lost the next time
# this file is regenerated.
#
#----------------------------------------------------------------------------
#
# This files sets 3 list variables based on files and directories in srcdir
# afw that are needed a build time:
#
#  AFWDEV_GENERATED_PUBLIC_HEADERS_LIST - All of the public header files.
#  AFWDEV_GENERATED_INCLUDE_DIRECTORIES_LIST - All of the build time include directories.
#  AFWDEV_GENERATED_SOURCE_LIST - All of the c source files to compile.
#
# After including this file, use cmake commands like list(FILTER ...) to modify
# the lists as needed.
#

# Header files that are installed for public use.
set(AFWDEV_GENERATED_PUBLIC_HEADERS_LIST 
    action/afw_action.h
    adaptor/afw_adaptor.h
    adaptor/afw_adaptor_add.h
    adaptor/afw_adaptor_delete.h
    adaptor/afw_adaptor_get.h
    adaptor/afw_adaptor_impl.h
    adaptor/afw_adaptor_impl_index.h
    adaptor/afw_adaptor_internal.h
    adaptor/afw_adaptor_journal.h
    adaptor/afw_adaptor_modify.h
    adaptor/afw_adaptor_replace.h
    adaptor/afw_adaptor_retrieve.h
    application/afw_application_internal.h
    array/afw_array.h
    array/afw_array_impl.h
    array/afw_array_internal.h
    array_template/afw_array_template.h
    ascii/afw_ascii.h
    associative_array/afw_associative_array.h
    associative_array/afw_associative_array_template.h
    atomic/afw_atomic.h
    authorization/afw_authorization.h
    authorization/afw_authorization_impl.h
    authorization/afw_authorization_internal.h
    compile/afw_compile.h
    compile/afw_compile_code_point.h
    compile/afw_compile_internal.h
    components/afw_components.h
    content_type/afw_content_type.h
    content_type/afw_content_type_impl.h
    context/afw_context.h
    data_type/afw_data_type.h
    debug/afw_debug.h
    endian/afw_endian.h
    environment/afw_environment.h
    environment/afw_environment_internal.h
    error/afw_error.h
    file/afw_file.h
    file/afw_file_internal.h
    flag/afw_flag.h
    function/afw_function.h
    generated/afw_adaptor_factory_impl_declares.h
    generated/afw_adaptor_impl_declares.h
    generated/afw_adaptor_impl_index_cursor_impl_declares.h
    generated/afw_adaptor_impl_index_impl_declares.h
    generated/afw_adaptor_journal_impl_declares.h
    generated/afw_adaptor_key_value_impl_declares.h
    generated/afw_adaptor_object_type_cache_impl_declares.h
    generated/afw_adaptor_session_impl_declares.h
    generated/afw_adaptor_transaction_impl_declares.h
    generated/afw_array_impl_declares.h
    generated/afw_array_setter_impl_declares.h
    generated/afw_authorization_handler_factory_impl_declares.h
    generated/afw_authorization_handler_impl_declares.h
    generated/afw_connection_impl_declares.h
    generated/afw_const_objects.h
    generated/afw_content_type_impl_declares.h
    generated/afw_content_type_object_list_writer_impl_declares.h
    generated/afw_data_type_anyURI_binding.h
    generated/afw_data_type_any_binding.h
    generated/afw_data_type_array_binding.h
    generated/afw_data_type_base64Binary_binding.h
    generated/afw_data_type_bindings.h
    generated/afw_data_type_boolean_binding.h
    generated/afw_data_type_dateTime_binding.h
    generated/afw_data_type_date_binding.h
    generated/afw_data_type_dayTimeDuration_binding.h
    generated/afw_data_type_dnsName_binding.h
    generated/afw_data_type_double_binding.h
    generated/afw_data_type_function_binding.h
    generated/afw_data_type_hexBinary_binding.h
    generated/afw_data_type_ia5String_binding.h
    generated/afw_data_type_impl_declares.h
    generated/afw_data_type_integer_binding.h
    generated/afw_data_type_ipAddress_binding.h
    generated/afw_data_type_json_binding.h
    generated/afw_data_type_null_binding.h
    generated/afw_data_type_objectId_binding.h
    generated/afw_data_type_objectPath_binding.h
    generated/afw_data_type_object_binding.h
    generated/afw_data_type_password_binding.h
    generated/afw_data_type_regexp_binding.h
    generated/afw_data_type_relaxed_json_binding.h
    generated/afw_data_type_rfc822Name_binding.h
    generated/afw_data_type_script_binding.h
    generated/afw_data_type_string_binding.h
    generated/afw_data_type_template_binding.h
    generated/afw_data_type_time_binding.h
    generated/afw_data_type_typedefs.h
    generated/afw_data_type_undefined_binding.h
    generated/afw_data_type_unevaluated_binding.h
    generated/afw_data_type_unknown_binding.h
    generated/afw_data_type_void_binding.h
    generated/afw_data_type_x500Name_binding.h
    generated/afw_data_type_xpathExpression_binding.h
    generated/afw_data_type_yearMonthDuration_binding.h
    generated/afw_declare_helpers.h
    generated/afw_extension_impl_declares.h
    generated/afw_function_bindings.h
    generated/afw_generated.h
    generated/afw_interface.h
    generated/afw_interface_opaques.h
    generated/afw_iterator_impl_declares.h
    generated/afw_log_factory_impl_declares.h
    generated/afw_log_impl_declares.h
    generated/afw_object_associative_array_impl_declares.h
    generated/afw_object_impl_declares.h
    generated/afw_object_setter_impl_declares.h
    generated/afw_pool_impl_declares.h
    generated/afw_request_handler_factory_impl_declares.h
    generated/afw_request_handler_impl_declares.h
    generated/afw_request_impl_declares.h
    generated/afw_runtime_object_maps.h
    generated/afw_server_impl_declares.h
    generated/afw_service_type_impl_declares.h
    generated/afw_stream_impl_declares.h
    generated/afw_strings.h
    generated/afw_value_impl_declares.h
    generated/afw_variable_handler_impl_declares.h
    generated/afw_version_info.h
    generated/afw_writer_impl_declares.h
    include/afw.h
    include/afw_common.h
    include/afw_common_opaques.h
    include/afw_doxygen.h
    include/afw_interface_common.h
    include/afw_internal.h
    include/afw_minimal.h
    json/afw_json.h
    json/afw_json_internal.h
    lock/afw_lock.h
    log/afw_log.h
    log/afw_log_deprecated.h
    log/afw_log_deprecated_interface.h
    log/afw_log_impl.h
    log/afw_log_internal.h
    memory/afw_memory.h
    model/afw_model.h
    model/afw_model_internal.h
    model/afw_model_location.h
    number/afw_number.h
    object/afw_object.h
    object/afw_object_impl.h
    object/afw_object_internal.h
    object/afw_object_meta.h
    object/afw_object_options.h
    object/afw_object_path.h
    object/afw_object_type.h
    object/afw_object_view.h
    object/afw_object_view_internal.h
    os/afw_os.h
    pool/afw_pool.h
    pool/afw_pool_internal.h
    query_criteria/afw_query_criteria.h
    request/afw_request.h
    request/afw_request_handler.h
    request/afw_request_handler_internal.h
    request/afw_request_impl.h
    runtime/afw_runtime.h
    runtime/afw_runtime_value_accessor.h
    safe_cast/afw_safe_cast.h
    service/afw_service.h
    service/afw_service_internal.h
    stack/afw_stack.h
    stream/afw_stream.h
    thread/afw_thread.h
    time/afw_time.h
    trace/afw_trace.h
    uri/afw_uri.h
    utf8/afw_utf8.h
    uuid/afw_uuid.h
    value/afw_value.h
    value/afw_value_internal.h
    version/afw_version.h
    writer/afw_writer.h
    xctx/afw_xctx.h
    xctx/afw_xctx_internal.h
)

# Full path of directories searched for headers at build time.
set(AFWDEV_GENERATED_INCLUDE_DIRECTORIES_LIST 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/action
    ${CMAKE_CURRENT_SOURCE_DIR}/adaptor
    ${CMAKE_CURRENT_SOURCE_DIR}/application
    ${CMAKE_CURRENT_SOURCE_DIR}/array
    ${CMAKE_CURRENT_SOURCE_DIR}/array_template
    ${CMAKE_CURRENT_SOURCE_DIR}/ascii
    ${CMAKE_CURRENT_SOURCE_DIR}/associative_array
    ${CMAKE_CURRENT_SOURCE_DIR}/atomic
    ${CMAKE_CURRENT_SOURCE_DIR}/authorization
    ${CMAKE_CURRENT_SOURCE_DIR}/compile
    ${CMAKE_CURRENT_SOURCE_DIR}/components
    ${CMAKE_CURRENT_SOURCE_DIR}/content_type
    ${CMAKE_CURRENT_SOURCE_DIR}/context
    ${CMAKE_CURRENT_SOURCE_DIR}/data_type
    ${CMAKE_CURRENT_SOURCE_DIR}/debug
    ${CMAKE_CURRENT_SOURCE_DIR}/endian
    ${CMAKE_CURRENT_SOURCE_DIR}/environment
    ${CMAKE_CURRENT_SOURCE_DIR}/error
    ${CMAKE_CURRENT_SOURCE_DIR}/file
    ${CMAKE_CURRENT_SOURCE_DIR}/flag
    ${CMAKE_CURRENT_SOURCE_DIR}/function
    ${CMAKE_CURRENT_SOURCE_DIR}/generated
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/json
    ${CMAKE_CURRENT_SOURCE_DIR}/lock
    ${CMAKE_CURRENT_SOURCE_DIR}/log
    ${CMAKE_CURRENT_SOURCE_DIR}/memory
    ${CMAKE_CURRENT_SOURCE_DIR}/model
    ${CMAKE_CURRENT_SOURCE_DIR}/number
    ${CMAKE_CURRENT_SOURCE_DIR}/object
    ${CMAKE_CURRENT_SOURCE_DIR}/os
    ${CMAKE_CURRENT_SOURCE_DIR}/pool
    ${CMAKE_CURRENT_SOURCE_DIR}/query_criteria
    ${CMAKE_CURRENT_SOURCE_DIR}/request
    ${CMAKE_CURRENT_SOURCE_DIR}/runtime
    ${CMAKE_CURRENT_SOURCE_DIR}/safe_cast
    ${CMAKE_CURRENT_SOURCE_DIR}/service
    ${CMAKE_CURRENT_SOURCE_DIR}/stack
    ${CMAKE_CURRENT_SOURCE_DIR}/stream
    ${CMAKE_CURRENT_SOURCE_DIR}/thread
    ${CMAKE_CURRENT_SOURCE_DIR}/time
    ${CMAKE_CURRENT_SOURCE_DIR}/trace
    ${CMAKE_CURRENT_SOURCE_DIR}/uri
    ${CMAKE_CURRENT_SOURCE_DIR}/utf8
    ${CMAKE_CURRENT_SOURCE_DIR}/uuid
    ${CMAKE_CURRENT_SOURCE_DIR}/value
    ${CMAKE_CURRENT_SOURCE_DIR}/version
    ${CMAKE_CURRENT_SOURCE_DIR}/writer
    ${CMAKE_CURRENT_SOURCE_DIR}/xctx
)

# Source files to compile.
set(AFWDEV_GENERATED_SOURCE_LIST 
    action/afw_action.c
    adaptor/afw_adaptor.c
    adaptor/afw_adaptor_add.c
    adaptor/afw_adaptor_context.c
    adaptor/afw_adaptor_delete.c
    adaptor/afw_adaptor_get.c
    adaptor/afw_adaptor_impl.c
    adaptor/afw_adaptor_impl_index.c
    adaptor/afw_adaptor_journal.c
    adaptor/afw_adaptor_modify.c
    adaptor/afw_adaptor_replace.c
    adaptor/afw_adaptor_retrieve.c
    application/afw_application.c
    array/afw_array.c
    array/afw_array_const_array.c
    array/afw_array_impl.c
    array/afw_array_memory.c
    array/afw_array_wrapper_for_array.c
    ascii/afw_ascii.c
    associative_array/afw_associative_array.c
    authorization/afw_authorization.c
    authorization/afw_authorization_handler_type_script.c
    authorization/afw_authorization_impl.c
    compile/afw_compile.c
    compile/afw_compile_code_point.c
    compile/afw_compile_lexical.c
    compile/afw_compile_parse.c
    compile/afw_compile_parse_assignment_target.c
    compile/afw_compile_parse_expression.c
    compile/afw_compile_parse_script.c
    compile/afw_compile_parse_template.c
    compile/afw_compile_parse_value.c
    content_type/afw_content_type.c
    content_type/afw_content_type_application_afw.c
    content_type/afw_content_type_impl.c
    context/afw_context.c
    data_type/afw_data_type.c
    debug/afw_debug.c
    environment/afw_environment.c
    environment/afw_environment_configuration.c
    environment/afw_environment_register_core.c
    environment/afw_environment_registry_object.c
    environment/afw_environment_variables_object.c
    error/afw_error.c
    file/afw_file.c
    file/afw_file_journal.c
    flag/afw_flag.c
    function/afw_function.c
    function/afw_function_adaptor.c
    function/afw_function_administrative.c
    function/afw_function_anyURI.c
    function/afw_function_array.c
    function/afw_function_authorization.c
    function/afw_function_base64Binary.c
    function/afw_function_compiler.c
    function/afw_function_compiler_expression.c
    function/afw_function_compiler_script.c
    function/afw_function_date.c
    function/afw_function_dateTime.c
    function/afw_function_double.c
    function/afw_function_hexBinary.c
    function/afw_function_higher_order_array.c
    function/afw_function_indexes.c
    function/afw_function_integer.c
    function/afw_function_journal.c
    function/afw_function_json.c
    function/afw_function_logical.c
    function/afw_function_miscellaneous.c
    function/afw_function_model.c
    function/afw_function_object.c
    function/afw_function_polymorphic.c
    function/afw_function_random.c
    function/afw_function_regexp.c
    function/afw_function_relaxed_json.c
    function/afw_function_rfc822Name.c
    function/afw_function_script.c
    function/afw_function_stream.c
    function/afw_function_string.c
    function/afw_function_template.c
    function/afw_function_time.c
    function/afw_function_value.c
    function/afw_function_x500Name.c
    function/afw_function_xml.c
    function/afw_function_xpathExpression.c
    generated/afw_const_objects.c
    generated/afw_data_type_anyURI_binding.c
    generated/afw_data_type_any_binding.c
    generated/afw_data_type_array_binding.c
    generated/afw_data_type_base64Binary_binding.c
    generated/afw_data_type_bindings.c
    generated/afw_data_type_boolean_binding.c
    generated/afw_data_type_dateTime_binding.c
    generated/afw_data_type_date_binding.c
    generated/afw_data_type_dayTimeDuration_binding.c
    generated/afw_data_type_dnsName_binding.c
    generated/afw_data_type_double_binding.c
    generated/afw_data_type_function_binding.c
    generated/afw_data_type_hexBinary_binding.c
    generated/afw_data_type_ia5String_binding.c
    generated/afw_data_type_integer_binding.c
    generated/afw_data_type_ipAddress_binding.c
    generated/afw_data_type_json_binding.c
    generated/afw_data_type_null_binding.c
    generated/afw_data_type_objectId_binding.c
    generated/afw_data_type_objectPath_binding.c
    generated/afw_data_type_object_binding.c
    generated/afw_data_type_password_binding.c
    generated/afw_data_type_regexp_binding.c
    generated/afw_data_type_relaxed_json_binding.c
    generated/afw_data_type_rfc822Name_binding.c
    generated/afw_data_type_script_binding.c
    generated/afw_data_type_string_binding.c
    generated/afw_data_type_template_binding.c
    generated/afw_data_type_time_binding.c
    generated/afw_data_type_undefined_binding.c
    generated/afw_data_type_unevaluated_binding.c
    generated/afw_data_type_unknown_binding.c
    generated/afw_data_type_void_binding.c
    generated/afw_data_type_x500Name_binding.c
    generated/afw_data_type_xpathExpression_binding.c
    generated/afw_data_type_yearMonthDuration_binding.c
    generated/afw_function_bindings.c
    generated/afw_generated.c
    generated/afw_runtime_object_maps.c
    generated/afw_strings.c
    json/afw_json.c
    json/afw_json_from_value.c
    lock/afw_lock.c
    log/afw_log.c
    log/afw_log_deprecated.c
    log/afw_log_file.c
    log/afw_log_standard.c
    memory/afw_memory.c
    model/afw_model.c
    model/afw_model_adaptor.c
    model/afw_model_compile.c
    model/afw_model_context.c
    model/afw_model_internal.c
    model/afw_model_location.c
    number/afw_number.c
    object/afw_object.c
    object/afw_object_aggregate_external.c
    object/afw_object_composite.c
    object/afw_object_const_key_value.c
    object/afw_object_impl.c
    object/afw_object_impl_property_meta.c
    object/afw_object_memory.c
    object/afw_object_memory_associative_array.c
    object/afw_object_meta.c
    object/afw_object_meta_accessor.c
    object/afw_object_meta_properties.c
    object/afw_object_meta_property.c
    object/afw_object_meta_propertyType.c
    object/afw_object_meta_propertyTypes.c
    object/afw_object_options.c
    object/afw_object_path.c
    object/afw_object_properties_callback.c
    object/afw_object_type.c
    object/afw_object_view.c
    os/nix/afw_os.c
    os/nix/afw_os_log.c
    os/win/afw_os.c
    os/win/afw_os_log.c
    pool/afw_pool_multithreaded.c
    pool/afw_pool_singlethreaded.c
    query_criteria/afw_query_criteria.c
    request/afw_request.c
    request/afw_request_handler.c
    request/afw_request_handler_adaptor.c
    request/afw_request_handler_director.c
    request/afw_request_handler_factory_adaptor.c
    request/afw_request_impl.c
    runtime/afw_runtime.c
    runtime/afw_runtime_const_meta.c
    runtime/afw_runtime_value_accessor.c
    service/afw_service.c
    service/afw_service_context.c
    stack/afw_stack.c
    stream/afw_stream.c
    stream/afw_stream_fd.c
    thread/afw_thread.c
    time/afw_time.c
    trace/afw_trace.c
    uri/afw_uri.c
    utf8/afw_utf8.c
    utf8/afw_utf8_stream.c
    utf8/afw_utf8_writer.c
    uuid/afw_uuid.c
    value/afw_value.c
    value/afw_value_assignment_target.c
    value/afw_value_block.c
    value/afw_value_call.c
    value/afw_value_call_built_in_function.c
    value/afw_value_call_script_function.c
    value/afw_value_call_test_script.c
    value/afw_value_closure_binding.c
    value/afw_value_compiled_value.c
    value/afw_value_compiler_listing.c
    value/afw_value_decompile.c
    value/afw_value_evaluated.c
    value/afw_value_function_definition.c
    value/afw_value_function_thunk.c
    value/afw_value_list_expression.c
    value/afw_value_meta.c
    value/afw_value_meta_values_list.c
    value/afw_value_meta_values_object.c
    value/afw_value_object_expression.c
    value/afw_value_qualified_variable_reference.c
    value/afw_value_reference_by_key.c
    value/afw_value_script_function.c
    value/afw_value_symbol_reference.c
    value/afw_value_template_definition.c
    version/afw_version.c
    writer/afw_writer.c
    xctx/afw_xctx.c
    xctx/afw_xctx_qualifier_object.c
)
