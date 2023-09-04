#! /usr/bin/env python3

import os
import fnmatch
import json
import xml.etree.ElementTree as ET
from _afwdev.generate import c
from _afwdev.common import msg, nfc



# ---- <name>.h ----
def generate_h(generated_by, prefix, name, tree, generated_dir_path):

    root = tree.getroot()
    filename = name + '.h'
    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_h_prologue(fd, generated_by, 'Interface ' + name + ' Header', filename)
        c.write_doxygen_file_section(fd, filename, 'Interface' + name + ' header.')

        fd.write('\n')
        fd.write('#include "afw_interface_common.h"\n')
        fd.write('#include "' + name + '_opaques.h"\n')
        fd.write('#include "' + prefix + 'common_opaques.h"\n')
        fd.write('\n')
        fd.write('AFW_BEGIN_DECLARES\n\n')

        fd.write('/**\n * @addtogroup ' + name + ' Interfaces\n')
        fd.write(' *\n')
        fd.write(' * @{\n')
        fd.write(' */\n\n')

        # c_header_prologue
        for c_header_prologue in root.findall('c_header_prologue'):
            fd.write(c_header_prologue.text)

        # each interface
        for interface in root.findall('interface'):
            interface_name = interface.get('name')

            # addtogroup and description
            fd.write('/**\n * @addtogroup ' + interface_name + '_interface ' +  interface_name + '\n')
            fd.write(' *\n')
            for description in interface.findall('description'):
                lines = [line.strip() for line in description.text.strip().split('\n')]
                text = '\n'.join(('' if not line else line) for line in lines)
                c.write_wrapped(fd, 80, ' * ', text)
            fd.write(' *\n')
            fd.write(' * @{\n')
            fd.write(' */\n\n')

            # public struct
            fd.write('\n/** @brief Interface ' + interface_name + ' public struct. */\n')
            fd.write('struct ' + interface_name + '_s {\n')
            fd.write('    const ' + interface_name + '_inf_t *inf;\n')
            for variable in interface.findall('variable'):
                if variable.findall('description'):
                    fd.write('\n    /**\n')
                    for description in variable.findall('description'):
                        lines = [line.strip() for line in description.text.strip().split('\n')]
                        text = '\n'.join(('' if not line else line) for line in lines)
                        c.write_wrapped(fd, 80, '     * ', text)
                    fd.write('     */\n')
                fd.write('    ' + variable.get('type') + ' ' + variable.get('name') + ';\n')
            for c_public in interface.findall('c_public'):
                fd.write(c_public.text)
            fd.write('};\n')

            # define for name
            fd.write('\n/** @brief define for interface ' + interface_name + ' name. */\n')
            fd.write('#define ' + interface_name.upper() + '_INTERFACE_NAME \\\n"' + interface_name + '"\n' )

            # method typedefs
            for method in interface.findall('method'):
                method_name = method.get('name')
                fd.write('\n/** @sa ' + interface_name + '_' + method_name + '() */\n')
                return_node = method.find('return')
                fd.write('typedef ' + return_node.get('type') + '\n')
                fd.write('(*' + interface_name + '_' + method_name + '_t)(')
                ending = '\n'
                for parameter in method.findall('parameter'):
                    if parameter.get('type') == '...':
                        fd.write(ending + '    ' + parameter.get('type'))
                    else:
                        fd.write(ending + '    ' + parameter.get('type') + ' ' + parameter.get('name'))
                    ending = ',\n'
                fd.write(');\n')

            # interface inf struct
            fd.write('\n/** @brief Interface ' + interface_name + '_inf_s struct. */\n')
            fd.write('struct ' + interface_name + '_inf_s {\n')
            fd.write('    afw_interface_implementation_rti_t rti;\n')
            for method in interface.findall('method'):
                method_name = method.get('name')
                fd.write('    ' + interface_name + '_' + method_name + '_t ' + method_name + ';\n')
            for variable in interface.findall('inf_variable'):
                if variable.findall('description'):
                    fd.write('\n    /**\n')
                    for description in variable.findall('description'):
                        lines = [line.strip() for line in description.text.strip().split('\n')]
                        text = '\n'.join(('' if not line else line) for line in lines)
                        c.write_wrapped(fd, 80, '     * ', text)
                    fd.write('     */\n')
                fd.write('    ' + variable.get('type') + ' ' + variable.get('name') + ';\n')
            fd.write('};\n')

            # method helpers
            for method in interface.findall('method'):
                method_name = method.get('name')

                fd.write('\n/**\n')
                c.write_wrapped(fd, 80, ' * ',
                    '@brief Call method ' + method_name + ' of interface ' + interface_name,
                    '    ')

                for parameter in method.findall('parameter'):
                    param = '@param ' + parameter.get('name')
                    if parameter.get('type') == '...':
                        param = '@param ...'
                    for description in parameter.findall('description'):
                        if description.text is not None:
                            lines = [line.strip() for line in description.text.strip().split('\n')]
                            param += '\n'.join(('' if not line else line) for line in lines)
                    c.write_wrapped(fd, 80, ' * ', param, '    ')

                fd.write(' */\n')

                fd.write('#define ' + interface_name + '_' + method_name + '(')

                prev = ''
                for parameter in method.findall('parameter'):
                    if parameter.get('type') == '...':
                        fd.write('    ...')
                        prev = ''
                    else:
                        if prev != '':
                            fd.write('    ' +  prev + ', \\\n')
                        else:
                            fd.write(' \\\n')
                        prev = parameter.get('name')
                if prev != '':
                    fd.write('    ' +  prev + ' \\\n')

                fd.write(') \\\n')
                fd.write('(instance)->inf->' + method_name + '(')

                prev = ''
                for parameter in method.findall('parameter'):
                    if parameter.get('type') == '...':
                        fd.write('    __VA_ARGS__')
                        prev = ''
                    else:
                        if prev != '':
                            fd.write('    (' +  prev + '), \\\n')
                        else:
                            fd.write(' \\\n')
                        prev = parameter.get('name')
                if prev != '':
                    fd.write('    (' +  prev + ') \\\n')

                fd.write(')\n')

            fd.write('\n/** @} */\n\n')

        # c_header_epilogue
        for c_header_epilogue in root.findall('c_header_epilogue'):
            fd.write(c_header_epilogue.text)

        fd.write('\n/** @} */\n\n')
        fd.write('AFW_END_DECLARES\n\n')

        c.write_h_epilogue(fd, filename)

