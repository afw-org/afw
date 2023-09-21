#! /usr/bin/env python3

import sys
import argparse
import os
import fnmatch
import zipfile
import shlex
import subprocess
from _afwdev.test import test, common
from _afwdev.build import build, ebnf_diagrams
from _afwdev.common import msg, package
from _afwdev.generate import generate
from _afwdev.common import resources, nfc
from _afwdev.validate import validate

#
# General information for afwdev command,
#

_afwdev_overall_info = {
        "description":
"""Command afwdev is a development tool used to create, extend, maintain, and
test an Adaptive Framework (AFW) Package.

An AFW package can be the one that holds the AFW core (afw) or other AFW
packages that contains AFW applications, commands, extensions, layouts,
servers, test data, etc.
""",
        "epilog":
    """
Directory structure of an AFW package
-------------------------------------

The basic structure of an AFW package is:

<AFW package root directory>
|-- doc/                         # Generated docs                   
|-- generated/                   # Produced by generate subcommand
|   |-- *.cmake                  # Generated cmake files
|-- src/                         # Commonly src/ contains source directories
|   |-- <SRCDIR>/                # One or more command, extension, or server
|       |-- generate/            # Processed by generate subcommand
|       |-- generated/           # Produced by generate subcommand
|       |   |-- c source         # Generated *.h and .c files
|       |   |-- *.cmake          # Generated cmake files
|       |   |-- ...              # Other generated files and directories
|       |-- c source             # <SRCDIR>_*.h and .c files
|       |-- CMakeLists.txt       # CMakeLists.txt for <SRCDIR>
|       |-- ...                  # Other source files and directories
|-- .editorconfig                # Source editor coding style
|-- .gitattributes               # Command git attributes for pathnames
|-- .gitignore                   # Command git files to ignore
|-- afw-package.json             # AFW package file managed by the afwdev
|-- CMakeLists.txt               # Root CMakeLists.txt for afw package
|-- COPYING                      # Licensing information.
|-- Doxyfile                     # Doxygen configuration file
|-- LICENSE                      # The license.
|-- package.json                 # Used to build a JavaScript package
|-- <PACKAGE_ID>_git_info.sh     # Used to produce <PACKAGE_ID>_git_info.h
|-- <PACKAGE_ID>_git_info.h      # Header file containing git information
|-- README.md                    # Repository README.md
|-- miscellaneous directories    # Other directories
|-- miscellaneous files          # Other files

Zero or more <SRCDIR> directories can exist.  Each <SRCDIR> contains the
source for one command, extension, or server implementation provided by the
AFW package. There can also be <SRCDIR> directories for other buildtypes.
This information is kept in afw-package.json.

All make-* subcommands make a new directory. The make-afw-package subcommand
makes a skeleton package directory that includes an afw-package.json file.
The other make-* subcommands make a skeleton directory in the src/ directory
and adds an entry to the afw-package.json "srcdir" property.

All add-* subcommands expect an existing "srcdir" entry in the afw-package.json
file that is either their implementation id or the value specified with the
--srcdir argument.

All add-* and make-* subcommands produces files with @todo comments that
should be searched for and replaced as appropriate.

If certain optional arguments are not specified, a prompt will occur that
allows the default value to be overridden. If the --noprompt argument is
specified or the specific optional argument is defined to never prompt, this
prompting will not occur. Specify the --prompt argument to insure prompting
occurs for all missing optional parameters.

Examples
--------

1) Make an AFW package "mypackage" in directory ~/mypackage:

    afwdev make-afw-package mypackage ~/mypackage

2) Make an extension "myextension" srcdir in "mypackage":

    cd ~/mypackage
    afwdev make-extension myextension

3) Add an adaptor type with id "myadaptor" to "myextension":

    cd ~/mypackage
    afwdev add-adaptor-type myadaptor --srcdir myextension

4) Make command "mycommand" srcdir in "mypackage":

    cd ~/mypackage
    afwdev make-command mycommand

5) Generate afwdev maintained files for "mypackage" after changes:

    cd ~/mypackage
    afwdev generate --srcdir-pattern \*

6) Run tests defined in "mypackage":

    cd ~/mypackage
    afwdev test --srcdir-pattern \*

7) Run tests defined in "mypackage" and show only errors:

    cd ~/mypackage
    afwdev test --errors --srcdir-pattern \*

"""
}

#
# Arguments available before all subcommands
#
# See subcommand doc below for attribute info.
#

_info_debug = {
    "optionName": "debug",
    "arg": "--debug",
    "action": "store_true",
    "default": False,
    "help": "Include maximum detail in output including afwdev debug info.",
    "exclusive_group": "verbosity"
}

_info_quiet = {
    "optionName": "quiet",
    "arg": "--quiet",
    "short": '-q',
    "action": "store_true",
    "default": False,
    "help": "Only output errors.",
    "exclusive_group": "verbosity"
}

_info_verbose = {
    "optionName": "verbose",
    "arg": "--verbose",
    "action": "store_true",
    "default": False,
    "help": "Include more detail in output.",
    "exclusive_group": "verbosity"
}

_info_noprompt = {
    "optionName": "noprompt",
    "arg": "--noprompt",
    "action": "store_true",
    "help":
        "Do not prompt for missing or invalid arguments. The --prompt argument "
        "overrides this argument."
}

_info_prompt = {
    "optionName": "prompt",
    "arg": "--prompt",
    "action": "store_true",
    "help":
        "Prompt for all missing optional arguments even if they normally would "
        "not be."
}

_info_version = {
    "optionName": "version",
    "arg": "--version",
    "short": "-v",
    "action": "version",
    "help": "Display version information."
}

_info_version_string = {
    "optionName": "version_string",
    "arg": "--version-string",
    "action": "version",
    "help": "Display version string information (no git info)."
}

_info_tmpdir = {
    "optionName": "tmpdir",
    "arg": "--tmpdir",
    "action": "store",
    "default": "/tmp",
    "help": "Directory to use for temporary files."
}

_args_for_all_commands_infos = [
    _info_debug,
    _info_quiet,
    _info_verbose,
    _info_noprompt,
    _info_prompt,
    _info_version,
    _info_version_string
]

