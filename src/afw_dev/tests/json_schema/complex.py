#!/usr/bin/env python3
"""Complex adaptive cases: inheritance chains and nested object graphs."""

import os
import sys

_HERE = os.path.dirname(os.path.abspath(__file__))
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)

from _common import (
    allof_property_types_refs,
    ancestor_ot_ids,
    entity_def,
    entity_property_names,
    is_clean_ref_form,
    load_json,
    load_ot,
    load_schema,
    make_test,
    ot_local_property_names,
    package_root,
    property_map,
    skip_if_no_schemas,
    validate_instance,
)

# (object type id, description)
INHERITANCE_CASES = [
    (
        '_AdaptivePolymorphicFunction_',
        'PolymorphicFunction inherits Function propertyTypes',
    ),
    (
        '_AdaptiveApplication_',
        'Application inherits Conf_application and Conf_ propertyTypes',
    ),
    (
        '_AdaptiveDataTypeGenerate_',
        'DataTypeGenerate inherits DataType propertyTypes',
    ),
    (
        '_AdaptiveRequestHandler_',
        'RequestHandler inherits Conf_requestHandler (and ancestors)',
    ),
]

NESTED_OBJECT_PROPS = [
    ('_AdaptiveFunction_', 'returns', '_AdaptiveFunctionParameter_'),
    ('_AdaptiveFunction_', 'parameters', None),  # array; check items
    ('_AdaptiveObjectType_', 'propertyTypes', '_AdaptivePropertyTypes_'),
    ('_AdaptiveObjectType_', 'otherProperties', '_AdaptiveValueMeta_'),
    ('_AdaptiveConf_application', 'authorizationControl',
     '_AdaptiveAuthorizationControl_'),
]

COMPLEX_INSTANCES = [
    (
        'src/afw/generate/objects/_AdaptivePolymorphicFunction_/abs.json',
        '_AdaptivePolymorphicFunction_',
        'complex-instance-poly-abs',
    ),
    (
        'src/afw/generate/objects/_AdaptivePolymorphicFunction_/eq.json',
        '_AdaptivePolymorphicFunction_',
        'complex-instance-poly-eq',
    ),
    (
        'src/afw/generate/objects/_AdaptiveFunctionGenerate_/and.json',
        '_AdaptiveFunctionGenerate_',
        'complex-instance-function-generate-and',
    ),
    (
        'src/afw/generate/objects/_AdaptiveObjectType_/_AdaptiveConf_application.json',
        '_AdaptiveObjectType_',
        'complex-instance-ot-conf-application',
    ),
]


