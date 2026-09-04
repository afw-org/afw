#! /usr/bin/env python3

##
# @file generate.py
# @ingroup afwdev_generate
# @brief This file contains the main entry point for the "generate" subcommand.
# @details For one srcdir, generate() removes generated/, optionally overlays
#          generate/external/ into generate/temp_generate/, then runs the
#          generators that match existing generate/ subdirectories (object
#          types, interfaces, polymorphic functions, bindings, strings, cmake
#          fragments, and related outputs). root_generate() runs once per
#          package after srcdirs and writes package-level generated/ content
#          such as cmake fragments and JSON schemas.
#

##
# @defgroup afw_bindings Language Bindings
# @ingroup package_afw
#

import glob
import os
import re
import sys
import shutil
import importlib

from _afwdev.generate import \
    c, \
    cmake, \
    const_objects, \
    data_type_bindings, \
    ebnf, \
    function_bindings, \
    javascript_bindings, \
    json_schema, \
    interfaces, \
    polymorphic_functions, \
    property_type, \
    python_bindings, \
    php_bindings, \
    perl_bindings, \
    react_components, \
    refresh_comments, \
    runtime_object_maps, \
    strings        
from _afwdev.common import msg, direct, resources, package, nfc



def generated_h(options):
    # Register glue: plain extern (not exported public API). Core + packages.
    # Filename *_internal.h so default install excludes it.
    filename = options['prefix'] + 'generated_internal.h'
    prefix = options['prefix']
    srcdir = prefix[:-1]
    afw_package = package.get_afw_package(options)
    copyright = afw_package.get('copyright')
    msg.info('Generating ' + filename)
    with nfc.open(options['generated_dir_path'] + filename, mode='w') as fd:
        c.write_h_prologue(
            fd, options['generated_by'],
            'Adaptive Framework Register Generated (' + srcdir +
            ') Internal Header', copyright, filename)
        c.write_doxygen_file_section(
            fd, filename,
            'Internal generated register header for srcdir `' + srcdir +
            '` (not public C API).')

        fd.write('\n#include "afw_minimal.h"\n')

        # Package *_declare_helpers.h is not generated (#172). Use afw_common.h.
        if options['const_objects']:
            fd.write('#include "' + options['prefix'] +
                     'const_objects_internal.h"\n')
        if options['data_types']:
            fd.write('#include "' + options['prefix'] + 'data_type_bindings.h"\n')
        if options['functions']:
            fd.write('#include "' + options['prefix'] +
                     'function_bindings_internal.h"\n')
        if options['runtime_object_maps']:
            fd.write('#include "afw_runtime.h"\n')
            fd.write('#include "' + options['prefix'] + 'runtime_object_maps.h"\n')
        if options['strings']:
            fd.write('#include "' + options['prefix'] + 'strings.h"\n')

        fd.write('\n\n/**\n')
        fd.write(' * @brief Generated register for ' + options['srcdir'] + '. \n')
        fd.write(' * @param xctx of caller.  Should be base xctx.\n')
        fd.write(' */\n')
        fd.write('extern void\n')
        fd.write(options['prefix'] + 'generated_register(afw_xctx_t *xctx);\n')

        if options['manifest']:
            for registry_type in sorted(os.listdir(options['generate_dir_path'] +  'manifest/')):
                if not os.path.isdir(options['generate_dir_path'] +'manifest/' + registry_type):
                    continue
                for key in sorted(os.listdir(options['generate_dir_path'] +  'manifest/' + registry_type + '/')):
                    if not key.endswith('.c'):
                        continue
                    key = key[:-2]
                    fd.write('\n\n/**\n')
                    fd.write(' * @brief Register function for ' + registry_type + ' ' + key + '\n')
                    fd.write(' * @param xctx of caller.\n')
                    fd.write(' */\n')
                    fd.write('extern void\n')
                    fd.write(options['prefix'] + registry_type +  '_' + key + '_register(\n')
                    fd.write('    afw_xctx_t *xctx);\n')

        c.write_h_epilogue(fd, filename)


