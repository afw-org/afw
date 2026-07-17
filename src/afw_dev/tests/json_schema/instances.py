#!/usr/bin/env python3
"""Validate real adaptive object instances against generated schemas."""

import copy
import os
import sys

_HERE = os.path.dirname(os.path.abspath(__file__))
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)

from _common import (
    load_json,
    load_schema,
    make_test,
    package_root,
    skip_if_no_schemas,
    validate_instance,
)

# (relative path under package root, schema entity id, test id suffix)
POSITIVE_INSTANCES = [
    (
        'src/afw/generate/objects/_AdaptiveFunctionGenerate_/and.json',
        '_AdaptiveFunctionGenerate_',
        'function-generate-and',
    ),
    (
        'src/afw/generate/objects/_AdaptiveFunctionGenerate_/if.json',
        '_AdaptiveFunctionGenerate_',
        'function-generate-if',
    ),
    (
        'src/afw/generate/objects/_AdaptivePolymorphicFunction_/abs.json',
        '_AdaptivePolymorphicFunction_',
        'poly-abs',
    ),
    (
        'src/afw/generate/objects/_AdaptivePolymorphicFunction_/eq.json',
        '_AdaptivePolymorphicFunction_',
        'poly-eq',
    ),
    (
        'src/afw/generate/objects/_AdaptiveObjectType_/_AdaptiveFunction_.json',
        '_AdaptiveObjectType_',
        'ot-function',
    ),
    (
        'src/afw/generate/objects/_AdaptiveObjectType_/_AdaptiveObject_.json',
        '_AdaptiveObjectType_',
        'ot-object',
    ),
    (
        'src/afw/generate/objects/_AdaptiveDataTypeGenerate_/object.json',
        '_AdaptiveDataTypeGenerate_',
        'datatype-object',
    ),
    (
        'afw-package.json',
        '_AdaptivePackage_',
        'afw-package',
    ),
]

NEGATIVE_FIXTURES = [
    (
        'data/invalid_function_missing_id.json',
        '_AdaptiveFunctionGenerate_',
        'neg-missing-functionId',
        'FunctionGenerate missing functionId is rejected',
    ),
    (
        'data/invalid_function_id_type.json',
        '_AdaptiveFunctionGenerate_',
        'neg-functionId-type',
        'FunctionGenerate with non-string functionId is rejected',
    ),
    (
        'data/invalid_poly_extra_property.json',
        '_AdaptivePolymorphicFunction_',
        'neg-poly-extra-property',
        'Polymorphic function with unknown property is rejected',
    ),
]


def run():
    response = {
        'description':
            'Adaptive object instances validated against generated JSON Schema',
        'tests': [],
    }
    tests = response['tests']
    if skip_if_no_schemas(tests):
        return response

    root = package_root()
    data_dir = os.path.join(_HERE)

    for rel_path, schema_id, suffix in POSITIVE_INSTANCES:
        try:
            instance = load_json(rel_path, root)
            schema_doc = load_schema(schema_id, root)
        except FileNotFoundError as exc:
            tests.append(make_test(
                'instance-' + suffix,
                'validate ' + rel_path + ' as ' + schema_id,
                False,
                detail=str(exc),
            ))
            continue
        ok, err = validate_instance(instance, schema_doc)
        tests.append(make_test(
            'instance-' + suffix,
            'validate ' + rel_path + ' as ' + schema_id,
            ok,
            detail=err[:300] if err else None,
        ))

    for rel_path, schema_id, suffix, description in NEGATIVE_FIXTURES:
        path = os.path.join(data_dir, rel_path)
        try:
            instance = load_json(path, root)
            schema_doc = load_schema(schema_id, root)
        except FileNotFoundError as exc:
            tests.append(make_test(
                suffix,
                description,
                False,
                detail=str(exc),
            ))
            continue
        ok, err = validate_instance(instance, schema_doc)
        # negative: validation must fail
        tests.append(make_test(
            suffix,
            description,
            not ok,
            detail=None if not ok else 'unexpectedly valid',
        ))

    # Inline negative: strip functionId from a known-good FunctionGenerate
    try:
        good = load_json(
            'src/afw/generate/objects/_AdaptiveFunctionGenerate_/and.json',
            root)
        schema_doc = load_schema('_AdaptiveFunctionGenerate_', root)
        broken = copy.deepcopy(good)
        broken.pop('functionId', None)
        ok, _ = validate_instance(broken, schema_doc)
        tests.append(make_test(
            'neg-strip-functionId-from-and',
            'and.json without functionId is rejected',
            not ok,
            detail=None if not ok else 'unexpectedly valid',
        ))
    except FileNotFoundError as exc:
        tests.append(make_test(
            'neg-strip-functionId-from-and',
            'and.json without functionId is rejected',
            False,
            detail=str(exc),
        ))

    return response
