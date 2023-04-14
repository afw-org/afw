#! /usr/bin/env python3

import os
import glob
from _afwdev.common import msg, package, resources, nfc

_data_types = None

# Look for pattern in setting.json json.schema.
def _fileMatch_exists(json_schema, pattern):
    for entry in json_schema:
        if entry.get('fileMatch'):
            for fileMatch in entry['fileMatch']:
                if fileMatch == pattern:
                    return True
    return False

# Update setting.json json.schema with unmatched object type schemas patterns.
def _update_vscode_settings_json_schema(options):
    filename = '.vscode/settings.json'
    fullpath = options['afw_package_dir_path'] + filename
    was_updated = False
    if os.path.exists(fullpath):
        with nfc.open(fullpath, 'r') as fd:
            settings = nfc.json_load(fd)
    else:
        settings = {}

    json_schema = settings.get('json.schemas')
    if json_schema is None:
        json_schema = []

    # Make sure entry for afw-package.json exists.
    if not _fileMatch_exists(json_schema, '/afw-package.json'):
        fileMatch = "/afw-package.json"
        json_schema.append({
            "fileMatch": [ fileMatch ],
            "url": "./generated/schemas/afw/_AdaptivePackage_.json"
        })
        msg.info('Added fileMatch ' + fileMatch + ' to ' + filename + ' json.schemas')
        was_updated = True

    # Make sure entries exist for all schemas in generated/schemas/afw/.
    relative_schema_path = 'generated/schemas/afw/'
    for full_schema_path in glob.glob(options['afw_package_dir_path'] + relative_schema_path + '*.json'):
        objectType = full_schema_path[len(options['afw_package_dir_path'] + relative_schema_path):-5]
        fileMatch = '/src/*/generate*/objects/' + objectType + '/*.json'
        if not _fileMatch_exists(json_schema, fileMatch):
            json_schema.append({
                "fileMatch": [ fileMatch ],
                "url": "./generated/schemas/afw/" + objectType + ".json"
            })
            msg.info('Added fileMatch ' + fileMatch + ' to ' + filename + ' json.schemas')
            was_updated = True

    if was_updated:
        settings['json.schemas'] = json_schema
        os.makedirs(options['afw_package_dir_path'] + '.vscode', exist_ok=True)
        with nfc.open(fullpath, 'w') as fd:
            nfc.json_dump(settings, fd, indent=4, sort_keys=True)
        msg.success('Update to settings.json json.schemas property for new object types successful (restart vscode required to take effect)')


# Add objectType's needs and make sure they're in global _all_schemas.
def _add_definition(mutable_objectTypes_needed,
    options, objectType, ref_prefix, ref_suffix,
    propertyTypesOnly=False, description=None, title=None):

    global _all_schemas
    global _all_object_type_objects

    # If the objectType is already in definitions, return.
    if objectType in mutable_objectTypes_needed:
        return
    
    # Load the objectTypeObject, convert it to a JSON schema, and add it to the
    # definitions.
    objectTypeObject = _all_object_type_objects.get(objectType)
    if objectTypeObject is None:
        msg.warn('Unable to load object type ' + objectType)

    # Set objectType property in definitions even if objectTypeObject wasn't
    # found so it wont be tried again.
    _convert_objectType(mutable_objectTypes_needed,
        options, objectType, objectTypeObject, ref_prefix, ref_suffix,
        propertyTypesOnly=propertyTypesOnly,
        description=description, title=title)


# Parse dataTypeParameter for list. Return None if invalid or items for
# subschema if good.
def _parse_dataTypeParameter_for_list(mutable_objectTypes_needed,
        options, dataTypeParameter, ref_prefix, ref_suffix) :

    t = dataTypeParameter.split(" ", 1)
    if len(t) == 0:
        return None
    if t[0] == 'list':
        items = {'type':'array'}
        if len(t) > 1:
            t = t[0].split(" ", 1)
            if t[0] != 'of' or len(t) < 2:
                return None
            r = _parse_dataTypeParameter_for_list(mutable_objectTypes_needed,
                options, t[1], ref_prefix, ref_suffix)
            if r is None:
                return None
            items['items'] = r
    else:
        items = {}
        data_type = _set_type(items, t[0])
        if not data_type:
            return None
        ptype = data_type.get('dataTypeParameterType')
        if ptype is not None and len(t) > 1:
            if ptype == 'ObjectType':
                _add_definition(mutable_objectTypes_needed,
                    options, t[1], ref_prefix, ref_suffix,
                    propertyTypesOnly=False)
                items['$ref'] = ref_prefix + t[1] + ref_suffix
            elif ptype == 'MediaType':
                property['contentMediaType'] = t[1]
    
    return items


