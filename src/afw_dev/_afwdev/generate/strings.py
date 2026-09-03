#! /usr/bin/env python3

##
# @file strings.py
# @ingroup afwdev_generate
# @brief This file contains the functions used to generate string constants.
#

import hashlib
import os, fnmatch, re
from _afwdev.generate import c
from _afwdev.common import msg, nfc, package

# Long invented labels use this many hex digits of sha256(value).
# 12 is enough to avoid collisions; we extend on a clash.
_ZZ_HASH_WIDTHS = (12, 16, 20, 32)


def _invented_zz_label(string, existing):
    """Make a valid C ident for a value that is not already one.

    Short values keep a sanitized spelling (zz__Abstract_Class). Long
    values use sha256 of the text so the name does not depend on harvest
    order or on how many long strings an earlier srcdir already minted.
    """
    if len(string) <= 60:
        return 'zz__' + re.sub(r'[^a-zA-Z0-9_]', '_', string)

    digest = hashlib.sha256(string.encode('utf-8')).hexdigest()
    for width in _ZZ_HASH_WIDTHS:
        label = 'zz__' + digest[:width]
        if existing.get(label, string) == string:
            return label
    msg.error_exit(
        'zz__ hash collision for string starting: ' + string[:40])


supported_dataTypes = {
    'anyURI'     : 'AFW_UTF8_LITERAL',
    'boolean'    : '',
    'dnsName'    : 'AFW_UTF8_LITERAL',
    'double'     : '',
    'ia5String'  : 'AFW_UTF8_LITERAL',
    'integer'    : '',
    'ipAddress'  : 'AFW_UTF8_LITERAL',
    'objectId'   : 'AFW_UTF8_LITERAL',
    'objectPath' : 'AFW_UTF8_LITERAL',
    'regexp'     : 'AFW_UTF8_LITERAL',
    'rfc822Name' : 'AFW_UTF8_LITERAL',
    'script'     : 'AFW_UTF8_LITERAL',
    'string'     : 'AFW_UTF8_LITERAL',
    'template'   : 'AFW_UTF8_LITERAL',
    'x500Name'   : 'AFW_UTF8_LITERAL'
}



# @brief Get the string label for a given string.
# @param[in] prefix The prefix for the string label.
# @param[in] strings The dictionary of strings.
# @param[in] string The string to get the label for.
# @param[in] type The type of label to get.
# @return The string label.
#
# Parameter strings is a dictionary of strings.  The keys are the primary part
# of string labels without the prefix and the values are the strings.
# 
# type can be:
# 'U'  for a #define unquoted string label.
# 'Q'  for a #define quoted string label.
# 's'  for a afw_utf8_t string label.
# '*s' for a afw_utf8_t string label that is a pointer.
# 'v'  for a afw_value_string_t label.
# '*v' for a afw_value_string_t label that is a pointer.
# '*z' for an afw_utf8_z_t zero-terminated string label that is a pointer.
def get_string_label(
        options, string, type, labelPreference=None, dataType='string'):
    if dataType not in supported_dataTypes:
        msg.error_exit('Unsupported dataType: ' + dataType)

    strings = options['const'].get(dataType)
    if strings is None:
        options['const'][dataType] = dict()
        strings = options['const'][dataType]
    
    determined = False
    if labelPreference is not None:
        # Explicit preferred labels always win (including a_* aliases that
        # share a value with another label). Value-only dedup applies when
        # labelPreference is omitted (see below).
        if labelPreference in strings:
            if strings[labelPreference] != string:
                msg.warn('Multiple different values for string label: ' +
                    labelPreference)
        label = labelPreference  
        strings[label] = string     
        determined = True 

    if not determined:
        label = None
        for key, value in options['const'][dataType].items():
            if value == string:
                label = key
                break

        if label is None:
            if labelPreference is not None:
                label = labelPreference
            else:
                label = string
                if not re.fullmatch(r'[a-zA-Z0-9_]+', label):
                    label = _invented_zz_label(label, strings)
            options['const'][dataType][label] = string


    use_prefix = options['prefix']
    if dataType != 'string':
        use_prefix += dataType + '_'

    if type == 'Q':
        return use_prefix.upper() + 'Q_' + label
    if type == 's':
        return use_prefix + 's_' + label
    if type == 'self_s':
        return use_prefix + 'self_s_' + label
    if type == '*z':
        return use_prefix+ 'z_' + label
    if type == 'U':
        return use_prefix.upper() + 'U_' + label
    if type == 'v':
        return use_prefix + 'v_' + label
    if type == 'self_v':
        return use_prefix + 'self_v_' + label   
    msg.error_exit('Invalid string type: ' + type)