def generated_version_h(options):
    filename = options['prefix'] + 'version_info.h'
    prefix = options['prefix']
    uprefix = prefix.upper()    
    afw_package = package.get_afw_package(options)
    copyright = afw_package.get('copyright')
    msg.info('Generating ' + filename)
    with nfc.open(options['generated_dir_path'] + filename, mode='w') as fd:
        c.write_h_prologue(fd, options['generated_by'], 
                           'Adaptive Framework Version (' + options['prefix'] + ') Header', copyright, filename)
        c.write_doxygen_file_section(
            fd, filename,
            'Generated version macros for prefix `'
            + options['prefix'] + '`.')
        fd.write('\n\n')

        if not options['core']:
            fd.write('#include "afw_version_info.h"\n')
        fd.write('#include "' + options['afwPackageGitInfoBaseName'] + '.h"\n\n')

        fd.write('#define ' + uprefix + 'VERSION_MAJOR ' + options['version_major'] + '\n')
        fd.write('#define ' + uprefix + 'VERSION_MINOR ' + options['version_minor'] + '\n')
        fd.write('#define ' + uprefix + 'VERSION_PATCH ' + options['version_patch'] + '\n')
        fd.write('#define ' + uprefix + 'VERSION_HEX ( \\\n' +
                        '    (AFW_VERSION_MAJOR << 16) | \\\n' +
                        '    (AFW_VERSION_MINOR <<  8) | \\\n' +
                        '    (AFW_VERSION_PATCH))\n')

        fd.write('#define ' + uprefix + 'VERSION_STRING "' + options['version_string'] + '"\n')
        fd.write('#define ' + uprefix + 'VERSION_WITH_GIT_INFO "' + options['version_string'] + '" \\\n')
        fd.write('    ' + options['afwPackageGitInfoBaseName'].upper() + '_BRANCH_DESCRIBE\n')

        fd.write('#define ' + uprefix + 'VERSION_COMMAND_STRING "' + options['srcdir'] + ' " \\\n')
        if not options['is_core_afw_package']:
            fd.write('    "(" AFW_VERSION_WITH_GIT_INFO ") " \\\n')
        fd.write('    ' + uprefix + 'VERSION_WITH_GIT_INFO\n')

        c.write_h_epilogue(fd, filename)

