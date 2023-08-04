#! /usr/bin/env python3

import os
import subprocess
import pathlib

from _afwdev.common import msg

# This builds the javascript modules and web apps
def build(options):

    if not os.path.exists("package.json"):
        msg.highlighted_info("No package.json found. Skipping javascript build.")
        return

    # first make sure node_modules are installed
    if not os.path.exists('node_modules'):
        msg.highlighted_info('Installing node modules...')
        rc = subprocess.run(['npm', 'install'])
        if rc.returncode != 0:
            msg.error('npm install failed')
            return

    # if specified, clean the source directories
    if options.get("build_clean", False):        
        msg.highlighted_info('Cleaning javascript modules and web apps...')
        rc = subprocess.run(['npm', 'run', 'clean'])
        if rc.returncode != 0:
            msg.error('npm run clean failed')
            return
    
    # now run the build
    msg.highlighted_info('Building javascript modules and web apps...')
    build_cmd = build_cmd = ['npm', 'run', 'build']
    if  options.get('build_make_jobs') == 0:
        build_cmd = ['npm', 'run', 'build-p']   

    rc = subprocess.run(build_cmd)
    if rc.returncode != 0:
        msg.error('npm run build failed')
        return

    # now copy the apps and modules into build/ folder
    msg.highlighted_info('Copying javascript modules and web apps to build folder...')

    # remove destination directories, if they exist
    if os.path.exists('build/js'):
        rc = subprocess.run(['rm', '-rf', 'build/js'])
    
    # create directory skeletons
    pathlib.Path('build/js/apps').mkdir(parents=True, exist_ok=True)
    pathlib.Path('build/js/modules').mkdir(parents=True, exist_ok=True)    

    # copy modules
    rc = subprocess.run(['cp', '-r', 'src/afw_client/javascript', 'build/js/modules/afw_client'])
    rc = subprocess.run(['cp', '-r', 'src/afw/generated/javascript_bindings', 'build/js/modules/afw'])

    # copy apps
    rc = subprocess.run(['cp', '-r', 'src/afw_app/admin/build', 'build/js/apps/admin'])

    if options.get("build_install", False):
        # if specified, install it locally on the system
        msg.highlighted_info('Installing javascript modules and web apps...')
    