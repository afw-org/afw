#! /usr/bin/env python3

##
# @file resources.py
# @ingroup afwdev_common
# @brief This file contains some common functions for working with resources 
#        associated with afw packages.
#

import os
import re
import zipfile
import subprocess
import platform
import stat
import glob
from subprocess import PIPE
from _afwdev.common import msg, nfc, package


def do_afwdev_substitutions(options, skeleton, path):
    if options.get('srcdirManifest') is None:
        afw_package = package.get_afw_package(options)
        options['srcdirManifest'] = afw_package.get('srcdirManifest')

    regexp = re.compile('<afwdev \{([^\}]+)\}>')
    exps = regexp.findall(skeleton)
    for exp in exps:
        parts = exp.split('.')
        value = options.get(parts[0])
        if value is None:
            msg.warn('Not replacing <afwdev {' + exp + '}> in ' + path)
            continue
        if len(parts) > 1:
            value = nfc.json_dumps(value) + '.' + '.'.join(parts[1:])
            value = eval(value)
        skeleton = skeleton.replace('<afwdev {' + exp + '}>', value)
    return skeleton


def sort_use_id_cb(obj):
    return obj['_meta_']['objectId']


#
# Copy resource
#
def _afwdev_src_copy_dir(options, path, fromdir, todir, substitution, recursive):
    list = []
    if not os.path.exists(fromdir):
        return list
    if todir is not None and not os.path.exists(todir):
        os.makedirs(todir)
    for filename in os.listdir(fromdir):
        if os.path.isdir(fromdir + filename):
            if recursive:
                _afwdev_src_copy_dir(options,
                    path + '/' + filename, fromdir + filename + '/', todir + filename + '/',
                    substitution, recursive)
        else:
            resource = nfc.read_path(fromdir + filename)
            if substitution:
                resource = do_afwdev_substitutions(options, resource, path + '/' + filename)
            object = { 'filename': filename, 'path': path, 'resource': resource }
            list.append(object)
            if todir is not None:
                with nfc.open(todir + filename, 'w') as fd:
                    fd.write(resource)

    return list


def _afwdev_src_copy_resources(options, folder, todir, substitution, recursive):
    # Ignore attempt to check previous directories
    if '..' in folder:
        return []

    path = folder
    if path != '' and path[-1] != '/':
        path += '/'

    fromdir = options['afwdev_dir_path'] + '_resources/' + path
    list = _afwdev_src_copy_dir(options, path, fromdir, todir, substitution, recursive)

    if (path == '' and recursive) or path == 'objects/_AdaptiveObjectType_/':
        fromdir = os.path.realpath(options['afwdev_dir_path'] +
            '../afw/generate/objects/_AdaptiveObjectType_/') + '/'

        list.extend(_afwdev_src_copy_dir(options, 'objects/_AdaptiveObjectType_/', fromdir,
            todir + 'objects/_AdaptiveObjectType_/' if path == '' else None,
            substitution, recursive))

    if (path == '' and recursive) or path == 'objects/_AdaptiveDataTypeGenerate_/':
        fromdir = os.path.realpath(options['afwdev_dir_path'] +
            '../afw/generate/objects/_AdaptiveDataTypeGenerate_/') + '/'

        list.extend(_afwdev_src_copy_dir(options, 'objects/_AdaptiveDataTypeGenerate_/', fromdir,
            todir + 'objects/_AdaptiveDataTypeGenerate_/' if path == '' else None,
            substitution, recursive))

    if (path == '' and recursive) or path == 'closet/':
        fromdir = os.path.realpath(options['afwdev_dir_path'] + '../afw/generated/interface_closet/') + '/'

        list.extend(_afwdev_src_copy_dir(options, 'closet/', fromdir,
            todir + 'closet/' if path == '' else '',
            substitution, recursive))

    return list


