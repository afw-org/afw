#! /usr/bin/env python3

##
# @file json_schema.py
# @ingroup afwdev_generate
# @brief Generate package-root JSON Schema files from adaptive object types.
# @details
# Adaptive object types under src/**/generated/objects/_AdaptiveObjectType_/
# are the source of truth. This module emits a JSON Schema (draft 2020-12)
# projection into generated/schemas/afw/.
#
# Primary purpose: help editors (VS Code json.schemas, etc.) when authoring
# adaptive object JSON under generate/objects/ — completion, hovers, and light
# validation. Schemas are generated and read-only, so verbosity is fine; prefer
# structures editors resolve well (flat property maps, explicit type object,
# $defs + $ref) over compact or validator-only forms.
#
# Secondary: afwdev validate / tests. Not a full adaptive meta model (runtime,
# allowQuery, etc. are omitted). Client-side UI validation should stay simple;
# these files are not optimized as a minimal runtime payload.
#
# Mapping choices:
# - Object-typed properties: pure $ref, or annotations + type object +
#   allOf:[{$ref}] (never $ref mixed with siblings; issue #3).
# - Inheritance: merge property maps with child override (not stacked allOf of
#   parent maps, which breaks overrides and confuses editors).
# - required applies only to the leaf object type.
#

import glob
import os

from _afwdev.common import msg, package, resources, nfc

# Adaptive dataTypeParameterType values with no JSON Schema projection yet.
_UNMAPPED_PARAMETER_TYPES = frozenset({
    'SourceParameter',
    'FunctionSignature',
    'Type',
    'xpathExpression',
})


##
# @brief Per-generate build state (object types, data types, emitted schemas).
#
class _SchemaBuildContext:

    def __init__(self, options):
        self.options = options
        self.data_types = {}
        self.object_types = {}
        self.schemas = {}
        self.ref_prefix = '#/$defs/'
        self.ref_suffix = ''
        self.json_schema_uri = 'https://json-schema.org/draft/2020-12/schema'

    def ref(self, name):
        return self.ref_prefix + name + self.ref_suffix


# ---------------------------------------------------------------------------
# VS Code settings (optional side effect of generate)
# ---------------------------------------------------------------------------

def _fileMatch_exists(json_schema_entries, pattern):
    for entry in json_schema_entries:
        for fileMatch in entry.get('fileMatch') or []:
            if fileMatch == pattern:
                return True
    return False


def _update_vscode_settings_json_schema(options):
    """Ensure .vscode/settings.json maps generate object paths to schemas."""
    filename = '.vscode/settings.json'
    fullpath = options['afw_package_dir_path'] + filename
    was_updated = False

    if os.path.exists(fullpath):
        with nfc.open(fullpath, 'r') as fd:
            settings = nfc.json_load(fd)
    else:
        settings = {}

    json_schema_entries = settings.get('json.schemas')
    if json_schema_entries is None:
        json_schema_entries = []

    if not _fileMatch_exists(json_schema_entries, '/afw-package.json'):
        json_schema_entries.append({
            'fileMatch': ['/afw-package.json'],
            'url': './generated/schemas/afw/_AdaptivePackage_.json',
        })
        msg.info('Added fileMatch /afw-package.json to ' + filename +
                 ' json.schemas')
        was_updated = True

    relative_schema_path = 'generated/schemas/afw/'
    schema_glob = options['afw_package_dir_path'] + relative_schema_path + '*.json'
    prefix_len = len(options['afw_package_dir_path'] + relative_schema_path)
    for full_schema_path in glob.glob(schema_glob):
        objectType = full_schema_path[prefix_len:-5]
        fileMatch = '/src/*/generate*/objects/' + objectType + '/*.json'
        if not _fileMatch_exists(json_schema_entries, fileMatch):
            json_schema_entries.append({
                'fileMatch': [fileMatch],
                'url': './generated/schemas/afw/' + objectType + '.json',
            })
            msg.info('Added fileMatch ' + fileMatch + ' to ' + filename +
                     ' json.schemas')
            was_updated = True

    if was_updated:
        settings['json.schemas'] = json_schema_entries
        os.makedirs(options['afw_package_dir_path'] + '.vscode', exist_ok=True)
        with nfc.open(fullpath, 'w') as fd:
            nfc.json_dump(settings, fd, indent=4, sort_keys=True)
        msg.success(
            'Updated settings.json json.schemas for new object types '
            '(restart vscode required to take effect)')


