#! /usr/bin/env python3

#other syntax: from urllib.request import Request, urlopen

import os
from _afwdev.common import direct
from _afwdev.common import msg, nfc
from _afwdev.generate import c

def get_data_type(data_type_list, dataType):
    result = None

    for e in data_type_list:
        if e['objectId'] == dataType:
            result = e
            break

    return result


def sort_category_functionLabel_cb(obj):
    category = obj.get('category')
    if category is None:
        category = ''
    return category + '~' + obj.get('functionLabel')


# afw_value_function_parameter_t
def write_parameter(fd, prefix, options, label, p, embedding_object_label, property_name):
    fd.write('\nstatic const afw_value_function_parameter_t\n')
    fd.write(label + ' = {\n')

    # meta (object)
    fd.write('    {\n')
    if options['core']:
        fd.write('        &afw_runtime_inf__AdaptiveFunctionParameter_,\n')
    else:
        fd.write('        NULL,\n')
    fd.write('        NULL,\n')
    fd.write('        {\n')
    fd.write('            NULL,\n')
    fd.write('            NULL,\n')
    fd.write('            NULL,\n')
    fd.write('            &' + prefix + 's__AdaptiveFunctionParameter_,\n')
    fd.write('            NULL,\n')
    fd.write('        }\n')
    fd.write('    },\n')

    # dataType
    dataType = p.get('dataType','')
    data_type = 'NULL'
    if options['core'] and dataType != '':
        data_type = '&afw_data_type_' + dataType + '_direct'
    fd.write('    ' + data_type + ',\n')
    fd.write('    AFW_UTF8_LITERAL("' + dataType + '"),\n')

    # dataTypeParameter
    dataTypeParameter = p.get('dataTypeParameter','')
    fd.write('    AFW_UTF8_LITERAL("' + dataTypeParameter  + '"),\n')

    # name
    name = p.get('name','')
    fd.write('    AFW_UTF8_LITERAL("' + name  + '"),\n')

    # brief
    brief = p.get('brief','') 
    fd.write('    AFW_UTF8_LITERAL(' + c.make_quoted(brief)  + '),\n')

    # description
    description = p.get('description','')
    fd.write('    AFW_UTF8_LITERAL(' + c.make_quoted(description)  + '),\n')
            
    # minArgs
    if p.get('minArgs') is not None:
        fd.write('    ' + str(p.get('minArgs'))  + ',\n')
    else:
        fd.write('    -1,\n')

    # optional
    if p.get('optional', False) == True:
        fd.write('    true,\n')
    else:
        fd.write('    false,\n')

    # canBeUndefined
    if p.get('canBeUndefined', False) == True:
        fd.write('    true,\n')
    else:
        fd.write('    false,\n')

    # polymorphicDataType
    if p.get('polymorphicDataType', False) == True:
        fd.write('    true,\n')
    else:
        fd.write('    false,\n')

    # polymorphicDataTypeParameter
    if p.get('polymorphicDataTypeParameter', False) == True:
        fd.write('    true,\n')
    else:
        fd.write('    false,\n')

    fd.write('};\n')

def function_arg(fd, arg):
    types = []
    line = '  '

    if arg.get('name') is not None:
        line = '  ' + arg.get('name', '') + ' - '

    line += '('
    if arg.get('minArgs') is not None:
        line += str(arg.get('minArgs')) + ' or more '
    elif arg.get('optional', False):
        line += 'optional '
    line += ' or '.join(types)
    if arg.get('polymorphicDataType', False) == True:
        line += '`<Type>`'
    else:
        if arg.get('dataType') is not None:
            line += arg.get('dataType')
        else:
            line += 'any dataType'
    if arg.get('polymorphicDataTypeParameter', False) == True:
        line += ' `<Type>`'
    elif arg.get('dataTypeParameter') is not None:
        line += ' ' + arg.get('dataTypeParameter')
    line +=  ')'

    if arg.get('description') is not None:
        line += ' '
        line += arg.get('description')
        if not line.endswith('.'):
            line += '.'
    c.write_wrapped(fd, 80, ' * ', line, '      ')

def make_Type(p):
    type = ""
    if p.get('dataTypeParameter') is not None:
        type += "(" 
    if p.get('dataType') is not None:
        type += p.get('dataType')
    elif p.get('polymorphicDataType', False):
        type += 'dataType'
    else:
        type += 'any'
    if p.get('dataTypeParameter') is not None:
        type += " " + p.get('dataTypeParameter') + ")"
    return type


