#!/usr/bin/env python3
"""
Gnarly inheritance cases centered on _AdaptiveModel_ and related types.

Real model instances use nested objectTypes maps whose values inherit
ModelObjectType → ObjectType, and propertyTypes maps whose values inherit
ModelPropertyType → ValueMeta. Child property definitions must override
parents (e.g. model propertyTypes is ModelPropertyTypes, not PropertyTypes).
"""

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
    package_root,
    property_map,
    skip_if_no_schemas,
    validate_instance,
)

# Real model instances scattered in the tree (prefer source over build/).
# Each entry: path, test id suffix, description, optional unwrap key for envelopes.
MODEL_INSTANCES = [
    (
        'src/afw/tests/environments/models/objects/_AdaptiveModel_/TestModel1.json',
        'test-model-1-environments',
        'TestModel1 from models test environment',
        None,
    ),
    (
        'src/afw/tests/authorization/application/models/objects/_AdaptiveModel_/TestModel1.json',
        'test-model-1-authorization',
        'TestModel1 from authorization application models',
        None,
    ),
    (
        'docker/images/afw/afw/models/_AdaptiveModel_/TIER Core Schema.json',
        'tier-core-schema',
        'TIER Core Schema docker model (large nested objectTypes)',
        None,
    ),
    (
        # URL-encoded filename; body is a get_object mock envelope {status, result}.
        'src/afw_test/javascript/src/__mocks__/get_object/models/_AdaptiveModel_/TIER%20Core%20Schema.json',
        'tier-core-schema-js-mock',
        'TIER Core Schema from JS get_object mock (unwrap result)',
        'result',
    ),
]


