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


def _const_array_element_value_expr(options, element_type, value):
    """C expression for an interned permanent value of one array element."""
    if element_type == 'integer':
        bag = str(value)
        label_pref = None
        if value == 0:
            label_pref = 'zero'
        elif value == 1:
            label_pref = 'one'
        return get_string_label(
            options, bag, 'v', dataType='integer',
            labelPreference=label_pref)
    if element_type == 'boolean':
        bag = 'true' if value else 'false'
        return get_string_label(
            options, bag, 'v', dataType='boolean')
    return get_string_label(
        options, value, 'v', dataType=element_type)


def write_const_array_of_values(
        fd, options, instance_label, value_label, element_type, py_values):
    """Emit a static typed const array of interned value pointers.

    instance_label names the afw_array_from_values_self_t.
    value_label names the dual afw_value_array_t (permanent).
    """
    values_label = instance_label + '_values'
    count = len(py_values) if py_values else 0

    if count > 0:
        fd.write('\nstatic const afw_value_t *\n')
        fd.write(values_label + '[] = {\n')
        comma = ''
        for value in py_values:
            fd.write(comma + '    ' +
                     _const_array_element_value_expr(
                         options, element_type, value))
            comma = ',\n'
        fd.write('\n};\n')

    fd.write('\nstatic const afw_value_array_t\n')
    fd.write(value_label + ';\n')

    fd.write('\nstatic const afw_array_from_values_self_t\n')
    fd.write(instance_label + ' = {\n')
    fd.write('    {\n')
    fd.write('        &afw_array_permanent_from_values_inf,\n')
    fd.write('        NULL,\n')
    fd.write('        (const afw_value_t *)&' + value_label + '\n')
    fd.write('    },\n')
    fd.write('    &afw_data_type_' + element_type + '_direct,\n')
    fd.write('    ' + str(count) + ',\n')
    if count > 0:
        fd.write('    ' + values_label + '\n')
    else:
        fd.write('    NULL\n')
    fd.write('};\n')

    fd.write('\nstatic const afw_value_array_t\n')
    fd.write(value_label + ' = {\n')
    fd.write('    {&afw_value_permanent_array_inf},\n')
    fd.write('    (const afw_array_t *)&' + instance_label + '\n')
    fd.write('};\n')


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
    
    v_ = prefix + 'v_'
    propnames = sorted(list(iter(obj)))
    meta = obj.get('_meta_', {})
    if '_meta_' in propnames:
        propnames.remove('_meta_')

    for propname in sorted(propnames):
        prop = obj[propname]
        tag_propname = propname.replace('-', '_')
        # Permanent scalar and typed array elements go through options['const']
        # via get_string_label so true/false/0/1 and repeated literals share
        # one afw_*_self_v_* (see strings.seed_from_strings_dir / strings.txt).
        # object properties still need per-instance permanent wrappers.
        use_shared_scalar = False
        value_expr = None

        if isinstance(prop, str):
            dataType = 'string'
            use_shared_scalar = True
            value_expr = (
                '&' + get_string_label(options, prop, 'self_v') + '.pub')
        elif isinstance(prop, bool):
            # JSON bool before int (bool is a subclass of int in Python).
            dataType = 'boolean'
            use_shared_scalar = True
            bag = 'true' if prop else 'false'
            value_expr = (
                '&' + get_string_label(
                    options, bag, 'self_v', dataType='boolean') + '.pub')
        elif isinstance(prop, int):
            dataType = 'integer'
            use_shared_scalar = True
            bag = str(prop)
            # Prefer conventional labels when first registration (after seed
            # these match integer::zero / integer::one from strings.txt).
            label_pref = None
            if prop == 0:
                label_pref = 'zero'
            elif prop == 1:
                label_pref = 'one'
            value_expr = (
                '&' + get_string_label(
                    options, bag, 'self_v', dataType='integer',
                    labelPreference=label_pref) + '.pub')
        elif isinstance(prop, float):
            dataType = 'double'
            use_shared_scalar = True
            # C initializer text stored in the bag; must match a strings.txt
            # double:: value text to share that permanent (e.g. 1.0e1).
            bag = str(prop)
            value_expr = (
                '&' + get_string_label(
                    options, bag, 'self_v', dataType='double') + '.pub')
        elif isinstance(prop, list):
            dataType = 'array'
            elementType = 'string'
            py_values = list(prop)
            if len(py_values) != 0:
                if isinstance(py_values[0], bool):
                    elementType = 'boolean'
                elif isinstance(py_values[0], int):
                    elementType = 'integer'
                    py_values = sorted(py_values)
                else:
                    py_values = sorted(py_values)
            label = obj['_meta_']['_label_'] + '_list_' + tag_propname
            value_label = label + '__value'
            write_const_array_of_values(
                fd, options, label, value_label, elementType, py_values)
            use_shared_scalar = True
            value_expr = '(const afw_value_t *)&' + value_label
        elif isinstance(prop, dict):
            dataType = 'object'
            value_expr = (
                '&' + obj['_meta_']['_label_'] +
                '_property_value_' + tag_propname + '.pub')
            value = '(const afw_object_t *)&' + prop['_meta_']['_label_']
        else:
            fd.write('Error>>>\n')
            return

        if not use_shared_scalar:
            fd.write('\nstatic const afw_value_' + dataType + '_t\n' +
                obj['_meta_']['_label_'] + '_property_value_' + tag_propname + ' = {\n')
            fd.write('    {&afw_value_permanent_' + dataType + '_inf},\n')
            fd.write('    ' + value + '\n')
            fd.write('};\n')

        fd.write('\nstatic const afw_runtime_property_t\n')
        fd.write(obj['_meta_']['_label_'] + '_property_' + tag_propname + ' = {\n')
        if '-' in propname:
            fd.write('    ' + v_ + 'a_' + tag_propname + ',\n')
        else:
            fd.write('    ' + v_ + propname + ',\n')

        fd.write('    ' + value_expr + '\n')
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
        write_const_array_of_values(
            fd, options,
            meta.get('_label_') + '_parentPaths_list',
            meta.get('_label_') + '_parentPaths',
            'anyURI', parentPaths)

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

    # Register API only; not public. Name matches install *_internal.h filter.
    filename = prefix + 'const_objects_internal.h'

    afw_package = package.get_afw_package(options)
    copyright = afw_package.get('copyright')

    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_h_prologue(
            fd, generated_by,
            'Adaptive Framework Builtin Objects Header (internal)', copyright,
            filename)
        c.write_doxygen_file_section(
            fd, filename,
            'Internal header for builtin const adaptive object register '
            '(not public C API).')
        fd.write('\n#include "afw_interface.h"\n')
        fd.write('\n\n/**\n')
        fd.write(' * @brief Register ' + prefix + ' const objects (package/core init).\n')
        fd.write(' */\n')
        fd.write('void\n')
        fd.write(prefix + 'const_objects_register(afw_xctx_t *xctx);\n')
        c.write_h_epilogue(fd, filename)

    filename = prefix + 'const_objects.c'
    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_c_prologue(fd, generated_by, 'Adaptive Framework Const', copyright)
        c.write_doxygen_file_section(
            fd, filename,
            'Generated builtin const adaptive objects.')
        fd.write('\n')
        fd.write('#include "afw.h"\n')
        # Core zz__* string permanents (property values, prose) for const objects.
        if options.get('core'):
            fd.write('#include "' + prefix + 'strings_internal.h"\n')
        fd.write('#include "' + prefix + 'generated_internal.h"\n')
        fd.write('\n')


        for obj in list:
            obj['_meta_']['_label_'] = new_label()           
            write_const_c(options, fd, prefix, obj)
        write_object_pointer_list(fd, prefix, list)
