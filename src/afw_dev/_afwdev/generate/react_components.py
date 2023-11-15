#! /usr/bin/env python3

##
# @file react_components.py
# @ingroup afwdev_generate
#

from _afwdev.common import direct
import os
from _afwdev.common import msg, nfc
from _afwdev.generate import c


def js_value(objectTypes, dataType, dataTypeParameter, possibleValues):

    if dataType == 'string':
        if possibleValues != None:
            return nfc.json_dumps(possibleValues[0], sort_keys=True)

        return '"string"'

    elif dataType == 'integer':
        if possibleValues != None:
            return str(possibleValues[0])

        return '1'

    elif dataType == 'boolean':
        return 'true'

    elif dataType == 'array':
        if possibleValues != None:
            return nfc.json_dumps(possibleValues, sort_keys=True)

        if dataTypeParameter == None:
            # generic list, so just choose a string
            return '[ "abc" ]'

        if dataTypeParameter == 'string':
            return '[ "abc", "xyz", "some string" ]'
        elif dataTypeParameter == 'integer':
            return '[ 0, 1, 2 ]}'
        elif dataTypeParameter == 'boolean':
            return '[ true, false, true ]'
        elif dataTypeParameter.startswith('object '):
            #@todo
            obj = {}
            objectType = objectTypes.get(dataTypeParameter.split()[1])
            if not objectType or objectType.get('propertyTypes') == None:
                return '[{}]'                

            for propertyName in sorted(objectType.get('propertyTypes')):
                propertyType = objectType.get('propertyTypes').get(propertyName)
               
                if propertyType.get('dataType') == 'string': 
                    obj[propertyName] = 'string'
                elif propertyType.get('dataType') == 'integer':
                    obj[propertyName] = 1
                elif propertyType.get('dataType') == 'boolean':
                    obj[propertyName] = True

            return '[' + nfc.json_dumps(obj, sort_keys=True) +  ']'
        else:
            return None

    elif dataType == 'object':
        objectType = objectTypes.get(dataTypeParameter)
        if objectType and objectType.get('propertyTypes'):
            obj = '{\n'
            for propertyName in sorted(objectType.get('propertyTypes')):
                propertyType = objectType.get('propertyTypes').get(propertyName)
                if propertyType.get('possibleValues') != None:
                    value = propertyType.get('possibleValues')[0]
                elif propertyType.get('dataType') == 'string':
                    value = "string"
                elif propertyType.get('dataType') == 'integer':
                    value = 1
                elif propertyType.get('dataType') == 'boolean':
                    value = True
                elif propertyType.get('dataType') == 'array':
                    value = []
                    if propertyType.get('dataTypeParameter') == 'string':
                        value.append('abc')
                    elif propertyType.get('dataTypeParameter') == 'integer':
                        value.append(1)
                else:
                    value = "Unknown"

                obj += '        ' + propertyName + ': ' + nfc.json_dumps(value, sort_keys=True) + ',\n'
            obj += '    }'

            return obj

        elif objectType and objectType.get('otherProperties'):
            obj = '{\n'
            otherProperties = objectType.get('otherProperties')
            if otherProperties.get('dataType') == 'string':
                value = "string"
            elif otherProperties.get('dataType') == 'integer':
                value = 1
            elif otherProperties.get('dataType') == 'boolean':
                value = True
            elif otherProperties.get('dataType') == 'array':
                value = []
                if otherProperties.get('dataTypeParameter') == 'string':
                    value.append('abc')
                elif otherProperties.get('dataTypeParameter') == 'integer':
                    value.append(1)
            else:
                value = "Unknown"

            obj += '                ' + 'propertyName' + ': ' + nfc.json_dumps(value, sort_keys=True) + ',\n'
            obj += '            }'

            return obj
        else:
            return None

    else:
        return '"unknown"'

