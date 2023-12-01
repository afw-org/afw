#! /usr/bin/env python3

##
# @file cmake.py
# @ingroup afwdev_generate
# @brief This file contains the functions used to generate cmake dependencies.
#

import os
import glob
from _afwdev.common import msg, package, resources, nfc
from _afwdev.generate import c

def write_cmake_prologue(fd, options, title):
    fd.write(
"""# See the 'COPYING' file in the project root for licensing information.
#
# {title}
#
#----------------------------- N O T E -------------------------------------
#
# This file is generated by "afwdev generate".
#
# Do not change this file directly or the changes will be lost the next time
# this file is regenerated.
#
#----------------------------------------------------------------------------
#
""".format(title=title))


def _root_generate_write_properties(fd, prefix, properties):
    for key, value in properties:
        if key == 'licenses':
            continue
        name = prefix + key
        if type(value) == str:
            fd.write('set(' + name + ' "' + value.replace('"','\\""') + '")\n')
        elif type(value) == bool:
            if value:
                fd.write('set(' + name + ' ON)\n')
            else:
                fd.write('set(' + name + ' OFF)\n')
        elif type(value) == dict:
            fd.write('set(' + name + ' \n')
            for key2, value2 in value.items():
                fd.write('    "' + key2 + '"\n')
            fd.write(')\n')
            _root_generate_write_properties(fd, name + '_', value.items())
        elif type(value) == list:
            fd.write('set(' + name + ' ' + ' '.join(value) + ')\n')


def _root_generate_git_info_script(options, afw_package):

    # Write sh script to get git version
    resources.write_skeleton(options, 'git_info.sh',
        options['generated_dir_path'] + options['afwPackageGitInfoBaseName'] + '.sh',
        substitution=True, executable=True)