def generated_c(options):

    filename = options['prefix'] + 'generated.c'
    afw_package = package.get_afw_package(options)
    copyright = afw_package.get('copyright')

    msg.info('Generating ' + filename)
    with nfc.open(options['generated_dir_path'] + filename, mode='w') as fd:
        c.write_c_prologue(fd, options['generated_by'],
            'Adaptive Framework Register Generated (' + options['prefix'] + ')', copyright)
        c.write_doxygen_file_section(
            fd, filename,
            'Generated register implementation for `'
            + options['srcdir'] + '`.')
        fd.write('\n')

        fd.write('#include "afw.h"\n')
        fd.write('#include "' + options['prefix'] + 'generated_internal.h"\n')
        fd.write('#include "' + options['prefix'] + 'version_info.h"\n')

        fd.write('\n')
        fd.write('static const afw_utf8_z_t * impl_version_info_z[] = {\n')
        fd.write('    "compileDate", __DATE__,\n')
        fd.write('    "compileTime", __TIME__,\n')
        fd.write('    "id", "' + options['srcdir'] + '",\n')
        fd.write('    "gitBranch", ' + options['afwPackageGitInfoBaseName'].upper() + '_BRANCH,\n')
        fd.write('    "gitDescribe", ' + options['afwPackageGitInfoBaseName'].upper() + '_DESCRIBE,\n')
        fd.write('    "gitSha", ' + options['afwPackageGitInfoBaseName'].upper() + '_SHA,\n')
        fd.write('    "libafwVersion", AFW_VERSION_STRING,\n')
        fd.write('    "libafwVersionWithGitInfo", AFW_VERSION_WITH_GIT_INFO,\n')
        fd.write('    "version", ' + options['prefix'].upper() + 'VERSION_STRING,\n')
        fd.write('    "versionWithGitInfo", ' + options['prefix'].upper() + 'VERSION_WITH_GIT_INFO,\n')
        fd.write('    NULL\n')
        fd.write('};\n')

        fd.write('\n')
        fd.write('static const afw_utf8_t impl_version_info_path =\n')
        fd.write('    AFW_UTF8_LITERAL("/afw/_AdaptiveVersionInfo_/' + options['srcdir'] + '");\n')

        fd.write('\n')
        fd.write('void ' + options['prefix'] + 'generated_register(afw_xctx_t *xctx)\n')
        fd.write('{\n')
        if options['data_types'] and options['core']:
            fd.write('\n')
            fd.write('    /* Register data types. */\n')
            fd.write('    ' + options['prefix'] + 'register_data_types(xctx);\n')
        if options['functions']:
            fd.write('\n')
            fd.write('    /* Register functions. */\n')
            fd.write('    afw_environment_register_functions(\n')
            fd.write('        ' + options['prefix'] + 'function_bindings_get(), xctx);\n')
        if options['runtime_object_maps']:
            fd.write('\n')
            fd.write('    /* Register object maps. */\n')
            fd.write('    ' + options['prefix'] + 'register_runtime_object_maps(xctx);\n')
        if options['strings']:
            fd.write('\n')
            fd.write('    /* Register reusable interned strings. */\n')
            fd.write('    afw_environment_register_string_literals(\n')
            fd.write('        ' + options['prefix'] +
                     'string_literals_get(), xctx);\n')
        if options['const_objects']:
            fd.write('\n')
            fd.write('    /* Register const runtime objects. */\n')
            fd.write('    ' + options['prefix'] + 'const_objects_register(xctx);\n')

        if options['data_types'] and options['core']:
            fd.write('\n')
            fd.write('    /* Register data type infs. */\n')
            fd.write('    ' + options['prefix'] + 'register_data_type_infs(xctx);\n')

        if options['manifest']:
            for registry_type in sorted(os.listdir(options['generate_dir_path'] +  'manifest/')):
                if not os.path.isdir(options['generate_dir_path'] +'manifest/' + registry_type):
                    continue
                for key in sorted(os.listdir(options['generate_dir_path'] +  'manifest/' + registry_type + '/')):
                    if not key.endswith('.c'):
                        continue
                    key = key[:-2]
                    fd.write('\n')
                    fd.write('    /* Register ' + registry_type + ' ' + key + ' */\n')
                    fd.write('    ' + options['prefix'] + registry_type +  '_' + key + '_register(xctx);\n')

        fd.write("\n")
        fd.write("    /* Register version info object. */\n")
        fd.write("    afw_runtime_env_set_object(\n")
        fd.write("        afw_object_create_const_from_key_value_strings_z(\n")
        fd.write("            &impl_version_info_path,\n")
        fd.write("            impl_version_info_z,\n")
        fd.write("            xctx->env->p, xctx),\n")
        fd.write("        false, xctx);\n")

        fd.write("\n")
        fd.write('}\n')

    if options['manifest']:
        for registry_type in sorted(os.listdir(options['generate_dir_path'] +  'manifest/')):
            if not os.path.isdir(options['generate_dir_path'] +'manifest/' + registry_type):
                continue
            for key in sorted(os.listdir(options['generate_dir_path'] +  'manifest/' + registry_type + '/')):
                if not key.endswith('.c'):
                    continue

                filename = options['prefix'] + 'register_' + registry_type + '_' + key
                msg.info('Generating ' + filename)

                cfile = nfc.read_path(options['generate_dir_path'] +  'manifest/' + registry_type + '/' + key)
                with nfc.open(options['generated_dir_path'] + filename, mode='w') as fd:
                    # Manifest sources are often bare C snippets (or snippets
                    # with a Doxygen block naming the *source* file). Always
                    # emit a generated TU header using the real output name,
                    # then append the body with any leading @file section
                    # stripped so Doxygen does not see a mismatched @file.
                    afw_package = package.get_afw_package(options)
                    copyright = afw_package.get('copyright')
                    title = (
                        'Register ' + registry_type + ' `'
                        + key[:-2] + '` for ' + options['srcdir'])
                    c.write_copyright(fd, title, copyright)
                    c.write_doxygen_file_section(
                        fd, filename,
                        'Generated environment register for '
                        + registry_type + ' `' + key[:-2]
                        + '` (' + options['srcdir'] + ').')
                    fd.write('\n')
                    body = cfile.lstrip('\n')
                    # Drop a leading Doxygen file section from the snippet.
                    if body.lstrip().startswith('/**'):
                        end = body.find('*/')
                        if end != -1:
                            head = body[:end + 2]
                            if '@file' in head or '\\file' in head:
                                body = body[end + 2:].lstrip('\n')
                    # Drop a leading license/copyright block if present so we
                    # do not double-header the generated file.
                    if body.startswith('// See the ') or body.startswith('/*'):
                        if body.startswith('// See the '):
                            nl = body.find('\n')
                            rest = body[nl + 1:] if nl != -1 else body
                            if rest.lstrip().startswith('/*'):
                                end = rest.find('*/')
                                if end != -1:
                                    body = rest[end + 2:].lstrip('\n')
                            else:
                                body = rest.lstrip('\n')
                        elif body.startswith('/*') and 'Copyright' in body[:400]:
                            end = body.find('*/')
                            if end != -1:
                                body = body[end + 2:].lstrip('\n')
                    fd.write(body)