#
# prop_value()
#
# Generate a sample value for a React prop, useful for testing.
def prop_value(objectTypes, dataType, dataTypeParameter, possibleValues):

    if dataType == 'string':
        if possibleValues != None:
            return nfc.json_dumps(possibleValues[0], sort_keys=True)

        return '"string"'

    elif dataType == 'integer':
        if possibleValues != None:
            return '{' + str(possibleValues[0]) + '}'

        return '{1}'

    elif dataType == 'boolean':
        return '{true}'

    elif dataType == 'array':
        if possibleValues != None:
            return '{' + nfc.json_dumps(possibleValues, sort_keys=True) + '}'

        if dataTypeParameter == None:
            # generic list, so just choose a string
            return '{[ "abc" ]}'

        if dataTypeParameter == 'string':
            return '{[ "abc", "xyz", "some string" ]}'
        elif dataTypeParameter == 'integer':
            return '{[ 0, 1, 2 ]}'
        elif dataTypeParameter == 'boolean':
            return '{[ true, false, true ]}'
        elif dataTypeParameter.startswith('object '):
            #@todo
            obj = {}
            objectType = objectTypes.get(dataTypeParameter.split()[1])
            if not objectType or objectType.get('propertyTypes') == None:
                return '{[{}]}'                

            for propertyName in sorted(objectType.get('propertyTypes')):
                propertyType = objectType.get('propertyTypes').get(propertyName)
               
                if propertyType.get('dataType') == 'string': 
                    obj[propertyName] = 'string'
                elif propertyType.get('dataType') == 'integer':
                    obj[propertyName] = 1
                elif propertyType.get('dataType') == 'boolean':
                    obj[propertyName] = True

            return '{[' + nfc.json_dumps(obj, sort_keys=True) +  ']}'
        else:
            return None

    elif dataType == 'object':
        objectType = objectTypes.get(dataTypeParameter)
        if objectType and objectType.get('propertyTypes'):
            obj = '{{\n'
            for propertyName in sorted(objectType.get('propertyTypes')):
                propertyType = objectType.get('propertyTypes').get(propertyName)
                if propertyType.get('possibleValues') != None:
                    value = propertyType.get('possibleValues')[0]
                elif propertyType.get('dataType') == 'string':
                    value = "string"
                elif propertyType.get('dataType') == 'integer':
                    value = 1
                elif propertyType.get('dataType') == 'boolean':
                    value = True
                elif propertyType.get('dataType') == 'array':
                    value = []
                    if propertyType.get('dataTypeParameter') == 'string':
                        value.append('abc')
                    elif propertyType.get('dataTypeParameter') == 'integer':
                        value.append(1)
                else:
                    value = "Unknown"

                obj += '            ' + propertyName + ': ' + nfc.json_dumps(value, sort_keys=True) + ',\n'
            obj += '        }}'

            return obj

        elif objectType and objectType.get('otherProperties'):
            obj = '{{\n'
            otherProperties = objectType.get('otherProperties')
            if otherProperties.get('dataType') == 'string':
                value = "string"
            elif otherProperties.get('dataType') == 'integer':
                value = 1
            elif otherProperties.get('dataType') == 'boolean':
                value = True
            elif otherProperties.get('dataType') == 'array':
                value = []
                if otherProperties.get('dataTypeParameter') == 'string':
                    value.append('abc')
                elif otherProperties.get('dataTypeParameter') == 'integer':
                    value.append(1)
            else:
                value = "Unknown"

            obj += '                    ' + 'propertyName' + ': ' + nfc.json_dumps(value, sort_keys=True) + ',\n'
            obj += '                }}'

            return obj
        else:
            return None

    else:
        return '"unknown"'

def typescript_type(objectTypes, dataType, dataTypeParameter, possibleValues):

    #if possibleValues != None:
        

    if dataType == 'string':
        return 'string'

    elif dataType == 'integer':
        return 'number'

    elif dataType == 'boolean':
        return 'boolean'

    elif dataType == 'dateTime':
        return 'string'

    elif dataType == 'anyURI':
        return 'string'

    elif dataType == 'object':
        if dataTypeParameter == None:
            return 'any'
        elif dataTypeParameter == '_AdaptiveLayoutComponentType_':
            return 'any'
        elif objectTypes.get(dataTypeParameter) != None:
            return 'any'
        else:
            return 'any'

    elif dataType == 'array':
        if dataTypeParameter == None:
            return 'any[]'
        if dataTypeParameter == 'string':
            return 'string[]'
        elif dataTypeParameter == 'integer':
            return 'number[]'
        elif dataTypeParameter == 'boolean':
            return 'boolean[]'
        elif dataTypeParameter.startswith('object '):
            objectType = dataTypeParameter.split()[1]

            if objectType and objectTypes.get(objectType):
                return 'any[]'
            elif objectType == '_AdaptiveLayoutComponentType_':
                return 'any[]'

            return 'any[]'
        else:
            return 'any[]'

    else:
        msg.info('** Unhandled property! ' + str(dataType))
        return 'any'

