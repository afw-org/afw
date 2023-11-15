#! /usr/bin/env python3

##
# @file runtime_object_maps.py
# @ingroup afwdev_generate
#

from _afwdev.common import direct
import os

from _afwdev.common import msg, nfc
from _afwdev.generate import c

def sort_use_id_cb(obj):
    return obj['_meta_']['objectId']

def write_h_map(fd, prefix, obj, options):
    id = obj['_meta_']['objectId']
    runtime = obj.get('runtime')

    fd.write('\n/**\n')
    fd.write(' * @brief Runtime object inf for object type ' + id + '\n')
    fd.write(' */\n')
    fd.write('AFW_DECLARE_CONST_DATA(afw_object_inf_t)\n')
    fd.write(prefix + 'runtime_inf_' + id + ';\n')
    

def write_c_map(fd, prefix, obj, options, onGetValueCFunctionNames):

    id = obj['_meta_']['objectId']
    s_ = '&' + prefix + 'self_s_'

    typedef = None
    runtime = obj.get('runtime')
    if runtime is not None:
        typedef = runtime.get('typedef')
        labels = runtime.get('labels')
        if labels is None:
            labels = {'objectId':'objectId'}

    if typedef is None:
        typedef = 'afw_runtime_const_object_instance_t'
        labels = {'objectId':'object_id',
                    'indirectObjectId': True,
                    'path':'path',
                    'properties':'properties',
                    'combined': True,
                    'embeddedInheritance' : True}

    fd.write('\n\n/* Runtime object map properties for ' + id + ' objects. */\n')

    if runtime is not None:
        fd.write('\nstatic const afw_runtime_object_map_property_t\n')
        fd.write('impl_properties_' + id + '[] = {\n')
        propertyTypes = obj.get('propertyTypes', {})
        propertiesNames = sorted(list(iter(propertyTypes)))
        if '_meta_' in propertiesNames:
            propertiesNames.remove('_meta_')
        remaining = len(propertiesNames)
        for name in sorted(propertiesNames):
            propertyType = propertyTypes[name]
            propertyType_runtime = propertyType.get('runtime', {})
            memberName = propertyType_runtime.get('memberName', name)
            fd.write('    {\n')
            fd.write('        ' + s_ +  name + ',\n')

            # offset or -1 if onGetValueCFunctionName specified
            if propertyType_runtime.get("onGetValueCFunctionName") is not None:
                fd.write('        -1,\n')
                onGetValueCFunctionNames.append(propertyType_runtime.get("onGetValueCFunctionName"))
            elif propertyType_runtime.get('zeroOffset', False) == True:
                fd.write('        0,\n')
            else:
                fd.write('        offsetof(' + typedef + ', ' + memberName + '),\n')

            # count_offset
            count_offset = propertyType_runtime.get('count_offset')
            if count_offset is not None:
                fd.write('        offsetof(' + typedef + ', ' + count_offset + '),\n')
            else:
                fd.write('        -1,\n')

            if propertyType.get('dataType') is None:
                fd.write('        { NULL },\n')
            else:
                if options['core']:
                    fd.write('        { &afw_data_type_' + propertyType.get('dataType') + '_direct },\n')
                else:
                    fd.write('        .unresolved_data_type_id = ' + s_ +  propertyType.get('dataType') + ',\n')

            # dataType and dataTypeParameter
            dataTypeParameter = propertyType.get('dataTypeParameter', '')
            if propertyType.get('dataType','') == 'array' and dataTypeParameter.startswith('object'):
                fd.write('        AFW_UTF8_LITERAL("' + dataTypeParameter[6:].strip() + '"),\n')
            else:
                fd.write('        AFW_UTF8_LITERAL("' + dataTypeParameter + '"),\n')

            if propertyType.get('dataTypeParameter') is not None and propertyType.get('dataType','') == 'array':
                dataTypeParameter = propertyType.get('dataTypeParameter').split()[0]
                if options['core']:
                    fd.write('        { &afw_data_type_' + dataTypeParameter + '_direct },\n')
                else:
                    fd.write('        .unresolved_data_type_parameter_data_type_id = ' + s_ +  dataTypeParameter + ',\n')
            else:
                fd.write('        { NULL },\n')

            # valueAccessor or onGetValueCFunctionName
            if propertyType_runtime.get("onGetValueCFunctionName") is not None:
                fd.write('        NULL,\n')
                fd.write('        ' + propertyType_runtime.get("onGetValueCFunctionName") + '\n')
            else:
                valueAccessor = propertyType_runtime.get("valueAccessor", "default")
                fd.write('        ' + s_ + valueAccessor + ',\n')
                if options['core']:
                    fd.write('        afw_runtime_value_accessor_' + valueAccessor + '\n')
                else:
                    fd.write('        NULL\n')

            fd.write('    }')
            remaining -= 1
            if remaining > 0:
                fd.write(',')
            fd.write('\n')
        fd.write('};\n')

        fd.write('\nstatic const afw_runtime_object_map_t\n')
        fd.write('impl_runtime_object_map_' + id + ' = {\n')
        fd.write('    ' + s_ + obj['_meta_']['objectId'] + ',\n')
        fd.write('    ' + str(len(propertyTypes.keys())) + ',\n')
        fd.write('    &impl_properties_' + id + '[0]\n')
        fd.write('};\n')

    # Begin of afw_runtime_object_type_meta_t
    fd.write('\nstatic const afw_runtime_object_type_meta_t\n')
    fd.write('impl_runtime_meta_' + id + ' = {\n')

    # object_type_id
    fd.write('    ' + s_ + id + ',\n')

    # property_map
    if runtime is not None:
        fd.write('    &' + 'impl_runtime_object_map_' + id + ',\n')
    else:
        fd.write('    NULL,\n')

    # properties_offset
    if labels.get('properties') is not None:
        fd.write('    offsetof(' + typedef + ', ' + labels.get('properties') + '),\n')
    else:
        fd.write('    -1,\n')

    # indirect
    if runtime is not None and runtime.get('indirect', False) == True:
        fd.write('    true,\n')
    else:
        fd.write('    false,\n')

    # end of afw_runtime_object_type_meta_t.
    fd.write('};\n')

    if options['core']:
        fd.write('\nAFW_RUNTIME_OBJECT_INF( \n')
        fd.write('    ' + prefix + 'runtime_inf_' + id + ', \n')
        fd.write('    impl_runtime_meta_' + id + ');\n')
    else:
        fd.write('\nAFW_RUNTIME_OBJECT_RTI( \n')
        fd.write('    impl_runtime_rti_' + id + ',\n')
        fd.write('    impl_runtime_meta_' + id + ');\n')