def special_merge_tree(fromdir, destdir):
    if not os.path.exists(destdir):
        shutil.copytree(fromdir, destdir)
    else:
        for filename in sorted(os.listdir(fromdir)):
            if filename.startswith('.'):
                pass
            elif os.path.isdir(fromdir + filename):
                os.makedirs(destdir + filename + '/', exist_ok=True)
                special_merge_tree(fromdir + filename + '/', destdir + filename + '/')
            else:
                shutil.copyfile(fromdir + filename, destdir + filename)

def has_more_than_readme(path):
    result = False
    if os.path.exists(path) and os.path.isdir(path):
        for name in sorted(os.listdir(path)):
            if not name.upper().startswith('README'):
                result = True
                break
    return result

def sort_use_id_cb(obj):
    return obj['_meta_']['objectId']


def update_doxyfile_project_number(options):
    """Set Doxyfile PROJECT_NUMBER from afw-package.json version.

    Kept in the package root (not under generated/) so git and source
    distributions show the correct Doxygen project number. Only the
    PROJECT_NUMBER line is rewritten.
    """
    doxyfile_path = options['afw_package_dir_path'] + 'Doxyfile'
    if not os.path.exists(doxyfile_path):
        return

    afw_package = package.get_afw_package(options)
    version = afw_package.get('version')
    if not version:
        msg.error_exit('version property missing in afw-package.json')

    with nfc.open(doxyfile_path, 'r') as fd:
        text = fd.read()

    new_text, n = re.subn(
        r'(?m)^(PROJECT_NUMBER\s*=\s*).*$',
        r'\g<1>' + version,
        text,
        count=1,
    )
    if n == 0:
        msg.warning(
            'Doxyfile has no PROJECT_NUMBER line; not updating for version '
            + version)
        return
    if new_text == text:
        msg.info('Doxyfile PROJECT_NUMBER already ' + version)
        return

    with nfc.open(doxyfile_path, 'w') as fd:
        fd.write(new_text)
    msg.info('Updated Doxyfile PROJECT_NUMBER to ' + version)


def root_generate(options):

    # Output will go in the source director's generated directory.  Make sure
    # it is empty.
    options['generated_dir_path'] = options['afw_package_dir_path'] + '/generated/'
    if os.path.exists(options['generated_dir_path'] ):
        msg.info('Removing old generated/ directory')
        shutil.rmtree(options['generated_dir_path'] , ignore_errors=False)

    # Make sure generated/ directory structure exists
    os.makedirs(options['generated_dir_path'], exist_ok=True)

    # Call cmake root generate
    cmake.root_generate(options)
    
    # Generate json_schema files
    json_schema.generate(options)

    # Keep Doxyfile project number aligned with package version (source tree).
    update_doxyfile_project_number(options)