def seed_from_strings_dir(options):
    """Register permanent labels from generate/strings/*.txt into options['const'].

    Call early (before function_bindings / const_objects) so preferred labels
    such as boolean::true and integer::zero exist first; later get_string_label
    calls with the same value reuse them. Safe to call again from generate().
    """
    strings_dir_path = options.get('strings_dir_path')
    if strings_dir_path is None or not os.path.exists(strings_dir_path):
        return

    for file in sorted(os.listdir(strings_dir_path)):
        if fnmatch.fnmatch(file, '*.txt') and file != 'README.txt':
            with nfc.open(strings_dir_path + file, 'r') as fd:
                for line in fd:
                    if len(line.strip()) == 0 or line[0] == '#':
                        continue
                    if '=' in line:
                        label, op, value = line.partition('=')
                        dataType = 'string'
                        if '::' in label:
                            dataType, op, label = label.partition('::')
                        get_string_label(
                            options, value.strip(),
                            'Q', labelPreference=label,
                            dataType=dataType)
                    else:
                        get_string_label(
                            options, line.strip(),
                            'Q', labelPreference=line.strip())


def _is_generated_label(name):
    """True for labels invented for non-C-ident / long values (zz__*).

    Core only: these go in strings_internal.h and are not exported as public
    libafw ABI. Preferred labels (strings.txt, object keys, ident-like values)
    stay in the public strings.h.
    """
    return name.startswith('zz__')


def _is_parse_word_string(name, value):
    """True if this catalog string is an identifier-like spelling parse may hit.

    `strings.txt` `label=value` (and other aliases) have name != value.
    zz__ invent-for-C names also differ from their text. Keep single words.
    """
    if name != value:
        return False
    return re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]*', value) is not None


def _write_string_decl(fd, prefix, dataType, name, value, decorate):
    """Write one string/const value declaration (macros + extern/self_v)."""
    use_prefix = prefix
    if dataType != 'string':
        use_prefix += dataType + '_'

    fd.write('\n')

    if supported_dataTypes[dataType] == 'AFW_UTF8_LITERAL':
        q_name = use_prefix.upper() + 'Q_' + name
        fd.write('\n/** @brief #define for ' + dataType + ' in quotes */\n')
        fd.write('#define ' + q_name + ' \\\n')
        line = repr(value)[1:-1].replace('"', '\\"')
        fd.write('    "' + line + '"\n')
        fd.write('\n/** @brief \'afw_utf8_t\' for ' + q_name + ' */\n')
        fd.write('#define ' + use_prefix + 's_' + name +
                 ' \\\n    (&' + use_prefix + 'self_v_' + name + '.internal)\n')
        fd.write('\n/** @brief \'afw_utf8_t\' for ' + q_name + ' */\n')
        fd.write('#define ' + use_prefix + 'self_s_' + name +
                 ' \\\n    (' + use_prefix + 'self_v_' + name + '.internal)\n')
        fd.write('\n/** @brief \'afw_value_' + dataType + '_t\' for ' +
                 q_name + ' */\n')
        if decorate:
            fd.write('AFW_DECLARE_CONST_DATA(afw_value_' + dataType + '_t)\n')
            fd.write(use_prefix + 'self_v_' + name + ';\n')
        else:
            fd.write('extern const afw_value_' + dataType + '_t \\\n    ' +
                     use_prefix + 'self_v_' + name + ';\n')
        fd.write('\n/** @brief \'afw_utf8_z_t *\' for ' + q_name + ' */\n')
        fd.write('#define ' + use_prefix + 'z_' + name +
                 ' \\\n    (' + use_prefix + 'self_v_' + name +
                 '.internal.s)\n')
        fd.write('\n/** @brief \'const afw_value_t *\' for ' + q_name + ' */\n')
        fd.write('#define ' + use_prefix + 'v_' + name +
                 ' \\\n    (&' + use_prefix + 'self_v_' + name + '.pub)\n')
    elif supported_dataTypes[dataType] == '':
        fd.write('\n/** @brief \'afw_value_' + dataType + '_t\' for ' +
                 value + ' */\n')
        if decorate:
            fd.write('AFW_DECLARE_CONST_DATA(afw_value_' + dataType + '_t)\n')
            fd.write(use_prefix + 'self_v_' + name + ';\n')
        else:
            fd.write('extern const afw_value_' + dataType + '_t \\\n    ' +
                     use_prefix + 'self_v_' + name + ';\n')
        fd.write('\n/** @brief \'const afw_value_t *\' for ' + dataType +
                 ' ' + value + ' */\n')
        fd.write('#define ' + use_prefix + 'v_' + name +
                 ' \\\n    (&' + use_prefix + 'self_v_' + name + '.pub)\n')
    else:
        msg.error_exit(
            'Unsupported supported_dataTypes[\'' +
            dataType +
            '\']: ' + supported_dataTypes[dataType])

    fd.write('\n')


