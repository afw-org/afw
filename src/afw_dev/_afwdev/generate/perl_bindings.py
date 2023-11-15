#! /usr/bin/env python3

##
# @file perl_bindings.py
# @ingroup afwdev_generate
# @brief This file contains the functions used to generate Perl bindings.
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

def generate(generated_by, data_type_list, objects_dir_path, generated_dir_path, options):

    # Make sure generated/ directory structure exists
    os.makedirs(generated_dir_path, exist_ok=True)

    # Get all functions and sort by category/functionLabel
    list = direct.retrieve_objects_direct(objects_dir_path + '_AdaptiveFunctionGenerate_/')
    list.sort(key=sort_category_functionLabel_cb)

    session_params = ['adaptorId']
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
        filename = category + '.pm'
        msg.info('Generating ' + filename)
        with nfc.open(generated_dir_path + filename, 'w') as fd:
            fd.write('package afw::' + category + ';\n')
            fd.write('use strict;\n')
            fd.write('use warnings;\n')
            fd.write('\n')
            fd.write('use Exporter qw(import);\n')
            fd.write('\n')

            fd.write('our @EXPORT_OK = qw(\n')
            for obj in list:
                if obj['category'] is not None and obj['category'] == category:
                    fd.write('    ' + obj.get('functionLabel') + ' \n')
            fd.write(');\n\n')

            # write out POD comments
            fd.write('=head1 NAME\n\n')
            fd.write('afw::' + category + ' - Perl module for ' + category + ' functions\n\n')

            fd.write('=head2 Functions\n\n')
            fd.write('The following functions are exported by default\n\n')
            for obj in list:
                functionLabel = obj.get('functionLabel')
                brief = obj.get('brief', '')
                description = obj.get('description', '')
                returns = obj.get('returns')

                if obj['category'] is not None and obj['category'] == category:
                    fd.write('=head3 ' + functionLabel + '\n\n')
                    c.write_wrapped(fd, 78, '', description, '', '', True)
                    fd.write(brief + '\n\n')

                    fd.write('=head4 Parameters\n\n')

                    for param in obj.get('parameters'):
                        fd.write('    $' + param.get('name') + '\n\n')
                        c.write_wrapped(fd, 78, '', param.get('description', ''))
                        fd.write('\n')

                    #fd.write('\n')
                    #fd.write('    Returns:\n')
                    #fd.write('    ' + returns.get('dataType', 'None') + ': ' + returns.get('description', '') + '\n')
                    #fd.write('    """\n')

            fd.write('=cut\n\n')

            for obj in list:
                functionLabel = obj.get('functionLabel')
                brief = obj.get('brief', '')
                description = obj.get('description', '')
                returns = obj.get('returns')

                # make sure it's not a python reserved keyword
                if keyword.iskeyword(functionLabel):
                    functionLabel = functionLabel + '_'
                
                if obj['category'] is not None and obj['category'] == category:
                    fd.write('sub ' + functionLabel + ' {\n')

                    fd.write('    my (')
                    # ensure optional parameters come after required
                    for param in obj.get('parameters')[:-1]:
                        #if param.get('optional') or param.get('name') in session_params:
                        #    pass
                        #else:
                        fd.write('$' + param.get('name') + ', ')
                    if len(obj.get('parameters')) > 0:
                        fd.write('$' + obj.get('parameters')[-1].get('name'))
                    fd.write(') = @_;\n')

                    fd.write('\n')
                    fd.write('    my $request = $session->request()\n')
                    fd.write('\n')
                    fd.write('    $request->set("function" => "' + obj.get('functionId') + '");\n')
                    for param in obj.get('parameters'):
                        if param.get('optional') or param.get('name') in session_params:
                            pass
                        else:
                            fd.write('    $request->set("' + param.get('name') + '", $' + param.get('name') + ');\n')
                    fd.write('\n')
 
                    for param in obj.get('parameters'):
                        if param.get('optional') or param.get('name') in session_params:
                            fd.write('    if (defined $' + param.get('name') + ')\n')
                            fd.write('        $request->set("' + param.get('name') + '", $' + param.get('name') + ');\n')
                            fd.write('\n')

                    fd.write('    return $request->getResult();\n')

                    fd.write('}\n')
                    fd.write('\n')

            fd.write('42;\n')

