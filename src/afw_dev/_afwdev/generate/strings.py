#! /usr/bin/env python3

import os, fnmatch, re, uuid
from _afwdev.generate import c
from _afwdev.common import msg, nfc

generated_string_number = 0


supported_dataTypes = {
    'anyURI'     : 'AFW_UTF8_LITERAL',
    'boolean'    : '',
    'dnsName'    : 'AFW_UTF8_LITERAL',
    'double'     : '',
    'ia5String'  : 'AFW_UTF8_LITERAL',
    'integer'    : '',
    'ipAddress'  : 'AFW_UTF8_LITERAL',
    'objectId'   : 'AFW_UTF8_LITERAL',
    'objectPath' : 'AFW_UTF8_LITERAL',
    'regexp'     : 'AFW_UTF8_LITERAL',
    'rfc822Name' : 'AFW_UTF8_LITERAL',
    'script'     : 'AFW_UTF8_LITERAL',
    'string'     : 'AFW_UTF8_LITERAL',
    'template'   : 'AFW_UTF8_LITERAL',
    'x500Name'   : 'AFW_UTF8_LITERAL'
}



# @brief Get the string label for a given string.
# @param[in] prefix The prefix for the string label.
# @param[in] strings The dictionary of strings.
# @param[in] string The string to get the label for.
# @param[in] type The type of label to get.
# @return The string label.
#
# Parameter strings is a dictionary of strings.  The keys are the primary part
# of string labels without the prefix and the values are the strings.
# 
# type can be:
# 'U'  for a #define unquoted string label.
# 'Q'  for a #define quoted string label.
# 's'  for a afw_utf8_t string label.
# '*s' for a afw_utf8_t string label that is a pointer.
# 'v'  for a afw_value_string_t label.
# '*v' for a afw_value_string_t label that is a pointer.
# '*z' for an afw_utf8_z_t zero-terminated string label that is a pointer.
def get_string_label(
        options, string, type, labelPreference=None, dataType='string'):
    global generated_string_number

    if dataType not in supported_dataTypes:
        msg.error_exit('Unsupported dataType: ' + dataType)

    strings = options['const'].get(dataType)
    if strings is None:
        options['const'][dataType] = dict()
        strings = options['const'][dataType]
    
    determined = False
    if labelPreference is not None:
        if labelPreference in strings:
            if strings[labelPreference] != string:
                msg.warn('Multiple different values for string label: ' +
                    labelPreference)
        label = labelPreference  
        strings[label] = string     
        determined = True 

    if not determined:
        label = None
        for key, value in options['const'][dataType].items():
            if value == string:
                label = key
                break

        if label is None:
            if labelPreference is not None:
                label = labelPreference
            else:
                label = string
                if not re.fullmatch(r'[a-zA-Z0-9_]+', label):
                    if len(label) > 60:
                        generated_string_number += 1
                        label = str(generated_string_number)
                    label = 'zz__' + re.sub(r'[^a-zA-Z0-9_]', '_', label)
            options['const'][dataType][label] = string


    use_prefix = options['prefix']
    if dataType != 'string':
        use_prefix += dataType + '_'

    if type == 'Q':
        return use_prefix.upper() + 'Q_' + label
    if type == 's':
        return use_prefix + 's_' + label
    if type == 'self_s':
        return use_prefix + 'self_s_' + label
    if type == '*z':
        return use_prefix+ 'z_' + label
    if type == 'U':
        return use_prefix.upper() + 'U_' + label
    if type == 'v':
        return use_prefix + 'v_' + label
    if type == 'self_v':
        return use_prefix + 'self_v_' + label   
    msg.error_exit('Invalid string type: ' + type)