#
# prop_type
#
# Generates a React propType definition
def prop_type(objectTypes, dataType, dataTypeParameter, possibleValues):

    if possibleValues != None:
        return 'PropTypes.oneOf(' + nfc.json_dumps(possibleValues, sort_keys=True) + ')'

    if dataType == 'string':
        return 'PropTypes.string'

    elif dataType == 'integer':
        return 'PropTypes.number'

    elif dataType == 'boolean':
        return 'PropTypes.bool'

    elif dataType == 'dateTime':
        return 'PropTypes.string'

    elif dataType == 'anyURI':
        return 'PropTypes.string'

    elif dataType == 'object':
        if dataTypeParameter == None:
            return 'PropTypes.any'
        elif dataTypeParameter == '_AdaptiveLayoutComponentType_':
            return 'PropTypes.oneOfType([ PropTypes.element, PropTypes.object ])'
        elif objectTypes.get(dataTypeParameter) != None:
            return 'PropTypes.shape(' + dataTypeParameter.split('_AdaptiveLayoutComponentType_')[1] + ')'
        else:
            return 'PropTypes.object'

    elif dataType == 'array':
        if dataTypeParameter == None:
            return 'PropTypes.array'
        if dataTypeParameter == 'string':
            return 'PropTypes.arrayOf(PropTypes.string)'
        elif dataTypeParameter == 'integer':
            return 'PropTypes.arrayOf(PropTypes.number)'
        elif dataTypeParameter == 'boolean':
            return 'PropTypes.arrayOf(PropTypes.bool)'
        elif dataTypeParameter.startswith('object '):
            objectType = dataTypeParameter.split()[1]

            if objectType and objectTypes.get(objectType):
                return 'PropTypes.arrayOf(PropTypes.shape(' + objectType.split('_AdaptiveLayoutComponentType_')[1] + '))'
            elif objectType == '_AdaptiveLayoutComponentType_':
                return 'PropTypes.arrayOf(PropTypes.oneOfType([ PropTypes.element, PropTypes.object ]))'

            return 'PropTypes.arrayOf(PropTypes.object)'
        else:
            return 'PropTypes.array'

    else:
        msg.info('** Unhandled property! ' + str(dataType))
        return 'PropTypes.any'

def js_type(objectTypes, dataType, dataTypeParameter, possibleValues):

    #if possibleValues != None:
    #    return 'PropTypes.oneOf(' + nfc.json_dumps(possibleValues, sort_keys=True) + ')'

    if dataType == 'string':
        return 'string'

    elif dataType == 'integer':
        return 'number'

    elif dataType == 'boolean':
        return 'boolean'

    elif dataType == 'dateTime':
        return 'string'

    elif dataType == 'anyURI':
        return 'string'

    elif dataType == 'object':           
        return 'object'

    elif dataType == 'array':
        return 'array'

    else:
        msg.info('** Unhandled property! ' + str(dataType))
        return ''


def generate_readme(fd, objectTypes, objectType, componentType, implementationId, layoutComponentType):

    description = layoutComponentType.get('description')
    brief = layoutComponentType.get('brief')    

    fd.write('# ' + implementationId + '\n\n')
    if brief != None:                
        fd.write(brief + '\n\n')

    fd.write('## Description\n')
    if description != None:        
        fd.write(description + '\n\n')

    fd.write('## Props\n')
    fd.write('| Name | Type | Required | Default | Description |\n')
    fd.write('|:----------|:----------|:----|:------------|:------------|\n')

    for propertyName in sorted(objectType.get('propertyTypes')):
        propertyType = objectType.get('propertyTypes').get(propertyName)

        if propertyName == '_meta_':
            continue

        required = propertyType.get('required', False)        
        dataType = propertyType.get('dataType')
        dataTypeParameter = propertyType.get('dataTypeParameter')
        possibleValues = propertyType.get('possibleValues')
        defaultValue = propertyType.get('defaultValue')
        propDescription = propertyType.get('description')
     
        propType = js_type(objectTypes, dataType, dataTypeParameter, possibleValues)

        fd.write('|' + propertyName + '|' + str(propType) + '|' + nfc.json_dumps(required) + '|' + nfc.json_dumps(defaultValue) + '|' + str(propDescription) + '|\n')

    fd.write('\n')
    fd.write('## Usage\n')
    fd.write('```js\n')    
    fd.write('import {' + implementationId + '} from "@afw/react";\n')
    fd.write('\n')
    fd.write('const App = (props) => {\n')
    fd.write('    return <' + implementationId + ' />\n')
    fd.write('}\n')
    fd.write('```\n')
    fd.write('\n')

    fd.write('## Files\n')
    fd.write('Each file in this directory has been generated for specific purposes.\n')
    fd.write(' * `' + implementationId + '.js`\n')
    fd.write('\n')
    fd.write('   This is the ReactJS component directly imported from this package.\n')
    fd.write('\n')
    fd.write('   It will use registered implementations, or call a fallback for default.\n')
    fd.write(' * `' + implementationId + '.propTypes.js`\n')
    fd.write('\n')
    fd.write('   Generated propTypes, which declare the parameters using React propType definitions.\n')
    fd.write('\n')
    fd.write(' * `' + implementationId + '.fallback.js`\n')
    fd.write('\n')
    fd.write('   A default fallback implementation, if none are supplied by the client.\n')
    fd.write('\n')
    fd.write(' * `' + implementationId + '.test.js`\n')
    fd.write('\n')
    fd.write('   Test cases used by @testing-library/jest.\n')
    fd.write('\n')
    fd.write(' * `' + implementationId + '.stories.js`\n')
    fd.write('\n')
    fd.write('   Storybook stories for showcasing the component.\n')
    fd.write('\n')
    fd.write(' * `' + implementationId + '.d.ts`\n')
    fd.write('\n')
    fd.write('   Typescript declaration file for declaring the type information for this component.\n')
    fd.write('\n')
    fd.write(' * `index.js`\n')
    fd.write('\n')
    fd.write('   An index that exports all relevant Component and auxiliary functions or objects for easy import.\n')
    fd.write('\n')
    fd.write(' * `index.d.ts`\n')
    fd.write('\n')
    fd.write('   A typescript declaration index that exports all relevant type information for third-party tools to use.\n')
    fd.write('\n')
    fd.write(' * `README.md`\n')
    fd.write('\n')
    fd.write('   This file that you are reading.\n')
    fd.write('\n')

    fd.write('## Reference\n')
    fd.write('This README was generated from:\n')
    fd.write('  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/' + implementationId + '.json)\n')
    fd.write('  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_' + implementationId + '.json)\n')
    fd.write('\n')      