#
# Subcommands, args, and options assignment.
#
# There are two types of objects below. The first is _info_<subcommand> which
# has info about the <subcommand> and a list of _info_<options>, and
# _info_<options> which has info for a <option>.
#
# _info_<subcommand>:
#     subcommand  - The subcommand
#     help        - Short help for this subcommand
#     description - Description for this subcommand
#     thing       - The thing this subcommand produces
#     arg         - A list of _info_<option>
#
# 
# _info_<option>:
#     optionName - The options[] key
#     help       - Short help for this option
#     default    - Optional default value for this option
#     noprompt   - Optional specify True to insure --noprompt for this option
#
#     If this is a command line argument, args and nargs  must be specified
#     that correspond to the parameters of parser.add_argument().
#
#     If these are not specified, default is required and will be used to
#     set the option.
#

# Often 1st
_info_implementation_id = {
    "optionName": "implementation_id",
    "arg": "implementation_id",
    "nargs": 1,
    "help": "Implementation id of this {thing}.",
}

# Often 2nd
_info_srcdir = {
    "optionName": "srcdir",
    "arg": "--srcdir",
    "default": "{implementation_id}",
    "nargs": 1,
    "help": "Name of source directory for this {thing}."
}

_info_prefix = {
    "optionName": "prefix",
    "arg": "--prefix",
    "nargs": 1,
    "default": "{implementation_id}_",
    "help": "The prefix for file names in the source directory for this {thing}."
}

_info_brief = {
    "optionName": "brief",
    "arg": "--brief",
    "nargs": 1,
    "default": "Implementation for {thing} {implementation_id}",
    "help": "A brief description for this {thing} without an ending period."
}

_info_description = {
    "optionName": "description",
    "arg": "--description",
    "nargs": 1,
    "default": "{brief}.",
    "help": "The description for this {thing}."
}

_info_srcdir_pattern = {
    "optionName": "srcdir_pattern",
    "arg": "--srcdir-pattern",
    "short": "-p",
    "default": "\*",
    "noprompt": True,
    "help": "<SRCDIR> name pattern"
}


# subcommand add-adaptor-type

_info_add_adaptor_type = {
    "subcommand": "add-adaptor-type",
    "help": "Add a new adaptor type",
    "description":  \
        "This will add skeletons for the basic interfaces afw_adaptor_factory, "
        "afw_adaptor, and "
        "afw_adaptor_session for a new adaptor type to an existing srcdir. "
        "The implementation_id specified will be the adaptor type. " 
        "If additional interfaces are needed, use subcommand add-core-interface.",
    "thing": "adaptor type",
    "args": [
        _info_implementation_id,
        _info_srcdir,
        _info_prefix,
        _info_brief,
        _info_description
    ]
}


# subcommand add-content-type

_info_add_content_type = {
    "subcommand": "add-content-type",
    "help": "Add a new content type",
    "description":
        "This will add skeletons for interfaces afw_content_type and "
        "afw_content_type_object_list_writer "
        "for a new content type to an existing srcdir, plus add a "
        "<CONTENT_TYPE_ID>.c to generate/manifest/content_type/. "
        "The implementation_id specified will be the content type id." ,
    "thing": "content type",
    "args": [
        _info_implementation_id,
        _info_srcdir,
        _info_prefix,
        _info_brief,
        _info_description
    ]
}


# subcommand add-core-interface

_info_interface = {
    "optionName": "interface_name",
    "arg": "interface",
    "nargs": 1,
    "help": "The name of the core interface to add."
}

_info_add_core_interface = {
    "subcommand": "add-core-interface",
    "help": "Add a core interface",
    "description":
        "This will add a skeleton for a core interface to an existing scrdir.",
    "thing": "interface",
    "args": [
        _info_interface,
        _info_implementation_id,
        _info_srcdir,
        _info_prefix,
        _info_brief,
        _info_description
    ]
}


# subcommand add-log-type

_info_add_log_type = {
    "subcommand": "add-log-type",
    "help": "Add a new log type",
    "description":
        "This will add the skeletons for the interfaces afw_log_factory and afw_log "
        "for a new log type to and existing srcdir. "
        "The implementation_id specified will be the log type id. " 
        "If additional interfaces are needed, use subcommand add-core-interface.",
    "thing": "log type",
    "args": [
        _info_implementation_id,
        _info_srcdir,
        _info_prefix,
        _info_brief,
        _info_description
    ]
}



# subcommand afwdev-parser-info

info_path = {
    "optionName": "output_path",
    "arg": "output_path",
    "nargs": 1,
    "help": "Output file path for {thing}.",
}



_info_afwdev_parser_info = {
    "subcommand": "afwdev-parser-info",
    "help": "afwdev parse info as xml",
    "description":
        "This subcommand will write the info used for afwdev argument parse to json.",
    "thing": "afwdev parse info",
    "args": [
        info_path
    ]
}


# subcommand build

_info_build_all = {
    "optionName": "build_all",
    "arg": "--all",     
    "action": "store_true",    
    "default": False,
    "help": "This is short for for the build type context switches --cmake, "
        "--docker, --docs, and --js."
}

_info_build_clean = {
    "optionName": "build_clean",
    "arg": "--clean",     
    "action": "store_true",    
    "default": False,
    "help": "Clean the build directory for each selected build type context "
        "before starting the build."
}

_info_build_cmake = {
    "optionName": "build_cmake",
    "arg": "--cmake",     
    "action": "store_true",    
    "default": False,
    "help": "This is the build type context switch to build everything that "
        "uses cmake in the AFW package. If no other build type context switches "
        "are specified, this is the default."
}

_info_build_docker = {
    "optionName": "build_docker",
    "arg": "--docker",     
    "action": "store_true",    
    "default": False,
    "help": "This is the build type context switch to build docker related "
        "parts of the AFW package."
}

_info_build_docs = {
    "optionName": "build_docs",
    "arg": "--docs",     
    "action": "store_true",    
    "default": False,
    "help": "This is the build type context switch to build the docs for the "
        "AFW package."
}

_info_build_cdev = {
    "optionName": "build_cdev",
    "arg": "--cdev",     
    "action": "store_true",    
    "default": False,
    "help": "This is a shortcut to enable common switches used during C development."
}

_info_build_generate = {
    "optionName": "build_generate",
    "arg": "--generate",     
    "action": "store_true",    
    "default": False,
    "help": "Call afwdev generate --srcdir-pattern \* before build."
}

_info_build_install = {
    "optionName": "build_install",
    "arg": "--install",     
    "action": "store_true",    
    "default": False,
    "help": "Install the built AFW package."
}

_info_build_jobs = {
    "optionName": "build_make_jobs",
    "arg": "--jobs",
    "short": "-j",
    "int" : True,
    "nargs": "?",
    "const": "0",
    "help":
        "Add -jobs [N] to make. "
        "If not specified, the \"make_jobs_argument\" property in the "
        "afwdev-settings.json file will be used if it exists."
}

