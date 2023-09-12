#! /usr/bin/env python3

import os, fnmatch, re, uuid
from _afwdev.generate import c
from _afwdev.common import msg, nfc

generated_string_number = 0



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
# 'Q'  for a #define quoted string label.
# 's'  for a afw_utf8_t string label.
# '*s' for a afw_utf8_t string label that is a pointer.
# 'v'  for a afw_value_string_t label.
# '*v' for a afw_value_string_t label that is a pointer.
# '*z' for an afw_utf8_z_t zero-terminated string label that is a pointer.
def get_string_label(options, string, type, labelPreference=None):
    global generated_string_number
    
    determined = False
    if labelPreference is not None:
        if labelPreference in options['extra_strings']:
            if options['extra_strings'][labelPreference] != string:
                msg.warn('Multiple different values for string label: ' +
                    labelPreference)
        label = labelPreference  
        options['extra_strings'][label] = string     
        determined = True 

    if not determined:
        label = None
        for key, value in options['extra_strings'].items():
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
                    label = '_g__' + re.sub(r'[^a-zA-Z0-9_]', '_', label)
            options['extra_strings'][label] = string

    if type == 'Q':
        return options['prefix'].upper() + 'Q_' + label
    if type == 's':
        return options['prefix'] + 's_' + label
    if type == '*s':
        return '&' + options['prefix'] + 's_' + label
    if type == 'v':
        return options['prefix'] + 'v_' + label
    if type == '*v':
        return '&' + options['prefix'] + 'v_' + label
    if type == '*z':
       return options['prefix'] + 'z_' + label
    msg.error_exit('Invalid string type: ' + type)



def generate_h(generated_by, prefix, strings, generated_dir_path):

    declare_data = prefix.upper() + 'DECLARE_CONST_DATA'

    filename = prefix + 'strings.h'
    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_h_prologue(fd, generated_by, 'Adaptive Framework (' + prefix + ') Strings Header', filename)
        c.write_doxygen_file_section(fd, filename, 'Adaptive Framework (' + prefix + ') strings header.')

        fd.write('\n#include "afw_interface.h"\n')
        fd.write('#include "' + prefix + 'declare_helpers.h"\n')

        for name, value in sorted(strings.items()):
            fd.write('\n')

            fd.write('\n/** @brief define for quoted string ' + name + ' */\n')
            q_name = prefix.upper() + 'Q_' + name 
            fd.write('#define ' + q_name + ' \\\n')
            # for line in break_into_substrings(repr(value)[1:-1].replace('"', '\\"')):
            #     fd.write('    "' + line + '" \\\n')
            line = repr(value)[1:-1].replace('"', '\\"')
            fd.write('    "' + line + '"\n')

            fd.write('\n/** @brief \'afw_utf8_t\' for string ' + q_name + ' */\n')
            fd.write('#define ' + prefix + 's_' + name + ' \\\n    (' +  prefix + 'v_' + name + '.internal)\n')
            
            fd.write('\n/** @brief \'afw_value_string_t\' for string ' + q_name + ' */\n')
            fd.write('extern const afw_value_string_t \\\n    ' + prefix + 'v_' + name + ';\n')

            fd.write('\n/** @brief \'afw_utf8_z_t *\' for string ' + q_name + ' */\n')
            fd.write('#define ' + prefix + 'z_' + name + ' \\\n    (' +  prefix + 'v_' + name + '.internal.s)\n')
            
            fd.write('\n')

        c.write_h_epilogue(fd, filename)

def generate_c(options, generated_by, prefix, strings, generated_dir_path):

    define_data = prefix.upper() + 'DEFINE_CONST_DATA'

    filename = prefix + 'strings.c'
    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_c_prologue(fd, generated_by, 'Adaptive Framework Const')
        c.write_doxygen_file_section(fd, filename, 'Adaptive Framework builtin objects.')
        fd.write('\n')
        fd.write('#include "afw.h"\n')
        fd.write('#include "' + prefix + 'strings.h"\n')        

        for name, value in sorted(strings.items()):
            fd.write('\n')
            fd.write('const afw_value_string_t ' + prefix + 'v_' + name + ' = {\n')
            fd.write('    &afw_value_permanent_string_inf,\n')
            fd.write('    AFW_UTF8_LITERAL(' + get_string_label(options, value, 'Q') + ')\n')
            fd.write('};\n')

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

def generate(options, generated_by, prefix, strings_dir_path, object_dir_path, generated_dir_path, extra_strings ):

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
                            parts = line.partition('=')
                            get_string_label(
                                options, parts[2].strip(),
                                'Q', labelPreference=parts[0])
                        else:
                            get_string_label(
                                options, line.strip(),
                                'Q', labelPreference=line.strip())


    generate_h(
        generated_by, prefix, options['extra_strings'], generated_dir_path)

    generate_c(
        options, generated_by, prefix, options['extra_strings'],
        generated_dir_path)