# ---- <name>_opaques.h ----
def generate_opaques_h(generated_by, prefix, name, tree, generated_dir_path):

    root = tree.getroot()
    filename = name + '_opaques.h'
    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_h_prologue(fd, generated_by, 'Interface ' + name + ' Opaque Typedefs', filename)
        c.write_doxygen_file_section(fd, filename, 'Interface ' + name + ' opaque typedefs.')

        fd.write('/**\n * @addtogroup ' + name + ' Interfaces\n')
        fd.write(' *\n')
        fd.write(' * @{\n')
        fd.write(' */\n\n')

        # each interface
        for interface in root.findall('interface'):
            interface_name = interface.get('name')

            fd.write('\ntypedef struct ' + interface_name + '_s\n')
            fd.write(interface_name + '_t;\n')

            fd.write('\ntypedef struct ' + interface_name + '_inf_s\n')
            fd.write(interface_name + '_inf_t;\n')

        fd.write('\n/** @} */\n\n')

        c.write_h_epilogue(fd, filename)

# ---- <interface>_impl_declares.h ----
def generate_impl_declares_hs(generated_by, prefix, name, tree, generated_dir_path):
    root = tree.getroot()
    for interface in root.findall('interface'):
        interface_name = interface.get('name')

        filename = interface_name + '_impl_declares.h'
        msg.info('Generating ' + filename)
        with nfc.open(generated_dir_path + filename, mode='w') as fd:
            guard = ('__' + filename + '__').upper().replace('.', '_')
            inf_only = (interface_name + '_INF_ONLY').upper().replace('.', '_')
            fd.write(c.get_copyright('Interface ' + name + ' Implementation Declares'))

            fd.write('\n')
            fd.write('#include "' + name + '_opaques.h"\n')
            fd.write('\n')
            fd.write('AFW_BEGIN_DECLARES\n\n')

            fd.write('/**\n * @addtogroup ' + name + ' Interfaces\n')
            fd.write(' *\n')
            fd.write(' * @{\n')
            fd.write(' */\n\n')

            fd.write('/**\n * @addtogroup ' + interface_name + '_interface ' +  interface_name + '\n')
            fd.write(' * @{\n')
            fd.write(' */\n\n')

            # documentation for *_impl_declares.h
            fd.write('/**\n * @addtogroup ' + interface_name + '_impl_declares ' +  interface_name + '\n')
            fd.write(' * @{\n')
            fd.write(' *\n')
            c.write_wrapped(fd, 80, ' * ',
                'This file should only be included in c source files that implement interface '  + interface_name +  '.\n',
                '')
            fd.write(' *\n')
            c.write_wrapped(fd, 80, ' * ',
                'You can find a skeleton for using this #include file in the corresponding ' +
                'source directory in file generated/interface_closet/skeleton_' +
                interface_name +  '.c.\n',
                '')
            fd.write(' *\n')
            fd.write(' * Mutiple *_impl.declares.h can be included in each c source file, but only\n')
            fd.write(' * one for each interface name.\n')
            fd.write(' *\n')
            c.write_wrapped(fd, 80, ' * ',
                'After the #include for  '  + interface_name +  '_impl_declares.h:\n',
                '   ')
            fd.write(' *\n')
            c.write_wrapped(fd, 80, ' * ',
                '1. declarations will exist for all of the interfaces methods with names of impl_'  + interface_name +  '_ followed by the method name.\n',
                '   ')
            c.write_wrapped(fd, 80, ' * ',
                '2. rti will be defined with a name of impl_'  + interface_name +  '_rti.\n',
                '   ')
            c.write_wrapped(fd, 80, ' * ',
                '3. inf will be defined with a name of impl_'  + interface_name +  '_inf.\n',
                '   ')
            fd.write(' *\n')
            fd.write(' * Before including, define the following symbols:\n')
            fd.write(' *\n')
            fd.write(' *   AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.\n')
            fd.write(' *\n')
            fd.write(' *   AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.\n')
            fd.write(' *\n')
            fd.write(' *   AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to \'impl_' + interface_name + '_inf\'.\n')
            fd.write(' *\n')
            interface_self_t = interface_name.upper() + '_SELF_T'
            fd.write(' *   ' + interface_self_t + ' - (optional) defaults to \'const ' + interface_name + '_t\'.\n')
            fd.write(' *       The const is not required and normally should not be specified. It is\n')
            fd.write(' *       the default for historical reasons.\n')
            fd.write(' *\n')
            if interface.find('inf_variable') is not None:
                fd.write(' * - AFW_IMPLEMENTATION_INF_VARIABLES - (optional) this will be added at the end of\n')
                fd.write(' *       the inf initializer and should represent the inf_variable\n')
                fd.write(' *       defined in interface.\n')
                fd.write(' *\n')
            fd.write(' * Example:\n')
            fd.write(' *~~~~~~~~~~~~~~~{.c}\n')
            fd.write(' *\n')
            fd.write(' *   static const sometype_t impl_myimpl_stuff =  {\n')
            fd.write(' *       ...\n')
            fd.write(' *   };\n')
            fd.write(' *\n')
            fd.write(' *   #define AFW_IMPLEMENTATION_ID "myimpl"\n')
            fd.write(' *   #define AFW_IMPLEMENTATION_SPECIFIC &impl_myimpl_stuff\n')
            fd.write(' *   #include "' + interface_name + '_impl_declares.h"\n')
            fd.write(' *\n')
            fd.write(' * A second #include "' + filename + '" can be specified if\n"')
            fd.write(' *   #define "' + inf_only + '" is specified to generate inf only."\n')

            fd.write(' *~~~~~~~~~~~~~~~\n')
            fd.write(' */\n\n')

            # \file
            c.write_doxygen_file_section(fd, filename, 'Interface ' + name + ' implementation declares.')

            # Make sure only included once
            fd.write('\n')
            fd.write('#ifdef ' + guard + '\n')
            fd.write('#ifndef ' + inf_only + '\n')
            fd.write('#error "' + interface_name + '_impl_declares.h already included."\n')
            fd.write('#endif\n')
            fd.write('#endif\n')
            fd.write('#ifndef ' + guard + '\n')
            fd.write('#ifdef ' + inf_only + '\n')
            fd.write('#error "' + interface_name + '_impl_declares.h must be included before ' + inf_only + '."\n')
            fd.write('#endif\n')
            fd.write('#define ' + guard + ' 1\n')
            fd.write('#endif\n')

            # _AFW_IMPLEMENTATION_ID_
            fd.write('\n')
            fd.write('#ifdef AFW_IMPLEMENTATION_ID\n')
            fd.write('#define _AFW_IMPLEMENTATION_ID_ AFW_IMPLEMENTATION_ID\n')
            fd.write('#else\n')
            fd.write('#define _AFW_IMPLEMENTATION_ID_ "unspecified"\n')
            fd.write('#endif\n')
            fd.write('#ifdef AFW_IMPLEMENTATION_SPECIFIC\n')
            fd.write('#define _AFW_IMPLEMENTATION_SPECIFIC_ AFW_IMPLEMENTATION_SPECIFIC\n')
            fd.write('#else\n')
            fd.write('#define _AFW_IMPLEMENTATION_SPECIFIC_ NULL\n')
            fd.write('#endif\n')

            # AFW_IMPLEMENTATION_SELF_T

            fd.write('\n')
            fd.write('#ifndef ' + interface_self_t + '\n')
            fd.write('#define ' + interface_self_t + ' const ' + interface_name + '_t\n')
            fd.write('#endif\n')           

            # method declarations
            fd.write('\n')
            fd.write('#ifndef ' + inf_only + '\n')
            for method in interface.findall('method'):
                method_name = method.get('name')
                method_function = 'impl_' + interface_name + '_' + method_name
                fd.write('\n#ifndef ' + method_function + '\n')
                fd.write('/* Declare method ' + method_name + ' */\n')
                return_node = method.find('return')
                fd.write('AFW_DECLARE_STATIC(' + return_node.get('type') + ')\n')
                fd.write(method_function + '(\n')
                ending = ''
                first = True
                for parameter in method.findall('parameter'):
                    if first:
                        first_parameter_type = interface_name + '_t *'
                        if (interface.get('self_const', 'true') != 'false'):
                            first_parameter_type = 'const ' + first_parameter_type
                        if parameter.get('type') != first_parameter_type:
                            msg.error_exit('The first parameter of ' + interface_name + ' method ' + method_name + ' must be type="' + first_parameter_type + '"')
                            fd.write(ending + '    ' + parameter.get('type'))
                        fd.write(ending + '    ' + interface_self_t + ' *self')
                        first = False
                    else:
                        if parameter.get('type') == '...':
                            fd.write(ending + '    ' + parameter.get('type'))
                        else:
                            fd.write(ending + '    ' + parameter.get('type') + ' ' + parameter.get('name'))
                    ending = ',\n'
                fd.write(');\n')
                fd.write('#endif\n')
            fd.write('#endif\n')

            # inf
            fd.write('\n')
            fd.write('/* inf for interface '  + interface_name +  ' */\n')
            fd.write('#ifdef __BASE_FILE__\n')
            fd.write('#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __BASE_FILE__\n')
            fd.write('#else\n')
            fd.write('#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __FILE__\n')
            fd.write('#endif\n')
            fd.write('#ifdef AFW_IMPLEMENTATION_INF_SPECIFIER\n')
            fd.write('AFW_IMPLEMENTATION_INF_SPECIFIER(' + interface_name + '_inf_t)\n')
            fd.write('#else\n')
            fd.write('static const ' + interface_name + '_inf_t\n')
            fd.write('#endif\n')
            fd.write('#ifdef AFW_IMPLEMENTATION_INF_LABEL\n')
            fd.write('AFW_IMPLEMENTATION_INF_LABEL = {\n')
            fd.write('#else\n')
            fd.write('impl_' + interface_name + '_inf = {\n')
            fd.write('#endif\n')
            fd.write('    {\n')
            fd.write('        AFW_UTF8_LITERAL("' + interface_name + '"),\n')
            fd.write('        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_SOURCE_FILE_),\n')
            fd.write('        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_ID_),\n')
            fd.write('        _AFW_IMPLEMENTATION_SPECIFIC_\n')
            fd.write('    },\n')

            ending = ''
            for method in interface.findall('method'):
                fd.write(ending + '    (' + interface_name + '_' + method.get('name') + '_t)\n')
                fd.write('    impl_' + interface_name + '_' + method.get('name'))
                ending = ',\n'
            if interface.find('inf_variable') is not None:
                fd.write("\n#ifdef AFW_IMPLEMENTATION_INF_VARIABLES")
                fd.write('\n    ,AFW_IMPLEMENTATION_INF_VARIABLES\n')
                fd.write("#endif\n")
            fd.write('\n};\n')

            # undefine _AFW_IMPLEMENTATION_ID_ and _AFW_IMPLEMENTATION_SPECIFIC_
            fd.write('\n')
            fd.write('#undef _AFW_IMPLEMENTATION_ID_\n')
            fd.write('#undef _AFW_IMPLEMENTATION_SPECIFIC_\n')
            fd.write('#undef _AFW_IMPLEMENTATION_SOURCE_FILE_\n')
          
            # close doxygen groups
            fd.write('\n/** @} */\n')           
            fd.write('\n/** @} */\n')           
            fd.write('\n/** @} */\n')