def generate_stories(fd, objectTypes, objectType, componentType, implementationId, layoutComponentType):

    category = layoutComponentType.get('category')
      
    fd.write('import {' + implementationId + '} from "./' + implementationId + '";\n')
    fd.write('\n')

    fd.write('export default {\n')
    fd.write('    title: "core / ' + category + ' / ' + implementationId + '",\n')
    fd.write('    component: ' + implementationId + ',\n')

    # generate storybook argTypes
    fd.write('    argTypes: {\n')
    for propertyName in sorted(objectType.get('propertyTypes')):
        if propertyName == '_meta_':
            continue

        propertyType = objectType.get('propertyTypes').get(propertyName)
        brief = propertyType.get('brief')
        description = propertyType.get('description')
        required = propertyType.get('required')
        dataType = propertyType.get('dataType')
        dataTypeParameter = propertyType.get('dataTypeParameter')
        possibleValues = propertyType.get('possibleValues')
        defaultValue = propertyType.get('defaultValue')

        value = prop_value(objectTypes, dataType, dataTypeParameter, possibleValues)
        if value:
            fd.write('        ' + propertyName + ': {\n')
            fd.write('            name: "' + propertyName + '",\n')
            fd.write('            type: {\n')
            fd.write('                name: "' + js_type(objectTypes, dataType, dataTypeParameter, possibleValues) + '",\n')
            fd.write('                required: ' + ('true' if required else 'false') + ',\n')
            fd.write('            },\n')
            fd.write('            description: ' + nfc.json_dumps(description) + ',\n')

            if defaultValue != None:
                fd.write('            defaultValue: ' + nfc.json_dumps(defaultValue, sort_keys=True) + ',\n')
                                
            fd.write('        },\n')

    fd.write('    }\n')

    fd.write('};\n')
    fd.write('\n')

    fd.write('\n')
    fd.write('const Template = (args) => <' + implementationId + ' {...args} />;\n')
    fd.write('\n')
    
    fd.write('export const Default = Template.bind({});\n')
    fd.write('\n')

    # output test that uses all props at once
    fd.write('export const AllProps = Template.bind({});\n')    
    fd.write(implementationId + '.args = {\n')        

    for propertyName in sorted(objectType.get('propertyTypes')):
        if propertyName == '_meta_':
            continue

        propertyType = objectType.get('propertyTypes').get(propertyName)
        required = propertyType.get('required')
        dataType = propertyType.get('dataType')
        dataTypeParameter = propertyType.get('dataTypeParameter')
        possibleValues = propertyType.get('possibleValues')

        value = js_value(objectTypes, dataType, dataTypeParameter, possibleValues)
        if value:
            fd.write('    ' + propertyName + ': ' + value + ',\n')

    fd.write('};\n')    


def generate_jest_tests(fd, objectTypes, objectType, componentType, implementationId):

    fd.write('import {render} from "@afw/test";\n')    
    fd.write('\n\n')
    fd.write('const Test = (wrapper, ' + implementationId + ') => {\n\n')
    fd.write('    describe("' + implementationId + ' tests", () => {\n\n')

    # output test that uses only required props
    fd.write('        test("' + implementationId + ' renders with minimal props", async () => {\n\n')
    fd.write('            render(\n')
    fd.write('                <' + implementationId + ' id="' + implementationId + '" />, \n')
    fd.write('                { wrapper }\n')
    fd.write('            );\n\n')
    fd.write('        });\n')
    fd.write('    });\n')
    fd.write('};\n\n')        
    fd.write('/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */\n')
    fd.write('test("Loads ' + implementationId + ' Test", async () => expect(true).toBe(true));\n\n')
    fd.write('export default Test;\n')    

def generate_type_shape(fd, propertyName, objectTypes, objectType):

    fd.write('\n')


