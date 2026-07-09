#! /usr/bin/env python3
##
# @file settings.py
# @ingroup afwdev_cli
# @brief Resolve afwdev-settings.json and related multi-package paths.
# @details Search walks up from the package root, then checks ~/ .
#          When afwPackages is unset, folders from afw.code-workspace that
#          contain afw-package.json are used. forSets['default'] is filled
#          with those package paths for the `afwdev for` subcommand.
#

import os
from _afwdev.common import msg, nfc


def get_afwdev_settings(options):
    afwdev_settings = {}
    afwdev_settings_path = None
    afwdev_settings_directory_path = None
    code_workspace_path = None
    code_workspace_directory_path = None
    afwPackages = None

    # Locate afwdev-settings.json
    dir = options['afw_package_dir_path'].rstrip('/')
    while True:
        if os.path.exists(dir + '/afwdev-settings.json'):
            afwdev_settings_path = os.path.realpath(dir + '/afwdev-settings.json')
            break
        if os.path.split(dir)[1] == '':
            break
        dir = os.path.split(dir)[0]
    if not afwdev_settings_path:
        if os.path.exists(os.path.expanduser('~/afwdev-settings.json')):
            afwdev_settings_path = os.path.expanduser('~/afwdev-settings.json')

    # If afwdev-settings.json found, use it to prime afwdev_settings
    if afwdev_settings_path:
        afwdev_settings = nfc.json_load_from_file(afwdev_settings_path)

        afwdev_settings['afwdev_settings_path'] = afwdev_settings_path
        afwdev_settings_directory_path = os.path.dirname(afwdev_settings_path)
        afwdev_settings_directory_path = os.path.realpath(afwdev_settings_directory_path)
        if not afwdev_settings_directory_path.endswith('/'):
            afwdev_settings_directory_path += '/'
        afwdev_settings['afwdev_settings_directory_path'] = afwdev_settings_directory_path
        afwPackages = afwdev_settings.get('afwPackages')

        # Make sure all afwPackage paths are real paths
        if afwPackages:
            for entry in afwPackages:
                if entry.get('path'):
                    if not entry['path'].startswith('/'):
                        entry['path'] = afwdev_settings_directory_path + entry['path']  
                    entry['path'] = os.path.realpath(entry['path'])
                    if not entry['path'].endswith('/'):
                        entry['path'] += '/'
                    if not os.path.exists(entry['path']):
                        msg.warn(
                            'path of entry in settings afwPackages could not be found: '
                            + entry['path'])

        if afwdev_settings.get('forSets'):
            for set_name, list in afwdev_settings['forSets'].items():
                i = 0
                for item in list:
                    if not item.startswith('/'):
                        item = afwdev_settings_directory_path + item
                        item = os.path.realpath(item)
                    if not item.endswith('/'):
                        item += '/'
                    if not os.path.exists(item):
                        msg.warn(
                            "forSets['" + set_name + "'] " + item +
                            " could not be found.")
                    list[i] = item
                    i += 1
                
 
    # Locate afw.code-workspace
    dir = options['afw_package_dir_path'].rstrip('/')
    code_workspace_path = afwdev_settings.get('codeWorkspacePath')
    if code_workspace_path:
        if not code_workspace_path.startswith('/'):
            code_workspace_path = afwdev_settings_directory_path + code_workspace_path
        if not os.path.exists(code_workspace_path):
            msg.warn('afwdev-settings.json[\'codeWorkspacePath\']: '
                + code_workspace_path + ' doesn\'t exists')
    else:
        while True:
            if os.path.exists(dir + '/afw.code-workspace'):
                code_workspace_path = dir + '/afw.code-workspace'
                break
            if os.path.split(dir)[1] == '':
                break
            dir = os.path.split(dir)[0]
        if not code_workspace_path:
            if os.path.exists(os.path.expanduser('~/afw.code-workspace')):
                code_workspace_path = os.path.expanduser('~/afw.code-workspace')

    # If afw.code-workspace found and afwdev_settings['afwPackages'] doesn't
    # exist, harvest them from the workspace folders.
    if code_workspace_path:
        code_workspace_path = os.path.realpath(code_workspace_path)
        afwdev_settings['code_workspace_path'] = code_workspace_path
        code_workspace_directory_path = os.path.dirname(code_workspace_path)
        code_workspace_directory_path = os.path.realpath(code_workspace_directory_path)
        if not code_workspace_directory_path.endswith('/'):
            code_workspace_directory_path += '/'
        afwdev_settings['code_workspace_directory_path'] = code_workspace_directory_path
        if not afwPackages:
            ws = nfc.json_load_from_file(code_workspace_path)
            if ws.get('folders'):
                afwPackages = []
                for folder in ws['folders']:
                    path = folder.get('path')
                    if path:
                        if not path.startswith('/'):
                            path = code_workspace_directory_path + path
                        path = os.path.realpath(path)
                        if not path.endswith('/'):
                            path += '/'
                        if os.path.exists(path + 'afw-package.json'):
                            afwPackages.append({"path": path})
                
    # If afwPackages is still not set, use current afw package path:
    if not afwPackages:
        afwPackages = [{'path': options['afw_package_dir_path']}]

    # Set afwPackages in afwdev_settings
    afwdev_settings['afwPackages'] = afwPackages

    # Make sure forSets default property exists
    if not afwdev_settings.get('afwPackages'):
        afwPackages = {}
        afwdev_settings['afwPackages'] = afwPackages
    if not afwdev_settings.get('forSets'):
        afwdev_settings['forSets'] = {}
    if not afwdev_settings['forSets'].get('default'):
        afwdev_settings['forSets']['default'] = []
        for package in afwPackages:
            if package.get('path'):
                afwdev_settings['forSets']['default'].append(package['path'])

    # Set afwdev_settings in options object
    options['afwdev_settings'] = afwdev_settings