def generate_skeletons_hs(generated_by, prefix, name, tree, generated_dir_path):
    root = tree.getroot()
    for interface in root.findall('interface'):
        interface_name = interface.get('name')

        filename = 'interface_closet/skeleton_' + interface_name + '.h'
        msg.info('Generating ' + filename)
        with nfc.open(generated_dir_path + filename, mode='w') as fd:

            fd.write(c.get_copyright('Interface ' + interface_name + ' implementation for <afwdev {implementation_id}>'))
           
            fd.write('\n')
            guard = ("__<afwdev {prefixed_interface_name.upper().replace('.', '_')}>_H__")
            fd.write('#ifndef ' + guard + '\n')
            fd.write('#define ' + guard + '\n')

            fd.write('\n')
            fd.write('/**\n')
            fd.write(' * @file <afwdev {prefixed_interface_name}>.h\n')
            fd.write(' * @brief <afwdev {brief}>\n')
            fd.write(' *\n')
            fd.write(' * This is the implementation of ' + interface_name + ' for <afwdev {implementation_id}>.\n')
            fd.write(' */\n')
            fd.write('\n')
 
            fd.write('#include "afw_interface.h"\n')
            fd.write('#include "generated/<afwdev {srcdir_prefix}>declare_helpers.h"\n')

            fd.write('\n')
            fd.write('AFW_BEGIN_DECLARES\n')
            fd.write('\n')

            interface_self_s = '<afwdev {prefixed_interface_name}>_self_s'
            interface_self_t = '<afwdev {prefixed_interface_name}>_self_t'

            # self struct
            fd.write('/**\n')
            fd.write(' * @brief Self typedef for <afwdev {implementation_id}> implementation of ' + interface_name + '.\n')
            fd.write(' */\n')
            fd.write('typedef struct\n')
            fd.write(interface_self_s + ' {\n')
            fd.write('    ' + interface_name + '_t pub;\n')
            fd.write('\n')
            fd.write('    /** @todo Add private implementation variables */\n')
            fd.write('\n')
            fd.write('} ' + interface_self_t + ';\n')
            fd.write('\n')

            model_c_code = interface.find("model_c_code[@label='create_h']")
            if model_c_code is not None:
                if model_c_code.text is not None:
                    fd.write(model_c_code.text)
            elif interface_name.endswith('_factory'):
                fd.write('/**\n')
                fd.write(' * @brief Singleton instance of ' + interface_name + ' for <afwdev {implementation_id}>.\n')
                fd.write(' */\n')
                fd.write('<afwdev {srcdir_prefix.upper()}>DECLARE_CONST_DATA(' + interface_name + '_t)\n')
                fd.write('<afwdev {prefixed_interface_name}>;\n')
                fd.write('\n')
                fd.write('/**\n')
                fd.write(' * @brief Get singleton instance of ' + interface_name + ' for <afwdev {implementation_id}>.\n')   
                fd.write(' */\n')
                fd.write('<afwdev {srcdir_prefix.upper()}>DECLARE(const ' + interface_name + '_t *)\n')
                fd.write('<afwdev {prefixed_interface_name}>_get(\n')
                fd.write('    const afw_object_t *parameters,\n')
                fd.write('    afw_xctx_t *xctx);\n')
                fd.write('\n')
            elif interface.get('create_function') is not None:
                create_function_name = '<afwdev {prefixed_interface_name}>' + '_' + interface.get('create_function')
                fd.write('/**\n')
                fd.write(' * @brief Create for <afwdev {implementation_id}> implementation of ' + interface_name + '\n')
                fd.write(' * @param xctx\n')
                fd.write(' * @param p used to create ' + interface_name + '.\n')
                fd.write(' * @return new instance of ' + interface_name + '\n')
                fd.write(' */\n')
                fd.write('<afwdev {srcdir_prefix.upper()}>DECLARE(const ' + interface_name + '_t *)\n')
                fd.write(create_function_name + '(\n')
                fd.write('    const afw_object_t *parameters,\n')
                fd.write('    const afw_pool_t *p,\n')
                fd.write('    afw_xctx_t *xctx);\n')
                fd.write('\n')

            fd.write('AFW_END_DECLARES\n')

            fd.write('\n#endif /* ' + guard + ' */\n')