# Freshly generated generated directory from objects in object store directory
def generate(passed_options):
    
    options = dict()
    options.update(passed_options)
    options.update(
        {
            "const_objects": False,
            "data_types": False,
            "exports": False,
            "functions": False,          
            "generated_by" : 'afwdev generate ' + options['srcdir'],
            "ebnf": False,
            "interfaces": False,
            "objects": False,
            "object_types": False,
            "is_srcdirManifest": False,
            "polymorphic_functions": False,
            "refresh": True,
            "manifest": False,
            "strings": False,
            "runtime_object_maps": False,
            "use_temporary_generate_dir": False,
            "react_components": False
        }
    )

    # Extra strings can be set in the is. key is string name and value is
    # string (often the same)
    options['const'] = dict()
    options['const']['string'] = dict()

    # Get afw package
    afw_package = package.get_afw_package(options)
    if afw_package.get('srcdirManifest','') == options['srcdir']:
        options['is_srcdirManifest'] = True

    srcdirInfo = afw_package.get('srcdirs',{}).get(options['srcdir'])
    if srcdirInfo is None:
        msg.error_exit('afw-package.json srcdirs property for ' + options['srcdir'] + ' is missing')
    produces = srcdirInfo.get('produces')
    if produces is None:
        msg.error_exit('produces property missing for ' + options['srcdir'])
    if produces.startswith('lib'):
        options['exports'] = True    
  
    options['core'] = options.get('core', False)

    # Used a lot so make a variable.
    generated_by = options['generated_by']

    #  >>>>> Check parameters and determine base directories.

    # The source directory passed must exist (must end in slash)
    if not os.path.exists(options['srcdir_path']):
        msg.error_exit("srcdir_path " + options['srcdir_path'] + " does not exist\n")

    # The source directory must have a generate directory.
    options['original_generate_dir_path'] = options['srcdir_path'] + 'generate/'
    options['generate_dir_path'] = options['original_generate_dir_path']
    if not os.path.exists(options['generate_dir_path']):
        msg.error_exit("generate directory " + options['generate_dir_path'] + " does not exist\n")

    # Get git info
    resources.set_git_and_version_info_options(options)

    # Determine function implementation path
    if os.path.exists(options['srcdir_path'] + 'function') and os.path.isdir(options['srcdir_path'] + 'function'):
        options['function_srcdir_path'] = options['srcdir_path'] + 'function/'
        options['function_srcsubdir'] = options['srcdir'] + '/function'
    else:
        options['function_srcdir_path'] = options['srcdir_path']
        options['function_srcsubdir'] = options['srcdir']

    # Output will go in the source director's generated directory.  Make sure
    # it is empty.
    options['generated_dir_path'] = options['srcdir_path'] + 'generated/'
    if os.path.exists(options['generated_dir_path'] ):
        msg.info('Removing old generated/ directory')
        shutil.rmtree(options['generated_dir_path'] , ignore_errors=False)
    os.makedirs(options['generated_dir_path'], exist_ok=True)

    # Delete existing temporary generate directory.
    options['temporary_generate_dir_path'] = options['srcdir_path'] + 'generate/temp_generate/'
    if os.path.exists(options['temporary_generate_dir_path'] ):
        msg.info('Removing old temp_generate/ directory')
        shutil.rmtree(options['temporary_generate_dir_path'])

    # If external/ make a composite in temp_generate/.
    if has_more_than_readme(options['original_generate_dir_path'] + 'external/'):

        msg.info('Making temp_generate/ directory')
        options['use_temporary_generate_dir'] = True
        options['generate_dir_path'] = options['temporary_generate_dir_path']
        os.makedirs(options['temporary_generate_dir_path'])

        if os.path.exists(options['original_generate_dir_path'] + 'external/'):
            for dir in sorted(os.listdir(options['original_generate_dir_path'] + 'external/')):
                dir_path = options['original_generate_dir_path'] + 'external/' + dir + '/'
                if os.path.isdir(dir_path) and \
                    dir != 'external' and not dir.startswith('temp'):
                    shutil.copytree(dir_path, options['generate_dir_path'] + dir + '/')

        for dir in sorted(os.listdir(options['original_generate_dir_path'])):
            dir_path = options['original_generate_dir_path'] + dir + '/'
            if  not dir.startswith('.') and \
                os.path.isdir(dir_path) and \
                dir != 'external' and \
                not dir.startswith('temp'):
                special_merge_tree(dir_path, options['temporary_generate_dir_path'] + dir + '/')


    # >>>>> Determine options based on generate/ directory structure.

    # Directory additional_include/ contains additional includes.
    options['additional_includes_dir_path'] = None
    if has_more_than_readme(options['generate_dir_path'] + 'additional_includes/'):
        options['additional_includes_dir_path'] = options['generate_dir_path'] + 'additional_includes/'
        headers = glob.glob(options['additional_includes_dir_path'] + '*.h')
        for path in sorted(headers):
            name = os.path.basename(path)[:-2]
            with nfc.open(path, 'r') as fd:
                options['additional_includes_' + name] = fd.read()
    

    # Directory /ebnf contains ebnf definitions.
    options['ebnf_dir_path'] = None
    if has_more_than_readme(options['generate_dir_path'] + 'ebnf/'):
        options['ebnf'] = True
        options['ebnf_dir_path'] = options['generate_dir_path'] + 'ebnf/'

    # Directory /objects contains constant objects.
    options['objects_dir_path'] = None
    if has_more_than_readme(options['generate_dir_path'] + 'objects/'):
        options['objects'] = True
        msg.info('Copying generate/objects/ to generated/objects/')
        options['objects_dir_path'] = options['generated_dir_path'] + 'objects/'
        shutil.copytree(options['generate_dir_path'] + 'objects/',  options['objects_dir_path'])

        if os.path.exists(options['objects_dir_path'] + '_AdaptiveObjectType_/'):
            options['object_types'] = True
        data_type_list = []
        options['data_types'] = has_more_than_readme(options['objects_dir_path'] + '_AdaptiveDataTypeGenerate_/')
        options['functions'] = has_more_than_readme(options['objects_dir_path'] + '_AdaptiveFunctionGenerate_/')
        options['runtime_object_maps'] = has_more_than_readme(options['objects_dir_path'] + '_AdaptiveObjectType_/')
        options['react_components'] = has_more_than_readme(options['objects_dir_path'] + '_AdaptiveLayoutComponentType_')

    # Directory polymorphic_functions/ contains functions.
    options['polymorphic_functions_dir_path'] = None
    if has_more_than_readme(options['generate_dir_path'] + 'objects/_AdaptivePolymorphicFunction_/'):
        if options['objects_dir_path'] is None:
            options['objects_dir_path'] = options['generated_dir_path'] + 'objects/'
        os.makedirs(options['objects_dir_path'], exist_ok=True)
        options['functions'] = True
        options['polymorphic_functions'] = True
        # options['use_temporary_generate_dir'] = True
        options['polymorphic_functions_dir_path'] = options['generate_dir_path'] + 'objects/_AdaptivePolymorphicFunction_/'

    # Directory /interfaces contains interface definitions. This also creates generated objects.
    options['interfaces_dir_path'] = None
    if has_more_than_readme(options['generate_dir_path'] + 'interfaces/'):
        options['interfaces'] = True
        options['interfaces_dir_path'] = options['generate_dir_path'] + 'interfaces/'

    # Directory /manifest contains environment registry <key>/ directories.
    options['registers_dir_path'] = None
    if has_more_than_readme(options['generate_dir_path'] + 'manifest/'):
        options['manifest'] = True
        options['registers_dir_path'] = options['generate_dir_path'] + 'manifest/'

    # Directory /strings contains strings.
    options['strings_dir_path'] = None
    if has_more_than_readme(options['generate_dir_path'] + 'strings/'):
        options['strings'] = True
        options['strings_dir_path'] = options['generate_dir_path'] + 'strings/'

    # Directory /additional_generate
    options['additional_generate'] = None
    if has_more_than_readme(options['generate_dir_path'] + 'additional_generate/'):
        if not os.path.isfile(options['generate_dir_path'] + 'additional_generate/additional_generate.py'):
            msg.error_exit(options['generate_dir_path'] + 'additional_generate/additional_generate.py missing')
        options['additional_generate'] = True
        options['additional_generate_dir_path'] = options['generate_dir_path'] + 'additional_generate/'

    # Directory /additional_javascript
    options['additional_javascript'] = None
    if has_more_than_readme(options['generate_dir_path'] + 'additional_javascript/'):        
        options['additional_javascript'] = True
        options['additional_javascript_dir_path'] = options['generate_dir_path'] + 'additional_javascript/'    


    # If there are objects, strings will always be produced
    if options['objects']:
        options['strings'] = True

    # Seed options['const'] from generate/strings/*.txt before function_bindings
    # / const_objects so preferred labels (boolean::true, integer::zero, …)
    # exist first and later get_string_label calls reuse those permanents.
    if options.get('strings_dir_path'):
        strings.seed_from_strings_dir(options)

    # If debug mode, print options.
    if msg.is_debug_mode():
        msg.debug(nfc.json_dumps(options, sort_keys=True, indent=4))
        msg.debug('\n')

    # >>>>> Optional generates

    # Generate optional object types (needs to be done early)
    if options['object_types']:
        # Load object types and build property types map.  This is very limited and
        # is only intended for core object types where all properties by the same
        # name have the same dataType and objectType.
        object_type_objects = resources.get_core_object_types(options)
        property_type.prepare(object_type_objects)

    # If responsible for manifest, collect (must happen before options['const_objects'])
    include_object_type__AdaptiveManifest_ = False
    if options['is_srcdirManifest']:

        # Store afw-package so it will be included as a const object.
        if options['objects_dir_path'] is None:
            options['objects_dir_path'] = options['generated_dir_path'] + 'objects/'
        os.makedirs(options['objects_dir_path'] + '_AdaptivePackage_', exist_ok=True)
        filename = '_AdaptivePackage_/' + afw_package['afwPackageId'] + '.json'
        msg.info('Generating ' + filename)
        with nfc.open(options['objects_dir_path'] + filename, mode='w') as fd:
            nfc.json_dump(afw_package, fd, indent=4, sort_keys=True)

        # Gather manifest for all srcdirs into one manifest.
        for srcdir, info in afw_package['srcdirs'].items():
            if info.get('skip', False) or info.get('buildType', '') != 'afwmake':
                continue
            this_srcdir_path = options['afw_package_dir_path'] + info['srcdirPath']

            # Load manifest object if it exists and make sure it is named correctly.
            manifest = None
            if os.path.exists(this_srcdir_path + 'generate/objects/_AdaptiveManifest_/'):
                for filename in sorted(os.listdir(this_srcdir_path + 'generate/objects/_AdaptiveManifest_/')):
                    if filename.endswith('.json'):
                        if filename == srcdir + '.json':
                            manifest_path = this_srcdir_path + 'generate/objects/_AdaptiveManifest_/' + srcdir + '.json'
                            manifest = nfc.json_load_from_file(manifest_path)
                        else:
                            msg.error('Named wrong: ' + srcdir + '/generate/objects/_AdaptiveManifest_/' + srcdir + '.json')

            # If there is a /generate/manifest/ for this subdir, apply it to manifest object.
            manifest_path = this_srcdir_path + 'generate/manifest/'
            if not has_more_than_readme(manifest_path):
                if manifest is not None:
                    msg.info('Generating /afw/_AdaptiveManifest_/' + srcdir + ' using src/' + srcdir + '/generate/objects/_AdaptiveManifest_/' + srcdir + '.json asis')
            else:
                if manifest is not None:
                    msg.error_exit(srcdir + '/generate/manifest/ supplies _AdaptiveManifest_ object.  Remove ' + manifest_path)

                if not os.path.exists(manifest_path + 'manifest.json'):
                    msg.error_exit('Missing ' + srcdir + '/generate/manifest/manifest.json')

                msg.info('Generating /afw/_AdaptiveManifest_/' + srcdir + ' using src/' + srcdir + '/generate/manifest/')
                with nfc.open(manifest_path + 'manifest.json') as fd:
                    manifest = nfc.json_load(fd)
                    if manifest.get('registers') is not None:
                        msg.error_exit('There should not be a registers property in ' + srcdir + '/generate/manifest/manifest.json')
                
                manifest['registers'] = []
                for key in sorted(os.listdir(this_srcdir_path + 'generate/manifest/')):
                    if not os.path.isdir(this_srcdir_path + 'generate/manifest/' + key):
                        continue
                    for value in sorted(os.listdir(this_srcdir_path + 'generate/manifest/' + key + '/')):
                        if not value.endswith('.c'):
                            continue
                        manifest['registers'].append(key + '/' + value[:-2])
                        with nfc.open(this_srcdir_path + 'generate/manifest/' + key + '/' + value) as fd:
                            while True:
                                line = nfc.read_line(fd)
                                if not line:
                                    break
                                if line.startswith('#define AFW_ADDITIONAL_REGISTRY_KEY_'):
                                    manifest['registers'].append(key + '/' + line.split()[2])

            # If there are objects, add providesObjects property
            objects_path = this_srcdir_path + '/generated/objects/'
            if manifest is not None and has_more_than_readme(objects_path):
                msg.info('Adding providesObjects property for ' + options['srcdir'])
                manifest['providesObjects'] = []
                for object_type_id in sorted(os.listdir(objects_path)):
                    if object_type_id.endswith('Generate_'):
                        continue
                    if os.path.isdir(objects_path + object_type_id):
                        for object_id in sorted(os.listdir(objects_path + object_type_id + '/')):
                            if object_id.endswith('.json'):
                                manifest['providesObjects'].append(
                                    '/afw/' + object_type_id + '/' + object_id[:-5])

            # Write manifest object
            if manifest is not None:
                if options['objects_dir_path'] is None:
                    options['objects_dir_path'] = options['generated_dir_path'] + 'objects/'
                os.makedirs(options['objects_dir_path'] + '_AdaptiveManifest_/', exist_ok=True)
                with nfc.open(options['objects_dir_path'] + '_AdaptiveManifest_/' + srcdir + '.json', mode='w') as fd:
                    nfc.json_dump(manifest, fd, sort_keys=True, indent=4)
                options['const']['string']['_AdaptiveManifest_'] = '_AdaptiveManifest_'
                options['const']['string'][srcdir] = srcdir

        if has_more_than_readme(options['objects_dir_path'] + '_AdaptiveManifest_/'):
            include_object_type__AdaptiveManifest_ = True



    # Generate optional polymorphic functions
    if options['polymorphic_functions']:
        msg.info('Adding objects/_AdaptiveFunctionGenerate_/ objects for polymorphic functions')
        polymorphic_functions.generate(options)

    # Generate optional data types (core / libafw only).
    if options['data_types']:
        if not options.get('core'):
            msg.error_exit(
                'Data type bindings are only supported in core (libafw). '
                'Package "' + options.get('srcdir', options.get('prefix', '?')) +
                '" has generate/objects/_AdaptiveDataTypeGenerate_/ but non-core '
                'packages must not define Adaptive data types. Implement new data '
                'types in src/afw; extensions and commands use existing types and '
                'register functions/adapters only.')
        data_type_list = direct.retrieve_objects_direct(options['objects_dir_path'] +
            '_AdaptiveDataTypeGenerate_/')
        data_type_list.sort(key=sort_use_id_cb)
        data_type_bindings.generate(generated_by, options['prefix'], data_type_list,
            options['generated_dir_path'] , options)

    # Generate optional functions:
    if options['functions']:
        function_bindings.generate(generated_by, options['prefix'], data_type_list,
            options['objects_dir_path'], options['generated_dir_path'] , options)
        python_bindings.generate(generated_by, data_type_list,
            options['generated_dir_path']  + 'objects/',
            options['generated_dir_path']  + 'python_bindings/',
            options)    
        php_bindings.generate(generated_by, data_type_list,
            options['generated_dir_path']  + 'objects/',
            options['generated_dir_path']  + 'php_bindings/',
            options)    
        perl_bindings.generate(generated_by, data_type_list,
            options['generated_dir_path']  + 'objects/',
            options['generated_dir_path']  + 'perl_bindings/',
            options)    
        javascript_bindings.generate(generated_by, data_type_list,
            options['generated_dir_path']  + 'objects/',
            options['generated_dir_path']  + 'javascript_bindings/',
            options)    
        if options['refresh']:
            refresh_comments.generate(
                options['generated_dir_path']  + 'function_closet/',
                options['function_srcdir_path'],
                options['function_srcsubdir'])

    # Generate optional ebnf.
    if options['ebnf']:
        ebnf.generate(generated_by, options['ebnf_dir_path'], options['generated_dir_path'], options['srcdir_path'] )

    # Generate optional interfaces.
    if options['interfaces']:
        interfaces.generate(generated_by, options['prefix'], options['interfaces_dir_path'], options['generated_dir_path'], afw_package.get('copyright') )

    # Figure out object types needed for const objects
    gen_const_objectTypes = []
    if os.path.exists(options['generated_dir_path'] + 'objects/'):
        objectTypes = [f.name for f in os.scandir(options['generated_dir_path'] + 'objects/') if f.is_dir()]
        objectTypes.sort()
        if '_AdaptiveDataType_' in objectTypes:
            msg.error_exit("_AdaptiveDataType_ is not allowed in generate/objects/. Use _AdaptiveDataTypeGenerate_ instead.")
        if '_AdaptiveFunction_' in objectTypes:
            msg.error_exit("_AdaptiveFunction_ is not allowed in generate/objects/. Use _AdaptiveFunctionGenerate_ instead.")
        special_objectTypes = ['_AdaptiveDataTypeGenerate_','_AdaptiveFunctionGenerate_','_AdaptiveManifest_','_AdaptivePolymorphicFunction_']
        gen_const_objectTypes = [n for n in objectTypes if n not in special_objectTypes]    
    if include_object_type__AdaptiveManifest_:
        gen_const_objectTypes.append('_AdaptiveManifest_')
    if len(gen_const_objectTypes) > 0:
        options['const_objects'] = True

    # Generate optional const objects.
    if options['const_objects']:
        const_objects.generate(generated_by, options['prefix'],
            options['objects_dir_path'], options['generated_dir_path'], gen_const_objectTypes, options)

    # Generate optional runtime object maps:
    if options['runtime_object_maps']:
        runtime_object_maps.generate(generated_by, options)

    # Generate optional strings.
    if len(options['const']['string']) > 0:
        options['strings'] = True
    if options['strings']:
        strings.generate(options, generated_by, options['prefix'], options['strings_dir_path'], options['objects_dir_path'], options['generated_dir_path'] )

    # Generate optional React components.
    if options['react_components']:
        react_components.generate(generated_by, options)

    # >>>>> Common generates

    # Core declare/define macros live in hand-written afw_common.h.
    # Package *_declare_helpers.h is not generated (#172).

    # Run generated.py script from additional_generate
    if options['additional_generate']:
        sys.path.insert(0,options['additional_generate_dir_path'])
        additional_generate = importlib.import_module('additional_generate')
        additional_generate.generate(
            generated_by,
            options['srcdir'],
            options['prefix'],
            options['generate_dir_path'],
            options['additional_generate_dir_path'],
            options['generated_dir_path'],
            options['srcdir_path'])
        sys.path.pop(0)

    # Generate generated_internal.h file.
    generated_h(options)

    # Generate version.h file.
    generated_version_h(options)

    # Generate generated.c file.
    generated_c(options)

    # Generate CMakeInclude-afwdev.txt
    cmake.generate(options)