def _root_generate_afw_package_variables(options, afw_package):

    afw_package = package.get_afw_package(options)
    filename = 'afw_package_variables.cmake'
    msg.info('Writing ' + filename)
    with nfc.open(options['generated_dir_path'] + filename, mode='w') as fd:
        write_cmake_prologue(fd, options, 'afw-package.json property variables')
        fd.write(
"""# This sets a variable for each property in afw-package.json file. The variable
# names are "afw_package_", followed by any parent property names with an
# underscore (\'_\') appended, followed by the property name. For objects, the
# value is a list of its property names.
#
# This also sets additional variables AFW_PACKAGE_ID, AFW_PACKAGE_PROJECT_NAME,
# and AFW_PACKAGE_SOURCE_DIR, plus a variable prefixed with "AFW_PACKAGE_"
# instead of "CMAKE_" for each property set by GNUInstallDirs with some
# customizations.
#
# AFW_PACKAGE_INSTALL_CMAKEDIR and AFW_PACKAGE_INSTALL_FULL_CMAKEDIR are also
# set with the path where cmake related files for this particular afw package
# are installed.
#

""")
        _root_generate_write_properties(fd, "", {"afw_package": afw_package}.items())


        fd.write(
"""

# Set package id.
set(AFW_PACKAGE_ID ${afw_package_afwPackageId})

# Set variable with afw package project name.
set(AFW_PACKAGE_PROJECT_NAME ${PROJECT_NAME})

# Set variable with afw package source dir.
set(AFW_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# ----------------------------------------------------------------------------
#
# The following ar sets for each 'CMAKE_INSTALL_' prefixed variable set by
# GNUInstallDirs with a prefix of 'AFW_PACKAGE_INSTALL_' instead along with
# customization for some variables.
# 
# The LIBDIR and INCLUDEDIR a slash and the installPackageSubdir property from
# afw-package.json appended and the PROJECT_NAME part of DOCDIR is the value of
# the afwPackageId property from afw-package.json.
#
# Check https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html for more
# information.
#
# Also, AFW_PACKAGE_INSTALL_CMAKEDIR and AFW_PACKAGE_INSTALL_FULL_CMAKEDIR are
# set with path of LIBDIR subdirectory cmake/${PROJECT_NAME}-${PROJECT_VERSION}
#
# These variables are used exclusively by afwdev generated cmake instead of
# their 'CMAKE_INSTALL_' counterparts.
#
# ----------------------------------------------------------------------------

include(GNUInstallDirs)

set(AFW_PACKAGE_INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR})

set(AFW_PACKAGE_INSTALL_SBINDIR ${CMAKE_INSTALL_SBINDIR})

set(AFW_PACKAGE_INSTALL_LIBEXECDIR ${CMAKE_INSTALL_LIBEXECDIR})

set(AFW_PACKAGE_INSTALL_SYSCONFDIR ${CMAKE_INSTALL_SYSCONFDIR})

set(AFW_PACKAGE_INSTALL_SHAREDSTATEDIR ${CMAKE_INSTALL_SHAREDSTATEDIR})

set(AFW_PACKAGE_INSTALL_LOCALSTATE ${CMAKE_INSTALL_LOCALSTATE})

set(AFW_PACKAGE_INSTALL_RUNSTATEDIR ${CMAKE_INSTALL_RUNSTATEDIR})

if(DEFINED afw_package_installPackageSubdir)
    set(AFW_PACKAGE_INSTALL_LIBDIR
        ${CMAKE_INSTALL_LIBDIR}/${afw_package_installPackageSubdir})
    set(AFW_PACKAGE_INSTALL_INCLUDEDIR
        ${CMAKE_INSTALL_INCLUDEDIR}/${afw_package_installPackageSubdir})
else()
    set(AFW_PACKAGE_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR})
    set(AFW_PACKAGE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR})
endif()

set(AFW_PACKAGE_INSTALL_OLDINCLUDEDIR ${CMAKE_INSTALL_OLDINCLUDEDIR})

set(AFW_PACKAGE_INSTALL_DATAROOTDIR ${CMAKE_INSTALL_DATAROOTDIR})

set(AFW_PACKAGE_INSTALL_DATADIR ${CMAKE_INSTALL_DATADIR})

set(AFW_PACKAGE_INSTALL_LOCALEDIR ${CMAKE_INSTALL_LOCALEDIR})

set(AFW_PACKAGE_INSTALL_MANDIR ${CMAKE_INSTALL_MANDIR})

set(AFW_PACKAGE_INSTALL_DOCDIR
    ${AFW_PACKAGE_INSTALL_DATAROOTDIR}/doc/${afw_package_afwPackageId})

set(AFW_PACKAGE_INSTALL_CMAKEDIR
    ${CMAKE_INSTALL_LIBDIR}/cmake/${afw_package_afwPackageId}-${afw_package_version}
)

# Repeat for CMAKE_INSTALL_FULL_<dir>

set(AFW_PACKAGE_INSTALL_FULL_BINDIR ${CMAKE_INSTALL_FULL_BINDIR})

set(AFW_PACKAGE_INSTALL_FULL_SBINDIR ${CMAKE_INSTALL_FULL_SBINDIR})

set(AFW_PACKAGE_INSTALL_FULL_LIBEXECDIR ${CMAKE_INSTALL_FULL_LIBEXECDIR})

set(AFW_PACKAGE_INSTALL_FULL_SYSCONFDIR ${CMAKE_INSTALL_FULL_SYSCONFDIR})

set(AFW_PACKAGE_INSTALL_FULL_SHAREDSTATEDIR ${CMAKE_INSTALL_FULL_SHAREDSTATEDIR})

set(AFW_PACKAGE_INSTALL_FULL_LOCALSTATE ${CMAKE_INSTALL_FULL_LOCALSTATE})

set(AFW_PACKAGE_INSTALL_FULL_RUNSTATEDIR ${CMAKE_INSTALL_FULL_RUNSTATEDIR})

if(DEFINED afw_package_installPackageSubdir)
    set(AFW_PACKAGE_INSTALL_FULL_LIBDIR
        ${CMAKE_INSTALL_FULL_LIBDIR}/${afw_package_installPackageSubdir})
    set(AFW_PACKAGE_INSTALL_FULL_INCLUDEDIR
        ${CMAKE_INSTALL_FULL_INCLUDEDIR}/${afw_package_installPackageSubdir})
else()
    set(AFW_PACKAGE_INSTALL_FULL_LIBDIR ${CMAKE_INSTALL_FULL_LIBDIR})
    set(AFW_PACKAGE_INSTALL_FULL_INCLUDEDIR ${CMAKE_INSTALL_FULL_INCLUDEDIR})
endif()

set(AFW_PACKAGE_INSTALL_FULL_OLDINCLUDEDIR ${CMAKE_INSTALL_FULL_OLDINCLUDEDIR})

set(AFW_PACKAGE_INSTALL_FULL_DATAROOTDIR ${CMAKE_INSTALL_FULL_DATAROOTDIR})

set(AFW_PACKAGE_INSTALL_FULL_DATADIR ${CMAKE_INSTALL_FULL_DATADIR})

set(AFW_PACKAGE_INSTALL_FULL_LOCALEDIR ${CMAKE_INSTALL_FULL_LOCALEDIR})

set(AFW_PACKAGE_INSTALL_FULL_MANDIR ${CMAKE_INSTALL_FULL_MANDIR})

set(AFW_PACKAGE_INSTALL_FULL_DOCDIR
    ${AFW_PACKAGE_INSTALL_FULL_DATAROOTDIR}/doc/${afw_package_afwPackageId})

set(AFW_PACKAGE_INSTALL_FULL_CMAKEDIR
    ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/${afw_package_afwPackageId}-${afw_package_version}
)
""")