# Set type and optional format for data type.
def _set_type(mutable_property, data_type_id):
    data_type = _data_types.get(data_type_id)
    if data_type is None:
        msg.error('Unknown data type ' + data_type_id)
        return None
    if data_type.get('jsonPrimitive') is not None:          
        mutable_property['type'] = data_type['jsonPrimitive']
        # Override for integer since JSON Schema supports integer.
        if data_type_id == "integer":
            mutable_property['type'] = 'integer'
    else:
        msg.warn('Missing jsonPrimitive for data type ' + data_type_id)
        return None
    
    if 'jsonSchemaStringFormat' in data_type:
        mutable_property['format'] = data_type['jsonSchemaStringFormat']
        
    return data_type


# Convert propertyType.
def _convert_propertyType(
        mutable_objectTypes_needed, options, propertyType, property_id_for_msg,
        ref_prefix, ref_suffix, description=None, title=None):

    property = {}

    # Convert dataType to type
    data_type = None
    if propertyType.get('dataType'):
        _set_type(property, propertyType['dataType'])
        data_type = _data_types.get(propertyType['dataType'])
    
    # If the propertyType has a label or title passed, use it as the title.
    if title:
        property['title'] = title
    elif 'label' in propertyType:
        property['title'] = propertyType['label']

    # If the propertyType has a default, use it as the default.
    if 'default' in propertyType:
        property['default'] = propertyType['default']

    # If propertyType has a description or one passed, use it as the description.
    if description:
        property['description'] = description
    elif 'description' in propertyType:
        property['description'] = propertyType['description']

    # If the propertyType has allowWrite, use it to determine readOnly.
    if 'allowWrite' in propertyType:
        property['readOnly'] = not propertyType['allowWrite']   

    # Any type
    if 'allowedValues' in propertyType:
        property['enum'] = propertyType['allowedValues']

    # JSON Schema extra type integer
    if property.get('type') == 'integer':
        if 'minValue' in propertyType:
            property['minimum'] = propertyType['minValue']
        if 'maxValue' in propertyType:
            property['maximum'] = propertyType['maxValue']

    # jsonPrimitive number
    elif property.get('type') == 'number':
        if 'minValue' in propertyType:
            property['minimum'] = propertyType['minValue']
        if 'maxValue' in propertyType:
            property['maximum'] = propertyType['maxValue']

    # jsonPrimitive string
    elif property.get('type') == 'string':
        if 'minLength' in propertyType:
            property['minLength'] = propertyType['minLength']
        if 'maxLength' in propertyType:
            property['maxLength'] = propertyType['maxLength']

    # jsonPrimitive array
    elif property.get('type') == 'array':
        pass

    # jsonPrimitive object
    elif property.get('type') == 'object':
        pass
 
    # jsonPrimitive boolean
    elif property.get('dataType') == 'boolean':
        pass

    # jsonPrimitive null
    elif property.get('dataType') == 'null':
        pass

    # Handle dataTypeParameter
    if (data_type and 
        'dataTypeParameter' in propertyType and
        'dataTypeParameterType' in data_type):
        ptype = data_type['dataTypeParameterType'].strip()
        param = propertyType['dataTypeParameter']
        if ptype == 'ObjectType':
            _add_definition(mutable_objectTypes_needed,
                options, param, ref_prefix, ref_suffix,
                propertyTypesOnly=False,
                description=propertyType.get('description'),
                title=propertyType.get('label'))
            property['$ref'] = ref_prefix + param + ref_suffix
        elif ptype == 'ListOf':
            items = _parse_dataTypeParameter_for_list(mutable_objectTypes_needed,
                options, param, ref_prefix, ref_suffix)           
            if "description" in propertyType:
                items["description"] = propertyType["description"]
            if "brief" in propertyType:
                items["title"] = propertyType["brief"]     
            if items:
                property['items'] = items
            else:
                msg.warn('Invalid ' + property_id_for_msg + ' dataTypeParameter')
        elif ptype == 'MediaType':
            property['contentMediaType'] = param 
        elif ptype == 'SourceParameter':
            pass
        elif ptype == 'FunctionSignature':
            pass
        elif ptype == 'Type':
            pass
        elif ptype == 'xpathExpression':
            pass
        else:
            msg.warn('Unknown dataTypeParameterType ' + ptype + ' in dataType ' + propertyType['dataType'])

    return property