# ---------------------------------------------------------------------------
# $ref helpers (issue #3 + editor-friendly object refs)
# ---------------------------------------------------------------------------

def _editor_docs(schema_node):
    """
    Duplicate description into markdownDescription for VS Code hovers.

    vscode.json-language-features recognizes markdownDescription on schemas.
    """
    if schema_node.get('description') and 'markdownDescription' not in schema_node:
        schema_node['markdownDescription'] = schema_node['description']


def _ref_schema(ref, base=None):
    """
    Build a schema that references another without mixing $ref and siblings.

    Editor-oriented forms (verbose is fine; schemas are generated):
    - No annotations: { "$ref": "..." }  (VS Code follows $ref cleanly)
    - With annotations: {
        "type": "object",
        "title", "description", "markdownDescription", ...
        "allOf": [ { "$ref": "..." } ]
      }
      type sits beside allOf, never beside $ref (issue #3).
    """
    annotations = {}
    if base:
        for key, value in base.items():
            # Target supplies structural type; never re-emit $ref/allOf here.
            if key in ('$ref', 'type', 'format', 'allOf'):
                continue
            annotations[key] = value
    if not annotations:
        return {'$ref': ref}
    result = dict(annotations)
    # Explicit object type helps completion before $ref is fully resolved.
    result['type'] = 'object'
    result['allOf'] = [{'$ref': ref}]
    _editor_docs(result)
    return result


def _apply_annotations(schema, annotations):
    """Attach title/description/etc. without putting them beside a bare $ref."""
    if not annotations:
        return schema
    if list(schema.keys()) == ['$ref']:
        return _ref_schema(schema['$ref'], annotations)
    all_of = schema.get('allOf')
    if (isinstance(all_of, list) and len(all_of) == 1 and
            isinstance(all_of[0], dict) and
            list(all_of[0].keys()) == ['$ref']):
        base = {k: v for k, v in schema.items() if k not in ('allOf', 'type')}
        base.update(annotations)
        return _ref_schema(all_of[0]['$ref'], base)
    result = dict(schema)
    result.update(annotations)
    _editor_docs(result)
    return result


# ---------------------------------------------------------------------------
# Data type / property conversion
# ---------------------------------------------------------------------------

def _set_type(ctx, schema_node, data_type_id):
    """Map adaptive dataType id onto JSON Schema type/format. Returns DT object."""
    data_type = ctx.data_types.get(data_type_id)
    if data_type is None:
        msg.error('Unknown data type ' + data_type_id)
        return None
    if data_type.get('jsonPrimitive') is None:
        msg.warn('Missing jsonPrimitive for data type ' + data_type_id)
        return None

    # Adaptive "integer" uses JSON Schema integer (not number).
    if data_type_id == 'integer':
        schema_node['type'] = 'integer'
    else:
        schema_node['type'] = data_type['jsonPrimitive']

    if 'jsonSchemaStringFormat' in data_type:
        schema_node['format'] = data_type['jsonSchemaStringFormat']
    return data_type


def _apply_property_annotations(schema_node, property_type,
                                description=None, title=None):
    """Copy title, description, default, readOnly, enum from adaptive value meta."""
    if title is not None:
        schema_node['title'] = title
    elif 'label' in property_type:
        schema_node['title'] = property_type['label']
    elif 'brief' in property_type:
        schema_node['title'] = property_type['brief']

    if description is not None:
        schema_node['description'] = description
    elif 'description' in property_type:
        schema_node['description'] = property_type['description']

    # Prefer label as completion text; brief as secondary hover when both exist.
    if ('brief' in property_type and
            property_type.get('brief') and
            schema_node.get('description') and
            property_type.get('brief') != schema_node.get('description') and
            'markdownDescription' not in schema_node):
        schema_node['markdownDescription'] = (
            '**' + str(property_type.get('label') or
                       property_type.get('brief')) + '**\n\n' +
            str(schema_node['description']))
    else:
        _editor_docs(schema_node)

    if 'defaultValue' in property_type:
        schema_node['default'] = property_type['defaultValue']
    elif 'default' in property_type:
        schema_node['default'] = property_type['default']

    if 'allowWrite' in property_type:
        schema_node['readOnly'] = not property_type['allowWrite']

    if 'allowedValues' in property_type:
        schema_node['enum'] = property_type['allowedValues']
    elif 'possibleValues' in property_type:
        schema_node['enum'] = property_type['possibleValues']