def generate_prop_shape(fd, propertyName, objectTypes, objectType):
    if objectType == '_AdaptiveLayoutComponentType_':
        return
    elif objectType == None:
        return
    elif objectTypes.get(objectType) == None:
        return
    else:
        propertyTypes = objectTypes.get(objectType).get('propertyTypes')
        if propertyTypes == None:
            if objectTypes.get(objectType).get('otherProperties') != None:
                fd.write('/* Generated objectType propType definition for property ' + propertyName + ' */\n')
                fd.write('const ' + objectType.split('_AdaptiveLayoutComponentType_')[1] + ' = {\n')

                fd.write('};\n')
                fd.write('\n')
            return

        # check for forward declarations
        forward_decl = False
        for propertyName in sorted(propertyTypes):
            if propertyTypes.get(propertyName).get('dataTypeParameter') == ('object ' + objectType):
                forward_decl = True

        fd.write('/* Generated objectType propType definition for property ' + propertyName + ' */\n')
        if forward_decl:
            fd.write('let ' + objectType.split('_AdaptiveLayoutComponentType_')[1] + ';\n')
            fd.write(objectType.split('_AdaptiveLayoutComponentType_')[1] + ' = {\n')
        else:
            fd.write('const ' + objectType.split('_AdaptiveLayoutComponentType_')[1] + ' = {\n')

        for propertyName in sorted(propertyTypes):
            propertyType = propertyTypes.get(propertyName)
            dataType = propertyType.get('dataType')
            dataTypeParameter = propertyType.get('dataTypeParameter')
            required = propertyType.get('required')
            possibleValues = propertyType.get('possibleValues')
            brief = propertyType.get('brief')
            description = propertyType.get('description')

            name = ('    ' + propertyName + ':').ljust(40, ' ')            
            comment = ('(' + dataType + ((', ' + dataTypeParameter + ')') if dataTypeParameter else ')')) if dataType else '(None)'

            propType = prop_type(objectTypes, dataType, dataTypeParameter, possibleValues)
            if required == True:
                propType = propType + '.isRequired'

            fd.write('    /**\n')
            fd.write('     * Property: ' + propertyName + '\n')
            fd.write('     * Data Type: ' + comment + '\n')
            fd.write('     * \n')

            if brief != None:
                # don't let lines go beyond 80 chars
                c.write_wrapped(fd, 78, '     * ', brief, '', '', True)

            if description != None:
                # don't let lines go beyond 80 chars
                c.write_wrapped(fd, 78, '     * ', description, '', '', True)

            fd.write('     */\n')
            fd.write(name + propType + ',' + '\n')
            
        fd.write('};\n')
        fd.write('\n')

def generate_typescript_types(fd, objectTypes, objectType, componentType, implementationId, layoutComponentType):

    shapes = {}

    # before writing out the propTypes, write out any object dependencies
    for propertyName in sorted(objectType.get('propertyTypes')):
        propertyType = objectType['propertyTypes'][propertyName]

        if propertyType.get('dataType') == 'object':
            dataTypeParameter = propertyType.get('dataTypeParameter')
            if dataTypeParameter == '_AdaptiveLayoutComponentType_':
                continue
            elif dataTypeParameter == None:
                msg.info('  * Skipping unknown objectType for property ' + propertyName)
                continue

            # make sure we haven't already handled this one
            if not shapes.get(dataTypeParameter):
                generate_type_shape(fd, propertyName, objectTypes, dataTypeParameter)
                shapes[dataTypeParameter] = True

        elif propertyType.get('dataType') == 'array':
            dataTypeParameter = propertyType.get('dataTypeParameter')
            if dataTypeParameter != None:
                if dataTypeParameter.split()[0] == 'object':
                    if len(dataTypeParameter.split()) > 1:
                        dataTypeObjectType = dataTypeParameter.split()[1]

                        if not shapes.get(dataTypeObjectType):
                            generate_type_shape(fd, propertyName, objectTypes, dataTypeObjectType)
                            shapes[dataTypeObjectType] = True

    fd.write('/**\n')
    fd.write(' * Typescript interface definition for propTypes\n') 
    fd.write(' */\n')

    PropsInterface = 'I' + implementationId + 'Props' 
    fd.write('export interface ' + PropsInterface + ' {\n')

    defaultValues = {} 
    for propertyName in sorted(objectType['propertyTypes']):
        if propertyName == "_meta_":
            continue

        propertyType = objectType['propertyTypes'][propertyName]

        dataType = propertyType.get('dataType')
        dataTypeParameter = propertyType.get('dataTypeParameter')
        description = propertyType.get('description')
        required = propertyType.get('required')
        defaultValue = propertyType.get('defaultValue')
        possibleValues = propertyType.get('possibleValues')
        brief = propertyType.get('brief')
        description = propertyType.get('description')

        if defaultValue != None:
            defaultValues[propertyName] = defaultValue

        name = propertyName
        if required != True:
            name = name + '?'

        name = ('    ' + name + ':').ljust(40, ' ')
        comment = ('(' + dataType + ((', ' + dataTypeParameter + ')') if dataTypeParameter else ')')) if dataType else '(None)'

        tsType = typescript_type(objectTypes, dataType, dataTypeParameter, possibleValues)
        

        fd.write('    /**\n')
        fd.write('     * ' + propertyName + '\n')
        fd.write('     * Data Type: ' + comment + '\n')
        fd.write('     * \n')

        if brief != None:
            # don't let lines go beyond 80 chars
            c.write_wrapped(fd, 78, '     * ', brief, '', '', True)
            fd.write('     * \n')

        if description != None:
            # don't let lines go beyond 80 chars
            c.write_wrapped(fd, 78, '     * ', description, '', '', True)

        fd.write('     */\n')
        fd.write(name + tsType + ';' + '\n')
      
    fd.write('}\n')
    fd.write('\n')    
    