def _root_generate_git_info(options, afw_package):

    filename = 'afw_package_git_info.cmake'
    msg.info('Writing ' + filename)
    with nfc.open(options['generated_dir_path'] + filename, mode='w') as fd:
        write_cmake_prologue(fd, options, 'Produce the *git_info.h file')
        fd.write(
"""# This sets various cmake variables and adds a custom target that will run every
# build to produce a *git_info.h file with current git version information.
#

# Set git_info name.
set(AFW_PACKAGE_GIT_INFO_NAME ${afw_package_afwPackageLabel}_git_info)

# Set git_info header name.
set(AFW_PACKAGE_GIT_INFO_HEADER_NAME ${AFW_PACKAGE_GIT_INFO_NAME}.h)

# Set AFW_PACKAGE_GIT_INFO_SCRIPT_PATH with the path to the shell script to
# generate the *git_info.h header. If running on WIN32, the bash command
# is required to run this script.
set(AFW_PACKAGE_GIT_INFO_SCRIPT_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/${AFW_PACKAGE_GIT_INFO_NAME}.sh)
if (WIN32)
    set(AFW_PACKAGE_GIT_INFO_SCRIPT_PATH
        "bash ${AFW_PACKAGE_GIT_INFO_SCRIPT_PATH}")
endif()

# Add a custom target to generate *_git_info.h every build.
add_custom_target(
    ${AFW_PACKAGE_GIT_INFO_NAME} ALL
    COMMAND ${AFW_PACKAGE_GIT_INFO_SCRIPT_PATH}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/generated
    COMMENT "Generating git info header file"
)

add_library(${PROJECT_NAME} INTERFACE)
add_dependencies(${PROJECT_NAME} ${AFW_PACKAGE_GIT_INFO_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/generated ${CMAKE_BINARY_DIR}>
    $<INSTALL_INTERFACE:${AFW_PACKAGE_INSTALL_INCLUDEDIR}>
)

install(FILES "generated/${AFW_PACKAGE_GIT_INFO_HEADER_NAME}"
    DESTINATION ${AFW_PACKAGE_INSTALL_INCLUDEDIR}
)

""")