_info_build_js = {
    "optionName": "build_js",
    "arg": "--js",     
    "action": "store_true",    
    "default": False,
    "help": "Build JavaScript related parts of the AFW package."
}

_info_build_maxloop = {
    "optionName": "build_maxloop",
    "arg": "--maxloop",     
    "nargs": 1,    
    "default": 10,
    "help": "This option is used by analyze-build. Specify the number of " 
        "times a block can be visited before giving up. Increase for more " 
        "comprehensive coverage at a cost of speed. "
}

_info_build_prefix = {
    "optionName": "build_prefix",
    "arg": "--prefix",     
    "nargs": 1,    
    "help": "This is the directory path that is prepended onto all install "
        "directories."
}

_info_build_sudo = {
    "optionName": "build_sudo",
    "arg": "--sudo",     
    "action": "store_true",    
    "default": False,
    "help": "Use sudo command when doing the install."
}

_info_build_scan = {
    "optionName": "build_scan",
    "arg": "--scan",     
    "action": "store_true",    
    "default": False,
    "help": "Use clang analyze-build to analyze C source after compilation. Errors will cause the build to fail."
}

_info_build_web_root = {
    "optionName": "build_web_root",
    "arg": "--web-root",
    "nargs": 1,
    "default": "/usr/share/nginx/html",
    "action": "store",
    "help": "The root directory for the web server on this local system."
}


_info_build = {
    "subcommand": "build",
    "help": "Build afw package",
    "description":
"""This subcommand will build an AFW package using information from the
afw-package.json file.

The build switches --cmake, --docker, --docs, --js provide build type context
that other switches, such as --clean, --install, and --generate will execute
under. The --all selects all of those contexts.
""",
    "thing": "build",
    "args": [
        _info_build_all,
        _info_build_cdev,
        _info_build_clean,
        _info_build_cmake,
        _info_build_docker,
        _info_build_docs,
        _info_build_generate,
        _info_build_install,
        _info_build_jobs,
        _info_build_js,
        _info_build_maxloop,
        _info_build_prefix,
        _info_build_scan,
        _info_build_sudo,
        _info_build_web_root
    ]
}


# subcommand ebnf
_info_ebnf = {
    "subcommand": "ebnf",
    "help": "Generate EBNF diagrams",
    "description": "This will generate EBNF diagrams from core syntax.ebnf.",
    "thing": "ebnf",
    "args": [
    ]
}

# subcommand for

_info_for_command = {
    "optionName": "command",
    "arg": "command",
    "nargs": 1,
    "help":
        "This is the command to run in a set of directories. "
        "If the command has arguments, it needs to be in quotes. "
        "Backslashes ('\\') will need to be escaped with a backslash. "
        "Multiple commands can be separated by \"&&\"."
}

_info_for_ignore_errors = {
    "optionName": "ignore_errors",
    "arg": "--ignore-errors",
    "short": "-g",   
    "action": "store_true",    
    "default": False,
    "help":
        "If specified, the failure of a command in one directory will not "
        "stop the running of the command in the remaining directories."
}

_info_set_name = {
    "optionName": "set_name",
    "arg": "--set-name",
    "short": "-s",   
    "nargs": 1,
    "default": "default",
    "noprompt": True,
    "help":
        "This is the \"set name\" used to identify the set of directories in "
        "which to run the command. "
}

_info_for = {
    "subcommand": "for",
    "help":
        "Run a command in a set of directories.",
    "description":
"""
        This will run a command in a set of directories identified by the 
        --set-name argument. 

        The set name is the name of the settings['forSets'] 
        property that has the list of directories to use. 

        Use the afwdev settings command and its --help for more information.
""",
    "thing": "for",
    "args": [
        _info_for_command,       
        _info_for_ignore_errors,
        _info_set_name
    ]
}

# subcommand generate

_info_generate = {
    "subcommand": "generate",
    "help": "Process <SRCDIR>/generate/",
    "description":
        "Process generate/ directories.  "
        "The SRCDIR_PATTERN is based on a subset of the rules used by unix shell "
        "and indicates that all matching <SRCDIR> directories that contain "
        "a generate/ directory should be processed.  See "
        "src/afwdev/doc/generate.txt "
        "in the AdaptiveFramework core package for more information.",
    "thing": "generate",
    "args": [
        _info_srcdir_pattern
    ]
}


# subcommand make-afw-package

_info_afw_package_dir_path = {
    "optionName": "afw_package_dir_path",
    "arg": "afw_package_dir_path",
    "nargs": 1,
    "help": "Directory path of new AFW package.",
}


_info_make_afw_package = {
    "subcommand": "make-afw-package",
    "help": "Make a AFW package directory.",
    "newPackageDirPath": True,
    "description":
        "This will make a new skeleton Adaptive Framework AFW package directory. "
        "The implementation_id specified will be the AFW Package Id and "
        "afw_package_dir_path it the new AFW packages directory path. "
        "An AFW package can hold AFW applications, commands, extensions, etc. "
        "The first afwdev make-extension issued in this AFW package will hold "
        "the package's manifest.",
    "thing": "AFW package",
    "args": [
        _info_implementation_id,
        _info_afw_package_dir_path,
        _info_brief,
        _info_description
    ]
}


# subcommand make-command


_info_command_name = {
    "optionName": "produces",
    "arg": "--name",
    "default": "{implementation_id}",
    "help": "The command name."
}

_info_make_command = {
    "subcommand": "make-command",
    "help": "Make a new command",
    "description": 
        "This will make a new command skeleton directory. "
        "The implementation_id specified will be the command name. "
        "Although more common for extensions, add-* subcommands can "
        "be used to add interfaces to this command.",
    "thing": "command",
    "args": [
        _info_implementation_id,
        _info_srcdir,
        _info_prefix,
        _info_command_name,
        _info_brief,
        _info_description
    ]
}


# subcommand make-extension

_info_library_name = {
    "optionName": "produces",
    "arg": "--name",
    "default": "lib{implementation_id}",
    "help": "The base part of library name (without .so). The name must begin with \"lib\"."
}

_info_make_extension = {
    "subcommand": "make-extension",
    "help": "Make a new extension",
    "description":
        "This will make a new extension skeleton directory that can hold "
        "implementations of AFW interfaces. "
        "The implementation_id specified will be the extension id. " 
        "Use add-* subcommands to add interfaces to this extension.",
    "thing": "extension",
    "args": [
        _info_implementation_id,
        _info_srcdir,
        _info_prefix,
        _info_library_name,
        _info_brief,
        _info_description
    ]
}

