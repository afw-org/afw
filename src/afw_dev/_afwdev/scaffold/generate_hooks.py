#! /usr/bin/env python3
##
# @file generate_hooks.py
# @ingroup afwdev_scaffold
#

import os
from _afwdev.common import msg, nfc, package, resources
from _afwdev.generate import generate


def call_generated_generate(options):
    opts = options.copy()
    afw_package = package.get_afw_package(opts)
    opts['afw_package'] = afw_package
    if afw_package.get('version') is not None:
        opts['version'] = afw_package['version']
    generate.generate(opts)


def generate_primary(options):
    opts = options.copy()
    afw_package = package.get_afw_package(opts)
    srcdirManifest_path = afw_package['srcdirs'][afw_package['srcdirManifest']]['srcdirPath']
    opts['srcdir_path'] = opts['afw_package_dir_path'] + srcdirManifest_path
    if not opts['srcdir_path'].endswith('/'):
        opts['srcdir_path'] += '/'
    opts['srcdir'] = afw_package['srcdirManifest']
    opts['core'] = (opts['srcdir'] == 'afw')
    opts['implementation_id'] = opts['srcdir']
    opts['prefix'] =  opts['implementation_id'] + '_'
    msg.info("Generating " + opts['srcdir'])
    call_generated_generate(opts)
    
    generate.root_generate(options)


def generate_register(options, registry_type, registry_key, registry_value):
    package.get_afw_package(options)
    msg.info('Adding src/' + options['srcdir'] + '/generate/manifest/' + registry_type + '/' + registry_key + '.c')
    options['registry_type'] = registry_type
    options['registry_key'] = registry_key
    options['registry_value'] = registry_value
    os.makedirs(options['srcdir_path'] + 'generate/manifest/' + registry_type + '/', exist_ok=True )
    skeleton = resources.load_skeleton(options, 'register_' + registry_type + '.c', substitution=True)
    with nfc.open(options['srcdir_path'] + 'generate/manifest/' + registry_type + '/' + registry_key + '.c', mode='w') as fd:
        fd.write(skeleton)