def generate_h(options, generated_by, prefix, generated_dir_path):

    declare_data = prefix.upper() + 'DECLARE_CONST_DATA'

    filename = prefix + 'strings.h'
    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_h_prologue(fd, generated_by, 'Adaptive Framework (' + prefix + ') Strings Header', filename)
        c.write_doxygen_file_section(fd, filename, 'Adaptive Framework (' + prefix + ') strings header.')

        fd.write('\n#include "afw_interface.h"\n')
        fd.write('#include "' + prefix + 'declare_helpers.h"\n')

        for dataType, strings in options['const'].items():
            use_prefix = prefix
            if dataType != 'string':
                use_prefix += dataType + '_'

            for name, value in sorted(strings.items()):
                fd.write('\n')
 
                if supported_dataTypes[dataType] == 'AFW_UTF8_LITERAL':
                    q_name = use_prefix.upper() + 'Q_' + name 
                    fd.write('\n/** @brief #define for ' + dataType + ' in quotes */\n')
                    fd.write('#define ' + q_name + ' \\\n')
                    line = repr(value)[1:-1].replace('"', '\\"')
                    fd.write('    "' + line + '"\n')
                    fd.write('\n/** @brief \'afw_utf8_t\' for ' + q_name + ' */\n')
                    fd.write('#define ' + use_prefix + 's_' + name + ' \\\n    (&' +  use_prefix + 'self_v_' + name + '.internal)\n')
                    fd.write('\n/** @brief \'afw_utf8_t\' for ' + q_name + ' */\n')
                    fd.write('#define ' + use_prefix + 'self_s_' + name + ' \\\n    (' +  use_prefix + 'self_v_' + name + '.internal)\n')
                    fd.write('\n/** @brief \'afw_value_' + dataType + '_t\' for ' + q_name + ' */\n')
                    fd.write('extern const afw_value_' + dataType + '_t \\\n    ' + use_prefix + 'self_v_' + name + ';\n')
                    fd.write('\n/** @brief \'afw_utf8_z_t *\' for ' + q_name + ' */\n')
                    fd.write('#define ' + use_prefix + 'z_' + name + ' \\\n    (' +  use_prefix + 'self_v_' + name + '.internal.s)\n')              
                    fd.write('\n/** @brief \'afw_utf8_z_t *\' for ' + q_name + ' */\n')
                    fd.write('#define ' + use_prefix + 'v_' + name + ' \\\n    ((const afw_value_t *)&' +  use_prefix + 'self_v_' + name + ')\n')              
                elif supported_dataTypes[dataType] == '':
                    fd.write('\n/** @brief #define for unquoted ' + dataType + ' ' + value + ' */\n')
                    fd.write('#define ' + use_prefix.upper() + 'U_' + name  + ' \\\n')
                    fd.write('    ' + value + '\n')
                    fd.write('\n/** @brief #define for quoted ' + dataType + ' ' + value + ' */\n')
                    fd.write('#define ' + use_prefix.upper() + 'Q_' + name  + ' \\\n')
                    line = repr(value)[1:-1].replace('"', '\\"')
                    fd.write('    "' + line + '"\n')
                    fd.write('\n/** @brief \'afw_value_' + dataType + '_t\' for ' + value + ' */\n')
                    fd.write('extern const afw_value_' + dataType + '_t \\\n    ' + use_prefix + 'self_v_' + name + ';\n')            
                else:
                    msg.error_exit(
                        'Unsupported supported_dataTypes[\'' +
                         dataType +
                         '\']: ' + supported_dataTypes[dataType])

                fd.write('\n')

        c.write_h_epilogue(fd, filename)