def _root_generate_add_subdirectories(options, afw_package):

    filename = 'afw_package_add_subdirectories.cmake'
    msg.info('Writing ' + filename)
    afw_config_build_prefix_has = afw_package['afwPackageLabel'].upper() + '_CONFIG_HAS_'
    with nfc.open(options['generated_dir_path'] + filename, mode='w') as fd:
        write_cmake_prologue(fd, options,
            'Add each subdirectory defined in afw-package.json')
        fd.write(
"""# This is an add_subdirectory() for each property of the "srcdirs" property in
# afw-package.json.
#
# If the subdir has the "optionalChoiceDefault" specified, the appropriate
# "option()" and "if()" logic surrounds the add_subdirectory().
#
""")
        for srcdir, info in afw_package['srcdirs'].items():

            # Skip the srcdirs whose buildType doesn't start with 'afwmake'
            #
            #NOTE: This will need to change if other buildTypes start using
            #      cmake.
            if not info.get('buildType','').startswith('afwmake'):
                continue

            srcdirPath = '${PROJECT_SOURCE_DIR}/' + info['srcdirPath']
            fd.write('\n')
            flag = srcdir.upper()
            if info.get('optionalChoiceDefault') is None:
                fd.write('set(' + afw_config_build_prefix_has + flag + ' 1)\n')
                fd.write('add_subdirectory("' + srcdirPath + '")\n')
            else:
                if info.get('optionalChoiceDefault'):
                    default = 'ON'
                else:
                    default = 'OFF'
                fd.write('option(BUILD_' + flag + ' "Enable making ' + info['produces'] + '" ' + default + ')\n')
                fd.write('if (BUILD_' + flag + ')\n')
                fd.write('    add_subdirectory("' + srcdirPath + '")\n')
                fd.write('    set(' + afw_config_build_prefix_has + flag + ' 1)\n')
                fd.write('else()\n')
                fd.write('    set(' + afw_config_build_prefix_has + flag + ' 0)\n')
                fd.write('endif()\n')


def _root_generate_config_header_in(options, afw_package):

    filename = afw_package['afwPackageLabel'] + '_config.h.in'
    msg.info('Writing ' + filename)
    afw_config_build_prefix = afw_package['afwPackageLabel'].upper() + '_CONFIG_'
    afw_config_build_prefix_has = afw_config_build_prefix + 'HAS_'
    with nfc.open(options['generated_dir_path'] + filename, mode='w') as fd:
        c.write_h_prologue(fd,
            'afwdev generate',
            'Config file for ' + afw_package['afwPackageId'],
            afw_package['afwPackageLabel'] + '_config.h')
            
        fd.write('#define ' + afw_config_build_prefix + 'INSTALL_BINDIR "@AFW_PACKAGE_INSTALL_BINDIR@"\n')
        fd.write('#define ' + afw_config_build_prefix + 'INSTALL_FULL_INSTALL_BINDIR "@AFW_PACKAGE_INSTALL_FULL_BINDIR@"\n')
        fd.write('#define ' + afw_config_build_prefix + 'INSTALL_INCLUDEDIR "@AFW_PACKAGE_INSTALL_INCLUDEDIR@"\n')
        fd.write('#define ' + afw_config_build_prefix + 'INSTALL_FULL_INCLUDEDIR "@AFW_PACKAGE_INSTALL_FULL_INCLUDEDIR@"\n')
        fd.write('#define ' + afw_config_build_prefix + 'INSTALL_LIBDIR "@AFW_PACKAGE_INSTALL_LIBDIR@"\n')
        fd.write('#define ' + afw_config_build_prefix + 'INSTALL_FULL_LIBDIR "@AFW_PACKAGE_INSTALL_FULL_LIBDIR@"\n')
        for srcdir, info in afw_package['srcdirs'].items():

            # Skip the srcdirs whose buildType doesn't start with 'afwmake'
            #
            #NOTE: This will need to change if other buildTypes start using
            #      cmake.
            if not info.get('buildType','').startswith('afwmake'):
                continue
            
            flag = srcdir.upper()
            fd.write('#cmakedefine ' + afw_config_build_prefix_has + flag + ' @' + afw_config_build_prefix_has + flag + '@\n')

        c.write_h_epilogue(fd, afw_package['afwPackageLabel'] + '_config.h')