def function_comment(fd, obj):
    if obj.get('deprecated') is not None and obj.get('deprecated') == True:
        fd.write(' *\n')
        fd.write(' * This function is deprecated.\n')
    if obj.get('description') is not None:
        fd.write(' *\n')
        c.write_wrapped(fd, 80, ' * ', obj.get('description'))

    if obj.get('pure', False):
        if obj.get('sideEffects') is not None:
            msg.error_exit('sideEffects can not be specified if pure is true.')
        fd.write(' *\n')
        fd.write(' * This function is pure, so it will always return the same result\n')
        fd.write(' * given exactly the same parameters and has no side effects.\n')
    else:
        fd.write(' *\n')
        fd.write(' * This function is not pure, so it may return a different result\n')
        if obj.get('sideEffects') is None:
            fd.write(' * given exactly the same parameters.\n')
        else:
            fd.write(' * given exactly the same parameters and has side effects.\n')

    if obj.get('polymorphic', False) != False:
        fd.write(' *\n')
        fd.write(' * Supported `<dataType>`:\n')
        fd.write(' *\n')
        c.write_wrapped(fd, 80, ' *   ', ', '.join(obj.get('polymorphicDataTypes')) + '.')
     
    # function declaration
    fd.write(' *\n')
    fd.write(' * Declaration:\n')
    fd.write(' *\n * ```\n')
    fd.write(' *   function ')
    fd.write(obj['functionId'])
    if obj.get('polymorphic', False):
        fd.write(" <dataType>")
    fd.write('(\n *   ')
    sep = '    '
    for p in obj.get('parameters'):
        fd.write(sep)
        sep = ',\n *       '
        if p.get('minArgs') is not None:
            i = 1
            n = int(p.get('minArgs'))
            while i <= n:
                fd.write(p.get('name') + '_' + str(i) + ': ' + make_Type(p) + sep)
                i += 1
            fd.write('...')
            if n == 0:
                fd.write(p.get('name') + ': (list of ' + make_Type(p) + ')')
            else:
                fd.write(p.get('name') + '_rest: (list of ' + make_Type(p) + ')')
        else:
            fd.write(p.get('name'))
            if p.get('optional', False):
                fd.write('?')
            fd.write(': ' + make_Type(p))
    fd.write('\n *   ): ')
    fd.write(make_Type(obj.get('returns')))
    fd.write(';\n * ```\n')

    fd.write(' *\n')
    fd.write(' * Parameters:\n')
    fd.write(' *\n')
    for arg in obj.get('parameters'):
        function_arg(fd, arg)
        fd.write(' *\n')
    fd.write(' * Returns:\n')
    fd.write(' *\n')
    function_arg(fd, obj.get('returns'))

    if obj.get('errorsThrown') is not None:
        fd.write(' *\n')
        fd.write(' * Errors thrown:\n')
        fd.write(' *\n')
        for err in obj.get('errorsThrown'):
            c.write_wrapped(fd, 80, ' *   ', err.get('error','') + ' - ' + \
                err.get('reason', ''), '    ')

def sort_useExecuteFunction(obj):
    return obj['useExecuteFunction']