def _zipped_copy_resources(options, folder, todir, substitution, recursive):
    list = []
    path = '_resources/' + folder
    if path[-1] != '/':
        path += '/'
    len_prefix = len(path)
    if todir is not None:
        os.makedirs(todir, exist_ok=True)
    with zipfile.ZipFile(options['afwdev_path']) as zf:
        if substitution and options.get('license_c') is None:
            options['license_c']  = str(zf.read('_resources/closet/default_license.c'), 'utf-8')

        for filepath in zf.namelist():
            if filepath.startswith(path):
                filename = filepath[len_prefix:]

                # If recursion is False, ignore if filename has slash
                if not recursive and '/' in filename:
                    continue

                if todir is None:
                    tofile = filename
                else:
                    tofile = todir + filename
                if tofile == '':
                    continue

                if tofile.endswith('/'):
                    if todir is not None:
                        os.makedirs(tofile, exist_ok=True)
                else:
                    resource = str(zf.read(filepath), 'utf-8')
                    if substitution:
                        resource = do_afwdev_substitutions(options, resource, filepath)
                    object = { 'filename': filename, 'path': path, 'resource': resource }
                    list.append(object)
                    if todir is not None:
                        with nfc.open(tofile, 'w') as fd:
                            fd.write(resource)
    return list


## Copy resources
## @param folder in virtual _resources containing resources to copy
## @param todir is the path to an optional directory to copy resources to
## @param substitution True or False
## @param recursive True or False of contained directories
## @returns list of object with filename, path, and resource properties.
def copy_resources(options, folder, todir=None, substitution=False, recursive=False):
    use_todir = todir
    if use_todir != None and use_todir[-1:] != '/':
        use_todir += '/'
    if options['afwdev_is_zipped']:
        list = _zipped_copy_resources(options, folder, use_todir, substitution, recursive)
    else:
        list = _afwdev_src_copy_resources(options, folder, use_todir, substitution, recursive)
    return list


def _afwdev_src_load_resource(options, resource_name, substitution):
    path = None

    if '..' not in resource_name:
        path = options['afwdev_dir_path'] + '_resources/' + resource_name
        if not os.path.exists(path):
            if resource_name.startswith('closet/skeleton_'):               
                path = options['afwdev_dir_path'] + '../afw/generated/interface_' + resource_name
                if not os.path.exists(path):
                    path = None
            elif resource_name.startswith('objects/'):
                path = None
                possible_paths = glob.glob(
                    options['afw_package_dir_path'] + 'src/*/generated/' + resource_name)
                if len(possible_paths) < 1:
                    possible_paths = glob.glob(
                        options['afw_package_dir_path'] + 'src/*/generate/' + resource_name,)
                if len(possible_paths) > 1:
                    msg.warn('Multiple paths found for resource ' + resource_name)
                if len(possible_paths) > 0:
                    path = possible_paths[0]
                if path and not os.path.exists(path):
                    path = None

    if path is None:
        msg.error_exit('Resource ' + resource_name + ' not found')

    resource = nfc.read_path(path)

    if substitution:
        if options.get('license_c') is None:
            with nfc.open(options['afwdev_dir_path'] + '_resources/closet/default_license.c') as fd:
                options['license_c']  = fd.read()
        resource = do_afwdev_substitutions(options, resource, path)

    return resource


def _zipped_load_resource(options, resource_name, substitution=False):

    with zipfile.ZipFile(options['afwdev_path']) as zf:
        path = '_resources/' + resource_name
        try:
            resource = str(zf.read(path), 'utf-8')
        except KeyError:
            msg.error_exit('Resource ' + resource_name + ' not found')

        if substitution and options.get('license_c') is None:
            options['license_c']  = str(zf.read('_resources/closet/default_license.c'), 'utf-8')

    if substitution:
        resource = do_afwdev_substitutions(options, resource, path)

    return resource


## Load a resource
## @param options object
## @param resource_name
## @param substitution True or False
## @return resource
def load_resource(options, resource_name, substitution=False):
    if options['afwdev_is_zipped']:
        resource = _zipped_load_resource(options, resource_name, substitution)
    else:
        resource = _afwdev_src_load_resource(options, resource_name, substitution)
    return resource


def load_skeleton(options, skeleton_name, substitution=False):
    return load_resource(options, 'closet/skeleton_' + skeleton_name, substitution)


