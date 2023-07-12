#! /usr/bin/env python3

import os
import jsonschema
import glob
import time
import sys
from _afwdev.common import msg, nfc

_schema_cache = {}

# The base name of json files whose parent directory is NOT the schema name.
_special_file_schemas = {
    'afw-package.json': '_AdaptivePackage_',
    'afwdev-settings.json': '_AdaptiveDevSettings_'
}

# Convert a path property to a string.
def _convert_path(e_path):
    path = ''
    for p in e_path:
        if isinstance(p, int):
            path += '[' + str(p) + ']'
        else:
            path += '/' + p
    if path == '':
        path = '/'
    return path


# Convert an exception to an error object.
def _exception_to_error(options, e):
    error = [str(type(e).__name__)]
    attrs = dir(e)
    if 'message' in attrs:
        error[0] += ': ' + str(e.message)
    if 'path' in attrs:
        error.append('objectPath: ' + _convert_path(e.path))
    if 'schema_path' in attrs:
        error.append('schemaPath: ' + _convert_path(e.schema_path))

    if options.get('debug', False):
        error.append('')
        error.append('Exception attributes:')
        for attr in attrs:
            if attr.startswith('_'):
                continue
            error.append('  ' + attr + ': ' + str(getattr(e, attr)))
    return error


#
# Validate JSON files matching a glob pattern.
#
# If the pattern starts with a '/', it is treated as an absolute path.
# Otherwise, it is treated as a path relative to the afw package directory.
#
def validate(options, path, json_object=None, schema_object=None):

    global _schema_cache
    global _special_file_schemas

    error = None

    # If json_object not passed, load it.
    if not json_object: 
        with nfc.open(path, mode='r') as fd:
            try:
                json_object = nfc.json_load(fd)
            except:
                error = ['Failed to load ' + path]

    # Determine object_type_id if schema_object was not passed.
    if not error and not schema_object:
        schema_uri = json_object.get('$schema')
        if schema_uri:
            if not schema_uri.startswith('./generated/schemas/afw/'):
                error = ['Schema ' + schema_uri +
                        ' does not start with ./generated/schemas/afw/']
            if not error:
                object_type_id = schema_uri[len('./generated/schemas/afw/'):]
                if object_type_id.endswith('.json'):
                    object_type_id = object_type_id[:-len('.json')]
        else:
            basename = os.path.basename(path)
            if basename in _special_file_schemas:
                object_type_id = _special_file_schemas[basename]
            else:
                object_type_id = os.path.split(os.path.dirname(path))[1]

    # If object_type_id is not already in schemas or schema_object passed, load
    # it.
    if not error and not schema_object:
        schema_object = _schema_cache.get(object_type_id)
        if not schema_object:
            try:
                schema_path = (options['afw_package_dir_path'] +
                    'generated/schemas/afw/' + object_type_id + '.json')
                with nfc.open(schema_path, mode='r') as fd2:
                    schema_object = nfc.json_load(fd2)
                    _schema_cache[object_type_id] = schema_object
            except:
                error = ['Schema ' + object_type_id + ' not found']

    # Validate object.
    if not error:
        try:
            jsonschema.validate(json_object, schema_object)
        except Exception as e:
            error = _exception_to_error(options, e)

    # Return error or None.
    return error


#
# Validate JSON files matching a glob pattern options['pattern'].
#
# If the pattern starts with a '/', it is treated as an absolute path.
# Otherwise, it is treated as a path relative to the afw package directory.
#
def run(options):

    # Use glob to get a list of paths for each pattern.
    paths = []
    for pattern in options['pattern'].split(';'):
        if not pattern.startswith('/'):
            pattern = options['afw_package_dir_path'] + pattern
        paths += glob.glob(pattern, recursive=True)
    paths = sorted(set(paths))

    #if not os.path.isfile(path):

    # Validate everything that matches pattern
    msg.highlighted_info('Validate JSON files matching ' + options['pattern'] + '\n')
    start = time.time()
    passed = 0
    failed = 0
    for path in paths:
    
        # Make a shortest_path for use in messages that is relative if the path
        # is in the afw package directory.
        shortest_path = path
        if path.startswith(options['afw_package_dir_path']):
            shortest_path = path[len(options['afw_package_dir_path']):]

        # Validate the file.
        error = validate(options, path)

        # Print a message.
        if error:
            failed += 1
            msg.error("\u2717", end="")
            msg.highlighted_info(" {}".format(shortest_path))
            if (msg.is_verbose_mode()):
                for e in error:
                    msg.error("  " + e)
            if options.get('bail', False):
                msg.highlighted_info("")            
                return 1
        else:
            passed += 1
            if not options.get('errors'):
                msg.success("\u2713", end="")
                msg.highlighted_info(" {}".format(shortest_path))
    end = time.time()  

    msg.highlighted_info("\nValidations: ", end="")
    total_tests = passed + failed
    if failed > 0:
        msg.error("{} failed".format(failed), end="")
        msg.highlighted_info(", ", end="")
    if passed > 0:
        msg.success("{} passed".format(passed), end="")
        msg.highlighted_info(", ", end="")    
            
    msg.highlighted_info("{} total".format(total_tests))
    msg.highlighted_info("Time:        {}s".format(
        round(end - start, 2)))

    # Return with 1 if there are errors or 0 if not.
    if failed > 0:
        return 1
    else:
        return 0
    