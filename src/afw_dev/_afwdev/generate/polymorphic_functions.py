import copy
import os

##
# @file polymorphic_functions.py
# @ingroup afwdev_generate
#

from _afwdev.common import direct
from _afwdev.common import msg, nfc

def sort_use_id_cb(obj):
    return obj['_meta_']['objectId']

def generate(options):

    # Put each polymorphic function and their data type specific function inobject_dir_path.

    functions_dir_path = options['objects_dir_path'] + '_AdaptiveFunctionGenerate_/'
    os.makedirs(functions_dir_path, exist_ok=True)

    polymorphic_functions = direct.retrieve_objects_direct(options['polymorphic_functions_dir_path'])
    polymorphic_functions.sort(key=sort_use_id_cb)
    for polymorphic_function in polymorphic_functions:

        functionId = polymorphic_function['_meta_']['objectId']
        del polymorphic_function['_meta_']
        if functionId != polymorphic_function['functionId']:
            msg.error_exit('Polymorphic function ' +  functionId + ' objectId does not match functionId\n')
        if polymorphic_function.get('useExecuteFunction'):
            msg.error_exit('useExecuteFunction can not be specified in polymorphic function')
        if polymorphic_function['parameters'] is None or len(polymorphic_function['parameters']) < 1:
            msg.error_exit('Polymorphic function ' +  functionId + ' is missing parameters parameter 1\n')
        if polymorphic_function['parameters'][0].get('dataType') is not None and polymorphic_function['parameters'][0].get('dataType') != 'array':
            msg.error_exit('Polymorphic function ' +  functionId + ' parameter 1 can not have dataType\n')

        # Include polymorphic version
        msg.info('Including function ' + functionId + ' polymorphic version')
        if polymorphic_function.get('polymorphicExecuteFunctionEvaluatesFirstParameter') is None:
            polymorphic_function['useExecuteFunction'] = 'AFW_FUNCTION_EXECUTE_STANDARD_POLYMORPHIC_FUNCTION_HANDLING'
        else:
            polymorphic_function['useExecuteFunction'] = polymorphic_function['polymorphicExecuteFunction']
        with nfc.open(functions_dir_path + functionId + '.json', mode='w') as fd:
            fd.write(nfc.json_dumps(polymorphic_function, sort_keys=True, indent=4))

        # Include data type specific version
        for dataType in polymorphic_function.get('polymorphicDataTypes', []):

            # function object
            function = {}

            # brief
            if polymorphic_function.get('brief') is not None:
                function['brief'] = polymorphic_function.get('brief').replace('<dataType>', dataType)

            # category
            function['category'] = dataType

            # dataTypeMethod
            function['dataTypeMethod'] = True

            # dataTypeMethodNumber
            if polymorphic_function.get('dataTypeMethodNumber') is not None:
                function['dataTypeMethodNumber'] = polymorphic_function.get('dataTypeMethodNumber')

            # deprecated
            if polymorphic_function.get('deprecated') is not None:
                function['deprecated'] = polymorphic_function.get('deprecated')

            # description
            if polymorphic_function.get('description') is not None:
                function['description'] = polymorphic_function['description'].replace('<dataType>', dataType)

            # details
            if polymorphic_function.get('details') is not None:
                function['details'] = polymorphic_function.get('details')

            # functionId
            function['functionId'] = polymorphic_function['functionId'] + '<' + dataType + '>'

            # functionLabel
            function['functionLabel'] = polymorphic_function['functionId'] + '_' + dataType

            # functionSignature is automatically generated

            # pure
            if polymorphic_function.get('pure') is not None:
                function['pure'] = polymorphic_function.get('pure')

            # op
            if polymorphic_function.get('op') is not None:
                function['op'] = polymorphic_function.get('op')

            # errorsThrown
            if polymorphic_function.get('errorsThrown') is not None:
                function['errorsThrown'] = polymorphic_function.get('errorsThrown')

            # parameters
            function['parameters'] = copy.deepcopy(polymorphic_function['parameters'])
            for parameter in function['parameters']:
                if parameter.get('brief') is not None:
                    parameter['brief'] = parameter.get('brief').replace('<dataType>', dataType)
                if parameter.get('description') is not None:
                    parameter['description'] = parameter.get('description').replace('<dataType>', dataType)
                if parameter.get('polymorphicDataType', False):
                    del parameter['polymorphicDataType']
                    parameter['dataType'] = dataType
                if parameter.get('polymorphicDataTypeParameter', False):
                    del parameter['polymorphicDataTypeParameter']
                    parameter['dataTypeParameter'] = dataType

            # returns
            returns = copy.deepcopy(polymorphic_function['returns'])
            function['returns'] = returns
            if returns.get('brief') is not None:
                returns['brief'] = returns.get('brief').replace('<dataType>', dataType)
            if returns.get('description') is not None:
                returns['description'] = returns.get('description').replace('<dataType>', dataType)
            if returns.get('polymorphicDataType', False):
                del returns['polymorphicDataType']
                returns['dataType'] = dataType
            if returns.get('polymorphicDataTypeParameter', False):
                del returns['polymorphicDataTypeParameter']
                returns['dataTypeParameter'] = dataType

            if polymorphic_function.get('polymorphicExecuteFunction'):
                function['useExecuteFunction'] = polymorphic_function.get('polymorphicExecuteFunction')

            #@todo Should this be a 'merge" form polymorphicOverrides to support parameters and returns??
            if polymorphic_function.get('polymorphicOverrides') is not None and \
                polymorphic_function['polymorphicOverrides'].get(dataType) is not None:
                for key, value in polymorphic_function['polymorphicOverrides'][dataType].items():
                    function[key] = value

            msg.info('Including function ' + functionId + ' ' + dataType + ' version')
            with nfc.open(functions_dir_path + functionId + '_' + dataType + '.json', mode='w') as fd:
                fd.write(nfc.json_dumps(function, sort_keys=True, indent=4))