# ---- interface_skeletons/<interface>.c ----
def generate_skeletons_cs(generated_by, prefix, name, tree, generated_dir_path):
    root = tree.getroot()
    for interface in root.findall('interface'):
        interface_name = interface.get('name')

        filename = 'interface_closet/skeleton_' + interface_name + '.c'
        msg.info('Generating ' + filename)
        with nfc.open(generated_dir_path + filename, mode='w') as fd:

            fd.write('<afwdev {license_c}>\n')

            fd.write('\n/**\n')
            fd.write(' * @file <afwdev {prefixed_interface_name}>.c\n')
            fd.write(' * @brief <afwdev {brief}>\n')
            fd.write(' * @todo Search this file and its header for @todo and make appropriate changes.\n')
            fd.write(' *\n')
            fd.write(' * This is the implementation of ' + interface_name + ' for <afwdev {implementation_id}>.\n')
            fd.write(' */\n')

            fd.write('\n')
            fd.write('#include "afw.h"\n')
            fd.write('#include "<afwdev {prefixed_interface_name}>.h"\n')
            fd.write('\n')
            fd.write('/* Declares and rti/inf defines for interface ' + interface_name + ' */\n')
            fd.write('#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"\n')
            interface_self_t = interface_name.upper() + '_SELF_T'
            fd.write('/* Change this to the name of the self typedef for this implementation */\n')
            fd.write('#define ' + interface_self_t + ' <afwdev {prefixed_interface_name}>_self_t\n')
            fd.write('#include "' + interface_name + '_impl_declares.h"\n')

            # optional before function defines
            model_c_code = interface.find("model_c_code[@label='more_includes']")
            if model_c_code is not None:
                if model_c_code.text is not None:
                    fd.write(model_c_code.text)
 
            # create function
            model_c_code = interface.find("model_c_code[@label='create']")
            create_function_name = '<afwdev {prefixed_interface_name}>_create'
            if interface.get('create_suffix') is not None:
                create_function_name += interface.get('create_suffix')
            if model_c_code is not None:
                if model_c_code.text is not None:
                    fd.write(model_c_code.text)
            elif interface_name.endswith('_factory'):
                fd.write('\n')
                fd.write('static const afw_utf8_t\n')
                fd.write('impl_<afwdev {prefixed_interface_name}>_description =\n')
                fd.write('    AFW_UTF8_LITERAL("<afwdev {description}>.");\n')
                fd.write('\n')
                fd.write('/* Singleton instance of ' + interface_name + ' for <afwdev {implementation_id}> */\n')
                fd.write('<afwdev {srcdir_prefix.upper()}>DEFINE_CONST_DATA(' + interface_name + '_t)\n')
                fd.write('<afwdev {prefixed_interface_name}> =\n')
                fd.write('{\n')
                fd.write('    &impl_' + interface_name + '_inf,\n')
                fd.write('    AFW_UTF8_LITERAL("<afwdev {implementation_id}>"),\n')
                fd.write('    &impl_<afwdev {prefixed_interface_name}>_description\n')
                fd.write('};\n')
                fd.write('\n')
                fd.write('/* Get singleton instance of ' + interface_name + ' for <afwdev {implementation_id}> */\n')
                fd.write('<afwdev {srcdir_prefix.upper()}>DEFINE(const ' + interface_name + '_t *)\n')
                fd.write('<afwdev {prefix}>get_' + interface_name + '(\n')
                fd.write('    const afw_object_t *parameters,\n')
                fd.write('    afw_xctx_t *xctx)\n')
                fd.write('{\n')
                fd.write('    return &<afwdev {prefixed_interface_name}>;\n')
                fd.write('}\n')
                fd.write('\n')
            elif interface.get('create_function') is not None:
                create_function_name = '<afwdev {prefixed_interface_name}>_' + interface.get('create_function')
                fd.write('\n')
                fd.write('/* Create for <afwdev {implementation_id}> implementation of ' + interface_name + ' */\n')
                fd.write('<afwdev {srcdir_prefix.upper()}>DEFINE(const ' + interface_name + '_t *)\n')
                fd.write(create_function_name + '(\n')
                fd.write('    const afw_object_t *parameters,\n')
                fd.write('    const afw_pool_t *p,\n')
                fd.write('    afw_xctx_t *xctx)\n')
                fd.write('{\n')
                fd.write('    <afwdev {prefixed_interface_name}>_self_t *self;\n')
                fd.write('\n')
                fd.write('    /* Using a subpool of the p passed, allocate and initialize self. */\n')
                fd.write('    p = afw_pool_create(p, xctx);\n')
                fd.write('    self = afw_pool_calloc_type(p,' + interface_self_t + ', xctx);\n')
                fd.write('    self->pub.inf = &impl_' + interface_name + '_inf;\n')
                fd.write('    self->pub.p = p;\n')
                fd.write('\n')
                fd.write('    /** @todo Finish processing parameters and initializing new instance. */\n')
                fd.write('\n')
                fd.write('    /* Return new instance. */\n')
                fd.write('    return (' + interface_name + '_t *)self;\n')
                fd.write('}\n\n')

            # optional before function defines
            model_c_code = interface.find("model_c_code[@label='before_function_defines']")
            if model_c_code is not None:
                if model_c_code.text is not None:
                    fd.write(model_c_code.text)

            # method definitions
            for method in interface.findall('method'):
                method_name = method.get('name')
                fd.write('\n')
                fd.write('/*\n')
                c.write_wrapped(fd, 80, ' * ',
                    'Implementation of method ' + method_name +
                    ' for interface ' + interface_name + '.\n',
                    '')
                fd.write(' */\n')
                return_node = method.find('return')
                fd.write(return_node.get('type') + '\n')
                fd.write('impl_' + interface_name + '_' + method_name + '(\n')
                ending = ''
                first = True
                for parameter in method.findall('parameter'):
                    if first:
                        first = False
                        fd.write('    ' + interface_self_t + ' *self')
                    elif parameter.get('type') == '...':
                        fd.write(ending + '    ' + parameter.get('type'))
                    else:
                        fd.write(ending + '    ' + parameter.get('type') + ' ' + parameter.get('name'))
                    ending = ',\n'
                fd.write(')\n')
                fd.write('{\n')


                # Get optional method code
                model_c_body_code = method.find("model_c_code[@label='method_body']")

                # method body
                if model_c_body_code is not None:
                    if model_c_body_code.text is not None:
                        fd.write(model_c_body_code.text)
                        if not model_c_body_code.text.endswith('\n'):
                            fd.write('\n')
                else:
                    fd.write('    /** @todo Add code to implement method. */\n')
                    fd.write('    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);\n')

                fd.write('}\n')