# Process _meta_ for parent.
def _process_parents(mutable_objectTypes_needed, options, objectType, meta,
    ref_prefix='./',
    ref_suffix='.json'):

    if not 'parentPaths' in meta:
        return None
    
    anyOf = []
    for parentPath in meta['parentPaths']:
        if parentPath.startswith('/afw/_AdaptiveObjectType_/'):
            parentObjectType = parentPath[26:]
            if parentObjectType.endswith('/propertyTypes'):
                parentObjectType = parentObjectType[:-14]
            else:
                msg.warn(objectType + 'parentPath does not end with /propertyTypes')
            anyOf.append({'$ref': ref_prefix + parentObjectType + '.propertyTypes' + ref_suffix})
            _add_definition(mutable_objectTypes_needed,
                options, parentObjectType, ref_prefix, ref_suffix,
                propertyTypesOnly=True)
            # parentsAnyOf = _process_parents(mutable_objectTypes_needed,
            #     options, parentObjectType, meta,
            #     ref_prefix=ref_prefix,
            #     ref_suffix=ref_suffix)
            # if parentsAnyOf:
            #     anyOf += parentsAnyOf
        else:
            msg.warn(objectType + ' parentPath does not begin with /afw/_AdaptiveObjectType_/')

    return anyOf


# Conversion based on:
#   http://json-schema.org/draft/2020-12/json-schema-validation.html#name-default

# Convert objectType.
def _convert_objectType(mutable_objectTypes_needed,
        options, objectType, objectTypeObject, ref_prefix, ref_suffix,
        propertyTypesOnly=False, description=None, title=None):
    
    global _all_schemas

    # If objectType propertyTypes is not already in _all_schemas, make it.
    # Skip if objectType is _AdaptivePropertyTypes_ since it has to be handled
    # specially because of _meta_.
    objectTypePropertyTypes = objectType + '.propertyTypes'
    if (objectTypePropertyTypes not in mutable_objectTypes_needed and
        objectType != '_AdaptivePropertyTypes_'):

        # Will need self and everything added to mutable_objectTypes_needed.
        mutable_objectTypes_needed.append(objectTypePropertyTypes)
    
        # Start schema object for propertyTypes and initialize some variables..
        schema = {
            "type": "object"
        } 
        properties = {}
        required = []

        # Process all of the propertyTypes.
        for propertyTypeName, propertyType in objectTypeObject.get('propertyTypes',{}).items():
            property =  _convert_propertyType(mutable_objectTypes_needed,
                options, propertyType,
                'objectType ' + objectType + ' property ' + propertyTypeName,
                ref_prefix, ref_suffix)     
            properties[propertyTypeName] = property
            # If the propertyType is required, add it to the required list.
            if propertyType.get('required', False):
                required.append(propertyTypeName)
        schema['properties'] = properties
        
        # If objectType has required properties, list them in schema required.
        if len(required) > 0:
            schema['required'] = required

        # Keep objectType in _all_schemas
        _all_schemas[objectTypePropertyTypes] = schema

    # If propertyTypesOnly or objectType already being handled, return.
    if propertyTypesOnly or objectType in mutable_objectTypes_needed:
        return
 
    # Start schema object and initialize some variables..
    mutable_objectTypes_needed.append(objectType)
    schema = {
        "type": "object"
    } 

    anyOf = []

    # Special handling for _meta_ in _AdaptivePropertyTypes_ because it's not
    # defined in Adaptive Object type but implied by core code.
    if objectType == '_AdaptivePropertyTypes_':
        schema['properties'] = {
            "_meta_": {
                "properties": {
                    "parentPaths":{
                        "title": "Parent Paths",
                        "description": "This is a list of paths to the parent object type.",
                        "items": {
                            "type": "string",
                            "title": "Parent Path",
                            "description": "This is a path to a parent object type."
                        },
                        "type": "array"
                    }
                },
                "description":"This is the special Meta object that has deltas between this instance and its Adaptive Object Type.",
                "title":"Meta"
            }
        }
    else:
        # Have _process_parents return anyOf.
        anyOf = [
            {'$ref': ref_prefix + objectTypePropertyTypes + ref_suffix}
        ]

    if 'propertyTypes' in objectTypeObject:
        propertyTypes = objectTypeObject['propertyTypes']
        if '_meta_' in propertyTypes:
            parentAnyOf = _process_parents(mutable_objectTypes_needed,
                options, objectType, propertyTypes['_meta_'],
                ref_prefix=ref_prefix, ref_suffix=ref_suffix)
            if parentAnyOf:
                anyOf += parentAnyOf


    # If objectType has otherProperties, use it for additionalProperties in
    # schema.
    if 'otherProperties' in objectTypeObject:
        additionalProperties = _convert_propertyType(
            mutable_objectTypes_needed, options,
            objectTypeObject['otherProperties'],
            objectType + ' additionalProperties',
            ref_prefix, ref_suffix,
            description=objectTypeObject.get('description'),
            title=objectTypeObject.get('label'),)
        schema['additionalProperties'] = additionalProperties
        
    # If objectType does not have otherProperties, set unevaluatedProperties to
    # false in outermost schema only to prevent additional properties.
    else:
        schema['unevaluatedProperties'] = False

    # Add anyOf to schema.
    if len(anyOf) > 0:
        schema['anyOf'] = anyOf

    # If objectType has a description or one passed, use it as the description.
    if description:
        schema['description'] = description
    elif 'description' in objectTypeObject:
        schema['description'] = objectTypeObject['description']

    # If objectType has a title or one passed, use it as the title.
    if title:
        schema['title'] = title
    elif 'title' in objectTypeObject:
        schema['title'] = objectTypeObject['title']

    # Keep objectType in _all_schemas
    _all_schemas[objectType] = schema