# subcommand settings

_info_settings = {
    "subcommand": "settings",
    "help": "Print settings",
    "description":
"""
This will print the settings used by afwdev. "
        
An "afwdev-settings.json" and/or an "afw.code-space" file 
located in the current AFW package directory, one of its parents 
or your home directory is used to produce these settings when 
afwdev starts. If none of these files exist, settings apply 
to your current AFW package directory. 

Information from "afwdev-settings.json" is load into settings 
first then normalized. 

If the "afwPackages" property does not exist, it will be 
constructed from paths in the folders of "afw.code-space" that 
contain an "afw-package.json" file 
or with a single entry for the current AFW package's root directory. 

If a "forSets['default']" property doesn't exist, one is created 
with all of the paths in "afwPackages". 

All paths in settings are normalized to their full path with a '/' 
at the end of all directory names.

The path to the "code-space" file can be explicitly specified 
with a "code_workspace_path" property in  "afwdev-settings.json". 

The default make --jobs argument for afwdev build can be specified 
with the "make_jobs_argument" property. For example, 
"make_jobs_argument":"--jobs 4" will run make using 4 threads 
if the -jobs argument is not specified on afwdev build.
""",        
    "thing": "settings",
    "args": [
    ]
}


# subcommand task

_info_task_name = {
    "optionName": "task_name",
    "arg": "task_name",
    "help": "The task name."
}

_info_task = {
    "subcommand": "task",
    "help": "Run a personalized afwdev task",
    "description": 
        "This will run the command(s) supplied by the \"tasks\" object property "
        "of afwdev-settings.json with a property name matching the specified "
        "task_name. Multiple commands can be separated by \"&&\". The failure "
        "of a command will cause the remaining commands to be skipped."
    ,
    "thing": "task",
    "args": [
        _info_task_name
    ]
}


# subcommand test

_info_test_bail = {
    "optionName": "bail",
    "arg": "--bail",     
    "short": "-b",
    "int" : True,
    "nargs": "?",
    "const": "0",
    "help": "Stop after [N] failed test."
}

_info_test_errors = {
    "optionName": "errors",
    "arg": "--errors",     
    "action": "store_true",    
    "default": True,
    "help": "Only show errors."
}

_info_test_list = {
    "optionName": "list",
    "arg": "--list",
    "action": "store_true",
    "default": False,
    "help": "List tests (do not run)"
}

_info_test_tags = {
    "optionName": "test_tags",
    "arg": "--tags",    
    "action": "store",
    "default": ".*",
    "noprompt": True,
    "help": "Test tags to match and run"
}

_info_test_watch = {
    "optionName": "watch",
    "arg": "--watch",
    "action": "store_true",
    "default": False,
    "help": "Watch for changes and re-run tests"
}

_info_test_jobs = {
    "optionName": "test_jobs",
    "arg": "--jobs",
    "short": "-j",
    "int" : True,
    "nargs": "?",
    "const": "0",
    "help":
        "Add -jobs [N] to test. "
        "If not specified, the \"test_jobs_argument\" property in the "
        "afwdev-settings.json file will be used if it exists."
}

_info_test_env_mode = {
    "optionName": "mode",
    "arg": "--env-mode",
    "action": "store",
    "default": "afw",
    "help": "The test environment mode to use."
}

_info_test_output = {
    "optionName": "output",
    "arg": "--output",    
    "action": "store",
    "default": "stdout",
    "noprompt": True,
    "help": "Where to send test output (default is stdout)."
}

_info_test_js = {
    "optionName": "javascript",
    "arg": "--js",    
    "action": "store_true",
    "default": False,
    "noprompt": True,
    "help": "Run Javascript and Web App tests."
}

_info_test = {
    "subcommand": "test",
    "help": "Run tests",
    "description": "Run tests for one more more source directories.",
    "thing": "test",
    "args": [        
        _info_test_bail,
        _info_test_list,
        _info_test_tags,
        _info_test_errors,
        _info_srcdir_pattern, 
        _info_test_watch,
        _info_test_jobs,
        _info_test_env_mode,
        _info_test_output,
        _info_test_js,
        _info_tmpdir
    ]
}


# subcommand validate

_info_pattern = {
    "optionName": "pattern",
    "arg": "--pattern",
    "short": "-p",
    "default": "afw-package.json;src/*/generate*/objects/*/*.json",
    "noprompt": True,
    "help": "One or more patterns, separated with a semicolon (';'), of files to validate."
}

_info_validation_errors_only = {
    "optionName": "errors",
    "arg": "--errors",     
    "action": "store_true",    
    "default": False,
    "help": "Only show validation errors."
}

_info_validation_bail = {
    "optionName": "bail",
    "arg": "--bail",     
    "action": "store_true",    
    "default": False,
    "help": "Stop on first validation failure."
}

_info_validate = {
    "subcommand": "validate",
    "help": "Validate json files using JSON Schemas",
    "description":
"""
This command will validate json files using JSON Schemas in the
generate/schemas/afw/ directory.

The pattern of objects to be validated are specified using the --pattern
argument. The default for pattern is "src/*/generate*/objects/*/*.json".

This pattern can have "*", "?", and "**" wildcards, much like Unix style
pathname patterns. The "**" wildcard will match any number of directories and
subdirectories. For example, the pattern "src/**/objects/*/*.json" will match
all *.json files in the objects/ directory of any subdirectory of the src/
package directory. '*' will match any number of characters except a '/' and '?'
will match any single character except a '/'.

If the pattern starts with a "/", it will be treated as an absolute path. If it
does not start with a "/", it will be treated as a relative path within the
afw package root directory that afwdev is operating on.

If a json file contains a "$schema" property, the value of that property will be
used to determine the schema to use. Otherwise, the schema will be determined
by the parent directory of the file. For example, if the json file is in the
"src/afw/generate/objects/_AdaptiveDataType_" directory, the
"_AdaptiveDataType_.json" schema in the "generated/schemas/afw/" directory will
be used.
""",
    "thing": "validate",
    "args": [
        _info_validation_errors_only,
        _info_validation_bail,
        _info_pattern
    ]
}


# List of all subcommand infos in order displayed in --help
_subcommand_infos = [
    _info_add_adaptor_type,
    _info_add_content_type,
    _info_add_core_interface,
    _info_add_log_type,
    _info_afwdev_parser_info,
    _info_build,
    _info_ebnf,
    _info_for,
    _info_generate,
    _info_make_afw_package,
    _info_make_command,
    _info_make_extension,
    _info_settings,
    _info_task,
    _info_test,
    _info_validate
]