def _apply_type_constraints(schema_node, property_type):
    """Map min/max/length/unique onto JSON Schema keywords by JSON type."""
    json_type = schema_node.get('type')
    if json_type in ('integer', 'number'):
        if 'minValue' in property_type:
            schema_node['minimum'] = property_type['minValue']
        if 'maxValue' in property_type:
            schema_node['maximum'] = property_type['maxValue']
    elif json_type == 'string':
        if 'minLength' in property_type:
            schema_node['minLength'] = property_type['minLength']
        if 'maxLength' in property_type:
            schema_node['maxLength'] = property_type['maxLength']
    elif json_type == 'array' and property_type.get('unique') is True:
        schema_node['uniqueItems'] = True


def _array_items_for_object_type(ctx, needed, object_type_id):
    _add_definition(ctx, needed, object_type_id, propertyTypesOnly=False)
    return _ref_schema(ctx.ref(object_type_id))


def _parse_array_data_type_parameter(ctx, needed, dataTypeParameter):
    """
    Parse adaptive ArrayOf dataTypeParameter into an items subschema.

    Accepts primitives (string, integer, ...), object types
    (object _AdaptiveX_ or bare _AdaptiveX_), and nested array of ...
    """
    if not dataTypeParameter or not str(dataTypeParameter).strip():
        return None

    tokens = dataTypeParameter.strip().split(' ', 1)
    head = tokens[0]
    rest = tokens[1] if len(tokens) > 1 else None

    # Nested arrays: "array of <element>"
    if head == 'array':
        if rest is None:
            return {'type': 'array'}
        rest_tokens = rest.split(' ', 1)
        if rest_tokens[0] != 'of' or len(rest_tokens) < 2:
            return None
        nested = _parse_array_data_type_parameter(ctx, needed, rest_tokens[1])
        if nested is None:
            return None
        return {'type': 'array', 'items': nested}

    # Bare object type id (layout components often omit the "object " prefix).
    if (head not in ctx.data_types and rest is None and
            (head in ctx.object_types or head.startswith('_Adaptive'))):
        return _array_items_for_object_type(ctx, needed, head)

    items = {}
    data_type = _set_type(ctx, items, head)
    if not data_type:
        return None

    ptype = data_type.get('dataTypeParameterType')
    if ptype is not None and rest is not None:
        if ptype == 'ObjectType':
            return _array_items_for_object_type(ctx, needed, rest)
        if ptype == 'MediaType':
            items['contentMediaType'] = rest

    return items


def _convert_property_type(ctx, needed, property_type, property_id_for_msg,
                           description=None, title=None):
    """Convert one adaptive propertyTypes entry to a JSON Schema property."""
    schema_node = {}
    data_type = None

    if property_type.get('dataType'):
        data_type = _set_type(ctx, schema_node, property_type['dataType'])

    _apply_property_annotations(
        schema_node, property_type, description=description, title=title)
    _apply_type_constraints(schema_node, property_type)

    if (data_type and
            'dataTypeParameter' in property_type and
            'dataTypeParameterType' in data_type):
        ptype = data_type['dataTypeParameterType'].strip()
        param = property_type['dataTypeParameter']

        if ptype == 'ObjectType':
            _add_definition(
                ctx, needed, param, propertyTypesOnly=False,
                description=property_type.get('description'),
                title=property_type.get('label'))
            schema_node = _ref_schema(ctx.ref(param), schema_node)

        elif ptype == 'ArrayOf':
            items = _parse_array_data_type_parameter(ctx, needed, param)
            if items:
                item_annotations = {}
                if 'description' in property_type:
                    item_annotations['description'] = property_type['description']
                if 'brief' in property_type:
                    item_annotations['title'] = property_type['brief']
                schema_node['items'] = _apply_annotations(
                    items, item_annotations)
            else:
                msg.warn('Invalid ' + property_id_for_msg +
                         ' dataTypeParameter')

        elif ptype == 'MediaType':
            schema_node['contentMediaType'] = param

        elif ptype in _UNMAPPED_PARAMETER_TYPES:
            pass

        else:
            msg.warn(
                'Unknown dataTypeParameterType ' + ptype +
                ' in dataType ' + property_type['dataType'])

    return schema_node


