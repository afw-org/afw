# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def add_properties(session, target, source):
    '''
    Add properties of one object to another

    Add the properties of one object to another replacing existing properties
    by the same name.

    Parameters:

        target (object): Target object or undefined if result will only
        contain the properties of source objects. The object must be mutable,
        if specified.

        source (object): Source object(s).

    Returns:
    object: The resulting object.
    '''

    request = session.Request()

    action = {
        "function": "add_properties",
        "target": target,
        "source": source
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def apply_object_options(session, object, options=None):
    '''
    Apply object options

    This will return an object with the specified object options applied.

    Parameters:

        object (object): Object used to produce result.

        options (object): Object options. See
        /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for more
        information.

    Returns:
    object: This is an object with object options applied.
    '''

    request = session.Request()

    action = {
        "function": "apply_object_options",
        "object": object
    }

    if options != None:
        action['options'] = options

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_object(session, values=None):
    '''
    Makes an array from values

    Takes any number of object values and returns an array of array.

    Parameters:

        values (array):

    Returns:
    array: 
    '''

    request = session.Request()

    action = {
        "function": "bag<object>"
    }

    if values != None:
        action['values'] = values

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_size_object(session, value):
    '''
    Returns the number of values in a bag

    This returns the integer number of values in array.

    Parameters:

        value (array):

    Returns:
    integer: 
    '''

    request = session.Request()

    action = {
        "function": "bag_size<object>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def clone_object(session, value):
    '''
    Clone object value

    Deep clone a object value.

    Parameters:

        value (object): The object value to clone.

    Returns:
    object: The cloned object value.
    '''

    request = session.Request()

    action = {
        "function": "clone<object>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eq_object(session, arg1, arg2):
    '''
    Checks for equal

    Determine if object arg1 is equal to the value of arg2 converted to the
    data type of arg1 then return the boolean result. Use 'eqx' ('===')
    instead if you want false to be returned if arg1 and arg2's data type
    don't match.

    Parameters:

        arg1 (object):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eq<object>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eqx_object(session, arg1, arg2):
    '''
    Checks for equal and type

    Determine if for object arg1 is equal to the value and data type of arg2
    then return the boolean result. Use 'eq' ('==') instead if you want arg2
    to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (object):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eqx<object>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ge_object(session, arg1, arg2):
    '''
    Checks for greater than or equal

    Checks for object arg1 is greater than or equal to object arg2 and return
    the boolean result.

    Parameters:

        arg1 (object):

        arg2 (object):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ge<object>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def gt_object(session, arg1, arg2):
    '''
    Checks for greater than

    Checks for object arg1 is greater than object arg2 and return the boolean
    result.

    Parameters:

        arg1 (object):

        arg2 (object):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "gt<object>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_object(session, value):
    '''
    Checks whether value is dataType object

    Checks whether value is dataType object and return the boolean result.

    Parameters:

        value (): Value to check

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "is<object>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def le_object(session, arg1, arg2):
    '''
    Checks for less than or equal

    Checks for object arg1 is less than or equal to object arg2 and return
    the boolean result.

    Parameters:

        arg1 (object):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "le<object>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def local_object_meta_set_ids(session, object, objectType, objectId, adaptorId=None):
    '''
    Set ids in a local mutable object

    This is used to set the ids in a local mutable object. The ids are used
    to construct a local path.

    Parameters:

        object (object): Object to set ids in.

        adaptorId (string): Adaptor id for object.

        objectType (string): Object type id for object.

        objectId (string): Object id for object.

    Returns:
    void: 
    '''

    request = session.Request()

    action = {
        "function": "local_object_meta_set_ids",
        "object": object,
        "objectType": objectType,
        "objectId": objectId
    }

    if adaptorId != None:
        action['adaptorId'] = adaptorId

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def lt_object(session, arg1, arg2):
    '''
    Checks for less than

    Checks for object arg1 is less that object arg2 and return the boolean
    result.

    Parameters:

        arg1 (object):

        arg2 (object):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "lt<object>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ne_object(session, arg1, arg2):
    '''
    Checks for not equal

    Determine if object arg1 is not equal to the value of arg2 converted to
    the data type of arg1 then return the boolean result. Use 'nex' ('!==')
    instead if you want true to be returned if arg1 and arg2's data type
    don't match.

    Parameters:

        arg1 (object):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ne<object>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def nex_object(session, arg1, arg2):
    '''
    Checks for not equal value or type

    Determine if for object arg1 is not equal to the value or data type of
    arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
    arg2 to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (object):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "nex<object>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def object(session, value):
    '''
    Convert to data type object

    Converts value to data type object returning object result.

    Parameters:

        value (): Value to convert

    Returns:
    object: Converted value
    '''

    request = session.Request()

    action = {
        "function": "object",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def property_delete(session, object, name):
    '''
    Delete a property

    Delete a property in an object.

    Parameters:

        object (object): This is the object to delete property from.

        name (string): This is a name of the property to delete.

    Returns:
    boolean: True if object had the property and it was deleted.
    '''

    request = session.Request()

    action = {
        "function": "property_delete",
        "object": object,
        "name": name
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def property_delete_by_reference(session, reference):
    '''
    Delete referenced property

    Delete a property from an object by reference.

    Parameters:

        reference (): This is a reference to the object property to delete.

    Returns:
    boolean: True if object had the property and it was deleted.
    '''

    request = session.Request()

    action = {
        "function": "property_delete_by_reference",
        "reference": reference
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def property_exists(session, object, name):
    '''
    Determine if a property exists in an object

    Return true if the named property exists in an object.

    Parameters:

        object (object): Object to get property from.

        name (string): Name of property to check.

    Returns:
    boolean: True if object has named property.
    '''

    request = session.Request()

    action = {
        "function": "property_exists",
        "object": object,
        "name": name
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def property_get(session, object, name, defaultValue=None):
    '''
    Get property value

    Return the value of a property of an object. If property is not
    available, return a default or null value.

    Parameters:

        object (object): Object to get property from.

        name (string): Name of property to get.

        defaultValue (): The default value of property if it does not exist
        in object. If not specified, null value is the default.

    Returns:
    None: Evaluated property value or default.
    '''

    request = session.Request()

    action = {
        "function": "property_get",
        "object": object,
        "name": name
    }

    if defaultValue != None:
        action['defaultValue'] = defaultValue

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def property_is_not_null(session, object, name):
    '''
    Determine if a property exists in an object and is not null

    Return true if the named property exists in an object and is not null.

    Parameters:

        object (object): Object to get property from.

        name (string): Name of property to check.

    Returns:
    boolean: True if object has named property that is not null.
    '''

    request = session.Request()

    action = {
        "function": "property_is_not_null",
        "object": object,
        "name": name
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_string_object(session, value):
    '''
    Converts value to string

    Converts object value to string. For array values, the to_string() value
    for each entry is returned separated with commas.

    Parameters:

        value (object): A object value.

    Returns:
    string: The string representation of the value.
    '''

    request = session.Request()

    action = {
        "function": "to_string<object>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

