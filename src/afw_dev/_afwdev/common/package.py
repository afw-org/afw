#! /usr/bin/env python3

##
# @file package.py
# @ingroup afwdev_common
# @brief This file contains some common functions for working with afw packages.
#

import os
from _afwdev.common import msg, nfc
from _afwdev.validate import validate


# Required keys in afw-package.json
_required_afw_package_properties = [
    'afwPackageId',
    'version'
    ]

# Optional keys in afw-package.json
_optional_afw_package_properties = [
    '$schema',
    'brief',
    'bugReportEmail',
    'compileOptions',
    'copyright',
    'description',
    'homePageUrl',
    'installPackageSubdir',
    'licenses',
    'srcdirManifest',
    'srcdirs',
    'tarballName']

_optional_srcdirs_properties = [
    'brief',
    'buildType',
    'buildTypeParameters',
    'description',
    'ignore',
    'optionalChoiceDefault',
    'prefix',
    'produces',
    'srcdirPath',
    'version']

def _check_srcdir_info(info):
    for name, value in info.items():
        if name not in _optional_srcdirs_properties:
            msg.error_exit('Invalid afw-package.json srcdirs property: ' + name)

def set_is_core_afw_package(options):
    options['is_core_afw_package'] = os.path.exists(
        options['afw_package_dir_path'] + 'src/afw/environment/afw_environment.c')


def _check_srcdirs(package):

    srcdirs = package.get('srcdirs')
    if srcdirs is None:
        package['srcdirs'] = {}
        return

    for srcdir, info in srcdirs.items():
        for name, value in info.items():
            if name not in _optional_srcdirs_properties:
                msg.error_exit('Invalid afw-package.json srcdirs property: ' + name)
        if info.get('srcdirPath') is None:
            info['srcdirPath'] = srcdir
        info['srcdirPath'] = info['srcdirPath'].replace('\\', '/') 
        if not info['srcdirPath'].endswith('/'):
            info['srcdirPath'] += '/'
        if info.get('prefix') is None:
            info['prefix'] = srcdir + '_'
        if info.get('buildType') is None:
            msg.info('afw-package.json srcdir ' + srcdir + ' has no buildType')
        if info.get('version') is None:
            info['version'] = package['version']
        # Find extendable way to check valid type or start using object type


def _read_package(afw_package_dir_path):
    if not os.path.exists(afw_package_dir_path):
        msg.error_exit(afw_package_dir_path + " not found")

    if not os.path.exists(afw_package_dir_path + 'afw-package.json'):
        msg.error_exit('Missing afw-package.json in ' + afw_package_dir_path)

    with nfc.open(afw_package_dir_path + 'afw-package.json') as fd:
        package = nfc.json_load(fd)

    return package


def _write_package(afw_package_dir_path, package):
    with nfc.open(afw_package_dir_path + 'afw-package.json', 'w') as fd:
        nfc.json_dump(package, fd, indent=4, sort_keys=True)


def _check_package(package):
    for name in package:
        if name not in _required_afw_package_properties and name not in _optional_afw_package_properties:
            msg.error_exit('Unknown property ' + name + ' in afw-package.json')

    for name in _required_afw_package_properties:
        if package.get(name) is None:
            msg.error_exit('afw-package.json ' + name + ' is required')

    _check_srcdirs(package)    

    return package

def _get_afw_package_dir_path(options):
    if options.get('afw_package_dir_path') is None:
        path = os.getcwd()
        while path != '':
            if os.path.exists(path + '/afw-package.json'):
                options['afw_package_dir_path'] = path + '/'
                break
            path = os.path.split(path)[0]
        if path == '':
            msg.error_exit('--path must be specified, or the current working directory must be inside of an AFW package')
    set_is_core_afw_package(options)
    return options['afw_package_dir_path']


