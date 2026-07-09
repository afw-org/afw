#! /usr/bin/env python3
##
# @file options.py
# @ingroup afwdev_cli
# @brief Documentation for the shared options dict passed through afwdev.
#
# The options dict is built in cli.parser.set_package_and_args_options and
# consumed by generate/build/test/validate/scaffold. Keys use snake_case via
# each _info_* entry's optionName (exceptions noted below).
#
# Common keys (always or usually present after package discovery):
#   afw_package_dir_path   Package root ending with /
#   afw_package            Parsed afw-package.json (via package.get_afw_package)
#   afwdev_settings        Merged afwdev-settings.json / workspace packages
#   afwdev_info            Full CLI metadata (afwdev_info from cli.info)
#   afwdev_path            Path to afwdev executable
#   afwdev_dir_path        Directory containing afwdev / _resources
#   afwdev_is_zipped       True when running from zipapp
#   is_core_afw_package    True if src/afw/environment/afw_environment.c exists
#   subcommand             Active subcommand name
#   subcommand_info        Matching _info_* dict for the subcommand
#   quiet / verbose / debug  Verbosity flags
#
# Srcdir-scoped keys (set by package.set_options_from_existing_package_srcdir
# or make-* scaffolding):
#   srcdir, srcdir_path, srcdir_relpath, prefix, buildType, produces, ...
#
# Test subcommand optionNames (see cli.info _info_test_*):
#   test_tags, test_jobs, test-pattern (hyphenated historical key),
#   errors, show_all, list, output, mode, watch, bail, javascript, tmpdir
#
# Prefer options.get('optionName') matching the _info_* declaration. When
# adding options, use snake_case optionName and update this list.
#

# Marker so importers can `from _afwdev.cli import options as options_doc`.
COMMON_OPTION_KEYS = (
    'afw_package_dir_path',
    'afw_package',
    'afwdev_settings',
    'afwdev_info',
    'srcdir',
    'srcdir_path',
    'srcdir_pattern',
    'subcommand',
)
