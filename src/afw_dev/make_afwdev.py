#! /usr/bin/env python3

#
# Make afwdev single file command
#
# Run this anytime something changes in afwdev, the afw object types, or
# afw interface_closet/
#

import argparse, json, os, shutil, sys, zipapp

options = {}

# Get command_path, insert afwdev to sys.path, and import afwdev.common modules.
command_path = os.path.dirname(os.path.realpath(sys.argv[0]))
sys.path.insert(0, command_path + '/afwdev') 
from _afwdev.common import msg, package, resources
msg.info('Generating afwdev')

# Get command_path and default dir path
default_dir = os.path.join(command_path, 'temp')

# Parse arguments
parser = argparse.ArgumentParser()
parser.add_argument('--dir', help='path to the build binary directory', default=default_dir)
args = parser.parse_args()

# Set necessary options to use limited parts of afwdev.common modules.
afwdev_dir_path = command_path
if afwdev_dir_path != '':
    afwdev_dir_path += '/'
options['afwdev_dir_path'] = afwdev_dir_path
afw_package = package.get_afw_package(options)
options['afwdev_is_zipped'] = False
options['version'] = afw_package.get('version')

# Remove old temp dir and create a new one
temp_dir_path = os.path.join(args.dir, 'temp')
if os.path.exists(temp_dir_path):
    shutil.rmtree(temp_dir_path)
os.makedirs(temp_dir_path)

# Copy afwdev to temp. Add any files that should be ignored to ignore_patterns.
ignore = shutil.ignore_patterns(
    '.kdev4',
    '.cproject',
    '.project',
    '.settings',
    '.vs/',
    '__pycache__',
    '.dirstamp',
    '.deps',
    '.libs',
    '.eslintcache'
    )
shutil.copytree(os.path.join(afwdev_dir_path, '_afwdev'), os.path.join(temp_dir_path, '_afwdev'), ignore=ignore)
shutil.copytree(os.path.join(afwdev_dir_path, '_resources'), os.path.join(temp_dir_path, '_resources'), ignore=ignore)
shutil.copyfile(os.path.join(afwdev_dir_path, 'afwdev.py'), os.path.join(temp_dir_path, 'afwdev.py'))

# Add afw object types to afwdev's resources objects/_AdaptiveObjectType_/
shutil.copytree(
    os.path.join(options['afw_package_dir_path'], 'src/afw/generate/objects/_AdaptiveObjectType_'),
    os.path.join(temp_dir_path, '_resources/objects/_AdaptiveObjectType_'),
    ignore=ignore)

# Add afw data type objects to afwdev's resources objects/_AdaptiveDataType_/
shutil.copytree(
    os.path.join(options['afw_package_dir_path'], 'src/afw/generate/objects/_AdaptiveDataTypeGenerate_'),
    os.path.join(temp_dir_path, '_resources/objects/_AdaptiveDataTypeGenerate_'),
    ignore=ignore)

# Add afwdev_git_info.json
afwdev_git_info = resources.get_afwdev_git_info(options)
with open(os.path.join(temp_dir_path, '_resources/afwdev_git_info.json'), 'w', newline='', encoding='utf-8') as fd:
    json.dump(afwdev_git_info, fd, indent=4, sort_keys=True)

# Copy nonduplicate skeletons from afw/generated/interface_closet to afwdev's resources closet
afw_skeletons_dir_path = os.path.join(options['afw_package_dir_path'], 'src/afw/generated/interface_closet')
temp_afwdev_closet_dir_path = os.path.join(temp_dir_path, '_resources/closet')
for filename in os.listdir(afw_skeletons_dir_path):
    if not filename.startswith('skeleton'):
        continue
    if os.path.exists(os.path.join(temp_afwdev_closet_dir_path, filename)):
        msg.info('Skipping interface_closet/' + filename + ' because it already exists in afwdev closet')
        continue

    shutil.copyfile(
        os.path.join(afw_skeletons_dir_path, filename),
        os.path.join(temp_afwdev_closet_dir_path, filename)
        )

# Generate afwdev command
zipapp.create_archive(
    temp_dir_path,
    target=os.path.join(args.dir, 'afwdev'),
    interpreter='/usr/bin/env python3',
    main='afwdev:main')

msg.info('afwdev make successfully')
