#!/usr/bin/env python3
"""Structural contracts on package-root generated/schemas/afw."""

import json
import os
import sys

_HERE = os.path.dirname(os.path.abspath(__file__))
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)

from _common import (
    find_mixed_refs,
    is_clean_ref_form,
    iter_schema_files,
    load_schema,
    make_test,
    package_root,
    property_map,
    schemas_dir,
    skip_if_no_schemas,
)


def run():
    response = {
        'description': 'JSON Schema structural contracts (generated/schemas/afw)',
        'tests': [],
    }
    tests = response['tests']
    if skip_if_no_schemas(tests):
        return response

    root = package_root()
    files = list(iter_schema_files(root))
    tests.append(make_test(
        'schemas-dir-exists',
        'generated/schemas/afw exists and contains schema files',
        len(files) > 0,
        detail=str(schemas_dir(root)),
    ))

    mixed = []
    for path in files:
        with open(path, encoding='utf-8') as fd:
            doc = json.load(fd)
        for issue_path, others in find_mixed_refs(doc):
            mixed.append(
                path.name + ' ' + issue_path + ' extras=' + str(others))
    tests.append(make_test(
        'no-mixed-ref',
        'no schema node combines $ref with sibling keywords (issue #3)',
        len(mixed) == 0,
        detail='; '.join(mixed[:5]) if mixed else None,
    ))

    for ot_id in (
            '_AdaptiveObject_',
            '_AdaptiveFunction_',
            '_AdaptivePolymorphicFunction_'):
        try:
            doc = load_schema(ot_id, root)
        except FileNotFoundError as exc:
            tests.append(make_test(
                'entity-document-shape-' + ot_id,
                'schema document shape for ' + ot_id,
                False,
                detail=str(exc),
            ))
            continue
        ok = (
            doc.get('$schema') is not None and
            isinstance(doc.get('$defs'), dict) and
            ot_id in doc['$defs'] and
            isinstance(doc.get('allOf'), list) and
            len(doc['allOf']) >= 1 and
            doc['allOf'][0].get('$ref') == '#/$defs/' + ot_id
        )
        tests.append(make_test(
            'entity-document-shape-' + ot_id,
            'schema document for ' + ot_id +
            ' has $schema, $defs, allOf->$ref entity',
            ok,
        ))

    ref_checks = [
        ('_AdaptiveFunction_', 'returns'),
        ('_AdaptiveObjectType_', 'otherProperties'),
        ('_AdaptiveObjectType_', 'propertyTypes'),
        ('_AdaptiveObjectType_', 'runtime'),
        ('_AdaptiveConf_application', 'authorizationControl'),
    ]
    for ot_id, prop_name in ref_checks:
        doc = None
        try:
            doc = load_schema(ot_id, root)
        except FileNotFoundError:
            if ot_id == '_AdaptiveConf_application':
                try:
                    doc = load_schema('_AdaptiveApplication_', root)
                except FileNotFoundError as exc:
                    tests.append(make_test(
                        'ref-form-' + ot_id + '-' + prop_name,
                        'object property ' + ot_id + '.' + prop_name +
                        ' uses clean $ref form',
                        False,
                        detail=str(exc),
                    ))
                    continue
            else:
                tests.append(make_test(
                    'ref-form-' + ot_id + '-' + prop_name,
                    'object property ' + ot_id + '.' + prop_name +
                    ' uses clean $ref form',
                    False,
                    detail='schema missing',
                ))
                continue

        props = property_map(doc, ot_id)
        if prop_name not in props:
            ptd = doc.get('$defs', {}).get(ot_id + '.propertyTypes', {})
            props = ptd.get('properties') or {}
        node = props.get(prop_name)
        ok = node is not None and is_clean_ref_form(node)
        tests.append(make_test(
            'ref-form-' + ot_id + '-' + prop_name,
            'object property ' + ot_id + '.' + prop_name +
            ' uses pure $ref or allOf+annotations',
            ok,
            detail=None if ok else repr(node)[:200],
        ))

    try:
        doc = load_schema('_AdaptiveFunction_', root)
        params = property_map(doc, '_AdaptiveFunction_').get('parameters') or {}
        items = params.get('items')
        ok = items is not None and is_clean_ref_form(items)
        tests.append(make_test(
            'ref-form-Function-parameters-items',
            'Function.parameters.items uses clean $ref form',
            ok,
            detail=None if ok else repr(items)[:200],
        ))
    except FileNotFoundError as exc:
        tests.append(make_test(
            'ref-form-Function-parameters-items',
            'Function.parameters.items uses clean $ref form',
            False,
            detail=str(exc),
        ))

    param_schema = schemas_dir(root) / '_AdaptiveFunctionParameter_.json'
    tests.append(make_test(
        'allowEntity-false-no-top-level-file',
        '_AdaptiveFunctionParameter_ (allowEntity false) has no top-level schema file',
        not param_schema.is_file(),
    ))

    tests.append(make_test(
        'allowEntity-entity-file-exists',
        '_AdaptiveFunction_ entity schema file exists',
        (schemas_dir(root) / '_AdaptiveFunction_.json').is_file(),
    ))

    try:
        doc = load_schema('_AdaptiveFunction_', root)
        props = property_map(doc, '_AdaptiveFunction_')
        tests.append(make_test(
            'meta-not-instance-property',
            '_meta_ is not listed as a Function instance property',
            '_meta_' not in props,
        ))
    except FileNotFoundError as exc:
        tests.append(make_test(
            'meta-not-instance-property',
            '_meta_ is not listed as a Function instance property',
            False,
            detail=str(exc),
        ))

    return response