def _root_generate_packaging(options, afw_package):

    filename = 'afw_package_packaging.cmake'
    msg.info('Writing ' + filename)
    with nfc.open(options['generated_dir_path'] + filename, mode='w') as fd:
        write_cmake_prologue(fd, options, 'CMake for cpack support')
        fd.write(
"""# This is the cmake for cpack support.
#
# You can include additional set() and other cmake commands before including
# this file as needed to further customize the packaging. 
#

# Set some cpack variable with values from afw-package.json.
set(CPACK_PACKAGE_NAME                  "${afw_package_afwPackageId}")
set(CPACK_PACKAGE_DESCRIPTION           "${afw_package_description}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY   "${afw_package_brief}")
set(CPACK_PACKAGE_HOMEPAGE_URL          "${afw_package_homePageUrl}")
set(CPACK_PACKAGE_CONTACT               "${afw_package_bugReportEmail}")

# Use git archive to generate a tarball of the source tree without untracked files
execute_process(
    COMMAND git archive --format=tar HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_FILE ${CMAKE_BINARY_DIR}/nonignored_source.tar
)

# Extract archive into ${CMAKE_BINARY_DIR}/nonignored_source.
# If cmake version is >= 3.18, use its file(ARCHIVE_EXTRACT) command. If
# cmake version is < 3.18, this is not available so use tar command.
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.18)
    file(ARCHIVE_EXTRACT
        INPUT ${CMAKE_BINARY_DIR}/nonignored_source.tar
        DESTINATION ${CMAKE_BINARY_DIR}/nonignored_source)
else()
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/nonignored_source)
    execute_process(
        COMMAND tar -xvf ${CMAKE_BINARY_DIR}/nonignored_source.tar -C ${CMAKE_BINARY_DIR}/nonignored_source
    )        
endif()
    
# Include CPack with CMAKE_SOURCE_DIR=${CMAKE_BINARY_DIR}/nonignored_source.
set(_PREVIOUS_CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR})
set(CMAKE_SOURCE_DIR ${CMAKE_BINARY_DIR}/nonignored_source)
include(CPack)
set(CMAKE_SOURCE_DIR ${_PREVIOUS_CMAKE_SOURCE_DIR})

""")


def _root_generate_afw_package_basic_build(options, afw_package):

    filename = 'afw_package_basic_build.cmake'
    msg.info('Writing ' + filename)
    with nfc.open(options['generated_dir_path'] + filename, mode='w') as fd:
        write_cmake_prologue(fd, options, 'Basic build for afw package')
        fd.write(
"""# This can be included in the root CMakeLists.txt file of an afw package if
# no customization is needed between includes. If you do need customization,
# you can selectively include the following commands in CMakeLists.txt as
# needed.
#

# We set the PROJECT_VERSION here, instead of in the top-level project() 
# command, because the afw-package.json variables are not available at the time.
set(PROJECT_VERSION ${afw_package_version})

# Variables for each property in afw-package.json.
include(generated/afw_package_variables.cmake)

# Produces and add the git_info.h file to CMAKE_BINARY_DIR.
include(generated/afw_package_git_info.cmake)

# add_subdirectory() for each srcdir in afw-package.json.
include(generated/afw_package_add_subdirectories.cmake)

""")
        config_filename = afw_package['afwPackageLabel'] + '_config.h'
        fd.write('# Configure the ' + config_filename + ' file into CMAKE_BINARY_DIR.\n')
        fd.write('configure_file(generated/' + config_filename + '.in\n')
        fd.write('    ${CMAKE_BINARY_DIR}/' + config_filename + ')\n')

        fd.write("""
# Include CPack support.
include(generated/afw_package_packaging.cmake)

""")