#
# This object contains all of the help info and more for afwdev.
#
afwdev_info = {
    "overall": _afwdev_overall_info,
    "args_for_all_commands" : _args_for_all_commands_infos,
    "subcommands": _subcommand_infos
}


#
# End of afwdev info.
#

def _msg_added_files(options):
    msg.success('Review the following files, paying close attention to @todo comments:')
    added = options['added_files']
    added.sort()
    for file in added:
        msg.success('  ' + file)

def _get_version(options):
    opt = options.copy()
    info = resources.get_afwdev_git_info(opt)
    return info.get('version_with_git_info')

def _get_version_string(options):
    opt = options.copy()
    info = resources.get_afwdev_git_info(opt)
    return info.get('version_string')


#
# Called by subcommands to make extension
#
def _make_extension(options): 
    msg.highlighted_info("Adding extension " + options['implementation_id'], empty_before=True)
    package.add_package_srcdir_using_options(options, produces=options['produces'], extension=True)
    options['interface_name'] = 'afw_extension'

    if options.get('manifest_description') is None:
        options['manifest_description'] = 'Manifest for ' + options['implementation_id'] + ' extension.'

    msg.info('Adding skeleton ' + options['srcdir_relpath'] + '/generate/ directory')
    options['added_files'].append(options['srcdir_relpath'] + '/generate/*')
    resources.copy_resources(options, 'closet/generate/', todir=options['srcdir_path'] + 'generate/')

    reldirpath = options['srcdir_relpath']  + 'generate/manifest/'
    relfilepath = reldirpath  + 'manifest.json'
    msg.info('Adding ' + relfilepath)
    options['added_files'].append(relfilepath)
    skeleton = resources.load_skeleton(options, 'manifest.json', substitution=True)
    os.makedirs(options['afw_package_dir_path'] + reldirpath, exist_ok=True)
    with nfc.open(options['afw_package_dir_path'] + relfilepath, mode='w') as fd:
        fd.write(skeleton)

    msg.info('Adding interface afw_extension')
    _add_interface(options)

    relfilepath = options['srcdir_relpath']  + 'manifest.json'
    msg.info("Adding " + relfilepath)
    options['added_files'].append(relfilepath)

    # Write CMakeLists.txt
    msg.info("Adding " + options['srcdir_relpath'] + " CMakeLists.txt")
    resources.write_skeleton(options, 'src_CMakeLists.txt',
        options['srcdir_path'] + 'CMakeLists.txt',
        substitution=True)

    # Generate extension
    msg.info("Generating " + options['srcdir'])
    _call_generated_generate(options)


#
# Called by subcommands to add an interface
#
def _add_interface(options):

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

#
# Called by subcommands to generate a srcdir
#
def _call_generated_generate(options):
    opts = options.copy()
    afw_package = package.get_afw_package(opts)
    opts['afw_package'] = afw_package
    if afw_package.get('version') is not None:
        opts['version'] = afw_package['version']
    generate.generate(opts)


#
# Called by subcommands to generate primary manifest source.
#
def _generate_primary(options):
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
    _call_generated_generate(opts)
    
    generate.root_generate(options)


#
# Called by subcommands to generate a register
#
def _generate_register(options, registry_type, registry_key, registry_value):
    afw_package = package.get_afw_package(options)
    msg.info('Adding src/' + options['srcdir'] + '/generate/manifest/' + registry_type + '/' + registry_key + '.c')
    options['registry_type'] = registry_type
    options['registry_key'] = registry_key
    options['registry_value'] = registry_value
    os.makedirs(options['srcdir_path'] + 'generate/manifest/' + registry_type + '/', exist_ok=True )
    skeleton = resources.load_skeleton(options, 'register_' + registry_type + '.c', substitution=True)
    with nfc.open(options['srcdir_path'] + 'generate/manifest/' + registry_type + '/' + registry_key + '.c', mode='w') as fd:
        fd.write(skeleton)


#
# Subcommand add-adaptor-type
#
def _subcommand_add_adaptor_type(args, options):
    package.set_options_from_existing_package_srcdir(options, options['srcdir'])
    msg.highlighted_info("Adding adaptor type " + options['implementation_id'] + ' using prefix ' + options['prefix'] + ' to ' + options['srcdir_path'], empty_before=True)
    options['added_files'] = []
    options['registry_key'] = options['implementation_id']

    options['interface_name'] = 'afw_adaptor_factory'
    _add_interface(options)

    options['interface_name'] = 'afw_adaptor'
    _add_interface(options)

    options['interface_name'] = 'afw_adaptor_session'
    _add_interface(options)

    msg.info('Adding generate/objects/_AdaptiveObjectType_/_AdaptiveConf_adaptor_' + options['implementation_id'] + '.json')
    skeleton = resources.load_skeleton(options, '_AdaptiveConf_adaptor.json', substitution=True)
    dir_path = options['srcdir_path'] + 'generate/objects/_AdaptiveObjectType_/'
    os.makedirs(dir_path, exist_ok=True)
    with nfc.open(dir_path + '_AdaptiveConf_adaptor_' + options['implementation_id'] + '.json', mode='w') as fd:
        fd.write(skeleton)

    _generate_register(options, 'adaptor_type', options['implementation_id'], '&' + options['prefix'] + 'adaptor_factory' )
  
    # Generate new adaptor type and primary
    msg.info("Generating " + options['srcdir'])
    _call_generated_generate(options)
    _generate_primary(options)

    # Success
    msg.success('Skeleton adaptor type '  + options['implementation_id'] + ' added to ' + options['srcdir_relpath'] + '.')
    _msg_added_files(options)

#
# Subcommand add-content-type
#
def _subcommand_add_content_type(args, options):
    package.set_options_from_existing_package_srcdir(options, options['srcdir'])
    msg.highlighted_info("Adding content type " + options['implementation_id'] + ' using prefix ' + options['prefix'] + ' to ' + options['srcdir_path'], empty_before=True)
    options['added_files'] = []
    options['registry_key'] = options['implementation_id']

    options['interface_name'] = 'afw_content_type'
    _add_interface(options)

    _generate_register(options, 'content_type', options['implementation_id'], '&' + options['prefix'] + 'content_type' )

    # Generate content type and primary
    msg.info("Generating " + options['srcdir'])
    _call_generated_generate(options)
    _generate_primary(options)

    # Success
    msg.success('Skeleton content type '  + options['implementation_id'] + ' added to ' + options['srcdir_relpath'] + '.')
    _msg_added_files(options)