def run():
    response = {
        'description':
            'Complex adaptive object types: inheritance and nested graphs',
        'tests': [],
    }
    tests = response['tests']
    if skip_if_no_schemas(tests):
        return response

    root = package_root()

    # --- inheritance: parentPaths recorded + composite property merge ---
    for ot_id, description in INHERITANCE_CASES:
        try:
            schema_doc = load_schema(ot_id, root)
            entity = entity_def(schema_doc, ot_id)
            expected_ancestors = set(ancestor_ot_ids(ot_id, root))
            actual_inherited = allof_property_types_refs(entity)
            prop_names = entity_property_names(entity)
        except FileNotFoundError as exc:
            tests.append(make_test(
                'inherit-' + ot_id,
                description,
                False,
                detail=str(exc),
            ))
            continue

        missing_inherited = sorted(expected_ancestors - actual_inherited)
        ok_chain = not missing_inherited
        tests.append(make_test(
            'inherit-' + ot_id,
            description + ' (x-afw-inheritedObjectTypes)',
            ok_chain,
            detail=None if ok_chain else (
                'missing inherited OTs: ' + ','.join(missing_inherited) +
                '; have ' + ','.join(sorted(actual_inherited))),
        ))

        # Composite entity properties include local + ancestor property names
        # (child overrides share names but still appear once).
        expected_props = set()
        try:
            for aid in list(expected_ancestors) + [ot_id]:
                expected_props.update(ot_local_property_names(load_ot(aid, root)))
        except FileNotFoundError:
            expected_props = set()
        # _meta_ is added as optional instance bag on closed composites
        expected_props.add('_meta_')
        missing_props = sorted(expected_props - prop_names)
        tests.append(make_test(
            'inherit-merged-props-' + ot_id,
            ot_id + ' entity properties include inherited + local names',
            not missing_props,
            detail=None if not missing_props else (
                'missing props: ' + ','.join(missing_props[:15])),
        ))

        # Polymorphic must not require Function's functionResourceId
        if ot_id == '_AdaptivePolymorphicFunction_':
            req = entity.get('required') or []
            tests.append(make_test(
                'inherit-poly-no-functionResourceId-required',
                'PolymorphicFunction does not require functionResourceId',
                'functionResourceId' not in req,
            ))
    # --- nested object / array-of-object projections ---
    for ot_id, prop_name, target_ot in NESTED_OBJECT_PROPS:
        try:
            # Conf_application may only live inside Application schema file
            try:
                schema_doc = load_schema(ot_id, root)
            except FileNotFoundError:
                if ot_id == '_AdaptiveConf_application':
                    schema_doc = load_schema('_AdaptiveApplication_', root)
                else:
                    raise
            props = property_map(schema_doc, ot_id)
            if prop_name not in props:
                ptd = schema_doc.get('$defs', {}).get(
                    ot_id + '.propertyTypes', {})
                props = ptd.get('properties') or {}
            node = props.get(prop_name)
        except FileNotFoundError as exc:
            tests.append(make_test(
                'nested-' + ot_id + '-' + prop_name,
                ot_id + '.' + prop_name + ' nested projection',
                False,
                detail=str(exc),
            ))
            continue

        if target_ot is None:
            # parameters: array with object items
            ok = (
                isinstance(node, dict) and
                node.get('type') == 'array' and
                is_clean_ref_form(node.get('items') or {}))
            items = (node or {}).get('items') or {}
            has_param_def = '_AdaptiveFunctionParameter_' in (
                schema_doc.get('$defs') or {})
            tests.append(make_test(
                'nested-' + ot_id + '-' + prop_name,
                ot_id + '.' + prop_name +
                ' is array of FunctionParameter refs',
                ok and has_param_def,
                detail=None if (ok and has_param_def) else repr(node)[:160],
            ))
        else:
            ok = node is not None and is_clean_ref_form(node)
            has_def = target_ot in (schema_doc.get('$defs') or {})
            tests.append(make_test(
                'nested-' + ot_id + '-' + prop_name,
                ot_id + '.' + prop_name + ' refs ' + target_ot,
                ok and has_def,
                detail=None if (ok and has_def) else repr(node)[:160],
            ))

    # errorsThrown items on Function
    try:
        doc = load_schema('_AdaptiveFunction_', root)
        thrown = property_map(doc, '_AdaptiveFunction_').get('errorsThrown') or {}
        items = thrown.get('items')
        ok = (
            thrown.get('type') == 'array' and
            is_clean_ref_form(items or {}) and
            '_AdaptiveFunctionErrorThrown_' in (doc.get('$defs') or {}))
        tests.append(make_test(
            'nested-Function-errorsThrown-items',
            'Function.errorsThrown items ref FunctionErrorThrown',
            ok,
            detail=None if ok else repr(thrown)[:160],
        ))
    except FileNotFoundError as exc:
        tests.append(make_test(
            'nested-Function-errorsThrown-items',
            'Function.errorsThrown items ref FunctionErrorThrown',
            False,
            detail=str(exc),
        ))

    # --- complex instances ---
    for rel_path, schema_id, test_id in COMPLEX_INSTANCES:
        try:
            instance = load_json(rel_path, root)
            schema_doc = load_schema(schema_id, root)
        except FileNotFoundError as exc:
            tests.append(make_test(
                test_id,
                'complex instance ' + rel_path,
                False,
                detail=str(exc),
            ))
            continue
        ok, err = validate_instance(instance, schema_doc)
        tests.append(make_test(
            test_id,
            'complex instance validates: ' + rel_path,
            ok,
            detail=err[:300] if err else None,
        ))

    # Poly abs uses inherited Function fields (category, functionId, returns)
    try:
        abs_inst = load_json(
            'src/afw/generate/objects/_AdaptivePolymorphicFunction_/abs.json',
            root)
        poly_ot = load_ot('_AdaptivePolymorphicFunction_', root)
        local = set(
            k for k in (poly_ot.get('propertyTypes') or {})
            if k != '_meta_')
        # category/functionId/returns are on Function, not local poly props
        inherited_used = [
            k for k in ('category', 'functionId', 'returns', 'parameters')
            if k in abs_inst and k not in local]
        tests.append(make_test(
            'complex-poly-uses-inherited-props',
            'poly abs instance uses inherited Function properties',
            len(inherited_used) >= 3,
            detail='inherited present: ' + ','.join(inherited_used),
        ))
    except FileNotFoundError as exc:
        tests.append(make_test(
            'complex-poly-uses-inherited-props',
            'poly abs instance uses inherited Function properties',
            False,
            detail=str(exc),
        ))

    return response