def generate_prop_types(fd, objectTypes, objectType, componentType, implementationId):

    shapes = {}

    fd.write('import PropTypes from "prop-types";\n')
    fd.write('\n')

    # before writing out the propTypes, write out any object dependencies
    for propertyName in sorted(objectType.get('propertyTypes')):
        propertyType = objectType['propertyTypes'][propertyName]

        if propertyType.get('dataType') == 'object':
            dataTypeParameter = propertyType.get('dataTypeParameter')
            if dataTypeParameter == '_AdaptiveLayoutComponentType_':
                continue
            elif dataTypeParameter == None:
                msg.info('  * Skipping unknown objectType for property ' + propertyName)
                continue

            # make sure we haven't already handled this one
            if not shapes.get(dataTypeParameter):
                generate_prop_shape(fd, propertyName, objectTypes, dataTypeParameter)
                shapes[dataTypeParameter] = True

        elif propertyType.get('dataType') == 'array':
            dataTypeParameter = propertyType.get('dataTypeParameter')
            if dataTypeParameter != None:
                if dataTypeParameter.split()[0] == 'object':
                    if len(dataTypeParameter.split()) > 1:
                        dataTypeObjectType = dataTypeParameter.split()[1]

                        if not shapes.get(dataTypeObjectType):
                            generate_prop_shape(fd, propertyName, objectTypes, dataTypeObjectType)
                            shapes[dataTypeObjectType] = True

    fd.write('/**\n')
    fd.write(' * PropTypes generated by ' + objectType.get('objectType') + '\n')
    fd.write(' */\n')
    fd.write('export const propTypes = {\n')
  
    defaultValues = {} 
    for propertyName in sorted(objectType['propertyTypes']):
        if propertyName == "_meta_":
            continue

        propertyType = objectType['propertyTypes'][propertyName]

        dataType = propertyType.get('dataType')
        dataTypeParameter = propertyType.get('dataTypeParameter')
        description = propertyType.get('description')
        required = propertyType.get('required')
        defaultValue = propertyType.get('defaultValue')
        possibleValues = propertyType.get('possibleValues')
        brief = propertyType.get('brief')
        description = propertyType.get('description')

        if defaultValue != None:
            defaultValues[propertyName] = defaultValue

        name = ('    ' + propertyName + ':').ljust(40, ' ')
        comment = ('(' + dataType + ((', ' + dataTypeParameter + ')') if dataTypeParameter else ')')) if dataType else '(None)'

        propType = prop_type(objectTypes, dataType, dataTypeParameter, possibleValues)
        if required == True:
            propType = propType + '.isRequired'

        fd.write('    /**\n')
        fd.write('     * ' + propertyName + '\n')
        fd.write('     * Data Type: ' + comment + '\n')
        fd.write('     * \n')

        if brief != None:
            # don't let lines go beyond 80 chars
            c.write_wrapped(fd, 78, '     * ', brief, '', '', True)

        if description != None:
            # don't let lines go beyond 80 chars
            c.write_wrapped(fd, 78, '     * ', description, '', '', True)

        fd.write('     */\n')
        fd.write(name + propType + ',' + '\n')

    fd.write('};\n\n')

    # write out default values
    fd.write('export const ' + 'defaultProps = {\n')
    for propertyName in sorted(defaultValues):
        defaultValue = defaultValues[propertyName]
        name = ('    ' + propertyName + ':').ljust(40, ' ')

        fd.write(name + nfc.json_dumps(defaultValue, sort_keys=True) + ',' + '\n')
    fd.write('};\n')
    fd.write('\n')
    fd.write('export default propTypes;\n')

