#! /usr/bin/env python3

##
# @file data_type_bindings.py
# @namespace afwdev_generate
# @brief Generate C data type bindings (value infs, create APIs, helpers).
# @details
# Metadata source of truth is generate/objects/_AdaptiveDataTypeGenerate_/*.json
# (fields such as cType, special, directReturn, scalar). This module emits the
# C APIs and afw_value_inf_t instances used to create and classify evaluated
# data-type values. Generated only for core (libafw); non-core packages must
# not define _AdaptiveDataTypeGenerate_ (afwdev generate errors if they do).
#
# Adaptive values (afw_value_t) have an inf pointer and a typed payload. For
# each data type this module generates afw_value_<dataType>_t (pub + internal
# cType) and afw_value_<dataType>_managed_t (same plus reference_count). For
# afw_utf8_t / afw_memory_t cTypes it also generates
# afw_value_<dataType>_managed_slice_t (view + containing managed value).
#
# Lifetime policies (inf chooses policy):
#
#   permanent     Built-in or lasts for the life of an AFW environment. Usually
#                 static const in the .so; env-lifetime values may also use the
#                 permanent inf. No refcount. Scalar clone_or_reference is
#                 as-is. Object/array clone_or_reference holds a memory face.
#   managed       Start-at-1 holdable clone. Header + copied internals in
#                 xctx->p, reference_count starts at 1 (must release).
#                 clone_or_reference bumps. Last release frees via
#                 xctx->p. Unmanaged scalar clone_or_reference calls
#                 create_managed.
#   managed_slice View into a containing managed utf8/memory value. Holds the
#                 containing string (get_reference at create). Header in
#                 xctx->p. Slice starts at 1; get_reference bumps the
#                 slice; last release of the slice releases containing
#                 and free_memorys the slice header via xctx->p.
#   unmanaged     Start-at-0 temp. No release unless clone_or_reference.
#                 Object/array clone_or_reference holds a memory face.
#
# Create path depends on cType / directReturn (see designs/memory-management.md phase 0a):
#   utf8/memory  — managed owns a byte copy after the header; slice available
#   other embed  — managed memcpy of struct into header
#   directReturn — store internal as-is; if internal is a pointer type, only
#                  the value header is refcounted (pointer not cloned)
#
# Generated files (prefix e.g. afw_):
#
#   <prefix>data_type_bindings.c / .h
#   <prefix>data_type_<dataType>_binding.c / .h
#   <prefix>data_type_typedefs.h
#
# Inf symbols: afw_value_{permanent,managed,managed_slice,unmanaged}_<dataType>_inf
# (managed_slice only for utf8/memory cTypes; special types get permanent only).
#
# Also generated: type-check macros, afw_value_as_*, object/array helpers, and
# afw_data_type_<dataType>_to_internal / to_utf8. Permanent const instances from
# strings.py / const_objects.py / this module use permanent_*_inf; there is no
# create_permanent_* API.
#


import os
from _afwdev.generate import c
from _afwdev.common import msg, nfc, package


def _supports_managed_slice(ctype):
    return ctype in ('afw_utf8_t', 'afw_memory_t')


def _scalar_holdable_create(type_id):
    """Managed create that starts at RC 1 (not object/array/bool/null)."""
    return type_id not in ('object', 'array', 'boolean', 'null')


def _unmanaged_create_fn(type_id):
    return 'afw_value_' + type_id + '_create'


def _managed_create_fn(type_id):
    return 'afw_value_' + type_id + '_create_managed'


def _managed_slice_fn(type_id):
    return 'afw_value_' + type_id + '_create_managed_slice'


def _allocate_fn(type_id):
    return 'afw_value_' + type_id + '_allocate'


def _managed_free_size_expr(type_id, ctype, var='self'):
    """User size for afw_pool_free_memory of a managed value header."""
    if ctype == 'afw_utf8_t':
        return ('sizeof(afw_value_' + type_id +
                '_managed_t) + ' + var + '->internal.len')
    if ctype == 'afw_memory_t':
        return ('sizeof(afw_value_' + type_id +
                '_managed_t) + ' + var + '->internal.size')
    return 'sizeof(afw_value_' + type_id + '_managed_t)'


def write_typedefs_h_section(fd, prefix, obj):
    id = obj['_meta_']['objectId']

    # internal type
    ctype = obj['cType']

    # parameter is always a const pointer
    parameter_ctype = ctype
    if not ctype.startswith('const'): parameter_ctype='const ' + parameter_ctype
    if not ctype.endswith('*'): parameter_ctype= parameter_ctype + ' *'

    # return type is influenced by directReturn
    direct_return = obj.get('directReturn', False) == True
    if direct_return: return_type = ctype
    else: return_type = parameter_ctype

    fd.write('\n/** @brief Typedef for data type ' + id + ' internal. */\n')
    fd.write('typedef ' + ctype + ' ' + prefix + 'data_type_' + id + '_internal_t;\n')

    fd.write('\n/** @brief Typedef for data type ' + id + ' parameter. */\n')
    fd.write('typedef ' + parameter_ctype + ' ' + prefix + 'data_type_' + id + '_parameter_t;\n')

    fd.write('\n/** @brief Typedef for data type ' + id + ' return. */\n')
    fd.write('typedef ' + return_type + ' ' + prefix + 'data_type_' + id + '_return_t;\n')

    fd.write('\n/** @brief Typedef for data type ' + id + ' value. */\n')
    fd.write('typedef struct afw_value_' + id + '_s\n' + '    afw_value_' + id + '_t;\n')

    fd.write('\n/** @brief Typedef for data type ' + id + ' managed value. */\n')
    fd.write('typedef struct afw_value_' + id + '_managed_s\n' + '    afw_value_' + id + '_managed_t;\n')

    if _supports_managed_slice(ctype):
        fd.write('\n/** @brief Typedef for data type ' + id + ' managed slice value. */\n')
        fd.write('typedef struct afw_value_' + id + '_managed_slice_s\n' +
            '    afw_value_' + id + '_managed_slice_t;\n')