# ---------------------------------------------------------------------------
# Object type conversion and inheritance
# ---------------------------------------------------------------------------

def _parent_object_type_from_path(parent_path):
    """
    Extract object type id from propertyTypes._meta_.parentPaths entry.

    Supports:
      /afw/_AdaptiveObjectType_/_AdaptiveConf_/propertyTypes
      /*/*/_AdaptiveLayoutComponentType_Common/propertyTypes
    """
    if not parent_path or not isinstance(parent_path, str):
        return None
    parts = [p for p in parent_path.split('/') if p and p != '*']
    if not parts:
        return None
    if parts[-1] == 'propertyTypes':
        if len(parts) < 2:
            return None
        return parts[-2]
    return parts[-1]


def _process_parents(ctx, needed, object_type, meta, visited=None):
    """
    Build allOf $refs for ancestor .propertyTypes (composite inheritance).

    Walks parentPaths recursively with cycle detection. Required is not
    inherited here — only property shape maps.
    """
    if not meta or 'parentPaths' not in meta:
        return []

    if visited is None:
        visited = set()

    parent_refs = []
    for parent_path in meta['parentPaths']:
        parent_id = _parent_object_type_from_path(parent_path)
        if not parent_id:
            msg.warn(object_type + ' parentPath could not be parsed: ' +
                     str(parent_path))
            continue
        if not str(parent_path).rstrip('/').endswith('propertyTypes'):
            msg.warn(object_type +
                     ' parentPath does not end with /propertyTypes: ' +
                     str(parent_path))

        if parent_id in visited:
            continue
        visited.add(parent_id)

        parent_refs.append({'$ref': ctx.ref(parent_id + '.propertyTypes')})
        _add_definition(ctx, needed, parent_id, propertyTypesOnly=True)

        parent_ot = ctx.object_types.get(parent_id)
        if parent_ot and isinstance(parent_ot.get('propertyTypes'), dict):
            parent_meta = parent_ot['propertyTypes'].get('_meta_')
            if parent_meta:
                parent_refs.extend(
                    _process_parents(
                        ctx, needed, parent_id, parent_meta, visited=visited))

    return parent_refs


def _ensure_property_types_schema(ctx, needed, object_type, object_type_object):
    """
    Build ObjectType.propertyTypes def: local property map only (no required).

    Kept for tooling/docs and as a building block. Entity schemas use a
    merged composite map so child property definitions override parents
    (adaptive composite), which plain allOf of parent maps cannot express.
    """
    key = object_type + '.propertyTypes'
    if key in needed or object_type == '_AdaptivePropertyTypes_':
        return
    needed.append(key)

    properties = {}
    for name, prop in (object_type_object.get('propertyTypes') or {}).items():
        # _meta_ is inheritance metadata, not an instance property.
        if name == '_meta_' or not isinstance(prop, dict):
            continue
        properties[name] = _convert_property_type(
            ctx, needed, prop,
            'objectType ' + object_type + ' property ' + name)

    ctx.schemas[key] = {
        'type': 'object',
        'properties': properties,
    }


def _ancestor_object_type_ids(ctx, object_type_object):
    """Return ancestor object type ids from parentPaths (nearest parent first)."""
    ordered = []
    seen = set()

    def walk(meta):
        if not meta or 'parentPaths' not in meta:
            return
        for parent_path in meta['parentPaths']:
            parent_id = _parent_object_type_from_path(parent_path)
            if not parent_id or parent_id in seen:
                continue
            seen.add(parent_id)
            ordered.append(parent_id)
            parent_ot = ctx.object_types.get(parent_id)
            if parent_ot and isinstance(parent_ot.get('propertyTypes'), dict):
                walk(parent_ot['propertyTypes'].get('_meta_'))

    pts = object_type_object.get('propertyTypes')
    if isinstance(pts, dict):
        walk(pts.get('_meta_'))
    return ordered