def generate_typescript(fd, objectTypes, objectType, componentType, implementationId, layoutComponentType):    

    category = layoutComponentType.get('category')
    description = layoutComponentType.get('description')
    brief = layoutComponentType.get('brief')
    propsInterface = 'I' + implementationId + 'Props'

    # write out the license
    c.write_copyright(fd, 'React Component definition for ' + implementationId)   

    fd.write('\n')
    fd.write('import React from "react";\n') 
    fd.write('import {propTypes, defaultProps} from "./' + implementationId + '.propTypes";\n')
    fd.write('import {' + propsInterface + '} from "./' + implementationId + '.types";\n')
    fd.write('import fallback from "./' + implementationId + '.fallback";\n')
    fd.write('\n')
    fd.write('import {AdaptiveComponent} from "@afw/react";\n')
    fd.write('import {ctx} from "@afw/react";\n')
    fd.write('\n')

    #generate_typescript_types(fd, objectTypes, objectType, componentType, implementationId, layoutComponentType)

    fd.write('\n')
    fd.write('/**\n')
    fd.write(' * Implementation Id : ' + implementationId + '\n')
    fd.write(' * Category          : ' + category + '\n')
    fd.write(' * \n')

    if brief != None:
        # don't let lines go beyond 80 chars
        c.write_wrapped(fd, 78, ' * ', brief, '', '', True)
        fd.write(' * \n')

    if description != None:
        # don't let lines go beyond 80 chars
        c.write_wrapped(fd, 78, ' * ', description, '', '', True)
        fd.write(' * \n')

    fd.write(' */\n')
    fd.write('export const ' + implementationId + ' : React.FunctionComponent<' + propsInterface + '> = (props) => {\n')
    fd.write('\n')
    fd.write('    return (\n')
    fd.write('        <AdaptiveComponent \n')
    fd.write('            {...props} \n')
    fd.write('            data-component-type={ctx(props, "' + implementationId + '")}\n')
    fd.write('            fallback={fallback}\n')
    fd.write('            layoutComponent={{\n')
    fd.write('                componentType: "' + implementationId + '",\n')
    fd.write('                parameters: props\n')
    fd.write('            }}\n')
    fd.write('        />\n')    
    fd.write('    );\n')
    fd.write('};\n')
    fd.write('\n')
    fd.write('\n')

    fd.write(implementationId + '.propTypes = propTypes;\n')
    fd.write(implementationId + '.defaultProps = defaultProps;\n')
    fd.write(implementationId + '.displayName = "' + implementationId + '";\n')

    fd.write('\n')
    fd.write('export default React.memo(' + implementationId + ');\n')

def generate_javascript(fd, objectTypes, objectType, componentType, implementationId, layoutComponentType):  

    category = layoutComponentType.get('category')
    description = layoutComponentType.get('description')
    brief = layoutComponentType.get('brief')  

    # write out the license
    c.write_copyright(fd, 'React Component definition for ' + implementationId)

    fd.write('\n')
    fd.write('import {memo} from "react";\n')
    fd.write('import {propTypes, defaultProps} from "./' + implementationId + '.propTypes";\n')
    fd.write('import fallback from "./' + implementationId + '.fallback";\n')
    fd.write('\n')
    fd.write('import {AdaptiveComponent} from "@afw/react";\n')
    fd.write('import {ctx} from "@afw/react";\n')
    fd.write('\n')
    fd.write('\n')
    fd.write('/**\n')
    fd.write(' * Implementation Id : ' + implementationId + '\n')
    fd.write(' * Category          : ' + category + '\n')
    fd.write(' * \n')

    if brief != None:
        # don't let lines go beyond 80 chars
        c.write_wrapped(fd, 78, ' * ', brief, '', '', True)
        fd.write(' * \n')

    if description != None:
        # don't let lines go beyond 80 chars
        c.write_wrapped(fd, 78, ' * ', description, '', '', True)
        fd.write(' * \n')

    fd.write(' */\n')
    fd.write('export const ' + implementationId + ' = (props) => {\n')
    fd.write('\n')
    fd.write('    return (\n')
    fd.write('        <AdaptiveComponent \n')
    fd.write('            {...props} \n')
    fd.write('            data-component-type={ctx(props, "' + implementationId + '")}\n')
    fd.write('            fallback={fallback}\n')
    fd.write('            layoutComponent={{\n')
    fd.write('                componentType: "' + implementationId + '",\n')
    fd.write('                parameters: props\n')
    fd.write('            }}\n')
    fd.write('        />\n')   
    fd.write('    );\n')
    fd.write('};\n')
    fd.write('\n')
    fd.write('\n')

    fd.write(implementationId + '.propTypes = propTypes;\n')
    fd.write(implementationId + '.defaultProps = defaultProps;\n')
    fd.write(implementationId + '.displayName = "' + implementationId + '";\n')

    fd.write('\n')
    fd.write('export default memo(' + implementationId + ');\n')