def generate_c(options, generated_by, prefix, generated_dir_path):

    define_data = prefix.upper() + 'DEFINE_CONST_DATA'

    filename = prefix + 'strings.c'
    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_c_prologue(fd, generated_by, 'Adaptive Framework Const')
        c.write_doxygen_file_section(fd, filename, 'Adaptive Framework builtin objects.')
        fd.write('\n')
        fd.write('#include "afw.h"\n')
        fd.write('#include "' + prefix + 'strings.h"\n')        

        for dataType, strings in options['const'].items():
            use_prefix = prefix
            if dataType != 'string':
                use_prefix += dataType + '_'

            for name, value in sorted(strings.items()):
                fd.write('\n')

                if supported_dataTypes[dataType] == 'AFW_UTF8_LITERAL':
                    fd.write('const afw_value_' + dataType + '_t\n' + use_prefix + 'self_v_' + name + ' = {\n')
                    fd.write('    &afw_value_permanent_' + dataType + '_inf,\n')
                    fd.write(
                        '    AFW_UTF8_LITERAL(' +
                        get_string_label(options, value, 'Q', dataType=dataType, labelPreference=name) +
                        ')\n')
                    fd.write('};\n')
                elif supported_dataTypes[dataType] == '':
                    fd.write('const afw_value_' + dataType + '_t\n' + use_prefix + 'self_v_' + name + ' = {\n')
                    fd.write('    &afw_value_permanent_' + dataType + '_inf,\n')
                    fd.write('    ' + value + '\n')
                    fd.write('};\n')
                else:
                    msg.error_exit(
                        'Unsupported supported_dataTypes[\'' +
                         dataType +
                         '\']: ' + supported_dataTypes[dataType])


def add_object_strings(options, obj):
    for name in obj.keys():
        tag_name = name
        if '-' in tag_name:
            tag_name = 'a_' + name.replace('-', '_')
        get_string_label(options, name, 'Q', labelPreference=tag_name)

        if isinstance(obj[name], dict):
            add_object_strings(options, obj[name])

        elif isinstance(obj[name], str):
            if name == 'dataType':
                get_string_label(
                    options, obj[name], 'Q', labelPreference=obj[name])

def generate(options, generated_by, prefix, strings_dir_path, object_dir_path, generated_dir_path):

    # Make sure generated/ directory structure exists
    os.makedirs(generated_dir_path, exist_ok=True)

    get_string_label(options, 'default', 'Q', labelPreference='default')
    get_string_label(options, 'indirect', 'Q', labelPreference='indirect')

    # Add all object id and property names to strings
    if object_dir_path is not None and os.path.exists(object_dir_path):
        for object_type in sorted(os.listdir(object_dir_path)):
            if os.path.isdir(object_dir_path + object_type):
                get_string_label(
                    options, object_type, 'Q', labelPreference=object_type)
                objects_type_dir_path = object_dir_path + object_type + '/'
                for file in sorted(os.listdir(objects_type_dir_path)):
                    if fnmatch.fnmatch(file, '*.json'):
                        name = file[:-5]
                        get_string_label(
                            options, name, 'Q', labelPreference=name)
                        with nfc.open(objects_type_dir_path + file, 'r') as fd:
                            content = fd.read()
                            obj = nfc.json_loads(content)
                            add_object_strings(options, obj)

    # If there are functions, make sure some needed strings are included
    if object_dir_path is not None and os.path.exists(object_dir_path + '_AdaptiveFunctionGenerate_/'):
        get_string_label(
            options, '_AdaptiveFunction_',
            'Q', labelPreference='_AdaptiveFunction_')
        get_string_label(
            options, '_AdaptiveFunctionParameter_',
            'Q', labelPreference='_AdaptiveFunctionParameter_')

    # Set all of the names from *.txt files in strings_dir_path
    if strings_dir_path is not None and os.path.exists(strings_dir_path):
        for file in sorted(os.listdir(strings_dir_path)):
            if fnmatch.fnmatch(file, '*.txt') and file != 'README.txt':
                with nfc.open(strings_dir_path + file, 'r') as fd:
                    for line in fd:
                        if len(line.strip()) == 0 or line[0] == '#': continue
                        if '=' in line:
                            label, op, value = line.partition('=')
                            dataType = 'string'
                            if '::' in label:
                                dataType, op, label = label.partition('::')
                            get_string_label(
                                options, value.strip(),
                                'Q', labelPreference=label,
                                dataType=dataType)                        
                        else:
                            get_string_label(
                                options, line.strip(),
                                'Q', labelPreference=line.strip())


    generate_h(options, generated_by, prefix, generated_dir_path)

    generate_c(options, generated_by, prefix, generated_dir_path)