def _merged_instance_properties(ctx, needed, object_type, object_type_object):
    """
    Merge propertyTypes along the inheritance chain with child override.

    Ancestors first, then this object type. Same-named properties from the
    child replace the parent (e.g. ModelObjectType.propertyTypes overrides
    ObjectType.propertyTypes). Returns a properties dict for the entity.
    """
    merged = {}
    # Farthest ancestor first so nearer ancestors and self overwrite.
    ancestor_ids = _ancestor_object_type_ids(ctx, object_type_object)
    chain = []
    for ancestor_id in reversed(ancestor_ids):
        ancestor_ot = ctx.object_types.get(ancestor_id)
        if ancestor_ot is not None:
            chain.append((ancestor_id, ancestor_ot))
        _ensure_property_types_schema(
            ctx, needed, ancestor_id, ancestor_ot or {})
    chain.append((object_type, object_type_object))

    for ot_id, ot_obj in chain:
        for name, prop in (ot_obj.get('propertyTypes') or {}).items():
            if name == '_meta_' or not isinstance(prop, dict):
                continue
            merged[name] = _convert_property_type(
                ctx, needed, prop,
                'objectType ' + ot_id + ' property ' + name)
    return merged


def _property_types_meta_schema():
    """Synthetic schema for _AdaptivePropertyTypes_._meta_ (core-implied)."""
    return {
        'properties': {
            'parentPaths': {
                'title': 'Parent Paths',
                'description':
                    'This is a list of paths to the parent object type.',
                'type': 'array',
                'items': {
                    'type': 'string',
                    'title': 'Parent Path',
                    'description':
                        'This is a path to a parent object type.',
                },
            }
        },
        'description':
            'This is the special Meta object that has deltas between this '
            'instance and its Adaptive Object Type.',
        'title': 'Meta',
    }


def _convert_object_type(ctx, needed, object_type, object_type_object,
                         propertyTypesOnly=False, description=None, title=None):
    """Convert one adaptive object type into ctx.schemas entries."""
    if object_type_object is None:
        object_type_object = {}

    _ensure_property_types_schema(ctx, needed, object_type, object_type_object)

    if propertyTypesOnly or object_type in needed:
        return

    needed.append(object_type)
    schema = {'type': 'object'}

    # Required only for instances of this object type (not ancestors).
    local_required = []
    for name, prop in (object_type_object.get('propertyTypes') or {}).items():
        if name == '_meta_' or not isinstance(prop, dict):
            continue
        if prop.get('required', False):
            local_required.append(name)

    if object_type == '_AdaptivePropertyTypes_':
        schema['properties'] = {'_meta_': _property_types_meta_schema()}
    else:
        # Composite map: inherited + local with child property override.
        # Also keep allOf refs to each .propertyTypes for documentation of
        # the inheritance chain (validators use properties= merged map).
        merged = _merged_instance_properties(
            ctx, needed, object_type, object_type_object)
        # Optional adaptive instance meta bag (common on real objects).
        merged.setdefault('_meta_', {
            'type': 'object',
            'additionalProperties': True,
            'title': 'Meta',
            'description':
                'Adaptive instance meta (objectId, path, objectType, ...).',
            'markdownDescription':
                'Adaptive instance meta (`objectId`, `path`, `objectType`, …).',
        })
        schema['properties'] = merged

        # Inheritance chain for tests/tooling only (not applied as allOf).
        inherited = _ancestor_object_type_ids(ctx, object_type_object)
        if inherited:
            schema['x-afw-inheritedObjectTypes'] = inherited
            _process_parents(
                ctx, needed, object_type,
                (object_type_object.get('propertyTypes') or {}).get('_meta_')
                or {})

    # otherProperties → additionalProperties; missing → closed object.
    # Prefer additionalProperties (widely understood by editors) over
    # unevaluatedProperties.
    if 'otherProperties' in object_type_object:
        other = object_type_object['otherProperties']
        if other is None or other == {}:
            schema['additionalProperties'] = True
        else:
            schema['additionalProperties'] = _convert_property_type(
                ctx, needed, other,
                object_type + ' additionalProperties',
                description=object_type_object.get('description'),
                title=object_type_object.get('label'))
    else:
        schema['additionalProperties'] = False

    if local_required:
        schema['required'] = local_required

    if description is not None:
        schema['description'] = description
    elif 'description' in object_type_object:
        schema['description'] = object_type_object['description']
    _editor_docs(schema)

    if title is not None:
        schema['title'] = title
    elif 'title' in object_type_object:
        schema['title'] = object_type_object['title']
    elif 'objectType' in object_type_object:
        # Editors show title in schema pickers / hovers.
        schema['title'] = object_type_object['objectType']

    ctx.schemas[object_type] = schema