# ---- skeleton_header.h ----
def generate_skeleton_header(generated_by, prefix, generated_dir_path):

    filename = 'interface_closet/skeleton_header.h'
    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:

        fd.write('#ifdef __@todo_H__\n')
        fd.write('#define __@todo_H__\n\n')

        fd.write(c.get_copyright('@todo title of header'))

        c.write_doxygen_file_section(fd, '@todo.h', '@todo brief of header.')

        fd.write('\nAFW_BEGIN_DECLARES\n')

        fd.write('\n/**\n * @addtogroup @todo @todo\n')
        fd.write(' *\n')
        fd.write(' * @{\n')
        fd.write(' */\n\n')

        fd.write('/** @todo Add to header. */\n')

        fd.write('\n/** @} */\n\n')

        fd.write('AFW_END_DECLARES\n')

        fd.write('\n#endif __@todo_H__\n')


def add_object_strings(obj, strings):
    for name in obj.keys():
        strings[name] = name

        if isinstance(obj[name], dict):
            add_object_strings(obj[name], strings)

        elif isinstance(obj[name], str):
            if name == 'dataType':
                strings[obj[name]] = obj[name]

def generate_objects(generated_by, prefix, name, tree, generated_dir_path):

    root = tree.getroot()
    for interface in root.findall('interface'):
        interface_name = interface.get('name')

        filename = 'objects/_AdaptiveInterface_/' + interface_name + '.json'
        msg.info('Generating ' + filename)
        with nfc.open(generated_dir_path + filename, mode='w') as fd:

            fd.write('{\n')                        
            fd.write('    "name": "' + interface_name + '",\n')            

            descriptions = interface.findall('description')
            if len(descriptions) > 0:
                for description in interface.findall('description'):    
                    if description.text is not None:            
                        fd.write('    "description": ' + c.make_quoted(description.text.strip()) + '\n')
                    else:
                        fd.write('    "description": ""\n')
            else:
                fd.write('    "description": ""\n')

            fd.write('}')
    


