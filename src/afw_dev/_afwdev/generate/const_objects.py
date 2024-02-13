#! /usr/bin/env python3

##
# @file const_objects.py
# @ingroup afwdev_generate
# @brief This file contains the functions used to generate C const objects.
#

import os
from _afwdev.generate import property_type
from _afwdev.common import msg, nfc, package
from _afwdev.generate import c
from _afwdev.common import direct
from _afwdev.generate.strings import get_string_label

def new_label():
    global label_counter

    label_counter += 1
    return 'impl_' + str(label_counter)


def sort_use_id_cb(obj):
    return obj['_meta_']['objectId']


def write_const_c(options, fd, prefix, obj, path=None, embedder=None, pt=None):
    propnames = sorted(list(iter(obj)))
    meta = obj.get('_meta_', {})
    if '_meta_' in propnames:
        propnames.remove('_meta_')

    if obj.get('_meta_') and obj['_meta_'].get('objectType'):
        object_type_id = obj['_meta_']['objectType']
    else:
        object_type_id = obj.get('objectType', '')

    if path is None:
        path = '/afw/' + object_type_id + '/' + obj['_meta_']['objectId']

    # Object declaration
    fd.write('\nstatic const afw_runtime_const_object_instance_t\n')
    fd.write(meta.get('_label_') + ';\n')

    # Process embedded objects
    for propname in sorted(propnames):
        pt = property_type.get(object_type_id, propname)
        prop = obj.get(propname)
        tag_propname = propname.replace('-', '_')
        if isinstance(prop, dict):
            if prop.get('_meta_') is None:
                prop['_meta_']={}

            if pt.get('dataTypeParameter') is not None:
                prop['_meta_']['objectType'] = pt.get('dataTypeParameter','')
            else:
                prop['_meta_']['objectType'] = pt.get('objectType','')
            prop['_meta_']['_label_'] = new_label()
            prop['_meta_']['propname'] = propname
            write_const_c(options, fd, prefix, prop, path + '/' + propname, obj, pt)
    
    # Object        
    fd.write('\n/*\n')
    fd.write(' * ' + path + '\n')
    fd.write(' */\n')
    
    s_ = '&' + prefix + 'self_s_'
    propnames = sorted(list(iter(obj)))
    meta = obj.get('_meta_', {})
    if '_meta_' in propnames:
        propnames.remove('_meta_')

    for propname in sorted(propnames):
        prop = obj[propname]
        tag_propname = propname.replace('-', '_')

        if isinstance(prop, str):
            dataType = 'string'
            value = 'AFW_UTF8_LITERAL(' + c.make_quoted(prop) + ')'
        elif isinstance(prop, bool):
            dataType = 'boolean'
            if prop:
                value = "true"
            else:
                value = "false"
                v = "false"
        elif isinstance(prop, int):
            dataType = 'integer'
            value = str(prop)
        elif isinstance(prop, float):
            dataType = 'double'
            value = str(prop)
        elif isinstance(prop, list):
            dataType = 'array'
            elementType = 'string'
            if len(prop) != 0:
                if isinstance(prop[0], int):
                    elementType = 'integer'
                    fd.write('\nstatic const afw_integer_t\n' +
                    obj['_meta_']['_label_'] + '_array_' + tag_propname + '[] = {\n')
                    comma = ''
                    for value in sorted(prop):
                        fd.write(comma + '    ' + str(value))
                        comma = ',\n'
                    fd.write('\n};\n')
                else:
                    fd.write('\nstatic const afw_utf8_t\n' +
                    obj['_meta_']['_label_'] + '_array_' + tag_propname + '[] = {\n')
                    comma = ''
                    for value in sorted(prop):
                        fd.write(comma + '    AFW_UTF8_LITERAL(' + c.make_quoted(value) + ')')
                        comma = ',\n'
                    fd.write('\n};\n')

            label = obj['_meta_']['_label_'] + '_list_' + tag_propname
            value_label = label + '__value'
            fd.write('\nstatic const afw_value_array_t\n')
            fd.write(value_label + ';\n')

            fd.write('\nstatic const afw_array_wrapper_for_array_self_t\n')
            fd.write(label + ' = {\n')
            fd.write('    {\n')
            fd.write('        &afw_array_wrapper_for_array_inf,\n')
            fd.write('        NULL,\n')
            fd.write('        (const afw_value_t *)&' + value_label +'\n')
            fd.write('    },\n')
            fd.write('    &afw_data_type_' + elementType + '_direct,\n')
            if len(prop) != 0:
                fd.write('    sizeof(' +  obj['_meta_']['_label_'] + '_array_' + tag_propname + ') / sizeof(')
                if (elementType == 'integer'):
                    fd.write('afw_integer_t')
                else:
                    fd.write('afw_utf8_t')
                fd.write('),\n')
                fd.write('    &' + obj['_meta_']['_label_'] + '_array_' + tag_propname + '\n')
            else:
                fd.write('    0,\n')
                fd.write('    NULL\n')
            fd.write('};\n')

            fd.write('\nstatic const afw_value_array_t\n')
            fd.write(value_label + ' = {\n')
            fd.write('    {&afw_value_permanent_array_inf},\n')
            fd.write('    (const afw_array_t *)&' + label +'\n')
            fd.write('};\n')

            value = '(const afw_array_t *)&' + obj['_meta_']['_label_'] + '_list_' + tag_propname
        elif isinstance(prop, dict):
            dataType = 'object'
            value = '(const afw_object_t *)&' + prop['_meta_']['_label_']
        else:
            fd.write('Error>>>\n')
            return

        if dataType != 'string':
            fd.write('\nstatic const afw_value_' + dataType + '_t\n' +
                obj['_meta_']['_label_'] + '_property_value_' + tag_propname + ' = {\n')
            fd.write('    {&afw_value_permanent_' + dataType + '_inf},\n')
            fd.write('    ' + value + '\n')
            fd.write('};\n')

        fd.write('\nstatic const afw_runtime_property_t\n')
        fd.write(obj['_meta_']['_label_'] + '_property_' + tag_propname + ' = {\n')
        if '-' in propname:
            fd.write('    ' + s_ + 'a_' + tag_propname + ',\n')
        else:
            fd.write('    ' + s_ + propname + ',\n')

        if dataType == 'string':
            fd.write('    &' + get_string_label(options, prop, 'self_v') + '.pub\n')
        else:
            fd.write('    &' + obj['_meta_']['_label_'] + '_property_value_' + tag_propname + '.pub\n')
        fd.write('};\n')

    fd.write('\nstatic const afw_runtime_property_t *\n')
    fd.write(obj['_meta_']['_label_'] + '_properties[] = {\n')
    for propname in sorted(propnames):
        tag_propname = propname.replace('-', '_')
        if propname != '_meta_':
            fd.write('    &' + obj['_meta_']['_label_'] + '_property_' + tag_propname + ',\n')
    fd.write('    NULL\n};\n')

    parentPaths = meta.get('parentPaths')
    parentPathsList = 'NULL'
    if parentPaths is not None:
        if not isinstance(parentPaths, list):
            msg.error_exit('parentPaths must be a list')
        fd.write('\nstatic const afw_utf8_t\n')
        fd.write(meta.get('_label_') + '_parentPaths_array[] = {\n')
        comma = ''
        for value in parentPaths:
            fd.write(comma + '    AFW_UTF8_LITERAL(' + c.make_quoted(value) + ')')
            comma = ',\n'
        fd.write('\n};\n')

        fd.write('\nstatic const afw_array_wrapper_for_array_self_t\n')
        fd.write(meta.get('_label_') + '_parentPaths_list = {\n')
        fd.write('    { &afw_array_wrapper_for_array_inf, NULL, NULL },\n')
        fd.write('    &afw_data_type_anyURI_direct,\n')
        fd.write('    sizeof(' +  meta.get('_label_') + '_parentPaths_array) / sizeof(afw_utf8_t),\n')
        fd.write('    (const void *)&' + meta.get('_label_') + '_parentPaths_array\n')
        fd.write('};\n')

        fd.write('\nstatic const afw_value_array_t\n')
        fd.write(meta.get('_label_') + '_parentPaths = {\n')
        fd.write('    {&afw_value_permanent_array_inf},\n')
        fd.write('    (const afw_array_t *)&' + meta.get('_label_') + '_parentPaths_list\n')
        fd.write('};\n')

        parentPathsList = '&' + meta.get('_label_') + '_parentPaths'

    

    value_label = meta.get('_label_') + '_meta_object__value'
    fd.write('\nstatic const afw_value_object_t\n')
    fd.write(value_label + ';\n')

    fd.write('\nstatic const afw_runtime_const_object_meta_object_t\n')
    fd.write(meta.get('_label_') + '_meta_object = {\n')
    fd.write('    {\n')
    fd.write('        &afw_runtime_inf_const_meta_object_inf,\n')
    fd.write('        NULL,\n')
    fd.write('        (const afw_value_t *)&' + value_label + '\n')
    fd.write('    },\n')
    fd.write('    &' + meta.get('_label_') + ',\n')
    fd.write('    ' + parentPathsList + '\n')
    fd.write('};\n')

    fd.write('\nstatic const afw_value_object_t\n')
    fd.write(value_label + ' = {\n')
    fd.write('    {&afw_value_permanent_object_inf},\n')
    fd.write('    (const afw_object_t *)&' + meta.get('_label_') +'_meta_object\n')
    fd.write('};\n')

    fd.write('\nstatic const afw_utf8_t\n')
    fd.write(meta.get('_label_') + '_meta_path =\n')
    fd.write('    AFW_UTF8_LITERAL("' + path + '");\n')

    value_label = meta.get('_label_') + '__value'
    fd.write('\nstatic const afw_value_object_t\n')
    fd.write(value_label + ';\n')

    if embedder is None:
        fd.write('\nstatic const afw_runtime_const_object_instance_t\n')
        fd.write(meta.get('_label_') + ' = {\n')
        fd.write('    {\n')
        fd.write('        &afw_runtime_inf_' + meta.get('objectType') + ',\n')
        fd.write('        NULL,\n')
        fd.write('        (const afw_value_t *)&' + value_label + ',\n')
        fd.write('        {\n')
        fd.write('            (const afw_object_t *)&' + meta.get('_label_') + '_meta_object,\n')
        fd.write('            NULL,\n')
        fd.write('            &' + get_string_label(options, meta.get('objectId'), 'self_s') + ',\n')
        fd.write('            &' + get_string_label(options, meta.get('objectType'), 'self_s') + ',\n')
        fd.write('            &' + meta.get('_label_') + '_meta_path\n')
        fd.write('        }\n')
        fd.write('    },\n')
        fd.write('    &' + meta.get('_label_') + '_properties[0]\n')
        fd.write('};\n')
    else:
        fd.write('\nstatic const afw_runtime_const_object_instance_t\n')
        fd.write(meta.get('_label_') + ' = {\n')

        fd.write('    {\n')
        fd.write('        &afw_runtime_inf_const_embedded_untyped_object,\n')
        fd.write('        NULL,\n')
        fd.write('        (const afw_value_t *)&' + value_label + ',\n')
        fd.write('        {\n')
        fd.write('            (const afw_object_t *)&' + meta.get('_label_') + '_meta_object,\n')
        fd.write('            (const afw_object_t *)&' + embedder['_meta_']['_label_'] + ',\n')
        fd.write('            &' + get_string_label(options, meta.get('propname'), 'self_s') + ',\n')

        if meta.get('objectType') is not None and meta.get('objectType') != '':
            fd.write('            &' + get_string_label(options, meta.get('objectType'), 'self_s') + ',\n')
        else:
            fd.write('            NULL,\n')
        fd.write('            &' + meta.get('_label_') + '_meta_path\n')
        #@todo object type and path
        fd.write('        }\n')
        fd.write('    },\n')
        fd.write('    &' + meta.get('_label_') + '_properties[0]\n')
        fd.write('};\n')

    fd.write('\nstatic const afw_value_object_t\n')
    fd.write(value_label + ' = {\n')
    fd.write('    {&afw_value_permanent_object_inf},\n')
    fd.write('    (const afw_object_t *)&' + meta.get('_label_') +'\n')
    fd.write('};\n')