def generate(generated_by, options):

    # Make sure generated/ directory structure exists
    os.makedirs(options['generated_dir_path'], exist_ok=True)

    # Get all object types and sort by id.
    list = []
    if os.path.exists(options['objects_dir_path'] + '_AdaptiveObjectType_/'):
        list = direct.retrieve_objects_direct(options['objects_dir_path'] + '_AdaptiveObjectType_/')
        list.sort(key=sort_use_id_cb)

    #list = [obj for obj in list if obj.get('runtime') is not None]

    filename = options['prefix'] + 'runtime_object_maps.c'
    msg.info('Generating ' + filename)
    onGetValueCFunctionNames = []
    with nfc.open(options['generated_dir_path'] + filename, mode='w') as fd:
        c.write_c_prologue(fd, generated_by, 'Adaptive Framework Runtime Object Mapping')
        c.write_doxygen_file_section(fd, filename, 'Adaptive Framework runtime object mapping.')
        fd.write('\n')
        fd.write('#include "afw.h"\n')
        fd.write('#include "' + options['prefix'] + 'generated.h"\n')
        if options.get('additional_includes_runtime_object_maps') is not None:
            fd.write(options['additional_includes_runtime_object_maps'])
        fd.write('\n')
        for obj in list:
            write_c_map(fd, options['prefix'], obj, options, onGetValueCFunctionNames);

        if options['core']:
            fd.write('\n/* NULL terminated list of inf pointers. */\n')
            fd.write('static const afw_object_inf_t * impl_inf[] = {\n')
            for obj in list:
                fd.write('    &afw_runtime_inf_' + obj['_meta_']['objectId'] + ',\n')
            fd.write('    NULL\n')
            fd.write('};\n')
            fd.write('\n/* Register (' + options['prefix'] + ') runtime object maps. */\n')
            fd.write('void ' + options['prefix'] + 'register_runtime_object_maps(\n')
            fd.write('    afw_xctx_t *xctx)\n')
            fd.write('{\n')
            fd.write('    afw_runtime_register_object_map_infs(\n')
            fd.write('        &impl_inf[0], \n')
            fd.write('        xctx);\n')
            fd.write('}\n')
        else:
            fd.write('\n/* NULL terminated list of rti pointers. */\n')
            fd.write('static const afw_interface_implementation_rti_t * impl_rti[] = {\n')
            for obj in list:
                fd.write('    &impl_runtime_rti_' + obj['_meta_']['objectId'] + ',\n')
            fd.write('    NULL\n')
            fd.write('};\n')
            fd.write('\n/* Register (' + options['prefix'] + ') runtime object maps. */\n')
            fd.write('void ' + options['prefix'] + 'register_runtime_object_maps(\n')
            fd.write('    afw_xctx_t *xctx)\n')
            fd.write('{\n')
            fd.write('    afw_runtime_resolve_and_register_object_map_infs(\n')
            fd.write('        &impl_rti[0], \n')
            fd.write('        xctx);\n')
            fd.write('}\n')

    filename = options['prefix'] + 'runtime_object_maps.h'
    msg.info('Generating ' + filename)
    with nfc.open(options['generated_dir_path'] + filename, mode='w') as fd:
        c.write_h_prologue(fd, generated_by, 'Adaptive Framework Runtime Object Mapping Header', filename)
        c.write_doxygen_file_section(fd, filename, 'Adaptive Framework runtime object mapping header.')
        fd.write('\n')
        for obj in list:
            write_h_map(fd, options['prefix'], obj, options);
        fd.write('\n/**\n')
        fd.write(' * @brief Register (' + options['prefix'] + ') runtime object maps.\n')
        fd.write(' * @param xctx of caller.  This should be base xctx.\n')
        fd.write(' */\n')
        fd.write('void ' + options['prefix'] + 'register_runtime_object_maps(\n')
        fd.write('    afw_xctx_t *xctx);\n')

        if len(onGetValueCFunctionNames) > 0:
            sortedOnGetValueCFunctionName = sorted(set(onGetValueCFunctionNames))
            for name in sortedOnGetValueCFunctionName:
                fd.write('\n/* @brief Value accessor ' + name + '. */\n')
                fd.write(options['prefix'].upper() + "DECLARE_INTERNAL(const afw_value_t *)\n")
                fd.write(name + '(\n')
                fd.write('    const afw_runtime_object_map_property_t *prop,\n')
                fd.write('    const void *internal,\n')
                fd.write('    const afw_pool_t *p,\n')
                fd.write('    afw_xctx_t *xctx);\n')
            fd.write('\n')

        c.write_h_epilogue(fd, filename)



 