def run():
    response = {
        'description':
            'Gnarly _AdaptiveModel_ inheritance and nested model object graphs',
        'tests': [],
    }
    tests = response['tests']
    if skip_if_no_schemas(tests):
        return response

    root = package_root()

    # --- Model schema graph structure ---
    try:
        model_schema = load_schema('_AdaptiveModel_', root)
        model_entity = entity_def(model_schema, '_AdaptiveModel_')
        model_props = property_map(model_schema, '_AdaptiveModel_')
        defs = model_schema.get('$defs') or {}
    except FileNotFoundError as exc:
        tests.append(make_test(
            'model-schema-load',
            'load _AdaptiveModel_ schema',
            False,
            detail=str(exc),
        ))
        return response

    tests.append(make_test(
        'model-schema-load',
        'load _AdaptiveModel_ schema',
        True,
    ))

    # Nested $defs required for a useful model projection
    for need in (
            '_AdaptiveModelObjectTypes_',
            '_AdaptiveModelObjectType_',
            '_AdaptiveModelPropertyTypes_',
            '_AdaptiveModelPropertyType_',
            '_AdaptiveObjectType_.propertyTypes',
            '_AdaptiveValueMeta_.propertyTypes',
            '_AdaptiveValueMeta_',
    ):
        tests.append(make_test(
            'model-def-' + need.replace('.', '_'),
            'Model schema $defs includes ' + need,
            need in defs,
        ))

    # objectTypes / propertyTypes on Model are clean object refs
    for prop_name, target in (
            ('objectTypes', '_AdaptiveModelObjectTypes_'),
            ('propertyTypes', '_AdaptiveModelPropertyTypes_'),
            ('custom', '_AdaptiveTemplateProperties_'),
    ):
        node = model_props.get(prop_name)
        ok = node is not None and is_clean_ref_form(node) and target in defs
        tests.append(make_test(
            'model-prop-ref-' + prop_name,
            'Model.' + prop_name + ' refs ' + target,
            ok,
            detail=None if ok else repr(node)[:160],
        ))

    # ModelObjectType inherits ObjectType (override-capable composite)
    try:
        mot = entity_def(model_schema, '_AdaptiveModelObjectType_')
        inherited = allof_property_types_refs(mot) if mot else set()
        # if MOT only appears as nested def, x-afw-inheritedObjectTypes should list ObjectType
        has_object_type = (
            '_AdaptiveObjectType_' in inherited or
            (mot and 'allowAdd' in (mot.get('properties') or {})) or
            (mot and 'mappedObjectType' in (mot.get('properties') or {})))
        # prefer: inherited list or merged props contain both sides
        props = (mot or {}).get('properties') or {}
        has_mapped = 'mappedObjectType' in props
        has_allow = 'allowAdd' in props  # from ObjectType
        has_model_hooks = 'onGetObject' in props or 'onRetrieveObjects' in props
        tests.append(make_test(
            'model-object-type-composite',
            'ModelObjectType composite has model hooks + ObjectType props '
            '(override-safe merge)',
            has_mapped and has_allow and has_model_hooks,
            detail='props sample: ' + ','.join(sorted(props)[:20]),
        ))
        tests.append(make_test(
            'model-object-type-inherits-ObjectType',
            'ModelObjectType records or merges ObjectType inheritance',
            '_AdaptiveObjectType_' in inherited or has_allow,
            detail='inherited=' + ','.join(sorted(inherited)),
        ))
    except Exception as exc:
        tests.append(make_test(
            'model-object-type-composite',
            'ModelObjectType composite has model hooks + ObjectType props',
            False,
            detail=str(exc),
        ))

    # ModelPropertyType inherits ValueMeta + mappedPropertyName
    try:
        mpt = entity_def(model_schema, '_AdaptiveModelPropertyType_')
        props = (mpt or {}).get('properties') or {}
        tests.append(make_test(
            'model-property-type-composite',
            'ModelPropertyType has mappedPropertyName and ValueMeta dataType',
            'mappedPropertyName' in props and 'dataType' in props,
            detail='props: ' + ','.join(sorted(props)[:25]),
        ))
        inherited = allof_property_types_refs(mpt) if mpt else set()
        tests.append(make_test(
            'model-property-type-inherits-ValueMeta',
            'ModelPropertyType inherits _AdaptiveValueMeta_',
            '_AdaptiveValueMeta_' in inherited or 'dataType' in props,
            detail='inherited=' + ','.join(sorted(inherited)),
        ))
    except Exception as exc:
        tests.append(make_test(
            'model-property-type-composite',
            'ModelPropertyType composite',
            False,
            detail=str(exc),
        ))

    # ModelObjectTypes / ModelPropertyTypes are open maps of nested types
    for map_id, value_id in (
            ('_AdaptiveModelObjectTypes_', '_AdaptiveModelObjectType_'),
            ('_AdaptiveModelPropertyTypes_', '_AdaptiveModelPropertyType_'),
    ):
        node = defs.get(map_id) or {}
        add = node.get('additionalProperties')
        ok = False
        if isinstance(add, dict):
            # allOf [ $ref value ] or pure $ref
            if is_clean_ref_form(add):
                ref = add.get('$ref') or (
                    add.get('allOf') or [{}])[0].get('$ref', '')
                ok = value_id in ref
            elif '$ref' in str(add):
                ok = value_id in str(add)
        tests.append(make_test(
            'model-map-' + map_id,
            map_id + ' additionalProperties refs ' + value_id,
            ok,
            detail=None if ok else repr(add)[:200],
        ))

    # propertyTypes on ModelObjectType must be ModelPropertyTypes, not
    # the parent ObjectType PropertyTypes (override check)
    mot = entity_def(model_schema, '_AdaptiveModelObjectType_') or {}
    pt_node = (mot.get('properties') or {}).get('propertyTypes')
    override_ok = False
    if isinstance(pt_node, dict):
        blob = json_dumps_safe(pt_node)
        override_ok = (
            '_AdaptiveModelPropertyTypes_' in blob and
            # should not solely be AdaptivePropertyTypes without Model
            ('_AdaptiveModelPropertyTypes_' in blob))
    tests.append(make_test(
        'model-object-type-propertyTypes-override',
        'ModelObjectType.propertyTypes overrides ObjectType with '
        'ModelPropertyTypes (not plain PropertyTypes only)',
        override_ok,
        detail=None if override_ok else repr(pt_node)[:200],
    ))

    # --- Real model instances ---
    for entry in MODEL_INSTANCES:
        rel_path, suffix, description, unwrap = entry
        path = root / rel_path
        if not path.is_file():
            tests.append(make_test(
                'instance-' + suffix,
                description,
                False,
                detail='file not found: ' + rel_path,
            ))
            continue
        try:
            instance = load_json(rel_path, root)
            if unwrap:
                if not isinstance(instance, dict) or unwrap not in instance:
                    tests.append(make_test(
                        'instance-' + suffix,
                        description,
                        False,
                        detail='missing envelope key ' + unwrap,
                    ))
                    continue
                instance = instance[unwrap]
        except Exception as exc:
            tests.append(make_test(
                'instance-' + suffix,
                description,
                False,
                detail=str(exc),
            ))
            continue
        ok, err = validate_instance(instance, model_schema)
        tests.append(make_test(
            'instance-' + suffix,
            description,
            ok,
            detail=err[:400] if err else None,
        ))
    # Structural gnarl: TestModel1 has hooks + mapped props + scripts
    try:
        test_model = load_json(
            'src/afw/tests/environments/models/objects/_AdaptiveModel_/'
            'TestModel1.json',
            root)
        ots = test_model.get('objectTypes') or {}
        tests.append(make_test(
            'test-model-1-has-multiple-object-types',
            'TestModel1 defines multiple model object types',
            len(ots) >= 5,
            detail='count=' + str(len(ots)),
        ))
        # at least one type with model hooks and one with mappedPropertyName
        has_hooks = any(
            isinstance(v, dict) and (
                'onGetObject' in v or 'onRetrieveObjects' in v)
            for v in ots.values())
        has_mapped_prop = any(
            isinstance(v, dict) and isinstance(v.get('propertyTypes'), dict) and
            any(
                isinstance(p, dict) and 'mappedPropertyName' in p
                for p in v['propertyTypes'].values())
            for v in ots.values())
        tests.append(make_test(
            'test-model-1-has-hooks-and-mapped-props',
            'TestModel1 uses on* hooks and mappedPropertyName',
            has_hooks and has_mapped_prop,
        ))
    except FileNotFoundError as exc:
        tests.append(make_test(
            'test-model-1-has-multiple-object-types',
            'TestModel1 defines multiple model object types',
            False,
            detail=str(exc),
        ))

    # TIER model scale
    tier_path = (
        'docker/images/afw/afw/models/_AdaptiveModel_/TIER Core Schema.json')
    try:
        tier = load_json(tier_path, root)
        n = len(tier.get('objectTypes') or {})
        tests.append(make_test(
            'tier-model-object-type-count',
            'TIER Core Schema has a substantial objectTypes map',
            n >= 10,
            detail='objectTypes count=' + str(n),
        ))
    except FileNotFoundError as exc:
        tests.append(make_test(
            'tier-model-object-type-count',
            'TIER Core Schema has a substantial objectTypes map',
            False,
            detail=str(exc),
        ))

    # Negative: model object type property with unknown field under closed MOT
    bad_model = {
        'modelId': 'bad',
        'objectTypes': {
            'T': {
                'mappedObjectType': 'X',
                'notAValidModelOrObjectTypeProperty': True,
            }
        },
    }
    ok, _ = validate_instance(bad_model, model_schema)
    tests.append(make_test(
        'neg-model-unknown-object-type-property',
        'Model rejects unknown properties on nested model object types',
        not ok,
        detail=None if not ok else 'unexpectedly valid',
    ))

    # Negative: model property type with unknown field
    bad_model2 = {
        'modelId': 'bad2',
        'objectTypes': {
            'T': {
                'propertyTypes': {
                    'p': {
                        'dataType': 'string',
                        'notAValidValueMetaOrModelProp': 1,
                    }
                }
            }
        },
    }
    ok, _ = validate_instance(bad_model2, model_schema)
    tests.append(make_test(
        'neg-model-unknown-property-type-field',
        'Model rejects unknown fields on nested model property types',
        not ok,
        detail=None if not ok else 'unexpectedly valid',
    ))

    return response


def json_dumps_safe(obj):
    import json
    return json.dumps(obj)