## Get core object types
## @param options object
def get_core_object_types(options):
    object_type_objects = []
    list = copy_resources(options, 'objects/_AdaptiveObjectType_/')

    for item in list:
        obj = nfc.json_loads(item.get('resource'))
        obj['_meta_'] = {}
        obj['_meta_']['objectId'] = obj['objectType']
        obj['_meta_']['objectType'] = '_AdaptiveObjectType_'
        object_type_objects.append(obj)
    object_type_objects.sort(key=sort_use_id_cb)

    return object_type_objects


def _set_get_git_info(options, cwd=None):
    if options.get('git_info') is not None:
        return

    # allow git info to be passed in via environment variables
    # this is useful for workflows that do not run out of a git repo
    git_branch = os.environ.get('GIT_BRANCH', '')
    git_describe = os.environ.get('GIT_DESCRIBE', '')
    git_sha = os.environ.get('GIT_SHA', '')

    git_info = {}
    if git_branch != '' and git_describe != '' and git_sha != '':
        git_info['git_branch'] = git_branch
        git_info['git_describe'] = git_describe
        git_info['git_sha'] = git_sha
    else:
        result = subprocess.run(["git", "rev-parse", "--abbrev-ref", "HEAD"], stdout=PIPE, cwd=cwd)
        if result.returncode != 0:
            msg.warn('Failed: git rev-parse --abbrev-ref HEAD')
            msg.warn('Using fake git rev-parse info')
            git_info['git_branch'] = 'unavailable-branch'
            git_info['git_describe'] = 'unavailable-describe'
            git_info['git_sha'] = "unavailable-sha"

        else:
            git_info['git_branch'] = result.stdout.decode().strip()
            result = subprocess.run(["git", "describe", "--dirty", "--always", "--tags"], stdout=PIPE, cwd=cwd)
            if result.returncode != 0:
                msg.error_exit('Failed: git describe --dirty --always --tags')
            git_info['git_describe'] = result.stdout.decode().strip()

            result = subprocess.run(["git", "rev-parse", "HEAD"], stdout=PIPE, cwd=cwd)
            if result.returncode != 0:
                msg.error_exit('Failed: git rev-parse HEAD')
            git_info['git_sha'] = result.stdout.decode().strip()

    options['git_info'] = git_info


def set_git_and_version_info_options(options, cwd=None):

    _set_get_git_info(options, cwd=cwd)
 
    if options.get('version') is None:
        msg.error_exit('version= property in afw-package.json is missing')

    result = options['git_info'].copy()
    result['version'] = options['version']
    version_major, version_minor, version_patch = result['version'].split('.')
    result['version_major'] = version_major
    result['version_minor'] = version_minor
    result['version_patch'] = version_patch
    result['version_string'] = version_major + '.' + version_minor + '.' + version_patch

    version_with_git_info = result['version']
    if result['git_branch'] != 'main':
        version_with_git_info += '-' + result['git_branch']
    version_with_git_info += '+' + result['git_describe']
    result['version_with_git_info'] = version_with_git_info

    options.update(result)
    return result


def get_afwdev_git_info(options):
    if options['afwdev_is_zipped']:
        afwdev_git_info = _zipped_load_resource(options, 'afwdev_git_info.json')
        afwdev_git_info = nfc.json_loads(afwdev_git_info)
    else:
        cwd = os.path.realpath(options['afwdev_dir_path'] + '../../')
        with nfc.open(os.path.join(cwd, 'afw-package.json')) as fd:
            afw_package = nfc.json_load(fd)
        options['version'] = afw_package.get('version','unknown')
        afwdev_git_info = set_git_and_version_info_options(options, cwd=cwd)
    return afwdev_git_info


def make_executable(path):
    if platform.system() == 'Linux' or platform.system() == 'Darwin':
        st = os.stat(path)
        os.chmod(path, st.st_mode | stat.S_IEXEC)     


def write_resource(options, resource_name, path, substitution=False, executable=False):
    resource = load_resource(options, resource_name, substitution=substitution)
    with nfc.open(path, 'w') as fd:
        fd.write(resource)
    if executable:
        make_executable(path) 


def write_skeleton(options, skeleton_name, path, substitution=False, executable=False):
    skeleton = load_skeleton(options, skeleton_name, substitution=substitution)
    with nfc.open(path, 'w') as fd:
        fd.write(skeleton)
    if executable:
        make_executable(path) 
