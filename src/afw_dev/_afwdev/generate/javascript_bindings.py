#! /usr/bin/env python3

import os
import shutil
from _afwdev.common import direct
from _afwdev.common import msg, nfc
from _afwdev.generate import c

def is_reserved_keyword(name):
    reserved = [
        'break', 'case', 'catch', 'class', 'const', 'continue', 'debugger', 
        'default', 'delete', 'do', 'else', 'export', 'extends', 'finally', 
        'for', 'function', 'if', 'import', 'in', 'instanceof', 'new', 'return',
        'super', 'switch', 'this', 'throw', 'try', 'typeof', 'var', 'void', 
        'while', 'with', 'yield', 'enum', 'implements', 'interface', 'let', 
        'package', 'private', 'protected', 'public', 'static', 'abstract', 
        'boolean', 'byte', 'char', 'double', 'final', 'float', 'goto', 'int', 
        'long', 'native', 'short', 'synchronized', 'throws', 'transient', 'volatile'
    ]
    return name in reserved

def sort_category_functionLabel_cb(obj):
    category = obj.get('category')
    if category is None:
        category = ''
    return category + '~' + obj.get('functionLabel')

def typescript_type(dataType):

    if dataType == 'string':
        return 'string'
    elif dataType == 'boolean':
        return 'boolean'
    elif dataType == 'integer':
        return 'number'
    elif dataType == 'double':
        return 'number'
    elif dataType == 'null':
        return 'null'
    elif dataType == 'array':
        return 'any[]'
    elif dataType == 'object':
        return 'object'
    else: 
        return 'any'

def generate(generated_by, data_type_list, objects_dir_path, generated_dir_path, options):

    # Make sure generated/ directory structure exists
    os.makedirs(generated_dir_path, exist_ok=True)
    os.makedirs(generated_dir_path + '/src', exist_ok=True)

    if options['additional_javascript']:        
        shutil.copyfile(options['additional_javascript_dir_path'] + 'package.json', generated_dir_path + 'package.json')
        shutil.copyfile(options['additional_javascript_dir_path'] + 'tsconfig.json', generated_dir_path + 'tsconfig.json')

    # Get all functions and sort by category/functionLabel
    list = direct.retrieve_objects_direct(objects_dir_path + '_AdaptiveFunctionGenerate_/')
    list.sort(key=sort_category_functionLabel_cb)

    action_params = ['adaptorId']
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

        # skip deprecated category methods
        if category == 'deprecated':
            continue

        filename = category + '.ts'
        msg.info('Generating ' + filename)
        with nfc.open(generated_dir_path + '/src/' + filename, 'w') as fd:
            c.write_copyright(fd, 'Javascript Bindings for ' + category)
            fd.write('\n')

            fd.write('\n')
            fd.write('interface IAnyObject {\n')
            fd.write('    [prop : string] : any;\n')
            fd.write('}\n')
            fd.write('\n')

            for obj in list:
                functionLabel = obj.get('afwCamelCaseFunctionLabel')
                brief = obj.get('brief', '')
                description = obj.get('description', '')
                returns = obj.get('returns')

                if obj['category'] is not None and obj['category'] == category:
                    fd.write('/**\n')
                    c.write_wrapped(fd, 78, ' * ', description, '', '', True)
                    fd.write(' * \n')

                    for param in obj.get('parameters'):
                        name = param.get('name')
                        if is_reserved_keyword(name):
                            name = '_' + name

                        dataType = param.get('dataType', '')
                        descr = param.get('description', '')

                        paramStr = '@param {' + dataType + '} ' + name + ' - ' + descr
                        c.write_wrapped(fd, 78, ' * ', paramStr, '    ', '', True)
                        fd.write(' * \n')

                    returnsStr = '@returns {' + returns.get('dataType', '') + '} ' + returns.get('description', '')
                    c.write_wrapped(fd, 78, ' * ', returnsStr, '    ', '', True)
                    fd.write(' */\n')

                    fd.write('export function ' + functionLabel + '(client : any')

                    # ensure optional parameters come after required
                    for param in obj.get('parameters'):
                        if param.get('optional') or param.get('name') in action_params:
                            pass
                        else:
                            if is_reserved_keyword(param.get('name')):
                                fd.write(', _' + param.get('name') + ' : ' + typescript_type(param.get('dataType')))
                            else:
                                fd.write(', ' + param.get('name') + ' : ' + typescript_type(param.get('dataType')))

                    for param in obj.get('parameters'):
                        if param.get('optional') or param.get('name') in action_params:
                            if is_reserved_keyword(param.get('name')):
                                fd.write(', _' + param.get('name') + '? : ' + typescript_type(param.get('dataType')))
                            else:
                                fd.write(', ' + param.get('name') + '? : ' + typescript_type(param.get('dataType')))

                    fd.write(') : any {\n')
                    fd.write('\n')
                    fd.write('    let _action : IAnyObject = {};\n')
                    fd.write('\n')
                    fd.write('    _action["function"] = "' + obj.get('functionId') + '";')
                    for param in obj.get('parameters'):
                        if param.get('optional') or param.get('name') in action_params:
                            pass
                        else:
                            fd.write('\n')
                            if is_reserved_keyword(param.get('name')):
                                fd.write('    _action["' + param.get('name') + '"] = _' + param.get('name') + ';')
                            else:
                                fd.write('    _action["' + param.get('name') + '"] = ' + param.get('name') + ';')
                    fd.write('\n')
                    fd.write('\n')
 
                    for param in obj.get('parameters'):
                        if param.get('optional') or param.get('name') in action_params:
                            if is_reserved_keyword(param.get('name')):
                                fd.write('    if (_' + param.get('name') + ' !== undefined)\n')
                                fd.write('        _action["' + param.get('name') + '"] = _' + param.get('name') + ';\n')
                            else:
                                fd.write('    if (' + param.get('name') + ' !== undefined)\n')
                                fd.write('        _action["' + param.get('name') + '"] = ' + param.get('name') + ';\n')
                            fd.write('\n')

                    fd.write('    return client.perform(_action);\n')
                    fd.write('}\n')
                    fd.write('\n')       
                    


    with nfc.open(generated_dir_path + 'src/index.js', 'w') as fd:

        for category in categories:
            if category == 'deprecated':
                continue

            fd.write('export * from "./' + category + '";\n')

    with nfc.open(generated_dir_path + 'src/index.ts', 'w') as fd:

        for category in categories:
            if category == 'deprecated':
                continue

            fd.write('export * from "./' + category + '";\n')
            