def root_generate(options):

    afw_package = package.get_afw_package(options)
 
    _root_generate_git_info_script(options, afw_package)
    _root_generate_git_info(options, afw_package)
    _root_generate_afw_package_variables(options, afw_package)
    _root_generate_add_subdirectories(options, afw_package)
    _root_generate_packaging(options, afw_package)
    _root_generate_config_header_in(options, afw_package)
    _root_generate_afw_package_basic_build(options, afw_package)



def _generate_subdir_afwdev_generated_variables(options, afw_package, srcdirInfo, generated_dir_path):

    srcdir = options['srcdir']
    srcdir_path = options['srcdir_path']
    srcdir_path_length = len(srcdir_path)
    prefix = options['prefix']
    ignore_dirs = [
        '/cmake/',
        '/doc/',
        '/generate/',
        '/generated/',
        '/tests/']

    # Determine c files that are not in ignored directories. Only subdirectories
    # are ignored in the generated/ directory.
    c_file_rpaths = [
            path[srcdir_path_length:].replace('\\','/')
            for path in glob.glob(srcdir_path + '**/*.c', recursive=True)
                if not any(ignore in path.replace('\\','/') for ignore in ignore_dirs)
        ] + [
            path[srcdir_path_length:].replace('\\','/')
            for path in glob.glob(srcdir_path + 'generated/*.c', recursive=False)       
        ]
 
    # Determine h files that are not in ignored directories. Only subdirectories
    # are ignored in the generated/ directory.
    h_file_rpaths = [
            path[srcdir_path_length:].replace('\\','/')
            for path in glob.glob(srcdir_path + '**/*.h', recursive=True)
                if not any(ignore in path.replace('\\','/') for ignore in ignore_dirs)
        ] + [
            path[srcdir_path_length:].replace('\\','/')
            for path in glob.glob(srcdir_path + 'generated/*.h', recursive=False)       
        ]

    # Determine h file directories needed during build
    h_file_rdirs = list(dict.fromkeys([
            os.path.dirname(path)[srcdir_path_length:].replace('\\','/')
            for path in glob.glob(srcdir_path + '**/*.h', recursive=True)
                if not any(ignore in path.replace('\\','/') for ignore in ignore_dirs)
        ] + [
            os.path.dirname(path)[srcdir_path_length:].replace('\\','/')
            for path in glob.glob(srcdir_path + 'generated/*.h', recursive=False)       
        ]))
    if '' in h_file_rdirs:
        h_file_rdirs.remove('')

    filename = 'afwdev_generated_variables.cmake'
    msg.info('Writing ' + filename)

    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        write_cmake_prologue(fd, options,
            'File list variables for subdir ' + options['srcdir'])
        fd.write('# This files sets 3 list variables based on files and directories in srcdir\n')
        fd.write('# ' + srcdir + ' that are needed a build time:\n')
        fd.write('#\n')
        fd.write('#  AFWDEV_GENERATED_PUBLIC_HEADERS_LIST - All of the public header files.\n')
        fd.write('#  AFWDEV_GENERATED_INCLUDE_DIRECTORIES_LIST - All of the build time include directories.\n')
        fd.write('#  AFWDEV_GENERATED_SOURCE_LIST - All of the c source files to compile.\n')
        fd.write('#\n')
        fd.write('# After including this file, use cmake commands like list(FILTER ...) to modify\n')
        fd.write('# the lists as needed.\n')
        fd.write('#\n')

        fd.write('\n')
        fd.write('# Header files that are installed for public use.\n')
        fd.write('set(AFWDEV_GENERATED_PUBLIC_HEADERS_LIST \n')
        for rpaths in sorted(h_file_rpaths):
            fd.write('    ' + rpaths + '\n')
        fd.write(')\n')

        fd.write('\n')
        fd.write('# Full path of directories searched for headers at build time.\n')
        fd.write('set(AFWDEV_GENERATED_INCLUDE_DIRECTORIES_LIST \n')
        fd.write('    ${CMAKE_CURRENT_SOURCE_DIR}\n')
        for rdirs in sorted(h_file_rdirs):
            fd.write('    ${CMAKE_CURRENT_SOURCE_DIR}/' + rdirs + '\n')
        fd.write(')\n')

        fd.write('\n')
        fd.write('# Source files to compile.\n')
        fd.write('set(AFWDEV_GENERATED_SOURCE_LIST \n')
        for rpaths in sorted(c_file_rpaths):
            fd.write('    ' + rpaths + '\n')
        fd.write(')\n')