#
# Subcommand add-core-interface
#
def _subcommand_add_core_interface(args, options):
    package.set_options_from_existing_package_srcdir(options, options['srcdir'])
    msg.highlighted_info("Adding interface " + options['interface_name'] + ' to ' + options['srcdir_path'], empty_before=True)
    options['added_files'] = []

    # Add a core interface
    _add_interface(options)

    # Success
    msg.success('Skeleton interface '  + options['interface_name'] + ' added to ' + options['srcdir_relpath'] + '.')
    _msg_added_files(options)


#
# Subcommand add-log-type
#
def _subcommand_add_log_type(args, options):
    
    package.set_options_from_existing_package_srcdir(options, options['srcdir'])
    msg.highlighted_info("Adding log type " + options['implementation_id'] + ' using prefix ' + options['prefix'] + ' to ' + options['srcdir_path'], empty_before=True)
    options['added_files'] = []
    options['registry_key'] = options['implementation_id']

    if options['verbose']:
        print("\nadd-log-type options:\n")
        print(nfc.json_dumps(options, sort_keys=True, indent=4))

    options['interface_name'] = 'afw_log_factory'
    _add_interface(options)

    options['interface_name'] = 'afw_log'
    _add_interface(options)

    reldirpath = options['srcdir_relpath'] + 'generate/objects/_AdaptiveObjectType_/'
    relfilepath = reldirpath + '_AdaptiveConf_log_' + options['implementation_id'] + '.json'
    msg.info('Adding ' + relfilepath)
    options['added_files'].append(relfilepath)
    skeleton = resources.load_skeleton(options, '_AdaptiveConf_log.json', substitution=True)
    os.makedirs(options['afw_package_dir_path'] + reldirpath, exist_ok=True)
    with nfc.open(options['afw_package_dir_path'] + relfilepath, mode='w') as fd:
        fd.write(skeleton)

    _generate_register(options, 'log_type', options['implementation_id'], '&' + options['prefix'] + 'log_factory' )

    # Generate log type and primary
    msg.info("Generating " + options['srcdir'])
    _call_generated_generate(options)
    _generate_primary(options)

    # Success
    msg.success('Skeleton log type '  + options['implementation_id'] + ' added to ' + options['srcdir_relpath'] + '.')
    _msg_added_files(options)


#
#  Subcommand afw-parser-info
#
def _subcommand_afwdev_parser_info(args, options):
    
    options['subcommand'] = "afw-parser-info"

    with nfc.open(options['output_path'], 'w') as fd:
        nfc.json_dump(afwdev_info, fd, indent=4, sort_keys=True)
    msg.success('Parser info written to:  ' + options['output_path'])


#
#  Subcommand build
#
def _subcommand_build(args, options):
    
    options['subcommand'] = "build"

    build.run(options)
    msg.success('Build successful')


#
#  Subcommand ebnf
#
def _subcommand_ebnf(args, options):

    options['subcommand'] = "ebnf"    

    ebnf_diagrams.build(options)
    msg.success("Generate EBNF syntax diagrams successful")



#
# for subcommand
#
def _subcommand_for(args, options):
    options['subcommand'] = "for"
    errors = 0
    paths = None

    # Get the paths for set_name from settings
    forSet = options['afwdev_settings']['forSets']
    if not forSet.get(options['set_name']):
        msg.error_exit("Can't locate set " + options['set_name'] )
    paths = forSet.get(options['set_name'])

    # Split one or more commands separated with '&&'
    commands = []
    command = []
    commands.append(command)
    for token in shlex.split(options['command']):
        if token == '&&':
            command = []
            commands.append(command)
        else:
            command.append(token)

    # Run command(s) in each path
    for path in paths:
        msg.highlighted_info('Entering ' + path + ':')
        cwd = path
        failed = False
        for command in commands:
            try:
                rc = subprocess.run(command, cwd=cwd)
                if rc.returncode != 0:
                    failed = True
            except:
                failed = True
            if failed:
                errors += 1
                if not options.get('ignore_errors', False):
                    msg.error_exit('Error occurred while processing command in ' + cwd)
                else:
                    msg.warn('Ignored an error that occurred while processing command in ' + cwd)
            msg.highlighted_info('')

    # Show error count
    if errors != 0:
        if errors == 1:
            msg.warn('Ignored 1 error')
        else:
            msg.warn('Ignored ' + str(errors) + ' errors')


#
#  Subcommand generate
#
def _subcommand_generate(args, options):
    afw_package = package.get_afw_package(options)

    # The .replace('\\','') is a hack needed because visual code launch args:
    #
    # "args": ["generate", "*"],
    #
    # causes args.SRCDIR_PATTERN to have a list of all files in directory
    #
    # and
    #
    # "args": ["generate", "\\*"],
    #
    # causes args.SRCDIR_PATTERN to have '\\*' on at lease some some
    # system/python levels.
    #
    options['srcdir_pattern'] = options['srcdir_pattern'].replace('\\','')
    options['subcommand'] = "generate"

    # Process all the matching srcdirs
    for srcdir in afw_package['srcdirs']:
        if not fnmatch.fnmatch(srcdir, options['srcdir_pattern']):
            continue
        package.set_options_from_existing_package_srcdir(options, srcdir, set_all=True)
        if options.get('buildType') != 'afwmake':
            continue
        if not os.path.exists(options['srcdir_path'] + 'generate'):
            continue
        msg.info("Generating " + srcdir, empty_before=True)
        _call_generated_generate(options)                    
        
        msg.success('Generate ' + srcdir + ' successful')

    # Do root generate
    generate.root_generate(options)



