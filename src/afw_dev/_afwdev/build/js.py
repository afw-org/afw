#! /usr/bin/env python3

import os
import subprocess
import pathlib
import json

from _afwdev.common import msg, package

# This builds the javascript modules and web apps
def build(options):

    # get the afwPackageId from afw-package.json
    afw_package = package.get_afw_package(options)
    afwPackageId = afw_package.get('afwPackageId')

    if not os.path.exists("package.json"):
        msg.highlighted_info("No package.json found. Skipping javascript build.")
        return
    
    # Unless verbose or debug mode, stdout will be sent to /dev/null.
    # Errors will still got to stderr either way.
    output = subprocess.DEVNULL
    if msg.is_verbose_mode() or msg.is_debug_mode():
        output = None

    # first make sure node_modules are installed
    if not os.path.exists('node_modules'):
        msg.highlighted_info('Installing node modules...')
        rc = subprocess.run(['npm', 'install'], stdout=output)
        if rc.returncode != 0:
            msg.error_exit('npm install failed')

    # if specified, clean the source directories
    if options.get("build_clean", False):        
        msg.highlighted_info('Cleaning javascript modules and web apps...')
        rc = subprocess.run(['npm', 'run', 'clean'], stdout=output)
        if rc.returncode != 0:
            msg.error_exit('npm run clean failed')
    
    # now run the modules builds
    msg.highlighted_info('Building javascript modules')
    if options.get('build_make_jobs') is None:
        rc = subprocess.run(['npm', 'run', 'build:modules'], stdout=output)
    else:
        rc = subprocess.run(['npm', 'run', 'build-p:modules'], stdout=output)
    if rc.returncode != 0:
        msg.error_exit('npm run build failed')

    # Use afw-package.json to get the list of apps and modules to build
    js_modules = filter(
        lambda x: x.get('buildType') == 'js-module', 
        afw_package.get('srcdirs').values())
    js_modules = list(map(lambda x: x.get('srcdirPath'), js_modules))

    js_apps = filter(
        lambda x: x.get('buildType') == 'js-app', 
        afw_package.get('srcdirs').values())
    js_apps = list(map(lambda x: x.get('srcdirPath'), js_apps))

    # now run the modules builds
    msg.highlighted_info('Building javascript apps')
    for app in js_apps:
        app_name = app.split('/')[-2]
        msg.highlighted_info('  Building ' + app_name)
        # For react apps, we need to set a few environment variables
        os.environ['PUBLIC_URL'] = '/apps/' + afwPackageId + '/' + app_name
        os.environ['REACT_APP_VERSION'] = afw_package.get('version')
        rc = subprocess.run(['npm', 'run', '--prefix', app, 'build'], stdout=output)
        if rc.returncode != 0:
            msg.error_exit('npm run build failed for app ' + app)

    # now copy the apps and modules into build/ folder
    msg.highlighted_info('Copying javascript modules and web apps to build folder...')

    # remove destination directories, if they exist
    if os.path.exists('build/js'):
        rc = subprocess.run(['rm', '-rf', 'build/js'], stdout=output)
        if rc.returncode != 0:
            msg.error_exit('Removing build/js failed.')
    
    # create directory skeletons
    pathlib.Path('build/js/apps/' + afwPackageId).mkdir(parents=True, exist_ok=True)
    pathlib.Path('build/js/modules/' + afwPackageId).mkdir(parents=True, exist_ok=True)    

    # copy to build/js/modules/${afwPackageId}/
    for module in js_modules:
        # parse the package.json file from the module path
        package_json_file = module + '/package.json'
        package_json = None
        with open(package_json_file, 'r') as f:
            package_json = json.load(f)

        if package_json is None:
            msg.error_exit('Could not parse ' + package_json_file)
        
        module_name = package_json.get('name')
        module_location = 'build/js/modules/' + afwPackageId + '/' + module_name

        # remove the "scripts" part of package.json
        package_json.pop('scripts', None)

        # add in the version
        package_json['version'] = afw_package.get('version')

        msg.highlighted_info('  Copying ' + module_name + ' to build/js/modules/' + afwPackageId)
        rc = subprocess.run(['mkdir', '-p', module_location], stdout=output)
        if rc.returncode != 0:
            msg.error_exit('mkdir ' + module_location + ' failed')

        rc = subprocess.run(['cp', '-r', module + '/build', module_location], stdout=output)
        if rc.returncode != 0:
            msg.error_exit('cp ' + module_location + ' failed')

        # write out new package.json to build location
        with open('build/js/modules/' + afwPackageId + '/' + module_name + '/package.json', 'w') as f:
            f.write(json.dumps(package_json, indent=2))
    
    # copy apps to build/js/apps/${afwPackageId}/
    for app in js_apps:
        app_name = app.split('/')[-2]
        app_location = 'build/js/apps/' + afwPackageId + '/' + app_name

        msg.highlighted_info('  Copying ' + app_name + ' to build/js/apps/')
        rc = subprocess.run(['cp', '-r', app + '/build', app_location], stdout=output)
        if rc.returncode != 0:
            msg.error_exit('copy to ' + app_location + ' failed')

    if options.get("build_install", False):
        web_root = options.get("build_web_root")

        # if specified, install it locally on the system
        msg.highlighted_info('Installing apps and modules into ' + web_root)

        # If --sudo argument was specified, use it.
        if options.get('build_sudo', False):
            rc = subprocess.run(['sudo', 'mkdir', '-p', web_root + '/apps/' + afwPackageId], stdout=output)
        else:
            rc = subprocess.run(['mkdir', '-p', web_root + '/apps/' + afwPackageId], stdout=output)

        # for each app, delete the target, if it exists and replace it with
        # the one from build/js/apps
        for app in js_apps:
            app_name = app.split('/')[-1]
            app_location = 'build/js/apps/' + afwPackageId + '/' + app_name

            if os.path.exists(web_root + '/apps/' + afwPackageId + '/' + app_name):
                msg.highlighted_info('  Removing existing app ' + app_name + ' from ' + web_root + '/apps/' + afwPackageId + '/')

                if options.get('build_sudo', False):
                    rc = subprocess.run(['sudo', 'rm', '-rf', web_root + '/apps/' + afwPackageId + '/' + app_name])
                else:
                    rc = subprocess.run(['rm', '-rf', web_root + '/apps/' + afwPackageId + '/' + app_name])

                if rc.returncode != 0:
                    msg.error("If the problem is a permission denied error and you can use sudo, try running with the --sudo parameter specified on afwdev. This will only use sudo for the install step.")
                    msg.error_exit('Removing ' + web_root + '/apps/' + afwPackageId + '/' + app_name + ' failed')

            msg.highlighted_info('  Installing new app ' + app_name + ' to ' + web_root + '/apps/' + afwPackageId + '/')

            if options.get('build_sudo', False):
                rc = subprocess.run(['sudo', 'cp', '-r', app_location, web_root + '/apps/' + afwPackageId + '/' + app_name])
            else:
                rc = subprocess.run(['cp', '-r', app_location, web_root + '/apps/' + afwPackageId + '/' + app_name])

            if rc.returncode != 0:
                msg.error("If the problem is a permission denied error and you can use sudo, try running with the --sudo parameter specified on afwdev. This will only use sudo for the install step.")
                msg.error_exit('Installation into ' + web_root + '/apps/' + afwPackageId + '/' + app_name + ' failed')
                