def _generate_subdir_basic_afw_project(options, afw_package, srcdirInfo, generated_dir_path):
    srcdir = options['srcdir']
    srcdir_path = options['srcdir_path']
    prefix = options['prefix']

    #
    # The "produces" from afw_package.json for this srcdir is used to determine
    # project name and output name.
    #
    # If "produces" begins with "lib", this is an extension. Since CMake
    # prefixes output name with "lib" when appropriate for OS, the project name
    # will set to "produces" with "lib" removed.
    #
    # If this is not an extension, use "produces" as the project name with
    # "_executable" appended to make sure it doesn't conflict with an extension
    # with the same name. A "set_target_properties()" will be generated to
    # set OUTPUT_NAME to the "produces" value.
    #
    produces = srcdirInfo.get('produces','')
    is_extension = produces.startswith('lib')
    project_name = produces[3:] if is_extension else produces + '_executable'

    # Write basic_afw_project.cmake
    filename = 'basic_afw_project.cmake'
    msg.info('Writing ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        write_cmake_prologue(fd, options,
            'Basic afw project for subdir ' + options['srcdir'])
        fd.write(
"""# This file contains the project() and other commands always needed to build
# an afw project. After including this file, add other cmake commands for
# dependencies and other customizations, as needed.
#
""")
        fd.write('\n')
        fd.write('# project\n')
        fd.write('project(\n')
        fd.write('    ' + project_name + '\n')
        fd.write('    LANGUAGES C\n')
        fd.write('    VERSION ${afw_package_srcdirs_' + options['srcdir'] + '_version}\n')
        fd.write(')\n')
        fd.write('\n')
        fd.write('# Make sure redistributes are available on Windows.\n')
        fd.write('include(InstallRequiredSystemLibraries)\n')

        if is_extension:
            fd.write('\n')
            fd.write('# Add project library.\n')
            fd.write('add_library(${PROJECT_NAME} SHARED)\n')
            fd.write('\n')
            fd.write('# Cause the appropriate symbols to be exported for this library\n')
            fd.write('target_compile_definitions(${PROJECT_NAME}\n')
            fd.write('    PRIVATE ' + prefix.upper() + 'DECLARE_EXPORT\n)\n')
        else:
            fd.write('\n')
            fd.write('# Add project executable.\n')
            fd.write('add_executable(${PROJECT_NAME})\n')
            fd.write('\n')
            fd.write('# Set output name.\n')
            fd.write('set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ' + produces + ')\n')

        fd.write('\n')
        fd.write('# Package project must be built first.\n')
        fd.write('add_dependencies(${PROJECT_NAME} ${AFW_PACKAGE_PROJECT_NAME})\n')

        _compileOptions = afw_package.get('compileOptions')
        if _compileOptions is not None:
            fd.write('\n')
            fd.write('# Compile options from afw-package.json.\n')
            fd.write('target_compile_options(${PROJECT_NAME} PRIVATE\n')
            for _compilerId, _cflags in _compileOptions.items():
                fd.write('    $<$<C_COMPILER_ID:' + _compilerId + '>: ${afw_package_compileOptions_' + _compilerId  + '} >\n')
            fd.write(')\n')
        fd.write('\n')
        fd.write('# Add source files to project.\n')
        fd.write('target_sources(${PROJECT_NAME} PRIVATE ${AFWDEV_GENERATED_SOURCE_LIST})\n')

        # If not the afw library, add afw to this project's include and link.
        if not options['core']:
            
            # If not core afw package, set INSTALL_RPATH to the afw library
            # since the afw library has not been installed yet.
            if options.get('is_core_afw_package', False):
                fd.write('\n')
                fd.write('# Since this project is part of the same build that builds the afw library,\n')
                fd.write('# which has not been installed yet, set INSTALL_RPATH to be able to find it.\n')
                fd.write('set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH \n')
                fd.write('    "$ORIGIN/../${AFW_PACKAGE_INSTALL_LIBDIR}")\n')

            # Add afw library to target_link_libraries and
            # target_include_directories.
            fd.write('\n')
            fd.write('# Uses AFW core library.\n')
            fd.write('target_link_libraries(${PROJECT_NAME} PRIVATE afw::afw)\n')
            fd.write('target_include_directories(${PROJECT_NAME} PRIVATE afw::afw)\n')

            # Add own include directories.
            fd.write('\n')
            fd.write('# Add own include directories.\n')
            fd.write('target_include_directories(${PROJECT_NAME}\n')
            fd.write('    PRIVATE ${AFWDEV_GENERATED_INCLUDE_DIRECTORIES_LIST} )\n')

        fd.write('\n')
        fd.write('# Include root project directory and build directory for generated headers.\n')
        fd.write('target_include_directories(${PROJECT_NAME}\n')
        fd.write('    PRIVATE ${AFW_PACKAGE_SOURCE_DIR}/generated ${CMAKE_BINARY_DIR})\n')
        fd.write('\n')



