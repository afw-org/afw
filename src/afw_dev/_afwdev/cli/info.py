#! /usr/bin/env python3
##
# @file info.py
# @ingroup afwdev_cli
# @brief Declarative CLI metadata (_info_* dicts) and afwdev_info export.
# @details Help text and optionNames here drive argparse and generated docs.
#          Each _subcommand_infos entry must have a matching handler in
#          registry.SUBCOMMAND_HANDLERS.
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

3) Add an adapter type with id "myadapter" to "myextension":

    cd ~/mypackage
    afwdev add-adapter-type myadapter --srcdir myextension

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


# subcommand add-adapter-type

_info_add_adapter_type = {
    "subcommand": "add-adapter-type",
    "help": "Add a new adapter type",
    "description":  \
        "This will add skeletons for the basic interfaces afw_adapter_factory, "
        "afw_adapter, and "
        "afw_adapter_session for a new adapter type to an existing srcdir. "
        "The implementation_id specified will be the adapter type. " 
        "If additional interfaces are needed, use subcommand add-core-interface.",
    "thing": "adapter type",
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

_info_build_all = {
    "optionName": "build_all",
    "arg": "--all",     
    "action": "store_true",    
    "default": False,
    "help": "This is short for the build type context switches --cmake, "
        "--docker, --docs, and --js. Does not enable --generate, --clean, "
        "--install, or --scan (see --fulldev)."
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
    "help": "C development shortcut: enables --generate, --clean, --install, "
        "and -j / parallel jobs (cmake context by default; not docs/JS/docker). "
        "Explicit -j N still overrides. See also --fulldev."
}

_info_build_fulldev = {
    "optionName": "build_fulldev",
    "arg": "--fulldev",
    "action": "store_true",
    "default": False,
    "help": "Full package dev-install shortcut: enables --all, --generate, "
        "--clean, --install, --scan, and -j / parallel jobs. Use when the "
        "whole tree (C, docs, JS, docker tags) should be rebuilt and "
        "installed. Explicit -j N still overrides."
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

_info_build_package = {
    "optionName": "build_package",
    "arg": "--package",     
    "action": "store_true",    
    "default": False,
    "help": "Build a distributable package based on the current distribution (.rpm/.deb/.tar)."
}


_info_build = {
    "subcommand": "build",
    "help": "Build afw package",
    "description":
"""This subcommand will build an AFW package using information from the
afw-package.json file.

The build switches --cmake, --docker, --docs, --js provide build type context
that other switches, such as --clean, --install, and --generate will execute
under. The --all selects all of those contexts (not generate/install).

Convenience profiles: --cdev (C day-to-day generate/clean/install/-j) and
--fulldev (all contexts plus generate/clean/install/scan/-j for a full dev install).
""",
    "thing": "build",
    "args": [
        _info_build_all,
        _info_build_cdev,
        _info_build_fulldev,
        _info_build_clean,
        _info_build_cmake,
        _info_build_docker,
        _info_build_docs,
        _info_build_generate,
        _info_build_install,
        _info_build_jobs,
        _info_build_js,
        _info_build_maxloop,
        _info_build_package,
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
    "help":
        "Only show failing tests (default). Use --show-all to include passes."
}

_info_test_show_all = {
    "optionName": "show_all",
    "arg": "--show-all",
    "action": "store_true",
    "default": False,
    "help": "Show all tests including passes (overrides default --errors)."
}

_info_test_list = {
    "optionName": "list",
    "arg": "--list",
    "action": "store_true",
    "default": False,
    "help": "List matching tests and exit without running them."
}

_info_test_tags = {
    "optionName": "test_tags",
    "arg": "--tags",    
    "action": "store",
    "default": ".*",
    "noprompt": True,
    "help":
        "Regex matched against each test group's Tags (from config.py). "
        "Default '.*' runs all groups. Groups without Tags are skipped when "
        "a non-default pattern is set."
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
    "help":
        "Where to write a results summary after the run. "
        "Default 'stdout' means do not write a summary file (human "
        "console only). Use a file path, or '-' to write the summary "
        "to stdout (useful with --output-format json for agents/CI)."
}

_info_test_output_format = {
    "optionName": "output_format",
    "arg": "--output-format",
    "action": "store",
    "default": "json",
    "noprompt": True,
    "help":
        "Format of --output summary when writing a file or '-'. "
        "json (default, indented), json-compact, or text. "
        "Does not change default human console reporting."
}

_info_test_pattern = {
    "optionName": "test-pattern",
    "arg": "--test-pattern",
    "default": ".*",
    "noprompt": True,
    "help": "Test filename pattern to match and run."
}

_info_test_js = {
    "optionName": "javascript",
    "arg": "--js",    
    "action": "store_true",
    "default": False,
    "noprompt": True,
    "help": "Run Javascript and Web App tests."
}

# Shared by test and blast: opt-in trees outside package src/*/tests
_info_tests_path = {
    "optionName": "tests_path",
    "arg": "--tests-path",
    "short": "-T",
    "action": "append",
    "noprompt": True,
    "help":
        "Directory of tests to use (repeatable). When any --tests-path is "
        "given, only those trees are searched (not package src/*/tests). "
        "Default test -j never scans these roots — put opt-in/regression "
        "experiments under e.g. src/afw/tests_special/. Same flag on "
        "afwdev blast for load thrash."
}

_info_test = {
    "subcommand": "test",
    "help": "Run tests",
    "description":
        "Run tests for one or more source directories. Default discovery is "
        "package src/*/tests (regression gate for test -j). Optional "
        "repeatable --tests-path/-T runs only those directory trees "
        "(e.g. tests_special/) and does not use package tests/.",
    "thing": "test",
    "args": [        
        _info_test_bail,
        _info_test_list,
        _info_test_tags,
        _info_test_errors,
        _info_test_show_all,
        _info_srcdir_pattern, 
        _info_tests_path,
        _info_test_watch,
        _info_test_jobs,
        _info_test_env_mode,
        _info_test_output,
        _info_test_output_format,
        _info_test_pattern,
        _info_test_js,
        _info_tmpdir
    ]
}

# subcommand blast (experimental on-demand afwfcgi firehose)

_info_blast_url = {
    "optionName": "url",
    "arg": "--url",
    "short": "-u",
    "noprompt": True,
    "help":
        "Attach mode: base URL of live AFW HTTP front door. "
        "Default when --conf is omitted: http://localhost:8080/afw "
        "(docker/dev nginx). Mutually exclusive with --conf."
}

_info_blast_conf = {
    "optionName": "conf",
    "arg": "--conf",
    "short": "-f",
    "noprompt": True,
    "help":
        "Managed mode: path to afw.conf; spawn installed afwfcgi "
        "(-f like afw/afwfcgi). Mutually exclusive with --url."
}

_info_blast_duration = {
    "optionName": "duration",
    "arg": "--duration",
    "short": "-d",
    "default": "5m",
    "noprompt": True,
    "help":
        "How long to blast (30s, 5m, 1h, or seconds). Default 5m. "
        "Use 0 with --max-requests only to disable the time limit."
}

_info_blast_max_requests = {
    "optionName": "max_requests",
    "arg": "--max-requests",
    "short": "-m",
    "int": True,
    "noprompt": True,
    "help": "Stop after approximately this many requests (optional)."
}

_info_blast_concurrency = {
    "optionName": "concurrency",
    "arg": "--concurrency",
    "short": "-c",
    "int": True,
    "default": "0",
    "noprompt": True,
    "help":
        "In-flight requests at once. Default 0 = 2×CPU count "
        "(classic gobench-style load)."
}

_info_blast_threads = {
    "optionName": "threads",
    "arg": "--threads",
    "short": "-n",
    "int": True,
    "default": "0",
    "noprompt": True,
    "help":
        "afwfcgi -n when using --conf. Default 0 = CPU count. "
        "Ignored for --url attach (use afwfcgi -n when you start it)."
}

_info_blast_request_timeout = {
    "optionName": "request_timeout",
    "arg": "--request-timeout",
    "default": "30",
    "noprompt": True,
    "help": "Per-request timeout in seconds (default 30)."
}

_info_blast_include_fixtures = {
    "optionName": "include_fixtures",
    "arg": "--include-fixtures",
    "action": "store_true",
    "default": False,
    "noprompt": True,
    "help":
        "Include tests that need private afw.conf, tests/environments "
        "(config.py Environment), or nearby conf. Default is to skip "
        "those so fail counts stay near zero unless something is wrong. "
        "Ignored when --tests-path is set (those roots are taken as-is)."
}

_info_blast_output = {
    "optionName": "output",
    "arg": "--output",
    "action": "store",
    "default": "stdout",
    "noprompt": True,
    "help":
        "Where to write a results summary after the run. Default 'stdout' "
        "means no summary file. Use a path or '-' (stdout) with "
        "--output-format. Same idea as afwdev test --output."
}

_info_blast = {
    "subcommand": "blast",
    "help": "On-demand load blast at afwfcgi (experimental)",
    "description":
        "*** Experimental *** Fire randomly chosen Adaptive test_script "
        "sources at afwfcgi for a period of time. Not part of "
        "'afwdev test -j'. Defaults target a typical docker/dev stack: "
        "attach to http://localhost:8080/afw for 5m with concurrency "
        "2×CPUs (and managed afwfcgi -n = CPUs) — plain 'afwdev blast' often "
        "suffices when nginx+afwfcgi are up. Override with -u/-f/-d/-c/-n. "
        "Corpus: package src/*/tests by default (--srcdir-pattern / "
        "--test-pattern / --tags), or only trees given with "
        "repeatable --tests-path/-T (exclusive; same flag as afwdev test). "
        "Fixture groups (Environment= / afw.conf) are skipped by default "
        "unless --include-fixtures (N/A with --tests-path). "
        "Optional --output / --output-format for a machine summary. "
        "Continues on Adaptive failures; stops if the server dies. "
        "See designs/afwdev-blast.md.",
    "thing": "blast",
    "args": [
        _info_blast_url,
        _info_blast_conf,
        _info_blast_duration,
        _info_blast_max_requests,
        _info_blast_concurrency,
        _info_blast_threads,
        _info_blast_request_timeout,
        _info_blast_include_fixtures,
        _info_tests_path,
        _info_blast_output,
        _info_test_output_format,
        _info_srcdir_pattern,
        _info_test_pattern,
        _info_test_tags,
    ]
}



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


# All subcommand infos (order here does not matter; --help sorts by name)
_subcommand_infos = [
    _info_add_adapter_type,
    _info_add_content_type,
    _info_add_core_interface,
    _info_add_log_type,
    _info_afwdev_parser_info,
    _info_blast,
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
    _info_validate,
]

#
# This object contains all of the help info and more for afwdev.
#
afwdev_info = {
    "overall": _afwdev_overall_info,
    "args_for_all_commands" : _args_for_all_commands_infos,
    "subcommands": sorted(
        _subcommand_infos, key=lambda i: i.get("subcommand") or ""),
}

