#!/usr/bin/env python3
"""Shared helpers for JSON Schema projection tests."""

import json
import os
from pathlib import Path

try:
    import jsonschema
    from jsonschema import Draft202012Validator
except ImportError:  # pragma: no cover
    jsonschema = None
    Draft202012Validator = None


def package_root():
    """Locate the AFW package root (directory containing afw-package.json)."""
    here = Path(__file__).resolve()
    for parent in [here] + list(here.parents):
        if (parent / 'afw-package.json').is_file():
            return parent
    raise RuntimeError(
        'Could not find afw-package.json above ' + str(here))


def schemas_dir(root=None):
    root = root or package_root()
    return root / 'generated' / 'schemas' / 'afw'


def schemas_available(root=None):
    d = schemas_dir(root)
    return d.is_dir() and any(d.glob('*.json'))


def load_schema(object_type_id, root=None):
    path = schemas_dir(root) / (object_type_id + '.json')
    if not path.is_file():
        raise FileNotFoundError('Schema not found: ' + str(path))
    with open(path, encoding='utf-8') as fd:
        return json.load(fd)


def load_ot(object_type_id, root=None):
    """Load adaptive object type JSON from src/**/generated/objects."""
    root = root or package_root()
    pattern = '**/generated/objects/_AdaptiveObjectType_/' + object_type_id + '.json'
    matches = list(root.glob('src/' + pattern))
    if not matches:
        # also accept without src/ prefix for safety
        matches = list(root.glob(pattern))
    if not matches:
        raise FileNotFoundError(
            'Object type not found: ' + object_type_id)
    with open(matches[0], encoding='utf-8') as fd:
        return json.load(fd)


def load_json(path, root=None):
    root = root or package_root()
    p = Path(path)
    if not p.is_absolute():
        p = root / p
    with open(p, encoding='utf-8') as fd:
        return json.load(fd)


def iter_schema_files(root=None):
    d = schemas_dir(root)
    if not d.is_dir():
        return []
    return sorted(d.glob('*.json'))


def entity_def(schema_doc, object_type_id):
    return schema_doc.get('$defs', {}).get(object_type_id)


def property_types_def(schema_doc, object_type_id):
    return schema_doc.get('$defs', {}).get(object_type_id + '.propertyTypes')


def property_map(schema_doc, object_type_id):
    ptd = property_types_def(schema_doc, object_type_id)
    if not ptd:
        return {}
    return ptd.get('properties') or {}


def find_mixed_refs(obj, path='$'):
    """Return paths where a dict has $ref plus other keys."""
    issues = []
    if isinstance(obj, dict):
        if '$ref' in obj and len(obj) > 1:
            others = sorted(k for k in obj if k != '$ref')
            issues.append((path, others))
        for key, value in obj.items():
            issues.extend(find_mixed_refs(value, path + '.' + key))
    elif isinstance(obj, list):
        for i, value in enumerate(obj):
            issues.extend(find_mixed_refs(value, path + '[' + str(i) + ']'))
    return issues


def is_clean_ref_form(node):
    """True if node is pure $ref or annotations + allOf:[{$ref}]."""
    if not isinstance(node, dict):
        return False
    if list(node.keys()) == ['$ref']:
        return True
    all_of = node.get('allOf')
    if not (isinstance(all_of, list) and len(all_of) == 1):
        return False
    if not isinstance(all_of[0], dict) or list(all_of[0].keys()) != ['$ref']:
        return False
    # siblings of allOf must not include $ref or type (type comes from target)
    for key in node:
        if key in ('allOf', 'title', 'description', 'default', 'readOnly',
                   'enum', 'minimum', 'maximum', 'minLength', 'maxLength',
                   'uniqueItems', 'contentMediaType'):
            continue
        if key == '$ref':
            return False
        # allow nothing else that would re-mix $ref semantics
        if key in ('type', 'format', 'properties', 'items', 'required'):
            return False
    return True


def allof_property_types_refs(entity):
    """Set of object-type ids referenced as .propertyTypes in entity allOf."""
    refs = set()
    if not entity:
        return refs
    for entry in entity.get('allOf') or []:
        ref = entry.get('$ref', '')
        # #/$defs/Foo.propertyTypes
        if ref.startswith('#/$defs/') and ref.endswith('.propertyTypes'):
            refs.add(ref[len('#/$defs/'):-len('.propertyTypes')])
    return refs


def parent_ot_ids_from_adaptive(ot_json):
    """Object type ids from propertyTypes._meta_.parentPaths (recursive one-level list)."""
    parents = []
    pts = ot_json.get('propertyTypes') or {}
    meta = pts.get('_meta_') or {}
    for parent_path in meta.get('parentPaths') or []:
        parts = [p for p in str(parent_path).split('/') if p and p != '*']
        if not parts:
            continue
        if parts[-1] == 'propertyTypes' and len(parts) >= 2:
            parents.append(parts[-2])
        else:
            parents.append(parts[-1])
    return parents


def ancestor_ot_ids(object_type_id, root=None, visited=None):
    """All ancestor OT ids via parentPaths, depth-first, no cycles."""
    if visited is None:
        visited = set()
    if object_type_id in visited:
        return []
    visited.add(object_type_id)
    ot = load_ot(object_type_id, root)
    result = []
    for parent_id in parent_ot_ids_from_adaptive(ot):
        if parent_id in visited:
            continue
        result.append(parent_id)
        result.extend(ancestor_ot_ids(parent_id, root, visited))
    return result


def ot_local_property_names(ot_json):
    pts = ot_json.get('propertyTypes') or {}
    return sorted(k for k in pts if k != '_meta_' and isinstance(pts[k], dict))


def ot_local_required(ot_json):
    pts = ot_json.get('propertyTypes') or {}
    return sorted(
        k for k, v in pts.items()
        if k != '_meta_' and isinstance(v, dict) and v.get('required') is True)


def validate_instance(instance, schema_doc):
    """Return (ok: bool, error_message: str|None)."""
    if Draft202012Validator is None:
        return False, 'jsonschema package not available'
    try:
        Draft202012Validator(schema_doc).validate(instance)
        return True, None
    except Exception as exc:
        return False, str(exc)


def make_test(test_id, description, passed, detail=None, skip=False):
    item = {
        'test': test_id,
        'description': description,
        'passed': bool(passed),
    }
    if skip:
        item['skip'] = True
        item['passed'] = True
    if detail and not passed and not skip:
        item['description'] = description + ' — ' + detail
    return item


def skip_if_no_schemas(tests_list):
    """Append a single skip/fail and return True if schemas are missing."""
    if schemas_available():
        return False
    tests_list.append(make_test(
        'schemas-dir-missing',
        'generated/schemas/afw not found or empty '
        '(run generate / build so schemas exist before tests)',
        passed=False,
        detail=str(schemas_dir()),
    ))
    return True
