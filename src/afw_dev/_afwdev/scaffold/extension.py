#! /usr/bin/env python3
##
# @file extension.py
# @ingroup afwdev_scaffold
#

import os
from _afwdev.common import msg, nfc, package, resources
from _afwdev.scaffold.interface import add_interface
from _afwdev.scaffold.generate_hooks import call_generated_generate


def make_extension(options): 
    msg.highlighted_info("Adding extension " + options['implementation_id'], empty_before=True)
    package.add_package_srcdir_using_options(options, produces=options['produces'], extension=True)
    options['interface_name'] = 'afw_extension'

    if options.get('manifest_description') is None:
        options['manifest_description'] = 'Manifest for ' + options['implementation_id'] + ' extension.'

    msg.info('Adding skeleton ' + options['srcdir_relpath'] + '/generate/ directory')
    options['added_files'].append(options['srcdir_relpath'] + '/generate/*')
    resources.copy_resources(options, 'closet/generate/', todir=options['srcdir_path'] + 'generate/')

    reldirpath = options['srcdir_relpath']  + 'generate/manifest/'
    relfilepath = reldirpath  + 'manifest.json'
    msg.info('Adding ' + relfilepath)
    options['added_files'].append(relfilepath)
    skeleton = resources.load_skeleton(options, 'manifest.json', substitution=True)
    os.makedirs(options['afw_package_dir_path'] + reldirpath, exist_ok=True)
    with nfc.open(options['afw_package_dir_path'] + relfilepath, mode='w') as fd:
        fd.write(skeleton)

    msg.info('Adding interface afw_extension')
    add_interface(options)

    relfilepath = options['srcdir_relpath']  + 'manifest.json'
    msg.info("Adding " + relfilepath)
    options['added_files'].append(relfilepath)

    # Write CMakeLists.txt
    msg.info("Adding " + options['srcdir_relpath'] + " CMakeLists.txt")
    resources.write_skeleton(options, 'src_CMakeLists.txt',
        options['srcdir_path'] + 'CMakeLists.txt',
        substitution=True)

    # Generate extension
    msg.info("Generating " + options['srcdir'])
    call_generated_generate(options)