def generate(generated_by, options):

    # Make sure generated/ directory structure exists
    react_closet_dir = options['generated_dir_path'] + 'react_component_closet/'    

    os.makedirs(options['generated_dir_path'], exist_ok=True)
    os.makedirs(os.path.dirname(options['generated_dir_path'] + 'react_component_closet/'), exist_ok=True)    

    # Get all object types and sort by id.
    objectTypes = []
    objectTypesHash = {}
    if os.path.exists(options['objects_dir_path'] + '_AdaptiveObjectType_/'):
        objectTypes = direct.retrieve_objects_direct(options['objects_dir_path'] + '_AdaptiveObjectType_/')
        for objectType in objectTypes:
            objectTypesHash[objectType['_meta_']['objectId']] = objectType

    # Get all component types and sort by id.
    layoutComponentTypes = []
    if os.path.exists(options['objects_dir_path'] + '_AdaptiveLayoutComponentType_/'):
        layoutComponentTypes = direct.retrieve_objects_direct(options['objects_dir_path'] + '_AdaptiveLayoutComponentType_/')

    # Generate index.js for all components to be exportable
    msg.info('Generating index.js for all react components\n')
    
    with nfc.open(react_closet_dir + 'index.js', mode='w') as fd:
        fd.write('/**\n')
        fd.write(' * This file is auto-generated, used to export all components.\n')
        fd.write(' */\n')        

        exports = []
        for layoutComponentType in layoutComponentTypes:
            componentType = layoutComponentType.get('componentType')
            instanceObjectType = layoutComponentType.get('instanceObjectType')
            category = layoutComponentType.get('category')
            description = layoutComponentType.get('description')
            brief = layoutComponentType.get('brief')
            implementationId = layoutComponentType.get('implementationId')

            # make sure this componentType has an instanceObjectType
            if objectTypesHash.get(instanceObjectType) == None:
                msg.info('  Skipping component ' + componentType + ', because it doesn\'t have an instance object type\n')
                continue

            exports.append(implementationId)
            fd.write('\n')
            fd.write('export {default as ' + implementationId + '} from "./' + implementationId + '";\n')
            fd.write('export * from "./' + implementationId + '";\n')                  

    for layoutComponentType in layoutComponentTypes:
        componentType = layoutComponentType.get('componentType')
        instanceObjectType = layoutComponentType.get('instanceObjectType')        
        implementationId = layoutComponentType.get('implementationId')

        msg.info('Processing layoutComponent ' + componentType)

        # make sure this componentType has an instanceObjectType
        if objectTypesHash.get(instanceObjectType) == None:
            msg.info('  Skipping component ' + componentType + ', because it doesn\'t have an instance object type\n')
            continue

        closet_dir = react_closet_dir + componentType + '/'
        os.makedirs(os.path.dirname(closet_dir), exist_ok=True)

        msg.info('Generating React Component in Javascript: ' + componentType)
        with nfc.open(closet_dir + componentType + '.js', mode='w') as fd:
            generate_javascript(fd, objectTypesHash, objectTypesHash[instanceObjectType], componentType, implementationId, layoutComponentType)

        msg.info('  Generating propTypes..')
        with nfc.open(closet_dir + componentType + '.propTypes.js', mode='w') as fd:
            # Generate React propTypes from the property type definitions
            generate_prop_types(fd, objectTypesHash, objectTypesHash[instanceObjectType], componentType, implementationId)

        msg.info('  Generating React Component in Typescript..')
        with nfc.open(closet_dir + componentType + '.tsx', mode='w') as fd:
            # Generate React propTypes from the property type definitions
            generate_typescript(fd, objectTypesHash, objectTypesHash[instanceObjectType], componentType, implementationId, layoutComponentType)

        msg.info('  Generating typescript type definition..')
        with nfc.open(closet_dir + componentType + '.types.ts', mode='w') as fd:
            # Generate React typescript interface definitions from the property type definitions
            generate_typescript_types(fd, objectTypesHash, objectTypesHash[instanceObjectType], componentType, implementationId, layoutComponentType)

        msg.info('  Generating index..')
        with nfc.open(closet_dir + 'index.js', mode='w') as fd:
            fd.write('export {default} from "./' + implementationId + '";\n')            

        msg.info('  Generating types index..')
        with nfc.open(closet_dir + 'index.d.ts', mode='w') as fd:
            fd.write('export {default} from "./' + implementationId + '";\n')
            fd.write('export * from "./' + implementationId + '";\n')            
            
        with nfc.open(closet_dir + componentType + '.fallback.js', mode='w') as fd:            
            fd.write('export const ' + implementationId + ' = () => <div />;\n')
            fd.write('\n')
            fd.write('export default ' + implementationId + ';\n')            

        msg.info('  Generating Tests..')
        with nfc.open(closet_dir + componentType + '.test.js', mode='w') as fd:
            generate_jest_tests(fd, objectTypesHash, objectTypesHash[instanceObjectType], componentType, implementationId)            

        msg.info('  Generating Storybook..')
        with nfc.open(closet_dir + componentType + '.stories.js', mode='w') as fd:
            generate_stories(fd, objectTypesHash, objectTypesHash[instanceObjectType], componentType, implementationId, layoutComponentType)            

        msg.info('  Generating README..')
        with nfc.open(closet_dir + 'README.md', mode='w') as fd:
            generate_readme(fd, objectTypesHash, objectTypesHash[instanceObjectType], componentType, implementationId, layoutComponentType)            
                  

