#! /usr/bin/env python3

import os
from _afwdev.generate import c
from _afwdev.common import msg, nfc


def write_typedefs_h_section(fd, prefix, obj):
    id = obj['_meta_']['objectId']

    # internal type
    ctype = obj['cType']

    # parameter is always a const pointer
    parameter_ctype = ctype
    if not ctype.startswith('const'): parameter_ctype='const ' + parameter_ctype
    if not ctype.endswith('*'): parameter_ctype= parameter_ctype + ' *'

    # return type is influenced by directReturn
    direct_return = obj.get('directReturn', False) == True
    if direct_return: return_type = ctype
    else: return_type = parameter_ctype

    fd.write('\n/** @brief Typedef for data type ' + id + ' internal. */\n')
    fd.write('typedef ' + ctype + ' ' + prefix + 'data_type_' + id + '_internal_t;\n')

    fd.write('\n/** @brief Typedef for data type ' + id + ' parameter. */\n')
    fd.write('typedef ' + parameter_ctype + ' ' + prefix + 'data_type_' + id + '_parameter_t;\n')

    fd.write('\n/** @brief Typedef for data type ' + id + ' return. */\n')
    fd.write('typedef ' + return_type + ' ' + prefix + 'data_type_' + id + '_return_t;\n')

    fd.write('\n/** @brief Typedef for data type ' + id + ' evaluated value. */\n')
    fd.write('typedef struct afw_value_' + id + '_s\n' + '    afw_value_' + id + '_t;\n')



