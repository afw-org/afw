#! /usr/bin/env python3

import os
import fnmatch
from _afwdev.generate import c
from _afwdev.common import msg, nfc


def generate_h(generated_by, prefix, strings, generated_dir_path):

    declare_data = prefix.upper() + 'DECLARE_CONST_DATA'

    filename = prefix + 'strings.h'
    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_h_prologue(fd, generated_by, 'Adaptive Framework (' + prefix + ') Strings Header', filename)
        c.write_doxygen_file_section(fd, filename, 'Adaptive Framework (' + prefix + ') strings header.')

        fd.write('\n#include "afw_interface.h"\n')
        fd.write('#include "' + prefix + 'declare_helpers.h"\n')
        fd.write('\n')

        for name, value in sorted(strings.items()):

            fd.write('\n/** @brief #define for quoted string ' + name + ' */\n')
            fd.write('#define ' + prefix.upper() + 'Q_' + name + ' "' + value + '"\n')

            fd.write('\n/** @brief afw_utf8_t for string ' + name + ' */\n')
            fd.write(declare_data + '(afw_utf8_t) ' + prefix + 's_' + name + ';\n')

        c.write_h_epilogue(fd, filename)

def generate_c(generated_by, prefix, strings, generated_dir_path):

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
            fd.write(define_data + '(afw_utf8_t) ' + prefix + 's_' + name + ' =\n')
            fd.write('AFW_UTF8_LITERAL("' + value + '");\n')

def add_object_strings(obj, strings):
    for name in obj.keys():
        tag_name = name
        if '-' in tag_name:
            tag_name = 'a_' + name.replace('-', '_')
        strings[tag_name] = name

        if isinstance(obj[name], dict):
            add_object_strings(obj[name], strings)

        elif isinstance(obj[name], str):
            if name == 'dataType':
                strings[obj[name]] = obj[name]

def generate(generated_by, prefix, strings_dir_path, object_dir_path, generated_dir_path, extra_strings ):

    # Make sure generated/ directory structure exists
    os.makedirs(generated_dir_path, exist_ok=True)

    strings = dict()
    strings.update(extra_strings)
    strings.update({'default': 'default', 'indirect': 'indirect'})

    # Add all object id and property names to strings
    if object_dir_path is not None and os.path.exists(object_dir_path):
        for object_type in sorted(os.listdir(object_dir_path)):
            if os.path.isdir(object_dir_path + object_type):
                strings[object_type] = object_type
                objects_type_dir_path = object_dir_path + object_type + '/'
                for file in sorted(os.listdir(objects_type_dir_path)):
                    if fnmatch.fnmatch(file, '*.json'):
                        name = file[:-5]
                        strings[name] = name
                        with nfc.open(objects_type_dir_path + file, 'r') as fd:
                            content = fd.read()
                            obj = nfc.json_loads(content)
                            add_object_strings(obj, strings)

    # If there are functions, make sure some needed strings are included
    if object_dir_path is not None and os.path.exists(object_dir_path + '_AdaptiveFunctionGenerate_/'):
        strings['_AdaptiveFunction_'] = '_AdaptiveFunction_'
        strings['_AdaptiveFunctionParameter_'] = '_AdaptiveFunctionParameter_'

    # Set all of the names from *.txt files in strings_dir_path
    if strings_dir_path is not None and os.path.exists(strings_dir_path):
        for file in sorted(os.listdir(strings_dir_path)):
            if fnmatch.fnmatch(file, '*.txt') and file != 'README.txt':
                with nfc.open(strings_dir_path + file, 'r') as fd:
                    for line in fd:
                        if len(line.strip()) == 0 or line[0] == '#': continue
                        if '=' in line:
                            parts = line.partition('=')
                            strings[parts[0]] = parts[2].strip()
                        else:
                            strings[line.strip()] = line.strip()


    generate_h(generated_by, prefix, strings, generated_dir_path)

    generate_c(generated_by, prefix, strings, generated_dir_path)
