#! /usr/bin/env python3

##
# @file php_bindings.py
# @ingroup afwdev_generate
# @brief This file contains the functions used to generate PHP bindings.
#

##
# @defgroup afw_bindings_php PHP
# @ingroup afw_bindings
#

import os
import keyword

from _afwdev.common import direct
from _afwdev.common import msg, nfc, package
from _afwdev.generate import c

def sort_category_functionLabel_cb(obj):
    category = obj.get('category')
    if category is None:
        category = ''
    return category + '~' + obj.get('functionLabel')

def generate(generated_by, data_type_list, objects_dir_path, generated_dir_path, options):

    # Make sure generated/ directory structure exists
    os.makedirs(generated_dir_path, exist_ok=True)

    # Get all functions and sort by category/functionLabel
    list = direct.retrieve_objects_direct(objects_dir_path + '_AdaptiveFunctionGenerate_/')
    list.sort(key=sort_category_functionLabel_cb)

    session_params = ['adaptorId']
    categories = []
    category = None

    afw_package = package.get_afw_package(options)
    copyright = afw_package.get('copyright')

    for obj in list:
        id = obj['functionLabel']
        x = obj.get('category')
        if x is None: x = ''
        if category is None or x != category:
            category =x
            categories.append(category)
 
    # Generate functions
    for category in categories:
        filename = category + '.php'
        msg.info('Generating ' + filename)
        with nfc.open(generated_dir_path + filename, 'w') as fd:
            fd.write('<?php\n\n')

            c.write_copyright(fd, 'PHP Bindings for ' + category, copyright)

            fd.write('\n')
            fd.write('/**\n')
            fd.write(' * @file ' + filename + '\n')
            fd.write(' * @ingroup afw_bindings_php\n')
            fd.write(' * @brief Class definition for function category ' + category + '\n')
            fd.write(' */\n')
            fd.write('class ' + category + '\n')
            fd.write('{\n')

            # private variables
            fd.write('    private $session;\n')
            fd.write('\n')

            # generate constructor
            fd.write('    /**\n')
            fd.write('     * Constructs the ' + category + ' class, using a session.\n')
            fd.write('     */\n')
            fd.write('    public function __construct($session = null)\n')
            fd.write('    {\n')
            fd.write('        $this->session = $session;\n')
            fd.write('    }\n')
            fd.write('\n')

            for obj in list:
                functionLabel = obj.get('functionLabel')

                #@todo make sure it's not a php reserved keyword
        
                if obj['category'] is not None and obj['category'] == category:
                    description = obj.get('description')
                    returns = obj.get('returns')

                    fd.write('    /**\n')
                    fd.write('     * ' + functionLabel + '()\n')

                    fd.write('     *\n')
                    c.write_wrapped(fd, 78, '     * ', description, '', '', True)

                    fd.write('     *\n')
                    for param in obj.get('parameters'):
                        dataType = param.get('dataType', '')
                        name = param.get('name')
                        descr = param.get('description', '')

                        c.write_wrapped(fd, 78, '     * ', '@param ' + dataType + ' $' + name + ' ' + descr, '          ' + ' ' * (len(name) + len(dataType)), '', True)

                    fd.write('     *\n')

                    c.write_wrapped(fd, 78, '     * ', '@return ' + returns.get('dataType', '') + ' ' + returns.get('description', ''), '         ' + ' ' * len(returns.get('dataType', '')), '', True)
                    fd.write('     */\n')
                    fd.write('    public function ' + functionLabel + '(')

                    # ensure optional parameters come after required
                    for param in obj.get('parameters'):
                        if param.get('optional') or param.get('name') in session_params:
                            pass
                        else:
                            fd.write(', $' + param.get('name'))

                    for param in obj.get('parameters'):
                        if param.get('optional') or param.get('name') in session_params:
                            fd.write(', $' + param.get('name') + ' = null')

                    fd.write(')\n')
                    fd.write('    {\n')
                    fd.write('        $request = $this->$session->request();\n')
                    fd.write('\n')
                    fd.write('        $request->set("function", "' + obj.get('functionId') + '");')

                    fd.write('\n\n')
                    fd.write('        /* pass along required parameters to the request payload */')
                    for param in obj.get('parameters'):
                        if param.get('optional') or param.get('name') in session_params:
                            pass
                        else:
                            fd.write('\n')
                            fd.write('        $request->set("' + param.get('name') + '", $' + param.get('name') + ');')
                    fd.write('\n')
                    fd.write('\n')
 
                    fd.write('        /* pass along any optional parameters to the request payload */\n')
                    for param in obj.get('parameters'):
                        if param.get('optional') or param.get('name') in session_params:
                            fd.write('        if ($' + param.get('name') + ' != null)\n')
                            fd.write("            $request->set('" + param.get('name') + "', $" + param.get('name') + ');\n')
                            fd.write('\n')

                    fd.write("        return $request->get_result();\n")
                    fd.write("    }\n")
                    fd.write("\n")
            fd.write('}\n')
            fd.write('\n')
            fd.write('?>\n')


