#! /usr/bin/env python3

import os
import stat
import json
import platform
from _afwdev.common import msg, nfc

# verbose = True will cause extra information in the errDir files
global verbose
verbose = True

def to_json(v):
    return json.JSONEncoder(indent='    ', ensure_ascii=False).encode(v)

def get_result(result):
    dataType = result.get('dataType')
    dataTypeParameter = result.get('dataTypeParameter')
    value = result.get('value')

    if dataType == 'bag':
        v_list = ''
        for v in value:
            if v_list != '':
                v_list += ','     
            v_list += '\n    '
            v_list += arg_to_json(dataTypeParameter, v)        
        return 'bag<' + dataTypeParameter + '>(' + v_list + '\n)'  

def get_args(arguments):
    args = ''
    for arg in arguments:
        if args != '':
            args += ','
        args += '\n    '
        dataType = arg.get('dataType')
        dataTypeParameter = arg.get('dataTypeParameter')
        value = arg.get('value')

        if dataType == 'bag':
            v_list = ''
            for v in value:
                if v_list != '':
                    v_list += ','
                v_list += '\n        '
                v_list += arg_to_json(dataTypeParameter, v)
            args += 'bag<' + dataTypeParameter + '>(' + v_list + '\n    )'
        else:
            args += arg_to_json(dataType, value)
    return args


def arg_to_json(dataType, value):
    
    if dataType == 'boolean':
        s = str(value).lower()

    elif dataType == 'integer':
        if '.' in value:
            i = value.find('.')
            if i > 0:
                s = value[0:i]
            else:
                s = value[i+1:]
        else:
            s = value

    elif dataType == 'double':
        if value[0:1] == '.':
            s = '0' + value
        elif value == 'INF':
            s = 'Infinity'
        elif value == '-INF':
            s = '-Infinity'
        else:
            s = value

    elif (dataType == 'function'):
        s = '"' + value + '"'
        return s

    elif (dataType != 'null') and value is None:
        s = '""'
        return s

    else:
        s = to_json(value)

    return dataType + '(' + s + ')'

def generate(options):

    msg.info('Generating data_type_tests..')

    # input and generated directories
    input_dir_path = options['objects_dir_path'] + '_AdaptiveDataTypeTest_/'
    generated_dir_path = options['generated_dir_path']  + 'tests/datatypes/'
    os.makedirs(generated_dir_path, exist_ok=True)
    
    cwd = os.getcwd()

    # run through all tests in each JSON file one at a time
    os.chdir(input_dir_path)

    for in_filename in sorted(os.listdir(os.getcwd())):

        if not in_filename.endswith('.json'):
            continue

        try:
            datatypeTests = nfc.json_load_from_file(in_filename)
        except FileNotFoundError as e:
            msg.info(os.getcwd() + ' ' + in_filename + ' not found exception\n')
            continue

        if datatypeTests.get('implemented', False) == False:
            continue

        filename = in_filename[:-5] + '.as'
        msg.info('Generating ' + filename)
        with nfc.open(generated_dir_path + filename, mode='w') as script:

            dataType = datatypeTests['dataType']
            testNumber = 0

            script.write('#!/usr/bin/env -S afw --syntax test_script\n')
            script.write('//?\n')
            script.write('//? testScript: datatypes/' + filename + '\n')
            script.write('//? customPurpose: Part of core dataType tests\n')
            if not datatypeTests.get('implemented', True):
                script.write('//? skip: true\n')
            description = datatypeTests.get('description', 'Tests for ' + dataType + ' dataType.')
            if len(description) < 63 and not '\n' in description:
                script.write('//? description: ' + description + '\n')
            else:
                script.write('//? description: ...\n')
                script.write(description)
                script.write('\n')
            script.write('//? sourceType: expression\n')

            # each 'test' element = 1 test_expression statement
            for test in datatypeTests['tests']:
                testNumber += 1
                testName = dataType + '-' + str(testNumber)
                script.write('//?\n')
                script.write('//? test: ' + testName + '\n')
                if not test.get('implemented', True):
                    script.write('//? skip: true\n')

                if test.get('description', 'Tests for ' + dataType + '.') is not None:
                    description = datatypeTests.get('description', 'Test ' + str(testNumber) + ' for ' + dataType + ' dataType.')
                    if len(description) < 63 and not '\n' in description:
                        script.write('//? description: ' + description + '\n')
                    else:
                        script.write('//? description: ...\n')
                        script.write(description)
                        script.write('\n')                
                
                script.write('//? expect: boolean(true)\n')                

                script.write('//? source: ...')
                if test.get('value') != None:                     
                    script.write('\n\n' 'eq<' + dataType + '>(\n' + 
                        '    ' + arg_to_json(dataType, test.get('value')) + ',\n' +
                        '    ' + arg_to_json(dataType, test.get('value')) + '\n)\n\n'
                    )
                    
        if platform.system() == 'Linux' or platform.system() == 'Darwin':
            msg.info('Marking ' + filename + ' executable.')
            st = os.stat(generated_dir_path + filename)
            os.chmod(generated_dir_path + filename, st.st_mode | stat.S_IEXEC)      

    os.chdir(cwd)

