#! /usr/bin/env python3
##
# @file make.py
# @ingroup afwdev_scaffold
#

import os
import subprocess
from _afwdev.common import msg, nfc, package, resources
from _afwdev.generate import generate
from _afwdev.scaffold.common import msg_added_files
from _afwdev.scaffold.extension import make_extension
from _afwdev.scaffold.generate_hooks import (
    call_generated_generate, generate_primary)


##
# @brief Subcommand make-afw-package
#
def subcommand_make_afw_package(args, options, get_afwdev_settings):
    msg.highlighted_info(
        'Making skeleton Adaptive Framework package ' +
        options['afw_package_dir_path'] + '.', empty_before=True)
    if os.path.exists(options['afw_package_dir_path']):
        msg.error_exit(options['afw_package_dir_path'] + ' already exists')
    os.makedirs(options['afw_package_dir_path'], exist_ok=False)
    options['afw_package_dir_path'] = (
        os.path.realpath(options['afw_package_dir_path']) + '/')

    msg.info('Writing afw-package.json')
    with nfc.open(options['afw_package_dir_path'] + 'afw-package.json', 'w') as fd:
        nfc.json_dump(
            {
                'afwPackageId': options['implementation_id'],
                'brief':  options['brief'],
                'description': options['description'],
                'homePageUrl': 'https://github.com/afw-org',
                'installPackageSubdir': 'afw',
                'copyright': 'Copyright (c) <Your Copyright Here>',
                'srcdirs': {
                },
                'version': '0.0.0'
            },
            fd,
            sort_keys=True,
            indent=4)

    get_afwdev_settings(options)

    msg.info('Copying initial files to package')
    resources.copy_resources(
        options, 'closet/package/', todir=options['afw_package_dir_path'],
        substitution=True)

    msg.info('Initializing git repository')
    rc = subprocess.run(
        ['git', 'init', '.'], cwd=options['afw_package_dir_path'])
    if rc.returncode != 0:
        msg.error_exit("git init . failed " + str(rc))

    msg.info('Making main default branch')
    rc = subprocess.run(
        ['git', 'checkout', '-b', 'main'], cwd=options['afw_package_dir_path'])
    if rc.returncode != 0:
        msg.error_exit("git checkout -b main failed " + str(rc))

    options['afwPackageId'] = options['implementation_id']
    options['afwPackageLabel'] = options['afwPackageId'].replace('-', '_')
    resources.write_skeleton(
        options, 'root_CMakeLists.txt',
        options['afw_package_dir_path'] + 'CMakeLists.txt',
        substitution=True)

    generate.root_generate(options)

    msg.success(
        'Skeleton AFW package made in ' + options['afw_package_dir_path'] + '.')
    msg.success(
        'Use other afwdev make-* subcommands and make changes as needed.')


##
# @brief Subcommand make-command
#
def subcommand_make_command(args, options):
    msg.highlighted_info(
        "Making command " + options['produces'], empty_before=True)
    package.add_package_srcdir_using_options(
        options, produces=options['produces'])
    options['added_files'] = []
    options['name'] = options['produces']
    options['description'] = 'Command ' + options['produces'] + '.'

    if options.get('verbose'):
        print("\nmake-command options:\n")
        print(nfc.json_dumps(options, sort_keys=True, indent=4))

    msg.info('Adding ' + options['srcdir_relpath'] + '/generate/ directory')
    options['added_files'].append(options['srcdir_relpath'] + '/generate/*')
    resources.copy_resources(
        options, 'closet/generate/', todir=options['srcdir_path'] + 'generate/')

    relfilepath = options['srcdir_relpath'] + options['produces'] + '.c'
    msg.info('Adding ' + relfilepath)
    options['added_files'].append(relfilepath)
    skeleton = resources.load_skeleton(options, 'command.c', substitution=True)
    with nfc.open(options['afw_package_dir_path'] + relfilepath, mode='w') as fd:
        fd.write(skeleton)

    msg.info("Adding " + options['srcdir_relpath'] + " CMakeLists.txt")
    resources.write_skeleton(
        options, 'src_CMakeLists.txt',
        options['srcdir_path'] + 'CMakeLists.txt',
        substitution=True)

    msg.info("Generating " + options['srcdir'])
    call_generated_generate(options)

    msg.success(
        'Skeleton command ' + options['produces'] +
        ' made in ' + options['srcdir_relpath'] + '.')
    msg_added_files(options)


##
# @brief Subcommand make-extension
#
def subcommand_make_extension(args, options):
    options['added_files'] = []
    make_extension(options)
    generate_primary(options)
    msg.success(
        'Skeleton extension ' + options['produces'] +
        ' made in ' + options['srcdir_relpath'] + '.')
    msg.success('Use afwdev add-* subcommands and make changes as needed.')
    msg_added_files(options)
