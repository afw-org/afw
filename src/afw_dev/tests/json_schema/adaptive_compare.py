#!/usr/bin/env python3
"""Compare adaptive object type JSON to generated JSON Schema projection."""

import os
import sys

_HERE = os.path.dirname(os.path.abspath(__file__))
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)

from _common import (
    entity_def,
    is_clean_ref_form,
    load_ot,
    load_schema,
    make_test,
    ot_local_property_names,
    ot_local_required,
    package_root,
    property_map,
    property_types_def,
    skip_if_no_schemas,
)

# Golden object types for property-level comparison.
GOLDEN_OTS = [
    '_AdaptiveObject_',
    '_AdaptiveObjectType_',
    '_AdaptiveFunction_',
    '_AdaptiveFunctionGenerate_',
    '_AdaptivePolymorphicFunction_',
    '_AdaptiveResponse_',
    '_AdaptiveApplication_',
]


def _check_property_names(ot_id, ot_json, schema_doc, tests):
    expected = set(ot_local_property_names(ot_json))
    actual = set(property_map(schema_doc, ot_id).keys())
    missing = sorted(expected - actual)
    extra = sorted(actual - expected)
    ok = not missing and not extra
    detail = None
    if missing:
        detail = 'missing in schema: ' + ','.join(missing[:10])
    elif extra:
        detail = 'extra in schema: ' + ','.join(extra[:10])
    tests.append(make_test(
        'props-' + ot_id,
        ot_id + ' property names match adaptive propertyTypes (minus _meta_)',
        ok,
        detail=detail,
    ))


def _check_object_refs(ot_id, ot_json, schema_doc, tests):
    props = property_map(schema_doc, ot_id)
    pts = ot_json.get('propertyTypes') or {}
    for name, prop in pts.items():
        if name == '_meta_' or not isinstance(prop, dict):
            continue
        if prop.get('dataType') != 'object' or not prop.get('dataTypeParameter'):
            continue
        node = props.get(name)
        ok = node is not None and is_clean_ref_form(node)
        # referenced type should appear in $defs
        target = prop['dataTypeParameter']
        has_def = target in (schema_doc.get('$defs') or {})
        tests.append(make_test(
            'object-ref-' + ot_id + '-' + name,
            ot_id + '.' + name + ' projects to clean $ref of ' + target,
            ok and has_def,
            detail=None if (ok and has_def) else repr(node)[:160],
        ))


def _check_array_object_items(ot_id, ot_json, schema_doc, tests):
    props = property_map(schema_doc, ot_id)
    pts = ot_json.get('propertyTypes') or {}
    for name, prop in pts.items():
        if name == '_meta_' or not isinstance(prop, dict):
            continue
        if prop.get('dataType') != 'array':
            continue
        param = prop.get('dataTypeParameter') or ''
        # object-typed elements: "object _AdaptiveX_" or bare _Adaptive*
        is_object_items = (
            param.startswith('object ') or
            (param.startswith('_Adaptive') and ' ' not in param.strip()))
        if not is_object_items:
            continue
        node = props.get(name) or {}
        items = node.get('items')
        ok = (
            node.get('type') == 'array' and
            items is not None and
            is_clean_ref_form(items))
        tests.append(make_test(
            'array-items-ref-' + ot_id + '-' + name,
            ot_id + '.' + name + ' is array with clean object $ref items',
            ok,
            detail=None if ok else repr(node)[:160],
        ))


def _check_defaults_and_enums(ot_id, ot_json, schema_doc, tests):
    props = property_map(schema_doc, ot_id)
    pts = ot_json.get('propertyTypes') or {}
    for name, prop in pts.items():
        if name == '_meta_' or not isinstance(prop, dict):
            continue
        node = props.get(name) or {}
        if 'defaultValue' in prop:
            ok = node.get('default') == prop['defaultValue']
            tests.append(make_test(
                'default-' + ot_id + '-' + name,
                ot_id + '.' + name + ' defaultValue → schema default',
                ok,
                detail=None if ok else repr(node.get('default')),
            ))
        if 'allowedValues' in prop or 'possibleValues' in prop:
            expected = prop.get('allowedValues', prop.get('possibleValues'))
            ok = node.get('enum') == expected
            tests.append(make_test(
                'enum-' + ot_id + '-' + name,
                ot_id + '.' + name + ' allowed/possibleValues → enum',
                ok,
                detail=None if ok else repr(node.get('enum')),
            ))


def _check_open_closed(ot_id, ot_json, schema_doc, tests):
    entity = entity_def(schema_doc, ot_id)
    if not entity:
        tests.append(make_test(
            'open-closed-' + ot_id,
            ot_id + ' open/closed matches otherProperties',
            False,
            detail='entity def missing',
        ))
        return
    if 'otherProperties' in ot_json:
        other = ot_json['otherProperties']
        if other is None or other == {}:
            ok = entity.get('additionalProperties') is True
            tests.append(make_test(
                'open-' + ot_id,
                ot_id + ' otherProperties {} → additionalProperties true',
                ok,
            ))
        else:
            ok = 'additionalProperties' in entity
            tests.append(make_test(
                'additional-' + ot_id,
                ot_id + ' typed otherProperties → additionalProperties present',
                ok,
            ))
    else:
        ok = (
            entity.get('additionalProperties') is False or
            entity.get('unevaluatedProperties') is False)
        tests.append(make_test(
            'closed-' + ot_id,
            ot_id + ' no otherProperties → closed additional/unevaluatedProperties',
            ok,
        ))


def _check_required(ot_id, ot_json, schema_doc, tests):
    entity = entity_def(schema_doc, ot_id)
    ptd = property_types_def(schema_doc, ot_id)
    expected = ot_local_required(ot_json)
    actual = sorted(entity.get('required') or []) if entity else []
    ok = actual == expected
    tests.append(make_test(
        'required-leaf-' + ot_id,
        ot_id + ' entity required matches local adaptive required only',
        ok,
        detail=None if ok else 'expected ' + str(expected) + ' got ' + str(actual),
    ))
    # shared propertyTypes map must not carry required
    if ptd is not None:
        tests.append(make_test(
            'required-not-on-propertyTypes-' + ot_id,
            ot_id + '.propertyTypes def has no required array',
            'required' not in ptd,
        ))


def run():
    response = {
        'description':
            'Adaptive object type JSON compared to generated JSON Schema',
        'tests': [],
    }
    tests = response['tests']
    if skip_if_no_schemas(tests):
        return response

    root = package_root()
    for ot_id in GOLDEN_OTS:
        try:
            ot_json = load_ot(ot_id, root)
            schema_doc = load_schema(ot_id, root)
        except FileNotFoundError as exc:
            tests.append(make_test(
                'load-' + ot_id,
                'load adaptive OT and schema for ' + ot_id,
                False,
                detail=str(exc),
            ))
            continue

        tests.append(make_test(
            'load-' + ot_id,
            'load adaptive OT and schema for ' + ot_id,
            True,
        ))
        _check_property_names(ot_id, ot_json, schema_doc, tests)
        _check_object_refs(ot_id, ot_json, schema_doc, tests)
        _check_array_object_items(ot_id, ot_json, schema_doc, tests)
        _check_defaults_and_enums(ot_id, ot_json, schema_doc, tests)
        _check_open_closed(ot_id, ot_json, schema_doc, tests)
        _check_required(ot_id, ot_json, schema_doc, tests)

    return response
