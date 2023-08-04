from _afwdev.common import msg, nfc

object_types = {}

def prepare(objects):
    for ot in objects:
        objectType = ot.get('objectType')
        if objectType is None:
            objectType = ot['_meta_']['objectId']
            ot['objectType'] = objectType
        object_types[objectType] = ot
        if ot.get('_meta_') is not None:
            if ot['_meta_'].get('parentPaths') is not None:
                object_types[objectType]['_parents_'] = []
                for parent_path in ot['_meta_']['parentPaths']:
                    parts = parent_path.split('/')
                    id = parts[3]
                    object_types[objectType]['_parents_'].append(id)
        if ot.get('propertyTypes'):
            if ot['propertyTypes'].get('_meta_') is not None:
                if ot['propertyTypes']['_meta_'].get('parentPaths') is not None:
                    if object_types[objectType].get('_parents_') is None:
                        object_types[objectType]['_parents_'] = []
                    for parent_path in ot['propertyTypes']['_meta_']['parentPaths']:
                        parts = parent_path.split('/')
                        id = parts[4:]
                        object_types[objectType]['_parents_'].append(id)


def get(object_type_id, property_name):

    if object_type_id is None or object_type_id == '':
        return {}
    
    object_type = object_types.get(object_type_id)
    if object_type is None:
        return {}

    if object_type.get('propertyTypes') is not None:
        if object_type['propertyTypes'].get(property_name):
            return object_type['propertyTypes'][property_name]

    if object_type.get('otherProperties') is not None:
        return object_type['otherProperties']

    result = None
    if object_type.get('_parents_'):
        for parent in object_type['_parents_']:
            result = get(parent, property_name)
            if result is not None:
                break

    if result is None:
        result = {}

    return result
 