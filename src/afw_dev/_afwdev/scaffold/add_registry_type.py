#! /usr/bin/env python3
##
# @file add_registry_type.py
# @ingroup afwdev_scaffold
# @brief Shared flow for add-adapter-type / add-content-type / add-log-type.
#

import os
from _afwdev.common import msg, nfc, package, resources
from _afwdev.scaffold.common import msg_added_files
from _afwdev.scaffold.interface import add_interface
from _afwdev.scaffold.generate_hooks import (
    call_generated_generate, generate_primary, generate_register)


def begin_add_to_srcdir(options, thing_label):
    package.set_options_from_existing_package_srcdir(options, options['srcdir'])
    msg.highlighted_info(
        "Adding " + thing_label + " " + options['implementation_id'] +
        ' using prefix ' + options['prefix'] + ' to ' + options['srcdir_path'],
        empty_before=True)
    options['added_files'] = []
    options['registry_key'] = options['implementation_id']


def add_interfaces(options, interface_names):
    for name in interface_names:
        options['interface_name'] = name
        add_interface(options)


def add_conf_object(options, skeleton_name, object_filename):
    """Write generate/objects/_AdaptiveObjectType_/<object_filename>."""
    reldirpath = options['srcdir_relpath'] + 'generate/objects/_AdaptiveObjectType_/'
    relfilepath = reldirpath + object_filename
    msg.info('Adding ' + relfilepath)
    options['added_files'].append(relfilepath)
    skeleton = resources.load_skeleton(options, skeleton_name, substitution=True)
    os.makedirs(options['afw_package_dir_path'] + reldirpath, exist_ok=True)
    with nfc.open(options['afw_package_dir_path'] + relfilepath, mode='w') as fd:
        fd.write(skeleton)


def finish_registry_type(options, registry_type, registry_value, success_thing):
    generate_register(
        options, registry_type, options['implementation_id'], registry_value)
    msg.info("Generating " + options['srcdir'])
    call_generated_generate(options)
    generate_primary(options)
    msg.success(
        'Skeleton ' + success_thing + ' ' + options['implementation_id'] +
        ' added to ' + options['srcdir_relpath'] + '.')
    msg_added_files(options)