def write_object_pointer_list(fd, prefix, list):
    fd.write('\n/* NULL terminated list of objects. */\n')
    fd.write('static const afw_object_t *\n')
    fd.write('impl_const_objects[] = {\n')
    for obj in list:
        fd.write('    (const afw_object_t *)&' + obj['_meta_']['_label_'] + ',\n')
    fd.write('    NULL\n')
    fd.write('};\n')
    fd.write('\n/* Set const objects */\n')
    fd.write('void\n')
    fd.write(prefix + 'const_objects_register(afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    afw_runtime_env_set_objects(\n')
    fd.write('        &impl_const_objects[0], false, xctx);\n')
    fd.write('}\n')


def generate(generated_by, prefix, object_dir_path,
                generated_dir_path, objectTypes, options):

    # Reset label counter
    global label_counter
    label_counter = 0

    # Make sure generated/ directory structure exists
    os.makedirs(generated_dir_path, exist_ok=True)

    # Get all object types and sort by id.
    list = []

    for objectType in objectTypes:
        path = object_dir_path + objectType + '/'
        if os.path.exists(path):
            list += direct.retrieve_objects_direct(path)           
    list.sort(key=sort_use_id_cb)

    filename = prefix + 'const_objects.h'

    afw_package = package.get_afw_package(options)
    copyright = afw_package.get('copyright')

    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_h_prologue(fd, generated_by, 'Adaptive Framework Builtin Objects Header', copyright, filename)
        c.write_doxygen_file_section(fd, filename, 'Adaptive Framework builtin objects header.')
        fd.write('\n#include "afw_interface.h"\n')
        fd.write('\n\n/**\n')
        fd.write(' * @brief Get array of ' + prefix + ' const objects. \n')
        fd.write(' */\n')
        fd.write('void\n')
        fd.write(prefix + 'const_objects_register(afw_xctx_t *xctx);\n')
        c.write_h_epilogue(fd, filename)

    filename = prefix + 'const_objects.c'
    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_c_prologue(fd, generated_by, 'Adaptive Framework Const', copyright)
        c.write_doxygen_file_section(fd, filename, 'Adaptive Framework builtin objects.')
        fd.write('\n')
        fd.write('#include "afw.h"\n')
        fd.write('#include "' + prefix + 'generated.h"\n')
        fd.write('\n')

        for obj in list:
            obj['_meta_']['_label_'] = new_label()           
            write_const_c(options, fd, prefix, obj)
        write_object_pointer_list(fd, prefix, list)