def generate(generated_by, prefix, interfaces_dir_path, generated_dir_path):

    # Make sure generated/ directory structure exists
    os.makedirs(generated_dir_path, exist_ok=True)
    os.makedirs(generated_dir_path + "interface_closet/", exist_ok=True)
    os.makedirs(generated_dir_path + 'objects/_AdaptiveInterface_', exist_ok=True)

    # Process *.xml interface files in optional interfaces dir.
    if interfaces_dir_path is not None and os.path.exists(interfaces_dir_path):
        for file in sorted(os.listdir(interfaces_dir_path)):
            if fnmatch.fnmatch(file, '*.xml'):
                tree = ET.parse(interfaces_dir_path + file)
                name = file[:-4]
                generate_h(generated_by, prefix, name, tree, generated_dir_path)
                generate_opaques_h(generated_by, prefix, name, tree, generated_dir_path)
                generate_impl_declares_hs(generated_by, prefix, name, tree, generated_dir_path)
                generate_skeletons_hs(generated_by, prefix, name, tree, generated_dir_path)
                generate_skeletons_cs(generated_by, prefix, name, tree, generated_dir_path)
                generate_skeleton_header(generated_by, prefix, generated_dir_path)
                generate_objects(generated_by, prefix, name, tree, generated_dir_path)