def write_h_section(fd, prefix, obj):
    declare = prefix.upper() + 'DECLARE'
    define = prefix.upper() + 'DEFINE'
    define_inline = prefix.upper() + 'DEFINE_STATIC_INLINE'
    declare_data =  prefix.upper() + 'DECLARE_CONST_DATA'
    id = obj['_meta_']['objectId']

    # internal type
    ctype = obj['cType']

    # special
    special = obj.get('special', False) == True

    # parameter is always a const pointer
    parameter_ctype = ctype
    if not ctype.startswith('const'): parameter_ctype='const ' + parameter_ctype
    if not ctype.endswith('*'): parameter_ctype= parameter_ctype + ' *'

    # return type is influenced by directReturn
    direct_return = obj.get('directReturn', False) == True
    if direct_return: return_type = ctype
    else: return_type = parameter_ctype

    # needs found
    needs_found = direct_return and ctype.find('*') == -1
    needs_found_param = ''
    if needs_found:
        needs_found_param = 'found, '
    amp_if_needed = ''
    if not direct_return: amp_if_needed='&'

    fd.write('\n/** @brief Quoted data type id for ' + id + ' */\n')
    fd.write('#define AFW_DATA_TYPE_ID_Q_' + id + ' "' + id + '"\n')
    fd.write('\n/** @brief Unquoted data type id for ' + id + ' */\n')
    fd.write('#define AFW_DATA_TYPE_ID_U_' + id + ' ' + id + '\n')
    fd.write('\n/** @brief String for data type id ' + id + ' */\n')
    fd.write('#define AFW_DATA_TYPE_ID_S_' + id + ' afw_s_' + id + '\n')
    fd.write('\n/** @brief Quoted c type for data type id ' + id + ' */\n')
    fd.write('#define AFW_DATA_TYPE_CTYPE_Q_' + id + ' "' + ctype + '"\n')

    fd.write('\n/** @brief Adaptive string value for \"' + id + '\". */\n')
    fd.write(declare_data + '(afw_value_t *)\n')
    fd.write('afw_data_type_' + id + '_id_value;\n')

    fd.write('\n/** @brief Data type struct for ' + id + '. */\n')
    fd.write(declare_data + '(afw_data_type_t *)\n')
    fd.write('afw_data_type_' + id + ';\n')

    if not special:

        fd.write('\n/**\n')
        fd.write(' * @brief Unmanaged evaluated value inf for data type ' + id + '.\n')
        fd.write(' *\n')
        fd.write(' * Lifetime is the containing pool until clone_or_reference.\n')
        fd.write(' * Scalar clone_or_reference creates a managed holdable in xctx->p.\n')
        fd.write(' * Object/array clone_or_reference holds a memory face.\n')
        fd.write(' * optional_release is NULL on unmanaged scalars.\n')
        fd.write(' */\n')
        fd.write(declare_data + '(afw_value_inf_t)\n')
        fd.write('afw_value_unmanaged_' + id + '_inf;\n')

        if id in ('object', 'array'):
            fd.write('\n/**\n')
            fd.write(' * @brief Assignable (script face) value inf for data type ' +
                     id + '.\n')
            fd.write(' *\n')
            fd.write(' * Minted by get_assignable_value of a memory bag. get_reference\n')
            fd.write(' * and get_assignable_value bump the instance.\n')
            fd.write(' */\n')
            fd.write(declare_data + '(afw_value_inf_t)\n')
            fd.write('afw_value_assignable_' + id + '_inf;\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Managed evaluated value inf for data type ' + id + '.\n')
        fd.write(' *\n')
        fd.write(' * Start-at-1 holdable clone in xctx->p (must release).\n')
        fd.write(' * clone_or_reference bumps. Last release frees via xctx->p.\n')
        fd.write(' */\n')
        fd.write(declare_data + '(afw_value_inf_t)\n')
        fd.write('afw_value_managed_' + id + '_inf;\n')

        if _supports_managed_slice(ctype):
            fd.write('\n/**\n')
            fd.write(' * @brief Managed slice value inf for data type ' + id + '.\n')
            fd.write(' *\n')
            fd.write(' * Slice of a managed value: get_reference containing at create.\n')
            fd.write(' * Header in xctx->p. Slice starts at 1. get_reference bumps\n')
            fd.write(' * the slice. Last release of the slice releases containing\n')
            fd.write(' * and free_memorys the slice header via xctx->p.\n')
            fd.write(' */\n')
            fd.write(declare_data + '(afw_value_inf_t)\n')
            fd.write('afw_value_managed_slice_' + id + '_inf;\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Permanent (life of afw environment) value inf for data type ' + id + '.\n')
    fd.write(' *\n')
    fd.write(' * Lifetime is the afw environment / static const storage. optional_release\n')
    fd.write(' * is NULL. Scalar clone_or_reference is as-is. Object/array\n')
    fd.write(' * clone_or_reference holds a memory face (same as unmanaged).\n')
    fd.write(' */\n')
    fd.write(declare_data + '(afw_value_inf_t)\n')
    fd.write('afw_value_permanent_' + id + '_inf;\n')

    fd.write('\n/**\n')
    fd.write(' * @brief Macro to determine if data type is ' + id + '.\n')
    fd.write(' * @param A_DATA_TYPE to test.\n')
    fd.write(' * @return boolean result.\n')
    fd.write(' */\n')
    fd.write('#define afw_data_type_is_' + id + '(A_DATA_TYPE) \\\n')
    fd.write('( \\\n')
    fd.write('    (A_DATA_TYPE) && \\\n')
    fd.write('    (A_DATA_TYPE) == afw_data_type_' + id + ' \\\n')
    fd.write(')\n')

    if not special:

        fd.write('\n/**\n')
        fd.write(' * @brief True if A_VALUE is an evaluated ' + id + ' value.\n')
        fd.write(' * @param A_VALUE to test.\n')
        fd.write(' * @return boolean result.\n')
        fd.write(' *\n')
        fd.write(' * For evaluated values only. When true, it is safe to cast A_VALUE to\n')
        fd.write(' * `const afw_value_' + id + '_t *`.\n')
        fd.write(' * If you want to know if the value will be ' + id + ' when fully\n')
        fd.write(' * evaluated (not necessarily cast-safe yet), use\n')
        fd.write(' * `AFW_VALUE_EVALUATES_TO_DATA_TYPE(A_VALUE, ' + id + ', xctx)` instead.\n')
        fd.write(' */\n')
        fd.write('#define afw_value_is_' + id + '(A_VALUE) \\\n')
        fd.write('( \\\n')
        fd.write('    (A_VALUE) && \\\n')
        fd.write('    (A_VALUE)->inf->is_evaluated_of_data_type == afw_data_type_' + id + '\\\n')
        fd.write(')\n')

        fd.write('\n/**\n')
        fd.write(' * @brief True if A_VALUE is an evaluated array of ' + id + '.\n')
        fd.write(' * @param A_VALUE to test.\n')
        fd.write(' * @return boolean result.\n')
        fd.write(' *\n')
        fd.write(' * When true, A_VALUE is an evaluated array (`const afw_value_array_t *`)\n')
        fd.write(' * whose element data type is ' + id + '.\n')
        fd.write(' */\n')
        fd.write('#define afw_value_is_array_of_' + id + '(A_VALUE) \\\n')
        fd.write('( \\\n')
        fd.write('    afw_value_is_array(A_VALUE) && \\\n')
        fd.write('    afw_array_get_data_type( \\\n')
        fd.write('        ((const afw_value_array_t *)(A_VALUE))->internal, \\\n')
        fd.write('        xctx) == afw_data_type_' + id + ' \\\n')
        fd.write(')\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Convert data type ' + id + ' string to ' + ctype + ' *.\n')
        fd.write(' * @param to_internal place to return result.  Must be sizeof(' + ctype + ').\n')
        fd.write(' * @param from_utf8 to convert.\n')
        fd.write(' * @param p to use for returned internal.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' */\n')
        fd.write(declare + '(void)\nafw_data_type_' + id + '_to_internal(\n    ' + ctype + ' *to_internal,\n')
        fd.write('    const afw_utf8_t *from_utf8,\n    const afw_pool_t *p,\n    afw_xctx_t *xctx);\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Convert data type ' + id + ' internal representation to utf-8.\n')
        fd.write(' * @param internal (' + return_type + ') to convert.\n')
        fd.write(' * @param p to use for returned string.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' * @return (const afw_utf8_t *) normalized string representation of value.\n')
        fd.write(' */\n')
        fd.write(declare + '(const afw_utf8_t *)\nafw_data_type_' + id + '_to_utf8(\n    ' + return_type + ' internal,\n')
        fd.write('    const afw_pool_t *p,\n    afw_xctx_t *xctx);\n')

    fd.write('\n/** @brief struct for data type ' + id + ' values. */\n')
    fd.write('struct afw_value_' + id + '_s {\n')
    fd.write('    /** @brief  Value inf union with afw_value_t pub to reduce casting needed. */\n')
    fd.write('    union {\n')
    fd.write('        const afw_value_inf_t *inf;\n')
    fd.write('        afw_value_t pub;\n')
    fd.write('    };\n')
    fd.write('\n')
    fd.write('    /** @brief  Internal ' + ctype + ' value. */\n')
    fd.write('    ' + ctype + ' internal;\n')
    fd.write('};\n')

    fd.write('\n/** @brief struct for managed data type ' + id + ' values.\n')
    fd.write(' *\n')
    fd.write(' * This is the same as afw_value_' + id + '_s with the addition of a\n')
    fd.write(' * reference count. This is intended for internal use only.\n')
    fd.write(' */\n')
    fd.write('struct afw_value_' + id + '_managed_s {\n')
    fd.write('    /** @brief  Value inf union with afw_value_t pub to reduce casting needed. */\n')
    fd.write('    union {\n')
    fd.write('        const afw_value_inf_t *inf;\n')
    fd.write('        afw_value_t pub;\n')
    fd.write('    };\n')
    fd.write('\n')
    fd.write('    /** @brief  Internal ' + ctype + ' value. */\n')
    fd.write('    ' + ctype + ' internal;\n')
    fd.write('\n')
    fd.write('    /** @brief  Reference count for value. */\n')
    fd.write('    afw_size_t reference_count;\n')
    fd.write('};\n')

    if _supports_managed_slice(ctype):
        fd.write('\n/** @brief struct for managed slice data type ' + id + ' values.\n')
        fd.write(' *\n')
        fd.write(' * A view into a containing managed value. Holds that value.\n')
        fd.write(' */\n')
        fd.write('struct afw_value_' + id + '_managed_slice_s {\n')
        fd.write('    /** @brief  Value inf union with afw_value_t pub to reduce casting needed. */\n')
        fd.write('    union {\n')
        fd.write('        const afw_value_inf_t *inf;\n')
        fd.write('        afw_value_t pub;\n')
        fd.write('    };\n')
        fd.write('\n')
        fd.write('    /** @brief  Internal ' + ctype + ' view (pointer into containing). */\n')
        fd.write('    ' + ctype + ' internal;\n')
        fd.write('\n')
        fd.write('    /** @brief  Containing managed value that owns the buffer. */\n')
        fd.write('    const afw_value_' + id + '_managed_t *containing_value;\n')
        fd.write('\n')
        fd.write('    /** @brief  Reference count for this slice. */\n')
        fd.write('    afw_size_t reference_count;\n')
        fd.write('};\n')

    if not special:
        fd.write('\n/**\n')
        fd.write(' * @brief Typesafe cast of data type ' + id + '.\n')
        fd.write(' * @param value (const afw_value_t *).\n')
        fd.write(' * @return (' + return_type + ')\n')
        fd.write(' */\n')
        fd.write(declare + '(' + return_type + ')\n')
        fd.write('afw_value_as_' + id + '(\n    const afw_value_t *value,\n    afw_xctx_t *xctx);\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Allocate function for data type ' + id + ' value.\n')
        fd.write(' * @param p to use for returned value.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' * @return Allocated afw_value_' + id + '_t with unmanaged inf set.\n')
        fd.write(' *\n')
        if id == 'null':
            fd.write(' * Prefer afw_value_null instead of allocate+fill. This API still\n')
            fd.write(' * allocates a pool header for rare callers that need a writable\n')
            fd.write(' * afw_value_null_t; that is not the permanent singleton.\n')
        elif id == 'boolean':
            fd.write(' * Prefer permanent afw_boolean_v_* / create_*_boolean (which return\n')
            fd.write(' * those permanents). allocate still makes a pool header for rare\n')
            fd.write(' * writable cases; that is not the permanent true/false.\n')
        else:
            fd.write(' * Unmanaged: lifetime is pool p; no value refcount.\n')
            fd.write(' * Caller fills internal after allocate.\n')
        fd.write(' */\n')
        fd.write(declare + '(afw_value_' + id + '_t *)\n')
        fd.write(_allocate_fn(id) + '(\n    const afw_pool_t *p,\n    afw_xctx_t *xctx);\n')
        fd.write('#define afw_value_allocate_unmanaged_' + id + ' ' +
                 _allocate_fn(id) + '\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Create function for managed data type ' + id + ' value.\n')
        fd.write(' * @param internal.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' * @return Created const afw_value_t *.\n')
        fd.write(' *\n')
        if id == 'null':
            fd.write(' * Returns the permanent singleton afw_value_null (address identity).\n')
            fd.write(' * Does not allocate. Prefer afw_value_null at call sites.\n')
            fd.write(' * internal is ignored (null has no payload).\n')
        elif id == 'boolean':
            fd.write(' * Returns permanent afw_boolean_v_true or afw_boolean_v_false\n')
            fd.write(' * (intentional; only two Adaptive booleans). Does not allocate.\n')
            fd.write(' * Prefer afw_value_for_boolean / afw_boolean_v_* at call sites.\n')
        else:
            if _scalar_holdable_create(id):
                fd.write(' * Allocates in xctx->p. Starts at reference count 1 (must release).\n')
                fd.write(' * clone_or_reference bumps. Last release frees via xctx->p.\n')
            else:
                fd.write(' * Allocates in p. Starts at reference count 1 (must release).\n')
                fd.write(' * clone_or_reference bumps. Last release frees via stored p.\n')
            if ctype in ('afw_utf8_t', 'afw_memory_t'):
                fd.write(
                    ' * Copies bytes into storage following the header '
                    '(value owns them).\n')
            elif direct_return and ctype.rstrip().endswith('*'):
                fd.write(
                    ' * Stores the pointer as-is; does not clone or take a '
                    'reference on the\n')
                fd.write(
                    ' * referent. Caller must ensure the referent outlives '
                    'this value (or\n')
                fd.write(
                    ' * a future object/array path may special-case '
                    'container RC).\n')
            elif direct_return:
                fd.write(' * Stores internal by value in the header.\n')
            else:
                fd.write(
                    ' * Copies *internal into the header when internal is '
                    'non-NULL.\n')
        fd.write(' */\n')
        fd.write(declare + '(const afw_value_t *)\n')
        fd.write(_managed_create_fn(id) + '(\n    ' + return_type + ' internal,\n')
        fd.write('    afw_xctx_t *xctx);\n')
        fd.write('#define afw_value_create_managed_' + id + ' ' +
                 _managed_create_fn(id) + '\n')

        if ctype == 'afw_utf8_t':
            fd.write('\n/**\n')
            fd.write(' * @brief Create a managed slice of a managed data type ' + id + ' value.\n')
            fd.write(' * @param containing_value managed (or managed_slice) value with cType afw_utf8_t.\n')
            fd.write(' * @param offset into containing value\'s internal.\n')
            fd.write(' * @param len of slice.\n')
            fd.write(' * @param xctx of caller.\n')
            fd.write(' * @return Created const afw_value_t * (managed_slice inf).\n')
            fd.write(' *\n')
            fd.write(' * View of a managed string. get_reference on containing. Slice starts\n')
            fd.write(' * at 1 (must release). Header allocated in xctx->p.\n')
            fd.write(' */\n')
            fd.write(declare + '(const afw_value_t *)\n')
            fd.write(_managed_slice_fn(id) + '(\n')
            fd.write('    const afw_value_t *containing_value,\n')
            fd.write('    afw_size_t offset,\n')
            fd.write('    afw_size_t len,\n')
            fd.write('    afw_xctx_t *xctx);\n')
            fd.write('#define afw_value_create_managed_' + id + '_slice ' +
                     _managed_slice_fn(id) + '\n')
        elif ctype == 'afw_memory_t':
            fd.write('\n/**\n')
            fd.write(' * @brief Create a managed slice of a managed data type ' + id + ' value.\n')
            fd.write(' * @param containing_value managed (or managed_slice) value with cType afw_memory_t.\n')
            fd.write(' * @param offset into containing value\'s internal.\n')
            fd.write(' * @param size of slice.\n')
            fd.write(' * @param xctx of caller.\n')
            fd.write(' * @return Created const afw_value_t * (managed_slice inf).\n')
            fd.write(' *\n')
            fd.write(' * View of a managed memory value. get_reference on containing. Slice\n')
            fd.write(' * starts at 1 (must release). Header allocated in xctx->p.\n')
            fd.write(' */\n')
            fd.write(declare + '(const afw_value_t *)\n')
            fd.write(_managed_slice_fn(id) + '(\n')
            fd.write('    const afw_value_t *containing_value,\n')
            fd.write('    afw_size_t offset,\n')
            fd.write('    afw_size_t size,\n')
            fd.write('    afw_xctx_t *xctx);\n')
            fd.write('#define afw_value_create_managed_' + id + '_slice ' +
                     _managed_slice_fn(id) + '\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Create function for unmanaged data type ' + id + ' value.\n')
        fd.write(' * @param internal.\n')
        fd.write(' * @param p to use for returned value.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' * @return Created const afw_value_t *.\n')
        fd.write(' *\n')
        if id == 'null':
            fd.write(' * Returns the permanent singleton afw_value_null (address identity).\n')
            fd.write(' * Does not allocate in p. Prefer afw_value_null at call sites.\n')
            fd.write(' * internal is ignored (null has no payload).\n')
        elif id == 'boolean':
            fd.write(' * Returns permanent afw_boolean_v_true or afw_boolean_v_false\n')
            fd.write(' * (intentional; only two Adaptive booleans). Does not allocate in p.\n')
            fd.write(' * Prefer afw_value_for_boolean / afw_boolean_v_* at call sites.\n')
        else:
            fd.write(
                ' * Allocates in pool p; lifetime is the pool '
                '(no value refcount).\n')
            if ctype in ('afw_utf8_t', 'afw_memory_t'):
                fd.write(
                    ' * Copies the utf8/memory header only, not the octets.\n')
            if id in ('object', 'array'):
                fd.write(
                    ' * clone_or_reference holds a memory face.\n')
            else:
                fd.write(
                    ' * clone_or_reference creates a managed holdable in xctx->p.\n')
            if direct_return and ctype.rstrip().endswith('*'):
                fd.write(
                    ' * Stores the pointer as-is; does not clone the '
                    'referent.\n')
        fd.write(' */\n')
        fd.write(declare + '(const afw_value_t *)\n')
        fd.write(_unmanaged_create_fn(id) + '(' + return_type + ' internal,\n')
        fd.write('    const afw_pool_t *p, afw_xctx_t *xctx);\n')
        fd.write('#define afw_value_create_unmanaged_' + id + ' ' +
                 _unmanaged_create_fn(id) + '\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Get property function for data type ' + id + ' value.\n')
        fd.write(' * @deprecated\n')
        fd.write(' * @param object of property to get.\n')
        fd.write(' * @param property_name of property to get.\n')
        if needs_found:
            fd.write(' * @param found is place to return whether property is found.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' * @return ' + return_type + '.\n')
        fd.write(' *\n')
        fd.write(' * This is a deprecated function used to get around an exception that\n')
        fd.write(' * was occurring when an object did not have a pool. Use the function\n')
        fd.write(' * without an "_old" in the name for all new code and replace calls in\n')
        fd.write(' * old code when possible.\n')
        fd.write(' *\n')
        fd.write(' */\n')
        fd.write('#define afw_object_old_get_property_as_' + id + '( \\\n')
        fd.write('    object, property_name, ' + needs_found_param + 'xctx) \\\n')
        fd.write('afw_object_get_property_as_' + id + '_source( \\\n')
        fd.write('    object, property_name, ' + needs_found_param + 'AFW__FILE_LINE__, \\\n')
        fd.write('    ((object)->p ? (object)->p : (xctx)->p), (xctx))\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Get property function for data type ' + id + ' value.\n')
        fd.write(' * @param object of property to get.\n')
        fd.write(' * @param property_name of property to get.\n')
        if needs_found:
            fd.write(' * @param found is place to return whether property is found.\n')
        fd.write(' * @param p to use for result if evaluation or conversion is required.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' * @return ' + return_type + '.\n')
        fd.write(' */\n')
        fd.write('#define afw_object_get_property_as_' + id + '( \\\n')
        fd.write('    object, property_name, ' + needs_found_param + 'p, xctx) \\\n')
        fd.write('afw_object_get_property_as_' + id + '_source( \\\n')
        fd.write('    object, property_name, ' + needs_found_param + 'AFW__FILE_LINE__, p, xctx)\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Get property function for data type ' + id + ' value.\n')
        fd.write(' * @param object of property to get.\n')
        fd.write(' * @param property_name of property to get.\n')
        if needs_found:
            fd.write(' * @param found is place to return whether property is found.\n')
        fd.write(' * @param source_z file:line.\n')
        fd.write(' * @param p to use for result if evaluation or conversion is required.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' * @return ' + return_type + '.\n')
        fd.write(' */\n')
        fd.write(declare + '(' + return_type + ')\n')
        fd.write('afw_object_get_property_as_' + id + '_source(\n')
        fd.write('    const afw_object_t *object,\n')
        fd.write('    const afw_value_t *property_name,\n')
        if needs_found:
            fd.write('    afw_boolean_t *found,\n')
        fd.write('    const afw_utf8_z_t *source_z,\n')
        fd.write('    const afw_pool_t *p,\n')
        fd.write('    afw_xctx_t *xctx);\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Get next property function for data type ' + id + ' value.\n')
        fd.write(' * @deprecated\n')
        fd.write(' * @param object of property to get.\n')
        fd.write(' * @param iterator pointer. Set to NULL before first call.\n')
        fd.write(' * @param property_name is place to return pointer to property name.\n')
        if needs_found:
            fd.write(' * @param found is place to return whether property is found.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' * @return ' + return_type + '.\n')
        fd.write(' *\n')
        fd.write(' * This is a deprecated function used to get around an exception that\n')
        fd.write(' * was occurring when an object did not have a pool. Use the function\n')
        fd.write(' * without an "_old" in the name for all new code and replace calls in\n')
        fd.write(' * old code when possible.\n')
        fd.write(' *\n')
        fd.write(' */\n')
        fd.write('#define afw_object_old_get_next_property_as_' + id + '( \\\n')
        fd.write('    object, iterator, property_name, ' + needs_found_param + 'xctx) \\\n')
        fd.write('afw_object_get_next_property_as_' + id + '_source( \\\n')
        fd.write('    object, iterator, property_name, ' + needs_found_param + 'AFW__FILE_LINE__, \\\n')
        fd.write('    ((object)->p ? (object)->p : (xctx)->p), (xctx))\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Get next property function for data type ' + id + ' value.\n')
        fd.write(' * @param object of property to get.\n')
        fd.write(' * @param iterator pointer. Set to NULL before first call.\n')
        fd.write(' * @param property_name is place to return pointer to property name.\n')
        if needs_found:
            fd.write(' * @param found is place to return whether property is found.\n')
        fd.write(' * @param p to use for result if evaluation or conversion is required.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' * @return ' + return_type + '.\n')
        fd.write(' */\n')
        fd.write('#define afw_object_get_next_property_as_' + id + '( \\\n')
        fd.write('    object, iterator, property_name, ' + needs_found_param + 'p, xctx) \\\n')
        fd.write('afw_object_get_next_property_as_' + id + '_source( \\\n')
        fd.write('    object, iterator, property_name, ' + needs_found_param + 'AFW__FILE_LINE__, p, xctx)\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Get property function for data type ' + id + ' value.\n')
        fd.write(' * @param object of property to get.\n')
        fd.write(' * @param iterator pointer. Set to NULL before first call.\n')
        fd.write(' * @param property_name is place to return pointer to property name.\n')
        if needs_found:
            fd.write(' * @param found is place to return whether property is found.\n')
        fd.write(' * @param source_z file:line.\n')
        fd.write(' * @param p to use for result if conversion is required.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' * @return ' + return_type + '.\n')
        fd.write(' */\n')
        fd.write(declare + '(' + return_type + ')\n')
        fd.write('afw_object_get_next_property_as_' + id + '_source(\n')
        fd.write('    const afw_object_t *object,\n')
        fd.write('    const afw_iterator_old_t * *iterator,\n')
        fd.write('    const afw_value_t * *property_name,\n')
        if needs_found:
            fd.write('    afw_boolean_t *found,\n')
        fd.write('    const afw_utf8_z_t *source_z,\n')
        fd.write('    const afw_pool_t *p,\n')
        fd.write('    afw_xctx_t *xctx);\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Set property function for data type ' + id + ' values.\n')
        fd.write(' * @param object of property to set.\n')
        fd.write(' * @param property_name of property to set.\n')
        fd.write(' * @param value of value to set.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' *\n')
        if id == 'boolean':
            fd.write(' * Uses permanent afw_boolean_v_true / afw_boolean_v_false\n')
            fd.write(' * (no pool allocation for the value).\n')
        elif id == 'integer':
            fd.write(' * Uses permanent afw_integer_v_zero / afw_integer_v_one when\n')
            fd.write(' * internal is 0 or 1; otherwise allocates in the object\'s pool.\n')
        else:
            fd.write(' * The value will be allocated in the object\'s pool.\n')
        fd.write(' * Prefer afw_object_set_property(..., afw_v_*, ...) when a\n')
        fd.write(' * static const value (e.g. from afw_strings.h) already\n')
        fd.write(' * exists for that constant.\n')
        fd.write(' *\n')
        fd.write(' */\n')
        fd.write(declare + '(void)\n')
        fd.write('afw_object_set_property_as_' + id + '(\n')
        fd.write('    const afw_object_t *object,\n')
        fd.write('    const afw_value_t *property_name,\n')
        fd.write('    ' + return_type + ' internal,\n')
        fd.write('    afw_xctx_t *xctx);\n')

        # array
        fd.write('\n/**\n')
        fd.write(' * @brief Get next value from array of ' + id + '.\n')
        fd.write(' * @param instance of array.\n')
        fd.write(' * @param iterator.\n')
        if needs_found:
            fd.write(' * @param found is place to return whether value is found.\n')
        fd.write(' * @param source_z file:line.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' * @return (' + return_type + ') or NULL.\n')
        fd.write(' * \n')
        fd.write(' * Set the iterator to NULL before the first call and anytime\n')
        fd.write(' * you want to start from the first value again.\n')
        fd.write(' */\n')
        fd.write('#define afw_array_of_' + id + '_get_next( \\\n')
        fd.write('    array, iterator, ' + needs_found_param + 'xctx) \\\n')
        fd.write('    afw_array_of_' + id + '_get_next_source( \\\n')
        fd.write('    array, iterator, ' + needs_found_param + 'AFW__FILE_LINE__, xctx)\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Get next value from array of ' + id + '.\n')
        fd.write(' * @param instance of array.\n')
        fd.write(' * @param iterator.\n')
        if needs_found:
            fd.write(' * @param found is place to return whether value is found.\n')
        fd.write(' * @param source_z file:line.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' * @return (' + return_type + ') or NULL.\n')
        fd.write(' * \n')
        fd.write(' * Set the iterator to NULL before the first call and anytime\n')
        fd.write(' * you want to start from the first value again.\n')
        fd.write(' */\n')
        fd.write(declare + '(' + return_type + ')\n')
        fd.write(prefix + 'array_of_' + id + '_get_next_source(\n')
        fd.write('    const afw_array_t *instance,\n')
        fd.write('    const afw_iterator_old_t * *iterator,\n')
        if needs_found:
            fd.write('    afw_boolean_t *found,\n')
        fd.write('    const afw_utf8_z_t *source_z,\n')
        fd.write('    afw_xctx_t *xctx);\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Add value from array of ' + id + '.\n')
        fd.write(' * @param instance of array.\n')
        fd.write(' * @param value to add.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' */\n')
        fd.write(declare + '(void)\n')
        fd.write(prefix + 'array_of_' + id + '_add(\n')
        fd.write('    const afw_array_t *instance,\n')
        fd.write('    ' + parameter_ctype + 'value,\n')
        fd.write('    afw_xctx_t *xctx);\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Remove value from array of ' + id + '.\n')
        fd.write(' * @param instance of array.\n')
        fd.write(' * @param value to remove.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' */\n')
        fd.write(declare + '(void)\n')
        fd.write(prefix + 'array_of_' + id + '_remove(\n')
        fd.write('    const afw_array_t *instance,\n')
        fd.write('    ' + parameter_ctype + 'value,\n')
        fd.write('    afw_xctx_t *xctx);\n')

    # Data type direct (public: extensions use afw_data_type_*_direct in statics).
    fd.write('\n/**\n')
    fd.write(' * @brief Public data type ' + id + ' struct instance.\n')
    fd.write(' *\n')
    fd.write(' * Prefer afw_data_type_' + id + ' when a pointer is enough and you are not\n')
    fd.write(' * initializing static data that must reference the struct object.\n')
    fd.write(' */\n')
    fd.write(declare_data + '(afw_data_type_t)\n')
    fd.write('afw_data_type_' + id + '_direct;\n')

    # Data type inf (vtable for the data type instance).
    fd.write('\n/**\n')
    fd.write(' * @brief Public data type ' + id + ' inf.\n')
    fd.write(' *\n')
    fd.write(' * Defined with the data type implementation; managed by the generated\n')
    fd.write(' * data type instance.\n')
    fd.write(' */\n')
    fd.write(declare_data + '(afw_data_type_inf_t)\n')
    fd.write('afw_data_type_' + id + '_inf;\n')


def write_c_section(fd, prefix, obj):
    define = prefix.upper() + 'DEFINE'
    define_data =  prefix.upper() + 'DEFINE_CONST_DATA'
    id = obj['_meta_']['objectId']

    # internal type
    ctype = obj['cType']

    # special
    special = obj.get('special', False) == True

    # parameter is always a const pointer
    parameter_ctype = ctype
    if not ctype.startswith('const'): parameter_ctype='const ' + parameter_ctype
    if not ctype.endswith('*'): parameter_ctype= parameter_ctype + ' *'

    # return type is influenced by directReturn
    direct_return = obj.get('directReturn', False) == True
    if direct_return: return_type = ctype
    else: return_type = parameter_ctype

    # needs found
    needs_found = direct_return and ctype.find('*') == -1
    needs_found_param = ''
    if needs_found:
        needs_found_param = 'found, '
    amp_if_needed = ''
    if not direct_return: amp_if_needed='&'

    fd.write('#include "afw_value_internal.h"\n')

    if not special:       
        fd.write('\n')
        fd.write('/* Declaration for method optional_release for managed value. */\n')
        fd.write('AFW_DECLARE_STATIC(void)\n')
        fd.write('impl_afw_value_managed_optional_release(\n')
        fd.write('    const afw_value_t *instance,\n')
        fd.write('    afw_xctx_t *xctx);\n')
        fd.write('\n')
        if id in ('object', 'array'):
            fd.write('/* Declaration for method optional_release for unmanaged '
                     'value. */\n')
            fd.write('AFW_DECLARE_STATIC(void)\n')
            fd.write('impl_afw_value_unmanaged_optional_release(\n')
            fd.write('    const afw_value_t *instance,\n')
            fd.write('    afw_xctx_t *xctx);\n')
            fd.write('\n')
        
        fd.write('\n')
        fd.write('/* Declaration for method get_reference for value. */\n')
        fd.write('AFW_DECLARE_STATIC(const afw_value_t *)\n')
        fd.write('impl_afw_value_get_reference(\n')
        fd.write('    const afw_value_t *instance,\n')
        fd.write('    const afw_pool_t *p,\n')
        fd.write('    afw_xctx_t *xctx);\n')
        fd.write('\n')
    
        fd.write('\n')
        fd.write('/* Declaration for method get_reference for managed value. */\n')
        fd.write('AFW_DECLARE_STATIC(const afw_value_t *)\n')
        fd.write('impl_afw_value_managed_get_reference(\n')
        fd.write('    const afw_value_t *instance,\n')
        fd.write('    const afw_pool_t *p,\n')
        fd.write('    afw_xctx_t *xctx);\n')
        fd.write('\n')

        if _supports_managed_slice(ctype):
            fd.write('\n')
            fd.write('/* Declaration for method optional_release for managed slice value. */\n')
            fd.write('AFW_DECLARE_STATIC(void)\n')
            fd.write('impl_afw_value_managed_slice_optional_release(\n')
            fd.write('    const afw_value_t *instance,\n')
            fd.write('    afw_xctx_t *xctx);\n')
            fd.write('\n')
            fd.write('/* Declaration for method get_reference for managed slice value. */\n')
            fd.write('AFW_DECLARE_STATIC(const afw_value_t *)\n')
            fd.write('impl_afw_value_managed_slice_get_reference(\n')
            fd.write('    const afw_value_t *instance,\n')
            fd.write('    const afw_pool_t *p,\n')
            fd.write('    afw_xctx_t *xctx);\n')
            fd.write('\n')

    fd.write('\n')
    fd.write('/* Declaration for method get_reference for permanent value. */\n')
    fd.write('AFW_DECLARE_STATIC(const afw_value_t *)\n')
    fd.write('impl_afw_value_permanent_get_reference(\n')
    fd.write('    const afw_value_t *instance,\n')
    fd.write('    const afw_pool_t *p,\n')
    fd.write('    afw_xctx_t *xctx);\n')
    fd.write('\n')

    x = obj.get("afw_value_get_evaluated_meta", "afw_value_internal_get_evaluated_meta_default")
    fd.write('\n#define impl_afw_value_get_evaluated_meta \\\n')
    fd.write('    ' + x +'\n')

    x = obj.get("afw_value_get_evaluated_metas", "afw_value_internal_get_evaluated_metas_default")
    fd.write('\n#define impl_afw_value_get_evaluated_metas \\\n')
    fd.write('    ' + x +'\n')

    fd.write('\n/* This is fully evaluated so optional_evaluate method is NULL. */\n')
    fd.write('#define impl_afw_value_optional_evaluate NULL\n')

    fd.write('\n/* Inf specific is always data type. */\n')
    fd.write('#define AFW_IMPLEMENTATION_SPECIFIC (const void *)&afw_data_type_' + id + '_direct\n')

    fd.write('\n/* Define inf variables for data_type and is_evaluated_of_data_type. */\n')
    fd.write('#define AFW_IMPLEMENTATION_INF_VARIABLES \\\n')
    fd.write('    (const void *)&afw_data_type_' + id + '_direct, \\\n')
    fd.write('    (const void *)&afw_data_type_' + id + '_direct\n')

    if not special:

        if id in ('object', 'array'):
            fd.write('\nAFW_DECLARE_STATIC(const afw_value_t *)\n')
            fd.write('impl_afw_value_get_assignable_value(\n')
            fd.write('    const afw_value_t *instance,\n')
            fd.write('    const afw_pool_t *p,\n')
            fd.write('    afw_xctx_t *xctx);\n')
            fd.write('\nAFW_DECLARE_STATIC(const afw_value_t *)\n')
            fd.write('impl_afw_value_assignable_get_reference(\n')
            fd.write('    const afw_value_t *instance,\n')
            fd.write('    const afw_pool_t *p,\n')
            fd.write('    afw_xctx_t *xctx);\n')

        fd.write('\n/* Declares and rti/inf defines for interface afw_value */\n')
        if id in ('object', 'array'):
            fd.write('/* unmanaged ' + id + ': optional_release holds the '
                     'instance. */\n')
            fd.write('/* clone_or_reference is object_hold / array_hold. */\n')
        else:
            fd.write('/* unmanaged ' + id + ': optional_release NULL; */\n')
            if obj.get('scalar', False) == True:
                fd.write('/* clone_or_reference creates a managed holdable in xctx->p. */\n')
            else:
                fd.write('/* clone_or_reference returns the same instance '
                         '(pool lifetime). */\n')
        fd.write('#define AFW_IMPLEMENTATION_ID "' + id + '"\n')
        fd.write('#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA\n')
        fd.write('#define AFW_IMPLEMENTATION_INF_LABEL afw_value_unmanaged_' + id + '_inf\n')
        if id in ('object', 'array'):
            fd.write('#define impl_afw_value_optional_release '
                     'impl_afw_value_unmanaged_optional_release\n')
        else:
            fd.write('#define impl_afw_value_optional_release NULL\n')
        fd.write('#define impl_afw_value_get_reference impl_afw_value_get_reference\n')
        if id in ('object', 'array'):
            fd.write('#define impl_afw_value_get_assignable_value '
                     'impl_afw_value_get_assignable_value\n')
        else:
            fd.write('#define impl_afw_value_get_assignable_value impl_afw_value_get_reference\n')
        fd.write('#define impl_afw_value_create_iterator NULL\n')

        fd.write('#include "afw_value_impl_declares.h"\n')
        fd.write('#undef AFW_IMPLEMENTATION_ID\n')
        fd.write('#undef AFW_IMPLEMENTATION_INF_LABEL\n')
        fd.write('#undef impl_afw_value_optional_release\n')
        fd.write('#undef impl_afw_value_get_reference\n')
        fd.write('#undef impl_afw_value_get_assignable_value\n')

        fd.write('\n/* Declares and rti/inf defines for interface afw_value */\n')
        fd.write('/* managed ' + id + ': optional_release frees header at RC 0; */\n')
        fd.write('/* clone_or_reference bumps RC and returns the same instance. */\n')
        fd.write('#define AFW_IMPLEMENTATION_ID "managed_' + id + '"\n')
        fd.write('#define AFW_IMPLEMENTATION_INF_LABEL afw_value_managed_' + id + '_inf\n')
        fd.write('#define impl_afw_value_optional_release impl_afw_value_managed_optional_release\n')
        fd.write('#define impl_afw_value_get_reference impl_afw_value_managed_get_reference\n')
        fd.write('#define impl_afw_value_get_assignable_value impl_afw_value_managed_get_reference\n')
        fd.write('#define AFW_VALUE_INF_ONLY 1\n')
        fd.write('#include "afw_value_impl_declares.h"\n')
        fd.write('#undef AFW_IMPLEMENTATION_ID\n')
        fd.write('#undef AFW_IMPLEMENTATION_INF_LABEL\n')
        fd.write('#undef impl_afw_value_optional_release\n')
        fd.write('#undef impl_afw_value_get_reference\n')
        fd.write('#undef impl_afw_value_get_assignable_value\n')
        fd.write('#undef AFW_VALUE_INF_ONLY\n')

        if _supports_managed_slice(ctype):
            fd.write('\n/* Declares and rti/inf defines for interface afw_value */\n')
            fd.write('/* managed_slice ' + id + ': own RC; holds containing; */\n')
            fd.write('/* last release frees slice header via xctx->p. */\n')
            fd.write('#define AFW_IMPLEMENTATION_ID "managed_slice_' + id + '"\n')
            fd.write('#define AFW_IMPLEMENTATION_INF_LABEL afw_value_managed_slice_' + id + '_inf\n')
            fd.write('#define impl_afw_value_optional_release '
                     'impl_afw_value_managed_slice_optional_release\n')
            fd.write('#define impl_afw_value_get_reference '
                     'impl_afw_value_managed_slice_get_reference\n')
            fd.write('#define impl_afw_value_get_assignable_value '
                     'impl_afw_value_managed_slice_get_reference\n')
            fd.write('#define AFW_VALUE_INF_ONLY 1\n')
            fd.write('#include "afw_value_impl_declares.h"\n')
            fd.write('#undef AFW_IMPLEMENTATION_ID\n')
            fd.write('#undef AFW_IMPLEMENTATION_INF_LABEL\n')
            fd.write('#undef impl_afw_value_optional_release\n')
            fd.write('#undef impl_afw_value_get_reference\n')
            fd.write('#undef impl_afw_value_get_assignable_value\n')
            fd.write('#undef AFW_VALUE_INF_ONLY\n')

        fd.write('\n/* Declares and rti/inf defines for interface afw_value */\n')
        fd.write('/* permanent ' + id + ': optional_release NULL; */\n')
        if id in ('object', 'array'):
            fd.write('/* clone_or_reference holds a memory face. */\n')
        else:
            fd.write('/* clone_or_reference returns the same instance as-is. */\n')
        fd.write('#define AFW_IMPLEMENTATION_ID "permanent_' + id + '"\n')
        fd.write('#define AFW_IMPLEMENTATION_INF_LABEL afw_value_permanent_' + id + '_inf\n')
        fd.write('#define impl_afw_value_optional_release NULL\n')
        fd.write('#define impl_afw_value_get_reference impl_afw_value_permanent_get_reference\n')
        if id in ('object', 'array'):
            fd.write('#define impl_afw_value_get_assignable_value '
                     'impl_afw_value_get_assignable_value\n')
        else:
            fd.write('#define impl_afw_value_get_assignable_value impl_afw_value_permanent_get_reference\n')
        fd.write('#define AFW_VALUE_INF_ONLY 1\n')
        fd.write('#include "afw_value_impl_declares.h"\n')
        fd.write('#undef AFW_IMPLEMENTATION_ID\n')
        fd.write('#undef AFW_IMPLEMENTATION_INF_LABEL\n')
        fd.write('#undef impl_afw_value_optional_release\n')
        fd.write('#undef impl_afw_value_get_reference\n')
        fd.write('#undef impl_afw_value_get_assignable_value\n')
        fd.write('#undef AFW_VALUE_INF_ONLY\n')

        if id in ('object', 'array'):
            fd.write('\n/* Declares and rti/inf defines for interface afw_value */\n')
            fd.write('/* assignable ' + id + ': script face; bump instance. */\n')
            fd.write('#define AFW_IMPLEMENTATION_ID "assignable_' + id + '"\n')
            fd.write('#define AFW_IMPLEMENTATION_INF_LABEL afw_value_assignable_' +
                     id + '_inf\n')
            fd.write('#define impl_afw_value_optional_release '
                     'impl_afw_value_unmanaged_optional_release\n')
            fd.write('#define impl_afw_value_get_reference '
                     'impl_afw_value_assignable_get_reference\n')
            fd.write('#define impl_afw_value_get_assignable_value '
                     'impl_afw_value_assignable_get_reference\n')
            fd.write('#define AFW_VALUE_INF_ONLY 1\n')
            fd.write('#include "afw_value_impl_declares.h"\n')
            fd.write('#undef AFW_IMPLEMENTATION_ID\n')
            fd.write('#undef AFW_IMPLEMENTATION_INF_LABEL\n')
            fd.write('#undef impl_afw_value_optional_release\n')
            fd.write('#undef impl_afw_value_get_reference\n')
            fd.write('#undef impl_afw_value_get_assignable_value\n')
            fd.write('#undef AFW_VALUE_INF_ONLY\n')

    else:
        fd.write('\n/* Declares and rti/inf defines for interface afw_value */\n')
        fd.write('/* permanent ' + id + ' (special type): optional_release NULL; */\n')
        fd.write('/* clone_or_reference returns the same instance as-is. */\n')
        fd.write('#define AFW_IMPLEMENTATION_ID "permanent_' + id + '"\n')
        fd.write('#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA\n')
        fd.write('#define AFW_IMPLEMENTATION_INF_LABEL afw_value_permanent_' + id + '_inf\n')
        fd.write('#define impl_afw_value_optional_release NULL\n')
        fd.write('#define impl_afw_value_get_reference impl_afw_value_permanent_get_reference\n')
        fd.write('#define impl_afw_value_get_assignable_value impl_afw_value_permanent_get_reference\n')
        fd.write('#define impl_afw_value_create_iterator NULL\n')
        fd.write('#include "afw_value_impl_declares.h"\n')
        fd.write('#undef AFW_IMPLEMENTATION_ID\n')
        fd.write('#undef AFW_IMPLEMENTATION_INF_LABEL\n')
        fd.write('#undef impl_afw_value_optional_release\n')
        fd.write('#undef impl_afw_value_get_reference\n')
        fd.write('#undef impl_afw_value_get_assignable_value\n')
       

    fd.write('\nstatic const afw_value_string_t\n')
    fd.write('impl_data_type_' + id + '_id_value = {\n')
    fd.write('    {&afw_value_permanent_string_inf},\n')
    fd.write('    AFW_UTF8_LITERAL(\"' + id + '\")\n')
    fd.write('};\n')
   
    fd.write('\n' + define_data + '(afw_value_t *)\n')
    fd.write('afw_data_type_' + id + '_id_value =\n')
    fd.write('{\n    (const afw_value_t *)&impl_data_type_' + id + '_id_value\n};\n')

    fd.write('\nstatic const afw_utf8_t\n')
    fd.write('impl_data_type_path_' + id + ' =\n')
    fd.write('    AFW_UTF8_LITERAL("' + '/afw/_AdaptiveDataType_/' +  id + '");\n')

    # Data type object
    value_label = 'impl_data_type_object_' + id + '__value'
    fd.write('\nstatic const afw_value_object_t\n')
    fd.write(value_label + ';\n')

    fd.write('\n/* Data type ' + id + ' object. */\n')
    fd.write('static const afw_runtime_object_indirect_t\n')
    fd.write('impl_data_type_object_' + id + ' = {\n')
    fd.write('    {\n')
    fd.write('        &afw_runtime_inf__AdaptiveDataType_,\n')
    fd.write('        NULL,\n')
    fd.write('        (const afw_value_t *)&' + value_label + ',\n')
    fd.write('        {\n')
    fd.write('            NULL,\n')
    fd.write('            NULL,\n')
    fd.write('            &' + prefix + 'self_s_' + id + ',\n')
    fd.write('            &' + prefix + 'self_s__AdaptiveDataType_,\n')
    fd.write('            &impl_data_type_path_' + id + '\n')
    fd.write('        }\n')
    fd.write('    },\n')
    fd.write('    (void *)&' + prefix + 'data_type_' + id + '_direct\n')
    fd.write('};\n')

    fd.write('\nstatic const afw_value_object_t\n')
    fd.write(value_label + ' = {\n')
    fd.write('    {&afw_value_permanent_object_inf},\n')
    fd.write('    (const afw_object_t *)&impl_data_type_object_' + id +'\n')
    fd.write('};\n')

    # Declare for empty array of this data type
    fd.write('\n/* Value for empty array of ' + id + '. */\n')
    fd.write('const afw_array_view_of_c_array_self_t\n')
    fd.write('impl_empty_array_of_' + id + ';\n')
    
    # Declare for empty array value of this data type
    fd.write('\n/* Value for empty array of ' + id + '. */\n')
    fd.write('const afw_value_array_t\n')
    fd.write('impl_value_empty_array_of_' + id + ';\n')
    
    # Data type (public const object)
    fd.write('\n/* Data type ' + id + ' instance. */\n')
    fd.write(define_data + '(afw_data_type_t)\n')
    fd.write('afw_data_type_' + id + '_direct = {\n')
    fd.write('    &' + prefix + 'data_type_' + id + '_inf,\n')

    # object
    fd.write('    (const afw_object_t *)&impl_data_type_object_' + id + ',\n')

    # data_type_id
    fd.write('    AFW_UTF8_LITERAL("' + id + '"),\n')

    # brief
    if obj.get('brief') is not None:
        fd.write('    AFW_UTF8_LITERAL(' + c.make_quoted(obj.get('brief'))  + '),\n')
    else:
        fd.write('    AFW_UTF8_LITERAL(""),\n')

    # description
    if obj.get('description') is not None:
        fd.write('    AFW_UTF8_LITERAL(' + c.make_quoted(obj.get('description'))  + '),\n')
    else:
        fd.write('    AFW_UTF8_LITERAL(""),\n')

    # data_type_parameter_type
    if obj.get('dataTypeParameterType') is not None:
        fd.write('    AFW_UTF8_LITERAL(' + c.make_quoted(obj.get('dataTypeParameterType'))  + '),\n')
    else:
        fd.write('    AFW_UTF8_LITERAL(""),\n')

    # data_type_number
    fd.write('    afw_data_type_number_' + id + ',\n')

    # ldapOid
    fd.write('    AFW_UTF8_LITERAL("' + obj.get('ldapOid', '') + '"),\n')

    # jsonPrimitive
    fd.write('    AFW_UTF8_LITERAL("' + obj.get('jsonPrimitive', '') + '"),\n')

    # jsonSchemaStringFormat
    fd.write('    AFW_UTF8_LITERAL("' + obj.get('jsonSchemaStringFormat', '') + '"),\n')

    # cType
    fd.write('    AFW_UTF8_LITERAL("' + obj.get('cType', '') + '"),\n')

    # c_type_size
    fd.write('    sizeof(' + obj.get('cType', 'void *') + '),\n')

    if special:
        fd.write('    NULL,\n')
        fd.write('    NULL,\n')
        fd.write('    NULL,\n')
    else:
        # empty_array
        fd.write('    (const afw_array_t *)&impl_empty_array_of_' + id +',\n')

        # empty_array_value
        fd.write('    (const afw_value_t *)&impl_value_empty_array_of_' + id +',\n')

        # evaluated_value_inf
        fd.write('    &afw_value_unmanaged_' + id + '_inf,\n')

    # compile_type
    if obj.get('compileType'):
        fd.write('    afw_compile_type_' + obj.get('compileType') + ',\n')
    else:
        fd.write('    afw_compile_type_error,\n')

    # json_implies_data_type
    if obj.get('jsonImpliesDataType', False) == True:
        fd.write('    true,\n')
    else:
        fd.write('    false,\n')

    # evaluated
    if obj.get('evaluated', False) == True:
        fd.write('    true,\n')
    else:
        fd.write('    false,\n')

    # directReturn
    if obj.get('directReturn', False) == True:
        fd.write('    true,\n')
    else:
        fd.write('    false,\n')

    # relationalCompares
    if obj.get('relationalCompares', False) == True:
        fd.write('    true,\n')
    else:
        fd.write('    false,\n')

    # scalar
    if obj.get('scalar', False) == True:
        fd.write('    true,\n')
    else:
        fd.write('    false,\n')

    # special
    if obj.get('special', False) == True:
        fd.write('    true,\n')
    else:
        fd.write('    false,\n')

    # iterator_return_data_type (const afw_data_type_t *; generate id or NULL)
    irt = obj.get('iteratorReturnDataType')
    if irt:
        fd.write('    &afw_data_type_' + irt + '_direct\n')
    else:
        fd.write('    NULL\n')

    fd.write('};\n')

    if not special:

        # Define for empty array of this data type
        fd.write('\n/* Value for empty array of ' + id + '. */\n')
        fd.write('const afw_array_view_of_c_array_self_t\n')
        fd.write('impl_empty_array_of_' + id + ' = {\n')
        fd.write('    {\n')
        fd.write('        &afw_array_view_of_c_array_inf,\n')
        fd.write('        NULL,\n')
        fd.write('        (const afw_value_t *)&impl_value_empty_array_of_' + id + '\n')
        fd.write('    },\n')
        fd.write('    &afw_data_type_' + id + '_direct,\n')
        fd.write('    0\n')
        fd.write('};\n')
        
        # Define for empty array of this data type
        fd.write('\n/* Value for empty array of ' + id + '. */\n')
        fd.write('const afw_value_array_t\n')
        fd.write('impl_value_empty_array_of_' + id + ' = {\n')
        fd.write('    {&afw_value_permanent_array_inf},\n')
        fd.write('    (const afw_array_t *)&impl_empty_array_of_' + id + '\n')
        fd.write('};\n')
    
    fd.write('\n/* Data type struct for ' + id + '. */\n')
    fd.write(define_data + '(afw_data_type_t *)\n')
    fd.write('afw_data_type_' + id + ' =\n    &afw_data_type_' + id + '_direct;\n')

    if not special:

        fd.write('\n/* Set property function for data type ' + id + ' values. */\n')
        fd.write(define + '(void)\n')
        fd.write('afw_object_set_property_as_' + id + '(\n')
        fd.write('    const afw_object_t *object,\n')
        fd.write('    const afw_value_t *property_name,\n')
        fd.write('    ' + return_type + ' internal,\n')
        fd.write('    afw_xctx_t *xctx)\n')
        fd.write('{\n')
        fd.write('    const afw_value_t *v;\n')
        fd.write('\n')
        fd.write('    if (!object->p) {\n')
        fd.write('        AFW_THROW_ERROR_Z(general,\n')
        fd.write('            "Object must have a pool",\n')
        fd.write('            xctx);\n')
        fd.write('    }\n')
        fd.write('\n')
        if id == 'boolean':
            # Permanent true/false — no per-call pool allocation.
            fd.write('    v = afw_value_for_boolean(internal);\n')
        elif id == 'integer':
            # Permanent 0 and 1 from strings.txt integer::zero / integer::one.
            fd.write('    if (internal == 0) {\n')
            fd.write('        v = afw_integer_v_zero;\n')
            fd.write('    }\n')
            fd.write('    else if (internal == 1) {\n')
            fd.write('        v = afw_integer_v_one;\n')
            fd.write('    }\n')
            fd.write('    else {\n')
            fd.write('        v = ' + _unmanaged_create_fn(id) +
                     '(internal, object->p, xctx);\n')
            fd.write('    }\n')
        elif id == 'null':
            # Permanent singleton — preserve address identity.
            fd.write('    v = afw_value_null;\n')
        else:
            fd.write('    v = ' + _unmanaged_create_fn(id) +
                     '(internal, object->p, xctx);\n')
        fd.write('    afw_object_set_property(object, property_name, v, xctx);\n')
        fd.write('}\n')

        fd.write('\n/* Typesafe cast of data type ' + id + '. */\n')
        fd.write(define + '(' + return_type + ')\n')
        fd.write('afw_value_as_' + id + '(const afw_value_t *value, afw_xctx_t *xctx)\n')
        fd.write('{\n')
        fd.write('    value = afw_value_evaluate(value, xctx->p, xctx);\n')
        fd.write('    if (!AFW_VALUE_IS_DATA_TYPE(value, ' + id + '))\n')
        fd.write('    {\n')
        fd.write('        const afw_utf8_t *data_type_id;\n')
        fd.write('\n')
        fd.write('        if (!value) {\n')
        fd.write('            AFW_THROW_ERROR_Z(general,\n')
        fd.write('                "Typesafe error: expecting \'' + id + '\' but "\n')
        fd.write('                "encountered \'undefined\'",\n')
        fd.write('                xctx);\n')
        fd.write('        }\n')
        fd.write('\n')
        fd.write('        data_type_id = afw_value_get_quick_data_type_id(value);\n')
        fd.write('        AFW_THROW_ERROR_FZ(general, xctx,\n')
        fd.write('            "Typesafe error: expecting \'' + id + '\' but "\n')
        fd.write('            "encountered " AFW_UTF8_FMT_Q ,\n')
        fd.write('            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));\n')
        fd.write('    }\n')
        fd.write('    return ' + amp_if_needed + '(((const afw_value_' + id + '_t *)value)->internal);\n')
        fd.write('}\n')

        fd.write('\n/* Allocate function for data type ' + id + ' values. */\n')
        fd.write(define + '(afw_value_' + id + '_t *)\n')
        fd.write(_allocate_fn(id) + '(const afw_pool_t *p, afw_xctx_t *xctx)\n')
        fd.write('{\n')
        fd.write('    afw_value_' + id + '_t *result;\n')
        fd.write('\n')
        fd.write('    result = afw_pool_calloc(p, sizeof(afw_value_' + id + '_t),\n')
        fd.write('        xctx);\n')
        fd.write('    result->inf = &afw_value_unmanaged_' + id + '_inf;\n')
        fd.write('    return result;\n')
        fd.write('}\n')

        fd.write('\n/* Create function for managed data type ' + id + ' value. */\n')
        fd.write(define + '(const afw_value_t *)\n')
        fd.write(_managed_create_fn(id) + '(\n    ' + return_type + ' internal,\n')
        fd.write('    afw_xctx_t *xctx)\n')
        fd.write('{\n')
        if id == 'null':
            # Singleton: preserve address identity for is/compare patterns.
            fd.write('    /* Permanent singleton; internal unused. */\n')
            fd.write('    (void)internal;\n')
            fd.write('    (void)xctx;\n')
            fd.write('    return afw_value_null;\n')
            fd.write('}\n')
        elif id == 'boolean':
            # Only two Adaptive booleans — permanent dual (intentional).
            fd.write('    /* Permanent true/false; no managed header. */\n')
            fd.write('    (void)xctx;\n')
            fd.write('    return afw_value_for_boolean(internal);\n')
            fd.write('}\n')
        else:
            fd.write('    afw_value_' + id + '_managed_t *v;\n')
            if ctype == 'afw_utf8_t':
                fd.write('    afw_size_t len;\n')
                fd.write('\n')
                fd.write('    len = (internal) ? internal->len : 0;\n')
                fd.write('    v = afw_pool_calloc(xctx->p,\n')
                fd.write('        sizeof(afw_value_' + id +
                         '_managed_t) + len, xctx);\n')
                fd.write('    v->inf = &afw_value_managed_' + id + '_inf;\n')
                fd.write('    v->internal.len = len;\n')
                fd.write('    v->internal.s = (const afw_utf8_octet_t *)v +\n'
                         '        sizeof(afw_value_' + id + '_managed_t);\n')
                fd.write('    if (internal && internal->s) {\n')
                fd.write('        memcpy((void *)v->internal.s, '
                         'internal->s, len);\n')
                fd.write('    }\n')
            elif ctype == 'afw_memory_t':
                fd.write('    afw_size_t size;\n')
                fd.write('\n')
                fd.write('    size = (internal) ? internal->size : 0;\n')
                fd.write('    v = afw_pool_calloc(xctx->p,\n')
                fd.write('        sizeof(afw_value_' + id +
                         '_managed_t) + size, xctx);\n')
                fd.write('    v->inf = &afw_value_managed_' + id + '_inf;\n')
                fd.write('    v->internal.size = (internal) ? '
                         'internal->size : 0;\n')
                fd.write('    v->internal.ptr = (const afw_byte_t *)v +\n'
                         '        sizeof(afw_value_' + id + '_managed_t);\n')
                fd.write('    if (internal && internal->ptr) {\n')
                fd.write('       memcpy((void *)v->internal.ptr, '
                         'internal->ptr, size);\n')
                fd.write('    }\n')
            elif direct_return == True:
                fd.write('\n')
                # Stores internal as-is. Pointer cTypes: no clone of referent.
                if id == 'object':
                    # Heap wrapper holds a container reference (1c).
                    # Require non-NULL: soft skip would leave a managed face
                    # that SEGV/misbehaves on release/get_reference later.
                    fd.write('    if (!internal) {\n')
                    fd.write('        AFW_THROW_ERROR_Z(general,\n')
                    fd.write('            "internal object required for "\n')
                    fd.write('            "managed object value",\n')
                    fd.write('            xctx);\n')
                    fd.write('    }\n')
                    fd.write('    afw_object_get_reference(internal, xctx);\n')
                elif id == 'array':
                    fd.write('    if (!internal) {\n')
                    fd.write('        AFW_THROW_ERROR_Z(general,\n')
                    fd.write('            "internal array required for "\n')
                    fd.write('            "managed array value",\n')
                    fd.write('            xctx);\n')
                    fd.write('    }\n')
                if _scalar_holdable_create(id):
                    fd.write('    v = afw_pool_calloc(xctx->p,\n')
                    fd.write('        sizeof(afw_value_' + id +
                             '_managed_t), xctx);\n')
                else:
                    fd.write('    v = afw_xctx_malloc(\n')
                    fd.write('        sizeof(afw_value_' + id +
                             '_managed_t), xctx);\n')
                fd.write('    v->inf = &afw_value_managed_' + id + '_inf;\n')
                fd.write('    v->internal = internal;\n')
                if id == 'object' or id == 'array':
                    fd.write('    /* Container hold is on object/array, not value RC. */\n')
                    fd.write('    v->reference_count = 0;\n')
            else:
                fd.write('\n')
                if _scalar_holdable_create(id):
                    fd.write('    v = afw_pool_calloc(xctx->p,\n')
                    fd.write('        sizeof(afw_value_' + id +
                             '_managed_t), xctx);\n')
                else:
                    fd.write('    v = afw_xctx_calloc(\n')
                    fd.write('        sizeof(afw_value_' + id +
                             '_managed_t), xctx);\n')
                fd.write('    v->inf = &afw_value_managed_' + id + '_inf;\n')
                fd.write('    if (internal) {\n')
                fd.write('        memcpy(&v->internal, internal, '
                         'sizeof(' + ctype + '));\n')
                fd.write('    }\n')
            if _scalar_holdable_create(id):
                fd.write('    v->reference_count = 1;\n')
            fd.write('\n')
            fd.write('    return &v->pub;\n')
            fd.write('}\n')

        if ctype == 'afw_utf8_t':
            fd.write('\n/* Create function for managed data type ' + id + ' slice value. */\n')
            fd.write(define + '(const afw_value_t *)\n')
            fd.write(_managed_slice_fn(id) + '(\n')
            fd.write('    const afw_value_t *containing_value,\n')
            fd.write('    afw_size_t offset,\n')
            fd.write('    afw_size_t len,\n')
            fd.write('    afw_xctx_t *xctx)\n')
            fd.write('{\n')
            fd.write('    const afw_value_' + id + '_managed_t *containing;\n')
            fd.write('    const afw_utf8_t *base;\n')
            fd.write('    afw_value_' + id + '_managed_slice_t *v;\n')
            fd.write('\n')
            fd.write('    if (!containing_value) {\n')
            fd.write('        AFW_THROW_ERROR_Z(general,\n')
            fd.write('            "containing_value required for managed slice", xctx);\n')
            fd.write('    }\n')
            fd.write('    if (containing_value->inf == &afw_value_managed_slice_' + id + '_inf) {\n')
            fd.write('        const afw_value_' + id + '_managed_slice_t *slice =\n')
            fd.write('            (const afw_value_' + id + '_managed_slice_t *)containing_value;\n')
            fd.write('        containing = slice->containing_value;\n')
            fd.write('        base = &slice->internal;\n')
            fd.write('    }\n')
            fd.write('    else if (containing_value->inf == &afw_value_managed_' + id + '_inf) {\n')
            fd.write('        containing =\n')
            fd.write('            (const afw_value_' + id + '_managed_t *)containing_value;\n')
            fd.write('        base = &containing->internal;\n')
            fd.write('    }\n')
            fd.write('    else {\n')
            fd.write('        AFW_THROW_ERROR_Z(general,\n')
            fd.write('            "containing_value must be managed or managed_slice ' + id + '",\n')
            fd.write('            xctx);\n')
            fd.write('    }\n')
            fd.write('    if (offset > base->len || len > base->len - offset) {\n')
            fd.write('        AFW_THROW_ERROR_Z(general,\n')
            fd.write('            "managed slice offset/len out of range", xctx);\n')
            fd.write('    }\n')
            fd.write('    v = afw_pool_calloc(xctx->p, sizeof(afw_value_' + id +
                     '_managed_slice_t), xctx);\n')
            fd.write('    v->inf = &afw_value_managed_slice_' + id + '_inf;\n')
            fd.write('    v->internal.s = base->s + offset;\n')
            fd.write('    v->internal.len = len;\n')
            fd.write('    v->containing_value = containing;\n')
            fd.write('    v->reference_count = 1;\n')
            fd.write('    afw_value_add_reference(&containing->pub, xctx->p,\n')
            fd.write('        xctx);\n')
            fd.write('    return &v->pub;\n')
            fd.write('}\n')
        elif ctype == 'afw_memory_t':
            fd.write('\n/* Create function for managed data type ' + id + ' slice value. */\n')
            fd.write(define + '(const afw_value_t *)\n')
            fd.write(_managed_slice_fn(id) + '(\n')
            fd.write('    const afw_value_t *containing_value,\n')
            fd.write('    afw_size_t offset,\n')
            fd.write('    afw_size_t size,\n')
            fd.write('    afw_xctx_t *xctx)\n')
            fd.write('{\n')
            fd.write('    const afw_value_' + id + '_managed_t *containing;\n')
            fd.write('    const afw_memory_t *base;\n')
            fd.write('    afw_value_' + id + '_managed_slice_t *v;\n')
            fd.write('\n')
            fd.write('    if (!containing_value) {\n')
            fd.write('        AFW_THROW_ERROR_Z(general,\n')
            fd.write('            "containing_value required for managed slice", xctx);\n')
            fd.write('    }\n')
            fd.write('    if (containing_value->inf == &afw_value_managed_slice_' + id + '_inf) {\n')
            fd.write('        const afw_value_' + id + '_managed_slice_t *slice =\n')
            fd.write('            (const afw_value_' + id + '_managed_slice_t *)containing_value;\n')
            fd.write('        containing = slice->containing_value;\n')
            fd.write('        base = &slice->internal;\n')
            fd.write('    }\n')
            fd.write('    else if (containing_value->inf == &afw_value_managed_' + id + '_inf) {\n')
            fd.write('        containing =\n')
            fd.write('            (const afw_value_' + id + '_managed_t *)containing_value;\n')
            fd.write('        base = &containing->internal;\n')
            fd.write('    }\n')
            fd.write('    else {\n')
            fd.write('        AFW_THROW_ERROR_Z(general,\n')
            fd.write('            "containing_value must be managed or managed_slice ' + id + '",\n')
            fd.write('            xctx);\n')
            fd.write('    }\n')
            fd.write('    if (offset > base->size || size > base->size - offset) {\n')
            fd.write('        AFW_THROW_ERROR_Z(general,\n')
            fd.write('            "managed slice offset/size out of range", xctx);\n')
            fd.write('    }\n')
            fd.write('    v = afw_pool_calloc(xctx->p, sizeof(afw_value_' + id +
                     '_managed_slice_t), xctx);\n')
            fd.write('    v->inf = &afw_value_managed_slice_' + id + '_inf;\n')
            fd.write('    v->internal.ptr = base->ptr + offset;\n')
            fd.write('    v->internal.size = size;\n')
            fd.write('    v->containing_value = containing;\n')
            fd.write('    v->reference_count = 1;\n')
            fd.write('    afw_value_add_reference(&containing->pub, xctx->p,\n')
            fd.write('        xctx);\n')
            fd.write('    return &v->pub;\n')
            fd.write('}\n')

        fd.write('\n/* Create function for data type ' + id + ' value. */\n')
        fd.write(define + '(const afw_value_t *)\n')
        fd.write(_unmanaged_create_fn(id) + '(' + return_type + ' internal,\n')
        fd.write('    const afw_pool_t *p, afw_xctx_t *xctx)\n')
        fd.write('{\n')
        if id == 'null':
            fd.write('    /* Permanent singleton; internal/p unused. */\n')
            fd.write('    (void)internal;\n')
            fd.write('    (void)p;\n')
            fd.write('    (void)xctx;\n')
            fd.write('    return afw_value_null;\n')
            fd.write('}\n')
        elif id == 'boolean':
            fd.write('    /* Permanent true/false; no pool header. */\n')
            fd.write('    (void)p;\n')
            fd.write('    (void)xctx;\n')
            fd.write('    return afw_value_for_boolean(internal);\n')
            fd.write('}\n')
        else:
            fd.write('    afw_value_' + id + '_t *v;\n')
            fd.write('\n')
            fd.write('    v = afw_pool_calloc(p, sizeof(afw_value_' + id + '_t),\n')
            fd.write('        xctx);\n')
            fd.write('    v->inf = &afw_value_unmanaged_' + id + '_inf;\n')
            if direct_return == True:
                fd.write('    v->internal = internal;\n')
            else:
                fd.write('    if (internal) {\n')
                fd.write('        memcpy(&v->internal, internal, '
                         'sizeof(' + ctype + '));\n')
                fd.write('    }\n')
            fd.write('    return &v->pub;\n')
            fd.write('}\n')

        fd.write('\n/* Convert data type ' + id + ' string to ' + ctype + ' *. */\n')
        fd.write(define + '(void)\nafw_data_type_' + id + '_to_internal(' + ctype + ' *to_internal,\n')
        fd.write('    const afw_utf8_t *from_utf8, const afw_pool_t *p, afw_xctx_t *xctx)\n')
        fd.write('{\n')
        fd.write('    afw_data_type_utf8_to_internal(\n')
        fd.write('        &afw_data_type_' + id + '_direct,\n')
        fd.write('        (void *)to_internal, from_utf8, p, xctx);\n')
        fd.write('}\n')

        fd.write('\n/*  Convert data type ' + id + ' internal representation to utf-8. */\n')
        fd.write(define + '(const afw_utf8_t *)\nafw_data_type_' + id + '_to_utf8(' + return_type + ' internal,\n')
        fd.write('    const afw_pool_t *p, afw_xctx_t *xctx)\n')
        fd.write('{\n')
        fd.write('    return afw_data_type_internal_to_utf8(\n')
        fd.write('        &afw_data_type_' + id + '_direct,\n')
        if direct_return == True:
            fd.write('        &internal, p, xctx);\n')
        else:
            fd.write('        internal, p, xctx);\n')

        fd.write('}\n')

        if needs_found:
            fd.write('\n/* Get property function for data type ' + id + ' values. */\n')
            fd.write(define + '(' + return_type + ')\n')
            fd.write('afw_object_get_property_as_' + id + '_source(\n')
            fd.write('    const afw_object_t *object,\n')
            fd.write('    const afw_value_t *property_name,\n')
            fd.write('    afw_boolean_t *found,\n')
            fd.write('    const afw_utf8_z_t *source_z,\n')
            fd.write('    const afw_pool_t *p,\n')
            fd.write('    afw_xctx_t *xctx)\n')
            fd.write('{\n')
            fd.write('    const afw_value_t *value;\n')
            fd.write('\n')
            fd.write('    *found = false;\n')
            fd.write('    value = afw_object_get_property(object, property_name, xctx);\n')
            fd.write('    if (!value) {\n')
            fd.write('        return 0;\n')
            fd.write('    }\n')
            fd.write('\n')
            fd.write('    value = afw_value_evaluate(value, p, xctx);\n')
            fd.write('    *found = true;\n')
            fd.write('    if (!AFW_VALUE_IS_DATA_TYPE(value, ' + id + '))\n')
            fd.write('    {\n')
            fd.write('        const afw_utf8_t *data_type_id;\n')
            fd.write('\n')
            fd.write('        data_type_id = afw_value_get_quick_data_type_id(value);\n')
            fd.write('        afw_error_set_fz(afw_error_code_general, source_z, xctx,\n')
            fd.write('            "Typesafe error: expecting \'' + id + '\' but "\n')
            fd.write('            "encountered " AFW_UTF8_FMT_Q,\n')
            fd.write('            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));\n')
            fd.write('        longjmp(((xctx)->current_try->throw_jmp_buf), afw_error_code_general);\n')
            fd.write('    }\n')
            fd.write('    return ' + amp_if_needed + '(((const afw_value_' + id + '_t *)value)->internal);\n')
            fd.write('}\n')

        else:
            fd.write('\n/* Get property function for data type ' + id + ' values. */\n')
            fd.write(define + '(' + return_type + ')\n')
            fd.write('afw_object_get_property_as_' + id + '_source(\n')
            fd.write('    const afw_object_t *object,\n')
            fd.write('    const afw_value_t *property_name,\n')
            fd.write('    const afw_utf8_z_t *source_z,\n')
            fd.write('    const afw_pool_t *p,\n')
            fd.write('    afw_xctx_t *xctx)\n')
            fd.write('{\n')
            fd.write('    const afw_value_t *value;\n')
            fd.write('\n')
            fd.write('    value = afw_object_get_property(object, property_name, xctx);\n')
            fd.write('    if (!value) {\n')
            fd.write('        return NULL;\n')
            fd.write('    }\n')
            fd.write('\n')
            fd.write('    value = afw_value_evaluate(value, p, xctx);\n')
            fd.write('    if (!AFW_VALUE_IS_DATA_TYPE(value, ' + id + '))\n')
            fd.write('    {\n')
            fd.write('        const afw_utf8_t *data_type_id;\n')
            fd.write('\n')
            fd.write('        data_type_id = afw_value_get_quick_data_type_id(value);\n')
            fd.write('        afw_error_set_fz(afw_error_code_general, source_z, xctx,\n')
            fd.write('            "Typesafe error: expecting \'' + id + '\' but "\n')
            fd.write('            "encountered " AFW_UTF8_FMT_Q,\n')
            fd.write('            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));\n')
            fd.write('        longjmp(((xctx)->current_try->throw_jmp_buf), afw_error_code_general);\n')
            fd.write('    }\n')
            fd.write('    return ' + amp_if_needed + '(((const afw_value_' + id + '_t *)value)->internal);\n')
            fd.write('}\n')

        if needs_found:
            fd.write('\n/* Get next property function for data type ' + id + ' values. */\n')
            fd.write(define + '(' + return_type + ')\n')
            fd.write('afw_object_get_next_property_as_' + id + '_source(\n')
            fd.write('    const afw_object_t *object,\n')
            fd.write('    const afw_iterator_old_t * *iterator,\n')
            fd.write('    const afw_value_t * *property_name,\n')
            fd.write('    afw_boolean_t *found,\n')
            fd.write('    const afw_utf8_z_t *source_z,\n')
            fd.write('    const afw_pool_t *p,\n')
            fd.write('    afw_xctx_t *xctx)\n')
            fd.write('{\n')
            fd.write('    const afw_value_t *value;\n')
            fd.write('\n')
            fd.write('    *found = false;\n')
            fd.write('    value = afw_object_get_next_property(object, iterator, property_name, xctx);\n')
            fd.write('    if (!value) {\n')
            fd.write('        return 0;\n')
            fd.write('    }\n')
            fd.write('\n')
            fd.write('    value = afw_value_evaluate(value, p, xctx);\n')
            fd.write('    *found = true;\n')
            fd.write('    if (!AFW_VALUE_IS_DATA_TYPE(value, ' + id + '))\n')
            fd.write('    {\n')
            fd.write('        const afw_utf8_t *data_type_id;\n')
            fd.write('\n')
            fd.write('        data_type_id = afw_value_get_quick_data_type_id(value);\n')
            fd.write('        afw_error_set_fz(afw_error_code_general, source_z, xctx,\n')
            fd.write('            "Typesafe error: expecting \'' + id + '\' but "\n')
            fd.write('            "encountered " AFW_UTF8_FMT_Q,\n')
            fd.write('            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));\n')
            fd.write('        longjmp(((xctx)->current_try->throw_jmp_buf), afw_error_code_general);\n')
            fd.write('    }\n')
            fd.write('    return ' + amp_if_needed + '(((const afw_value_' + id + '_t *)value)->internal);\n')
            fd.write('}\n')

        else:
            fd.write('\n/* Get next property function for data type ' + id + ' values. */\n')
            fd.write(define + '(' + return_type + ')\n')
            fd.write('afw_object_get_next_property_as_' + id + '_source(\n')
            fd.write('    const afw_object_t *object,\n')
            fd.write('    const afw_iterator_old_t * *iterator,\n')
            fd.write('    const afw_value_t * *property_name,\n')
            fd.write('    const afw_utf8_z_t *source_z,\n')
            fd.write('    const afw_pool_t *p,\n')
            fd.write('    afw_xctx_t *xctx)\n')
            fd.write('{\n')
            fd.write('    const afw_value_t *value;\n')
            fd.write('\n')
            fd.write('    value = afw_object_get_next_property(object, iterator, property_name, xctx);\n')
            fd.write('    if (!value) {\n')
            fd.write('        return NULL;\n')
            fd.write('    }\n')
            fd.write('\n')
            fd.write('    value = afw_value_evaluate(value, p, xctx);\n')
            fd.write('    if (!AFW_VALUE_IS_DATA_TYPE(value, ' + id + '))\n')
            fd.write('    {\n')
            fd.write('        const afw_utf8_t *data_type_id;\n')
            fd.write('\n')
            fd.write('        data_type_id = afw_value_get_quick_data_type_id(value);\n')
            fd.write('        afw_error_set_fz(afw_error_code_general, source_z, xctx,\n')
            fd.write('            "Typesafe error: expecting \'' + id + '\' but "\n')
            fd.write('            "encountered " AFW_UTF8_FMT_Q,\n')
            fd.write('            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));\n')
            fd.write('        longjmp(((xctx)->current_try->throw_jmp_buf), afw_error_code_general);\n')
            fd.write('    }\n')
            fd.write('    return ' + amp_if_needed + '(((const afw_value_' + id + '_t *)value)->internal);\n')
            fd.write('}\n')

        fd.write('\n')
        fd.write('/* Implementation of method optional_release for managed value. */\n')
        fd.write('AFW_DECLARE_STATIC(void)\n')
        fd.write('impl_afw_value_managed_optional_release(\n')
        fd.write('    const afw_value_t *instance,\n')
        fd.write('    afw_xctx_t *xctx)\n')
        fd.write('{\n')
        if id == 'object':
            # Container-aware: object RC is the hold; heap wrappers still
            # use value reference_count only to free the header (multi-ref).
            # Never free an embedded dual-face value (no managed header).
            fd.write('    const afw_value_object_t *self =\n')
            fd.write('        (const afw_value_object_t *)instance;\n')
            fd.write('    const afw_object_t *obj = self->internal;\n')
            fd.write('    afw_boolean_t embedded;\n')
            fd.write('\n')
            fd.write('    /* Embedded when instance is the object\'s dual face. */\n')
            fd.write('    embedded = (obj && obj->value == instance);\n')
            fd.write('\n')
            fd.write('    if (obj) {\n')
            fd.write('        /* Paired with managed clone_or_reference. */\n')
            fd.write('        afw_object_release(obj, xctx);\n')
            fd.write('    }\n')
            fd.write('\n')
            fd.write('    /* Free only heap wrappers; dual face has no freeable header. */\n')
            fd.write('    if (!embedded) {\n')
            fd.write('        afw_value_object_managed_t *managed =\n')
            fd.write('            (afw_value_object_managed_t *)instance;\n')
            fd.write('        if (managed->reference_count == 0) {\n')
            fd.write('            return;\n')
            fd.write('        }\n')
            fd.write('        managed->reference_count--;\n')
            fd.write('    }\n')
        elif id == 'array':
            # Arrays often have no get_reference; release is often a no-op.
            # Never free an embedded dual-face value. Heap wrappers use RC
            # only for header free (same multi-ref rule as other managed).
            fd.write('    const afw_value_array_t *self =\n')
            fd.write('        (const afw_value_array_t *)instance;\n')
            fd.write('    const afw_array_t *arr = self->internal;\n')
            fd.write('    afw_boolean_t embedded;\n')
            fd.write('\n')
            fd.write('    embedded = (arr && arr->value == instance);\n')
            fd.write('\n')
            fd.write('    if (arr) {\n')
            fd.write('        afw_array_release(arr, xctx);\n')
            fd.write('    }\n')
            fd.write('\n')
            fd.write('    if (!embedded) {\n')
            fd.write('        afw_value_array_managed_t *managed =\n')
            fd.write('            (afw_value_array_managed_t *)instance;\n')
            fd.write('        if (managed->reference_count == 0) {\n')
            fd.write('            return;\n')
            fd.write('        }\n')
            fd.write('        managed->reference_count--;\n')
            fd.write('    }\n')
        else:
            fd.write('    afw_value_' + id + '_managed_t *self =\n')
            fd.write('        (afw_value_' + id + '_managed_t *)instance;\n')
            fd.write('\n')
            fd.write('    if (self->reference_count == 0) {\n')
            fd.write('        return;\n')
            fd.write('    }\n')
            fd.write('    self->reference_count--;\n')
            if _scalar_holdable_create(id):
                fd.write('    if (self->reference_count == 0) {\n')
                fd.write('        afw_pool_free_memory(xctx->p, self,\n')
                fd.write('            ' + _managed_free_size_expr(id, ctype) +
                         ', xctx);\n')
                fd.write('    }\n')
        fd.write('}\n')

        if id in ('object', 'array'):
            fd.write('\n')
            fd.write('/* Implementation of method optional_release for '
                     'unmanaged value. */\n')
            fd.write('AFW_DECLARE_STATIC(void)\n')
            fd.write('impl_afw_value_unmanaged_optional_release(\n')
            fd.write('    const afw_value_t *instance,\n')
            fd.write('    afw_xctx_t *xctx)\n')
            fd.write('{\n')
            if id == 'object':
                fd.write('    const afw_value_object_t *self =\n')
                fd.write('        (const afw_value_object_t *)instance;\n')
                fd.write('\n')
                fd.write('    if (self->internal) {\n')
                fd.write('        afw_object_release(self->internal, xctx);\n')
                fd.write('    }\n')
            else:
                fd.write('    const afw_value_array_t *self =\n')
                fd.write('        (const afw_value_array_t *)instance;\n')
                fd.write('\n')
                fd.write('    if (self->internal) {\n')
                fd.write('        afw_array_release(self->internal, xctx);\n')
                fd.write('    }\n')
            fd.write('}\n')
        
        fd.write('\n')
        fd.write('/* Implementation of method get_reference for unmanaged value. */\n')
        fd.write('AFW_DECLARE_STATIC(const afw_value_t *)\n')
        fd.write('impl_afw_value_get_reference(\n')
        fd.write('    const afw_value_t *instance,\n')
        fd.write('    const afw_pool_t *p,\n')
        fd.write('    afw_xctx_t *xctx)\n')
        fd.write('{\n')
        # Null sentinels (model useDefaultProcessing) are unmanaged null
        # with pointer identity; boxing would collapse them to afw_value_null.
        if obj.get('scalar', False) == True and id != 'null':
            fd.write('    const afw_value_' + id + '_t *self =\n')
            fd.write('        (const afw_value_' + id + '_t *)instance;\n')
            fd.write('\n')
            fd.write('    (void)p;\n')
            if id == 'boolean':
                fd.write('    return afw_value_for_boolean(self->internal);\n')
            elif ctype == 'afw_utf8_t' or ctype == 'afw_memory_t':
                fd.write('    return ' + _managed_create_fn(id) + '(\n')
                fd.write('        &self->internal, xctx);\n')
            elif direct_return:
                fd.write('    return ' + _managed_create_fn(id) + '(\n')
                fd.write('        self->internal, xctx);\n')
            else:
                fd.write('    return ' + _managed_create_fn(id) + '(\n')
                fd.write('        &self->internal, xctx);\n')
        else:
            if id == 'null':
                fd.write('    /* Unmanaged null sentinels keep pointer identity. */\n')
                fd.write('    (void)p;\n')
                fd.write('    (void)xctx;\n')
                fd.write('    return instance;\n')
            elif id == 'object':
                fd.write('    const afw_value_object_t *self =\n')
                fd.write('        (const afw_value_object_t *)instance;\n')
                fd.write('\n')
                fd.write('    (void)p;\n')
                fd.write('    if (self->internal) {\n')
                fd.write('        afw_object_get_reference(self->internal, xctx);\n')
                fd.write('    }\n')
                fd.write('    return instance;\n')
            elif id == 'array':
                fd.write('    const afw_value_array_t *self =\n')
                fd.write('        (const afw_value_array_t *)instance;\n')
                fd.write('\n')
                fd.write('    (void)p;\n')
                fd.write('    if (self->internal) {\n')
                fd.write('        afw_array_get_reference(self->internal, xctx);\n')
                fd.write('    }\n')
                fd.write('    return instance;\n')
            else:
                fd.write('    /* Function: instance hold is a later slice. */\n')
                fd.write('    (void)p;\n')
                fd.write('    (void)xctx;\n')
                fd.write('    return instance;\n')
        fd.write('}\n')
        fd.write('\n')

        if id in ('object', 'array'):
            fd.write('/* Slot fill: mint an assignable face. */\n')
            fd.write('AFW_DECLARE_STATIC(const afw_value_t *)\n')
            fd.write('impl_afw_value_get_assignable_value(\n')
            fd.write('    const afw_value_t *instance,\n')
            fd.write('    const afw_pool_t *p,\n')
            fd.write('    afw_xctx_t *xctx)\n')
            fd.write('{\n')
            if id == 'object':
                fd.write('    return afw_value_object_hold(instance, p, xctx);\n')
            else:
                fd.write('    return afw_value_array_hold(instance, p, xctx);\n')
            fd.write('}\n')
            fd.write('\n')
            fd.write('/* Assignable face: bump instance. */\n')
            fd.write('AFW_DECLARE_STATIC(const afw_value_t *)\n')
            fd.write('impl_afw_value_assignable_get_reference(\n')
            fd.write('    const afw_value_t *instance,\n')
            fd.write('    const afw_pool_t *p,\n')
            fd.write('    afw_xctx_t *xctx)\n')
            fd.write('{\n')
            if id == 'object':
                fd.write('    const afw_value_object_t *self =\n')
                fd.write('        (const afw_value_object_t *)instance;\n')
                fd.write('\n')
                fd.write('    (void)p;\n')
                fd.write('    if (self->internal) {\n')
                fd.write('        afw_object_get_reference(self->internal, xctx);\n')
                fd.write('    }\n')
            else:
                fd.write('    const afw_value_array_t *self =\n')
                fd.write('        (const afw_value_array_t *)instance;\n')
                fd.write('\n')
                fd.write('    (void)p;\n')
                fd.write('    if (self->internal) {\n')
                fd.write('        afw_array_get_reference(self->internal, xctx);\n')
                fd.write('    }\n')
            fd.write('    return instance;\n')
            fd.write('}\n')
            fd.write('\n')
    
        fd.write('\n')
        fd.write('/* Implementation of method get_reference for managed value. */\n')
        fd.write('AFW_DECLARE_STATIC(const afw_value_t *)\n')
        fd.write('impl_afw_value_managed_get_reference(\n')
        fd.write('    const afw_value_t *instance,\n')
        fd.write('    const afw_pool_t *p,\n')
        fd.write('    afw_xctx_t *xctx)\n')
        fd.write('{\n')
        if id == 'object':
            fd.write('    const afw_value_object_t *self =\n')
            fd.write('        (const afw_value_object_t *)instance;\n')
            fd.write('    const afw_object_t *obj = self->internal;\n')
            fd.write('    afw_boolean_t embedded;\n')
            fd.write('\n')
            fd.write('    /* Hold is on the object (pool/entity RC). */\n')
            fd.write('    if (obj) {\n')
            fd.write('        afw_object_get_reference(obj, xctx);\n')
            fd.write('    }\n')
            fd.write('\n')
            fd.write('    /* Heap wrappers: bump value RC so optional_release frees once. */\n')
            fd.write('    embedded = (obj && obj->value == instance);\n')
            fd.write('    if (!embedded) {\n')
            fd.write('        ((afw_value_object_managed_t *)instance)->\n')
            fd.write('            reference_count++;\n')
            fd.write('    }\n')
            fd.write('    return instance;\n')
        elif id == 'array':
            fd.write('    const afw_value_array_t *self =\n')
            fd.write('        (const afw_value_array_t *)instance;\n')
            fd.write('    const afw_array_t *arr = self->internal;\n')
            fd.write('    afw_boolean_t embedded;\n')
            fd.write('\n')
            fd.write('    /* Hold is on the array (pool RC). */\n')
            fd.write('    if (arr) {\n')
            fd.write('        afw_array_get_reference(arr, xctx);\n')
            fd.write('    }\n')
            fd.write('\n')
            fd.write('    /* Heap wrappers: bump value RC so optional_release frees once. */\n')
            fd.write('    (void)p;\n')
            fd.write('    embedded = (arr && arr->value == instance);\n')
            fd.write('    if (!embedded) {\n')
            fd.write('        ((afw_value_array_managed_t *)instance)->\n')
            fd.write('            reference_count++;\n')
            fd.write('    }\n')
            fd.write('    return instance;\n')
        else:
            fd.write('    afw_value_' + id + '_managed_t *self =\n')
            fd.write('        (afw_value_' + id + '_managed_t *)instance;\n')
            fd.write('\n')
            fd.write('    (void)p;\n')
            fd.write('    (void)xctx;\n')
            fd.write('    self->reference_count++;\n')
            fd.write('    return instance;\n')
        fd.write('}\n')
        fd.write('\n')

        if _supports_managed_slice(ctype):
            fd.write('\n')
            fd.write('/* Implementation of method optional_release for managed slice. */\n')
            fd.write('AFW_DECLARE_STATIC(void)\n')
            fd.write('impl_afw_value_managed_slice_optional_release(\n')
            fd.write('    const afw_value_t *instance,\n')
            fd.write('    afw_xctx_t *xctx)\n')
            fd.write('{\n')
            fd.write('    afw_value_' + id + '_managed_slice_t *self =\n')
            fd.write('        (afw_value_' + id + '_managed_slice_t *)instance;\n')
            fd.write('\n')
            fd.write('    if (self->reference_count == 0) {\n')
            fd.write('        return;\n')
            fd.write('    }\n')
            fd.write('    self->reference_count--;\n')
            fd.write('    if (self->reference_count == 0) {\n')
            fd.write('        if (self->containing_value) {\n')
            fd.write('            afw_value_release(&self->containing_value->pub, xctx);\n')
            fd.write('        }\n')
            fd.write('        afw_pool_free_memory(xctx->p, self,\n')
            fd.write('            sizeof(afw_value_' + id +
                     '_managed_slice_t), xctx);\n')
            fd.write('    }\n')
            fd.write('}\n')
            fd.write('\n')
            fd.write('/* Implementation of method get_reference for managed slice. */\n')
            fd.write('AFW_DECLARE_STATIC(const afw_value_t *)\n')
            fd.write('impl_afw_value_managed_slice_get_reference(\n')
            fd.write('    const afw_value_t *instance,\n')
            fd.write('    const afw_pool_t *p,\n')
            fd.write('    afw_xctx_t *xctx)\n')
            fd.write('{\n')
            fd.write('    afw_value_' + id + '_managed_slice_t *self =\n')
            fd.write('        (afw_value_' + id + '_managed_slice_t *)instance;\n')
            fd.write('\n')
            fd.write('    (void)p;\n')
            fd.write('    (void)xctx;\n')
            fd.write('    self->reference_count++;\n')
            fd.write('    return instance;\n')
            fd.write('}\n')
            fd.write('\n')

    fd.write('\n')
    fd.write('/* Implementation of method get_reference for permanent value. */\n')
    fd.write('AFW_DECLARE_STATIC(const afw_value_t *)\n')
    fd.write('impl_afw_value_permanent_get_reference(\n')
    fd.write('    const afw_value_t *instance,\n')
    fd.write('    const afw_pool_t *p,\n')
    fd.write('    afw_xctx_t *xctx)\n')
    fd.write('{\n')
    if id in ('object', 'array'):
        fd.write('    /* Permanent bag: hold is self; isolate is get_assignable_value. */\n')
        fd.write('    (void)p;\n')
        fd.write('    (void)xctx;\n')
        fd.write('    return instance;\n')
    else:
        fd.write('    /* Permanent scalar: same instance as-is. */\n')
        fd.write('    (void)p;\n')
        fd.write('    (void)xctx;\n')
        fd.write('    return instance;\n')
    fd.write('}\n')
    fd.write('\n')

    fd.write('/*\n')
    fd.write(' * Implementation of method get_data_type for interface afw_value.\n')
    fd.write(' */\n')
    fd.write('const afw_data_type_t *\n')
    fd.write('impl_afw_value_get_data_type(\n')
    fd.write('    const afw_value_t *instance,\n')
    fd.write('    afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    return afw_data_type_' + id + ';\n')
    fd.write('}\n')
    fd.write('\n')

    fd.write('/*\n')
    fd.write(' * Implementation of method compiler_listing for interface afw_value.\n')
    fd.write(' */\n')
    fd.write('void\n')
    fd.write('impl_afw_value_produce_compiler_listing(\n')
    fd.write('    const afw_value_t *instance,\n')
    fd.write('    const afw_writer_t *writer,\n')
    fd.write('    afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    afw_data_type_value_compiler_listing(\n')
    fd.write('        afw_data_type_' + id + ',\n')
    fd.write('        writer, instance, xctx);\n')
    fd.write('}\n')
    fd.write('\n')

    fd.write('/*\n')
    fd.write(' * Implementation of method decompile for interface afw_value.\n')
    fd.write(' */\n')
    fd.write('void\n')
    fd.write('impl_afw_value_decompile(\n')
    fd.write('    const afw_value_t *instance,\n')
    fd.write('    const afw_writer_t *writer,\n')
    fd.write('    afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    afw_data_type_write_as_expression(\n')
    fd.write('        afw_data_type_' + id + ',\n')
    fd.write('        writer,\n')
    fd.write('        (const void *)&(((const afw_value_common_t *)instance)->internal),\n')
    fd.write('        xctx);\n')
    fd.write('}\n')
    fd.write('\n')
    
    fd.write('/*\n')
    fd.write(' * Implementation of method get_info for interface afw_value.\n')
    fd.write(' */\n')
    fd.write('void\n')
    fd.write('impl_afw_value_get_info(\n')
    fd.write('    const afw_value_t *instance,\n')
    fd.write('    afw_value_info_t *info,\n')
    fd.write('    const afw_pool_t *p,\n')
    fd.write('    afw_xctx_t *xctx)\n')
    fd.write('{\n')
    fd.write('    afw_memory_clear(info);\n')
    fd.write('    info->value_inf_id = &instance->inf->rti.implementation_id;\n')
    fd.write('    info->evaluated_data_type = afw_data_type_' + id + ';\n')
    fd.write('    info->optimized_value = instance;\n')
    fd.write('}\n')
    fd.write('\n')


    # array
    if not special:
        fd.write('\n/* Get next value from array of ' + id + '. */\n')
        fd.write(define + '(' + return_type + ')\n')
        fd.write(prefix + 'array_of_' + id + '_get_next_source(\n')
        fd.write('    const afw_array_t *instance,\n')
        fd.write('    const afw_iterator_old_t * *iterator,\n')
        if needs_found:
            fd.write('    afw_boolean_t *found,\n')
        fd.write('    const afw_utf8_z_t *source_z,\n')
        fd.write('    afw_xctx_t *xctx)\n')
        fd.write('{\n')
        fd.write('    const void *internal;\n')
        fd.write('    const afw_data_type_t *data_type;\n')
        fd.write('\n')
        fd.write('    afw_array_get_next_internal(instance, iterator, &data_type, &internal, xctx);\n')
        if needs_found:
            fd.write('    *found = true;\n')
            fd.write('    if (!internal) {\n')
            fd.write('        *found = false;\n')
            fd.write('        return 0;\n')
            fd.write('    }\n')
        else:
            fd.write('    if (!internal) {\n')
            fd.write('        return NULL;\n')
            fd.write('    }\n')    
        fd.write('    if (data_type != afw_data_type_' + id + ') {\n')
        fd.write('        const afw_utf8_t *data_type_id;\n')
        fd.write('\n')
        fd.write('        data_type_id = &data_type->data_type_id;\n')
        fd.write('        afw_error_set_fz(afw_error_code_general, source_z, xctx,\n')
        fd.write('            "Typesafe error: expecting \'' + id + '\' but "\n')
        fd.write('            "encountered " AFW_UTF8_FMT_Q,\n')
        fd.write('            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));\n')
        fd.write('        longjmp(((xctx)->current_try->throw_jmp_buf), afw_error_code_general);\n')
        fd.write('    }\n')
        if direct_return:
            fd.write('    return *(' + ctype + ' *)internal;\n')
        else:
            fd.write('    return (' + return_type + ')internal;\n')
        fd.write('}\n')

        fd.write('\n/* Add value from array of ' + id + ' */\n')
        fd.write(define + '(void)\n')
        fd.write(prefix + 'array_of_' + id + '_add(\n')
        fd.write('    const afw_array_t *instance,\n')
        fd.write('    ' + parameter_ctype + 'value,\n')
        fd.write('    afw_xctx_t *xctx)\n')
        fd.write('{\n')
        fd.write('    const afw_array_setter_t *setter;\n')
        if ctype.endswith('*'):
            fd.write('    ' + parameter_ctype + 'internal;\n')
        fd.write('\n')
        fd.write('    setter = afw_array_get_setter(instance, xctx);\n')
        fd.write('    if (!setter) {\n')
        fd.write('        AFW_LIST_ERROR_OBJECT_IMMUTABLE;\n')
        fd.write('    }\n')
        fd.write('\n')
        if ctype.endswith('*'):
            fd.write('    internal = value;\n')
            fd.write('    afw_array_setter_push_internal(setter, \n')
            fd.write('        afw_data_type_' + id + ',\n')
            fd.write('        (const void *)&internal, xctx);\n')
        else:
            fd.write('    afw_array_setter_push_internal(setter, \n')
            fd.write('        afw_data_type_' + id + ',\n')
            fd.write('        (const void *)value, xctx);\n')
        fd.write('}\n')

        fd.write('\n/* Remove value from array of ' + id + ' */\n')
        fd.write(define + '(void)\n')
        fd.write(prefix + 'array_of_' + id + '_remove(\n')
        fd.write('    const afw_array_t *instance,\n')
        fd.write('    ' + parameter_ctype + 'value,\n')
        fd.write('    afw_xctx_t *xctx)\n')
        fd.write('{\n')
        if ctype.endswith('*'):
            fd.write('    ' + parameter_ctype + 'internal;\n')
        fd.write('    const afw_array_setter_t *setter;\n')
        fd.write('\n')
        fd.write('    setter = afw_array_get_setter(instance, xctx);\n')
        fd.write('    if (!setter) {\n')
        fd.write('        AFW_LIST_ERROR_OBJECT_IMMUTABLE;\n')
        fd.write('    }\n')
        fd.write('\n')
        if ctype.endswith('*'):
            fd.write('    internal = value;\n')
            fd.write('    afw_array_setter_remove_internal(setter, \n')
            fd.write('        afw_data_type_' + id + ',\n')
            fd.write('        (const void *)&internal, xctx);\n')
        else:
            fd.write('    afw_array_setter_remove_internal(setter, \n')
            fd.write('        afw_data_type_' + id + ',\n')
            fd.write('        (const void *)value, xctx);\n')
        fd.write('}\n')


def generate_h(prefix, obj, id, generated_by, dir, copyright, filename, options):
    msg.info('Generating ' + filename)
    with nfc.open(dir+filename, mode='w') as fd:
        c.write_h_prologue(fd, generated_by, 'Adaptive Data Type ' + id , copyright, filename)
        fd.write('\n#include "afw_minimal.h"\n')
        fd.write('#include "' + prefix + 'data_type_typedefs.h"\n')
        # AFW_DECLARE* macros come from afw_common.h via afw_minimal.h (core only).
        if options['core']:
            fd.write('\n/**\n')
            fd.write(' * @defgroup afw_c_api_data_type_' + id + ' ' + id + '\n')
            fd.write(' * @ingroup afw_c_api_data_types\n')
            fd.write(' *\n')
            fd.write(' * Adaptive Data Type ' + id + '\n')
            fd.write(' *\n')
            fd.write(' * @{\n')
            fd.write(' */\n')
        c.write_doxygen_file_section(
            fd, filename,
            'Generated header for adaptive data type `' + id + '`.')
        fd.write('\nAFW_BEGIN_DECLARES\n')

        write_h_section(fd, prefix, obj)

        fd.write('\nAFW_END_DECLARES\n')
        if options['core']:
            fd.write('\n/** @} */\n')
        c.write_h_epilogue(fd, filename)


def generate_typedefs_h(prefix, data_type_array, id, generated_by, dir, copyright, filename, options):
    with nfc.open(dir+filename, mode='w') as fd:
        c.write_h_prologue(fd, generated_by, 'Adaptive Data Type Typedefs ' + id, copyright, filename)
        c.write_doxygen_file_section(
            fd, filename,
            'Generated typedefs header for adaptive data type `' + id + '`.')
        fd.write('\n#include "afw_minimal.h"\n')
        fd.write('\nAFW_BEGIN_DECLARES\n')

        for obj in data_type_array:
            id = obj['_meta_']['objectId']
            if options['core']:
                fd.write('\n/**\n')
                fd.write(' * @addtogroup afw_c_api_data_type_' + id + '\n')
                fd.write(' * @{\n')
                fd.write(' */\n')
            filename = prefix + 'data_type_' + id + '_binding.h'
            write_typedefs_h_section(fd, prefix, obj)
            if options['core']:
                fd.write('\n/** @} */\n')

        fd.write('\nAFW_END_DECLARES\n')
        c.write_h_epilogue(fd, filename)


def generate_c(prefix, obj, id, generated_by, dir, copyright, filename):
    msg.info('Generating ' + filename)
    with nfc.open(dir+filename, mode='w') as fd:
        c.write_c_prologue(fd, generated_by, 'Adaptive Framework Core Data Types ', copyright)
        c.write_doxygen_file_section(
            fd, filename,
            'Generated core adaptive data type implementations.')
        fd.write('\n')
        fd.write('#include "afw.h"\n')
        fd.write('#include "afw_runtime_object_maps.h"')
        fd.write('\n')

        write_c_section(fd, prefix, obj)

def generate(generated_by, prefix, data_type_array, generated_dir_path, options):
    declare = prefix.upper() + 'DECLARE'
    define = prefix.upper() + 'DEFINE'

    # Just return if no data types
    if len(data_type_array) == 0: return

    if not options.get('core'):
        msg.error_exit(
            'Data type bindings are only supported in core (libafw). '
            'Non-core packages must not define _AdaptiveDataTypeGenerate_/.')

    afw_package = package.get_afw_package(options)
    copyright = afw_package.get('copyright')

    # Make sure generated/ directory structure exists
    os.makedirs(generated_dir_path, exist_ok=True)

    # Generate <prefix>_data_type_<dataType>.h for each data type
    for obj in data_type_array:
        id = obj['_meta_']['objectId']
        filename = prefix + 'data_type_' + id + '_binding.h'
        generate_h(prefix, obj, id, generated_by, generated_dir_path, copyright, filename, options)

    # Generate <prefix>_data_type_typedefs.h
    filename = prefix + 'data_type_typedefs.h'
    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        generate_typedefs_h(prefix, data_type_array, id, generated_by, generated_dir_path, copyright, filename, options)

    # Generate <prefix>_data_type_bindings.h
    filename = prefix + 'data_type_bindings.h'
    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_h_prologue(fd, generated_by, 'Adaptive Framework Data Type Bindings', copyright, filename)
        c.write_doxygen_file_section(
            fd, filename,
            'Generated core adaptive data type bindings header.')
        fd.write('\n')

        for obj in data_type_array:
            id = obj['_meta_']['objectId']
            fd.write('#include "' + prefix + 'data_type_' + id + '_binding.h"\n')

        fd.write('\n/**\n')
        fd.write(' * @brief Data type map.\n')
        fd.write(' */\n')
        fd.write('#define ' + prefix.upper() + 'DATA_TYPES_MAP(XX) \\\n')
        for obj in data_type_array:
            id = obj['_meta_']['objectId']
            fd.write('    XX(' + id + ') \\\n')
        fd.write('\n')

        if options['core']: 
            fd.write('\n/**\n')
            fd.write(' * @brief Core data types map.\n')
            fd.write(' */\n')
            fd.write('typedef enum afw_data_type_number_e {\n')
            fd.write('    afw_data_type_number_special,\n')
            fd.write('#define XX(id) afw_data_type_number_ ## id,\n')
            fd.write('    AFW_DATA_TYPES_MAP(XX)\n')
            fd.write('    #undef XX\n')
            fd.write('    afw_data_type_number_max\n')
            fd.write('} afw_data_type_number_t;\n')

       
        fd.write('\n/**\n')
        fd.write(' * @brief Register each ' + prefix + 'data_type_<dataType>.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' */\n')
        fd.write(declare + '(void)\n')
        fd.write(prefix + 'register_data_types(\n')
        fd.write('    afw_xctx_t *xctx);\n')

       
        fd.write('\n/**\n')
        fd.write(' * @brief Register each ' + prefix + '_value_<dataType>_inf.\n')
        fd.write(' * @param xctx of caller.\n')
        fd.write(' */\n')
        fd.write(declare + '(void)\n')
        fd.write(prefix + 'register_data_type_infs(\n')
        fd.write('    afw_xctx_t *xctx);\n')

        c.write_h_epilogue(fd, filename)

    # Generate <prefix>data_type_<id>_binding.c
    for obj in data_type_array:
        id = obj['_meta_']['objectId']
        filename = prefix + 'data_type_' + id + '_binding.c'
        generate_c(prefix, obj, id, generated_by, generated_dir_path, copyright, filename)

    # Generate <prefix>data_types_bindings.c
    filename = prefix + 'data_type_bindings.c'
    msg.info('Generating ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        c.write_c_prologue(fd, generated_by, 'Adaptive Framework Data Type Bindings', copyright)
        c.write_doxygen_file_section(
            fd, filename,
            'Generated adaptive data type bindings implementation.')
        fd.write('\n')
        fd.write('#include "afw.h"\n')
        fd.write('#include "' + prefix + 'data_type_bindings.h"\n')
        fd.write('\n')
        fd.write('static const afw_data_type_t * ' + 'impl_data_type_bindings[] = {\n')
        fd.write('#define XX(id) &' + prefix + 'data_type_ ##id##_direct,\n')
        fd.write(prefix.upper() + 'DATA_TYPES_MAP(XX)\n')
        fd.write('    NULL\n')
        fd.write('};\n')
        fd.write('\n')
        fd.write(define + '(void)\n')
        fd.write(prefix + 'register_data_types(\n')
        fd.write('    afw_xctx_t *xctx)\n')
        fd.write('{\n')
        fd.write('    /* Register data types */\n')
        fd.write('    afw_environment_register_data_types(\n')
        fd.write('        &impl_data_type_bindings[0], xctx);\n')
        fd.write('\n')
        fd.write('}\n')

        fd.write('\n')
        fd.write(define + '(void)\n')
        fd.write(prefix + 'register_data_type_infs(\n')
        fd.write('    afw_xctx_t *xctx)\n')
        fd.write('{\n')

        for obj in data_type_array:
            special = obj.get('special',False)
            id = obj['_meta_']['objectId']
            fd.write('\n')
            fd.write('    /* Register value inf id evaluated_' + id + ' */\n')
            fd.write('    afw_environment_register_value_inf(\n')
            if special:
                fd.write('        &afw_value_permanent_' + id + '_inf.rti.implementation_id,\n')
                fd.write('        NULL,\n')
            else:
                fd.write('        &afw_value_unmanaged_' + id + '_inf.rti.implementation_id,\n')
                fd.write('        &afw_value_unmanaged_' + id + '_inf,\n')
            fd.write('        xctx);\n')
    
        fd.write('\n')
        fd.write('}\n')