#
#  Subcommand make-afw-package
#
def _subcommand_make_afw_package(args, options):
    msg.highlighted_info('Making skeleton Adaptive Framework package '+ options['afw_package_dir_path'] + '.', empty_before=True)
    if os.path.exists(options['afw_package_dir_path']):
        msg.error_exit(options['afw_package_dir_path'] + ' already exists')
    os.makedirs(options['afw_package_dir_path'], exist_ok=False)
    options['afw_package_dir_path'] = os.path.realpath(options['afw_package_dir_path']) + '/'

    # Write skeleton afw-package.json.
    ##@todo installPackageSubdir and homePageUrl should be something else
    msg.info('Writing afw-package.json')
    with nfc.open(options['afw_package_dir_path'] + 'afw-package.json', 'w') as fd:
        nfc.json_dump(
                    {
                        'afwPackageId': options['implementation_id'],
                        'brief':  options['brief'],
                        'description': options['description'],
                        'homePageUrl': 'https://github.com/afw-org',
                        'installPackageSubdir':'afw',
                        'srcdirs': {
                        },
                        'version': '0.0.0'
                    },
                    fd,
                    sort_keys=True,
                    indent=4)

    # Get options['afwdev_settings']
    _get_afwdev_settings(options)

    msg.info('Copying initial files to package')
    resources.copy_resources(options, 'closet/package/', todir=options['afw_package_dir_path'], substitution=True)

    msg.info('Initializing git repository')
    rc = subprocess.run(['git', 'init', '.'],cwd=options['afw_package_dir_path'])
    if rc.returncode != 0:
        msg.error_exit("git init . failed " + str(rc))

    # This method is required because older git installations do not have --initial-branch argument.
    msg.info('Making main default branch')
    rc = subprocess.run(['git', 'checkout', '-b', 'main'],cwd=options['afw_package_dir_path'])
    if rc.returncode != 0:
        msg.error_exit("git checkout -b main failed " + str(rc))

    # Write CMakeLists.txt
    options['afwPackageLabel'] = options['afwPackageId'].replace('-','_')
    resources.write_skeleton(options, 'root_CMakeLists.txt',
        options['afw_package_dir_path'] + 'CMakeLists.txt',
        substitution=True)

    # Do root generate.
    generate.root_generate(options)

    msg.success('Skeleton AFW package made in ' + options['afw_package_dir_path'] + '.')
    msg.success('Use other afwdev make-* subcommands and make changes as needed.')

#
#  Subcommand make-command
#
def _subcommand_make_command(args, options):
    msg.highlighted_info("Making command " + options['produces'], empty_before=True)
    package.add_package_srcdir_using_options(options, produces=options['produces'])
    options['added_files'] = []
    options['name'] = options['produces']
    options['description'] = 'Command ' + options['produces'] + '.'

    if options['verbose']:
        print("\nmake-command options:\n")
        print(nfc.json_dumps(options, sort_keys=True, indent=4))

    msg.info('Adding ' + options['srcdir_relpath'] + '/generate/ directory')
    options['added_files'].append(options['srcdir_relpath'] + '/generate/*')
    resources.copy_resources(options, 'closet/generate/', todir=options['srcdir_path'] + 'generate/')

    # Add skeleton command.c file.
    relfilepath = options['srcdir_relpath'] + options['produces'] + '.c'
    msg.info('Adding ' + relfilepath)
    options['added_files'].append(relfilepath)
    skeleton = resources.load_skeleton(options, 'command.c', substitution=True)
    with nfc.open(options['afw_package_dir_path'] + relfilepath, mode='w') as fd:
        fd.write(skeleton)

    # Write CMakeLists.txt
    msg.info("Adding " + options['srcdir_relpath'] + " CMakeLists.txt")
    resources.write_skeleton(options, 'src_CMakeLists.txt',
        options['srcdir_path'] + 'CMakeLists.txt',
        substitution=True)

    # Generate command and primary
    msg.info("Generating " + options['srcdir'])
    _call_generated_generate(options)

    # Success
    msg.success('Skeleton command '  + options['produces'] + ' made in ' + options['srcdir_relpath'] + '.')
    _msg_added_files(options)


#
#  Subcommand make-extension
#
def _subcommand_make_extension(args, options):

    options['added_files'] = []

    # Make extension
    _make_extension(options)

    # Generate primary extension just in case
    _generate_primary(options)

    # Success
    msg.success('Skeleton extension '  + options['produces'] + ' made in ' + options['srcdir_relpath'] + '.')
    msg.success('Use afwdev add-* subcommands and make changes as needed.')
    _msg_added_files(options)



#
#  Subcommand settings
#
def _subcommand_settings(args, options):
    options['subcommand'] = "settings"

    print(nfc.json_dumps(options['afwdev_settings'], indent=2, sort_keys=True))


#
#  Subcommand task
#
def _subcommand_task(args, options):
    options['subcommand'] = "task"

    # Get command string
    string =  options['afwdev_settings'].get('tasks',{}).get(options['task_name'])
    if not string:
        msg.error_exit("Task " + options['task_name'] + " doesn't exist")

    # Split string into one or more commands separated with '&&'
    commands = []
    command = []
    commands.append(command)
    for token in shlex.split(string):
        if token == '&&':
            command = []
            commands.append(command)
        else:
            command.append(token)

    # Run command(s)
    failed = False
    try:
        for command in commands:
            msg.info('Running ' + ' '.join(command))
            rc = subprocess.run(command)
            if rc.returncode != 0:
                failed = True
                break
    except:
        failed = True

    if failed:
        msg.error_exit("Task " + options['task_name'] + " failed" )


#
#  Subcommand test
#
def _subcommand_test(args, options):
    
    options['srcdir_pattern'] = options.get('srcdir_pattern', '\\*').replace('\\','')
    options['subcommand'] = "test"

    # call the test run entry point
    test.run(options)


#
#  Subcommand validate
#
def _subcommand_validate(args, options):

    options['subcommand'] = "validate"

    # Run validate.
    exit_code = validate.run(options)

    # Exit with the validate return code.
    sys.exit(exit_code) 


#
# Parser setup
#
def _add_args(subparsers, info):

    parser = subparsers.add_parser(
        info['subcommand'],
        help=info['help'],
        description=info['description'].format(**info))

    for arg in info['args']:

        # If 'arg' is not specified, skip.
        if arg.get('arg') is None:
            continue

        if arg.get('short'):
            args = [arg.get('short'), arg.get('arg')]
        else:
            args = [arg.get('arg')]

        # add_argument without or with dest.
        help = arg.get('help').format(**info)
        if arg.get('default') is not None:
            help += ' The default is "' + str(arg.get('default')).replace('{thing}', info['thing']) + '".'
        
        # This if/else is strange but supplying None for keywords to ignore still caused an error.
        if arg.get('const'):
            if arg.get('int', False):
                parser.add_argument(
                    *args,
                    type=int,
                    nargs=arg.get('nargs'),               
                    action=arg.get('action'),
                    default=arg.get('default'),
                    const=arg.get('const', None),              
                    help=help)
            else:
                parser.add_argument(
                    *args,
                    nargs=arg.get('nargs'),               
                    action=arg.get('action'),
                    default=arg.get('default'),
                    const=arg.get('const', None),              
                    help=help)

        elif arg.get('action'):
            parser.add_argument(
                *args,                
                action=arg.get('action'),
                default=arg.get('default'),          
                help=help)

        else:
            # default is handled later for non-actions so not specified here.
            parser.add_argument(
                *args,
                nargs=arg.get('nargs'),                  
                help=help)
           
    parser.set_defaults(func=globals().get('_subcommand_' + info['subcommand'].replace('-','_')))
    return parser