def write_h_section(fd, prefix, obj):
    declare = prefix.upper() + 'DECLARE'
    define = prefix.upper() + 'DEFINE'
    define_inline = prefix.upper() + 'DEFINE_STATIC_INLINE'
    declare_data =  prefix.upper() + 'DECLARE_CONST_DATA'
    id = obj['_meta_']['objectId']

    # internal type
    ctype = obj['cType']

    # parameter is always a const pointer
    parameter_ctype = ctype
    if not ctype.startswith('const'): parameter_ctype='const ' + parameter_ctype
    if not ctype.endswith('*'): parameter_ctype= parameter_ctype + ' *'

    # return type is influenced by directReturn
    direct_return = obj.get('directReturn', False) == True
    if direct_return: return_type = ctype
    else: return_type = parameter_ctype

    # needs found
    needs_found = direct_return and ctype.find('*') == -1
    needs_found_param = ''
    if needs_found:
        needs_found_param = 'found, '
    amp_if_needed = ''
    if not direct_return: amp_if_needed='&'

    fd.write('\n/** @brief Quoted data type id for ' + id + ' */\n')
    fd.write('#define AFW_DATA_TYPE_ID_Q_' + id + ' "' + id + '"\n')
    fd.write('\n/** @brief Unquoted data type id for ' + id + ' */\n')
    fd.write('#define AFW_DATA_TYPE_ID_U_' + id + ' ' + id + '\n')
    fd.write('\n/** @brief String for data type id ' + id + ' */\n')
    fd.write('#define AFW_DATA_TYPE_ID_S_' + id + ' afw_s_' + id + '\n')
    fd.write('\n/** @brief Quoted c type for data type id ' + id + ' */\n')
    fd.write('#define AFW_DATA_TYPE_CTYPE_Q_' + id + ' "' + ctype + '"\n')

    fd.write('\n/** @brief Adaptive string value for \"' + id + '\". */\n')
    fd.write(declare_data + '(afw_value_t *)\n')
    fd.write('afw_data_type_' + id + '_id_value;\n')

    fd.write('\n/** @brief Data type struct for ' + id + '. */\n')
    fd.write(declare_data + '(afw_data_type_t *)\n')
    fd.write('afw_data_type_' + id + ';\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Unmanaged evaluated value inf for data type ' + id + '.\n')
    fd.write(' *\n')
    fd.write(' * The lifetime of the value is the lifetime of its containing pool.\n')
    fd.write(' */\n')
    fd.write(declare_data + '(afw_value_inf_t)\n')
    fd.write('afw_value_evaluated_' + id + '_inf;\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Managed evaluated value inf for data type ' + id + '.\n')
    fd.write(' *\n')
    fd.write(' * The lifetime of the value is managed by reference.\n')
    fd.write(' */\n')
    fd.write(declare_data + '(afw_value_inf_t)\n')
    fd.write('afw_value_managed_' + id + '_inf;\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Permanent managed (life of afw environment) value inf for data type ' + id + '.\n')
    fd.write(' *\n')
    fd.write(' * The lifetime of the value is the lifetime of the afw environment.\n')
    fd.write(' */\n')
    fd.write(declare_data + '(afw_value_inf_t)\n')
    fd.write('afw_value_permanent_' + id + '_inf;\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Macro to determine if data type is ' + id + '.\n')
    fd.write(' * @param A_DATA_TYPE to test.\n')
    fd.write(' * @return boolean result.\n')
    fd.write(' */\n')
    fd.write('#define afw_data_type_is_' + id + '(A_DATA_TYPE) \\\n')
    fd.write('( \\\n')
    fd.write('    (A_DATA_TYPE) && \\\n')
    fd.write('    (A_DATA_TYPE) == afw_data_type_' + id + ' \\\n')
    fd.write(')\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Macro to determine if value is evaluated ' + id + '.\n')
    fd.write(' * @param A_VALUE to test.\n')
    fd.write(' * @return boolean result.\n')
    fd.write(' */\n')
    fd.write('#define afw_value_is_' + id + '(A_VALUE) \\\n')
    fd.write('( \\\n')
    fd.write('    (A_VALUE) && \\\n')
    fd.write('    (A_VALUE)->inf->is_evaluated_of_data_type == afw_data_type_' + id + '\\\n')
    fd.write(')\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Macro to determine if value is evaluated list of ' + id + '.\n')
    fd.write(' * @param A_VALUE to test.\n')
    fd.write(' * @return boolean result.\n')
    fd.write(' */\n')
    fd.write('#define afw_value_is_list_of_' + id + '(A_VALUE) \\\n')
    fd.write('( \\\n')
    fd.write('    afw_value_is_list(A_VALUE) && \\\n')
    fd.write('    afw_list_get_data_type( \\\n')
    fd.write('        ((const afw_value_list_t *)(A_VALUE))->internal, \\\n')
    fd.write('        xctx) == afw_data_type_' + id + ' \\\n')
    fd.write(')\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Convert data type ' + id + ' string to ' + ctype + ' *.\n')
    fd.write(' * @param to_internal place to return result.  Must be sizeof(' + ctype + ').\n')
    fd.write(' * @param from_utf8 to convert.\n')
    fd.write(' * @param p to use for returned internal.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' */\n')
    fd.write(declare + '(void)\nafw_data_type_' + id + '_to_internal(' + ctype + ' *to_internal,\n')
    fd.write('    const afw_utf8_t *from_utf8, const afw_pool_t *p, afw_xctx_t *xctx);\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Convert data type ' + id + ' internal representation to utf-8.\n')
    fd.write(' * @param internal (' + return_type + ') to convert.\n')
    fd.write(' * @param p to use for returned string.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' * @return (const afw_utf8_t *) normalized string representation of value.\n')
    fd.write(' */\n')
    fd.write(declare + '(const afw_utf8_t *)\nafw_data_type_' + id + '_to_utf8(' + return_type + ' internal,\n')
    fd.write('    const afw_pool_t *p, afw_xctx_t *xctx);\n')

    fd.write('\n/** @brief struct for data type ' + id + ' values. */\n')
    fd.write('struct afw_value_' + id + '_s {\n')
    fd.write('    const afw_value_inf_t *inf;\n')
    fd.write('    ' + ctype + ' internal;\n')
    fd.write('};\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Typesafe cast of data type ' + id + '.\n')
    fd.write(' * @param value (const afw_value_t *).\n')
    fd.write(' * @return (' + return_type + ')\n')
    fd.write(' */\n')
    fd.write(declare + '(' + return_type + ')\n')
    fd.write('afw_value_as_' + id + '(const afw_value_t *value, afw_xctx_t *xctx);\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Allocate function for unmanaged data type ' + id + ' value.\n')
    fd.write(' * @param p to use for returned value.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' * @return Allocated afw_value_' + id + '_t with appropriate inf set.\n')
    fd.write(' */\n')
    fd.write(declare + '(afw_value_' + id + '_t *)\n')
    fd.write('afw_value_allocate_' + id + '(const afw_pool_t *p, afw_xctx_t *xctx);\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Create function for unmanaged data type ' + id + ' value.\n')
    fd.write(' * @param internal.\n')
    fd.write(' * @param p to use for returned value.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' * @return Created const afw_value_t *.\n')
    fd.write(' */\n')
    fd.write(declare + '(const afw_value_t *)\n')
    fd.write('afw_value_create_' + id + '(' + return_type + ' internal,\n')
    fd.write('    const afw_pool_t *p, afw_xctx_t *xctx);\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Allocate function for managed data type ' + id + ' value.\n')
    fd.write(' * @param p to use for returned value.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' * @return Allocated afw_value_' + id + '_t with appropriate inf set.\n')
    fd.write(' */\n')
    fd.write(declare + '(afw_value_' + id + '_t *)\n')
    fd.write('afw_value_allocate_managed_' + id + '(const afw_pool_t *p, afw_xctx_t *xctx);\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Create function for managed data type ' + id + ' value.\n')
    fd.write(' * @param internal.\n')
    fd.write(' * @param p to use for returned value.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' * @return Created const afw_value_t *.\n')
    fd.write(' */\n')
    fd.write(declare + '(const afw_value_t *)\n')
    fd.write('afw_value_create_managed_' + id + '(' + return_type + ' internal,\n')
    fd.write('    const afw_pool_t *p, afw_xctx_t *xctx);\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Allocate function for permanent data type ' + id + ' value.\n')
    fd.write(' * @param p to use for returned value.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' * @return Allocated afw_value_' + id + '_t with appropriate inf set.\n')
    fd.write(' */\n')
    fd.write(declare + '(afw_value_' + id + '_t *)\n')
    fd.write('afw_value_allocate_permanent_' + id + '(const afw_pool_t *p, afw_xctx_t *xctx);\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Create function for permanent data type ' + id + ' value.\n')
    fd.write(' * @param internal.\n')
    fd.write(' * @param p to use for returned value.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' * @return Created const afw_value_t *.\n')
    fd.write(' */\n')
    fd.write(declare + '(const afw_value_t *)\n')
    fd.write('afw_value_create_permanent_' + id + '(' + return_type + ' internal,\n')
    fd.write('    const afw_pool_t *p, afw_xctx_t *xctx);\n')

    if needs_found:
        fd.write('\n/* Data type ' + id + ' not yet supported for some functions. */\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Get property function for data type ' + id + ' value.\n')
    fd.write(' * @deprecated\n')
    fd.write(' * @param object of property to get.\n')
    fd.write(' * @param property_name of property to get.\n')
    if needs_found:
        fd.write(' * @param found is place to return whether property is found.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' * @return ' + return_type + '.\n')
    fd.write(' *\n')
    fd.write(' * This is a deprecated function used to get around an exception that\n')
    fd.write(' * was occurring when an object did not have a pool. Use the function\n')
    fd.write(' * without an "_old" in the name for all new code and replace calls in\n')
    fd.write(' * old code when possible.\n')
    fd.write(' *\n')
    fd.write(' */\n')
    fd.write('#define afw_object_old_get_property_as_' + id + '( \\\n')
    fd.write('    object, property_name, ' + needs_found_param + 'xctx) \\\n')
    fd.write('afw_object_get_property_as_' + id + '_source( \\\n')
    fd.write('    object, property_name, ' + needs_found_param + 'AFW__FILE_LINE__, \\\n')
    fd.write('    ((object)->p ? (object)->p : (xctx)->p), (xctx))\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Get property function for data type ' + id + ' value.\n')
    fd.write(' * @param object of property to get.\n')
    fd.write(' * @param property_name of property to get.\n')
    if needs_found:
        fd.write(' * @param found is place to return whether property is found.\n')
    fd.write(' * @param p to use for result if evaluation or conversion is required.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' * @return ' + return_type + '.\n')
    fd.write(' */\n')
    fd.write('#define afw_object_get_property_as_' + id + '( \\\n')
    fd.write('    object, property_name, ' + needs_found_param + 'p, xctx) \\\n')
    fd.write('afw_object_get_property_as_' + id + '_source( \\\n')
    fd.write('    object, property_name, ' + needs_found_param + 'AFW__FILE_LINE__, p, xctx)\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Get property function for data type ' + id + ' value.\n')
    fd.write(' * @param object of property to get.\n')
    fd.write(' * @param property_name of property to get.\n')
    if needs_found:
        fd.write(' * @param found is place to return whether property is found.\n')
    fd.write(' * @param source_z file:line.\n')
    fd.write(' * @param p to use for result if evaluation or conversion is required.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' * @return ' + return_type + '.\n')
    fd.write(' */\n')
    fd.write(declare + '(' + return_type + ')\n')
    fd.write('afw_object_get_property_as_' + id + '_source(\n')
    fd.write('    const afw_object_t *object,\n')
    fd.write('    const afw_utf8_t *property_name,\n')
    if needs_found:
        fd.write('    afw_boolean_t *found,\n')
    fd.write('    const afw_utf8_z_t *source_z,\n')
    fd.write('    const afw_pool_t *p,\n')
    fd.write('    afw_xctx_t *xctx);\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Get next property function for data type ' + id + ' value.\n')
    fd.write(' * @deprecated\n')
    fd.write(' * @param object of property to get.\n')
    fd.write(' * @param iterator pointer. Set to NULL before first call.\n')
    fd.write(' * @param property_name is place to return pointer to property name.\n')
    if needs_found:
        fd.write(' * @param found is place to return whether property is found.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' * @return ' + return_type + '.\n')
    fd.write(' *\n')
    fd.write(' * This is a deprecated function used to get around an exception that\n')
    fd.write(' * was occurring when an object did not have a pool. Use the function\n')
    fd.write(' * without an "_old" in the name for all new code and replace calls in\n')
    fd.write(' * old code when possible.\n')
    fd.write(' *\n')
    fd.write(' */\n')
    fd.write('#define afw_object_old_get_next_property_as_' + id + '( \\\n')
    fd.write('    object, iterator, property_name, ' + needs_found_param + 'xctx) \\\n')
    fd.write('afw_object_get_next_property_as_' + id + '_source( \\\n')
    fd.write('    object, iterator, property_name, ' + needs_found_param + 'AFW__FILE_LINE__, \\\n')
    fd.write('    ((object)->p ? (object)->p : (xctx)->p), (xctx))\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Get next property function for data type ' + id + ' value.\n')
    fd.write(' * @param object of property to get.\n')
    fd.write(' * @param iterator pointer. Set to NULL before first call.\n')
    fd.write(' * @param property_name is place to return pointer to property name.\n')
    if needs_found:
        fd.write(' * @param found is place to return whether property is found.\n')
    fd.write(' * @param p to use for result if evaluation or conversion is required.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' * @return ' + return_type + '.\n')
    fd.write(' */\n')
    fd.write('#define afw_object_get_next_property_as_' + id + '( \\\n')
    fd.write('    object, iterator, property_name, ' + needs_found_param + 'p, xctx) \\\n')
    fd.write('afw_object_get_next_property_as_' + id + '_source( \\\n')
    fd.write('    object, iterator, property_name, ' + needs_found_param + 'AFW__FILE_LINE__, p, xctx)\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Get property function for data type ' + id + ' value.\n')
    fd.write(' * @param object of property to get.\n')
    fd.write(' * @param iterator pointer. Set to NULL before first call.\n')
    fd.write(' * @param property_name is place to return pointer to property name.\n')
    if needs_found:
        fd.write(' * @param found is place to return whether property is found.\n')
    fd.write(' * @param source_z file:line.\n')
    fd.write(' * @param p to use for result if conversion is required.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' * @return ' + return_type + '.\n')
    fd.write(' */\n')
    fd.write(declare + '(' + return_type + ')\n')
    fd.write('afw_object_get_next_property_as_' + id + '_source(\n')
    fd.write('    const afw_object_t *object,\n')
    fd.write('    const afw_iterator_t * *iterator,\n')
    fd.write('    const afw_utf8_t * *property_name,\n')
    if needs_found:
        fd.write('    afw_boolean_t *found,\n')
    fd.write('    const afw_utf8_z_t *source_z,\n')
    fd.write('    const afw_pool_t *p,\n')
    fd.write('    afw_xctx_t *xctx);\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Set property function for data type ' + id + ' values.\n')
    fd.write(' * @param object of property to set.\n')
    fd.write(' * @param property_name of property to set.\n')
    fd.write(' * @param value of value to set.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' *\n')
    fd.write(' * The value will be allocated in the object\'s pool.')
    fd.write(' *\n')
    fd.write(' */\n')
    fd.write(declare + '(void)\n')
    fd.write('afw_object_set_property_as_' + id + '(\n')
    fd.write('    const afw_object_t *object,\n')
    fd.write('    const afw_utf8_t *property_name,\n')
    fd.write('    ' + return_type + ' internal,\n')
    fd.write('    afw_xctx_t *xctx);\n')

    # list
    fd.write('\n/**\n')
    fd.write(' * @brief Get next value from list of ' + id + '.\n')
    fd.write(' * @param instance of list.\n')
    fd.write(' * @param iterator.\n')
    if needs_found:
        fd.write(' * @param found is place to return whether value is found.\n')
    fd.write(' * @param source_z file:line.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' * @return (' + return_type + ') or NULL.\n')
    fd.write(' * \n')
    fd.write(' * Set the iterator to NULL before the first call and anytime\n')
    fd.write(' * you want to start from the first value again.\n')
    fd.write(' */\n')
    fd.write('#define afw_list_of_' + id + '_get_next( \\\n')
    fd.write('    list, iterator, ' + needs_found_param + 'xctx) \\\n')
    fd.write('    afw_list_of_' + id + '_get_next_source( \\\n')
    fd.write('    list, iterator, ' + needs_found_param + 'AFW__FILE_LINE__, xctx)\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Get next value from list of ' + id + '.\n')
    fd.write(' * @param instance of list.\n')
    fd.write(' * @param iterator.\n')
    if needs_found:
        fd.write(' * @param found is place to return whether value is found.\n')
    fd.write(' * @param source_z file:line.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' * @return (' + return_type + ') or NULL.\n')
    fd.write(' * \n')
    fd.write(' * Set the iterator to NULL before the first call and anytime\n')
    fd.write(' * you want to start from the first value again.\n')
    fd.write(' */\n')
    fd.write(declare + '(' + return_type + ')\n')
    fd.write(prefix + 'list_of_' + id + '_get_next_source(\n')
    fd.write('    const afw_list_t *instance,\n')
    fd.write('    const afw_iterator_t * *iterator,\n')
    if needs_found:
        fd.write('    afw_boolean_t *found,\n')
    fd.write('    const afw_utf8_z_t *source_z,\n')
    fd.write('    afw_xctx_t *xctx);\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Add value from list of ' + id + '.\n')
    fd.write(' * @param instance of list.\n')
    fd.write(' * @param value to add.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' */\n')
    fd.write(declare + '(void)\n')
    fd.write(prefix + 'list_of_' + id + '_add(\n')
    fd.write('    const afw_list_t *instance,\n')
    fd.write('    ' + parameter_ctype + 'value,\n')
    fd.write('    afw_xctx_t *xctx);\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Remove value from list of ' + id + '.\n')
    fd.write(' * @param instance of list.\n')
    fd.write(' * @param value to remove.\n')
    fd.write(' * @param xctx of caller.\n')
    fd.write(' */\n')
    fd.write(declare + '(void)\n')
    fd.write(prefix + 'list_of_' + id + '_remove(\n')
    fd.write('    const afw_list_t *instance,\n')
    fd.write('    ' + parameter_ctype + 'value,\n')
    fd.write('    afw_xctx_t *xctx);\n')

    # Data type direct extern.
    fd.write('\n/**\n')
    fd.write(' * @brief extern for data type ' + id + ' struct.\n')
    fd.write(' *\n')
    fd.write(' * This should only be referenced in the linkage unit the extern is\n')
    fd.write(' * defined in.  Use afw_data_type_' + id + ' when not referencing in\n')
    fd.write(' * a static.\n')
    fd.write(' */\n')
    fd.write('AFW_DECLARE_INTERNAL_CONST_DATA(afw_data_type_t)\n')
    fd.write('afw_data_type_' + id + '_direct;\n')

    # Data type inf extern.
    fd.write('\n/**\n')
    fd.write(' * @brief extern for data type ' + id + ' inf.\n')
    fd.write(' *\n')
    fd.write(' * This should only be referenced in the linkage unit the extern is\n')
    fd.write(' * defined in.\n')
    fd.write(' *\n')
    fd.write(' * The implementation of the data type must define this.  It is\n')
    fd.write(' * referenced by the generated data type instance.\n')
    fd.write(' */\n')
    fd.write('AFW_DECLARE_INTERNAL_CONST_DATA(afw_data_type_inf_t)\n')
    fd.write('afw_data_type_' + id + '_inf;\n')


def write_c_section(fd, prefix, obj):
    define = prefix.upper() + 'DEFINE'
    define_data =  prefix.upper() + 'DEFINE_CONST_DATA'
    id = obj['_meta_']['objectId']

    # internal type
    ctype = obj['cType']

    # parameter is always a const pointer
    parameter_ctype = ctype
    if not ctype.startswith('const'): parameter_ctype='const ' + parameter_ctype
    if not ctype.endswith('*'): parameter_ctype= parameter_ctype + ' *'

    # return type is influenced by directReturn
    direct_return = obj.get('directReturn', False) == True
    if direct_return: return_type = ctype
    else: return_type = parameter_ctype

    # needs found
    needs_found = direct_return and ctype.find('*') == -1
    needs_found_param = ''
    if needs_found:
        needs_found_param = 'found, '
    amp_if_needed = ''
    if not direct_return: amp_if_needed='&'

    fd.write('#include "afw_value_internal.h"\n')

        
    fd.write('\n')
    fd.write('/* Declaration for method optional_release for managed value. */\n')
    fd.write('AFW_DECLARE_STATIC(void)\n')
    fd.write('impl_afw_value_managed_optional_release(\n')
    fd.write('    const afw_value_t *instance,\n')
    fd.write('    afw_xctx_t *xctx);\n')
    fd.write('\n')
    
    fd.write('\n')
    fd.write('/* Declaration for method get_reference for unmanaged value. */\n')
    fd.write('AFW_DECLARE_STATIC(const afw_value_t *)\n')
    fd.write('impl_afw_value_unmanaged_get_reference(\n')
    fd.write('    const afw_value_t *instance,\n')
    fd.write('    const afw_pool_t *p,\n')
    fd.write('    afw_xctx_t *xctx);\n')
    fd.write('\n')
    
    fd.write('\n')
    fd.write('/* Declaration for method get_reference for managed value. */\n')
    fd.write('AFW_DECLARE_STATIC(const afw_value_t *)\n')
    fd.write('impl_afw_value_managed_get_reference(\n')
    fd.write('    const afw_value_t *instance,\n')
    fd.write('    const afw_pool_t *p,\n')
    fd.write('    afw_xctx_t *xctx);\n')
    fd.write('\n')

    fd.write('\n')
    fd.write('/* Declaration for method get_reference for permanent value. */\n')
    fd.write('AFW_DECLARE_STATIC(const afw_value_t *)\n')
    fd.write('impl_afw_value_permanent_get_reference(\n')
    fd.write('    const afw_value_t *instance,\n')
    fd.write('    const afw_pool_t *p,\n')
    fd.write('    afw_xctx_t *xctx);\n')
    fd.write('\n')

    x = obj.get("afw_value_get_evaluated_meta", "afw_value_internal_get_evaluated_meta_default")
    fd.write('\n#define impl_afw_value_get_evaluated_meta \\\n')
    fd.write('    ' + x +'\n')

    x = obj.get("afw_value_get_evaluated_metas", "afw_value_internal_get_evaluated_metas_default")
    fd.write('\n#define impl_afw_value_get_evaluated_metas \\\n')
    fd.write('    ' + x +'\n')

    fd.write('\n/* All infs have a NULL optional evaluate. */\n')
    fd.write('#define impl_afw_value_optional_evaluate NULL\n')

    fd.write('\n/* Inf specific is always data type. */\n')
    fd.write('#define AFW_IMPLEMENTATION_SPECIFIC (const void *)&afw_data_type_' + id + '_direct\n')

    fd.write('\n/* Define inf variables for data_type and is_evaluated_of_data_type. */\n')
    fd.write('#define AFW_IMPLEMENTATION_INF_VARIABLES \\\n')
    fd.write('    (const void *)&afw_data_type_' + id + '_direct, \\\n')
    fd.write('    (const void *)&afw_data_type_' + id + '_direct\n')

    fd.write('\n/* Declares and rti/inf defines for interface afw_value */\n')
    fd.write('/* This is the inf for unmanaged ' + id + ' values. For this one */\n')
    fd.write('/* optional_release is NULL and get_reference returns new reference. */\n')
    fd.write('#define AFW_IMPLEMENTATION_ID "' + id + '"\n')
    fd.write('#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA\n')
    fd.write('#define AFW_IMPLEMENTATION_INF_LABEL afw_value_evaluated_' + id + '_inf\n')
    fd.write('#define impl_afw_value_optional_release NULL\n')
    fd.write('#define impl_afw_value_get_reference impl_afw_value_unmanaged_get_reference\n')
    fd.write('#include "afw_value_impl_declares.h"\n')
    fd.write('#undef AFW_IMPLEMENTATION_ID\n')
    fd.write('#undef AFW_IMPLEMENTATION_INF_LABEL\n')
    fd.write('#undef impl_afw_value_optional_release\n')
    fd.write('#undef impl_afw_value_get_reference\n')

    fd.write('\n/* Declares and rti/inf defines for interface afw_value */\n')
    fd.write('/* This is the inf for managed ' + id + ' values. For this one */\n')
    fd.write('/* optional_release releases value and get_reference returns new reference. */\n')
    fd.write('#define AFW_IMPLEMENTATION_ID "managed_' + id + '"\n')
    fd.write('#define AFW_IMPLEMENTATION_INF_LABEL afw_value_managed_' + id + '_inf\n')
    fd.write('#define impl_afw_value_optional_release impl_afw_value_managed_optional_release\n')
    fd.write('#define impl_afw_value_get_reference impl_afw_value_managed_get_reference\n')
    fd.write('#define AFW_VALUE_INF_ONLY 1\n')
    fd.write('#include "afw_value_impl_declares.h"\n')
    fd.write('#undef AFW_IMPLEMENTATION_ID\n')
    fd.write('#undef AFW_IMPLEMENTATION_INF_LABEL\n')
    fd.write('#undef impl_afw_value_optional_release\n')
    fd.write('#undef impl_afw_value_get_reference\n')
    fd.write('#undef AFW_VALUE_INF_ONLY\n')

    fd.write('\n/* Declares and rti/inf defines for interface afw_value */\n')
    fd.write('/* This is the inf for permanent ' + id + ' values. For this one */\n')
    fd.write('/* optional_release is NULL and get_reference returns instance asis. */\n')
    fd.write('#define AFW_IMPLEMENTATION_ID "permanent_' + id + '"\n')
    fd.write('#define AFW_IMPLEMENTATION_INF_LABEL afw_value_permanent_' + id + '_inf\n')
    fd.write('#define impl_afw_value_optional_release NULL\n')
    fd.write('#define impl_afw_value_get_reference impl_afw_value_permanent_get_reference\n')
    fd.write('#define AFW_VALUE_INF_ONLY 1\n')
    fd.write('#include "afw_value_impl_declares.h"\n')
    fd.write('#undef AFW_IMPLEMENTATION_ID\n')
    fd.write('#undef AFW_IMPLEMENTATION_INF_LABEL\n')
    fd.write('#undef impl_afw_value_optional_release\n')
    fd.write('#undef impl_afw_value_get_reference\n')
    fd.write('#undef AFW_VALUE_INF_ONLY\n')

    fd.write('\nstatic const afw_value_string_t\n')
    fd.write('impl_data_type_' + id + '_id_value = {\n')
    fd.write('    &afw_value_evaluated_string_inf,\n')
    fd.write('    AFW_UTF8_LITERAL(\"' + id + '\")\n')
    fd.write('};\n')
   
    fd.write('\n' + define_data + '(afw_value_t *)\n')
    fd.write('afw_data_type_' + id + '_id_value =\n')
    fd.write('{\n    (const afw_value_t *)&impl_data_type_' + id + '_id_value\n};\n')

    fd.write('\nstatic const afw_utf8_t\n')
    fd.write('impl_data_type_path_' + id + ' =\n')
    fd.write('    AFW_UTF8_LITERAL("' + '/afw/_AdaptiveDataType_/' +  id + '");\n')

    # Data type object
    fd.write('\n/* Data type ' + id + ' object. */\n')
    fd.write('static const afw_runtime_object_indirect_t\n')
    fd.write('impl_data_type_object_' + id + ' = {\n')
    fd.write('    {\n')
    fd.write('        &afw_runtime_inf__AdaptiveDataType_,\n')
    fd.write('        NULL,\n')
    fd.write('        {\n')
    fd.write('            NULL,\n')
    fd.write('            NULL,\n')
    fd.write('            &' + prefix + 's_' + id + ',\n')
    fd.write('            &' + prefix + 's__AdaptiveDataType_,\n')
    fd.write('            &impl_data_type_path_' + id + '\n')
    fd.write('        }\n')
    fd.write('    },\n')
    fd.write('    (void *)&' + prefix + 'data_type_' + id + '_direct\n')
    fd.write('};\n')

    # Declare for empty list of this data type
    fd.write('\n/* Value for empty list of ' + id + '. */\n')
    fd.write('AFW_DEFINE_INTERNAL_CONST_DATA(afw_list_wrapper_for_array_self_t)\n')
    fd.write('impl_empty_list_of_' + id + ';\n')
    
    # Declare for empty list value of this data type
    fd.write('\n/* Value for empty list of ' + id + '. */\n')
    fd.write('AFW_DEFINE_INTERNAL_CONST_DATA(afw_value_list_t)\n')
    fd.write('impl_value_empty_list_of_' + id + ';\n')
    
    # Data type
    fd.write('\n/* Data type ' + id + ' instance. */\n')
    fd.write('AFW_DEFINE_INTERNAL_CONST_DATA(afw_data_type_t)\n')
    fd.write('afw_data_type_' + id + '_direct = {\n')
    fd.write('    &' + prefix + 'data_type_' + id + '_inf,\n')

    # object
    fd.write('    (const afw_object_t *)&impl_data_type_object_' + id + ',\n')

    # data_type_id
    fd.write('    AFW_UTF8_LITERAL("' + id + '"),\n')

    # brief
    if obj.get('brief') is not None:
        fd.write('    AFW_UTF8_LITERAL(' + c.make_quoted(obj.get('brief'))  + '),\n')
    else:
        fd.write('    AFW_UTF8_LITERAL(""),\n')

    # description
    if obj.get('description') is not None:
        fd.write('    AFW_UTF8_LITERAL(' + c.make_quoted(obj.get('description'))  + '),\n')
    else:
        fd.write('    AFW_UTF8_LITERAL(""),\n')

    # data_type_parameter_type
    if obj.get('dataTypeParameterType') is not None:
        fd.write('    AFW_UTF8_LITERAL(' + c.make_quoted(obj.get('dataTypeParameterType'))  + '),\n')
    else:
        fd.write('    AFW_UTF8_LITERAL(""),\n')

    # data_type_number
    fd.write('    afw_data_type_number_' + id + ',\n')

    # ldapOid
    fd.write('    AFW_UTF8_LITERAL("' + obj.get('ldapOid', '') + '"),\n')

    # jsonPrimitive
    fd.write('    AFW_UTF8_LITERAL("' + obj.get('jsonPrimitive', '') + '"),\n')

    # jsonSchemaStringFormat
    fd.write('    AFW_UTF8_LITERAL("' + obj.get('jsonSchemaStringFormat', '') + '"),\n')

    # cType
    fd.write('    AFW_UTF8_LITERAL("' + obj.get('cType', '') + '"),\n')

    # c_type_size
    fd.write('    sizeof(' + obj.get('cType', 'void *') + '),\n')

    # empty_list
    fd.write('    (const afw_list_t *)&impl_empty_list_of_' + id +',\n')

    # empty_list_value
    fd.write('    (const afw_value_t *)&impl_value_empty_list_of_' + id +',\n')

    # evaluated_value_inf
    fd.write('    &afw_value_evaluated_' + id + '_inf,\n')

    # compile_type
    if obj.get('compileType'):
        fd.write('    afw_compile_type_' + obj.get('compileType') + ',\n')
    else:
        fd.write('    afw_compile_type_error,\n')

    # json_implies_data_type
    if obj.get('jsonImpliesDataType', False) == True:
        fd.write('    true,\n')
    else:
        fd.write('    false,\n')

    # evaluated
    if obj.get('evaluated', False) == True:
        fd.write('    true,\n')
    else:
        fd.write('    false,\n')

    # directReturn
    if obj.get('directReturn', False) == True:
        fd.write('    true,\n')
    else:
        fd.write('    false,\n')

    # relationalCompares
    if obj.get('relationalCompares', False) == True:
        fd.write('    true,\n')
    else:
        fd.write('    false,\n')

    # scalar
    if obj.get('scalar', False) == True:
        fd.write('    true\n')
    else:
        fd.write('    false\n')

    fd.write('};\n')

    # Define for empty list of this data type
    fd.write('\n/* Value for empty list of ' + id + '. */\n')
    fd.write('AFW_DEFINE_INTERNAL_CONST_DATA(afw_list_wrapper_for_array_self_t)\n')
    fd.write('impl_empty_list_of_' + id + ' = {\n')
    fd.write('    &afw_list_wrapper_for_array_inf,\n')
    fd.write('    &afw_data_type_' + id + '_direct,\n')
    fd.write('    0\n')
    fd.write('};\n')
    
    # Define for empty list of this data type
    fd.write('\n/* Value for empty list of ' + id + '. */\n')
    fd.write('AFW_DEFINE_INTERNAL_CONST_DATA(afw_value_list_t)\n')
    fd.write('impl_value_empty_list_of_' + id + ' = {\n')
    fd.write('    &afw_value_permanent_list_inf,\n')
    fd.write('    (const afw_list_t *)&impl_empty_list_of_' + id + '\n')
    fd.write('};\n')
    
    fd.write('\n/* Data type struct for ' + id + '. */\n')
    fd.write(define_data + '(afw_data_type_t *)\n')
    fd.write('afw_data_type_' + id + ' =\n    &afw_data_type_' + id + '_direct;\n')


    fd.write('\n/* Set property function for data type ' + id + ' values. */\n')
    fd.write(define + '(void)\n')
    fd.write('afw_object_set_property_as_' + id + '(\n')
    fd.write('    const afw_object_t *object,\n')
    fd.write('    const afw_utf8_t *property_name,\n')
    fd.write('    ' + return_type + ' internal,\n')
    fd.write('    afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    const afw_value_t *v;\n')
    fd.write('\n')
    fd.write('    if (!object->p) {\n')
    fd.write('        AFW_THROW_ERROR_Z(general,\n')
    fd.write('            "Object must have a pool",\n')
    fd.write('            xctx);\n')
    fd.write('    }\n')
    fd.write('\n')
    fd.write('    v = afw_value_create_' + id + '(internal, object->p, xctx);\n')
    fd.write('    afw_object_set_property(object, property_name, v, xctx);\n')
    fd.write('}\n')

    fd.write('\n/* Typesafe cast of data type ' + id + '. */\n')
    fd.write(define + '(' + return_type + ')\n')
    fd.write('afw_value_as_' + id + '(const afw_value_t *value, afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    value = afw_value_evaluate(value, xctx->p, xctx);\n')
    fd.write('    if (!AFW_VALUE_IS_DATA_TYPE(value, ' + id + '))\n')
    fd.write('    {\n')
    fd.write('        const afw_utf8_t *data_type_id;\n')
    fd.write('\n')
    fd.write('        if (!value) {\n')
    fd.write('            AFW_THROW_ERROR_Z(general,\n')
    fd.write('                "Typesafe error: expecting ' + id + ' but "\n')
    fd.write('                "encountered <undefined>",\n')
    fd.write('                xctx);\n')
    fd.write('        }\n')
    fd.write('\n')
    fd.write('        data_type_id = afw_value_get_quick_data_type_id(value);\n')
    fd.write('        AFW_THROW_ERROR_FZ(general, xctx,\n')
    fd.write('            "Typesafe error: expecting ' + id + ' but "\n')
    fd.write('            "encountered %" AFW_UTF8_FMT,\n')
    fd.write('            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));\n')
    fd.write('    }\n')
    fd.write('    return ' + amp_if_needed + '(((const afw_value_' + id + '_t *)value)->internal);\n')
    fd.write('}\n')

    fd.write('\n/* Allocate function for unmanaged data type ' + id + ' values. */\n')
    fd.write(define + '(afw_value_' + id + '_t *)\n')
    fd.write('afw_value_allocate_' + id + '(const afw_pool_t *p, afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    afw_value_' + id + '_t *result;\n')
    fd.write('\n')
    fd.write('    result = afw_pool_calloc(p, sizeof(afw_value_' + id + '_t),\n')
    fd.write('        xctx);\n')
    fd.write('    result->inf = &afw_value_evaluated_' + id + '_inf;\n')
    fd.write('    return result;\n')
    fd.write('}\n')

    fd.write('\n/* Create function for unmanaged data type ' + id + ' value. */\n')
    fd.write(define + '(const afw_value_t *)\n')
    fd.write('afw_value_create_' + id + '(' + return_type + ' internal,\n')
    fd.write('    const afw_pool_t *p, afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    afw_value_' + id + '_t *v;\n')
    fd.write('\n')
    fd.write('    v = afw_value_allocate_' + id + '(p, xctx);\n')
    if direct_return == True:
        fd.write('    v->internal = internal;\n')
    else:
        fd.write('    if (internal) {\n')
        fd.write('        memcpy(&v->internal, internal, sizeof(' + ctype + '));\n')
        fd.write('    }\n')
    fd.write('    return (const afw_value_t *)v;\n')
    fd.write('}\n')

    fd.write('\n/* Allocate function for managed data type ' + id + ' values. */\n')
    fd.write(define + '(afw_value_' + id + '_t *)\n')
    fd.write('afw_value_allocate_managed_' + id + '(const afw_pool_t *p, afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    afw_value_' + id + '_t *result;\n')
    fd.write('\n')
    fd.write('    result = afw_pool_calloc(p, sizeof(afw_value_' + id + '_t),\n')
    fd.write('        xctx);\n')
    fd.write('    result->inf = &afw_value_managed_' + id + '_inf;\n')
    fd.write('    return result;\n')
    fd.write('}\n')

    fd.write('\n/* Create function for data type ' + id + ' value. */\n')
    fd.write(define + '(const afw_value_t *)\n')
    fd.write('afw_value_create_managed_' + id + '(' + return_type + ' internal,\n')
    fd.write('    const afw_pool_t *p, afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    afw_value_' + id + '_t *v;\n')
    fd.write('\n')
    fd.write('    v = afw_value_allocate_managed_' + id + '(p, xctx);\n')
    if direct_return == True:
        fd.write('    v->internal = internal;\n')
    else:
        fd.write('    if (internal) {\n')
        fd.write('        memcpy(&v->internal, internal, sizeof(' + ctype + '));\n')
        fd.write('    }\n')
    fd.write('    return (const afw_value_t *)v;\n')
    fd.write('}\n')

    fd.write('\n/* Allocate function for permanent data type ' + id + ' values. */\n')
    fd.write(define + '(afw_value_' + id + '_t *)\n')
    fd.write('afw_value_allocate_permanent_' + id + '(const afw_pool_t *p, afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    afw_value_' + id + '_t *result;\n')
    fd.write('\n')
    fd.write('    result = afw_pool_calloc(p, sizeof(afw_value_' + id + '_t),\n')
    fd.write('        xctx);\n')
    fd.write('    result->inf = &afw_value_permanent_' + id + '_inf;\n')
    fd.write('    return result;\n')
    fd.write('}\n')

    fd.write('\n/* Create function for data type ' + id + ' value. */\n')
    fd.write(define + '(const afw_value_t *)\n')
    fd.write('afw_value_create_permanent_' + id + '(' + return_type + ' internal,\n')
    fd.write('    const afw_pool_t *p, afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    afw_value_' + id + '_t *v;\n')
    fd.write('\n')
    fd.write('    v = afw_value_allocate_permanent_' + id + '(p, xctx);\n')
    if direct_return == True:
        fd.write('    v->internal = internal;\n')
    else:
        fd.write('    if (internal) {\n')
        fd.write('        memcpy(&v->internal, internal, sizeof(' + ctype + '));\n')
        fd.write('    }\n')
    fd.write('    return (const afw_value_t *)v;\n')
    fd.write('}\n')

    fd.write('\n/* Convert data type ' + id + ' string to ' + ctype + ' *. */\n')
    fd.write(define + '(void)\nafw_data_type_' + id + '_to_internal(' + ctype + ' *to_internal,\n')
    fd.write('    const afw_utf8_t *from_utf8, const afw_pool_t *p, afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    afw_data_type_utf8_to_internal(\n')
    fd.write('        &afw_data_type_' + id + '_direct,\n')
    fd.write('        (void *)to_internal, from_utf8, p, xctx);\n')
    fd.write('}\n')

    fd.write('\n/*  Convert data type ' + id + ' internal representation to utf-8. */\n')
    fd.write(define + '(const afw_utf8_t *)\nafw_data_type_' + id + '_to_utf8(' + return_type + ' internal,\n')
    fd.write('    const afw_pool_t *p, afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    return afw_data_type_internal_to_utf8(\n')
    fd.write('        &afw_data_type_' + id + '_direct,\n')
    if direct_return == True:
        fd.write('        &internal, p, xctx);\n')
    else:
        fd.write('        internal, p, xctx);\n')

    fd.write('}\n')

    if needs_found:
        fd.write('\n/* Get property function for data type ' + id + ' values. */\n')
        fd.write(define + '(' + return_type + ')\n')
        fd.write('afw_object_get_property_as_' + id + '_source(\n')
        fd.write('    const afw_object_t *object,\n')
        fd.write('    const afw_utf8_t *property_name,\n')
        fd.write('    afw_boolean_t *found,\n')
        fd.write('    const afw_utf8_z_t *source_z,\n')
        fd.write('    const afw_pool_t *p,\n')
        fd.write('    afw_xctx_t *xctx)\n')
        fd.write('{\n')
        fd.write('    const afw_value_t *value;\n')
        fd.write('\n')
        fd.write('    *found = false;\n')
        fd.write('    value = afw_object_get_property(object, property_name, xctx);\n')
        fd.write('    if (!value) return 0;\n')
        fd.write('\n')
        fd.write('    value = afw_value_evaluate(value, p, xctx);\n')
        fd.write('    *found = true;\n')
        fd.write('    if (!AFW_VALUE_IS_DATA_TYPE(value, ' + id + '))\n')
        fd.write('    {\n')
        fd.write('        const afw_utf8_t *data_type_id;\n')
        fd.write('\n')
        fd.write('        data_type_id = afw_value_get_quick_data_type_id(value);\n')
        fd.write('        afw_error_set_fz(afw_error_code_general, source_z, xctx,\n')
        fd.write('            "Typesafe error: expecting ' + id + ' but "\n')
        fd.write('            "encountered %" AFW_UTF8_FMT,\n')
        fd.write('            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));\n')
        fd.write('        longjmp(((xctx)->current_try->throw_jmp_buf), afw_error_code_general);\n')
        fd.write('    }\n')
        fd.write('    return ' + amp_if_needed + '(((const afw_value_' + id + '_t *)value)->internal);\n')
        fd.write('}\n')

    else:
        fd.write('\n/* Get property function for data type ' + id + ' values. */\n')
        fd.write(define + '(' + return_type + ')\n')
        fd.write('afw_object_get_property_as_' + id + '_source(\n')
        fd.write('    const afw_object_t *object,\n')
        fd.write('    const afw_utf8_t *property_name,\n')
        fd.write('    const afw_utf8_z_t *source_z,\n')
        fd.write('    const afw_pool_t *p,\n')
        fd.write('    afw_xctx_t *xctx)\n')
        fd.write('{\n')
        fd.write('    const afw_value_t *value;\n')
        fd.write('\n')
        fd.write('    value = afw_object_get_property(object, property_name, xctx);\n')
        fd.write('    if (!value) return NULL;\n')
        fd.write('\n')
        fd.write('    value = afw_value_evaluate(value, p, xctx);\n')
        fd.write('    if (!AFW_VALUE_IS_DATA_TYPE(value, ' + id + '))\n')
        fd.write('    {\n')
        fd.write('        const afw_utf8_t *data_type_id;\n')
        fd.write('\n')
        fd.write('        data_type_id = afw_value_get_quick_data_type_id(value);\n')
        fd.write('        afw_error_set_fz(afw_error_code_general, source_z, xctx,\n')
        fd.write('            "Typesafe error: expecting ' + id + ' but "\n')
        fd.write('            "encountered %" AFW_UTF8_FMT,\n')
        fd.write('            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));\n')
        fd.write('        longjmp(((xctx)->current_try->throw_jmp_buf), afw_error_code_general);\n')
        fd.write('    }\n')
        fd.write('    return ' + amp_if_needed + '(((const afw_value_' + id + '_t *)value)->internal);\n')
        fd.write('}\n')

    if needs_found:
        fd.write('\n/* Get next property function for data type ' + id + ' values. */\n')
        fd.write(define + '(' + return_type + ')\n')
        fd.write('afw_object_get_next_property_as_' + id + '_source(\n')
        fd.write('    const afw_object_t *object,\n')
        fd.write('    const afw_iterator_t * *iterator,\n')
        fd.write('    const afw_utf8_t * *property_name,\n')
        fd.write('    afw_boolean_t *found,\n')
        fd.write('    const afw_utf8_z_t *source_z,\n')
        fd.write('    const afw_pool_t *p,\n')
        fd.write('    afw_xctx_t *xctx)\n')
        fd.write('{\n')
        fd.write('    const afw_value_t *value;\n')
        fd.write('\n')
        fd.write('    *found = false;\n')
        fd.write('    value = afw_object_get_next_property(object, iterator, property_name, xctx);\n')
        fd.write('    if (!value) return 0;\n')
        fd.write('\n')
        fd.write('    value = afw_value_evaluate(value, p, xctx);\n')
        fd.write('    *found = true;\n')
        fd.write('    if (!AFW_VALUE_IS_DATA_TYPE(value, ' + id + '))\n')
        fd.write('    {\n')
        fd.write('        const afw_utf8_t *data_type_id;\n')
        fd.write('\n')
        fd.write('        data_type_id = afw_value_get_quick_data_type_id(value);\n')
        fd.write('        afw_error_set_fz(afw_error_code_general, source_z, xctx,\n')
        fd.write('            "Typesafe error: expecting ' + id + ' but "\n')
        fd.write('            "encountered %" AFW_UTF8_FMT,\n')
        fd.write('            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));\n')
        fd.write('        longjmp(((xctx)->current_try->throw_jmp_buf), afw_error_code_general);\n')
        fd.write('    }\n')
        fd.write('    return ' + amp_if_needed + '(((const afw_value_' + id + '_t *)value)->internal);\n')
        fd.write('}\n')

    else:
        fd.write('\n/* Get next property function for data type ' + id + ' values. */\n')
        fd.write(define + '(' + return_type + ')\n')
        fd.write('afw_object_get_next_property_as_' + id + '_source(\n')
        fd.write('    const afw_object_t *object,\n')
        fd.write('    const afw_iterator_t * *iterator,\n')
        fd.write('    const afw_utf8_t * *property_name,\n')
        fd.write('    const afw_utf8_z_t *source_z,\n')
        fd.write('    const afw_pool_t *p,\n')
        fd.write('    afw_xctx_t *xctx)\n')
        fd.write('{\n')
        fd.write('    const afw_value_t *value;\n')
        fd.write('\n')
        fd.write('    value = afw_object_get_next_property(object, iterator, property_name, xctx);\n')
        fd.write('    if (!value) return NULL;\n')
        fd.write('\n')
        fd.write('    value = afw_value_evaluate(value, p, xctx);\n')
        fd.write('    if (!AFW_VALUE_IS_DATA_TYPE(value, ' + id + '))\n')
        fd.write('    {\n')
        fd.write('        const afw_utf8_t *data_type_id;\n')
        fd.write('\n')
        fd.write('        data_type_id = afw_value_get_quick_data_type_id(value);\n')
        fd.write('        afw_error_set_fz(afw_error_code_general, source_z, xctx,\n')
        fd.write('            "Typesafe error: expecting ' + id + ' but "\n')
        fd.write('            "encountered %" AFW_UTF8_FMT,\n')
        fd.write('            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));\n')
        fd.write('        longjmp(((xctx)->current_try->throw_jmp_buf), afw_error_code_general);\n')
        fd.write('    }\n')
        fd.write('    return ' + amp_if_needed + '(((const afw_value_' + id + '_t *)value)->internal);\n')
        fd.write('}\n')

    fd.write('\n')
    fd.write('/* Implementation of method optional_release for managed value. */\n')
    fd.write('AFW_DECLARE_STATIC(void)\n')
    fd.write('impl_afw_value_managed_optional_release(\n')
    fd.write('    const afw_value_t *instance,\n')
    fd.write('    afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    /** @todo this needs to release reference of value or free it. */\n')
    fd.write('}\n')
    
    fd.write('\n')
    fd.write('/* Implementation of method get_reference for  unmanaged value. */\n')
    fd.write('AFW_DECLARE_STATIC(const afw_value_t *)\n')
    fd.write('impl_afw_value_unmanaged_get_reference(\n')
    fd.write('    const afw_value_t *instance,\n')
    fd.write('    const afw_pool_t *p,\n')
    fd.write('    afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    /** @todo this needs to addref or return a copy. */\n')
    fd.write('    return instance;\n')
    fd.write('}\n')
    fd.write('\n')
    
    fd.write('\n')
    fd.write('/* Implementation of method get_reference for managed value. */\n')
    fd.write('AFW_DECLARE_STATIC(const afw_value_t *)\n')
    fd.write('impl_afw_value_managed_get_reference(\n')
    fd.write('    const afw_value_t *instance,\n')
    fd.write('    const afw_pool_t *p,\n')
    fd.write('    afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    /* For unmanaged value, just return the instance passed. */\n')
    fd.write('    return instance;\n')
    fd.write('}\n')
    fd.write('\n')

    fd.write('\n')
    fd.write('/* Implementation of method get_reference for permanent value. */\n')
    fd.write('AFW_DECLARE_STATIC(const afw_value_t *)\n')
    fd.write('impl_afw_value_permanent_get_reference(\n')
    fd.write('    const afw_value_t *instance,\n')
    fd.write('    const afw_pool_t *p,\n')
    fd.write('    afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    /* For unmanaged value, just return the instance passed. */\n')
    fd.write('    return instance;\n')
    fd.write('}\n')
    fd.write('\n')

    fd.write('/*\n')
    fd.write(' * Implementation of method get_data_type for interface afw_value.\n')
    fd.write(' */\n')
    fd.write('const afw_data_type_t *\n')
    fd.write('impl_afw_value_get_data_type(\n')
    fd.write('    const afw_value_t *instance,\n')
    fd.write('    afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    return afw_data_type_' + id + ';\n')
    fd.write('}\n')
    fd.write('\n')
    fd.write('/*\n')
    fd.write(' * Implementation of method compiler_listing for interface afw_value.\n')
    fd.write(' */\n')
    fd.write('void\n')
    fd.write('impl_afw_value_produce_compiler_listing(\n')
    fd.write('    const afw_value_t *instance,\n')
    fd.write('    const afw_writer_t *writer,\n')
    fd.write('    afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    afw_data_type_value_compiler_listing(\n')
    fd.write('        afw_data_type_' + id + ',\n')
    fd.write('        writer, instance, xctx);\n')
    fd.write('}\n')
    fd.write('\n')
    fd.write('/*\n')
    fd.write(' * Implementation of method decompile for interface afw_value.\n')
    fd.write(' */\n')
    fd.write('void\n')
    fd.write('impl_afw_value_decompile(\n')
    fd.write('    const afw_value_t *instance,\n')
    fd.write('    const afw_writer_t *writer,\n')
    fd.write('    afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    afw_data_type_write_as_expression(\n')
    fd.write('        afw_data_type_' + id + ',\n')
    fd.write('        writer,\n')
    fd.write('        (const void *)&(((const afw_value_evaluated_t *)instance)->internal),\n')
    fd.write('        xctx);\n')
    fd.write('}\n')
    fd.write('\n')
    
    fd.write('/*\n')
    fd.write(' * Implementation of method get_info for interface afw_value.\n')
    fd.write(' */\n')
    fd.write('void\n')
    fd.write('impl_afw_value_get_info(\n')
    fd.write('    const afw_value_t *instance,\n')
    fd.write('    afw_value_info_t *info,\n')
    fd.write('    const afw_pool_t *p,\n')
    fd.write('    afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    afw_memory_clear(info);\n')
    fd.write('    info->value_inf_id = &instance->inf->rti.implementation_id;\n')
    fd.write('    info->optimized_value = instance;\n')
    fd.write('    info->optimized_value_data_type = afw_data_type_' + id + ';\n')
    fd.write('}\n')
    fd.write('\n')


    # list

    fd.write('\n/* Get next value from list of ' + id + '. */\n')
    fd.write(define + '(' + return_type + ')\n')
    fd.write(prefix + 'list_of_' + id + '_get_next_source(\n')
    fd.write('    const afw_list_t *instance,\n')
    fd.write('    const afw_iterator_t * *iterator,\n')
    if needs_found:
        fd.write('    afw_boolean_t *found,\n')
    fd.write('    const afw_utf8_z_t *source_z,\n')
    fd.write('    afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    const void *internal;\n')
    fd.write('    const afw_data_type_t *data_type;\n')
    fd.write('\n')
    fd.write('    afw_list_get_next_internal(instance, iterator, &data_type, &internal, xctx);\n')
    if needs_found:
        fd.write('    *found = true;\n')
        fd.write('    if (!internal) {\n')
        fd.write('        *found = false;\n')
        fd.write('        return 0;\n')
        fd.write('    }\n')
    else:
        fd.write('    if (!internal) {\n')
        fd.write('        return NULL;\n')
        fd.write('    }\n')    
    fd.write('    if (data_type != afw_data_type_' + id + ') {\n')
    fd.write('        const afw_utf8_t *data_type_id;\n')
    fd.write('\n')
    fd.write('        data_type_id = &data_type->data_type_id;\n')
    fd.write('        afw_error_set_fz(afw_error_code_general, source_z, xctx,\n')
    fd.write('            "Typesafe error: expecting ' + id + ' but "\n')
    fd.write('            "encountered %" AFW_UTF8_FMT,\n')
    fd.write('            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));\n')
    fd.write('        longjmp(((xctx)->current_try->throw_jmp_buf), afw_error_code_general);\n')
    fd.write('    }\n')
    if direct_return:
        fd.write('    return *(' + ctype + ' *)internal;\n')
    else:
        fd.write('    return (' + return_type + ')internal;\n')
    fd.write('}\n')

    fd.write('\n/* Add value from list of ' + id + ' */\n')
    fd.write(define + '(void)\n')
    fd.write(prefix + 'list_of_' + id + '_add(\n')
    fd.write('    const afw_list_t *instance,\n')
    fd.write('    ' + parameter_ctype + 'value,\n')
    fd.write('    afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    const afw_list_setter_t *setter;\n')
    if ctype.endswith('*'):
        fd.write('    ' + parameter_ctype + 'internal;\n')
    fd.write('\n')
    fd.write('    setter = afw_list_get_setter(instance, xctx);\n')
    fd.write('    if (!setter) {\n')
    fd.write('        AFW_LIST_ERROR_OBJECT_IMMUTABLE;\n')
    fd.write('    }\n')
    fd.write('\n')
    if ctype.endswith('*'):
        fd.write('    internal = value;\n')
        fd.write('    afw_list_setter_add_internal(setter, \n')
        fd.write('        afw_data_type_' + id + ',\n')
        fd.write('        (const void *)&internal, xctx);\n')
    else:
        fd.write('    afw_list_setter_add_internal(setter, \n')
        fd.write('        afw_data_type_' + id + ',\n')
        fd.write('        (const void *)value, xctx);\n')
    fd.write('}\n')

    fd.write('\n/* Remove value from list of ' + id + ' */\n')
    fd.write(define + '(void)\n')
    fd.write(prefix + 'list_of_' + id + '_remove(\n')
    fd.write('    const afw_list_t *instance,\n')
    fd.write('    ' + parameter_ctype + 'value,\n')
    fd.write('    afw_xctx_t *xctx)\n')
    fd.write('{\n')
    if ctype.endswith('*'):
        fd.write('    ' + parameter_ctype + 'internal;\n')
    fd.write('    const afw_list_setter_t *setter;\n')
    fd.write('\n')
    fd.write('    setter = afw_list_get_setter(instance, xctx);\n')
    fd.write('    if (!setter) {\n')
    fd.write('        AFW_LIST_ERROR_OBJECT_IMMUTABLE;\n')
    fd.write('    }\n')
    fd.write('\n')
    if ctype.endswith('*'):
        fd.write('    internal = value;\n')
        fd.write('    afw_list_setter_remove_internal(setter, \n')
        fd.write('        afw_data_type_' + id + ',\n')
        fd.write('        (const void *)&internal, xctx);\n')
    else:
        fd.write('    afw_list_setter_remove_internal(setter, \n')
        fd.write('        afw_data_type_' + id + ',\n')
        fd.write('        (const void *)value, xctx);\n')
    fd.write('}\n')


def generate_h(prefix, obj, id, generated_by, dir, filename, options):
    msg.info('Generating ' + filename)
    with nfc.open(dir+filename, mode='w') as fd:
        c.write_h_prologue(fd, generated_by, 'Adaptive Data Type ' + id , filename)
        fd.write('\n#include "afw_minimal.h"\n')
        fd.write('#include "' + prefix + 'data_type_typedefs.h"\n')
        fd.write('#include "' + prefix + 'declare_helpers.h"\n')
        if options['core']:
            fd.write('\n/**\n')
            fd.write(' * @defgroup afw_c_api_data_type_' + id + ' ' + id + '\n')
            fd.write(' * @ingroup afw_c_api_data_types\n')
            fd.write(' *\n')
            fd.write(' * Adaptive Data Type ' + id + '\n')
            fd.write(' *\n')
            fd.write(' * @{\n')
            fd.write(' */\n')
        c.write_doxygen_file_section(fd, filename, 'Adaptive data type ' + id + ' header.')
        fd.write('\nAFW_BEGIN_DECLARES\n')

        write_h_section(fd, prefix, obj)

        fd.write('\nAFW_END_DECLARES\n')
        if options['core']:
            fd.write('\n/** @} */\n')
        c.write_h_epilogue(fd, filename)


def generate_typedefs_h(prefix, data_type_list, id, generated_by, dir, filename, options):
    with nfc.open(dir+filename, mode='w') as fd:
        c.write_h_prologue(fd, generated_by, 'Adaptive Data Type Typedefs ' + id , filename)
        c.write_doxygen_file_section(fd, filename, 'Adaptive data type typedefs ' + id + ' header.')
        fd.write('\n#include "afw_minimal.h"\n')
        fd.write('#include "' + prefix + 'declare_helpers.h"\n')
        fd.write('\nAFW_BEGIN_DECLARES\n')

        for obj in data_type_list:
            id = obj['_meta_']['objectId']
            if options['core']:
                fd.write('\n/**\n')
                fd.write(' * @addtogroup afw_c_api_data_type_' + id + '\n')
                fd.write(' * @{\n')
                fd.write(' */\n')
            filename = prefix + 'data_type_' + id + '_binding.h'
            write_typedefs_h_section(fd, prefix, obj)
            if options['core']:
                fd.write('\n/** @} */\n')

        fd.write('\nAFW_END_DECLARES\n')
        c.write_h_epilogue(fd, filename)


def generate_c(prefix, obj, id, generated_by, dir, filename):
    msg.info('Generating ' + filename)
    with nfc.open(dir+filename, mode='w') as fd:
        c.write_c_prologue(fd, generated_by, 'Adaptive Framework Core Data Types ')
        c.write_doxygen_file_section(fd, filename, 'Adaptive Framework core data types.')
        fd.write('\n')
        fd.write('#include "afw.h"\n')
        fd.write('#include "afw_runtime_object_maps.h"')
        fd.write('\n')

        write_c_section(fd, prefix, obj)

def generate(generated_by, prefix, data_type_list, generated_dir_path, options):
    declare = prefix.upper() + 'DECLARE'
    define = prefix.upper() + 'DEFINE'

    # Just return if no data types
    if len(data_type_list) == 0: return

    # Make sure generated/ directory structure exists
    os.makedirs(generated_dir_path, exist_ok=True)

    # Generate <prefix>_data_type_<type>.h for each data type
    for obj in data_type_list:
        id = obj['_meta_']['objectId']
        filename = prefix + 'data_type_' + id + '_binding.h'
        generate_h(prefix, obj, id, generated_by, generated_dir_path, filename, options)

    # Generate <prefix>_data_type_typedefs.h
    filename = prefix + 'data_type_typedefs.h'
    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        generate_typedefs_h(prefix, data_type_list, id, generated_by, generated_dir_path, filename, options)

    # Generate <prefix>_data_type_bindings.h
    filename = prefix + 'data_type_bindings.h'
    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_h_prologue(fd, generated_by, 'Adaptive Framework Data Type Bindings', filename)
        c.write_doxygen_file_section(fd, filename, 'Adaptive Framework core data type bindings header.')
        fd.write('\n')

        for obj in data_type_list:
            id = obj['_meta_']['objectId']
            fd.write('#include "' + prefix + 'data_type_' + id + '_binding.h"\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Data type map.\n')
        fd.write(' */\n')
        fd.write('#define ' + prefix.upper() + 'DATA_TYPES_MAP(XX) \\\n')
        for obj in data_type_list:
            id = obj['_meta_']['objectId']
            fd.write('    XX(' + id + ') \\\n')
        fd.write('\n')

        if options['core']: 
            fd.write('\n/**\n')
            fd.write(' * @brief Core data types map.\n')
            fd.write(' */\n')
            fd.write('typedef enum afw_data_type_number_e {\n')
            fd.write('    afw_data_type_number_External,\n')
            fd.write('#define XX(id) afw_data_type_number_ ## id,\n')
            fd.write('    AFW_DATA_TYPES_MAP(XX)\n')
            fd.write('    #undef XX\n')
            fd.write('    afw_data_type_number_max\n')
            fd.write('} afw_data_type_number_t;\n')

       
        fd.write('\n/**\n')
        fd.write(' * @brief Register each ' + prefix + 'data_type_<dataType>.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' */\n')
        fd.write(declare + '(void)\n')
        fd.write(prefix + 'register_data_types(\n')
        fd.write('    afw_xctx_t *xctx);\n')

       
        fd.write('\n/**\n')
        fd.write(' * @brief Register each ' + prefix + '_value_evaluated_<dataType>_inf.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' */\n')
        fd.write(declare + '(void)\n')
        fd.write(prefix + 'register_data_type_infs(\n')
        fd.write('    afw_xctx_t *xctx);\n')

        c.write_h_epilogue(fd, filename)

    # Generate <prefix>data_type_<id>_binding.c
    for obj in data_type_list:
        id = obj['_meta_']['objectId']
        filename = prefix + 'data_type_' + id + '_binding.c'
        generate_c(prefix, obj, id, generated_by, generated_dir_path, filename)

    # Generate <prefix>data_types_bindings.c
    filename = prefix + 'data_type_bindings.c'
    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_c_prologue(fd, generated_by, 'Adaptive Framework Data Type Bindings')
        c.write_doxygen_file_section(fd, filename, 'Adaptive Framework data type bindings.')
        fd.write('\n')
        fd.write('#include "afw.h"\n')
        fd.write('#include "' + prefix + 'data_type_bindings.h"\n')
        fd.write('\n')
        fd.write('static const afw_data_type_t * ' + 'impl_data_type_bindings[] = {\n')
        fd.write('#define XX(id) &' + prefix + 'data_type_ ##id##_direct,\n')
        fd.write(prefix.upper() + 'DATA_TYPES_MAP(XX)\n')
        fd.write('    NULL\n')
        fd.write('};\n')
        fd.write('\n')
        fd.write(define + '(void)\n')
        fd.write(prefix + 'register_data_types(\n')
        fd.write('    afw_xctx_t *xctx)\n')
        fd.write('{\n')
        fd.write('    /* Register data types */\n')
        fd.write('    afw_environment_register_data_types(\n')
        fd.write('        &impl_data_type_bindings[0], xctx);\n')
        fd.write('\n')
        fd.write('}\n')

        fd.write('\n')
        fd.write(define + '(void)\n')
        fd.write(prefix + 'register_data_type_infs(\n')
        fd.write('    afw_xctx_t *xctx)\n')
        fd.write('{\n')

        for obj in data_type_list:
            id = obj['_meta_']['objectId']
            fd.write('\n')
            fd.write('    /* Register value inf id evaluated_' + id + ' */\n')
            fd.write('    afw_environment_register_value_inf(\n')
            fd.write('        &afw_value_evaluated_' + id + '_inf.rti.implementation_id,\n')
            fd.write('        &afw_value_evaluated_' + id + '_inf,\n')
            fd.write('        xctx);\n')
    
        fd.write('\n')
        fd.write('}\n')