def _write_strings_header(
        options, generated_by, prefix, generated_dir_path, filename,
        title, brief, names_filter, decorate, emit_get=False):
    """Write one strings header; names_filter(name) selects labels to emit."""
    afw_package = package.get_afw_package(options)
    copyright = afw_package.get('copyright')

    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_h_prologue(fd, generated_by, title, copyright, filename)
        c.write_doxygen_file_section(fd, filename, brief)

        fd.write('\n#include "afw_interface.h"\n')

        for dataType, strings in options['const'].items():
            for name, value in sorted(strings.items()):
                if not names_filter(name):
                    continue
                _write_string_decl(
                    fd, prefix, dataType, name, value, decorate=decorate)

        if emit_get:
            fd.write('\n/**\n')
            fd.write(' * @brief NULL-terminated catalog of interned string values.\n')
            fd.write(' *\n')
            fd.write(' * For `afw_environment_register_string_literals()`.\n')
            fd.write(' */\n')
            if decorate:
                fd.write('AFW_DECLARE(const afw_value_string_t * const *)\n')
            else:
                fd.write('extern const afw_value_string_t * const *\n')
            fd.write(prefix + 'string_literals_get(void);\n')

        c.write_h_epilogue(fd, filename)


def generate_h(options, generated_by, prefix, generated_dir_path):

    # Core: public strings.h (stable labels) + strings_internal.h (zz__*).
    # Packages: single package-private strings.h (everything, undecorated).
    core = options.get('core')

    if core:
        _write_strings_header(
            options, generated_by, prefix, generated_dir_path,
            prefix + 'strings.h',
            'Adaptive Framework (' + prefix + ') Strings Header',
            'Public generated string constants for prefix `' + prefix +
            '`. Stable labels only (not zz__* invent-for-C names).',
            lambda name: not _is_generated_label(name),
            decorate=True,
            emit_get=True)
        _write_strings_header(
            options, generated_by, prefix, generated_dir_path,
            prefix + 'strings_internal.h',
            'Adaptive Framework (' + prefix + ') Strings Internal Header',
            'Internal generated string constants for prefix `' + prefix +
            '` (zz__* labels). For core generated TUs only; not public API.',
            _is_generated_label,
            decorate=False)
    else:
        _write_strings_header(
            options, generated_by, prefix, generated_dir_path,
            prefix + 'strings.h',
            'Adaptive Framework (' + prefix + ') Strings Header',
            'Generated string constants header for prefix `' + prefix + '`.',
            lambda name: True,
            decorate=False,
            emit_get=True)