def get_afw_package(options):
    afw_package_dir_path = _get_afw_package_dir_path(options)
    options['afw_package'] = _read_package(afw_package_dir_path)
    _check_package(options['afw_package'])

    # Check new way and warn if error.
    # error = validate.validate(options, afw_package_dir_path, json_object=options['afw_package'])
    # if error is not None:
    #     msg.warn('afw-package.json has errors')
    #     msg.warn('\n'.join(error) + '\n')

    # afwPackageLabel is afwPackageId with '-' changed to '_'. Needs to be suitable for variable name.
    options['afw_package']['afwPackageLabel'] = options['afw_package']['afwPackageId'].replace('-','_')
    options['afwPackageId'] = options['afw_package']['afwPackageId']
    options['afwPackageGitInfoBaseName'] = options['afwPackageId'].replace('-','_') + '_git_info'
    # set the default copyright, if not present
    options['afw_package']['copyright'] = options['afw_package'].get('copyright', 'Copyright (c) 2010-2024 Clemson University')
    return options['afw_package']


def refresh_afw_package(options):
    options['afw_package'] = None
    return get_afw_package(options)

# This function can be called to set options using the properties of the
# existing srcdir object in afw-package.json.
#
# If set_all is not True, options prefix, brief, and description will not set.
# Generally, the optional arguments and their defaults of add-* subcommands
# have already set these options which might only apply to the thing being
# added and not the whole srcdir.
def set_options_from_existing_package_srcdir(options, srcdir, set_all=False):
    afw_package = get_afw_package(options)
    if afw_package.get('srcdirs') is None:
        msg.error_exit('afw-package.json missing srcdir ' + srcdir)
    info = afw_package['srcdirs'].get(srcdir)
    if info is None:
        msg.error_exit('afw-package.json missing srcdir ' + srcdir)

    options['srcdir_info'] = info
    options['core'] = (srcdir == 'afw')
    options['buildType'] = info.get('buildType')
    options['produces'] = info.get('produces')
    options['srcdir'] = srcdir
    options['srcdir_prefix'] = info['prefix']
    options['srcdir_relpath'] = info['srcdirPath']    
    options['srcdir_path'] = options['afw_package_dir_path'] + info['srcdirPath']    
    set_is_core_afw_package(options)
    if set_all:
        options['prefix'] =  info.get('prefix')
        options['brief'] = info.get('brief')
        options['description'] = info.get('description')
  
    if not os.path.exists(options['srcdir_path']):
        msg.error_exit(options['srcdir_path'] + ' for ' + options['srcdir'] + ' does not exist')


# options afw_package_dir_path, brief, description, prefix, and srcdir must already be set.
def add_package_srcdir_using_options(options, produces, buildType='afwmake',extension=False):
    afw_package_dir_path = options['afw_package_dir_path']
    srcdir = options['srcdir']
    info = {
        'brief': options.get('brief'),
        'buildType': buildType,
        'description': options.get('description'),
        'prefix': options.get('prefix'),
        'produces': produces,
        'srcdirPath': 'src/' + srcdir + '/'
    }
    options['srcdir_info'] = info
    options['core'] = (srcdir == 'afw')
    options['buildType'] = buildType
    options['produces'] = produces
    options['srcdir_prefix'] = info['prefix']
    options['srcdir_relpath'] = info['srcdirPath']    
    options['srcdir_path'] = options['afw_package_dir_path'] + info['srcdirPath']    
    set_is_core_afw_package(options)
    
 
    _check_srcdir_info(info)
    package = _read_package(afw_package_dir_path)
    srcdirs = package.get('srcdirs')
    if srcdirs is None:
        srcdirs = {}
        package['srcdirs'] = srcdirs
    elif srcdirs.get(srcdir) is not None:
        msg.error_exit('srcdir ' + srcdir + ' is already in afw-package.json')

    if os.path.exists(options['srcdir_path']):
        msg.error_exit('srcdirPath ' + options['srcdir_relpath'] + ' already exists')

    os.makedirs(options['srcdir_path'])
    if extension and package.get('srcdirManifest') is None:
        package['srcdirManifest'] = srcdir
        msg.info('Making ' + srcdir + ' the manifest extension')

    package['srcdirs'][srcdir] = info
    _write_package(afw_package_dir_path, package)
    refresh_afw_package(options)