def _get_afwdev_settings(options):

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


#
# Set afw-package path and args options for a subcommand
#
def _set_package_and_args_options(args, options):

    vs = vars(args)

    # There must to be a global variable of the form '_info_' follow by subcommand with '-'s replaced by '_'.
    info = globals().get('_info_' + args.SUBCOMMAND.replace('-','_'))
    if info is None:
        print(args.SUBCOMMAND + ' is not converted yet')
        return

    # set the afwdev_info object
    options['afwdev_info'] = afwdev_info
    
    # If not newPackageDirPath, find afw-package.json in cwd or it's parent and set options['afw_package_dir_path']
    options['is_core_afw_package'] = False
    if not info.get('newPackageDirPath', False):
        options['afw_package_dir_path'] = os.getcwd()
        while options['afw_package_dir_path'] != '':
            if os.path.exists(options['afw_package_dir_path'] + '/afw-package.json'):
                options['afw_package_dir_path'] += '/'
                break
            if options['afw_package_dir_path'] == '/':
                options['afw_package_dir_path'] = ''
                break
            options['afw_package_dir_path'] = os.path.split(options['afw_package_dir_path'])[0]
        if options['afw_package_dir_path'] == '':
            msg.error_exit('The current working directory must be inside of an AFW package')

        # The core afw package will have 'src/afw/environment/afw_environment.c' as a fingerprint.
        package.set_is_core_afw_package(options)

        # Get options['afwdev_settings']
        _get_afwdev_settings(options)

    # Set 'subcommand_info' associated options.
    options['subcommand_info'] = info
    for arg in info['args']:
        v = None

        # If arg['arg'] is None, set v for option to default with optional substitution
        if arg.get('arg') is None:
            if arg.get('default') is None:
                msg.error_exit('default required if arg is not specified')
            v = arg.get('default').replace('{thing}', info['thing']).format(**options)

        # If this is an optional arg, look for v with arg name without '--'. Prompt if requested.
        elif arg['arg'].startswith('--'):
            v = vs.get(arg['arg'][2:].replace('-','_'))
            if v is None:
                v = arg.get('default')
                # Only str default are supported for prompt.
                if not isinstance(v, str):
                    continue
                if v is not None:
                    v = v.replace('{thing}', info['thing']).format(**options)            
                if (vs.get('prompt', False) or
                    (not vs.get('noprompt', False)
                    and not arg.get('noprompt', False))
                ):
                    i = input(
                        arg.get('help').replace('{thing}', info['thing']).format(**options) +
                        '\nEnter ' + arg.get('optionName') + ' or press <enter> to use default.\n' +
                        'Default is: '+  v + '\n> ')
                    if i != '': v = i

        # If this is an optional arg, look for v with arg name. If nargs is 1, v is v[0]
        else:                
            v = vs.get(arg['arg'])

        # If nargs=1 and v is list, make it single value
        if v is not None and isinstance(v, list) and arg.get('nargs', 0) == 1:
            v = v[0]

        # Set the associated options property (arg['optionName']) to v.
        if v is None:
            msg.warn(arg['optionName'] + ' is None')
        options[arg['optionName']] = v


#
# Parser setup
#
def _setup_parser(options):

    parser = argparse.ArgumentParser(
        formatter_class=argparse.RawTextHelpFormatter,
        **_afwdev_overall_info
    )

    #>>> Common arguments

    # --debug, --quiet, and --verbose
    verbosity_group = parser.add_mutually_exclusive_group()
    verbosity_group.add_argument("--debug",
        help=_info_debug['help'],
        action=_info_debug['action'])
    verbosity_group.add_argument("-q", "--quiet",
        help=_info_quiet['help'],
        action=_info_quiet['action'])
    verbosity_group.add_argument("--verbose",
        help=_info_verbose['help'],
        action=_info_verbose['action'])

    # version
    parser.add_argument("-v", "--version",
        help=_info_version['help'],
        action=_info_version['action'],
        version=_get_version(options))

    # version-string
    parser.add_argument("--version-string",
        help=_info_version_string['help'],
        action=_info_version_string['action'],
        version=_get_version_string(options))

    # noprompt
    parser.add_argument("--noprompt",
        help=_info_noprompt['help'],
        action=_info_noprompt['action'])

    # noprompt
    parser.add_argument("--prompt",
        help=_info_prompt['help'],
        action=_info_prompt['action'])

    # subcommands
    subparsers = parser.add_subparsers(
        title='subcommands',
        dest="SUBCOMMAND",
        help="Additional help",
        description="Valid subcommands")
    for info in _subcommand_infos:
        _add_args(subparsers, info)

    return parser
    #<<< End of parser setup


#
#  MAIN
#
def main():

    options = {}
  
    # Minimum python requirement
    MIN_PYTHON = (3, 6)
    if sys.version_info < MIN_PYTHON:
        print('afwdev requires Python %s.%s or newer.' % MIN_PYTHON)
        sys.exit(1)

    # Set options related to afwdev resources
    options['afwdev_path'] = os.path.realpath(sys.argv[0])
    options['afwdev_dir_path'] = os.path.dirname(options['afwdev_path']) + '/'
    options['afwdev_is_zipped'] = zipfile.is_zipfile(options['afwdev_path'])
    if not options['afwdev_is_zipped'] and not os.path.exists(options['afwdev_dir_path'] + '_resources'):
        print(options['afwdev_path'])
        print('afwdev can not find its resources')
        sys.exit(1)

    # parse args
    parser = _setup_parser(options)
    args = parser.parse_args()

    # Make sure subcommand specified
    if not 'func' in args:
        msg.error_exit("subcommand required")

    # Set verbosity
    options['verbose'] = args.verbose
    options['quiet'] = args.quiet
    options['debug'] = args.debug
    msg.set_verbosity(options)

    # Set afw-package path and args related options for the subcommand
    _set_package_and_args_options(args, options)

    # If debug mode, print options
    if msg.is_debug_mode():
        msg.debug("\nOptions before calling process function:")
        msg.debug(nfc.json_dumps(options, sort_keys=True, indent=4))

    # Call subcommand processing function
    args.func(args, options)

#
# If afwdev.py is being being called directly instead of main() being called
# from the afwdev zipped file, call main().
#
if __name__ == "__main__":
    main()