def generate(generated_by, prefix, data_type_list, object_dir_path,
                generated_dir_path, options):

    declare = prefix.upper() + 'DECLARE'
    define = prefix.upper() + 'DEFINE'
    declare_data =  prefix.upper() + 'DECLARE_CONST_DATA'
    string_ref = '&' + prefix + 's_'

    # Make sure generated/ directory structure exists
    os.makedirs(generated_dir_path, exist_ok=True)
    os.makedirs(os.path.dirname(generated_dir_path + 'function_closet/'), exist_ok=True)

    # Get all functions and sort by category/functionLabel
    list = direct.retrieve_objects_direct(object_dir_path + '_AdaptiveFunctionGenerate_/')
    list.sort(key=sort_category_functionLabel_cb)

    # Generate function_binding.h and generate list of categories
    categories = []
    category = None
    dataTypeMethod = []
    useExecuteFunction = []
    filename = prefix + 'function_bindings.h'
    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_h_prologue(fd, generated_by,
            'Adaptive Framework Core Adaptive Function Bindings', filename)
        c.write_doxygen_file_section(fd, filename,
            'Adaptive Framework core adaptive function bindings header.')
        fd.write('\n')
        fd.write('#include "' + prefix + 'declare_helpers.h"\n')


        fd.write('\n/**\n')
        fd.write(' * @addtogroup afw_c_api_public\n')
        fd.write(' * @{\n')
        fd.write(' *\n')
        fd.write(' */\n')
        fd.write('\n')
        fd.write('/**\n')
        fd.write(' * @addtogroup afw_c_api_functions Adaptive functions\n')
        fd.write(' *\n')
        fd.write(' * @{\n')
        fd.write(' */\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Get array of pointers to ' + prefix + 'function bindings.\n')
        fd.write(' * @return pointer to array of function value pointers.\n')
        fd.write(' */\n')
        fd.write(declare + '(const afw_value_function_definition_t **)\n')
        fd.write(prefix + 'function_bindings_get();\n')

        for obj in list:
            id = obj['functionLabel']

            # Add afwCamelCaseFunctionLabel if needed
            if obj.get("afwCamelCaseFunctionLabel") is None or obj.get("camelCaseFunctionLabel") is None:
                filename2 = 'objects/_AdaptiveFunctionGenerate_/' + id + '.json'
                msg.info('Adding afwCamelCaseFunctionLabel and/or camelCaseFunctionLabel to ' + id + '.json')
                with nfc.open(generated_dir_path + filename2, mode='r') as fd2:
                    obj2 = nfc.json_load(fd2)
                if obj.get("afwCamelCaseFunctionLabel") is None:
                    obj["afwCamelCaseFunctionLabel"] = 'afw' + ''.join(part[0].capitalize() + part[1:] for part in obj.get("functionLabel").split('_'))
                    obj2["afwCamelCaseFunctionLabel"] = obj["afwCamelCaseFunctionLabel"]
                if obj.get("camelCaseFunctionLabel") is None:
                    obj["camelCaseFunctionLabel"] = ''.join(part[0].capitalize() + part[1:] for part in obj.get("functionLabel").split('_'))
                    obj["camelCaseFunctionLabel"] = obj["camelCaseFunctionLabel"][0].lower() + obj["camelCaseFunctionLabel"][1:]
                    obj2["camelCaseFunctionLabel"] = obj["camelCaseFunctionLabel"]
                with nfc.open(generated_dir_path + filename2, mode='w') as fd2:
                    nfc.json_dump(obj2, fd2, sort_keys=True, indent=4)

            #
            x = obj.get('category')
            if x is None: x = ''
            if category is None or x != category:
                if category is not None:
                    fd.write('\n/** @} */\n')
                category =x
                categories.append(category)

                fd.write('\n\n/** @addtogroup afw_functions_' + category +
                            ' ' + category + ' functions\n')
                fd.write(' *\n')
                fd.write(' * ' + category + ' adaptive functions.\n')
                fd.write(' *\n')
                fd.write(' * @{\n')
                fd.write(' */\n')

            # Declaration for core function definition.
            if options['core']:
                fd.write('\n/** @brief Function definition ' + obj['functionId'] + ' */\n') 
                fd.write('AFW_DECLARE_INTERNAL_CONST_DATA(afw_value_function_definition_t)\n')
                fd.write('afw_function_definition_' + obj['functionLabel'] + ';\n')

            fd.write('\n/**\n')
            fd.write(' * @brief Adaptive Function `' + obj['functionId'] + '`\n')
            fd.write(' * @param x function execute parameter.\n')
            function_comment(fd, obj)
            if obj.get('useExecuteFunction') is None:
                fd.write(' */\n')
                fd.write('const afw_value_t *\n')
                fd.write(prefix + 'function_execute_' + obj['functionLabel'] + '(\n')
                fd.write('    afw_function_execute_t *x);\n')
            else:
                fd.write(' *\n')
                fd.write(' * ' + 'Implemented by ' + obj.get('useExecuteFunction') + '()\n')
                fd.write(' *\n')
                fd.write(' * __________\n')
                fd.write(' */\n')
                useExecuteFunction.append(obj)

            if obj.get('dataTypeMethod', False) == True:
                id = obj['functionId'].split('<')[0]
                if not id in dataTypeMethod:
                    dataTypeMethod.append(id)

        if len(dataTypeMethod) > 0 and options['core']:
            fd.write('\n/**\n')
            fd.write(' * @brief Data type method id map for ' + prefix + '\n')
            fd.write(' */\n')
            c.write_wrapped(fd, 80, '', '#define ' + prefix.upper() + 'DATA_TYPE_METHOD_ID_MAP(XX)',
                last_char='\\')
            for id in dataTypeMethod:
                c.write_wrapped(fd, 80, '    XX(', id + ')', last_char='\\')
            fd.write('\n')
            fd.write('\n/**\n')
            fd.write(' * @brief Data type method number enum for ' + prefix + '\n')
            fd.write(' *\n')
            fd.write(' * Warning: Do not use these directly outside of core since their value may\n')
            fd.write(' *          change between builds.\n')
            fd.write(' */\n')
            fd.write('typedef enum ' + prefix + 'data_type_method_number_e {\n')
            fd.write('    ' + prefix + 'data_type_method_number_unassigned = 0,\n')
            fd.write('#define XX(id) ' + prefix + 'data_type_method_number_ ## id,\n')
            fd.write('    ' + prefix.upper() + 'DATA_TYPE_METHOD_ID_MAP(XX)\n')
            fd.write('#undef XX\n')
            fd.write('    ' + prefix + 'data_type_method_number__max\n')
            fd.write('} ' + prefix + 'data_type_method_number_t;\n')

        useExecuteFunction.sort(key=sort_useExecuteFunction)
        useExecuteFunction.append(None)
        lastkey = ''
        for obj in useExecuteFunction:
            if obj is None or obj['useExecuteFunction'] != lastkey:
                if lastkey == 'AFW_FUNCTION_EXECUTE_STANDARD_POLYMORPHIC_FUNCTION_HANDLING':
                    fd.write(' *\n')
                    fd.write(' * This execute function is represented by NULL and is handled by standard \n')
                    fd.write(' * polymorphic function handling in the afw_value_call_built_in_function\n')
                    fd.write(' * optional_evaluate() method.\n')
                    fd.write(' */\n')
                elif lastkey != '':
                    fd.write(' */\n')
                    fd.write('const afw_value_t *\n')
                    fd.write(lastkey + '(\n')
                    fd.write('    afw_function_execute_t *x);\n')
                if obj == None: break
                lastkey = obj['useExecuteFunction']
                fd.write('\n/**\n')
                fd.write(' * @brief Function implementation function ' + obj['useExecuteFunction'] + '\n')
                fd.write(' *\n')
                fd.write(' * Implementation for functions:\n *\n')
            fd.write(' * ' + obj['functionId'] + '\n')



        if category is not None:
            fd.write('\n/** @} */\n')

        fd.write('\n/** @} */\n')
        fd.write('/** @} */\n')

        c.write_h_epilogue(fd, filename)

 
    # Generate function_binding.c
    filename = prefix + 'function_bindings.c'
    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_c_prologue(fd, generated_by, 
            'Adaptive Framework Core Adaptive Function Bindings ')
        c.write_doxygen_file_section(fd, filename,
            'Adaptive Framework core adaptive function bindings.')
        fd.write('\n')
        fd.write('#include "afw.h"\n')
        if options['core']:
            fd.write('#include "afw_internal.h"\n')
        fd.write('#include "' + prefix + 'function_bindings.h"\n')
        if options['runtime_object_maps']:
            fd.write('#include "' + prefix + 'runtime_object_maps.h"\n')
        if options['strings']:
            fd.write('#include "' + prefix + 'strings.h"\n')
        fd.write('\n')

        for obj in list:
            label = obj['functionLabel']
            fd.write('\n/* ---------- ' + obj['functionId'] + ' ---------- */\n')

            # Declaration for function value afw_value_function_definition_t if not core.
            # Core functions definitions are declared in afw_function_bindings.h.
            if not options['core']:
                fd.write('\nstatic const afw_value_function_definition_t\n')
                fd.write('impl_' + label + ';\n')

            fd.write('\nstatic const afw_utf8_t\n')
            fd.write('impl_object_path__' + label + ' =\n');
            fd.write('    AFW_UTF8_LITERAL("' + '/afw/_AdaptiveFunction_/' +  label + '");\n')

            # Create runtime object for this function
            fd.write('\nstatic const afw_runtime_object_indirect_t\n')
            fd.write('impl_object__' + label + ' = {\n') 
            fd.write('    {\n')
            if options['core']:
                fd.write('        &afw_runtime_inf__AdaptiveFunction_,\n')
            else:
                fd.write('        NULL,\n')
            fd.write('        NULL,\n')
            fd.write('        {\n')
            fd.write('            NULL,\n')
            fd.write('            NULL,\n')
            fd.write('            ' + string_ref + label + ',\n')
            fd.write('            ' + string_ref + '_AdaptiveFunction_,\n')
            fd.write('            &impl_object_path__' + label + '\n')
            fd.write('        }\n')
            fd.write('    },\n')
            if options['core']:
                fd.write('    (void *)&afw_function_definition_' + label + '\n')
            else:
                fd.write('    (void *)&impl_' + label + '\n')
            fd.write('};\n')

            # returns
            write_parameter(fd, prefix, options, 'impl_' + label + '_returns', obj.get('returns'), 'impl_' + label, 'returns')

            # parameters
            parametersCount = 0
            for parameter in obj.get('parameters'):
                parametersCount += 1
                write_parameter(fd, prefix, options, 'impl_' + label + '_parameter_' + str(parametersCount), parameter, 'impl_' + label, "parameters")
            fd.write('\nstatic const afw_value_function_parameter_t *\n')
            fd.write('impl_' + label + '_parameters[] = {\n')
            for i in range(parametersCount):
                fd.write('    &impl_' + label + '_parameter_' + str(i+1) + ',\n')
            fd.write('    NULL\n')
            fd.write('};\n')

            # polymorphicDataTypes
            polymorphicDataTypes = obj.get("polymorphicDataTypes")
            if polymorphicDataTypes is not None:
                fd.write('\nstatic const afw_utf8_t\n')
                fd.write('impl_' + label + '_polymorphicDataTypes[] = {\n')

                for dataType in polymorphicDataTypes:
                    fd.write('    AFW_UTF8_LITERAL("' + dataType + '"),\n')

                fd.write('    { NULL }\n')

                fd.write('};\n')

            # errorsThrown
            errorsThrown = obj.get("errorsThrown")
            if errorsThrown is not None:
                fd.write('\nstatic const afw_utf8_z_t *\n')
                fd.write('impl_' + label + '_errorsThrown[] = {\n')

                for errorThrown in errorsThrown:
                    for key, value in errorThrown.items():
                        fd.write('    ' + c.make_quoted(key) + ', ' + c.make_quoted(value) + ',\n')             
                    fd.write('    NULL,\n')
                fd.write('    NULL\n')

                fd.write('};\n')

            # sideEffects
            sideEffects = obj.get("sideEffects")
            if sideEffects is not None:
                fd.write('\nstatic const afw_utf8_t\n')
                fd.write('impl_' + label + '_sideEffects[] = {\n')

                for sideEffect in sideEffects:
                    fd.write('    AFW_UTF8_LITERAL("' + sideEffect + '"),\n')

                fd.write('    { NULL }\n')

                fd.write('};\n')

            # Define function define and meta
            if options['core']:
                fd.write('\nAFW_DEFINE_INTERNAL_CONST_DATA(afw_value_function_definition_t)\n')
                fd.write('afw_function_definition_' + label + ' = {\n')
                fd.write('    &afw_value_function_definition_inf,\n')
            else:
                fd.write('\nstatic const afw_value_function_definition_t\n')
                fd.write('impl_' + label + ' = {\n')
                fd.write('    NULL,\n')

            # object
            fd.write('    (const afw_object_t *)&impl_object__' + label +',\n')

            # category
            category = obj.get('category','')
            fd.write('    AFW_UTF8_LITERAL(' + c.make_quoted(obj.get('category'))  + '),\n')

            # functionId
            fd.write('    AFW_UTF8_LITERAL("' + obj.get('functionId')  + '"),\n')

            # untypedFunctionId
            fd.write('    AFW_UTF8_LITERAL("' + obj.get('functionId').split('<')[0]  + '"),\n')

            # functionLabel
            fd.write('    AFW_UTF8_LITERAL("' + label  + '"),\n')

            # camelCaseFunctionLabel
            fd.write('    AFW_UTF8_LITERAL("' + obj.get('camelCaseFunctionLabel') + '"),\n')

            # afwCamelCaseFunctionLabel
            fd.write('    AFW_UTF8_LITERAL("' + obj.get('afwCamelCaseFunctionLabel') + '"),\n')

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
            
            # functionSignature
            functionSignature = ""
            if obj.get('polymorphic', False):
                functionSignature += "`<dataType>`"
            functionSignature += '('
            sep = ''
            for p in obj.get('parameters'):
                functionSignature += sep
                sep = ', '
                saveLen = len(functionSignature)
                if p.get('minArgs') is not None:
                    i = 1
                    n = int(p.get('minArgs'))
                    while i <= n:
                        functionSignature += p.get('name') + '_' + str(i) + ': ' + make_Type(p) + sep
                        i += 1
                    functionSignature += '...'
                    if n == 0:
                        functionSignature += p.get('name') + ': (list of ' + make_Type(p) + ')'
                    else:
                        functionSignature += p.get('name') + '_rest: (list of ' + make_Type(p) + ')'
                else:
                    functionSignature += p.get('name')
                    if p.get('optional', False):
                        functionSignature += '?'             
                    functionSignature += ': ' + make_Type(p)
            functionSignature += '): '
            functionSignature += make_Type(obj.get('returns'))
            fd.write('    AFW_UTF8_LITERAL(' + c.make_quoted(functionSignature)  + '),\n') #@todo

            # functionDeclaration
            functionDeclaration = ""
            if obj.get('brief') is not None:
                functionDeclaration += '/* ' + obj.get('brief') + ' */\n'
            functionDeclaration += "function " + obj.get('functionId') + ' '
            if obj.get('polymorphic', False):
                functionDeclaration += "`<dataType>`"
 
            # functionDeclaration: Calculate max len of "name: Type" part of parameter
            maxLen = 0
            for p in obj.get('parameters'):
                thisLen = 0
                if p.get('optional', False):
                    thisLen = 1
                thisLen += len(p.get('name')) + 2 + len(make_Type(p))
                if thisLen > maxLen:
                    maxLen = thisLen

            # functionDeclaration: Emit parameters
            numberOfRequiredParameters = 0
            maximumNumberOfParameters = 0
            encounteredOptional = False
            encounteredMinArgs = False
            functionDeclaration += '(\n'
            sep = '    '
            for p in obj.get('parameters'):
                if encounteredMinArgs:
                    msg.error_exit(obj.get('functionId') + ': minArgs can only be specified on last parameter')
                functionDeclaration += sep
                sep = ',\n    '
                saveLen = len(functionDeclaration)
                if p.get('minArgs') is not None:
                    encounteredMinArgs = True
                    i = 1
                    n = int(p.get('minArgs'))
                    if n > 1 and encounteredOptional:
                        msg.error_exit(obj.get('functionId') + ': minArgs > 1 is only allowed if there are no optional parameters')
                    numberOfRequiredParameters += n
                    maximumNumberOfParameters = -1
                    while i <= n:
                        functionDeclaration += p.get('name') + '_' + str(i) + ': ' + make_Type(p) + sep
                        i += 1
                    functionDeclaration += '...'
                    if n == 0:
                        functionDeclaration += p.get('name') + ': (list of ' + make_Type(p) + ')'
                    else:
                        functionDeclaration += p.get('name') + '_rest: (list of ' + make_Type(p) + ')'
                else:
                    maximumNumberOfParameters += 1
                    functionDeclaration += p.get('name')
                    if p.get('optional', False):
                        encounteredOptional = True
                        functionDeclaration += '?'
                    else:
                        if encounteredOptional:
                            msg.error_exit(obj.get('functionId') + ': all required parameters must be first')
                        numberOfRequiredParameters += 1
                    functionDeclaration += ': ' + make_Type(p)

                if p.get('brief') is not None:
                    functionDeclaration += ' ' * (maxLen - (len(functionDeclaration) - saveLen))
                    functionDeclaration += ' /* ' + p.get('brief') + ' */'
            functionDeclaration += '\n): '


            # functionDeclaration: Return type
            functionDeclaration += make_Type(obj.get('returns'))
            functionDeclaration += ';'
            if obj.get('returns') is not None and obj.get('returns').get('brief') is not None:
                functionDeclaration += ' /* ' + obj.get('returns').get('brief') + ' */'
            functionDeclaration += '\n'


            # functionDeclaration: write
            fd.write('    AFW_UTF8_LITERAL(' + c.make_quoted(functionDeclaration)  + '),\n')
            
            # op
            if obj.get('op') is not None:
                fd.write('    AFW_UTF8_LITERAL("' + obj.get('op')  + '"),\n')
            else:
                fd.write('    AFW_UTF8_LITERAL(""),\n')

            # execute
            if obj.get('useExecuteFunction') is None:
                fd.write('    ' + prefix + 'function_execute_' + label + ',\n')
            else:
                fd.write('    ' + obj.get('useExecuteFunction') + ',\n')

            # arg_check
            fd.write('    NULL,\n') #@todo

            # numberOfRequiredParameters
            fd.write('    ' + str(numberOfRequiredParameters) + ',\n')

            # maximumNumberOfParameters
            fd.write('    ' + str(maximumNumberOfParameters) + ',\n')

            # parameters
            fd.write('    &impl_' + label + '_parameters[0],\n')

            # parameters count
            fd.write('    ' + str(parametersCount) + ',\n')

            # returns
            fd.write('    &impl_' + label + '_returns,\n')

            # polymorphicDataTypes
            if obj.get("polymorphicDataTypes") is not None:
                fd.write('    &impl_' + label + '_polymorphicDataTypes[0],\n')
            else:
                fd.write('    NULL,\n')

            # data_type
            dataType = ''
            if obj.get('dataTypeMethod', False) and category != 'polymorphic':
                dataType = category
                if options['core']:
                    fd.write('    &afw_data_type_' + dataType + '_direct,\n')
                else:
                    fd.write('    NULL,\n')
            else:
                fd.write('    NULL,\n')

            # dataType
            fd.write('    AFW_UTF8_LITERAL("' + dataType + '"),\n')

            # errorsThrown
            if obj.get("errorsThrown") is not None:
                fd.write('    &impl_' + label + '_errorsThrown[0],\n')
            else:
                fd.write('    NULL,\n')

            # sideEffects
            if obj.get("sideEffects") is not None:
                fd.write('    &impl_' + label + '_sideEffects[0],\n')
            else:
                fd.write('    NULL,\n')

            # dataTypeMethod
            if obj.get('dataTypeMethod', False) == True:
                if options['core']:
                    fd.write('    ' + prefix + 'data_type_method_number_' + \
                        obj.get('functionId').split('<')[0] + ',\n')
                else:
                    fd.write('    0, /* Assigned in a copy during function registration. */\n')
            else:
                fd.write('    0, /* Not a data type method. */\n')

            # scriptSupport
            if obj.get('scriptSupport', False) == True:
                if not options['core']:
                    msg.error_exit('scriptSupport can only be true for core functions')
                fd.write('    AFW_VALUE_SCRIPT_SUPPORT_NUMBER_' + obj.get('functionId').upper() + ',\n')
            else:
                fd.write('    0,\n')

            # polymorphic
            if obj.get('polymorphic', False):
                fd.write('    true,\n')
            else:
                fd.write('    false,\n')

            # polymorphicExecuteFunctionEvaluatesFirstParameter
            if obj.get('polymorphicExecuteFunctionEvaluatesFirstParameter', False):
                fd.write('    true,\n')
            else:
                fd.write('    false,\n')

            # deprecated
            if obj.get('deprecated', False):
                fd.write('    true,\n')
            else:
                fd.write('    false,\n')

            # pure
            if obj.get('pure', False):
                fd.write('    true,\n')
            else:
                fd.write('    false,\n')

            # signatureOnly
            if obj.get('signatureOnly', False):
                fd.write('    true\n')
            else:
                fd.write('    false\n')

            fd.write('};\n')

        fd.write('\nstatic const afw_value_function_definition_t * \n')
        fd.write('impl_function_bindings[] = {\n')
        for obj in list:
            if options['core']:
                fd.write('    &afw_function_definition_' + obj['functionLabel'] + ',\n')
            else:
                fd.write('    &impl_' + obj['functionLabel'] + ',\n')
        fd.write('    NULL\n')
        fd.write('};\n')

        fd.write('\n/* Get array of pointers to ' + prefix + 'function bindings. */\n')
        fd.write(define + '(const afw_value_function_definition_t **)\n')
        fd.write(prefix + 'function_bindings_get()\n')
        fd.write('{\n')
        fd.write('    return &impl_function_bindings[0];\n')
        fd.write('}\n')


    # Generate polymorphic data type functions skeleton
    if options['core']:
        polymorphic_functions = []
        polymorphic_skeleton_needed = {}
        for obj in list:
            if obj.get('category', '') == 'polymorphic':
                polymorphic_functions.append(obj)
            elif obj.get('dataTypeMethod', False) and obj.get('useExecuteFunction') is not None:
                polymorphicFunctionId = obj.get('functionId').split('<')[0]
                if obj.get('useExecuteFunction') == options['prefix'] + 'function_execute_' + polymorphicFunctionId:
                    polymorphic_skeleton_needed[obj.get('functionId').split('<')[0]] = True

        if len(polymorphic_skeleton_needed) > 1:
            filename = options['prefix'] + 'function_polymorphic.c'
            msg.info('Generating skeleton ' + filename)
            with nfc.open(generated_dir_path + 'function_closet/' + filename, mode='w') as fd:
                c.write_copyright(fd, prefix + 'common polymorphic function_execute_* functions')
                c.write_doxygen_file_section(fd, filename,
                    prefix + 'common polymorphic function_execute_* functions.')
                fd.write('\n')
                fd.write('#include "afw.h"\n')

                for obj in polymorphic_functions:
                    functionId = obj.get('functionId')
                    if polymorphic_skeleton_needed.get(functionId, False):
                        fd.write('\n\n\n/*\n')
                        fd.write(' * Common polymorphic function for ' + obj['functionId'] + '\n')
                        fd.write(' *\n')
                        fd.write(' * ' + prefix + 'function_execute_' + obj['functionLabel'] + '\n')
                        fd.write(' *\n')
                        fd.write(' * See ' + prefix + 'function_bindings.h for more information.\n')
                        function_comment(fd, obj)
                        fd.write(' */\n')
                        fd.write('const afw_value_t *\n')
                        fd.write(prefix + 'function_execute_' + obj['functionLabel'] + '(\n')
                        fd.write('    afw_function_execute_t *x)\n')
                        fd.write('{\n')
                        fd.write('    /** @todo Add code. */\n')
                        fd.write('    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);\n')
                        fd.write('}\n')


    # Generate function skeletons
    for category in categories:
        filename = prefix + 'function_' + category + '.c'

        # Only produce if category has any functions without useExecuteFunction
        hasFunction = False
        for obj in list:
            # if obj['category'] is not None and obj['category'] == category and obj.get('useExecuteFunction') is None:
            if (obj['category'] is not None and
                obj['category'] == category and
                (obj.get('useExecuteFunction') is None or obj.get('addExecuteFunctionToCategory', False))
                ):
                hasFunction = True
                break
        if not hasFunction:
            msg.info('Skipping unneeded skeleton ' + filename)
            continue

        msg.info('Generating skeleton ' + filename)
        with nfc.open(generated_dir_path + 'function_closet/' + filename, mode='w') as fd:
            c.write_copyright(fd, prefix + 'function_execute_* functions for ' + category)
            c.write_doxygen_file_section(fd, filename,
                prefix + 'function_execute_* functions for ' + category + '.')
            fd.write('\n')
            fd.write('#include "afw.h"\n')

            for obj in list:
                if obj['category'] is not None and obj['category'] == category:
                    if obj.get('useExecuteFunction') is not None and not obj.get('addExecuteFunctionToCategory', False):
                        continue
                    fd.write('\n\n\n/*\n')
                    fd.write(' * Adaptive function: ' + obj['functionId'] + '\n')
                    fd.write(' *\n')
                    fd.write(' * ' + prefix + 'function_execute_' + obj['functionLabel'] + '\n')
                    fd.write(' *\n')
                    fd.write(' * See ' + prefix + 'function_bindings.h for more information.\n')
                    function_comment(fd, obj)
                    fd.write(' */\n')
                    fd.write('const afw_value_t *\n')
                    fd.write(prefix + 'function_execute_' + obj['functionLabel'] + '(\n')
                    fd.write('    afw_function_execute_t *x)\n')
                    fd.write('{\n')
                    fd.write('    /** @todo Add code. */\n')
                    fd.write('    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);\n')
                    fd.write('}\n')