def _generate_subdir_basic_afw_install(options, afw_package, srcdirInfo, generated_dir_path):

    filename = 'basic_afw_install.cmake'
    msg.info('Writing ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        write_cmake_prologue(fd, options,
            'Basic install for subdir ' + options['srcdir'])
        fd.write(
"""# The basic afw install for a subdir is to install the library or executable
# built by the subdir's project. Do not include this file if you need a
# different type of install.
#

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${AFW_PACKAGE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${AFW_PACKAGE_INSTALL_BINDIR}
)
""")


def _generate_subdir_basic_afw_build(options, afw_package, srcdirInfo, generated_dir_path):

    filename = 'basic_afw_build.cmake'
    msg.info('Writing ' + filename)
    with nfc.open(generated_dir_path + filename, mode='w') as fd:
        write_cmake_prologue(fd, options,
            'Basic build for subdir ' + options['srcdir'])
        fd.write(
"""# This can be included by the CMakeLists.txt file of an srcdir if no
# customization is needed. If customization is needed and any of the following
# includes are useful, put them directly in the CMakeLists.txt.
#

# Include afwdev generated variables.
include(generated/afwdev_generated_variables.cmake)

# Include basic afw project.
include(generated/basic_afw_project.cmake)

# Include basic afw install.
include(generated/basic_afw_install.cmake)
""")

def generate(options):
    afw_package = package.get_afw_package(options)
    srcdirInfo = afw_package.get('srcdirs',{}).get(options['srcdir'])
    generated_dir_path = options['generated_dir_path']

    # Make subdirectory 'cmake' in options['generated_dir_path']
    if not os.path.exists(generated_dir_path):
        os.makedirs(generated_dir_path)

    _generate_subdir_afwdev_generated_variables(options, afw_package, srcdirInfo, generated_dir_path)
    _generate_subdir_basic_afw_project(options, afw_package, srcdirInfo, generated_dir_path)
    _generate_subdir_basic_afw_install(options, afw_package, srcdirInfo, generated_dir_path)
    _generate_subdir_basic_afw_build(options, afw_package, srcdirInfo, generated_dir_path)