def generate_c(options, generated_by, prefix, generated_dir_path):

    core = options.get('core')

    filename = prefix + 'strings.c'

    afw_package = package.get_afw_package(options)
    copyright = afw_package.get('copyright')

    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_c_prologue(fd, generated_by, 'Adaptive Framework Const', copyright)
        c.write_doxygen_file_section(
            fd, filename,
            'Generated string/const value definitions for prefix `'
            + prefix + '`.')
        fd.write('\n')
        fd.write('#include "afw.h"\n')
        fd.write('#include "' + prefix + 'strings.h"\n')
        if core:
            fd.write('#include "' + prefix + 'strings_internal.h"\n')

        for dataType, strings in options['const'].items():
            use_prefix = prefix
            if dataType != 'string':
                use_prefix += dataType + '_'

            for name, value in sorted(strings.items()):
                fd.write('\n')
                # Core public labels export; zz__* stay linkage-internal.
                export = core and not _is_generated_label(name)

                if supported_dataTypes[dataType] == 'AFW_UTF8_LITERAL':
                    if export:
                        fd.write('AFW_DEFINE_CONST_DATA(afw_value_' +
                                 dataType + '_t)\n')
                    else:
                        fd.write('const afw_value_' + dataType + '_t\n')
                    fd.write(use_prefix + 'self_v_' + name + ' = {\n')
                    fd.write('    {&afw_value_permanent_' + dataType +
                             '_inf},\n')
                    fd.write(
                        '    AFW_UTF8_LITERAL(' +
                        get_string_label(
                            options, value, 'Q', dataType=dataType,
                            labelPreference=name) +
                        ')\n')
                    fd.write('};\n')
                elif supported_dataTypes[dataType] == '':
                    if export:
                        fd.write('AFW_DEFINE_CONST_DATA(afw_value_' +
                                 dataType + '_t)\n')
                    else:
                        fd.write('const afw_value_' + dataType + '_t\n')
                    fd.write(use_prefix + 'self_v_' + name + ' = {\n')
                    fd.write('    {&afw_value_permanent_' + dataType +
                             '_inf},\n')
                    fd.write('    ' + value + '\n')
                    fd.write('};\n')
                else:
                    msg.error_exit(
                        'Unsupported supported_dataTypes[\'' +
                        dataType +
                        '\']: ' + supported_dataTypes[dataType])

        fd.write('\n')
        fd.write('static const afw_value_string_t * impl_string_literals[] = {\n')
        seen_values = set()
        for name, value in sorted(options['const'].get('string', {}).items()):
            if not _is_parse_word_string(name, value):
                continue
            if value in seen_values:
                continue
            seen_values.add(value)
            fd.write('    &' + prefix + 'self_v_' + name + ',\n')
        fd.write('    NULL\n')
        fd.write('};\n')
        fd.write('\n')
        if core:
            fd.write('AFW_DEFINE(const afw_value_string_t * const *)\n')
        else:
            fd.write('const afw_value_string_t * const *\n')
        fd.write(prefix + 'string_literals_get(void)\n')
        fd.write('{\n')
        fd.write('    return impl_string_literals;\n')
        fd.write('}\n')


def add_object_strings(options, obj):
    for name in obj.keys():
        tag_name = name
        if '-' in tag_name:
            tag_name = 'a_' + name.replace('-', '_')
        get_string_label(options, name, 'Q', labelPreference=tag_name)

        if isinstance(obj[name], dict):
            add_object_strings(options, obj[name])

        elif isinstance(obj[name], str):
            if name == 'dataType':
                get_string_label(
                    options, obj[name], 'Q', labelPreference=obj[name])

def generate(options, generated_by, prefix, strings_dir_path, object_dir_path, generated_dir_path):

    # Make sure generated/ directory structure exists
    os.makedirs(generated_dir_path, exist_ok=True)

    get_string_label(options, 'default', 'Q', labelPreference='default')
    get_string_label(options, 'indirect', 'Q', labelPreference='indirect')

    # Add all object id and property names to strings
    if object_dir_path is not None and os.path.exists(object_dir_path):
        for object_type in sorted(os.listdir(object_dir_path)):
            if os.path.isdir(object_dir_path + object_type):
                get_string_label(
                    options, object_type, 'Q', labelPreference=object_type)
                objects_type_dir_path = object_dir_path + object_type + '/'
                for file in sorted(os.listdir(objects_type_dir_path)):
                    if fnmatch.fnmatch(file, '*.json'):
                        name = file[:-5]
                        get_string_label(
                            options, name, 'Q', labelPreference=name)
                        with nfc.open(objects_type_dir_path + file, 'r') as fd:
                            content = fd.read()
                            obj = nfc.json_loads(content)
                            add_object_strings(options, obj)

    # If there are functions, make sure some needed strings are included
    if object_dir_path is not None and os.path.exists(object_dir_path + '_AdaptiveFunctionGenerate_/'):
        get_string_label(
            options, '_AdaptiveFunction_',
            'Q', labelPreference='_AdaptiveFunction_')
        get_string_label(
            options, '_AdaptiveFunctionParameter_',
            'Q', labelPreference='_AdaptiveFunctionParameter_')

    # names from generate/strings/*.txt (may already be seeded early)
    seed_from_strings_dir(options)

    generate_h(options, generated_by, prefix, generated_dir_path)

    generate_c(options, generated_by, prefix, generated_dir_path)
