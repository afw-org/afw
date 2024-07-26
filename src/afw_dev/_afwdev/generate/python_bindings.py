#! /usr/bin/env python3

##
# @file python_bindings.py
# @ingroup afwdev_generate
# @brief This file contains the functions used to generate Python bindings.
#

##
# @defgroup afw_bindings_python Python
# @ingroup afw_bindings
#

import os
import keyword

from _afwdev.common import direct
from _afwdev.common import msg, nfc
from _afwdev.generate import c

def sort_category_functionLabel_cb(obj):
    category = obj.get('category')
    if category is None:
        category = ''
    return category + '~' + obj.get('functionLabel')

# returns the Python dataType from the Adaptive dataType
def get_data_type(dataType):

    if dataType == 'boolean':
        return 'bool'
    elif dataType == 'integer':
        return 'int'
    elif dataType == 'double':
        return 'float'
    elif dataType == 'string':
        return 'str'
    elif dataType == 'object':
        return 'dict'
    elif dataType == 'array':
        return 'list'
    elif dataType == 'null':
        return 'None'
    else:
        return 'object'

def generate(generated_by, data_type_list, objects_dir_path, generated_dir_path, options):

    # Make sure generated/ directory structure exists
    os.makedirs(generated_dir_path, exist_ok=True)

    # Get all functions and sort by category/functionLabel
    list = direct.retrieve_objects_direct(objects_dir_path + '_AdaptiveFunctionGenerate_/')
    list.sort(key=sort_category_functionLabel_cb)

    session_params = ['adapterId']
    categories = []
    category = None

    for obj in list:
        id = obj['functionLabel']
        x = obj.get('category')
        if x is None: x = ''
        if category is None or x != category:
            category =x
            categories.append(category)
 
    # Generate functions
    for category in categories:
        filename = category + '.py'
        msg.info('Generating ' + filename)
        with nfc.open(generated_dir_path + filename, 'w') as fd:
            fd.write('#!/usr/bin/env python3\n')
            fd.write('\n')
            fd.write('##\n')
            fd.write('# @file ' + filename + '\n')
            fd.write('# @ingroup afw_bindings_python\n')
            fd.write('# @brief This file contains the Python bindings for the ' + category + ' category.\n')
            fd.write('#\n')
            fd.write('\n')
            fd.write('# Generated by ' + generated_by + '\n')
            fd.write('\n')
            fd.write('import requests\n')
            fd.write('import json\n')
            fd.write('\n')

            for obj in list:
                functionLabel = obj.get('functionLabel')
                brief = obj.get('brief', '')
                description = obj.get('description', '')
                returns = obj.get('returns')

                # make sure it's not a python reserved keyword
                if keyword.iskeyword(functionLabel):
                    functionLabel = functionLabel + '_'
                
                if obj['category'] is not None and obj['category'] == category:
                    fd.write('def ' + functionLabel + '(session')

                    # ensure optional parameters come after required
                    for param in obj.get('parameters'):
                        if param.get('optional') or param.get('name') in session_params:
                            pass
                        else:
                            # make sure parameter isn't a reserved keyword
                            if keyword.iskeyword(param.get('name')):
                                fd.write(', _' + param.get('name'))
                            else:
                                fd.write(', ' + param.get('name'))

                    for param in obj.get('parameters'):
                        if param.get('optional') or param.get('name') in session_params:
                            # make sure parameter isn't a reserved keyword
                            if keyword.iskeyword(param.get('name')):
                                fd.write(', _' + param.get('name') + '=None')
                            else:
                                fd.write(', ' + param.get('name') + '=None')

                    fd.write('):\n')

                    # write out docstrings comments
                    fd.write('    """\n')
                    fd.write('    ' + brief + '\n')
                    fd.write('\n')
                    #fd.write('    ' + description + '\n')
                    c.write_wrapped(fd, 78, '    ', description, '', '', True)
                    fd.write('\n')
                    fd.write('    Args:')

                    for param in obj.get('parameters'):
                        fd.write('\n')
                        paramDescription = param.get('name') + ' (' + get_data_type(param.get('dataType')) + '): ' + param.get('description', '')
                        c.write_wrapped(fd, 78, '        ', paramDescription, '', '', True)                        

                    fd.write('\n')
                    fd.write('    Returns:\n')
                    returnDescription = get_data_type(returns.get('dataType')) + ': ' + returns.get('description', '')
                    c.write_wrapped(fd, 78, '        ', returnDescription, '', '', True)
                    fd.write('    """\n')

                    fd.write('\n')
                    fd.write('    request = session.Request()\n')
                    fd.write('\n')
                    fd.write('    action = {\n')
                    fd.write('        "function": "' + obj.get('functionId') + '"')
                    for param in obj.get('parameters'):
                        if param.get('optional') or param.get('name') in session_params:
                            pass
                        else:
                            fd.write(',\n')
                            if keyword.iskeyword(param.get('name')):
                                fd.write('        "' + param.get('name') + '": _' + param.get('name'))
                            else:
                                fd.write('        "' + param.get('name') + '": ' + param.get('name'))
                    fd.write('\n')
                    fd.write('    }\n')
                    fd.write('\n')
 
                    for param in obj.get('parameters'):
                        if param.get('optional') or param.get('name') in session_params:
                            if keyword.iskeyword(param.get('name')):
                                fd.write('    if _' + param.get('name') + ' != None:\n')
                                fd.write("        action['" + param.get('name') + "'] = _" + param.get('name') + '\n')
                            else:
                                fd.write('    if ' + param.get('name') + ' != None:\n')
                                fd.write("        action['" + param.get('name') + "'] = " + param.get('name') + '\n')
                            fd.write('\n')

                    fd.write('    request.add_action(action)\n')
                    fd.write('\n')
                    fd.write('    response = request.perform()\n')
                    fd.write("    if response.get('status') == 'error':\n")
                    fd.write("        raise Exception(response.get('error'))\n")
                    fd.write('\n')
                    fd.write("    return response['actions'][0]['result']\n")
                    fd.write('\n')