def _add_definition(ctx, needed, object_type, propertyTypesOnly=False,
                    description=None, title=None):
    """Ensure object_type (and deps) are converted into ctx.schemas."""
    if object_type in needed:
        return

    object_type_object = ctx.object_types.get(object_type)
    if object_type_object is None:
        msg.warn('Unable to load object type ' + object_type)

    _convert_object_type(
        ctx, needed, object_type, object_type_object,
        propertyTypesOnly=propertyTypesOnly,
        description=description, title=title)


# ---------------------------------------------------------------------------
# Public entry
# ---------------------------------------------------------------------------

def _load_data_types(ctx):
    entries = resources.copy_resources(
        ctx.options, 'objects/_AdaptiveDataTypeGenerate_/')
    for entry in entries:
        data_type_object = nfc.json_loads(entry['resource'])
        ctx.data_types[data_type_object['dataType']] = data_type_object


def _load_object_types(ctx):
    options = ctx.options
    if not options['is_core_afw_package']:
        for object_type_object in resources.get_core_object_types(options):
            object_type = object_type_object['_meta_']['objectId']
            ctx.object_types[object_type] = object_type_object

    src_dir_path = options['afw_package_dir_path'] + 'src/'
    pattern = src_dir_path + '**/generated/objects/_AdaptiveObjectType_/*.json'
    for object_type_file in glob.glob(pattern, recursive=True):
        object_type = os.path.basename(object_type_file)[:-len('.json')]
        msg.info('Generating JSON schema for ' + object_type)
        with nfc.open(object_type_file, 'r') as fd:
            ctx.object_types[object_type] = nfc.json_load(fd)


def _write_entity_schemas(ctx):
    """
    Write one schema file per allowEntity object type.

    allowEntity defaults to true when omitted (matches adaptive OT meta).

    Document shape is editor-first: the entity schema is at the document root
    (type/properties/required/...) so VS Code applies it directly to matched
    files. $defs holds the entity and nested types for $ref resolution.
    """
    schemas_dir = ctx.options['afw_package_dir_path'] + 'generated/schemas/afw/'
    os.makedirs(schemas_dir, exist_ok=True)

    for object_type, object_type_object in ctx.object_types.items():
        if object_type_object.get('allowEntity', True) is False:
            continue

        filename = object_type + '.json'
        msg.info('Writing ' + schemas_dir + filename)

        needed = []
        _convert_object_type(
            ctx, needed, object_type, object_type_object,
            propertyTypesOnly=False)

        defs = {}
        for need in sorted(set(needed)):
            if need not in ctx.schemas:
                msg.warn(
                    'Missing schema definition for ' + need +
                    ' while writing ' + object_type)
                continue
            defs[need] = ctx.schemas[need]

        entity = ctx.schemas.get(object_type)
        if entity is None:
            msg.warn('No entity schema for ' + object_type)
            continue

        # Root = entity keywords (editor applies this to the open file).
        # $defs keeps the same entity plus dependencies for internal $refs.
        document = {
            '$schema': ctx.json_schema_uri,
            '$comment':
                'Generated from adaptive object type ' + object_type +
                '. Primary use: editor completion/validation for '
                'generate/objects JSON. Do not hand-edit.',
            '$defs': defs,
        }
        for key, value in entity.items():
            document[key] = value

        with nfc.open(schemas_dir + filename, 'w') as fd:
            nfc.json_dump(document, fd, indent=4, sort_keys=True)


def generate(options):
    """Generate generated/schemas/afw/*.json and update VS Code schema map."""
    msg.info(
        'Generating JSON schema files for generated _AdaptiveObjectType_ '
        'objects')

    ctx = _SchemaBuildContext(options)
    afw_package = package.get_afw_package(options)

    # Reserved for possible future $id emission.
    options['json_schema_id_base_uri'] = (
        'https://adaptiveframework.org/' +
        afw_package['afwPackageId'] + '-' + afw_package['version'] +
        '/schemas/afw/')
    options['json_compound_schema_document_uri'] = (
        'https://adaptiveframework.org/' +
        afw_package['afwPackageId'] + '-' + afw_package['version'] +
        '/schema')
    options['json_schema_uri'] = ctx.json_schema_uri

    _load_data_types(ctx)
    _load_object_types(ctx)
    _write_entity_schemas(ctx)
    _update_vscode_settings_json_schema(options)
    msg.success('Generate JSON schema files successful')