# Generate generated/schemas/afw and generated/schema.json.
def generate(options):

    global _data_types
    global _all_schemas
    global _all_object_type_objects

    _all_schemas = {}
    _all_object_type_objects = {}


    msg.info('Generating JSON schema files for generated _AdaptiveObjectType_ objects')

    # Get afw package
    afw_package = package.get_afw_package(options)
    options['json_schema_id_base_uri'] = (
        'https://adaptiveframework.org/' +
        afw_package['afwPackageId'] + '-' + afw_package['version'] +
        '/schemas/afw/')
    options['json_compound_schema_document_uri'] = (
        'https://adaptiveframework.org/' +
        afw_package['afwPackageId'] + '-' + afw_package['version'] +
        '/schema')
    options['json_schema_uri'] = 'https://json-schema.org/draft/2020-12/schema'

    # Copy afwdev's resources to generated/afwdev/_resources
    if _data_types is None:
        list = resources.copy_resources(
            options, 'objects/_AdaptiveDataTypeGenerate_/')
        _data_types = {}
        for entry in list:
            object = nfc.json_loads(entry['resource'])
            _data_types[object['dataType']] = object

    # JSON schema files for all subdirs go in package's generated/schemas directory.
    schemasDir = options['afw_package_dir_path'] + 'generated/schemas/afw/'
    os.makedirs(schemasDir, exist_ok=True)

    ref_prefix = '#/$defs/'
    ref_suffix = ''

    # If not core afw package, get the core _AdaptiveObjectType_ objects and add
    # them to _all_object_type_objects.
    if not options['is_core_afw_package']:
        for objectTypeObject in resources.get_core_object_types(options):
            objectType = objectTypeObject['_meta_']['objectId']
            _all_object_type_objects[objectType] = objectTypeObject

    # Get all the generated _AdaptiveObjectType_ objects and add them to
    # _all_object_type_objects.
    srcDirPath = options['afw_package_dir_path'] + 'src/'  
    for objectTypeFile in glob.glob(
        srcDirPath + '**/generated/objects/_AdaptiveObjectType_/*.json',
        recursive=True):
        objectType = os.path.basename(objectTypeFile)[: -len('.json')]
        msg.info('Generating JSON schema for ' +  objectType)

        with nfc.open(objectTypeFile, 'r') as fd:
            objectTypeObject = nfc.json_load(fd)
            _all_object_type_objects[objectType] = objectTypeObject

    # Process all of the _AdaptiveObjectType_ objects.    
    for objectType, objectTypeObject in _all_object_type_objects.items():

        if not objectTypeObject.get('allowEntity', False):
            continue

        filename = objectType + '.json'
        msg.info('Writing ' + schemasDir + filename)
        with nfc.open(schemasDir + filename, 'w') as fd:
            definitions = {}

            mutable_objectTypes_needed = []
            _convert_objectType(mutable_objectTypes_needed,
                options, objectType, objectTypeObject, ref_prefix, ref_suffix,
                propertyTypesOnly=False)
            
            # Add definitions for all object types needed by this objectType.
            defs = {}
            for need in sorted(set(mutable_objectTypes_needed)):
                defs[need] = _all_schemas[need]

            schema = {
                # Removed> '$id': options['json_schema_id_base_uri'] + objectType,
                '$schema': options['json_schema_uri'],
                '$defs': defs,
                'anyOf': [
                    {'$ref': ref_prefix + objectType + ref_suffix}
                ]
            }

            nfc.json_dump(schema, fd, indent=4, sort_keys=True)         

    # Update vscode settings.json with schema.json if needed.
    _update_vscode_settings_json_schema(options)

    # Indicate success
    msg.success('Generate JSON schema files successful')
