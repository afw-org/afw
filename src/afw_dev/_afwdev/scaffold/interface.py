#! /usr/bin/env python3
##
# @file interface.py
# @ingroup afwdev_scaffold
# @brief Copy a core interface skeleton (.h/.c) into a srcdir with substitutions.
# @details Expects options['interface_name'] and prefix/srcdir paths. Created
#          paths are appended to options['added_files'].
#

import os
from _afwdev.common import msg, nfc, resources


def add_interface(options):
    if not os.path.exists(options['srcdir_path']):
        msg.error_exit(options['srcdir_path'] + ' for ' + options['srcdir'] + ' does not exist')

    options['prefixed_interface_name'] = options['prefix'] + options['interface_name'].replace('afw_','')
    options['interface_h_file'] = options['prefixed_interface_name']  + '.h'
    options['interface_h_relpath'] = options['srcdir_relpath'] + options['interface_h_file']
    options['interface_h_path'] = options['afw_package_dir_path'] + options['interface_h_relpath']

    if os.path.exists(options['interface_h_path']):
        msg.error_exit(options['interface_h_path'] + ' already exists')
       
    msg.info('Adding src/' + options['srcdir'] + '/' + options['interface_h_file'])
    options['added_files'].append(options['interface_h_relpath'])
    skeleton = resources.load_skeleton(options, options['interface_name'] + '.h', substitution=True)
    with nfc.open(options['interface_h_path'], mode='w') as fd:
        fd.write(skeleton)

    options['interface_c_file'] = options['prefixed_interface_name']  + '.c'
    options['interface_c_relpath'] = options['srcdir_relpath'] + options['interface_c_file']
    options['interface_c_path'] = options['afw_package_dir_path'] +  options['interface_c_relpath'] 

    if os.path.exists(options['interface_c_path']):
        msg.error_exit(options['interface_c_path'] + ' already exists')

    msg.info('Adding ' + options['interface_c_relpath'])
    options['added_files'].append(options['interface_c_relpath'])
    skeleton = resources.load_skeleton(options, options['interface_name'] + '.c', substitution=True)
    with nfc.open(options['interface_c_path'], mode='w') as fd:
        fd.write(skeleton)
